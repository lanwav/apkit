
#ifndef __ANT_MESSAGES_H__
#define __ANT_MESSAGES_H__

//////////////////////////////////////////////
// ANT Clock Definition
//////////////////////////////////////////////
#define ANT_CLOCK_FREQUENCY                        ((ULONG)32768)          ///< ANT system clock frequency

//////////////////////////////////////////////
// Radio TX Power Definitions
//////////////////////////////////////////////
#define RADIO_TX_POWER_MASK                        ((UCHAR)0x03)
#define RADIO_TX_POWER_MINUS20DB                   ((UCHAR)0x00)
#define RADIO_TX_POWER_MINUS10DB                   ((UCHAR)0x01)
#define RADIO_TX_POWER_MINUS5DB                    ((UCHAR)0x02)
#define RADIO_TX_POWER_0DB                         ((UCHAR)0x03)

//////////////////////////////////////////////
// Default System Definitions
//////////////////////////////////////////////
#define DEFAULT_CHANNEL_MESSAGE_FREQUENCY          ((ULONG)4)
#define DEFAULT_CHANNEL_MESSAGE_PERIOD             ((USHORT)( ANT_CLOCK_FREQUENCY / DEFAULT_CHANNEL_MESSAGE_FREQUENCY )) ///< 8192 (4Hz)
#define DEFAULT_RADIO_TX_POWER                     RADIO_TX_POWER_0DB      ///< ANT default RF power
#define DEFAULT_RADIO_CHANNEL                      ((UCHAR)66)             ///< 2400MHz + 1MHz * Channel Number = 2466MHz
#define DEFAULT_RX_SEARCH_TIMEOUT                  ((UCHAR)12)             ///< 12 * 2.5 seconds = 30 seconds

//////////////////////////////////////////////
// ID Definitions
//////////////////////////////////////////////
#define ID_MANUFACTURER_OFFSET                     ((UCHAR)3)
#define ID_DEVICE_TYPE_OFFSET                      ((UCHAR)2)
#define ID_DEVICE_NUMBER_HIGH_OFFSET               ((UCHAR)1)
#define ID_DEVICE_NUMBER_LOW_OFFSET                ((UCHAR)0)
#define ID_DEVICE_TYPE_PAIRING_FLAG                ((UCHAR)0x80)

//////////////////////////////////////////////
// Assign Channel Parameters
//////////////////////////////////////////////
#define PARAMETER_TX_NOT_RX                        ((UCHAR)0x10)
#define PARAMETER_MULTIPLE_ACCESS_CHANNEL          ((UCHAR)0x20)  
#define PARAMETER_NO_TX_GUARD_BAND                 ((UCHAR)0x40)   
#define PARAMETER_ALWAYS_RX_WILD_CARD_SEARCH_ID    ((UCHAR)0x40)

//////////////////////////////////////////////
// Assign Channel Types
//////////////////////////////////////////////
#define CHANNEL_TYPE_SLAVE                         ((UCHAR) 0x00)
#define CHANNEL_TYPE_MASTER                        ((UCHAR) 0x10)
#define CHANNEL_TYPE_MASTER_TX_ONLY                ((UCHAR) 0x50)
#define CHANNEL_TYPE_SHARED_SLAVE                  ((UCHAR) 0x20)
#define CHANNEL_TYPE_SHARED_MASTER                 ((UCHAR) 0x30)

//////////////////////////////////////////////
// Channel Status
//////////////////////////////////////////////
#define STATUS_UNASSIGNED_CHANNEL                  ((UCHAR)0x00)
#define STATUS_ASSIGNED_CHANNEL                    ((UCHAR)0x01)
#define STATUS_SEARCHING_CHANNEL                   ((UCHAR)0x02)
#define STATUS_TRACKING_CHANNEL                    ((UCHAR)0x03)
#define STATUS_OVERRUN                             ((UCHAR)0x40)
#define STATUS_UNDERRUN                            ((UCHAR)0x80)

//////////////////////////////////////////////
// Standard capabilities defines
//////////////////////////////////////////////
#define CAPABILITIES_NO_RX_CHANNELS                ((UCHAR)0x01)
#define CAPABILITIES_NO_TX_CHANNELS                ((UCHAR)0x02)
#define CAPABILITIES_NO_RX_MESSAGES                ((UCHAR)0x04)
#define CAPABILITIES_NO_TX_MESSAGES                ((UCHAR)0x08)
#define CAPABILITIES_NO_ACKD_MESSAGES              ((UCHAR)0x10)
#define CAPABILITIES_NO_BURST_TRANSFER             ((UCHAR)0x20)

