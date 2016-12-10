
#ifndef HCI_TL_H
#define HCI_TL_H

#ifdef __cplusplus
extern "C"
{
#endif

// HCI Packet Types
#define HCI_CMD_PACKET                 0x01
#define HCI_ACL_DATA_PACKET            0x02
#define HCI_SCO_DATA_PACKET            0x03
#define HCI_EVENT_PACKET               0x04


/*
** HCI Command Opcodes
*/

// Link Control Commands
#define HCI_DISCONNECT                                    0x0406
#define HCI_READ_REMOTE_VERSION_INFO                      0x041D

// Controller and Baseband Commands
#define HCI_SET_EVENT_MASK                                0x0C01
#define HCI_RESET                                         0x0C03
#define HCI_READ_TRANSMIT_POWER                           0x0C2D
#define HCI_SET_CONTROLLER_TO_HOST_FLOW_CONTROL           0x0C31
#define HCI_HOST_BUFFER_SIZE                              0x0C33
#define HCI_HOST_NUM_COMPLETED_PACKETS                    0x0C35
#define HCI_SET_EVENT_MASK_PAGE_2                         0x0C63
#define HCI_READ_AUTH_PAYLOAD_TIMEOUT                     0x0C7B
#define HCI_WRITE_AUTH_PAYLOAD_TIMEOUT                    0x0C7C

// Information Parameters
#define HCI_READ_LOCAL_VERSION_INFO                       0x1001
#define HCI_READ_LOCAL_SUPPORTED_COMMANDS                 0x1002
#define HCI_READ_LOCAL_SUPPORTED_FEATURES                 0x1003
#define HCI_READ_BDADDR                                   0x1009

// Status Parameters
#define HCI_READ_RSSI                                     0x1405

// LE Commands
// V4.0
#define HCI_LE_SET_EVENT_MASK                             0x2001
#define HCI_LE_READ_BUFFER_SIZE                           0x2002
#define HCI_LE_READ_LOCAL_SUPPORTED_FEATURES              0x2003
#define HCI_LE_SET_RANDOM_ADDR                            0x2005
#define HCI_LE_SET_ADV_PARAM                              0x2006
#define HCI_LE_READ_ADV_CHANNEL_TX_POWER                  0x2007
#define HCI_LE_SET_ADV_DATA                               0x2008
#define HCI_LE_SET_SCAN_RSP_DATA                          0x2009
#define HCI_LE_SET_ADV_ENABLE                             0x200A
#define HCI_LE_SET_SCAN_PARAM                             0x200B
#define HCI_LE_SET_SCAN_ENABLE                            0x200C
#define HCI_LE_CREATE_CONNECTION                          0x200D
#define HCI_LE_CREATE_CONNECTION_CANCEL                   0x200E
#define HCI_LE_READ_WHITE_LIST_SIZE                       0x200F
#define HCI_LE_CLEAR_WHITE_LIST                           0x2010
#define HCI_LE_ADD_WHITE_LIST                             0x2011
#define HCI_LE_REMOVE_WHITE_LIST                          0x2012
#define HCI_LE_CONNECTION_UPDATE                          0x2013
#define HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION            0x2014
#define HCI_LE_READ_CHANNEL_MAP                           0x2015
#define HCI_LE_READ_REMOTE_USED_FEATURES                  0x2016
#define HCI_LE_ENCRYPT                                    0x2017
#define HCI_LE_RAND                                       0x2018
#define HCI_LE_START_ENCRYPTION                           0x2019
#define HCI_LE_LTK_REQ_REPLY                              0x201A
#define HCI_LE_LTK_REQ_NEG_REPLY                          0x201B
#define HCI_LE_READ_SUPPORTED_STATES                      0x201C
#define HCI_LE_RECEIVER_TEST                              0x201D
#define HCI_LE_TRANSMITTER_TEST                           0x201E
#define HCI_LE_TEST_END                                   0x201F
// V4.1
#define HCI_LE_REMOTE_CONN_PARAM_REQ_REPLY                0x2020
#define HCI_LE_REMOTE_CONN_PARAM_REQ_NEG_REPLY            0x2021
// V4.2
#define HCI_LE_SET_DATA_LENGTH                            0x2022
#define HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LENGTH         0x2023
#define HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH        0x2024
#define HCI_LE_READ_LOCAL_P256_PUBLIC_KEY                 0x2025
#define HCI_LE_GENERATE_DHKEY                             0x2026
#define HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST               0x2027
#define HCI_LE_REMOVE_DEVICE_FROM_RESOLVING_LIST          0x2028
#define HCI_LE_CLEAR_RESOLVING_LIST                       0x2029
#define HCI_LE_READ_RESOLVING_LIST_SIZE                   0x202A
#define HCI_LE_READ_PEER_RESOLVABLE_ADDRESS               0x202B
#define HCI_LE_READ_LOCAL_RESOLVABLE_ADDRESS              0x202C
#define HCI_LE_SET_ADDRESS_RESOLUTION_ENABLE              0x202D
#define HCI_LE_SET_RESOLVABLE_PRIVATE_ADDRESS_TIMEOUT     0x202E
#define HCI_LE_READ_MAXIMUM_DATA_LENGTH                   0x202F
// V5.0
#define HCI_LE_READ_PHY                                   0x2030
#define HCI_LE_SET_DEFAULT_PHY                            0x2031
#define HCI_LE_SET_PHY                                    0x2032
#define HCI_LE_ENHANCED_RECEIVER_TEST                     0x2033
#define HCI_LE_ENHANCED_TRANSMITTER_TEST                  0x2034

// LE Vendor Specific LL Extension Commands
#define HCI_EXT_SET_RX_GAIN                               0xFC00
#define HCI_EXT_SET_TX_POWER                              0xFC01
#define HCI_EXT_ONE_PKT_PER_EVT                           0xFC02
#define HCI_EXT_CLK_DIVIDE_ON_HALT                        0xFC03
#define HCI_EXT_DECLARE_NV_USAGE                          0xFC04
#define HCI_EXT_DECRYPT                                   0xFC05
#define HCI_EXT_SET_LOCAL_SUPPORTED_FEATURES              0xFC06
#define HCI_EXT_SET_FAST_TX_RESP_TIME                     0xFC07
#define HCI_EXT_MODEM_TEST_TX                             0xFC08
#define HCI_EXT_MODEM_HOP_TEST_TX                         0xFC09
#define HCI_EXT_MODEM_TEST_RX                             0xFC0A
#define HCI_EXT_END_MODEM_TEST                            0xFC0B
#define HCI_EXT_SET_BDADDR                                0xFC0C
#define HCI_EXT_SET_SCA                                   0xFC0D
#define HCI_EXT_ENABLE_PTM                                0xFC0E // Not a supported HCI command! Application only.
#define HCI_EXT_SET_FREQ_TUNE                             0xFC0F
#define HCI_EXT_SAVE_FREQ_TUNE                            0xFC10
#define HCI_EXT_SET_MAX_DTM_TX_POWER                      0xFC11
#define HCI_EXT_MAP_PM_IO_PORT                            0xFC12
#define HCI_EXT_DISCONNECT_IMMED                          0xFC13
#define HCI_EXT_PER                                       0xFC14
#define HCI_EXT_PER_BY_CHAN                               0xFC15 // Not a supported HCI command! Application only.
#define HCI_EXT_EXTEND_RF_RANGE                           0xFC16
#define HCI_EXT_ADV_EVENT_NOTICE                          0xFC17 // Not a supported HCI command! Application only.
#define HCI_EXT_CONN_EVENT_NOTICE                         0xFC18 // Not a supported HCI command! Application only.
#define HCI_EXT_HALT_DURING_RF                            0xFC19
#define HCI_EXT_OVERRIDE_SL                               0xFC1A
#define HCI_EXT_BUILD_REVISION                            0xFC1B
#define HCI_EXT_DELAY_SLEEP                               0xFC1C
#define HCI_EXT_RESET_SYSTEM                              0xFC1D
#define HCI_EXT_OVERLAPPED_PROCESSING                     0xFC1E
#define HCI_EXT_NUM_COMPLETED_PKTS_LIMIT                  0xFC1F
#define HCI_EXT_GET_CONNECTION_INFO                       0xFC20
#
#define HCI_EXT_LL_TEST_MODE                              0xFC70

// L2CAP Commands
#define L2CAP_DISCON_REQ								  0xFC86
#define L2CAP_INFO_REQ								  	  0xFC8A
#define L2CAP_CON_PAR_UPDATE							  0xFC92
#define L2CAP_CONNECT_REQ								  0xFC94
#define L2CAP_CONNECT_RSP								  0xFC95
#define L2CAP_FLOW_CTRL_CRT								  0xFC96
#define L2CAP_DATA										  0xFCF0
#define L2CAP_REG_PSM									  0xFCF1
#define L2CAP_DEREG_PSM									  0xFCF2
#define L2CAP_PSM_INFO									  0xFCF3
#define L2CAP_PSM_CHANNEL								  0xFCF4
#define L2CAP_CHANNEL_INFO								  0xFCF5

//ATT Commands
#define ATT_ERROR_RSP									  0xFD01
#define ATT_EXC_MTU_REQ									  0xFD02
#define ATT_EXC_MTU_RSP									  0xFD03
#define ATT_FIND_INFO_REQ								  0xFD04
#define ATT_FIND_INFO_RSP								  0xFD05
#define ATT_FIND_BY_TYPE								  0xFD06
#define ATT_FIND_BY_TYPE_RSP							  0xFD07
#define ATT_READ_BY_TYPE_REQ							  0xFD08
#define ATT_READ_BY_TYPE_RSP							  0xFD09
#define ATT_READ_REQ									  0xFD0A
#define ATT_READ_RSP									  0xFD0B
#define ATT_READ_BLOB_REQ								  0xFD0C
#define ATT_READ_BLOB_RSP								  0xFD0D
#define ATT_READ_MULTI_REQ								  0xFD0E
#define ATT_READ_MULTI_RSP								  0xFD0F
#define ATT_READ_BY_GROUP_TYPE_REQ						  0xFD10
#define ATT_READ_BY_GROUP_TYPE_RSP						  0xFD11
#define ATT_WRITE_REQ									  0xFD12
#define ATT_WRITE_RSP									  0xFD13
#define ATT_PREP_WRITE_REQ								  0xFD16
#define ATT_PREP_WRITE_RSP								  0xFD17
#define ATT_EXE_WRITE_REQ								  0xFD18
#define ATT_HDL_VALUE_NOTIFY							  0xFD19
#define ATT_HDL_VALUE_IND								  0xFD1B
#define ATT_HDL_VALUE_CONFIRM							  0xFD1E

//GATT Commands
#define GATT_EXCHANGE_MTU								  0xFD82
#define GATT_DISC_ALL_CHAR_DESC							  0xFD84
#define GATT_DISC_PRI_SERV_BY_UUID						  0xFD86
#define GATT_DISC_CHAR_BY_UUID							  0xFD88
#define GATT_READ_LONG_CHAR_VAL							  0xFD8C
#define GATT_READ_MULTI_CHAR_VAL						  0xFD8E
#define GATT_DISC_ALL_PRI_SERV							  0xFD90
#define GATT_WRITE_CHAR_VAL								  0xFD92
#define GATT_WRITE_LONG_CHAR_VAL						  0xFD96
#define GATT_NOTIFY										  0xFD9B
#define GATT_INDICATION									  0xFD9D

#define GATT_FIND_INC_SERV								  0xFDB0
#define GATT_DISC_ALL_CHAR								  0xFDB2
#define GATT_READ_USE_CHAR_UUID							  0xFDB4
#define GATT_WRITE_NO_RES								  0xFDB6
#define GATT_SINGLE_WRITE_NO_RES						  0xFDB8
#define GATT_READ_CHAR_VAL								  0xFDBA
#define GATT_READ_CHAR_DESC								  0xFDBC
#define GATT_READ_LONG_CHAR_DESC						  0xFDBE
#define GATT_WRITE_CHAR_DESC						      0xFDC0
#define GATT_WRITE_LONG_CHAR_DESC					      0xFDC2
#define GATT_ADD_SERVICE							      0xFDFC
#define GATT_DEL_SERVICE							      0xFDFD
#define GATT_ADD_ATTRIBUTE							      0xFDFE

//GAP Commands
#define GAP_DEVICE_INIT								      0xFE00
#define GAP_CONFIG_DEVICE_ADDR						      0xFE03
#define GAP_DEVICE_DISCOVERY_REQ						  0xFE04
#define GAP_DEVICE_DISCOVERY_CANCEL						  0xFE05
#define GAP_MAKE_DISCOVERABLE							  0xFE06
#define GAP_UPDATE_ADV_DATA								  0xFE07
#define GAP_END_DISCOVERABLE							  0xFE08
#define GAP_ESTABLISH_LINK_REQ							  0xFE09
#define GAP_TERMINATE_LINK_REQ							  0xFE0A
#define GAP_AUTHENTIC_REQ								  0xFE0B
#define GAP_PASSKEY_UPDATE								  0xFE0C
#define GAP_SLAVE_SEC_REQ								  0xFE0D
#define GAP_SIGNABLE									  0xFE0E
#define GAP_BOND										  0xFE0F
#define GAP_TERMINATE_AUTH								  0xFE10
#define GAP_UPDATE_LINK_PARA_REQ						  0xFE11

#define GAP_SET_PARAM									  0xFE30
#define GAP_GET_PARAM									  0xFE31
#define GAP_RESOLVE_PRI_ADDR							  0xFE32
#define GAP_SET_ADV_TOKEN								  0xFE33
#define GAP_REMOVE_ADV_TOKEN							  0xFE34
#define GAP_UPDATE_ADV_TOKEN							  0xFE35
#define GAP_BOND_SET_PARAM								  0xFE36
#define GAP_BOND_GET_PARAM								  0xFE37

//UTIL Commands
#define UTIL_RESET										  0xFE80
#define UTIL_NV_READ									  0xFE81
#define UTIL_NV_WRITE									  0xFE82

#ifdef __cplusplus
}
#endif

#endif /* HCI_TL_H */
