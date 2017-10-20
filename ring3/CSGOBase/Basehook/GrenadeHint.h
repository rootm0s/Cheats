#pragma once
#include "DllMain.h"

class CCSGrenadeHint
{
public:

private:

	int CCSGrenadeHint::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);
	void CCSGrenadeHint::Tick(CTickVars& vars);
};
extern CCSGrenadeHint g_GrenadeHint;