#include "main.h"
#define M_PI	3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )

CAntiaim g_Antiaim;

void CAntiaim::EdgeAntiAim1(CBaseEntity *pLocal, float &fYawOut)
{
	Vector vEyePos = pLocal->GetEyePosition();

	ValveSDK::CTrace::ITraceFilter* tracefilter = (ValveSDK::CTrace::ITraceFilter*)&tfNoPlayers;

	for (int y = 0; y < 360; y++)
	{
		Vector qTmp(10.0f, fYawOut, 0.0f);
		qTmp.y += y;

		if (qTmp.y > 180.0)
			qTmp.y -= 360.0;
		else if (qTmp.y < -180.0)
			qTmp.y += 360.0;

		g_Aimbot.ClampAngles(qTmp);

		Vector vForward;

		g_Math.vectorAnglesVec(qTmp, vForward);

		float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
		vForward *= fLength;

		ValveSDK::CTrace::trace_t trData;

		Vector vTraceEnd = vEyePos + vForward;

		ValveSDK::CTrace::Ray_t ray;

		ray.Init(vEyePos, vTraceEnd);
		g_Valve.pKoloTrace->TraceRay(ray, 0x4600400B, tracefilter, &trData);


		if (trData.fraction != 1.0f)
		{
			Vector angles;

			Vector vNegative = Vector(trData.plane.normal.x * -1.0f, trData.plane.normal.y * -1.0f, trData.plane.normal.z * -1.0f);

			g_Math.vectorAnglesVec(vNegative, angles);

			g_Aimbot.NormalizeVector(angles);

			qTmp.y = angles.y;

			g_Aimbot.ClampAngles(qTmp);

			ValveSDK::CTrace::trace_t trLeft, trRight;

			Vector vLeft, vRight;
			g_Math.vectorAnglesVec(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
			g_Math.vectorAnglesVec(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);

			vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
			vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));

			vTraceEnd = vEyePos + vLeft;

			ray.Init(vEyePos, vTraceEnd);
			g_Valve.pKoloTrace->TraceRay(ray, 0x4600400B, tracefilter, &trLeft);

			vTraceEnd = vEyePos + vRight;

			ray.Init(vEyePos, vTraceEnd);
			g_Valve.pKoloTrace->TraceRay(ray, 0x4600400B, tracefilter, &trRight);

			if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
				qTmp.y -= 90.f;
			else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
				qTmp.y += 90.f;

			if (qTmp.y > 180.0)
				qTmp.y -= 360.0;
			else if (qTmp.y < -180.0)
				qTmp.y += 360.0;

			fYawOut = qTmp.y;

			if (fYawOut > 180.0)
				fYawOut -= 360.0;
			else if (fYawOut < -180.0)
				fYawOut += 360.0;

			//if (!bHeadOnWall)
			//	bHeadOnWall = true;

			return;
		}
	}

	fYawOut -= 180.0f;

	if (fYawOut > 180.0)
		fYawOut -= 360.0;
	else if (fYawOut < -180.0)
		fYawOut += 360.0;

	//if (bHeadOnWall)
	//	bHeadOnWall = false;
}

