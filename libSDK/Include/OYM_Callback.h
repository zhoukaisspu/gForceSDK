#ifndef __OYM_CALLBACK_H__
#define __OYM_CALLBACK_H__

#include "oym_types.h"

class OYM_CallBack
{
public:
	OYM_CallBack(OYM_LONG eventmask, OYM_UINT8 index) :mEventMask(eventmask), mIndex(index){}
	virtual OYM_STATUS OnDeviceFound(BLE_DEVICE new_device) = 0;
	virtual OYM_STATUS OnConnect(OYM_PUINT8 data, OYM_UINT16 length) = 0;

	virtual OYM_STATUS OnEvent(OYM_UINT32 event, OYM_PUINT8 data, OYM_UINT16 length) = 0;

	OYM_LONG GetEventMask()
	{
		return mEventMask;
	}
	
	OYM_UINT8 GetIndex()
	{
		return mIndex;
	}

private:
	OYM_LONG mEventMask;//which event is intent to process.
	OYM_UINT8 mIndex;
};
#endif