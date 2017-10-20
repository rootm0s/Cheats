#include "stdafx.h"

bool tESP::tESPFunctions::OnScreen(D3DXVECTOR2 ScreenPosition)
{
	if (ScreenPosition.x >= pOverlayWindow.X && ScreenPosition.x <= pOverlayWindow.Width && ScreenPosition.y >= pOverlayWindow.Y && ScreenPosition.y <= pOverlayWindow.Height)
	{
		return true;
	}
	return false;
}

D3DXVECTOR2 tESP::tESPFunctions::WorldToScreen(D3DXVECTOR3 EnemyPosition, tLocalPlayerEntity::tDataStructs::tViewMatrix ViewMatrix)
{
	D3DXVECTOR2 ScreenPosition;
	if (ViewMatrix.Matrix)
	{
		ScreenPosition.x = ViewMatrix.Matrix[0][0] * EnemyPosition.x + ViewMatrix.Matrix[0][1] * EnemyPosition.y + ViewMatrix.Matrix[0][2] * EnemyPosition.z + ViewMatrix.Matrix[0][3];
		ScreenPosition.y = ViewMatrix.Matrix[1][0] * EnemyPosition.x + ViewMatrix.Matrix[1][1] * EnemyPosition.y + ViewMatrix.Matrix[1][2] * EnemyPosition.z + ViewMatrix.Matrix[1][3];
		float w = ViewMatrix.Matrix[3][0] * EnemyPosition.x + ViewMatrix.Matrix[3][1] * EnemyPosition.y + ViewMatrix.Matrix[3][2] * EnemyPosition.z + ViewMatrix.Matrix[3][3];
		if (w > 0.01f)
		{
			float invw = 1.0f / w;
			ScreenPosition.x *= invw;
			ScreenPosition.y *= invw;
			float x = pOverlayWindow.Width / 2.f;
			float y = pOverlayWindow.Height / 2.f;
			x += 0.5f * ScreenPosition.x * pOverlayWindow.Width + 0.5f;
			y -= 0.5f * ScreenPosition.y * pOverlayWindow.Height + 0.5f;
			ScreenPosition.x = x + pOverlayWindow.X;
			ScreenPosition.y = y + pOverlayWindow.Y;
		}
		else
		{
			ScreenPosition.x = -1.f;
			ScreenPosition.y = -1.f;
		}
	}
	return ScreenPosition;
}

