#pragma once
#include "main.h"

void StartGlow(DWORD GlowPointer, int iGlow, float r, float g, float b, float a) {
	*(float*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x4) = (1 / 255.f)*r;
	*(float*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x8) = (1 / 255.f)*g;
	*(float*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0xC) = (1 / 255.f)*b;
	*(float*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x10) = (1 / 255.f)*a;

	*(bool*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x24) = true;
	*(bool*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x25) = false;
	*(bool*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x26) = false;
}

void DrawLine2D(Vector& start, Vector& end, Color dat) {
	DrawLine(start.x, start.y, end.x, end.y, dat);
}

float DotProductT(const float* a, const float* b)
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}
void VectorTransformA(const float *in1, const matrix3x4& in2, float *out)
{

	out[0] = DotProductT(in1, in2[0]) + in2[0][3];
	out[1] = DotProductT(in1, in2[1]) + in2[1][3];
	out[2] = DotProductT(in1, in2[2]) + in2[2][3];
}

void VectorTransformA2(const float *in1, const matrix3x4& in2, float *out)
{

	out[0] = DotProductT(in1, in2[0]) + in2[0][3];
	out[1] = DotProductT(in1, in2[1]) + in2[1][3];
	out[2] = DotProductT(in1, in2[2]) + in2[2][3];
}

inline void VectorTransformZ2(const Vector& in1, const matrix3x4 &in2, Vector &out)
{
	VectorTransformA2(&in1.x, in2, &out.x);
}


inline void VectorTransformZ(const Vector& in1, const matrix3x4 &in2, Vector &out)
{
	VectorTransformA(&in1.x, in2, &out.x);
}

void DrawPlayerBox(CBaseEntity* pEntity, Color dat, int x, int y, player_info_t pInfo, UICheckBox vis_acti, UICheckBox vis_box, UICheckBox vis_name, UICheckBox vis_health, UICheckBox vis_weapon)
{
	DWORD m_rgflCoordinateFrame = (DWORD)0x470 - 0x30;//int 
	const matrix3x4& trnsf = *(matrix3x4*)((DWORD)pEntity + (DWORD)m_rgflCoordinateFrame);


	Vector min = *reinterpret_cast< Vector* >((DWORD)pEntity + (DWORD)0x318 + 0x8);//m_angEyeAngles[0] //0x239C)
	Vector max = *reinterpret_cast< Vector* >((DWORD)pEntity + (DWORD)0x318 + 0x14);



	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };



	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++)
	{
		VectorTransformZ(points[i], trnsf, pointsTransformed[i]);
	}


	Vector pos = pEntity->GetAbsOrigin();
	Vector flb;
	Vector brt;
	Vector blb;
	Vector frt;
	Vector frb;
	Vector brb;
	Vector blt;
	Vector flt;


	if (!WorldToScreen(pointsTransformed[3], flb) || !WorldToScreen(pointsTransformed[5], brt)
		|| !WorldToScreen(pointsTransformed[0], blb) || !WorldToScreen(pointsTransformed[4], frt)
		|| !WorldToScreen(pointsTransformed[2], frb) || !WorldToScreen(pointsTransformed[1], brb)
		|| !WorldToScreen(pointsTransformed[6], blt) || !WorldToScreen(pointsTransformed[7], flt))
		return;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;        // left
	float top = flb.y;        // top
	float right = flb.x;    // right
	float bottom = flb.y;    // bottom

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	char playerName[32];
	sprintf(playerName, "%s", pInfo.name);

	if (vis_acti.getValve()) {
		if (vis_name.getValve())
		{
			DrawStringMiddle(pFont, true, x, y, Color(255, 255, 255, 255), pInfo.name);
		}

		if (vis_box.getValve())
		{

			DrawLine2D(Vector(left, bottom, 0), Vector(left, top, 0), dat);
			DrawLine2D(Vector(left, top, 0), Vector(right, top, 0), dat);
			DrawLine2D(Vector(right, top, 0), Vector(right, bottom, 0), dat);
			DrawLine2D(Vector(right, bottom, 0), Vector(left, bottom, 0), dat);
		}

	}

}

void RenderESP(UICheckBox vis_acti, UICheckBox vis_box, UICheckBox vis_name, UICheckBox vis_health, UICheckBox vis_weapon, UICheckBox vis_skele) {
	if (pEngine->IsInGame() && pEngine->IsConnected()) {
		if (vis_acti.getValve()) {
			C_BaseEntity* Local = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
			if (!Local)
				return;

			for (int i = 1; i < pEntList->GetHighestEntityIndex(); i++) {
				C_BaseEntity* Entity = (C_BaseEntity*)pEntList->GetClientEntity(i);

				if (!Entity)
					continue;

				if (Entity->IsDormant())
					continue;

				if (Entity->Health() < 1)
					continue;

				if (Entity == Local) // Varför skulle vi vilja ha esp på våran egna gubbe? kek.
					continue;

				Vector mon, nom;

				nom = Entity->GetAbsOrigin();

				if (Entity->Flags() & FL_DUCKING) {
					mon = nom + Vector(0, 0, 50.f);
				}
				else {
					mon = nom + Vector(0, 0, 70.f);
				}
				Vector bot, top;
				player_info_t pInfo;
				pEngine->GetPlayerInfo(i, &pInfo);
				if (WorldToScreen(nom, bot) && WorldToScreen(mon, top)) {
					float h = (bot.y - top.y);
					float w = h / 3.5f;

					if (Entity->Team() == 2) {
						if (vis_box.getValve()) {
							EdgedOutline(top.x - w, top.y, w * 2, h, Color(155, 0, 0, 255));
						}
						//	DrawPlayerBox(Entity, Color(155, 0, 0, 255), top.x, top.y, pInfo, vis_acti, vis_box, vis_name, vis_health, vis_weapon);
						if (vis_health.getValve()) {
							DrawHeader(top.x - w, top.y + h + 2, w * 2, 155, 0, 0, Entity->Health());
						}
						if (vis_name.getValve()) {
							DrawStringMiddle(pFont, true, top.x, top.y - 5, Color(240, 240, 240, 255), pInfo.name);
						}
						
					}
					if (Entity->Team() == 3) {

						if (vis_box.getValve()) {
							EdgedOutline(top.x - w, top.y, w * 2, h, Color(0, 155, 155, 255));
						}
						//	DrawPlayerBox(Entity, Color(0, 155, 155, 255), top.x, top.y, pInfo, vis_acti, vis_box, vis_name, vis_health, vis_weapon);
						if (vis_health.getValve()) {
							DrawHeader(top.x - w, top.y + h + 2, w * 2, 0, 155, 155, Entity->Health());
						}
						if (vis_name.getValve()) {
							DrawStringMiddle(pFont, true, top.x, top.y - 5, Color(240, 240, 240, 255), pInfo.name);
						}
					}
				}
				if (glow_a.getValue() > 0) {
					DWORD gGlowPointer = *reinterpret_cast<DWORD*>((DWORD)GetModuleHandleA("client.dll") + (DWORD)0x4FAD8EC);

					if (Entity->Team() == 2) {
						StartGlow(gGlowPointer, Entity->GlowIndex(), 192, 175, 97, glow_a.getValue());
					}

					if (Entity->Team() == 3) {
						StartGlow(gGlowPointer, Entity->GlowIndex(), 82, 133, 188, glow_a.getValue());
					}
				}
			}
		}
	}
}