// define GForce device
#pragma once

#include "Device.h"
#include "IDongle.h"
#include "AdapterManager.h"
#include "Utils.h"
#include <set>
using namespace std;

namespace oym
{

	class BLEDevice : public Device
	{
	public:
		BLEDevice(IDongle* dongle, const BLE_DEVICE& bleDev);
		virtual ~BLEDevice() { }

		virtual GF_UINT8 getAddrType() const { return mAddrType; }
		virtual GF_RET_CODE getAddress(GF_UINT8* addr, GF_SIZE bufLen) const;
		virtual tstring getAddress() const;
		virtual tstring getName() const { return mName; }
		virtual GF_UINT8 getRssi() const { return mRssi; }
		virtual GF_UINT16 getHandle() const { return mHandle; }
		virtual GF_UINT16 getConnInt() const { return mConnInt; }
		virtual GF_UINT16 getSuperTO() const { return mSuperTO; }
		virtual GF_UINT16 getSlavelatency() const { return mSlavelatency; }
		virtual GF_UINT16 getMTUsize() const { return mMTUsize; }

		virtual GF_RET_CODE identifyDevice(int msec = 1000);

		virtual GF_RET_CODE registerListener(const gfwPtr<DeviceListener>& listener);

		virtual DeviceConnectionStatus getConnectionStatus() const { return mCnntStatus; }
		virtual GF_RET_CODE setPostion(DevicePosition pos);
		virtual DevicePosition getPosition() const { return mPosition; }

		virtual GF_RET_CODE connect(bool directConn = true);
		virtual GF_RET_CODE disconnect();
		virtual GF_RET_CODE cancelConnect();

		virtual GF_RET_CODE configMtuSize(GF_UINT16 mtuSize);
		virtual GF_RET_CODE connectionParameterUpdate(GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max,
			GF_UINT16 slave_latence, GF_UINT16 supervision_timeout);
		virtual GF_RET_CODE WriteCharacteristic(GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data);
		virtual GF_RET_CODE ReadCharacteristic(GF_UINT16 attribute_handle);

	public:
		virtual void updateData(const BLE_DEVICE& bleDev);
		virtual void onConnected(const GF_ConnectedDevice& connedDevice);
		virtual void onDisconnected(GF_UINT8 reason);
		virtual void onMTUSizeChanged(GF_STATUS status, GF_UINT16 mtu_size);
		virtual void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency);
		virtual void onChracteristicValueRead(GF_STATUS status, GF_UINT8 length, GF_PUINT8 data);

	public:
		virtual bool operator < (const BLEDevice& devRight) const {
			if (mAddrType == devRight.mAddrType) {
				return ((0 > memcmp(mAddress, devRight.mAddress, sizeof(mAddress))) ?
					true : false);
			}
			else {
				return (mAddrType < devRight.mAddrType ? true : false);
			}
		}
		virtual bool isMyself(GF_UINT8 addrType, GF_UINT8 addr[]) const;
		virtual bool isMyself(GF_UINT8 addrType, tstring addr) const;
		virtual bool takeover(BLEDevice& from);

	protected:
		IDongle* mDongle;

	protected:
		DeviceConnectionStatus mCnntStatus = DeviceConnectionStatus::Disconnected;
		DevicePosition mPosition = DevicePosition::NotAssigned;
		GF_UINT8	mAddrType = 0;
		GF_UINT8	mAddress[BT_ADDRESS_SIZE];
		tstring		mName;
		GF_UINT8	mRssi = 0;

		GF_UINT16	mHandle = 0;
		GF_UINT16	mConnInt = 0;
		GF_UINT16	mSuperTO = 0;
		GF_UINT16	mSlavelatency = 0;
		GF_UINT16	mMTUsize = 0;

	protected:
		set<gfwPtr<DeviceListener>, WeakPtrComp<DeviceListener>> mListeners;
	};

} // namespace oym
