#pragma once

#pragma warning(disable: 4127) // warning C4127: ���ǽ��� ����Դϴ�.

#if defined(__cplusplus)
#define SGAP_BEGIN	namespace sgap {
#define SGAP_END		}
#endif

SGAP_BEGIN

typedef unsigned int UINT;

#define UNREFERENCED_PARAMETER(P)	(P)
#define SINGLETON(x) public : static x& instance() { static x inst; return inst; }

SGAP_END