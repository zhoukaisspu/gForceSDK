/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
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
