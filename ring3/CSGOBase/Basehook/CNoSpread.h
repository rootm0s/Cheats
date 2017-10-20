#pragma once

class CNoSpread
{
public:
	void applySpread(int seed, PFLOAT pflInAngles, PFLOAT pflOutAngles);
	void applyNoSpread(int seed, PFLOAT pflInAngles, PFLOAT pflOutAngles);
	void AngleVectors2(const QAngle &angles, Vector *forward);
	CBaseCombatWeapon *GetBaseCombatActiveWeapon(CBaseEntity *pEntity);
	int GetWeaponId(CBaseCombatWeapon *pWeapon);
	void vectorAngles(float* forward, float* angles);
	void NewAngleVectors(const QAngle &angles, Vector *forward);
	void angleVectors(PFLOAT angles, PFLOAT f, PFLOAT r, PFLOAT u);
};