#pragma once

class Collision
{

public:
	static float PickFromRay(_In_ Mesh* pmesh, _In_ XMVECTOR raypos, _In_ XMVECTOR raydir);
	static bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point);
};

