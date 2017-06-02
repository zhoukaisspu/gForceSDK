#pragma once

#ifdef WIN32

#ifdef GFORCE4CS_EXPORTS
#define GFORCE4CS_API __declspec(dllexport)
#else
#define GFORCE4CS_API __declspec(dllimport)
#endif

#else // #ifdef WIN32

#ifdef GFORCE4CS_EXPORTS
#define GFORCE4CS_API extern "C" __attribute__ ((visibility ("default")))
#else
#define GFORCE4CS_API __attribute__ ((visibility ("hidden")))
#endif

#endif


#include "gfTypes.h"

typedef bool(*FunEnumDevice)(gf::GF_HANDLE);
typedef void(*FunClientLog)(gf::GF_UINT, const gf::GF_PCHAR);

#pragma pack(push,1)
struct ListenerCalls
{
	void(*onScanFinished)() = nullptr;
	void(*onStateChanged)(gf::GF_UINT state) = nullptr;
	void(*onDeviceFound)(gf::GF_HANDLE device) = nullptr;
	void(*onDeviceDiscard)(gf::GF_HANDLE device) = nullptr;
	void(*onDeviceConnected)(gf::GF_HANDLE device) = nullptr;
	void(*onDeviceDisconnected)(gf::GF_HANDLE device, gf::GF_UINT reason) = nullptr;
	void(*onOrientationData)(gf::GF_HANDLE device, float w, float x, float y, float z) = nullptr;
	void(*onGestureData)(gf::GF_HANDLE device, gf::GF_UINT gest) = nullptr;
	void(*onReCenter)(gf::GF_HANDLE device) = nullptr;
};
#pragma pack(pop)

// Log from client
GFORCE4CS_API void gf_set_log_method(FunClientLog);
// hub
GFORCE4CS_API gf::GF_UINT hub_instance(const gf::GF_PCHAR identifier);
GFORCE4CS_API gf::GF_UINT hub_init(gf::GF_UINT comport);
GFORCE4CS_API gf::GF_UINT hub_deinit();
GFORCE4CS_API gf::GF_UINT hub_set_workmode(gf::GF_UINT newMode);
GFORCE4CS_API gf::GF_UINT hub_get_workmode();
GFORCE4CS_API gf::GF_UINT hub_get_status();
GFORCE4CS_API gf::GF_UINT hub_register_listener(ListenerCalls* lis);
GFORCE4CS_API gf::GF_UINT hub_unregister_listener(ListenerCalls* lis);
GFORCE4CS_API gf::GF_UINT hub_start_scan();
GFORCE4CS_API gf::GF_UINT hub_stop_scan();
GFORCE4CS_API gf::GF_SIZE hub_get_num_devices(bool bConnectedOnly);
GFORCE4CS_API gf::GF_UINT hub_enum_devices(FunEnumDevice fn, bool bConnectedOnly);
GFORCE4CS_API gf::GF_UINT hub_run(gf::GF_UINT ms);

// device
GFORCE4CS_API gf::GF_UINT device_get_addr_type(gf::GF_HANDLE device);
GFORCE4CS_API gf::GF_UINT device_get_address(gf::GF_HANDLE device, gf::GF_PCHAR addr, gf::GF_SIZE buflen);
GFORCE4CS_API gf::GF_UINT device_get_name(gf::GF_HANDLE device, gf::GF_PCHAR name, gf::GF_SIZE buflen);
GFORCE4CS_API gf::GF_UINT device_get_connection_status(gf::GF_HANDLE device);
GFORCE4CS_API gf::GF_UINT device_connect(gf::GF_HANDLE device);
GFORCE4CS_API gf::GF_UINT device_disconnect(gf::GF_HANDLE device);