bool CAntiaim::EdgeAntiAim(CBaseEntity* pLocalBaseEntity, ValveSDK::CInput::CUserCmd* cmd, float flWall, float flCornor)
{
	ValveSDK::CTrace::Ray_t ray;
	ValveSDK::CTrace::trace_t tr;
	//ValveSDK::EngineTraceClient::CTraceFilter traceFilter(pLocalBaseEntity);
	//ValveSDK::CTrace::ITraceFilter *pTraceFilter = (ValveSDK::CTrace::ITraceFilter*)&tfNoPlayers;
	//traceFilter.pSkip = pLocalBaseEntity;

	bool ret = false;

	Vector localPosition = pLocalBaseEntity->GetEyePosition();

	for (int y = 0; y < 360; y++)
	{
		Vector tmp(10.0f, cmd->viewangles.y, 0.0f);
		tmp.y += y;

		g_Aimbot.NormalizeVector(tmp);

		Vector forward;
		g_Math.VectorAngles3D(forward, tmp);

		float length = ((16.0f + 3.0f) + ((16.0f + 3.0f) * sin(DEG2RAD(10.0f)))) + 7.0f;
		forward *= length;

		ray.Init(localPosition, (localPosition + forward));
		g_Valve.pKoloTrace->TraceRay(ray, 0x4600400B, (ValveSDK::CTrace::ITraceFilter*)&tfNoPlayers, &tr);

		if (tr.fraction != 1.0f)
		{
			Vector angles;
			Vector Negate = tr.plane.normal;

			Negate *= -1;
			g_Math.VectorAngles3D(Negate, angles);

			tmp.y = angles.y;

			g_Aimbot.NormalizeVector(tmp);
			ValveSDK::CTrace::trace_t leftTrace, rightTrace;

			Vector left, right;
			g_Math.VectorAngles3D(left, tmp + Vector(0.0f, 30.0f, 0.0f));
			g_Math.VectorAngles3D(right, tmp - Vector(0.0f, 30.0f, 0.0f));

			left *= (length + (length * sin(DEG2RAD(30.0f))));
			right *= (length + (length * sin(DEG2RAD(30.0f))));

			ray.Init(localPosition, (localPosition + left));
			g_Valve.pKoloTrace->TraceRay(ray, 0x4600400B, (ValveSDK::CTrace::ITraceFilter*)&tfNoPlayers, &leftTrace);

			ray.Init(localPosition, (localPosition + right));
			g_Valve.pKoloTrace->TraceRay(ray, 0x4600400B, (ValveSDK::CTrace::ITraceFilter*)&tfNoPlayers, &rightTrace);

			if ((leftTrace.fraction == 1.0f)
				&& (rightTrace.fraction != 1.0f))
			{
				tmp.y -= flCornor; // left
			}
			else if ((leftTrace.fraction != 1.0f)
				&& (rightTrace.fraction == 1.0f))
			{
				tmp.y += flCornor; // right
			}

			cmd->viewangles.y = tmp.y;
			cmd->viewangles.y -= flWall;
			ret = true;
		}
	}
	return ret;
}

void CAntiaim::FakeOrigin(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket)
{
	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	Vector vMove2;
	Vector vRealView(cmd->viewangles);
	g_Math.VectorAngles3D(vMove, vMove2);

	static int iChoked = -1;
	iChoked++;
	if (iChoked < g_pConsole->getValue("antiaim_choke"))
	{
		SendPacket = false;
		cmd->viewangles.y -= 92;
	}
	else
	{
		SendPacket = true;
		iChoked = -1;
	}

	flYaw = DEG2RAD(cmd->viewangles.y - vRealView.y + vMove2.y);
	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;
}

void CAntiaim::Fake1(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket)
{
	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	Vector vMove2;
	Vector vRealView(cmd->viewangles);
	g_Math.VectorAngles3D(vMove, vMove2);

	static int jitterangle = 0;

	if (jitterangle <= 2)
	{
		cmd->viewangles.x = 89;
		jitterangle += 1;
	}
	else if (jitterangle > 2 && jitterangle <= 4)
	{
		cmd->viewangles.x = 70;
		jitterangle += 1;
	}
	else
	{
		jitterangle = 0;
	}
	cmd->viewangles.y += 90;

	static int iChoked = -1;
	iChoked++;
	if (iChoked < g_pConsole->getValue("antiaim_choke"))
	{
		SendPacket = false;
		cmd->viewangles.y -= 180;
	}
	else
	{
		SendPacket = true;
		iChoked = -1;
	}

	flYaw = DEG2RAD(cmd->viewangles.y - vRealView.y + vMove2.y);
	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;
}

void CAntiaim::Fake2(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket)
{
	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	Vector vMove2;
	Vector vRealView(cmd->viewangles);
	g_Math.VectorAngles3D(vMove, vMove2);

	cmd->viewangles.x = 88;
	cmd->viewangles.y += 180;

	static int iChoked = -1;
	iChoked++;
	if (iChoked < g_pConsole->getValue("antiaim_choke"))
	{
		SendPacket = false;
		cmd->viewangles.y += 90;
	}
	else
	{
		SendPacket = true;
		iChoked = -1;
	}

	flYaw = DEG2RAD(cmd->viewangles.y - vRealView.y + vMove2.y);
	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;
}

