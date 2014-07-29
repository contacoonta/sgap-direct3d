#ifndef _BASEDEF_H_
#define _BASEDEF_H_

#pragma warning(disable: 4127) // warning C4127: 조건식이 상수입니다.

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <xnamath.h>
//#include <d3dx10math.h>

#if defined(__cplusplus)
#define SGAP_BEGIN	namespace sgap {
#define SGAP_END		}
#endif

SGAP_BEGIN

typedef unsigned int UINT;

#define UNREFERENCED_PARAMETER(P)	(P)
#define SINGLETON(x) public : static x& instance() { static x inst; return inst; }

SGAP_END

#endif