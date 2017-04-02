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

#include <sstream>

namespace gf
{

	/// \class Quaternion
	/// \brief
	///             Data type of quaternion
	///
	template <class _Ty>
	class Quaternion
	{
	public:
		/// \brief Default constructor
		///
		Quaternion() {}

		/// \brief Constructor with 4 elements
		///
		Quaternion(_Ty w, _Ty x, _Ty y, _Ty z)
			: mW(w)
			, mX(x)
			, mY(y)
			, mZ(z)
		{
		}

		/// \brief Get W
		///
		/// \return The W value
		_Ty w() const { return mW; }

		/// \brief Get X
		///
		/// \return The X value
		_Ty x() const { return mX; }

		/// \brief Get Y
		///
		/// \return The Y value
		_Ty y() const { return mY; }

		/// \brief Get Z
		///
		/// \return The Z value
		_Ty z() const { return mZ; }

		/// \brief Get a human-readable value
		///
		/// \return Value in text for human read convenience
		std::string toString() const
		{
			std::ostringstream stm;
			stm << "w: " << mW << ", x: " << mX << ", y: " << mY << ", z: " << mZ;
			return stm.str();
		}

	private:
		_Ty mW = 1;
		_Ty mX = 0;
		_Ty mY = 0;
		_Ty mZ = 0;
	};

} // namespace gf