//////////////////////////////////////////////
// Advanced capabilities defines
//////////////////////////////////////////////
#define CAPABILITIES_DETECT_OVERRUN_UNDERRUN       ((UCHAR)0x01)
#define CAPABILITIES_NETWORK_ENABLED               ((UCHAR)0x02)

//////////////////////////////////////////////
// Burst Message Sequence 
//////////////////////////////////////////////
#define CHANNEL_NUMBER_MASK                        ((UCHAR)0x1F)
#define SEQUENCE_NUMBER_MASK                       ((UCHAR)0xE0)
#define SEQUENCE_NUMBER_INC                        ((UCHAR)0x20)
#define SEQUENCE_NUMBER_ROLLOVER                   ((UCHAR)0x60)
#define SEQUENCE_LAST_MESSAGE                      ((UCHAR)0x80)

//////////////////////////////////////////////
// Shared Channel Commands / Datatypes
//////////////////////////////////////////////
#define SHARED_CMD_SLOT_AVALIBLE                   ((UCHAR)0xFF)
#define SHARED_CMD_BUSY_ACQUIRING                  ((UCHAR)0xFE)
#define SHARED_CMD_COMMAND_REQUEST_TO_ACQUIRE      ((UCHAR)0xFD)
#define SHARED_CMD_CONFIRM_ACQUIRED                ((UCHAR)0xFC)
#define SHARED_CMD_NO_SLOTS_AVAILABLE              ((UCHAR)0xFB)
//...
#define SHARED_TYPE_RELAY                          ((UCHAR)0x43)           
#define SHARED_TYPE_COUNTER                        ((UCHAR)0x42)
#define SHARED_TYPE_A_TO_D                         ((UCHAR)0x41)
#define SHARED_TYPE_DIGITAL                        ((UCHAR)0x40)           
#define SHARED_TYPE_UNDEFINED                      ((UCHAR)0x00)

///////////////////////////////////////////////////////////////////////
// AtoD SubTypes
///////////////////////////////////////////////////////////////////////
#define TEMPERATURE                                ((UCHAR)0xFE)
#define BATT_VOLTAGE                               ((UCHAR)0xFF)

//////////////////////////////////////////////
// Response / Event Codes
//////////////////////////////////////////////
#define RESPONSE_NO_ERROR                          ((UCHAR)0x00)             

#define EVENT_RX_SEARCH_TIMEOUT                    ((UCHAR)0x01)             
#define EVENT_RX_FAIL                              ((UCHAR)0x02)             
#define EVENT_TX                                   ((UCHAR)0x03)             
#define EVENT_TRANSFER_RX_FAILED                   ((UCHAR)0x04)             
#define EVENT_TRANSFER_TX_COMPLETED                ((UCHAR)0x05)             
#define EVENT_TRANSFER_TX_FAILED                   ((UCHAR)0x06)             
#define EVENT_CHANNEL_CLOSED                       ((UCHAR)0x07)
#define EVENT_RX_FAIL_GO_TO_SEARCH                 ((UCHAR)0x08)
#define EVENT_CHANNEL_COLLISION                    ((UCHAR)0x09)


#define CHANNEL_IN_WRONG_STATE                     ((UCHAR)0x15)           // returned on attempt to perform an action from the wrong channel state
#define CHANNEL_NOT_OPENED                         ((UCHAR)0x16)           // returned on attempt to communicate on a channel that is not open
#define CHANNEL_ID_NOT_SET                         ((UCHAR)0x18)           // returned on attempt to open a channel without setting the channel ID

#define TRANSFER_IN_PROGRESS                       ((UCHAR)0x1F)           // returned on attempt to communicate on a channel with a TX transfer in progress
#define TRANSFER_SEQUENCE_NUMBER_ERROR             ((UCHAR)0x20)           // returned when sequence number is out of order on a Burst transfer

#define INVALID_MESSAGE                            ((UCHAR)0x28)           // returned when the message has an invalid parameter
#define INVALID_NETWORK_NUMBER                     ((UCHAR)0x29)           // returned when an invalid network number is provided
#define NO_RESPONSE_MESSAGE                        ((UCHAR)0x50)           // returned to the Command_SerialMessageProcess function, so no reply message is generated