void tESP::tESPFunctions::DrawBox(int PlayerNumber)
{
	if (pGlobalVars.EspSettings.DrawEnemyBox)
	{
		if (pESP.LocalPlayerEntityInfo.TeamNumber != pESP.GeneralPlayerEntityInfo.TeamNumber)
		{
			D3DXVECTOR2 BottomPosition = pESP.Functions.WorldToScreen(pESP.GeneralPlayerEntityInfo.Origin, pESP.LocalPlayerEntityInfo.ViewMatrix);
			D3DXVECTOR3 OriginPlusHeight = pESP.GeneralPlayerEntityInfo.Origin;
			OriginPlusHeight.z += 80.f;
			D3DXVECTOR2 TopPosition = pESP.Functions.WorldToScreen(OriginPlusHeight, pESP.LocalPlayerEntityInfo.ViewMatrix);
			if (pGlobalVars.EspSettings.DrawTargetBoxHighlight && pESP.LocalPlayerEntityInfo.CrosshairEntityIndex - 1 == PlayerNumber)
			{
				float x, y, h, w;
				y = TopPosition.y;
				h = BottomPosition.y - TopPosition.y;
				w = h * (45.f / 80.f);
				x = TopPosition.x - w / 2.f;
				pD3D.Functions.DrawRect(x, y, w, h, pGlobalVars.EspSettings.TargetEnemyBoxLineWidth, pGlobalVars.EspSettings.TargetEnemyBoxColor, false, true, pGlobalVars.EspSettings.TargetEnemyBoxOutlineWidth, pGlobalVars.EspSettings.TargetEnemyBoxOutlineColor);
			}
			else
			{
				float x, y, h, w;
				y = TopPosition.y;
				h = BottomPosition.y - TopPosition.y;
				w = h * (45.f / 80.f);
				x = TopPosition.x - w / 2.f;
				D3DXVECTOR2 ScreenPosition;
				ScreenPosition.x = x;
				ScreenPosition.y = y;
				if (OnScreen(ScreenPosition))
				{
					pD3D.Functions.DrawRect(x, y, w, h, pGlobalVars.EspSettings.EnemyBoxLineWidth, pGlobalVars.EspSettings.EnemyBoxColor, false, true, pGlobalVars.EspSettings.EnemyBoxOutlineWidth, pGlobalVars.EspSettings.EnemyBoxOutlineColor);
				}
			}
		}
	}
	if (pGlobalVars.EspSettings.DrawFriendlyBox)
	{
		if (pESP.LocalPlayerEntityInfo.TeamNumber == pESP.GeneralPlayerEntityInfo.TeamNumber)
		{
			D3DXVECTOR2 BottomPosition = pESP.Functions.WorldToScreen(pESP.GeneralPlayerEntityInfo.Origin, pESP.LocalPlayerEntityInfo.ViewMatrix);
			D3DXVECTOR3 OriginPlusHeight = pESP.GeneralPlayerEntityInfo.Origin;
			OriginPlusHeight.z += 80.f;
			D3DXVECTOR2 TopPosition = pESP.Functions.WorldToScreen(OriginPlusHeight, pESP.LocalPlayerEntityInfo.ViewMatrix);
			float x, y, h, w;
			y = TopPosition.y;
			h = BottomPosition.y - TopPosition.y;
			w = h * (45.f / 80.f);
			x = TopPosition.x - w / 2.f;
			D3DXVECTOR2 ScreenPosition;
			ScreenPosition.x = x;
			ScreenPosition.y = y;
			if (OnScreen(ScreenPosition))
			{
				pD3D.Functions.DrawRect(x, y, w, h, pGlobalVars.EspSettings.FriendlyBoxLineWidth, pGlobalVars.EspSettings.FriendlyBoxColor, false, true, pGlobalVars.EspSettings.FriendlyBoxOutlineWidth, pGlobalVars.EspSettings.FriendlyBoxOutlineColor);
			}
		}
	}
}

void tESP::tESPFunctions::DrawSnapLines(int PlayerNumber)
{
	if (pGlobalVars.EspSettings.DrawEnemySnapLines)
	{
		if (pESP.LocalPlayerEntityInfo.TeamNumber != pESP.GeneralPlayerEntityInfo.TeamNumber)
		{
			D3DXVECTOR2 BottomPosition = pESP.Functions.WorldToScreen(pESP.GeneralPlayerEntityInfo.Origin, pESP.LocalPlayerEntityInfo.ViewMatrix);
			if (pGlobalVars.EspSettings.DrawTargetSnapLineHighlight && pESP.LocalPlayerEntityInfo.CrosshairEntityIndex - 1 == PlayerNumber)
			{
				pD3D.Functions.DrawLine(pOverlayWindow.Width / 2.f, pOverlayWindow.Height, BottomPosition.x, BottomPosition.y, pGlobalVars.EspSettings.TargetEnemySnapLineWidth, pGlobalVars.EspSettings.TargetEnemySnapLineColor, true, pGlobalVars.EspSettings.TargetEnemySnapLineOutlineWidth, pGlobalVars.EspSettings.TargetEnemySnapLineOutlineColor);
			}
			else if (OnScreen(BottomPosition))
			{
				pD3D.Functions.DrawLine(pOverlayWindow.Width / 2.f, pOverlayWindow.Height, BottomPosition.x, BottomPosition.y, pGlobalVars.EspSettings.EnemySnapLineWidth, pGlobalVars.EspSettings.EnemySnapLineColor, true, pGlobalVars.EspSettings.EnemySnapLineOutlineWidth, pGlobalVars.EspSettings.EnemySnapLineOutlineColor);
			}
		}
	}
	if (pGlobalVars.EspSettings.DrawFriendlySnapLines)
	{
		if (pESP.LocalPlayerEntityInfo.TeamNumber == pESP.GeneralPlayerEntityInfo.TeamNumber)
		{
			D3DXVECTOR2 BottomPosition = pESP.Functions.WorldToScreen(pESP.GeneralPlayerEntityInfo.Origin, pESP.LocalPlayerEntityInfo.ViewMatrix);
			if (OnScreen(BottomPosition))
			{
				pD3D.Functions.DrawLine(pOverlayWindow.Width / 2.f, pOverlayWindow.Height, BottomPosition.x, BottomPosition.y, pGlobalVars.EspSettings.FriendlySnapLineWidth, pGlobalVars.EspSettings.FriendlySnapLineColor, true, pGlobalVars.EspSettings.FriendlySnapLineOutlineWidth, pGlobalVars.EspSettings.FriendlySnapLineOutlineColor);
			}
		}
	}
}

