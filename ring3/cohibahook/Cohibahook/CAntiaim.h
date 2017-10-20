#pragma once

class CAntiaim
{
public:
	void EdgeAntiAim1(CBaseEntity *pLocal, float &fYawOut);
	bool EdgeAntiAim(CBaseEntity* pLocalBaseEntity, ValveSDK::CInput::CUserCmd* cmd, float flWall, float flCornor);
	void FakeOrigin(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket);
	void Fake1(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket);
	void Fake2(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket);
	void Fake3(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket);
	void FakeJitter1(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket);
	void FakeJitter2(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket);
	void Jitter(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd);
	void doAntiaim(CBaseEntity* pLocal, ValveSDK::CInput::CUserCmd* cmd, bool &SendPacket);
	ValveSDK::CTrace::CTraceFilterNoPlayer tfNoPlayers;
private:
};

extern CAntiaim g_Antiaim;