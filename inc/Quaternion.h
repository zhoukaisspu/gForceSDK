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
/*!
 * \file Quaternion.h
 * \brief Quaternion data type
 *
 * \version 0.1
 * \date 2017.4.3
 */
#pragma once

#include <cmath>
#include <sstream>

namespace gf
{

	/// \class Euler
	/// \brief
	///             Data type of Eulerian angle
	///
	class Euler
	{
	public:
		/// \brief Default constructor
		///
		Euler() {}

		/// \brief Constructor with 3 elements
		///
		Euler(float pitch, float roll, float yaw)
			: mPitch(pitch)
			, mRoll(roll)
			, mYaw(yaw)
		{
		}

		/// \brief Get pitch
		///
		/// \return The pitch value
		float pitch() const { return mPitch; }

		/// \brief Get roll
		///
		/// \return The roll value
		float roll() const { return mRoll; }

		/// \brief Get yaw
		///
		/// \return The yaw value
		float yaw() const { return mYaw; }

		/// \brief Get a human-readable value
		///
		/// \return Value in text for human read convenience
		std::string toString() const
		{
			std::ostringstream stm;
			stm << "pitch: " << mPitch << ", roll: " << mRoll << ", yaw: " << mYaw;
			return stm.str();
		}

	private:
		float mPitch = 0;
		float mRoll = 0;
		float mYaw = 0;
	};

	/// \class Quaternion
	/// \brief
	///             Data type of quaternion
	///
	class Quaternion
	{
	public:
		/// \brief Default constructor
		///
		Quaternion() {}

		/// \brief Constructor with 4 elements
		///
		Quaternion(float w, float x, float y, float z)
			: mW(w)
			, mX(x)
			, mY(y)
			, mZ(z)
		{
		}

		/// \brief Get W
		///
		/// \return The W value
		float w() const { return mW; }

		/// \brief Get X
		///
		/// \return The X value
		float x() const { return mX; }

		/// \brief Get Y
		///
		/// \return The Y value
		float y() const { return mY; }

		/// \brief Get Z
		///
		/// \return The Z value
		float z() const { return mZ; }

		/// \brief Get a human-readable value
		///
		/// \return Value in text for human read convenience
		std::string toString() const
		{
			std::ostringstream stm;
			stm << "w: " << mW << ", x: " << mX << ", y: " << mY << ", z: " << mZ;
			return stm.str();
		}

	public:
		/// \brief Convert quaternion to Eulerian angle
		///
		/// \return The converted Eulerian angle
		Euler toEuler() const
		{
			static const float PI = 3.14159265f;
			float pitch = 0, roll = 0, yaw = 0;

			long t1, t2, t3;
			long q00, q01, q02, q03, q11, q12, q13, q22, q23, q33;
			long quat0, quat1, quat2, quat3;
			quat0 = valueToLong(mW);
			quat1 = valueToLong(mX);
			quat2 = valueToLong(mY);
			quat3 = valueToLong(mZ);
			q00 = multiplyShift29(quat0, quat0);
			q01 = multiplyShift29(quat0, quat1);
			q02 = multiplyShift29(quat0, quat2);
			q03 = multiplyShift29(quat0, quat3);
			q11 = multiplyShift29(quat1, quat1);
			q12 = multiplyShift29(quat1, quat2);
			q13 = multiplyShift29(quat1, quat3);
			q22 = multiplyShift29(quat2, quat2);
			q23 = multiplyShift29(quat2, quat3);
			q33 = multiplyShift29(quat3, quat3);

			/* X component of the Ybody axis in World frame */
			t1 = q12 - q03;

			/* Y component of the Ybody axis in World frame */
			t2 = q22 + q00 - (1L << 30);
			yaw = -std::atan2((float)t1, (float)t2) * 180.f / (float)PI;

			/* Z component of the Ybody axis in World frame */
			t3 = q23 + q01;
			pitch = std::atan2((float)t3,
				std::sqrt((float)t1 * t1 + (float)t2 * t2)) * 180.f / PI;
			/* Z component of the Zbody axis in World frame */
			t2 = q33 + q00 - (1L << 30);
			if (t2 < 0) {
				if (pitch >= 0) {
					pitch = 180.f - (pitch);
				}
				else {
					pitch = -180.f - (pitch);
				}
			}

			/* X component of the Xbody axis in World frame */
			t1 = q11 + q00 - (1L << 30);
			/* Y component of the Xbody axis in World frame */
			t2 = q12 + q03;
			/* Z component of the Xbody axis in World frame */
			t3 = q13 - q02;

			roll = std::atan2(((float)(q33 + q00 - (1L << 30))),
				(float)(q13 - q02)) * 180.f / PI - 90;
			if (roll >= 90) {
				roll = 180 - roll;
			}

			if (roll < -90) {
				roll = -180 - roll;
			}

			return Euler(pitch, roll, yaw);
		}

	private:
		float mW = 1;
		float mX = 0;
		float mY = 0;
		float mZ = 0;

	private:
		long valueToLong(float q) const
		{
			return (long)(q * (1L << 30));
		}

		long multiplyShift29(long a, long b) const
		{
			return (long)((float)a * b / (1L << 29));
		}
	};

} // namespace gf
