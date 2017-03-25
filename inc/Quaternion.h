#pragma once

#include <sstream>

namespace gf
{

	template <class _Ty>
	class Quaternion
	{
	public:
		Quaternion() {}
		Quaternion(_Ty w, _Ty x, _Ty y, _Ty z)
			: mW(w)
			, mX(x)
			, mY(y)
			, mZ(z)
		{
		}
		_Ty w() const { return mW; }
		_Ty x() const { return mX; }
		_Ty y() const { return mY; }
		_Ty z() const { return mZ; }

		string toString() const
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
