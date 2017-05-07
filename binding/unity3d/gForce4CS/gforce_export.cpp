#include "stdafx.h"

#define TAG "gForce4CS"

#include "gforce_export.h"
#include "gfTypes.h"
#include "Quaternion.h"
#include "HubManagerImpl.h"
#include "HubListener.h"
#include "Hub.h"
#include "Device.h"
#include "Utils.h"
#include "LogUtils.h"
#include <set>

using namespace gf;
using namespace std;

#define RETURN_UINT_VALUE_FROM_RET_CODE static_cast<GF_UINT>(ret)
#define RETURN_UINT_VALUE_FROM_WORKMODE static_cast<GF_UINT>(ret)
#define RETURN_UINT_VALUE_FROM_HUBSTATE static_cast<GF_UINT>(ret)
#define RETURN_UINT_VALUE_FROM_CONNSTAT static_cast<GF_UINT>(ret)

class gForceException
{
public:
	gForceException(unsigned int _code, EXCEPTION_POINTERS* _ptr)
		: code(_code), ptrException(_ptr) {}
	~gForceException() {}
	unsigned int code;
	EXCEPTION_POINTERS* ptrException;
};

void throw_exception(unsigned int code, EXCEPTION_POINTERS* ptr)
{
	throw gForceException(code, ptr); // throw exception 
}

HANDLE getAddrAsHandle(WPDEVICE device);
SPDEVICE getDeviceFromHandle(HANDLE h);

class ListenerTranslate : public HubListener
{
public:
	ListenerTranslate() {}

	virtual void onScanfinished() {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onScanfinished)
					i->onScanfinished();
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}
	virtual void onStateChanged(HubState state) {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onStateChanged)
					i->onStateChanged(static_cast<GF_UINT>(state));
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}
	virtual void onDeviceFound(WPDEVICE device) {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onDeviceFound)
					i->onDeviceFound(getAddrAsHandle(device));
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}
	virtual void onDeviceDiscard(WPDEVICE device) {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onDeviceDiscard)
					i->onDeviceDiscard(getAddrAsHandle(device));
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}
	virtual void onDeviceConnected(WPDEVICE device) {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onDeviceConnected)
					i->onDeviceConnected(getAddrAsHandle(device));
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}
	virtual void onDeviceDisconnected(WPDEVICE device, GF_UINT8 reason) {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onDeviceDisconnected)
					i->onDeviceDisconnected(getAddrAsHandle(device), reason);
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}
	virtual void onOrientationData(WPDEVICE device, const Quaternion& orientation) {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onOrientationData)
					i->onOrientationData(getAddrAsHandle(device),
					orientation.w(), orientation.x(), orientation.y(), orientation.z());
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}
	virtual void onGestureData(WPDEVICE device, Gesture gest) {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onGestureData)
					i->onGestureData(getAddrAsHandle(device), static_cast<GF_UINT>(gest));
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}
	virtual void onReCenter(WPDEVICE device) {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onReCenter)
					i->onReCenter(getAddrAsHandle(device));
		}
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
	}

	bool addCallbacks(gfsPtr<ListenerCalls> lc)
	{
		auto ret = mlc.insert(lc);
		return ret.second;
	}
	bool removeCallbacks(gfsPtr<ListenerCalls> lc)
	{
		auto ret = mlc.erase(lc);
		return ret == 0 ? false : true;
	}
	GF_SIZE getNumCallbacks()
	{
		return mlc.size();
	}

private:
	// see comment in hub_register_listener
	template <class _T> struct ListenerComp {
		bool operator ()(const gfsPtr<_T>& left, const gfsPtr<_T>& right) const {
			return (left->onScanfinished < right->onScanfinished);
		}
	};
	set<gfsPtr<ListenerCalls>, ListenerComp<ListenerCalls>> mlc;
};


class RefHelper
{
public:
	gfsPtr<Hub> hub = nullptr;
	gfsPtr<ListenerTranslate> lt = make_shared<ListenerTranslate>();

	RefHelper()
	{
		_set_se_translator(throw_exception);
	}
};

static RefHelper _ref;