#define FIT_ACTIVE_SEARCH_TIMEOUT                  ((UCHAR)0x60)           // event added for timeout of the pairing state after the Fit module becomes active
#define FIT_WATCH_PAIRED                           ((UCHAR)0x61)           // event added for timeout of the pairing state after the Fit module becomes active
#define FIT_WATCH_UNPAIRED                         ((UCHAR)0x62)           // event added for timeout of the pairing state after the Fit module becomes active


#define EVENT_COMMAND_TIMEOUT                      ((UCHAR)0xA9)           // (Host Only)returned when no response is recieved from ANT module after a command has been sent
#define EVENT_ACK_TIMEOUT                          ((UCHAR)0xAA)           // (Host Only) returned if not response to an Ack command is recieved for a set period of time.



///////////////////////////////////////////////////////////////
// Application Level defines
///////////////////////////////////////////////////////////////
#define USE_FREQUENCY_HOPPER                       
// #define USE_AUTO_SHARED_MASTER                     

#define NUM_CHANNELS                               ((UCHAR)0x04)           // Number of channels to be initialized and used by the application
#define NUM_FREQUENCY_HOPS                         ((UCHAR)0x04)           // Number of frequency hops to make if using frequency hopper

///////////////////////////////////////////////////////////////
// +LINK Mode Commands
///////////////////////////////////////////////////////////////
#define PLUS_LINK_MSG_STATUS         ((UCHAR)0x01)
#define PLUS_LINK_MSG_CONNECT        ((UCHAR)0x02)
#define PLUS_LINK_MSG_DISCONNECT     ((UCHAR)0x03)
#define PLUS_LINK_MSG_AUTHENTICATE   ((UCHAR)0x04)
#define PLUS_LINK_MSG_PAIR           ((UCHAR)0x05)
#define PLUS_LINK_MSG_DOWNLOAD       ((UCHAR)0x06)
#define PLUS_LINK_MSG_UPLOAD         ((UCHAR)0x07)
#define PLUS_LINK_MSG_ERASE          ((UCHAR)0x08)

#define TRANSFER_BUSY                              ((UCHAR)0x22) 

#define CHANNEL_0						0
///////////////////////////////////////////////////////////////
// State Machine Return Flags
///////////////////////////////////////////////////////////////
#define STATE_STATUS_NONE						((UCHAR)0x00) // State machine did not handle event in any way
#define STATE_STATUS_TRANSMIT             ((UCHAR)0x01) // State machine requires tx buffer to be transmitted. 
#define STATE_STATUS_HANDLED              ((UCHAR)0x02) // State machine handled input events, no further processing required 




//////////////////////////////////////////////
// PC Application Event Codes
//////////////////////////////////////////////
//NOTE: These events are not generated by the embedded ANT module

#define EVENT_RX_BROADCAST                         ((UCHAR)0x9A)           // returned when module receives broadcast data
#define EVENT_RX_ACKNOWLEDGED                      ((UCHAR)0x9B)           // returned when module receives acknowledged data
#define EVENT_RX_BURST_PACKET                      ((UCHAR)0x9C)           // returned when module receives burst data

#define EVENT_RX_EXT_BROADCAST                     ((UCHAR)0x9D)           // returned when module receives broadcast data
#define EVENT_RX_EXT_ACKNOWLEDGED                  ((UCHAR)0x9E)           // returned when module receives acknowledged data
#define EVENT_RX_EXT_BURST_PACKET                  ((UCHAR)0x9F)           // returned when module receives burst data

#define EVENT_RX_RSSI_BROADCAST                    ((UCHAR)0xA0)           // returned when module receives broadcast data
#define EVENT_RX_RSSI_ACKNOWLEDGED                 ((UCHAR)0xA1)           // returned when module receives acknowledged data
#define EVENT_RX_RSSI_BURST_PACKET                 ((UCHAR)0xA2)           // returned when module receives burst data

#define EVENT_RX_BTH_BROADCAST                     ((UCHAR)0xA3)           // returned when module receives broadcast data
#define EVENT_RX_BTH_ACKNOWLEDGED                  ((UCHAR)0xA4)           // returned when module receives acknowledged data
#define EVENT_RX_BTH_BURST_PACKET                  ((UCHAR)0xA5)           // returned when module receives burst data

