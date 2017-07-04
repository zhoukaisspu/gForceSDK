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
#include "GForceDevice.h"
#include "Utils.h"
#include "LogUtils.h"
#include "Quaternion.h"

using namespace gf;

#if (defined(DEBUG) || defined(_DEBUG)) && defined(DEBUG_ANALYSE_PACKAGE_LOST)
atomic<GF_UINT32> GForceDevice::dataCnt = 0;
atomic<GF_UINT32> GForceDevice::lastUpdated = 0;
thread GForceDevice::dbgThread;
void GForceDevice::timefun()
{
	bool loop = true;
	while (loop)
	{
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		if (lastUpdated < dataCnt)
		{
			GF_LOGD("gForce data package rate: %d", dataCnt - lastUpdated);
			lastUpdated = dataCnt.load();
		}
	}
}
#endif

void GForceDevice::onData(GF_UINT8 length, GF_PUINT8 data)
{
	if (length <= 1)
		return;

#if (defined(DEBUG) || defined(_DEBUG)) && defined(DEBUG_ANALYSE_PACKAGE_LOST)
	if (!dbgThread.joinable())
		dbgThread = thread(GForceDevice::timefun);
	dataCnt++;
#endif
	GF_UINT8 evtType = data[0] & EVENT_MASK;
	GF_UINT8 packageIdFlag = (data[0] >> 7) & PCKID_FLAG_MASK;
	GF_UINT8 payloadLength = data[1];
	GF_UINT16 currPackageId = INVALID_PACKAGE_ID;
	GF_PUINT8 payload = nullptr;
	
	if (packageIdFlag != 0)
	{
		currPackageId = data[2];
		payload = &data[3];
		--payloadLength;

		lock_guard<mutex> lock(mMutex);
		if (mPackageId == INVALID_PACKAGE_ID)
		{
			mPackageId = currPackageId;
		}
		else
		{
			++mPackageId;
			if (mPackageId > 0xFF)
				mPackageId = 0;
			if (mPackageId != currPackageId)
			{
#if (defined(DEBUG) || defined(_DEBUG)) && defined(DEBUG_ANALYSE_PACKAGE_LOST)
				GF_LOGE("%s:%s: package id error. id supposed is %u, but now is %u, gap is %u", __FUNCTION__,
					utils::tostring(getName()).c_str(), mPackageId, currPackageId, (GF_UINT8)(currPackageId - mPackageId));
#endif
				mPackageId = currPackageId;
			}
		}
	}
	else
	{
		payload = &data[2];
	}

	switch (evtType)
	{
	case EVENT_QUATERNION:
		onQuaternion(payloadLength, payload);
		break;
	case EVENT_GESTURE:
		onGesture(payloadLength, payload);
		break;
	case EVENT_STATUS:
		onStatus(payloadLength, payload);
		break;
	default:
		GF_LOGE("%s: unknown event ID: %2.2X", __FUNCTION__, evtType);
		;
	}
}

void GForceDevice::onQuaternion(GF_UINT8 length, GF_PUINT8 data)
{
	length; data;
	if (length < 16)
	{
		GF_LOGD("%s, length: %u, data insufficient.", __FUNCTION__, length);
		return;
	}
	GF_FLOAT f[4];
	// prevent align issue
	memcpy(f, data,  sizeof(f));
	Quaternion q(f[0], f[1], f[2], f[3]);
	// GF_LOGD("Device: %s, Quaternion: %s", utils::tostring(getName()).c_str(), q.toString().c_str());

	mHub.notifyOrientationData(*this, q);
}

void GForceDevice::onGesture(GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD("%s, length: %u", __FUNCTION__, length);
	if (length > 0)
	{
		Gesture gesture;
		switch (data[0])
		{
		case static_cast<GF_UINT8>(Gesture::Relax) :
			gesture = Gesture::Relax;
			break;
		case static_cast<GF_UINT8>(Gesture::Fist) :
			gesture = Gesture::Fist;
			break;
		case static_cast<GF_UINT8>(Gesture::SpreadFingers) :
			gesture = Gesture::SpreadFingers;
			break;
		case static_cast<GF_UINT8>(Gesture::WaveTowardPalm) :
			gesture = Gesture::WaveTowardPalm;
			break;
		case static_cast<GF_UINT8>(Gesture::WaveBackwardPalm) :
			gesture = Gesture::WaveBackwardPalm;
			break;
		case static_cast<GF_UINT8>(Gesture::TuckFingers) :
			gesture = Gesture::TuckFingers;
			break;
		case static_cast<GF_UINT8>(Gesture::Shoot) :
			gesture = Gesture::Shoot;
			break;
		case static_cast<GF_UINT8>(Gesture::Undefined) :
		default:
			gesture = Gesture::Undefined;
		}
		mHub.notifyGestureData(*this, gesture);
	}
}

void GForceDevice::onStatus(GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD("%s, length: %u", __FUNCTION__, length);
	if (length > 0)
	{
		GF_UINT8 status = data[0] & EVENT_RECENTER_MASK;
		if (1 == status)
		{
			mHub.notifyReCenter(*this);
		}
	}
}