void tESP::tESPFunctions::DrawHealthBar()
{
	if (pGlobalVars.EspSettings.DrawEnemyHealthBar)
	{
		if (pESP.LocalPlayerEntityInfo.TeamNumber != pESP.GeneralPlayerEntityInfo.TeamNumber)
		{
			D3DXVECTOR2 BottomPosition = pESP.Functions.WorldToScreen(pESP.GeneralPlayerEntityInfo.Origin, pESP.LocalPlayerEntityInfo.ViewMatrix);
			D3DXVECTOR3 OriginPlusHeight = pESP.GeneralPlayerEntityInfo.Origin;
			OriginPlusHeight.z += 80.f;
			D3DXVECTOR2 TopPosition = pESP.Functions.WorldToScreen(OriginPlusHeight, pESP.LocalPlayerEntityInfo.ViewMatrix);
			float x, y, h, w;
			y = TopPosition.y;
			h = BottomPosition.y - TopPosition.y;
			w = h * (pGlobalVars.EspSettings.EnemyHealthBarWidth / 80.f);
			x = TopPosition.x - (h * (45.f / 80.f)) / 2.f;
			D3DXVECTOR2 ScreenPosition;
			ScreenPosition.x = x;
			ScreenPosition.y = y;
			if (OnScreen(ScreenPosition))
			{
				pD3D.Functions.DrawRect(x, y, w, h, 0, pGlobalVars.EspSettings.EnemyHealthBarBackColor, true, true, pGlobalVars.EspSettings.EnemyHealthBarOutlineWidth, pGlobalVars.EspSettings.EnemyHealthBarOutlineColor);
				pD3D.Functions.DrawRect(x, y += (h / 100.f) * (100 - pESP.GeneralPlayerEntityInfo.Health), w, (h / 100.f) * pESP.GeneralPlayerEntityInfo.Health, 0, pGlobalVars.EspSettings.EnemyHealthBarFrontColor, true, true, pGlobalVars.EspSettings.EnemyHealthBarOutlineWidth, pGlobalVars.EspSettings.EnemyHealthBarOutlineColor);
			}
		}
	}
	if (pGlobalVars.EspSettings.DrawFriendlyHealthBar)
	{
		if (pESP.LocalPlayerEntityInfo.TeamNumber == pESP.GeneralPlayerEntityInfo.TeamNumber)
		{
			D3DXVECTOR2 BottomPosition = pESP.Functions.WorldToScreen(pESP.GeneralPlayerEntityInfo.Origin, pESP.LocalPlayerEntityInfo.ViewMatrix);
			D3DXVECTOR3 OriginPlusHeight = pESP.GeneralPlayerEntityInfo.Origin;
			OriginPlusHeight.z += 80.f;
			D3DXVECTOR2 TopPosition = pESP.Functions.WorldToScreen(OriginPlusHeight, pESP.LocalPlayerEntityInfo.ViewMatrix);
			float x, y, h, w;
			y = TopPosition.y;
			h = BottomPosition.y - TopPosition.y;
			w = h * (pGlobalVars.EspSettings.FriendlyHealthBarWidth / 80.f);
			x = TopPosition.x - (h * (45.f / 80.f)) / 2.f;
			D3DXVECTOR2 ScreenPosition;
			ScreenPosition.x = x;
			ScreenPosition.y = y;
			if (OnScreen(ScreenPosition))
			{
				pD3D.Functions.DrawRect(x, y, w, h, 0, pGlobalVars.EspSettings.FriendlyHealthBarBackColor, true, true, pGlobalVars.EspSettings.FriendlyHealthBarOutlineWidth, pGlobalVars.EspSettings.FriendlyHealthBarOutlineColor);
				pD3D.Functions.DrawRect(x, y += (h / 100.f) * (100 - pESP.GeneralPlayerEntityInfo.Health), w, (h / 100.f) * pESP.GeneralPlayerEntityInfo.Health, 0, pGlobalVars.EspSettings.FriendlyHealthBarFrontColor, true, true, pGlobalVars.EspSettings.FriendlyHealthBarOutlineWidth, pGlobalVars.EspSettings.FriendlyHealthBarOutlineColor);
			}
		}
	}
}

