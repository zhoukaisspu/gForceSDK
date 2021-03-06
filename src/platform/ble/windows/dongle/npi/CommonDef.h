/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
#ifndef _CMNDEF_H_
#define _CMNDEF_H_

#include"windows.h"
#include"npi_queue.h"
#include"Thread.h"
//#define STD_OUT

#define MAX_RX_SIZE     255
#define MAX_TX_SIZE     255

#define CMD_QUEUE_SIZE        1024
#define EVT_QUEUE_SIZE        1024

#define EVT_HEADER_LEN        3
#define BLE_ADDR_LEN          6
#define IRK_LEN               16
#define CSRK_LEN              16
#define SIGN_COUNT_LEN        4
#define OOB_DATA_LEN          16
#define PASSKEY_LEN           16
#define LTK_LEN               16
#define LTK_RAND_PAIR_LEN     8
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

template<class TKey, class TVal, unsigned max>
class NPI_Dict
{
	TKey key[max];
	TVal val[max];
	LONG count;
public:
	NPI_Dict() : count(0)
	{
	}
	void put(TKey k, TVal v)
	{
		key[count] = k;
		val[count] = v;
		if ((count++) == max) {
			count = 0;
		}
	}
	TVal get(TKey k)
	{
		for (int i = 0; i < count; i++) {
			if (key[i] == k) {
				return val[i];
			}
		}
		return NULL;
	}
};

#endif