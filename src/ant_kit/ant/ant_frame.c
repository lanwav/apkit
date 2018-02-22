/*
 * ant_frame.c
 *
 * Created: 2012/7/11 10:44:00
 * Copyright (C) 2012-2014 Jiaxing Lanwan Electronics co ., ltd .
 * Author: steven.zdwang
 */

#include "../cyg_config.h"

#if CYG_FEATURE_ANT

#include "ant_types.h"
#include "ant_frame.h"
#include "ant_messages.h"
#include "../pt.h"


#define CHANNEL_RESPOND_TIMEOUT         10      //50ms

//#define SERIAL_BYTE_SYNCHRONOUS
//#define SERIAL_BIT_SYNCHRONOUS
#define SERIAL_UART_ASYNC

#define CYG_ANT_CHANNEL_IDLE    0
#define CYG_ANT_CHANNEL_BUSY    1
#define CYG_ANT_CHANNEL_ERROR   3

static UCHAR cygANTMaxChannelCount = 0;
static UCHAR cygANTTxBuffer[MESG_MAX_SIZE];
static CYG_ANT_MESSAGE_T cygANTRxMsg;

#define  RX_FIFO_SIZE   MESG_MAX_SIZE + 10
static UCHAR aucRxFifo[RX_FIFO_SIZE];
static UCHAR ucRxIndex = 0;
static UCHAR ucRxSize = 0;
static UCHAR ucCheckSum = 0;
  
CYG_ANT_CHANNEL_T cygANTChannels[MAX_CHANNEL_NUM + 1];


static UCHAR* Serial_Get_Tx_Buffer(void)
{
    return (&cygANTTxBuffer[1]);
}

static void Serial_Put_Tx_Buffer(void)
{
    static UCHAR ucMesgLen;
    static UCHAR ucIndex;
    static UCHAR ucCheckSum;			

#if (defined __AVR_ATmega88A__)
	while( (PIND & _BV(PD2)) );//wait RTS is low
#endif	

    cygANTTxBuffer[0] = MESG_TX_SYNC;
    ucMesgLen = cygANTTxBuffer[1] + MESG_SAVED_FRAME_SIZE + 1;                          // read the message length of the message to send and adjust for frame size
    ucIndex   = 0;
    ucCheckSum = 0;

    DbgPrint("TX:");
    do
    {
		DbgPrint("%02X ", cygANTTxBuffer[ucIndex]);
        fputc( cygANTTxBuffer[ucIndex], &gfile_ant );                               // write out message byte
        ucCheckSum ^= cygANTTxBuffer[ucIndex];                                      // update the checksum

    }
    while (++ucIndex < ucMesgLen);                                                  // loop through message

    DbgPrint(" %02X\r\n", ucCheckSum);
    fputc( ucCheckSum, &gfile_ant );
}

static void Serial_Transaction(void)
{
    Serial_Put_Tx_Buffer();
}

static void SetWaitForCondition(CYG_ANT_CHANNEL_T *pContxt_, UCHAR ucChannel_, UCHAR ucResponseID_, UCHAR ucMessageID_)
{
    if( pContxt_ )
    {
		//cygActiveANTChannelNum = ucChannel_;
        pContxt_->ucMessageID = ucMessageID_;
        pContxt_->ucResponseID = ucResponseID_;
        pContxt_->ucChannel = ucChannel_;
        pContxt_->bWaitingForResponse = TRUE;

        pContxt_->usTimeout = CHANNEL_RESPOND_TIMEOUT;    // Start the command timeout timer

        //DbgPrint("Wait Msg:%X, %X, %X, %d\r\n", ucMessageID_, ucResponseID_, ucChannel_, CHANNEL_RESPOND_TIMEOUT);		
		timer_set(&(pContxt_->tTimer), pContxt_->usTimeout);
    }
}