void tESP::tESPFunctions::DrawInfoText(int PlayerNumber)
{
	pESP.InfoText.clear();
	if (pESP.LocalPlayerEntityInfo.TeamNumber != pESP.GeneralPlayerEntityInfo.TeamNumber)
	{
		pESP.InfoTextColor = pGlobalVars.EspSettings.EnemyInfoTextColor;
		pESP.InfoTextOutlineColor = pGlobalVars.EspSettings.EnemyInfoTextOutlineColor;
		if (pGlobalVars.EspSettings.DrawEnemyNameText)
		{
			pESP.InfoText.append(L"Name: ");
			pESP.InfoText.append(pESP.GeneralPlayerEntityInfo.PlayerName.Text);
		}
		if (pGlobalVars.EspSettings.DrawEnemyHealthText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"Health: ");
			pESP.InfoText.append(std::to_wstring(pESP.GeneralPlayerEntityInfo.Health));
		}
		if (pGlobalVars.EspSettings.DrawEnemyKillsText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"Kills: ");
			pESP.InfoText.append(std::to_wstring(pESP.GeneralPlayerEntityInfo.Kills));
		}
		if (pGlobalVars.EspSettings.DrawEnemyDeathsText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"Deaths: ");
			pESP.InfoText.append(std::to_wstring(pESP.GeneralPlayerEntityInfo.Deaths));
		}
		if (pGlobalVars.EspSettings.DrawEnemyKDRatioText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"K/D Ratio: ");
			float KDRatio = 0;
			if (pESP.GeneralPlayerEntityInfo.Kills != 0)
			{
				if (pESP.GeneralPlayerEntityInfo.Deaths != 0)
				{
					KDRatio = (float)pESP.GeneralPlayerEntityInfo.Kills / (float)pESP.GeneralPlayerEntityInfo.Deaths;
				}
				else
				{
					KDRatio = (float)pESP.GeneralPlayerEntityInfo.Kills;
				}
			}
			pESP.InfoText.append(std::to_wstring(KDRatio).substr(0, std::to_wstring(KDRatio).find(L".") + 3));
		}
		if (pGlobalVars.EspSettings.DrawEnemyRankText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"Rank: ");
			pESP.InfoText.append(std::to_wstring(pESP.GeneralPlayerEntityInfo.CompetetiveRankNumber));
		}
		if (pGlobalVars.EspSettings.DrawEnemyBombCarrierText)
		{
			pESP.InfoText.append(L"\n");
			if (pESP.GeneralPlayerEntityInfo.HasC4 - 1 == PlayerNumber)
			{
				pESP.InfoText.append(L"C4\n");
			}
		}
		if (pGlobalVars.EspSettings.DrawEnemyDefuseKitCarrierText)
		{
			if (pESP.GeneralPlayerEntityInfo.HasDefuser)
			{
				pESP.InfoText.append(L"Defuse Kit");
			}
		}
	}
	else
	{
		pESP.InfoTextColor = pGlobalVars.EspSettings.FriendlyInfoTextColor;
		pESP.InfoTextOutlineColor = pGlobalVars.EspSettings.FriendlyInfoTextOutlineColor;
		if (pGlobalVars.EspSettings.DrawFriendlyNameText)
		{
			pESP.InfoText.append(L"Name: ");
			pESP.InfoText.append(pESP.GeneralPlayerEntityInfo.PlayerName.Text);
		}
		if (pGlobalVars.EspSettings.DrawFriendlyHealthText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"Health: ");
			pESP.InfoText.append(std::to_wstring(pESP.GeneralPlayerEntityInfo.Health));
		}
		if (pGlobalVars.EspSettings.DrawFriendlyKillsText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"Kills: ");
			pESP.InfoText.append(std::to_wstring(pESP.GeneralPlayerEntityInfo.Kills));
		}
		if (pGlobalVars.EspSettings.DrawFriendlyDeathsText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"Deaths: ");
			pESP.InfoText.append(std::to_wstring(pESP.GeneralPlayerEntityInfo.Deaths));
		}
		if (pGlobalVars.EspSettings.DrawFriendlyKDRatioText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"K/D Ratio: ");
			float KDRatio = 0.f;
			if (pESP.GeneralPlayerEntityInfo.Kills != 0)
			{
				if (pESP.GeneralPlayerEntityInfo.Deaths != 0)
				{
					KDRatio = (float)pESP.GeneralPlayerEntityInfo.Kills / (float)pESP.GeneralPlayerEntityInfo.Deaths;
				}
				else
				{
					KDRatio = (float)pESP.GeneralPlayerEntityInfo.Kills;
				}
			}
			pESP.InfoText.append(std::to_wstring(KDRatio).substr(0, std::to_wstring(KDRatio).find(L".") + 3));
		}
		if (pGlobalVars.EspSettings.DrawFriendlyRankText)
		{
			pESP.InfoText.append(L"\n");
			pESP.InfoText.append(L"Rank: ");
			pESP.InfoText.append(std::to_wstring(pESP.GeneralPlayerEntityInfo.CompetetiveRankNumber));
		}
		if (pGlobalVars.EspSettings.DrawFriendlyBombCarrierText)
		{
			pESP.InfoText.append(L"\n");
			if (pESP.GeneralPlayerEntityInfo.HasC4 - 1 == PlayerNumber)
			{
				pESP.InfoText.append(L"C4\n");
			}
		}
		if (pGlobalVars.EspSettings.DrawEnemyDefuseKitCarrierText)
		{
			if (pESP.GeneralPlayerEntityInfo.HasDefuser)
			{
				pESP.InfoText.append(L"Defuse Kit");
			}
		}
	}
	if (!pESP.InfoText.empty())
	{
		D3DXVECTOR2 BottomPosition = pESP.Functions.WorldToScreen(pESP.GeneralPlayerEntityInfo.Origin, pESP.LocalPlayerEntityInfo.ViewMatrix);
		D3DXVECTOR3 OriginMinusHeight = pESP.GeneralPlayerEntityInfo.Origin;
		OriginMinusHeight.z -= 5.f;
		D3DXVECTOR2 TextPosition = pESP.Functions.WorldToScreen(OriginMinusHeight, pESP.LocalPlayerEntityInfo.ViewMatrix);
		float x, y;
		y = TextPosition.y;
		x = TextPosition.x;
		D3DXVECTOR2 ScreenPosition;
		ScreenPosition.x = x;
		ScreenPosition.y = y;
		if (OnScreen(ScreenPosition))
		{
			pD3D.Functions.DrawString(x, y, pESP.InfoText, pESP.InfoTextColor, true, pESP.InfoTextOutlineColor, true);
		}
	}
}

