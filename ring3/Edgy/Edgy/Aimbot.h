#ifndef AIMBOT_H
#define AIMBOT_H
#include "CGeneralEntity.h"
#include "Offsets.h"
#include <d3dx9.h>
#include <random>
#include <math.h>

class Aimbot {
public:
	void Tick(CGeneralEntity);
	float GetFOV(D3DXVECTOR3 &, D3DXVECTOR3 &, D3DXVECTOR3 &, D3DXVECTOR3); //My view angles, my eye position, enemy eye position

private:

	bool InitialBoneSet = false;
	bool DirectionSet = false;

	float timer, aimbotTimer, Direction, xOffset, zOffset;

	float GetRandomFloat(float, float);

	bool IsTargetStillValid(CGeneralEntity);

	void calcAngle(D3DXVECTOR3 &, D3DXVECTOR3 &, D3DXVECTOR3 &);//FOV

	D3DXVECTOR3 CalcAngleRCS(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, float, float);//Normal
	D3DXVECTOR3 VelocityCompensation(D3DXVECTOR3&, D3DXVECTOR3);
	D3DXVECTOR3 SmoothAngle(D3DXVECTOR3 &, D3DXVECTOR3 &, float);
	D3DXVECTOR3 Curve(D3DXVECTOR3, D3DXVECTOR3);

	void SetAngle(D3DXVECTOR3);
	void MakeVector(D3DXVECTOR3 &, D3DXVECTOR3 &);
	void ClampAngle(D3DXVECTOR3 &);

	float get3DDistance(D3DXVECTOR3, D3DXVECTOR3);

	CGeneralEntity GetTarget(CGeneralEntity);

	D3DXVECTOR3 RandomizeBoneSpot(D3DXVECTOR3, float, float);

	bool WorldToScreen(D3DXVECTOR3 &, D3DXVECTOR3 &, CGeneralEntity);

	CGeneralEntity TargetIndex = 0;

	D3DXVECTOR3 AimAngles, SmoothedAngles, RandomizedBone;

	// Credits: dude719
	// Performs a cubic bezier interpolation between four control points,
	// returning the value at the specified time (t ranges 0 to 1).
	// This template implementation can be used to interpolate XMVECTOR,
	// Vector, float, or any other types that define suitable * and + operators.
	template<typename T>
	static T CubicInterpolate(T const& p1, T const& p2, T const& p3, T const& p4, float t)
	{
		return p1 * (1 - t) * (1 - t) * (1 - t) +
			p2 * 3 * t * (1 - t) * (1 - t) +
			p3 * 3 * t * t * (1 - t) +
			p4 * t * t * t;
	}

};

#endif