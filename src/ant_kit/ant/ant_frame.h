/*
 * ant_frame.h
 *
 * Created: 2012/7/11 10:43:42
 * Author: steven.zdwang
 */ 


#ifndef __ANT_FRAME__
#define __ANT_FRAME__

#include "ant_types.h"

#define MAX_CHANNEL_NUM                 1
#define COMMON_CHANNEL_NUM      MAX_CHANNEL_NUM  

typedef struct
{
   UCHAR ucMessageID;
   UCHAR* pucData;
} CYG_ANT_MESSAGE_T, *PCYG_ANT_MESSAGE_T;

typedef struct tagCYG_ANT_CHANNEL_T 
{     
    UCHAR ucChannel;
    UCHAR ucMessageID;
    UCHAR ucResponseID;
    UCHAR ucMessageCode;   
    BOOL  bWaitingForResponse;
    USHORT usTimeout;     
	struct timer tTimer; 
} CYG_ANT_CHANNEL_T, *PCYG_ANT_CHANNEL_T;                          

PCYG_ANT_MESSAGE_T 
cygANT_GetRxMsg(
void);

BOOL
cygANT_Init(
    UCHAR ucMaxChannelCount_);
    
void
cygANT_Deinit(
    void);    

BOOL
cygANT_UnAssignChannel(
    UCHAR ucChannelNumber_);

/*
Channel Type
Bidirectional Channels:
0x00 每 Receive Channel
0x10 - Transmit Channel 
Unidirectional Channels:
0x50 每 Transmit Only Channel
0x40 每 Receive Only Channel 
Shared Channels:
0x20 每 Shared Bidirectional Receive Channel
0x30 每 Shared Bidirectional Transmit Channe
*/
BOOL 
cygANT_AssignChannel(    
    UCHAR ucChannelNumber_, 
    UCHAR ucChannelType_, 
    UCHAR ucNetworkNumber_);
      
BOOL 
cygANT_SetChannelId(    
    UCHAR ucANTChannel_, 
    USHORT usDeviceNumber_, 
    UCHAR ucDeviceType_, 
    UCHAR ucTransmitType_);
      
BOOL 
cygANT_SetChannelPeriod(    
    UCHAR ucANTChannel_, 
    USHORT usPeriod_);
        	  
BOOL 
cygANT_SetChannelSearchTimeout(    
    UCHAR ucChannelNumber_, 
    UCHAR ucTimeout_);


BOOL 
cygANT_ChannelRFFreq(    
	UCHAR ucANTChannel_,
    UCHAR ucFreq_);
      
BOOL 
cygANT_SetNetworkKey(    
    UCHAR ucNetworkNumber_, 
    const UCHAR* pucKey_);
   
   
BOOL 
cygANT_SetTransmitPower(    
    UCHAR ucANTChannel_, 
    UCHAR ucPower_);
   
BOOL
cygANT_AddChannelID(    
    UCHAR ucANTChannel_,
    USHORT usDeviceNumber_,
    UCHAR ucDeviceTypeID_,
    UCHAR ucTransType_,
    UCHAR ucListIndex_);  
      
BOOL
cygANT_ConfigList(    
    UCHAR ucANTChannel_,
    UCHAR ucListSize_,
    UCHAR ucExclude_);    

BOOL
cygANT_SetChannelTxPower(    
    UCHAR ucANTChannel_,
    UCHAR ucTXPower_);
    
BOOL
cygANT_SetLowProrityChannelSearchTimeout(    
    UCHAR ucANTChannel_,
    UCHAR ucSearchTimeout_);
    
    
BOOL
cygANT_SetSerialNumChannelId(    
    UCHAR ucANTChannel_,
    UCHAR ucDeviceTypeID_,
    UCHAR ucTransType_);    
    
BOOL
cygANT_RxExtMesgsEnable(   
    UCHAR ucEnable_);
        
BOOL
cygANT_EnableLED(    
    UCHAR ucEnable_);    
    
BOOL
cygANT_CrystalEnable(    
    void);
    
BOOL
cygANT_LibConfig(    
    UCHAR ucLibConfig_);
    
BOOL
cygANT_ConfigFrequencyAgility(    
    UCHAR ucANTChannel_,
    UCHAR ucFreq1_,
    UCHAR ucFreq2_,
    UCHAR ucFreq3_);
    
BOOL
cygANT_SetProximitySearch(    
    UCHAR ucANTChannel_,
    UCHAR ucSearchThreshold_);
    
BOOL
cygANT_SetChannelSearchPriority(    
    UCHAR ucANTChannel_,
    UCHAR ucSearchPriority_);
    
BOOL
cygANT_ResetSystem(    
    void);
    
BOOL 
cygANT_OpenChannel(    
    UCHAR ucANTChannel_);
   
BOOL 
cygANT_CloseChannel(
    UCHAR ucANTChannel_);
   
   
BOOL
cygANT_OpenRxScanMode(    
    void);
    
BOOL 
cygANT_RequestMessage(   
    UCHAR ucANTChannel_, 
    UCHAR ucRequestedMessage_);
   
BOOL 
cygANT_SleepMessage(
    void); 
   
BOOL 
cygANT_SendBroadcastData(
    UCHAR ucANTChannel_, 
    UCHAR* pucBuffer_);
      
BOOL 
cygANT_SendAcknowledgedData(
    UCHAR ucChannel_, 
    UCHAR* pucData_, 
    USHORT usTimeout_);      
   
USHORT 
cygANT_SendBurstTransferPackage(
    UCHAR ucAntChannel_, 
    UCHAR* pucBuffer_, 
    USHORT usPackets_);
	
void 
cygANT_ProcessAntEvents(
    UCHAR* pucBuffer_);			  
	
void
cygANT_HandleTimerOneMS(
    void);
	
	
#define WAIT_RESPONSE_ERROR     -1	
#define WAIT_RESPONSE_OK        0	
#define WAIT_RESPONSE_TIMEOUT   1
#define WAIT_RESPONSE_WAITING   2	
int cygANT_WaitForResponse(CYG_ANT_CHANNEL_T *pContxt_);
void cygANT_ProcessAntEvents(UCHAR* pucBuffer_);
void cygANT_ProcessByte(UCHAR ucByte_);

#endif /* __ANT_FRAME__ */