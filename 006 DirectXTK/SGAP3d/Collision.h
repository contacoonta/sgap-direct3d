#pragma once

class Collision
{

public:
	static bool pickHit(_In_ Mesh* pmesh, _In_ XMVECTOR raypos, _In_ XMVECTOR raydir, _Out_ FLOAT& fdist, _Out_ XMVECTOR& hitpos);
private:
	static bool IntersectTriangle(const XMVECTOR& orig, const XMVECTOR& dir,
									XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2,
									FLOAT& t, FLOAT& u, FLOAT& v);
};

