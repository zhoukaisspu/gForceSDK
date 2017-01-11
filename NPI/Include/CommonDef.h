#ifndef _CMNDEF_H_
#define _CMNDEF_H_

#include"windows.h"
#include"npi_queue.h"
#include"Thread.h"
#include"log.h"
//#define STD_OUT
#define MFC_GUI

#define MAX_RX_SIZE     255
#define MAX_TX_SIZE     255

#define CMD_BUF_SIZE    MAX_TX_SIZE*10

#define EVT_HEADER_LEN        3
#define BLE_ADDR_LEN          6
#define IRK_LEN               16
#define CSRK_LEN              16
#define SIGN_COUNT_LEN        4
#define OOB_DATA_LEN          16
#define PASSKEY_LEN           16
#define LTK_LEN               16
#define LTK_RAND_PAIR_LEN     16
#define B_RANDOM_NUM_SIZE       8
#define FEATURES_LEN            8
#define ADV_CHANNEL_37          0x01
#define ADV_CHANNEL_38          0x02
#define ADV_CHANNEL_39          0x04
#define ADV_CHANNEL_ALL         (ADV_CHANNEL_37 | ADV_CHANNEL_38 | ADV_CHANNEL_39)


#define BUILD_UINT16(loByte, hiByte) \
          ((UINT16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))
#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)
#endif