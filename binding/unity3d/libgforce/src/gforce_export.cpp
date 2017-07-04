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
#ifdef WIN32
#include "../windows/targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "eh.h"
#endif

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

#ifdef WIN32
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
#endif

GF_HANDLE getAddrAsHandle(WPDEVICE device);
SPDEVICE getDeviceFromHandle(GF_HANDLE h);

class ListenerTranslate : public HubListener
{
public:
	ListenerTranslate() {}

	virtual void onScanFinished() override {
		try {
			GF_LOGD("%s: callback fn here. %d", __FUNCTION__, __LINE__);
			for (auto& i : mlc)
			{
				GF_LOGD("%s: callback fn here. i = %p", __FUNCTION__, i.get());
				if (nullptr != i->onScanFinished)
				{
					GF_LOGD("%s: callback fn here. i.fn = %p", __FUNCTION__, i->onScanFinished);
					i->onScanFinished();
				}
			}
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
	}
#endif
	}
	virtual void onStateChanged(HubState state) override {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onStateChanged)
					i->onStateChanged(static_cast<GF_UINT>(state));
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
		}
#endif
	}
	virtual void onDeviceFound(WPDEVICE device) override {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onDeviceFound)
					i->onDeviceFound(getAddrAsHandle(device));
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
		}
#endif
	}
	virtual void onDeviceDiscard(WPDEVICE device) override {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onDeviceDiscard)
					i->onDeviceDiscard(getAddrAsHandle(device));
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
		}
#endif
	}
	virtual void onDeviceConnected(WPDEVICE device) override {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onDeviceConnected)
					i->onDeviceConnected(getAddrAsHandle(device));
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
		}
#endif
	}
	virtual void onDeviceDisconnected(WPDEVICE device, GF_UINT8 reason) override {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onDeviceDisconnected)
					i->onDeviceDisconnected(getAddrAsHandle(device), reason);
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
		}
#endif
	}
	virtual void onOrientationData(WPDEVICE device, const Quaternion& orientation) override {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onOrientationData)
					i->onOrientationData(getAddrAsHandle(device),
					orientation.w(), orientation.x(), orientation.y(), orientation.z());
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
		}
#endif
	}
	virtual void onGestureData(WPDEVICE device, Gesture gest) override {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onGestureData)
					i->onGestureData(getAddrAsHandle(device), static_cast<GF_UINT>(gest));
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
		}
#endif
	}
	virtual void onReCenter(WPDEVICE device) override {
		try {
			for (auto& i : mlc)
				if (nullptr != i->onReCenter)
					i->onReCenter(getAddrAsHandle(device));
		}
#ifdef WIN32
		catch (gForceException e) {
			GF_LOGE("%s: callback fn failed. code = %u, ExceptionCode = %u",
				__FUNCTION__, e.code, e.ptrException->ExceptionRecord->ExceptionCode);
		}
#else
		catch (...) {
			GF_LOGE("%s: callback fn failed.", __FUNCTION__);
		}
#endif
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
			return (left->onScanFinished < right->onScanFinished);
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
#ifdef WIN32
		_set_se_translator(throw_exception);
#endif
	}
};

static RefHelper _ref;


GF_HANDLE getAddrAsHandle(WPDEVICE device)
{
	auto sp = device.lock();
	if (nullptr != sp)
	{
		return sp.get();
	}
	return nullptr;
}

SPDEVICE getDeviceFromHandle(GF_HANDLE h)
{
	// enum devices to find the given
	SPDEVICE ret;
	if (nullptr != _ref.hub)
	{
		function<bool(WPDEVICE)> func = [&ret, h](WPDEVICE device){
			auto sp = device.lock();
			if (nullptr != sp && h == (GF_HANDLE)sp.get())
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
	// renew lt object
	_ref.lt = make_shared<ListenerTranslate>();
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

