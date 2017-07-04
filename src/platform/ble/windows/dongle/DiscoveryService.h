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
#ifndef __DISCOVERYSERVICE_H_HH
#define __DISCOVERYSERVICE_H_HH

#include <GFBLETypes.h>
#include <NpiInterface.h>
#include <LogPrint.h>
//#include "AdapterManager.h"

#define LE_CONN_ADV			0
#define LE_SCAN_RESPONSE    4

#define LE_ADV_EVENT_TYPE_OFFSET   1
#define LE_ADV_ADDRESS_TYPE_OFFSET (LE_ADV_EVENT_TYPE_OFFSET+1)
#define LE_ADV_ADDRESS_OFFSET      (LE_ADV_ADDRESS_TYPE_OFFSET+1)
#define LE_ADV_RSSI_OFFSET         (LE_ADV_ADDRESS_OFFSET+6)
#define LE_ADV_DATA_LEN_OFFSET     (LE_ADV_RSSI_OFFSET+1)
#define LE_ADV_DATA_OFFSET         (LE_ADV_DATA_LEN_OFFSET+1)

//the threshlold can be adjust.
#define LE_ADV_RSSI_THRESHOLD 0xC8

//define return value for adv checking
#define LE_ADV_PROCESS_OK			 0 //can been report to upper level for new device
#define LE_ADV_UNDISCOVERABEL		 1 //the adv is either general discoverabel or limited discoverabel
#define LE_ADV_WAIT_FOR_DEVICE_NAME  2 //report new device to upper level until device name is availabel, whcih may exist in scan reponse

#define BLE_AD_TYPE_FLAG			0x01
#define BLE_AD_TYPE_16SRV_PART      0x02
#define BLE_AD_TYPE_NAME_CMPL       0x09
#define BLE_AD_TYPE_APPEARANCE      0x19

#define BLE_LIMIT_DISC_FLAG         (0x01 << 0)
#define BLE_GEN_DISC_FLAG           (0x01 << 1)

#define DEVICE_NAME_PATTERN_LENGTH 9
const char DEVICE_NAME_PATTERN[DEVICE_NAME_PATTERN_LENGTH] = { 0x67, 0x46, 0x6F, 0x72, 0x63, 0x65, 0x4A, 0x4F, 0x59 };

#define DISCOVERY_SERVICE_EVENT_MASK (EVENT_MASK_GAP_DEVICE_INFO_MSG | EVENT_MASK_GAP_DEVICE_DISCOVERY_MSG)
	
#define MODUAL_TAG_DS "DiscoveryService"

class GF_CDiscoveryService : public GF_CCallBack
{
public:
	GF_CDiscoveryService(GF_CNpiInterface* minterface, GF_CCallBack* mcallback);
	//GF_CDiscoveryService(GF_CNpiInterface* minterface);
	~GF_CDiscoveryService();

	GF_STATUS Init();
	GF_STATUS Deinit();

	GF_STATUS OnDeviceFound(GF_BLEDevice new_device)
	{
		return GF_FAIL;
	}

	GF_STATUS OnEvent(GF_UINT32 event, GF_PUINT8 data, GF_UINT16 length);

	GF_STATUS StartScan(GF_UINT8 RSSI_Threshold);
	GF_STATUS StopScan();

private:
	GF_STATUS ProcessScanFinished();
	GF_STATUS ProcessScanResult(GF_PUINT8 data, GF_UINT16 length);
	GF_STATUS CheckAdvData(GF_PUINT8 data, GF_UINT16 length);
	GF_BOOL GetDeviceName(GF_PCHAR result, GF_PUINT8 data, GF_UINT16 length);

	GF_CNpiInterface *mInterface;
	GF_BOOL mIsScanning;
	//GF_CAdapterManager* mAdapterManager;

	//fix Log name to mLog!!
	GF_CPCHAR mTag;

	list<GF_BLEDevice> mAvalbalDevice;
	GF_CCallBack* mCallback;
	GF_UINT8 mRSSI_Threshold;
};
#endif