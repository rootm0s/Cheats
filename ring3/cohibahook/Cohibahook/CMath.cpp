#include "main.h"
#define M_PI	3.14159265358979323846f

CMath g_Math;

VOID CMath::sinCos(float radians, PFLOAT sine, PFLOAT cosine)
{
	__asm
	{
		fld dword ptr[radians]
			fsincos
			mov edx, dword ptr[cosine]
			mov eax, dword ptr[sine]
			fstp dword ptr[edx]
			fstp dword ptr[eax]
	}
}

VOID CMath::angleVectors(PFLOAT angles, PFLOAT f, PFLOAT r, PFLOAT u)
{
	float sp, sy, sr, cp, cy, cr;

	sinCos(DEG2RAD(angles[0]), &sp, &cp);
	sinCos(DEG2RAD(angles[1]), &sy, &cy);
	sinCos(DEG2RAD(angles[2]), &sr, &cr);

	f[0] = cp * cy;
	f[1] = cp * sy;
	f[2] = -sp;

	r[0] = -1.0f * sr * sp * cy + -1.0f * cr * -sy;
	r[1] = -1.0f * sr * sp * sy + -1.0f * cr * cy;
	r[2] = -1.0f * sr * cp;

	u[0] = cr * sp * cy + -sr * -sy;
	u[1] = cr * sp * sy + -sr * cy;
	u[2] = cr * cp;
}

float CMath::sseSqrt(float x)
{
	float root = 0.0f;

	__asm
	{
		sqrtss xmm0, x
			movss root, xmm0
	}

	return root;
}

void CMath::vectorAngles(float* forward, float* angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		float len2d = sseSqrt(square(forward[0]) + square(forward[1]));

		angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		if (angles[0] < 0.0f) angles[0] += 360.0f;
		if (angles[1] < 0.0f) angles[1] += 360.0f;
	}

	angles[2] = 0.0f;
}

void CMath::vectorAnglesVec(Vector forward, Vector &angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		float len2d = sseSqrt(square(forward[0]) + square(forward[1]));

		angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		if (angles[0] < 0.0f) angles[0] += 360.0f;
		if (angles[1] < 0.0f) angles[1] += 360.0f;
	}

	angles[2] = 0.0f;
}

void CMath::VectorAngles3D(const Vector&vecForward, Vector&vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}

float deg2rad(float deg) {
	return (deg * M_PI / 180);
}

float CMath::DotProduct(const Vector &v1, const float *v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void CMath::VectorRotate(const Vector &in1, const Vector &in2, Vector &out)
{
	matrix3x4_t matRotate;

	AngleMatrix(in2, matRotate);

	out[0] = DotProduct(in1, matRotate[0]);
	out[1] = DotProduct(in1, matRotate[1]);
	out[2] = DotProduct(in1, matRotate[2]);
}

void CMath::AngleMatrix(const Vector &angles, matrix3x4_t &matrix)
{
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = angles.x * (6.283185f / 360);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles.y * (6.283185f / 360);
	sy = sin(angle);
	cy = cos(angle);

	angle = angles.z * (6.283185f / 360);
	sr = sin(angle);
	cr = cos(angle);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;

	matrix[2][0] = -sp;
	matrix[0][1] = sr*sp*cy + cr*-sy;

	matrix[1][1] = sr*sp*sy + cr*cy;
	matrix[2][1] = sr*cp;

	matrix[0][2] = (cr*sp*cy + -sr*-sy);
	matrix[1][2] = (cr*sp*sy + -sr*cy);

	matrix[2][2] = cr*cp;

	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}

VOID CMath::VectorTransform(const Vector in1, const matrix3x4_t& in2, Vector& out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

bool CMath::WorldToScreen( const Vector& vecOrigin, Vector& vecScreen )
{
	return ( g_pDebugOverlay->ScreenPosition( vecOrigin, vecScreen ) != 1 );
}

CMath* CMath::Singleton( void )
{
	static CMath* pMath = NULL;

	if( !pMath )
		pMath = new CMath();

	return pMath;
}
CMath* g_pMath = CMath::Singleton();