#define EVENT_RX_BTH_EXT_BROADCAST                 ((UCHAR)0xA6)           // returned when module receives broadcast data
#define EVENT_RX_BTH_EXT_ACKNOWLEDGED              ((UCHAR)0xA7)           // returned when module receives acknowledged data
#define EVENT_RX_BTH_EXT_BURST_PACKET              ((UCHAR)0xA8)           // returned when module receives burst data
//////////////////////////////////////////////
// NVM Command Codes
//////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// Macros
///////////////////////////////////////////////////////////////
#define HIGH_BYTE(usWord) (UCHAR)((usWord >> 8) & 0x00FF) 
#define LOW_BYTE(usWord)  (UCHAR)(usWord & 0x00FF) 
#define BYTE0(x)                      ((UCHAR) x & 0xFF)
#define BYTE1(x)                      ((UCHAR) (x >> 8) & 0xFF)
#define BYTE2(x)                      ((UCHAR) (x >> 16) & 0xFF)
#define BYTE3(x)                      ((UCHAR) (x >> 24) & 0xFF)
#define MIN(x,y)        (((x)<(y))?(x):(y))
#define MAX(x,y)        (((x)>(y))?(x):(y))


/////////////////////////////////////////////////////////////////////////////
// Message Format
// Messages are in the format:
//
// AX XX YY -------- CK
//
// where: AX    is the 1 byte sync byte either transmit or recieve
//        XX    is the 1 byte size of the message (0-249) NOTE: THIS WILL BE LIMITED BY THE EMBEDDED RECEIVE BUFFER SIZE
//        YY    is the 1 byte ID of the message (1-255, 0 is invalid)
//        ----- is the data of the message (0-249 bytes of data)
//        CK    is the 1 byte Checksum of the message
/////////////////////////////////////////////////////////////////////////////
#define MESG_TX_SYNC                      ((UCHAR)0xA4)
#define MESG_RX_SYNC                      ((UCHAR)0xA5)
#define MESG_SIZE_OFFSET                  ((UCHAR)0)    
#define MESG_ID_OFFSET                    ((UCHAR)1)     
#define MESG_SYNC_SIZE                    ((UCHAR)1)
#define MESG_SIZE_SIZE                    ((UCHAR)1)
#define MESG_ID_SIZE                      ((UCHAR)1)
#define MESG_CHECKSUM_SIZE                ((UCHAR)1)
#define MESG_MAX_DATA_SIZE                ((UCHAR)9)     
#define MESG_MAX_DATA_BYTES               ((UCHAR)8)
#define MESG_CHANNEL_NUM_SIZE             ((UCHAR)1)
#define MESG_HEADER_SIZE                  (MESG_SYNC_SIZE + MESG_SIZE_SIZE + MESG_ID_SIZE)
#define MESG_DATA_OFFSET                  MESG_HEADER_SIZE-1  //2
#define MESG_FRAME_SIZE                   (MESG_HEADER_SIZE + MESG_CHECKSUM_SIZE)
#define MESG_SAVED_FRAME_SIZE             (MESG_SIZE_SIZE + MESG_ID_SIZE)
#define MESG_MAX_SIZE                     (MESG_MAX_DATA_SIZE + MESG_FRAME_SIZE)
#define MESG_BUFFER_SIZE                  (MESG_MAX_DATA_SIZE + MESG_SAVED_FRAME_SIZE)

#define ANT_DATA_SIZE                     8                    // ANT message payload size.
#define ANT_TX_SYNC                      ((UCHAR) 0xA4)
#define ANT_RX_SYNC                      ((UCHAR) 0xA5)
#define ANT_SIZE_OFFSET                  ((UCHAR) 0x00)
#define ANT_ID_OFFSET                    ((UCHAR) 0x01)
#define ANT_SYNC_SIZE                    ((UCHAR) 0x01)
#define ANT_SIZE_SIZE                    ((UCHAR) 0x01)
#define ANT_ID_SIZE                      ((UCHAR) 0x01)
#define ANT_RESPONSE_ERROR_SIZE          ((UCHAR) 0x01)
#define ANT_CHECKSUM_SIZE                ((UCHAR) 0x01)
#define ANT_MAX_DATA_SIZE                ((UCHAR) 0x17)
#define ANT_HEADER_SIZE                  (ANT_SYNC_SIZE + ANT_SIZE_SIZE + ANT_ID_SIZE)
#define ANT_REPLY_HEADER_SIZE            ANT_HEADER_SIZE + ANT_RESPONSE_ERROR_SIZE
#define ANT_DATA_OFFSET                  ANT_HEADER_SIZE-1     
#define ANT_FRAME_SIZE                   (ANT_HEADER_SIZE + ANT_CHECKSUM_SIZE)
#define ANT_MAX_SIZE                     (ANT_MAX_DATA_SIZE + ANT_FRAME_SIZE)


