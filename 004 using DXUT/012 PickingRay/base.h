#pragma once

//#define _XM_NO_INTRINSICS_

#define SINGLETON(x)	public: static x& instance() { static x inst; return inst; } \
						private: x(); x(const x&); ~x();

