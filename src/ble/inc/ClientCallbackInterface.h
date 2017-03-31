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
#ifndef ICLIENTCALLBADK
#define ICLIENTCALLBADK
#include "GFBLETypes.h"

class GF_CClientCallback
{
public:
	virtual void onScanResult(GF_BLEDevice* device) = 0;
	virtual void onScanFinished() = 0;
	virtual void onDeviceConnected(GF_STATUS status, GF_ConnectedDevice *device) = 0;
	virtual void onDeviceDisconnected(GF_STATUS status, GF_ConnectedDevice *device, GF_UINT8 reason) = 0;

	virtual void onMTUSizeChanged(GF_STATUS status, GF_UINT16 handle, GF_UINT16 mtu_size) = 0;
	virtual void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency) = 0;
	virtual void onCharacteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data) = 0;

	/*Notification format: data length(1 byte N) + data(N Bytes)*/
	virtual void onNotificationReceived(GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data) = 0;

	virtual void onComDestory() = 0;
};
#endif