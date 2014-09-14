#pragma once


#if defined(__cplusplus)

#define SGAP_BEGIN	namespace sgap {
#define SGAP_END		}

#endif

#define SINGLETON(x)	public : static x& instance() \
						{ static x inst; return inst; } \
						private: x(); x(const x&); ~x();