void CAntiaim::Fake3(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket)
{
	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	Vector vMove2;
	Vector vRealView(cmd->viewangles);
	g_Math.VectorAngles3D(vMove, vMove2);

	cmd->viewangles.x = 88;
	cmd->viewangles.y += 180;

	static int iChoked = -1;
	iChoked++;
	if (iChoked < g_pConsole->getValue("antiaim_choke"))
	{
		SendPacket = false;
		cmd->viewangles.y -= 180;
	}
	else
	{
		SendPacket = true;
		iChoked = -1;
	}

	flYaw = DEG2RAD(cmd->viewangles.y - vRealView.y + vMove2.y);
	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;
}

void CAntiaim::FakeJitter1(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket)
{
	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	Vector vMove2;
	Vector vRealView(cmd->viewangles);
	g_Math.VectorAngles3D(vMove, vMove2);

	static int jitterangle = 0;

	if (jitterangle <= 1)
	{
		cmd->viewangles.x = 88;
		cmd->viewangles.y += 135;
	}
	else if (jitterangle > 1 && jitterangle <= 3)
	{
		cmd->viewangles.x = 88;
		cmd->viewangles.y += 225;
	}

	static int iChoked = -1;
	iChoked++;
	if (iChoked < g_pConsole->getValue("antiaim_choke"))
	{
		SendPacket = false;
		if (jitterangle <= 1)
		{
			cmd->viewangles.y += 45;
			jitterangle += 1;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			cmd->viewangles.y -= 45;
			jitterangle += 1;
		}
		else
		{
			jitterangle = 0;
		}
	}
	else
	{
		SendPacket = true;
		iChoked = -1;
	}
	g_Aimbot.NormalizeVector(cmd->viewangles);

	flYaw = DEG2RAD(cmd->viewangles.y - vRealView.y + vMove2.y);
	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;
}

void CAntiaim::FakeJitter2(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket)
{
	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	Vector vMove2;
	Vector vRealView(cmd->viewangles);
	g_Math.VectorAngles3D(vMove, vMove2);

	static int jitterangle = 0;

	if (jitterangle <= 1)
	{
		cmd->viewangles.x = 88;
		cmd->viewangles.y += 90;
	}
	else if (jitterangle > 1 && jitterangle <= 3)
	{
		cmd->viewangles.x = 88;
		cmd->viewangles.y -= 90;
	}

	static int iChoked = -1;
	iChoked++;
	if (iChoked < g_pConsole->getValue("antiaim_choke"))
	{
		SendPacket = false;
		if (jitterangle <= 1)
		{
			cmd->viewangles.y += 180;
			jitterangle += 1;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			cmd->viewangles.y -= 180;
			jitterangle += 1;
		}
		else
		{
			jitterangle = 0;
		}
	}
	else
	{
		SendPacket = true;
		iChoked = -1;
	}
	g_Aimbot.NormalizeVector(cmd->viewangles);

	flYaw = DEG2RAD(cmd->viewangles.y - vRealView.y + vMove2.y);
	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;
}

void CAntiaim::Jitter(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd)
{
	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	Vector vMove2;
	Vector vRealView(cmd->viewangles);
	g_Math.VectorAngles3D(vMove, vMove2);

	static int jitterangle = 0;

	if (jitterangle <= 1)
	{
		cmd->viewangles.x = 89;
		cmd->viewangles.y += 91;
		jitterangle += 1;
	}
	else if (jitterangle > 1 && jitterangle <= 3)
	{
		cmd->viewangles.x = 87;
		cmd->viewangles.y -= 89;
		jitterangle += 1;
	}
	else
	{
		jitterangle = 0;
	}

	flYaw = DEG2RAD(cmd->viewangles.y - vRealView.y + vMove2.y);
	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;
}

void CAntiaim::doAntiaim(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket)
{
	if (g_pConsole->getValue("antiaim_fake") == 1)
		FakeOrigin(pLocal, cmd, SendPacket);
	if (g_pConsole->getValue("antiaim_fake") == 2)
		Fake1(pLocal, cmd, SendPacket);
	if (g_pConsole->getValue("antiaim_fake") == 3)
		Fake2(pLocal, cmd, SendPacket);
	if (g_pConsole->getValue("antiaim_fake") == 4)
		Fake3(pLocal, cmd, SendPacket);
	if (g_pConsole->getValue("antiaim_fake") == 5)
		FakeJitter1(pLocal, cmd, SendPacket);
	if (g_pConsole->getValue("antiaim_fake") == 6)
		FakeJitter2(pLocal, cmd, SendPacket);
	if (g_pConsole->getValue("antiaim_fake") == 7)
		Jitter(pLocal, cmd);
}