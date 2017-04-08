#pragma once

#ifdef GFORCE4CS_EXPORTS
#define GFORCE4CS_API __declspec(dllexport)
#else
#define GFORCE4CS_API __declspec(dllimport)
#endif


typedef int GF_HANDLE;


// hub
GFORCE4CS_API int hub_init(int comport);
GFORCE4CS_API int hub_deinit();
GFORCE4CS_API int hub_register_listener();
GFORCE4CS_API int hub_unregister_listener();
GFORCE4CS_API int hub_start_scan();
GFORCE4CS_API int hub_stop_scan();
GFORCE4CS_API int hub_get_num_devices();
GFORCE4CS_API int hub_get_num_devices_connected();
GFORCE4CS_API int hub_enum_devices();
GFORCE4CS_API void hub_set_workmode();
GFORCE4CS_API int hub_get_workmode();
GFORCE4CS_API int hub_run();

// device
GFORCE4CS_API int device_get_addr_type();
GFORCE4CS_API int device_get_address();
GFORCE4CS_API int device_get_connection_status();
GFORCE4CS_API int device_connect();
GFORCE4CS_API int device_disconnect();
GFORCE4CS_API int device_cancel_connect();

// quaternion
// w,x,y,z
