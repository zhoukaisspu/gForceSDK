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
// define gForce device

#pragma once

#include "BLEDevice.h"

//#define DEBUG_ANALYSE_PACKAGE_LOST

#if (defined(DEBUG) || defined(_DEBUG)) && defined(DEBUG_ANALYSE_PACKAGE_LOST)
#include <chrono>
#include <atomic>
#include <thread>
#endif

namespace gf
{
	class BLEHub;

#define EVENT_MASK				0x7F
#define PCKID_FLAG_MASK			0x01
#define EVENT_RECENTER_MASK		0x01
#define INVALID_PACKAGE_ID		0xFFFF

	enum {
		EVENT_QUATERNION = 0x02,
		EVENT_GESTURE = 0x0F,
		EVENT_STATUS = 0x14
	};

	class GForceDevice : public BLEDevice
	{
	public:
		GForceDevice(IHub& hub, const GF_BLEDevice& bleDev) : BLEDevice(hub, bleDev) {}
		virtual ~GForceDevice() {}

	protected:
		virtual void onData(GF_UINT8 length, GF_PUINT8 data) override;

	private:
		void onQuaternion(GF_UINT8 length, GF_PUINT8 data);
		void onGesture(GF_UINT8 length, GF_PUINT8 data);
		void onStatus(GF_UINT8 length, GF_PUINT8 data);

	private:
		GF_UINT16 mPackageId = INVALID_PACKAGE_ID;

#if (defined(DEBUG) || defined(_DEBUG)) && defined(DEBUG_ANALYSE_PACKAGE_LOST)
	private:
		static atomic<GF_UINT32> dataCnt;
		static atomic<GF_UINT32> lastUpdated;
		static void timefun();
		static thread dbgThread;
#endif
	};

} // namespace gf
