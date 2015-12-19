#pragma once

namespace Neptune
{
	// Matrix with 3 rows and 3 columns
	template <typename T> class Mat3x3;
	typedef Mat3x3<float> Mat3;
}

// Platform-specific Implementation
#include "Math/Vectors/Mat3Implementation.h" 