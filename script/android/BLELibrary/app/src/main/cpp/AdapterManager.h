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
#ifndef __ADAPTERMANAGER_H__
#define __ADAPTERMANAGER_H__
#include "GFBLETypes.h"
#include "AdapterManagerInterface.h"
#include <list>
using namespace std;

class GF_CClientCallback;

class GF_CAdapterManager : public GF_CAdapterManagerInterface
{
public:
	GF_STATUS Init(GF_UINT8 com_num, GF_UINT8 log_type);
	GF_STATUS Deinit();
	GF_STATUS StartScan(GF_UINT8 RSSI_Threshold);
	GF_STATUS StopScan();
	GF_STATUS Connect(GF_PUINT8 addr, GF_UINT8 addr_type, GF_BOOL is_direct_conn);
	GF_STATUS CancelConnect(GF_PUINT8 addr, GF_UINT8 addr_type);
	GF_STATUS Disconnect(GF_UINT16 handle);
	GF_STATUS RegisterClientCallback(GF_CClientCallback* callback);
	GF_STATUS UnregisterClientCallback();

	GF_STATUS ConfigMtuSize(GF_UINT16 conn_handle, GF_UINT16 MTU_Size);
	GF_STATUS ConnectionParameterUpdate(GF_UINT16 conn_handle, GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout);
	GF_STATUS WriteCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data);
	GF_STATUS ReadCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle);

    GF_HubState GetHubState();
	GF_UINT8 GetConnectedDeviceNum();
	/*connected_device is output result*/
	GF_STATUS GetConnectedDeviceByIndex(GF_UINT8 index, GF_ConnectedDevice* connected_device);

	//GF_STATUS OnEvent(GF_UINT32 event, GF_PUINT8 data, GF_UINT16 length);
	//GF_STATUS OnDeviceFound(GF_BLEDevice new_device);
	GF_CAdapterManagerInterface* GetInstance();

protected:
	GF_CAdapterManager();
	~GF_CAdapterManager();
	friend class GF_CAdapterManagerInterface;
private:
	static GF_CAdapterManager* mInstance;
	//GF_STATUS ConnectComplete(GF_UINT16 handle);
	//GF_STATUS ScanFinished();
	//GF_STATUS OnConnectEvent(GF_PUINT8 data, GF_UINT16 length);
	//GF_CRemoteDevice* GetDeviceByHandle(GF_UINT16 handle);
	//GF_STATUS mIsConnecting;
	GF_STATUS mIsScanning;
	//GF_CNpiInterface* mInterface;
	//GF_CPCHAR mTag;
	//GF_CDiscoveryService* mDS;

	//list<GF_CRemoteDevice*> mAvailabeDevice;
	//list<GF_CRemoteDevice*> mConnectingDevice;
	//list<GF_CRemoteDevice*> mConnectedDevice;
	//GF_CRemoteDevice* mCancelConnectingDevice;
	//GF_CClientCallback* mClientCallback;
	jclass GlobalClass;
    jobject GlobalObject;
};
#endif