void tESP::tESPFunctions::DrawTargetBoneMarker()
{
	if (pESP.LocalPlayerEntityInfo.TeamNumber != pESP.GeneralPlayerEntityInfo.TeamNumber)
	{
		if (pGlobalVars.EspSettings.DrawEnemyTargetBoneMarker && pAimbot.Functions.Get3dDistance(pGeneralPlayerEntity.Functions.GetBonePosition(&pESP.GeneralPlayerEntityInfo.BoneMatrix, pGlobalVars.AimbotSettings.TargetBone), pESP.GeneralPlayerEntityInfo.Origin) < 100.f)
		{
			D3DXVECTOR2 ScreenPosition = pESP.Functions.WorldToScreen(pGeneralPlayerEntity.Functions.GetBonePosition(&pESP.GeneralPlayerEntityInfo.BoneMatrix, pGlobalVars.AimbotSettings.TargetBone), pESP.LocalPlayerEntityInfo.ViewMatrix);
			if (OnScreen(ScreenPosition))
			{
				if (pGlobalVars.EspSettings.EnemyTargetBoneMarkerType == 1)
				{
					pD3D.Functions.DrawCircle(ScreenPosition.x, ScreenPosition.y, pGlobalVars.EspSettings.EnemyTargetBoneMarkerSize, pGlobalVars.EspSettings.EnemyTargetBoneMarkerLineWidth, pGlobalVars.EspSettings.EnemyTargetBoneMarkerColor, true, pGlobalVars.EspSettings.EnemyTargetBoneMarkerOutlineWidth, pGlobalVars.EspSettings.EnemyTargetBoneMarkerOutlineColor);
				}
				if (pGlobalVars.EspSettings.EnemyTargetBoneMarkerType == 2)
				{
					pD3D.Functions.DrawCross(ScreenPosition.x, ScreenPosition.y, pGlobalVars.EspSettings.EnemyTargetBoneMarkerSize, pGlobalVars.EspSettings.EnemyTargetBoneMarkerLineWidth, pGlobalVars.EspSettings.EnemyTargetBoneMarkerColor, true, pGlobalVars.EspSettings.EnemyTargetBoneMarkerOutlineWidth, pGlobalVars.EspSettings.EnemyTargetBoneMarkerOutlineColor);
				}
			}
		}
	}
	else if(pESP.LocalPlayerEntityInfo.TeamNumber == pESP.GeneralPlayerEntityInfo.TeamNumber)
	{
		if (pGlobalVars.EspSettings.DrawFriendlyTargetBoneMarker && pAimbot.Functions.Get3dDistance(pGeneralPlayerEntity.Functions.GetBonePosition(&pESP.GeneralPlayerEntityInfo.BoneMatrix, pGlobalVars.AimbotSettings.TargetBone), pESP.GeneralPlayerEntityInfo.Origin) < 100.f)
		{
			D3DXVECTOR2 ScreenPosition = pESP.Functions.WorldToScreen(pGeneralPlayerEntity.Functions.GetBonePosition(&pESP.GeneralPlayerEntityInfo.BoneMatrix, pGlobalVars.AimbotSettings.TargetBone), pESP.LocalPlayerEntityInfo.ViewMatrix);
			if (OnScreen(ScreenPosition))
			{
				if (pGlobalVars.EspSettings.FriendlyTargetBoneMarkerType == 1)
				{
					pD3D.Functions.DrawCircle(ScreenPosition.x, ScreenPosition.y, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerSize, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerLineWidth, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerColor, true, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerOutlineWidth, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerOutlineColor);
				}
				if (pGlobalVars.EspSettings.FriendlyTargetBoneMarkerType == 2)
				{
					pD3D.Functions.DrawCross(ScreenPosition.x, ScreenPosition.y, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerSize, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerLineWidth, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerColor, true, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerOutlineWidth, pGlobalVars.EspSettings.FriendlyTargetBoneMarkerOutlineColor);
				}
			}
		}
	}
}

