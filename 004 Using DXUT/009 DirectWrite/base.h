#pragma once

#define SINGLETON(x)	public: static x& instance() { static x inst; return inst; } \
						private: x(); x(const x&); ~x();

