#include "stdafx.h"

int tAimbot::tAimbotFunctions::GetNewTarget()
{
	if (pAimbot.LocalPlayerEntityInfo.CrosshairEntityIndex > -1 && pAimbot.LocalPlayerEntityInfo.CrosshairEntityIndex < 64)
	{
		return pAimbot.LocalPlayerEntityInfo.CrosshairEntityIndex - 1;
	}
	return -1;
}

D3DXVECTOR3 tAimbot::tAimbotFunctions::ClampAngle(D3DXVECTOR3 ViewAngle)
{
	if (ViewAngle.x > 89.0f && ViewAngle.x <= 180.0f)
	{
		ViewAngle.x = 89.0f;
	}
	if (ViewAngle.x > 180.f)
	{
		ViewAngle.x -= 360.f;
	}
	if (ViewAngle.x < -89.0f)
	{
		ViewAngle.x = -89.0f;
	}
	if (ViewAngle.y > 180.f)
	{
		ViewAngle.y -= 360.f;
	}
	if (ViewAngle.y < -180.f)
	{
		ViewAngle.y += 360.f;
	}
	if (ViewAngle.z != 0.0f)
	{
		ViewAngle.z = 0.0f;
	}
	return ViewAngle;
}

float  tAimbot::tAimbotFunctions::Get3dDistance(D3DXVECTOR3 PlayerPosition, D3DXVECTOR3 EnemyPosition)
{
	return sqrt(powf(EnemyPosition.x - PlayerPosition.x, 2.f) + powf(EnemyPosition.y - PlayerPosition.y, 2.f) + powf(EnemyPosition.z - PlayerPosition.z, 2.f));
}

float  tAimbot::tAimbotFunctions::GetRandomFloat(float min, float max)
{
	std::random_device Random;
	std::mt19937 RandomGenerator(Random());
	std::uniform_real<float> RandomDistribute(min, max);
	return RandomDistribute(RandomGenerator);
}

D3DXVECTOR3 tAimbot::tAimbotFunctions::CalcAngle(D3DXVECTOR3 PlayerPosition, D3DXVECTOR3 EnemyPosition, D3DXVECTOR3 PunchAngle, D3DXVECTOR3 ViewOffset, float YawRecoilReductionFactor, float PitchRecoilReductionFactor)
{
	D3DXVECTOR3 AimAngle;
	D3DXVECTOR3 Delta = { (PlayerPosition.x - EnemyPosition.x), (PlayerPosition.y - EnemyPosition.y), ((PlayerPosition.z + ViewOffset.z) - EnemyPosition.z) };
	float hyp = sqrt(Delta.x * Delta.x + Delta.y * Delta.y);
	AimAngle.x = atanf(Delta.z / hyp) * 57.295779513082f - PunchAngle.x * YawRecoilReductionFactor;
	AimAngle.y = atanf(Delta.y / Delta.x) * 57.295779513082f - PunchAngle.y * PitchRecoilReductionFactor;
	AimAngle.z = 0.0f;
	if (Delta.x >= 0.0)
	{
		AimAngle.y += 180.0f;
	}
	return ClampAngle(AimAngle);
}

D3DXVECTOR3 tAimbot::tAimbotFunctions::SmoothAngle(D3DXVECTOR3 SourceAngle, D3DXVECTOR3 DestinationAngle, float SmoothPercent)
{
	if (SmoothPercent < 0.f)
	{
		SmoothPercent = 0.f;
	}
	D3DXVECTOR3 SmoothedAngle;
	SmoothedAngle = DestinationAngle - SourceAngle;
	SmoothedAngle = ClampAngle(SmoothedAngle);
	SmoothedAngle = SourceAngle + SmoothedAngle / 100.f * (100.f - SmoothPercent);
	return ClampAngle(SmoothedAngle);
}
D3DXVECTOR3 tAimbot::tAimbotFunctions::VelocityComp(D3DXVECTOR3 EnemyPosition, D3DXVECTOR3 EnemyVelocity, D3DXVECTOR3 PlayerVelocity, float Distance)
{
	EnemyPosition += EnemyVelocity / Distance;
	EnemyPosition -= PlayerVelocity / Distance;
	return EnemyPosition;
}

