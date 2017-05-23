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
#ifndef __ADAPTERMANAGERINTERFACE_H__
#define __ADAPTERMANAGERINTERFACE_H__
#include "GFBLETypes.h"

class GF_CClientCallback;
//class GF_CAdapterManager;

class GF_CAdapterManagerInterface
{
public:
	static GF_CAdapterManagerInterface* GetInstance();
	virtual GF_STATUS Init(GF_UINT8 com_num, GF_UINT8 log_type) = 0;
	virtual GF_STATUS Deinit() = 0;
	virtual GF_STATUS StartScan(GF_UINT8 RSSI_Threshold) = 0;
	virtual GF_STATUS StopScan() = 0;
	virtual GF_STATUS Connect(GF_PUINT8 addr, GF_UINT8 addr_type, GF_BOOL is_direct_conn) = 0;
	virtual GF_STATUS CancelConnect(GF_PUINT8 addr, GF_UINT8 addr_type) = 0;
	virtual GF_STATUS Disconnect(GF_UINT16 handle) = 0;
	virtual GF_STATUS RegisterClientCallback(GF_CClientCallback* callback) = 0;
	virtual GF_STATUS UnregisterClientCallback() = 0;

	//virtual GF_STATUS ConfigMtuSize(GF_UINT16 conn_handle, GF_UINT16 MTU_Size) = 0;
	//virtual GF_STATUS ConnectionParameterUpdate(GF_UINT16 conn_handle, GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout) = 0;
	//virtual GF_STATUS WriteCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data) = 0;
	//virtual GF_STATUS ReadCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle) = 0;

   // virtual GF_HubState GetHubState() = 0;
	//virtual GF_UINT8 GetConnectedDeviceNum() = 0;
	/*connected_device is output result*/
	//virtual GF_STATUS GetConnectedDeviceByIndex(GF_UINT8 index, GF_ConnectedDevice* connected_device) = 0;
};
#endif