void tESP::tESPFunctions::DrawRecoilMarker()
{
	if (pGlobalVars.EspSettings.DrawRecoilMarker)
	{
		float CenterX = pOverlayWindow.Width / 2.f;
		float CenterY = pOverlayWindow.Height / 2.f;
		float HeightPixelsPerAngle = pOverlayWindow.Height / 178.f;
		float WidthPixelsPerAngle = pOverlayWindow.Width / 358.f;
		CenterX -= (HeightPixelsPerAngle*(pESP.LocalPlayerEntityInfo.PunchAngle.y * 2.f));
		CenterY += (WidthPixelsPerAngle*(pESP.LocalPlayerEntityInfo.PunchAngle.x * 2.f));
		if (pGlobalVars.EspSettings.RecoilMarkerType == 1)
		{
			pD3D.Functions.DrawCircle(CenterX, CenterY, pGlobalVars.EspSettings.RecoilMarkerSize, pGlobalVars.EspSettings.RecoilMarkerLineWidth, pGlobalVars.EspSettings.RecoilMarkerColor, true, pGlobalVars.EspSettings.RecoilMarkerOutlineWidth, pGlobalVars.EspSettings.RecoilMarkerOutlineColor);
		}
		if (pGlobalVars.EspSettings.RecoilMarkerType == 2)
		{
			pD3D.Functions.DrawCross(CenterX, CenterY, pGlobalVars.EspSettings.RecoilMarkerSize, pGlobalVars.EspSettings.RecoilMarkerLineWidth, pGlobalVars.EspSettings.RecoilMarkerColor, true, pGlobalVars.EspSettings.RecoilMarkerOutlineWidth, pGlobalVars.EspSettings.RecoilMarkerOutlineColor);
		}
	}
}

