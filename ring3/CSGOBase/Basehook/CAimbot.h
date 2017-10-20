#pragma once
class CAimbot
{
public:
	bool isValidPlayer(CBaseEntity* pBaseEntity);
	//ValveSDK::CModelInfo::model_t *GetModel(CBaseEntity* pEntity);
	bool SetupBones(CBaseEntity *pAnimating, matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );
	bool GetHitbox ( Vector &vec, int hitbox, int index );
	VOID doAim(ValveSDK::CInput::CUserCmd* pCmd);
	bool IsVisible(Vector& vecStart, Vector& vecEnd, CBaseEntity* pEntity);
	void CAimbot::ClampAngle(Vector& qaAng);
	void CAimbot::NormalizeVector(Vector& Aim);
	Vector DecayPunchangle(Vector vPunchAngle);
	bool GetHitboxPosition(int iHitBox, Vector& vecOrigin, QAngle qAngles, int index);
	float GetFov(Vector vLocalOrigin, Vector vPosition, Vector vForward);
	void Normalize(float& flValue);
	float GetFov2(QAngle angle, Vector src, Vector dst);
	bool AutoWall(const Vector &vecAbsStart, Vector &vecAbsEnd, CBaseEntity* pLocal, float flDamage);
	inline void VectorTransform(const Vector& in1, const matrix3x4 &in2, Vector &out);
	inline void  CAimbot::MatrixPosition(const matrix3x4 &matrix, Vector &position)
	{
		position[0] = matrix[0][3];
		position[1] = matrix[1][3];
		position[2] = matrix[2][3];
	}
	void  CAimbot::MatrixAngles(const matrix3x4& matrix, QAngle angles)
	{
#ifdef _VPROF_MATHLIB
		VPROF_BUDGET("MatrixAngles", "Mathlib");
#endif
		Assert(s_bMathlibInitialized);
		float forward[3];
		float left[3];
		float up[3];

		//
		// Extract the basis vectors from the matrix. Since we only need the Z
		// component of the up vector, we don't get X and Y.
		//
		forward[0] = matrix[0][0];
		forward[1] = matrix[1][0];
		forward[2] = matrix[2][0];
		left[0] = matrix[0][1];
		left[1] = matrix[1][1];
		left[2] = matrix[2][1];
		up[2] = matrix[2][2];

		float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

		// enough here to get angles?
		if (xyDist > 0.001f)
		{
			// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the X axis
			angles.y = RAD2DEG(atan2f(forward[1], forward[0]));

			// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
			angles.x = RAD2DEG(atan2f(-forward[2], xyDist));

			// (roll)	z = ATAN( left.z, up.z );
			angles.z = RAD2DEG(atan2f(left[2], up[2]));
		}
		else	// forward is mostly Z, gimbal lock-
		{
			// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
			angles.y = RAD2DEG(atan2f(-left[0], left[1]));

			// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
			angles.x = RAD2DEG(atan2f(-forward[2], xyDist));

			// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
			angles.z = 0;
		}
	}
	inline void CAimbot::MatrixAngles(const matrix3x4& matrix, QAngle angles, Vector &position)
	{
		MatrixAngles(matrix, angles);
		MatrixPosition(matrix, position);
	}
private:
};