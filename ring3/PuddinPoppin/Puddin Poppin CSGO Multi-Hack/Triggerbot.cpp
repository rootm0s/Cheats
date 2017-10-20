#include "stdafx.h"

int tTriggerbot::tTriggerbotFunctions::GetNewTarget()
{
	if (pTriggerbot.LocalPlayerEntityInfo.CrosshairEntityIndex > -1 && pTriggerbot.LocalPlayerEntityInfo.CrosshairEntityIndex < 64)
	{
		return pTriggerbot.LocalPlayerEntityInfo.CrosshairEntityIndex - 1;
	}
	return -1;
}

void tTriggerbot::tTriggerbotFunctions::Tick()
{
	if (pGlobalVars.TriggerbotSettings.Enabled)
	{
		if (GetAsyncKeyState(pGlobalVars.TriggerbotSettings.ActivationKey) < 0)
		{
			pLocalPlayerEntity.Functions.GetLocalPlayerEntityInfo(&pTriggerbot.LocalPlayerEntityInfo);
			if (pTriggerbot.LocalPlayerEntityInfo.Valid)
			{
				int NewTargetIndex = GetNewTarget();
				if (NewTargetIndex != -1)
				{
					if (pTriggerbot.Timer > pGlobalVars.TriggerbotSettings.Timeout)
					{
						return;
					}
					if (pGlobalVars.TriggerbotSettings.EnableRecoilLimit)
					{
						if (powf(pTriggerbot.LocalPlayerEntityInfo.PunchAngle.x, 2.f) > pGlobalVars.TriggerbotSettings.YawRecoilLimit || powf(pTriggerbot.LocalPlayerEntityInfo.PunchAngle.y, 2.f) > pGlobalVars.TriggerbotSettings.PitchRecoilLimit)
						{
							return;
						}
					}
					if (NewTargetIndex != pTriggerbot.CurrentTargetIndex)
					{
						pTriggerbot.Timer = 0.f;
						Sleep(pGlobalVars.TriggerbotSettings.FirstShotDelay);
						pTriggerbot.Timer += pGlobalVars.TriggerbotSettings.FirstShotDelay;
						pTriggerbot.CurrentTargetIndex = NewTargetIndex;
					}
					pGeneralPlayerEntity.Functions.GetGeneralPlayerEntityInfo(&pTriggerbot.GeneralPlayerEntityInfo, pTriggerbot.CurrentTargetIndex);
					if (pTriggerbot.GeneralPlayerEntityInfo.Valid && pTriggerbot.GeneralPlayerEntityInfo.TeamNumber != pTriggerbot.LocalPlayerEntityInfo.TeamNumber)
					{
						mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
						Sleep(pGlobalVars.TriggerbotSettings.FollowUpDelay);
						pTriggerbot.Timer += pGlobalVars.TriggerbotSettings.FollowUpDelay;
					}
				}
			}
		}
		else
		{
			pTriggerbot.Timer = 0.f;
			pTriggerbot.CurrentTargetIndex = GetNewTarget();
		}
	}
}
