#include "stdafx.h"

volatile struct tLocalPlayerEntity
{

    struct tDataStructs
    {
        struct tViewMatrix
        {
            float Matrix[4][4];
        };
		struct tDataChunk
		{
			byte Data[70000];
		};
    };
	struct tLocalPlayerEntityInfo
	{
		tDataStructs::tDataChunk DataChunk;
		bool Valid;
		byte Flags;
		DWORD BaseAddress;
		bool LifeState;
		int TeamNumber;
		int ShotsFired;
		int CrosshairEntityIndex;
		int Index;
		D3DXVECTOR3 Origin;
		D3DXVECTOR3 PunchAngle;
		D3DXVECTOR3 ViewOffset;
		D3DXVECTOR3 ViewAngle;
		D3DXVECTOR3 Velocity;
		DWORD ClientStateBaseAddress;
		tLocalPlayerEntity::tDataStructs::tViewMatrix ViewMatrix;
	};
    struct tLocalPlayerEntityFunctions
    {
		bool IsJumping(byte Flags);
        void SetViewAngle(DWORD BaseAddress, DWORD Offset, D3DXVECTOR3 ViewAngle);
		void GetLocalPlayerEntityInfo(tLocalPlayerEntityInfo* LocalPlayerEntityInfo);
    } Functions;

};

extern tLocalPlayerEntity pLocalPlayerEntity;