HANDLE getAddrAsHandle(WPDEVICE device)
{
	auto sp = device.lock();
	if (nullptr != sp)
	{
		return sp.get();
	}
	return nullptr;
}

SPDEVICE getDeviceFromHandle(HANDLE h)
{
	// enum devices to find the given
	SPDEVICE ret;
	if (nullptr != _ref.hub)
	{
		function<bool(WPDEVICE)> func = [&ret, h](WPDEVICE device){
			auto sp = device.lock();
			if (nullptr != sp && h == (HANDLE)sp.get())
			{
				ret = sp;
				return false;
			}
			return true;
		};
		_ref.hub->enumDevices(func, false);
	}
	return ret;
}

GFORCE4CS_API void gf_set_log_method(FunClientLog logMethod)
{
	log_set_output_function(logMethod);
}

GFORCE4CS_API GF_UINT hub_instance(const GF_PCHAR identifier)
{
	string id(identifier);
	_ref.hub = HubManagerImpl::getHubInstanceImpl(utils::totstring(id));
	GF_RET_CODE ret = GF_RET_CODE::GF_SUCCESS;
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT hub_init(GF_UINT comport)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr != _ref.hub)
		ret = _ref.hub->init(comport);
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT hub_deinit()
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr != _ref.hub)
		ret = _ref.hub->deinit();
	_ref.hub = nullptr;
	_ref.lt = nullptr;
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT hub_set_workmode(GF_UINT newMode)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr != _ref.hub)
	{
		ret = GF_RET_CODE::GF_SUCCESS;
		switch (newMode)
		{
		case static_cast<GF_UINT>(WorkMode::Freerun) :
			_ref.hub->setWorkMode(WorkMode::Freerun);
			break;
		case static_cast<GF_UINT>(WorkMode::Polling) :
			_ref.hub->setWorkMode(WorkMode::Polling);
			break;
		default:
			ret = GF_RET_CODE::GF_ERROR_BAD_PARAM;
		}
	}
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT hub_get_workmode()
{
	WorkMode ret = WorkMode::Freerun;
	if (nullptr != _ref.hub)
		ret = _ref.hub->getWorkMode();
	return RETURN_UINT_VALUE_FROM_WORKMODE;
}

GFORCE4CS_API GF_UINT hub_get_status()
{
	HubState ret = HubState::Unknown;
	if (nullptr != _ref.hub)
		ret = _ref.hub->getState();
	return RETURN_UINT_VALUE_FROM_HUBSTATE;
}

GFORCE4CS_API GF_UINT hub_register_listener(ListenerCalls* lis)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_PARAM;
	if (nullptr == lis)
	{
		GF_LOGE("Invalid parameter in registering listener.");
		return RETURN_UINT_VALUE_FROM_RET_CODE;
	}

	// NOTE:
	// 1. because the struct ListenerCalls in C# is not spinable, it's not possible
	//    keeping lis pointer in native code, we have to use new instance and save
	//    the content in it (the function pointers).
	// 2. because of reason 1, comparing between LIstenerCalls objects becomes comparing
	//    function pointers of them. currently we compare only the first func pointer.
	//    see @struct ListenerComp
	gfsPtr<ListenerCalls> plc = make_shared<ListenerCalls>(*lis);
	if (!_ref.lt->addCallbacks(plc))
	{
		GF_LOGE("Failed to add listener.");
		return RETURN_UINT_VALUE_FROM_RET_CODE;
	}
	//_ref.lt->onReCenter(WPDEVICE()); // for test only
	if (1 < _ref.lt->getNumCallbacks())
	{
		GF_LOGD("registering listener OK.");
		ret = GF_RET_CODE::GF_SUCCESS;
	}
	else
	{
		GF_LOGD("Register delegate listener.");
		ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
		if (nullptr != _ref.hub)
			ret = _ref.hub->registerListener(static_pointer_cast<HubListener>(_ref.lt));
	}
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT hub_unregister_listener(ListenerCalls* lis)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_PARAM;
	if (nullptr == lis)
	{
		GF_LOGE("Invalid parameter in registering listener.");
		return RETURN_UINT_VALUE_FROM_RET_CODE;
	}

	gfsPtr<ListenerCalls> plc = make_shared<ListenerCalls>(*lis);
	if (!_ref.lt->removeCallbacks(plc))
	{
		GF_LOGE("Failed to unregister listener.");
		return RETURN_UINT_VALUE_FROM_RET_CODE;
	}
	if (0 < _ref.lt->getNumCallbacks())
	{
		ret = GF_RET_CODE::GF_SUCCESS;
	}
	else
	{
		GF_LOGD("Unregister delegate listener.");
		ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
		if (nullptr != _ref.hub)
			ret = _ref.hub->unRegisterListener(static_pointer_cast<HubListener>(_ref.lt));
	}
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT hub_start_scan()
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr != _ref.hub)
		ret = _ref.hub->startScan();
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT hub_stop_scan()
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr != _ref.hub)
		ret = _ref.hub->stopScan();
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_SIZE hub_get_num_devices(bool bConnectedOnly)
{
	if (nullptr != _ref.hub)
		return _ref.hub->getNumOfDevices(bConnectedOnly);
	else
		return 0;
}

