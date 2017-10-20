#ifndef __CSPREAD_H__
#define __CSPREAD_H__

#ifdef _MSC_VER
#pragma once
#endif

class CSpread
{
public:
	float GetInaccuracy(ValveSDK::CBaseCombatWeapon *pWeapon);
	float GetSpread(ValveSDK::CBaseCombatWeapon *pWeapon);
	void UpdateAccuracyPenalty(ValveSDK::CBaseCombatWeapon *pWeapon);
	void SpreadFactor(ValveSDK::CInput::CUserCmd *pCmd, ValveSDK::CBaseCombatWeapon *pWeapon);
	void angleVectors(PFLOAT angles, PFLOAT f, PFLOAT r, PFLOAT u);
	void angleVectors(Vector angles, Vector &f, Vector &r, Vector &u, bool bNoThanks);
	void vectorAngles(float* forward, float* angles);
	ValveSDK::CBaseCombatWeapon* GetBaseCombatActiveWeapon(CBaseEntity *pEntity);
};

extern CSpread g_pSpread;

#endif