//////////////////////////////////////////////
// Buffer Indices
//////////////////////////////////////////////
#define BUFFER_INDEX_MESG_SIZE            ((UCHAR)0x00)
#define BUFFER_INDEX_MESG_ID              ((UCHAR)0x01)
#define BUFFER_INDEX_CHANNEL_NUM          ((UCHAR)0x02)
#define BUFFER_INDEX_MESG_DATA            ((UCHAR)0x03)
#define BUFFER_INDEX_RESPONSE_MESG_ID     ((UCHAR)0x03)
#define BUFFER_INDEX_RESPONSE_CODE        ((UCHAR)0x04)
#define BUFFER_INDEX_DATA0                ((UCHAR)0x03)
#define BUFFER_INDEX_SHARED_ADDRESS_LSB   ((UCHAR)0x03)
#define BUFFER_INDEX_SHARED_ADDRESS_MSB   ((UCHAR)0x04)
#define BUFFER_INDEX_SHARED_DATA_TYPE     ((UCHAR)0x05)

//////////////////////////////////////////////
// Message ID's
//////////////////////////////////////////////
#define MESG_INVALID_ID                   ((UCHAR)0x00)
#define MESG_EVENT_ID                     ((UCHAR)0x01)

#define MESG_APPVERSION_ID                ((UCHAR)0x3D)  ///< application interface version
#define MESG_VERSION_ID                   ((UCHAR)0x3E)  ///< protocol library version
#define MESG_RESPONSE_EVENT_ID            ((UCHAR)0x40)

#define MESG_UNASSIGN_CHANNEL_ID          ((UCHAR)0x41)
#define MESG_ASSIGN_CHANNEL_ID            ((UCHAR)0x42)
#define MESG_CHANNEL_MESG_PERIOD_ID       ((UCHAR)0x43)
#define MESG_CHANNEL_SEARCH_TIMEOUT_ID    ((UCHAR)0x44)
#define MESG_CHANNEL_RADIO_FREQ_ID        ((UCHAR)0x45)
#define MESG_NETWORK_KEY_ID               ((UCHAR)0x46)
#define MESG_RADIO_TX_POWER_ID            ((UCHAR)0x47)
#define MESG_RADIO_CW_MODE_ID             ((UCHAR)0x48)
#define MESG_SEARCH_WAVEFORM_ID           ((UCHAR)0x49)

#define MESG_SYSTEM_RESET_ID              ((UCHAR)0x4A)
#define MESG_OPEN_CHANNEL_ID              ((UCHAR)0x4B)
#define MESG_CLOSE_CHANNEL_ID             ((UCHAR)0x4C)
#define MESG_REQUEST_ID                   ((UCHAR)0x4D)

#define MESG_BROADCAST_DATA_ID            ((UCHAR)0x4E)
#define MESG_ACKNOWLEDGED_DATA_ID         ((UCHAR)0x4F)
#define MESG_BURST_DATA_ID                ((UCHAR)0x50)

#define MESG_CHANNEL_ID_ID                ((UCHAR)0x51)
#define MESG_CHANNEL_STATUS_ID            ((UCHAR)0x52)
#define MESG_RADIO_CW_INIT_ID             ((UCHAR)0x53)
#define MESG_CAPABILITIES_ID              ((UCHAR)0x54)

#define MESG_ADD_CHANNEL_ID                                     ((UCHAR)0x59) //Steven ->add
#define MESG_CONFIG_LIST_ID                                     ((UCHAR)0x5A)
#define MESG_SET_CHANNEL_TX_POWER_ID                            ((UCHAR)0x60)
#define MESG_SET_LOW_PRIORITY_CHANNEL_SEARCH_TIMEOUT_ID         ((UCHAR)0x63)
#define MESG_SET_SERIAL_NUM_CHANNEL_ID_ID                       ((UCHAR)0x65)
#define MESG_RX_EXT_MESGS_ENABLE_ID                             ((UCHAR)0x66)
#define MESG_ENABLE_LED_ID                                      ((UCHAR)0x68)
#define MESG_CRYSTAL_ENABLE_ID                                  ((UCHAR)0x6D)
#define MESG_LIB_CONFIG_ID                                      ((UCHAR)0x6E)
#define MESG_CONFIG_FREQUENCY_AGILITY_ID                        ((UCHAR)0x70)
#define MESG_SET_PROXIMITY_SEARCH_ID                            ((UCHAR)0x71)
#define MESG_SET_CHANNEL_SEARCH_PRIORITY_ID                     ((UCHAR)0x75)
#define MESG_OPEN_RX_SCAN_MODE_ID                               ((UCHAR)0x5B)
#define MESG_SLEEP_MESSAGE_ID                                   ((UCHAR)0xC5)
//#define MESG_

