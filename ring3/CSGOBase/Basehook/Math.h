#ifndef __SOURCEMATH_H__
#define __SOURCEMATH_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "DLLMain.h"

#define M_PI	3.14159265358979323846f
#define M_RADPI	57.295779513082f

namespace source_engine
{
	namespace Math
	{
		extern float	GetFOV(const Vector& vecAngle, const Vector& vecOrigin, const Vector& vecEntity);
		extern float	VectorNormalize(Vector& vec);
		extern VOID		CalculateAngles(const Vector& vecOrigin, const Vector& vecEntity, Vector& vecAngles);
		extern VOID		MakeVector(const Vector& vecIn, Vector& vecOut);
		extern VOID		VectorSubtract(const Vector& a, const Vector& b, Vector& c);
		extern VOID		VectorAdd(const Vector& a, const Vector& b, Vector& c);
		extern VOID		VectorDivide(const Vector& a, const float& b, Vector& c);
		extern VOID		VectorClear(Vector& vec);
		extern VOID		VectorCopy(const Vector& vecSource, Vector& vecDest);
		extern VOID		AngleLimit(float& flAngle);
		extern void AngleVectors(const Vector angles, float *forward, float *right, float *up);
		extern void VectorAngles(const Vector forward, Vector &angles);
	}
}

#endif


#define M_PI	3.14159265358979323846f
#define M_RADPI	57.295779513082f

namespace source_engine
{
	namespace Math
	{
		float GetFOV(const Vector& vecAngle, const Vector& vecOrigin, const Vector& vecEntity)
		{
			Vector vecAng, vecAim;

			CalculateAngles(vecOrigin, vecEntity, vecAng);
			MakeVector(vecAngle, vecAim);
			MakeVector(vecAng, vecAng);

			float mag_s = sqrt((vecAim.x * vecAim.x) + (vecAim.y * vecAim.y) + (vecAim.z * vecAim.z));
			float mag_d = sqrt((vecAim.x * vecAim.x) + (vecAim.y * vecAim.y) + (vecAim.z * vecAim.z));

			float u_dot_v = vecAim.x * vecAng.x + vecAim.y * vecAng.y + vecAim.z * vecAng.z;

			return (float)acos(u_dot_v / (mag_s*mag_d)) * (180.0 / M_PI);
		}

		float VectorNormalize(Vector& vec)
		{
			float flLength = (float)sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));

			if (flLength)
			{
				float flTemp = 1.f / flLength;
				vec[0] *= flTemp;
				vec[1] *= flTemp;
				vec[2] *= flTemp;
			}

			return flLength;
		}

		VOID CalculateAngles(const Vector& vecOrigin, const Vector& vecEntity, Vector& vecAngles)
		{
			float delta[3] = { vecOrigin.x - vecEntity.x, vecOrigin.y - vecEntity.y, vecOrigin.z - vecEntity.z };
			float hyp = (float)sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

			vecAngles[0] = (float)(atan(delta[2] / hyp) * M_RADPI);
			vecAngles[1] = (float)(atan(delta[1] / delta[0]) * M_RADPI);
			vecAngles[2] = 0.f;
			if (delta[0] >= 0.f)
			{
				vecAngles[1] += 180.f;
			}
		}

		VOID MakeVector(const Vector& vecIn, Vector& vecOut)
		{
			float pitch;
			float yaw;
			float tmp;

			pitch = (float)(vecIn.x * M_PI / 180);
			yaw = (float)(vecIn.y * M_PI / 180);
			tmp = (float)cos(pitch);

			vecOut[0] = (float)(-tmp * -cos(yaw));
			vecOut[1] = (float)(sin(yaw)*tmp);
			vecOut[2] = (float)-sin(pitch);
		}

		VOID VectorSubtract(const Vector& a, const Vector& b, Vector& c)
		{
			c.x = a.x - b.x;
			c.y = a.y - b.y;
			c.z = a.z - b.z;
		}

		VOID VectorAdd(const Vector& a, const Vector& b, Vector& c)
		{
			c.x = a.x + b.x;
			c.y = a.y + b.y;
			c.z = a.z + b.z;
		}

		VOID VectorDivide(const Vector& a, const float& b, Vector& c)
		{
			float oob = 1.0f / b;
			c.x = a.x * oob;
			c.y = a.y * oob;
			c.z = a.z * oob;
		}
		VOID VectorClear(Vector& vec)
		{
			vec.x = vec.y = vec.z = 0;
		}

		VOID VectorCopy(const Vector& vecSource, Vector& vecDest)
		{
			vecDest.x = vecSource.x;
			vecDest.y = vecSource.y;
			vecDest.z = vecSource.z;
		}

		VOID AngleLimit(float& flAngle)
		{
			if (flAngle > 180.f)
			{
				flAngle -= 360;
			}
			if (flAngle < -180)
			{
				flAngle += 360;
			}
		}

		void AngleVectors(const Vector angles, float *forward, float *right, float *up)
		{
			float angle;
			static float sp, sy, cp, cy;

			angle = angles[0] * (M_PI / 180.f);
			sp = sin(angle);
			cp = cos(angle);

			angle = angles[1] * (M_PI / 180.f);
			sy = sin(angle);
			cy = cos(angle);

			if (forward)
			{
				forward[0] = cp * cy;
				forward[1] = cp * sy;
				forward[2] = -sp;
			}
			if (right || up)
			{
				static float sr, cr;

				angle = angles[2] * (M_PI / 180.f);
				sr = sin(angle);
				cr = cos(angle);

				if (right)
				{
					right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
					right[1] = -1 * sr * sp * sy + -1 * cr *cy;
					right[2] = -1 * sr * cp;
				}
				if (up)
				{
					up[0] = cr * sp *cy + -sr * -sy;
					up[1] = cr * sp *sy + -sr * cy;
					up[2] = cr * cp;
				}
			}
		}
		void VectorAngles(const Vector forward, Vector &angles)
		{
			float	tmp, yaw, pitch;

			if (forward[1] == 0 && forward[0] == 0)
			{
				yaw = 0;
				if (forward[2] > 0)
					pitch = 270;
				else
					pitch = 90;
			}
			else
			{
				yaw = (atan2(forward[1], forward[0]) * 180.f / M_PI);
				if (yaw < 0.f)
					yaw += 360.f;

				tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
				pitch = (atan2(-forward[2], tmp) * 180.f / M_PI);
				if (pitch < 0.f)
					pitch += 360.f;
			}

			angles[0] = pitch;
			angles[1] = yaw;
			angles[2] = 0;
		}
	}
}