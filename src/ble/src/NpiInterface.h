/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
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
#ifndef __GF_NIF_H__
#define __GF_NIF_H__

#include <npi_cmd.h>
#include <npi_evt.h>
#include <GFBLETypes.h>
#include <LogPrint.h>
#include <iostream>
#include <list>
using namespace std;

#define MODUAL_TAG "Native Interface"

#define ADAPTER_MANAGER_CALLBACK_INDEX     0
#define DISCOVERY_SERVICE_CALLBACK_INDEX   1
#define CALLBACK_MAX_INDEX				   2
//list<int> mCallback;
typedef list<GF_CCallBack *> LISTCALLBACK;

class GF_CNpiInterface:public Runnable
{
public:
	GF_VOID Run();
	GF_CNpiInterface();
	~GF_CNpiInterface();
	GF_STATUS Init(GF_UINT8 com_num);
	GF_STATUS Deinit();

	GF_STATUS InitDevice();
	GF_STATUS StartLEScan();
	GF_STATUS StopLEScan();
	GF_STATUS Connect(GF_PUINT8 addr, UINT8 addr_type, GF_BOOL use_whitelist);
	GF_STATUS Disconnect(GF_UINT16 handle);
	
	GF_STATUS Authenticate(GF_UINT16 handle);
	GF_STATUS Bond(GF_UINT16 handle, GF_PUINT8 ltk, GF_UINT16 div, GF_PUINT8 rand, GF_UINT8 ltk_size);
	
	GF_STATUS ExchangeMTUSize(GF_UINT16 handle, GF_UINT16 mtu);
	GF_STATUS UpdateConnectionParameter(GF_UINT16 handle, GF_UINT16 int_min, GF_UINT16 int_max, GF_UINT16 slave_latency, GF_UINT16 supervision_timeout);
	GF_STATUS DiscoveryAllPrimaryService(GF_UINT16 handle);
	GF_STATUS DiscoveryIncludedPrimaryService(GF_UINT16 conn_handle, GF_UINT16 start_handle, GF_UINT16 end_handle);
	GF_STATUS DiscoveryCharacteristic(GF_UINT16 conn_handle, GF_UINT16 start_handle, GF_UINT16 end_handle);
	GF_STATUS ReadCharacteristicValue(GF_UINT16 conn_handle, GF_UINT16 att_handle);
	GF_STATUS ReadCharacteristicLongValue(GF_UINT16 conn_handle, GF_UINT16 att_handle, UINT16 offset);
	GF_STATUS FindCharacteristicDescriptor(GF_UINT16 conn_handle, GF_UINT16 start_handle, GF_UINT16 end_handle);
	GF_STATUS WriteCharacVlaue(GF_UINT16 conn_handle, GF_UINT16 att_handle, GF_PUINT8 data, GF_UINT8 len);

	//call this function to register callback function to process event.
	GF_STATUS RegisterCallback(GF_CCallBack *callback);
	GF_STATUS UnRegisterCallback(GF_CCallBack *callback);
	
private:
	NPI_CMD* mCommand;
	//LISTCALLBACK mCallback;
	GF_CCallBack * mCallback[CALLBACK_MAX_INDEX];
	CThread *mEvtThread;

	/*event queue.
	* NPI will push event to this queue,
	* NIF will pop event from this queue to process.
	*/
	NPI_Queue<sEvt*, EVT_QUEUE_SIZE>* mEventQueue;
	GF_STATUS GF_Process_Event(GF_UINT16 event_code, GF_PUINT8 data, GF_UINT8 length);

	GF_CPCHAR mTag;

	CRITICAL_SECTION mMutex;
};

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define EVENT_MASK_GAP_STATUS_MSG				BIT0
#define EVENT_MASK_GAP_DEVICE_INFO_MSG			BIT1
#define EVENT_MASK_GAP_DEVICE_DISCOVERY_MSG		BIT2
#define EVENT_MASK_GAP_LINK_ESTABLISHED_MSG		BIT3
#define EVENT_MASK_GAP_LINK_TERMINATED_MSG		BIT4
#define EVENT_MASK_SLAVE_REQUESTED_SECURITY_MSG BIT5
#define EVENT_MASK_AUTH_COMPLETE_MSG			BIT6
#define EVENT_MASK_BOND_COMPLETE_MSG			BIT7

#define EVENT_MASK_LINK_PARA_UPDATE_MSG			(BIT0 << 8)
#define EVENT_MASK_ATT_NOTI_MSG					(BIT2 << 8)
#define EVENT_MASK_ATT_READ_BY_GRP_TYPE_MSG		(BIT3 << 8)
#define EVENT_MASK_ATT_READ_BY_TYPE_MSG			(BIT4 << 8)
#define EVENT_MASK_ATT_READ_BY_INFO_MSG			(BIT5 << 8)
#define EVENT_MASK_ATT_ERROR_MSG				(BIT6 << 8)
#define EVENT_MASK_ATT_READ_RESP_MSG			(BIT7 << 8)

#define EVENT_MASK_INTERNAL_DEVICE_FOUND		(BIT0 << 16)
#define EVENT_MASK_INTERNAL_SCAN_FINISHED		(BIT1 << 16)
#define EVENT_MASK_ATT_READ_BLOB_RESP_MSG		(BIT2 << 16)
#define EVENT_MASK_ATT_EXCHANGE_MTU_MSG			(BIT3 << 16)
#define EVENT_MASK_ATT_WRITE_MSG				(BIT4 << 16)
#define EVENT_MASK_GAP_SCAN_RESULT				(BIT5 << 16)
#define EVENT_MASK_GAP_SCAN_FINISHED			(BIT6 << 16)

#define EVENT_MASK_DEVICE_CONNECTED				(BIT0 << 24)
#define EVENT_MASK_DEVICE_PARAMETER_UPDATED		(BIT1 << 24)

#define EVENT_SERIAL_PORT_NOT_AVAILABLE			(BIT2 << 24)
#endif