void tAimbot::tAimbotFunctions::Tick()
{
	if (pGlobalVars.AimbotSettings.Enabled)
	{
		if (GetAsyncKeyState(pGlobalVars.AimbotSettings.ActivationKey) < 0)
		{
			pLocalPlayerEntity.Functions.GetLocalPlayerEntityInfo(&pAimbot.LocalPlayerEntityInfo);
			if (pAimbot.LocalPlayerEntityInfo.Valid && pAimbot.LocalPlayerEntityInfo.ShotsFired >= pGlobalVars.AimbotSettings.IgnoreShotCount)
			{
				if (pAimbot.CurrentTargetIndex != -1)
				{
					pGeneralPlayerEntity.Functions.GetGeneralPlayerEntityInfo(&pAimbot.GeneralPlayerEntityInfo, pAimbot.CurrentTargetIndex);
					if (pAimbot.GeneralPlayerEntityInfo.Valid && pAimbot.GeneralPlayerEntityInfo.TeamNumber != pAimbot.LocalPlayerEntityInfo.TeamNumber)
					{
						if (!pGlobalVars.AimbotSettings.TargetJumpingPlayers && pGeneralPlayerEntity.Functions.IsJumping(pAimbot.GeneralPlayerEntityInfo.Flags))
						{
							return;
						}
						if (pGlobalVars.AimbotSettings.EnableTimeout && pAimbot.Timer > pGlobalVars.AimbotSettings.Timeout)
						{
							return;
						}
						if (pGlobalVars.AimbotSettings.EnableFire && !(GetAsyncKeyState(VK_LBUTTON) < 0))
						{
							mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
						}
						float YawRecoilReductionFactor, PitchRecoilReductionFactor;
						if (pGlobalVars.AimbotSettings.EnableRandomYawRecoilReductionFactor)
						{
							YawRecoilReductionFactor = GetRandomFloat(pGlobalVars.AimbotSettings.MinRandomYaw, pGlobalVars.AimbotSettings.MaxRandomYaw);
						}
						else
						{
							YawRecoilReductionFactor = pGlobalVars.AimbotSettings.StaticYaw;
						}
						if (pGlobalVars.AimbotSettings.EnableRandomPitchRecoilReductionFactor)
						{
							PitchRecoilReductionFactor = GetRandomFloat(pGlobalVars.AimbotSettings.MinRandomPitch, pGlobalVars.AimbotSettings.MaxRandomPitch);
						}
						else
						{
							PitchRecoilReductionFactor = pGlobalVars.AimbotSettings.StaticPitch;
						}
						pLocalPlayerEntity.Functions.SetViewAngle(pAimbot.LocalPlayerEntityInfo.ClientStateBaseAddress, pGlobalVars.Offsets.m_dwViewAngle, SmoothAngle(pAimbot.LocalPlayerEntityInfo.ViewAngle, CalcAngle(pAimbot.LocalPlayerEntityInfo.Origin, pGeneralPlayerEntity.Functions.GetBonePosition(&pAimbot.GeneralPlayerEntityInfo.BoneMatrix, pGlobalVars.AimbotSettings.TargetBone), pAimbot.LocalPlayerEntityInfo.PunchAngle, pAimbot.LocalPlayerEntityInfo.ViewOffset, YawRecoilReductionFactor, PitchRecoilReductionFactor), pGlobalVars.AimbotSettings.SmoothPercent - (pAimbot.Timer / 100.f)));
						pAimbot.Timer++;
						return;
					}
					else
					{
						pAimbot.Timer = 0.f;
						pLocalPlayerEntity.Functions.GetLocalPlayerEntityInfo(&pAimbot.LocalPlayerEntityInfo);
						pAimbot.CurrentTargetIndex = GetNewTarget();
					}
				}
				else
				{
					pAimbot.Timer = 0.f;
					pLocalPlayerEntity.Functions.GetLocalPlayerEntityInfo(&pAimbot.LocalPlayerEntityInfo);
					pAimbot.CurrentTargetIndex = GetNewTarget();
				}

			}
		}
		else
		{
			if (pGlobalVars.AimbotSettings.EnableFire && GetAsyncKeyState(VK_LBUTTON) < 0)
			{
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
			}
			pAimbot.Timer = 0.f;
			pAimbot.CurrentTargetIndex = -1;
		}
	}
}