int cygANT_WaitForResponse(CYG_ANT_CHANNEL_T *pContxt_)
{
    if( !pContxt_ ) return WAIT_RESPONSE_ERROR;

    if( pContxt_->bWaitingForResponse == FALSE)
    {
		DbgPrint("%X, %X, OK\r\n", pContxt_->ucMessageCode, pContxt_->ucMessageID);
        return WAIT_RESPONSE_OK;
    }
	
	if( timer_expired(&(pContxt_->tTimer)) )
	{
		//DbgPrint("time out\r\n");
		return WAIT_RESPONSE_TIMEOUT;
	}
		
	return WAIT_RESPONSE_WAITING;
}

BOOL cygANT_UnAssignChannel( UCHAR ucChannel_ )
{
    BOOL bSuccess = FALSE;

    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_UNASSIGN_CHANNEL_SIZE;
        pucBuffer[1] = MESG_UNASSIGN_CHANNEL_ID;
        pucBuffer[2] = ucChannel_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_UNASSIGN_CHANNEL_ID);
        Serial_Put_Tx_Buffer();                            // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_AssignChannel( UCHAR ucChannel_, UCHAR ucChannelType_, UCHAR ucNetworkNumber_ )
{
    BOOL bSuccess = FALSE;

    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_ASSIGN_CHANNEL_SIZE;
        pucBuffer[1] = MESG_ASSIGN_CHANNEL_ID;
        pucBuffer[2] = ucChannel_;
        pucBuffer[3] = ucChannelType_;
        pucBuffer[4] = ucNetworkNumber_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_ASSIGN_CHANNEL_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetChannelId( UCHAR ucChannel_, USHORT usDeviceNumber_, UCHAR ucDeviceType_, UCHAR ucTransmitType_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_CHANNEL_ID_SIZE;
        pucBuffer[1] = MESG_CHANNEL_ID_ID;
        pucBuffer[2] = ucChannel_;
        pucBuffer[3] = LOW_BYTE(usDeviceNumber_);
        pucBuffer[4] = HIGH_BYTE(usDeviceNumber_);
        pucBuffer[5] = ucDeviceType_;
        pucBuffer[6] = ucTransmitType_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_CHANNEL_ID_ID );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetChannelPeriod( UCHAR ucChannel_, USHORT usPeriod_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_CHANNEL_MESG_PERIOD_SIZE;
        pucBuffer[1] = MESG_CHANNEL_MESG_PERIOD_ID;
        pucBuffer[2] = ucChannel_;
        pucBuffer[3] = LOW_BYTE(usPeriod_);
        pucBuffer[4] = HIGH_BYTE(usPeriod_);

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_CHANNEL_MESG_PERIOD_ID );

        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetChannelSearchTimeout( UCHAR ucChannel_, UCHAR ucTimeout_ )
{
    BOOL bSuccess = FALSE;

    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_CHANNEL_SEARCH_TIMEOUT_SIZE;
        pucBuffer[1] = MESG_CHANNEL_SEARCH_TIMEOUT_ID;
        pucBuffer[2] = ucChannel_;
        pucBuffer[3] = ucTimeout_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_CHANNEL_SEARCH_TIMEOUT_ID );

        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_ChannelRFFreq( UCHAR ucChannel_, UCHAR ucFreq_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_CHANNEL_RADIO_FREQ_SIZE;
        pucBuffer[1] = MESG_CHANNEL_RADIO_FREQ_ID;
        pucBuffer[2] = ucChannel_;
        pucBuffer[3] = ucFreq_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_CHANNEL_RADIO_FREQ_ID );
		
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetNetworkKey( UCHAR ucNetworkNumber_, const UCHAR* pucKey_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_NETWORK_KEY_SIZE;
        pucBuffer[1] = MESG_NETWORK_KEY_ID;
        pucBuffer[2] = ucNetworkNumber_;
        pucBuffer[3] = pucKey_[0];
        pucBuffer[4] = pucKey_[1];
        pucBuffer[5] = pucKey_[2];
        pucBuffer[6] = pucKey_[3];
        pucBuffer[7] = pucKey_[4];
        pucBuffer[8] = pucKey_[5];
        pucBuffer[9] = pucKey_[6];
        pucBuffer[10] = pucKey_[7];

        SetWaitForCondition(&cygANTChannels[0], 0, MESG_RESPONSE_EVENT_ID, MESG_NETWORK_KEY_ID );

        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetTransmitPower( UCHAR ucChannel_, UCHAR ucPower_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_RADIO_TX_POWER_SIZE;
        pucBuffer[1] = MESG_RADIO_TX_POWER_ID;
        pucBuffer[2] = ucChannel_;
        pucBuffer[3] = ucPower_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_RADIO_TX_POWER_ID );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_AddChannelID( UCHAR ucChannel_, USHORT usDeviceNumber_, UCHAR ucDeviceTypeID_, UCHAR ucTransType_, UCHAR ucListIndex_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_ADD_CHANNEL_SIZE;
        pucBuffer[1] = MESG_ADD_CHANNEL_ID;
        pucBuffer[2] = LOW_BYTE(usDeviceNumber_);
        pucBuffer[3] = HIGH_BYTE(usDeviceNumber_);
        pucBuffer[4] = ucDeviceTypeID_;
        pucBuffer[5] = ucTransType_;
        pucBuffer[6] = ucListIndex_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_ADD_CHANNEL_ID );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_ConfigList( UCHAR ucChannel_, UCHAR ucListSize_, UCHAR ucExclude_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_CONFIG_LIST_SIZE;
        pucBuffer[1] = MESG_CONFIG_LIST_ID;
        pucBuffer[2] = ucListSize_;
        pucBuffer[3] = ucExclude_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_CONFIG_LIST_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetChannelTxPower( UCHAR ucChannel_, UCHAR ucTXPower_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_SET_CHANNEL_TX_POWER_SIZE;
        pucBuffer[1] = MESG_SET_CHANNEL_TX_POWER_ID;
        pucBuffer[2] = ucTXPower_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_SET_CHANNEL_TX_POWER_ID );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetLowProrityChannelSearchTimeout( UCHAR ucChannel_, UCHAR ucSearchTimeout_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_SET_LOW_PRIORITY_CHANNEL_SEARCH_TIMEOUT_SIZE;
        pucBuffer[1] = MESG_SET_LOW_PRIORITY_CHANNEL_SEARCH_TIMEOUT_ID;
        pucBuffer[2] = ucSearchTimeout_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_SET_LOW_PRIORITY_CHANNEL_SEARCH_TIMEOUT_ID );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetSerialNumChannelId( UCHAR ucChannel_, UCHAR ucDeviceTypeID_, UCHAR ucTransType_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_SET_SERIAL_NUM_CHANNEL_ID_SIZE;
        pucBuffer[1] = MESG_SET_SERIAL_NUM_CHANNEL_ID_ID;
        pucBuffer[2] = ucDeviceTypeID_;
        pucBuffer[3] = ucTransType_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_SET_SERIAL_NUM_CHANNEL_ID_ID );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_RxExtMesgsEnable( UCHAR ucEnable_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_RX_EXT_MESGS_ENABLE_SIZE;
        pucBuffer[1] = MESG_RX_EXT_MESGS_ENABLE_ID;
        pucBuffer[2] = ucEnable_;

        SetWaitForCondition(&cygANTChannels[COMMON_CHANNEL_NUM], COMMON_CHANNEL_NUM, MESG_RESPONSE_EVENT_ID, MESG_RX_EXT_MESGS_ENABLE_ID );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_EnableLED( UCHAR ucEnable_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_ENABLE_LED_SIZE;
        pucBuffer[1] = MESG_ENABLE_LED_ID;
        pucBuffer[2] = ucEnable_;

        SetWaitForCondition(&cygANTChannels[COMMON_CHANNEL_NUM], COMMON_CHANNEL_NUM, MESG_RESPONSE_EVENT_ID, MESG_ENABLE_LED_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_CrystalEnable( void )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_CRYSTAL_ENABLE_SIZE;
        pucBuffer[1] = MESG_CRYSTAL_ENABLE_ID;
        pucBuffer[2] = 0;

        SetWaitForCondition(&cygANTChannels[COMMON_CHANNEL_NUM], COMMON_CHANNEL_NUM, MESG_RESPONSE_EVENT_ID, MESG_CRYSTAL_ENABLE_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_LibConfig( UCHAR ucLibConfig_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_LIB_CONFIG_SIZE;
        pucBuffer[1] = MESG_LIB_CONFIG_ID;
        pucBuffer[2] = ucLibConfig_;

        SetWaitForCondition(&cygANTChannels[COMMON_CHANNEL_NUM], COMMON_CHANNEL_NUM, MESG_RESPONSE_EVENT_ID, MESG_LIB_CONFIG_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_ConfigFrequencyAgility( UCHAR ucChannel_, UCHAR ucFreq1_, UCHAR ucFreq2_, UCHAR ucFreq3_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_CONFIG_FREQUENCY_AGILITY_SIZE;
        pucBuffer[1] = MESG_CONFIG_FREQUENCY_AGILITY_ID;
        pucBuffer[2] = ucFreq1_;
        pucBuffer[3] = ucFreq2_;
        pucBuffer[4] = ucFreq3_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_CONFIG_FREQUENCY_AGILITY_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SetProximitySearch( UCHAR ucChannel_, UCHAR ucSearchThreshold_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_SET_PROXIMITY_SEARCH_SIZE;
        pucBuffer[1] = MESG_SET_PROXIMITY_SEARCH_ID;
        pucBuffer[2] = ucSearchThreshold_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_SET_PROXIMITY_SEARCH_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }


    return(bSuccess);
}

BOOL cygANT_SetChannelSearchPriority( UCHAR ucChannel_, UCHAR ucSearchPriority_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_SET_CHANNEL_SEARCH_PRIORITY_SIZE;
        pucBuffer[1] = MESG_SET_CHANNEL_SEARCH_PRIORITY_ID;
        pucBuffer[2] = ucSearchPriority_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_SET_CHANNEL_SEARCH_PRIORITY_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }


    return(bSuccess);
}

BOOL cygANT_ResetSystem( void )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_SYSTEM_RESET_SIZE;
        pucBuffer[1] = MESG_SYSTEM_RESET_ID;
        pucBuffer[2] = 0;
            
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_OpenChannel( UCHAR ucChannel_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_OPEN_CHANNEL_SIZE;
        pucBuffer[1] = MESG_OPEN_CHANNEL_ID;
        pucBuffer[2] = ucChannel_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_OPEN_CHANNEL_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_CloseChannel( UCHAR ucChannel_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_CLOSE_CHANNEL_SIZE;
        pucBuffer[1] = MESG_CLOSE_CHANNEL_ID;
        pucBuffer[2] = ucChannel_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_CLOSE_CHANNEL_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_OpenRxScanMode( void )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_OPEN_RX_SCAN_MODE_SIZE;
        pucBuffer[1] = MESG_OPEN_RX_SCAN_MODE_ID;
        pucBuffer[2] = 0;

        SetWaitForCondition(&cygANTChannels[COMMON_CHANNEL_NUM], COMMON_CHANNEL_NUM, MESG_RESPONSE_EVENT_ID, MESG_OPEN_RX_SCAN_MODE_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_RequestMessage( UCHAR ucChannel_, UCHAR ucRequestedMessage_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_REQUEST_SIZE;
        pucBuffer[1] = MESG_REQUEST_ID;
        pucBuffer[2] = ucRequestedMessage_;

        SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_REQUEST_ID);
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SleepMessage( void )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_SLEEP_MESSAGE_SIZE;
        pucBuffer[1] = MESG_SLEEP_MESSAGE_ID;
        pucBuffer[2] = 0;

        SetWaitForCondition(&cygANTChannels[COMMON_CHANNEL_NUM], COMMON_CHANNEL_NUM, MESG_RESPONSE_EVENT_ID, MESG_CLOSE_CHANNEL_ID );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SendBroadcastData( UCHAR ucChannel_, UCHAR* pucBuffer_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue


    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_DATA_SIZE;
        pucBuffer[1] = MESG_BROADCAST_DATA_ID;
		pucBuffer[2] = ucChannel_;
        memcpy( (void*)&pucBuffer[3], (void*)pucBuffer_, 8 );
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

BOOL cygANT_SendAcknowledgedData( UCHAR ucChannel_, UCHAR* pucData_, USHORT usTimeout_ )
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue


    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_DATA_SIZE;
        pucBuffer[1] = MESG_ACKNOWLEDGED_DATA_ID;
		pucBuffer[2] = ucChannel_;
        memcpy( (void*)&pucBuffer[3], (void*)pucData_, 8 );

        if( usTimeout_ > 0 )
        {
            cygANTChannels[ucChannel_].usTimeout = usTimeout_;
            SetWaitForCondition(&cygANTChannels[ucChannel_], ucChannel_, MESG_RESPONSE_EVENT_ID, MESG_EVENT_ID );
        }

        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }




    return(bSuccess);
}

static BOOL cygANT_BurstPacket(UCHAR ucControl_, UCHAR* pucBuffer_)
{
    BOOL bSuccess = FALSE;
    UCHAR* pucBuffer = Serial_Get_Tx_Buffer();            // Get space from the queue

    if(pucBuffer)                                         // If there is space in the queue
    {
        pucBuffer[0] = MESG_DATA_SIZE;
        pucBuffer[1] = MESG_BURST_DATA_ID;
        pucBuffer[2] = ucControl_;
        pucBuffer[3] = pucBuffer_[0];
        pucBuffer[4] = pucBuffer_[1];
        pucBuffer[5] = pucBuffer_[2];
        pucBuffer[6] = pucBuffer_[3];
        pucBuffer[7] = pucBuffer_[4];
        pucBuffer[8] = pucBuffer_[5];
        pucBuffer[9] = pucBuffer_[6];
        pucBuffer[10] = pucBuffer_[7];
        Serial_Put_Tx_Buffer();                         // Put buffer back in queue
        bSuccess = TRUE;
    }

    return(bSuccess);
}

USHORT cygANT_SendBurstTransferPackage( UCHAR ucAntChannel_, UCHAR* pucBuffer_, USHORT usPackets_ )
{
    UCHAR ucSeq = 0;

    do
    {
        if (usPackets_ == 1)
            ucSeq |= SEQUENCE_LAST_MESSAGE;


        if(cygANT_BurstPacket(ucAntChannel_ | ucSeq, pucBuffer_))
        {

            //Move to next 8 byte block in data
            pucBuffer_ += MESG_DATA_SIZE - 1;  //Use size - 1 since the channel data is not in the data buffer

            //Adjust sequence number
            if (ucSeq == SEQUENCE_NUMBER_ROLLOVER)
                ucSeq = SEQUENCE_NUMBER_INC;
            else
                ucSeq += SEQUENCE_NUMBER_INC;
        }

        //TODO: Transmit and receive messages to/from ANT
        Serial_Transaction();
    }
    while(--usPackets_);

    return usPackets_;
}

PCYG_ANT_MESSAGE_T cygANT_GetRxATNMsg(void)
{
	return &cygANTRxMsg;
}

BOOL cygANT_Init( UCHAR ucMaxChannelCount_ )
{
    cygANTMaxChannelCount = ucMaxChannelCount_;
    return TRUE;
}

void cygANT_Deinit( void )
{

}

void cygANT_ProcessMessage(UCHAR* pucBuffer_)
{
    if( pucBuffer_ == NULL ) return;

    pucBuffer_++;		
			
    UCHAR ucMessageId = pucBuffer_[BUFFER_INDEX_MESG_ID];
    UCHAR ucChannel = 0;	
	
	//Save RxMsg and another thread will handle ACKNOWLEDGED or BROADCAST
	cygANTRxMsg.ucMessageID = ucMessageId;
	cygANTRxMsg.pucData = pucBuffer_;

    switch(ucMessageId)
    {
    case MESG_RESPONSE_EVENT_ID:
    {		
        ucChannel = pucBuffer_[BUFFER_INDEX_CHANNEL_NUM];		
        cygANTChannels[ucChannel].bWaitingForResponse = !( \
                cygANTChannels[ucChannel].ucMessageID == pucBuffer_[BUFFER_INDEX_RESPONSE_MESG_ID]  && \
                cygANTChannels[ucChannel].ucResponseID == MESG_RESPONSE_EVENT_ID);
				
		//DbgPrint("[%X, %X, %X, %X]\r\n", cygANTChannels[ucChannel].ucMessageID, pucBuffer_[BUFFER_INDEX_RESPONSE_MESG_ID], cygANTChannels[ucChannel].ucResponseID, MESG_RESPONSE_EVENT_ID);		

        goto _handle_response;
    }
    break;
    case MESG_ACKNOWLEDGED_DATA_ID:
    {
        ucChannel = pucBuffer_[BUFFER_INDEX_CHANNEL_NUM];		
    }
    break;
    case MESG_BROADCAST_DATA_ID:
    {
        ucChannel = pucBuffer_[BUFFER_INDEX_CHANNEL_NUM];
    }
    break;
    default:
        break;
    }


    return;

_handle_response:    

	//DbgPrint("MSG: %X, %X, %X\r\n", ucMessageId, ucChannel, cygANTChannels[ucChannel].bWaitingForResponse);
	
    if( !cygANTChannels[ucChannel].bWaitingForResponse )
    {
        switch(pucBuffer_[4])
        {
        case EVENT_TRANSFER_TX_COMPLETED:
        case EVENT_TRANSFER_TX_FAILED:
        case EVENT_CHANNEL_CLOSED:
        case CHANNEL_NOT_OPENED:
        case TRANSFER_IN_PROGRESS:
        case CHANNEL_IN_WRONG_STATE:
        {
			DbgPrint("Channel:%d\r\n", ucChannel);
            cygANTChannels[ucChannel].bWaitingForResponse = FALSE;
            break;
        }
        default:
            break;
        }
    }			

}

void cygANT_ProcessByte( UCHAR ucByte_ )
{
    if (ucRxIndex == 0)                                      // If we are looking for the start of a message.
    {
        if (ucByte_ == MESG_TX_SYNC)                          // If it is a valid first byte.
        {
            aucRxFifo[ucRxIndex++] = ucByte_;                  // Save it.
            ucCheckSum = ucByte_;                              // Initialize the checksum.
            ucRxSize = 2;                                      // We have to init high so we can read enough bytes to determine real length
        }
    }
    else if (ucRxIndex == 1)                                 // Determine RX message size.
    {
        aucRxFifo[ucRxIndex++] = ucByte_;                     // Save it.
        ucRxSize = ucByte_ + (MESG_FRAME_SIZE - MESG_SYNC_SIZE);  // We just got the length.
        ucCheckSum ^= ucByte_;                                // Calculate checksum.

        if (ucRxSize > RX_FIFO_SIZE)                          // If our buffer can't handle this message, turf it.
        {
            if (ucByte_ == MESG_TX_SYNC)
            {
                aucRxFifo[0] = ucByte_;                         // Save the byte.
                ucCheckSum = ucByte_;                           // Initialize the checksum.
                ucRxSize = 2;                                   // We have to init high so we can read enough bytes to determine real length
                ucRxIndex = 1;                                  // Set the Rx Index for the next iteration.
            }
            else
            {
                ucRxIndex = 0;                                  // Invalid size, so restart.
            }

        }
    }
    else
    {
        aucRxFifo[ucRxIndex] = ucByte_;                       // Save the byte.
        ucCheckSum ^= ucByte_;                                // Calculate checksum.

        if (ucRxIndex >= ucRxSize)                            // If we have received the whole message.
        {
            if (ucCheckSum == 0)                               // The CRC passed.
            {
                cygANT_ProcessMessage(aucRxFifo);                               // Process the ANT message.
            }
            else
            {
                //TODO: Steven need fix CRC error!
                // Set a serial error for the bad crc.
            }
            ucRxIndex = 0;                                     // Reset the index.
        }
        else
        {
            ucRxIndex++;
        }
    }
}

#endif //CYG_FEATURE_ANT