void tESP::tESPFunctions::Tick()
{
	pD3D.Functions.BeginRender();
	if (pGlobalVars.EspSettings.Enabled)
	{
		for (int PlayerNumber = 0; PlayerNumber < 64; PlayerNumber++)
		{
			pLocalPlayerEntity.Functions.GetLocalPlayerEntityInfo(&pESP.LocalPlayerEntityInfo);
			if (pESP.LocalPlayerEntityInfo.Valid)
			{
				pGeneralPlayerEntity.Functions.GetGeneralPlayerEntityInfo(&pESP.GeneralPlayerEntityInfo, PlayerNumber);
				if (pESP.GeneralPlayerEntityInfo.Valid && pESP.LocalPlayerEntityInfo.Index != PlayerNumber)
				{
					DrawBox(PlayerNumber);
					DrawHealthBar();
					DrawSnapLines(PlayerNumber);
					DrawInfoText(PlayerNumber);
					DrawTargetBoneMarker();
				}
				DrawRecoilMarker();
			}
		}
		pD3D.Functions.DrawString(pOverlayWindow.Width / 2.f, 10, pGlobalVars.Version, D3DCOLOR_ARGB(80, 255, 255, 255), true, D3DCOLOR_ARGB(120, 0, 0, 0), true);
		//pD3D.Functions.DrawString(5, 10, pGlobalVars.debuglog, D3DCOLOR_ARGB(255, 255, 0, 0), true, D3DCOLOR_ARGB(255, 0, 0, 0), false);
	}
	pD3D.Functions.EndRender();
}