#define MESG_SENSRCORE_DIGITAL_DATA       ((UCHAR)0x40)

#define MESG_PIN_DIODE_CONTROL_ID		   ((UCHAR)0x90)
#define MESG_RUN_SCRIPT_ID                ((UCHAR)0x91)


//////////////////////////////////////////////
// Message Sizes
//////////////////////////////////////////////
#define MESG_INVALID_SIZE                 ((UCHAR)0)

#define MESG_RESPONSE_EVENT_SIZE          ((UCHAR)3)
#define MESG_CHANNEL_STATUS_SIZE          ((UCHAR)2)
#define MESG_VERSION_SIZE                 ((UCHAR)9)

#define MESG_UNASSIGN_CHANNEL_SIZE        ((UCHAR)1)
#define MESG_ASSIGN_CHANNEL_SIZE          ((UCHAR)3)
#define MESG_CHANNEL_ID_SIZE              ((UCHAR)5)
#define MESG_CHANNEL_MESG_PERIOD_SIZE     ((UCHAR)3)
#define MESG_CHANNEL_SEARCH_TIMEOUT_SIZE  ((UCHAR)2)
#define MESG_CHANNEL_RADIO_FREQ_SIZE      ((UCHAR)2)
#define MESG_NETWORK_KEY_SIZE             ((UCHAR)9)
#define MESG_RADIO_TX_POWER_SIZE          ((UCHAR)2)
#define MESG_RADIO_CW_MODE_SIZE           ((UCHAR)3)
#define MESG_RADIO_CW_INIT_SIZE           ((UCHAR)1)
#define MESG_SEARCH_WAVEFORM_SIZE         ((UCHAR)3)

#define MESG_SYSTEM_RESET_SIZE            ((UCHAR)1)
#define MESG_OPEN_CHANNEL_SIZE            ((UCHAR)1)
#define MESG_CLOSE_CHANNEL_SIZE           ((UCHAR)1)
#define MESG_REQUEST_SIZE                 ((UCHAR)2)
#define MESG_CAPABILITIES_SIZE            ((UCHAR)4)

#define MESG_ADD_CHANNEL_SIZE                                     ((UCHAR)6) //Steven ->add
#define MESG_CONFIG_LIST_SIZE                                     ((UCHAR)3)
#define MESG_SET_CHANNEL_TX_POWER_SIZE                            ((UCHAR)2)
#define MESG_SET_LOW_PRIORITY_CHANNEL_SEARCH_TIMEOUT_SIZE         ((UCHAR)2)
#define MESG_SET_SERIAL_NUM_CHANNEL_ID_SIZE                       ((UCHAR)3)
#define MESG_RX_EXT_MESGS_ENABLE_SIZE                             ((UCHAR)2)
#define MESG_ENABLE_LED_SIZE                                      ((UCHAR)2)
#define MESG_CRYSTAL_ENABLE_SIZE                                  ((UCHAR)1)
#define MESG_LIB_CONFIG_SIZE                                      ((UCHAR)2)
#define MESG_CONFIG_FREQUENCY_AGILITY_SIZE                        ((UCHAR)4)
#define MESG_SET_PROXIMITY_SEARCH_SIZE                            ((UCHAR)2)
#define MESG_SET_CHANNEL_SEARCH_PRIORITY_SIZE                     ((UCHAR)2)
#define MESG_OPEN_RX_SCAN_MODE_SIZE                               ((UCHAR)1)
#define MESG_SLEEP_MESSAGE_SIZE                                   ((UCHAR)1)

#define MESG_DATA_SIZE                    ((UCHAR)9)

#define MESG_PIN_DIODE_CONTROL_ID_SIZE    ((UCHAR)2)
#define MESG_RUN_SCRIPT_SIZE              ((UCHAR)2)

#endif // __ANT_MESSAGES_H__
