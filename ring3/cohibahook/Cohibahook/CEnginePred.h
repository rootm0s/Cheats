#include "main.h"

namespace ValveSDK
{
	class IMoveHelper
	{
	public:
		void SetHost(CBaseEntity *pPlayer)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *pPlayer);
			getvfunc<OriginalFn>(this, 1)(this, pPlayer);
		}
	};

	class CPrediction
	{
	public:
		void SetupMove(CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID empty, PVOID moveData)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID empty, PVOID moveData);
			getvfunc<OriginalFn>(this, 20)(this, player, ucmd, NULL, moveData);
		}
		void FinishMove(CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID moveData)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *player, ValveSDK::CInput::CUserCmd *ucmd, PVOID moveData);
			getvfunc<OriginalFn>(this, 21)(this, player, ucmd, moveData);
		}
	};

	class CGameMovement
	{
	public:
		void ProcessMovement(CBaseEntity *pPlayer, PVOID moveData)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *pPlayer, PVOID moveData);
			getvfunc<OriginalFn>(this, 1)(this, pPlayer, moveData);
		}
	};
}