GFORCE4CS_API GF_UINT hub_enum_devices(FunEnumDevice fn, bool bConnectedOnly)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr != _ref.hub)
	{
		function<bool(WPDEVICE)> func = [fn](WPDEVICE device){
			return fn(getAddrAsHandle(device));
		};
		_ref.hub->enumDevices(func, bConnectedOnly);
		ret = GF_RET_CODE::GF_SUCCESS;
	}
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT hub_run(GF_UINT ms)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr != _ref.hub)
		ret = _ref.hub->run(ms);
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT device_get_addr_type(GF_HANDLE device)
{
	GF_UINT ret = (GF_UINT)-1;
	SPDEVICE sp = getDeviceFromHandle(device);
	if (nullptr != sp)
		ret = sp->getAddrType();
	return ret;
}

GFORCE4CS_API GF_UINT device_get_address(GF_HANDLE device, GF_PCHAR addr, GF_SIZE buflen)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr == addr || buflen == 0)
	{
		ret = GF_RET_CODE::GF_ERROR_BAD_PARAM;
		return RETURN_UINT_VALUE_FROM_RET_CODE;
	}
	SPDEVICE sp = getDeviceFromHandle(device);
	if (nullptr != sp)
	{
		memset(addr, 0, buflen);
		string strAddr = utils::tostring(sp->getAddress());
		memcpy(addr, strAddr.c_str(), min(buflen - 1, strAddr.length()));
		ret = GF_RET_CODE::GF_SUCCESS;
	}
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT device_get_name(GF_HANDLE device, GF_PCHAR name, GF_SIZE buflen)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (nullptr == name || buflen == 0)
	{
		ret = GF_RET_CODE::GF_ERROR_BAD_PARAM;
		return RETURN_UINT_VALUE_FROM_RET_CODE;
	}
	SPDEVICE sp = getDeviceFromHandle(device);
	if (nullptr != sp)
	{
		memset(name, 0, buflen);
		string strName = utils::tostring(sp->getName());
		memcpy(name, strName.c_str(), min(buflen - 1, strName.length()));
		ret = GF_RET_CODE::GF_SUCCESS;
	}
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT device_get_connection_status(GF_HANDLE device)
{
	DeviceConnectionStatus ret = DeviceConnectionStatus::Disconnected;
	SPDEVICE sp = getDeviceFromHandle(device);
	if (nullptr != sp)
		ret = sp->getConnectionStatus();
	return RETURN_UINT_VALUE_FROM_CONNSTAT;
}

GFORCE4CS_API GF_UINT device_connect(GF_HANDLE device)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	SPDEVICE sp = getDeviceFromHandle(device);
	if (nullptr != sp)
		ret = sp->connect();
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

GFORCE4CS_API GF_UINT device_disconnect(GF_HANDLE device)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	SPDEVICE sp = getDeviceFromHandle(device);
	if (nullptr != sp)
		ret = sp->disconnect();
	return RETURN_UINT_VALUE_FROM_RET_CODE;
}

