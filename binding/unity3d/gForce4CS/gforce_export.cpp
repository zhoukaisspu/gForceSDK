#include "stdafx.h"
#include "gforce_export.h"
#include "gfTypes.h"
#include "Quaternion.h"
#include "HubManagerImpl.h"
#include "HubListener.h"
#include "Hub.h"
#include "Device.h"

using namespace gf;
using namespace std;

#define RETURN_INT_VALUE static_cast<int>(ret)

gfsPtr<Hub> hub = nullptr;

GFORCE4CS_API int hub_init(int comport)
{
	hub = HubManagerImpl::getHubInstanceImpl(_T("Unity3D helper"));
	GF_RET_CODE ret = hub->init(comport);
	return RETURN_INT_VALUE;
	//return 10;
}

GFORCE4CS_API int hub_deinit()
{
	return 0;
}

GFORCE4CS_API int hub_register_listener()
{
	return 0;
}

GFORCE4CS_API int hub_unregister_listener()
{
	return 0;
}

GFORCE4CS_API int hub_start_scan()
{
	return 0;
}

GFORCE4CS_API int hub_stop_scan()
{
	return 0;
}

GFORCE4CS_API int hub_get_num_devices()
{
	return 0;
}

GFORCE4CS_API int hub_get_num_devices_connected()
{
	return 0;
}

GFORCE4CS_API int hub_enum_devices()
{
	return 0;
}

GFORCE4CS_API void hub_set_workmode()
{
}

GFORCE4CS_API int hub_get_workmode()
{
	return 0;
}

GFORCE4CS_API int hub_run()
{
	return 0;
}

GFORCE4CS_API int device_get_addr_type()
{
	return 0;
}

GFORCE4CS_API int device_get_address()
{
	return 0;
}

GFORCE4CS_API int device_get_connection_status()
{
	return 0;
}

GFORCE4CS_API int device_connect()
{
	return 0;
}

GFORCE4CS_API int device_disconnect()
{
	return 0;
}

GFORCE4CS_API int device_cancel_connect()
{
	return 0;
}

