#ifndef __OYM_CALLBACK_H__
#define __OYM_CALLBACK_H__

#include "oym_types.h"

class OYM_CallBack
{
public:
	OYM_CallBack(OYM_LONG eventmask) :mEventMask(eventmask){}
	virtual OYM_STATUS OnDeviceFound(BLE_DEVICE new_device) = 0;
	virtual OYM_STATUS OnScanFinished() = 0;
	virtual OYM_STATUS OnScanResult(OYM_PUINT8 data, OYM_UINT16 length) = 0;
	virtual OYM_STATUS OnConnect(OYM_PUINT8 data, OYM_UINT16 length) = 0;

	virtual OYM_STATUS OnEvent(OYM_UINT32 event, OYM_PUINT8 data, OYM_UINT16 length) = 0;

	OYM_LONG GetEventMask()
	{
		return mEventMask;
	}

private:
	OYM_LONG mEventMask;//which event is intent to process.
};
#endif