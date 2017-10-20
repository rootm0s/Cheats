#include "stdafx.h"

struct tGeneralPlayerEntity
{
    struct tDataStructs
    {
        struct tPlayerName
        {
            wchar_t Text[32];
        };
        struct tBoneMatrix
        {
            byte pad1[12];
            struct
            {
                float x;
                byte pad1[12];
                float y;
                byte pad2[12];
                float z;
                byte pad3[12];
            } Bones[128];
        };
		struct tDataChunk
		{
			byte Data[50000];
		};
    };
	struct tGeneralPlayerEntityInfo
	{
		tDataStructs::tDataChunk DataChunk1;
		tDataStructs::tDataChunk DataChunk2;
		DWORD BaseAddress;
		bool LifeState;
		int TeamNumber;
		int Health;
		D3DXVECTOR3 Origin;
		DWORD BoneMatrixBaseAddress;
		D3DXVECTOR3 Velocity;
		tDataStructs::tBoneMatrix BoneMatrix;
		int Kills;
		int Deaths;
		bool Valid;
		int HasC4;
		int Armor;
		bool HasDefuser;
		int CompetetiveRankNumber;
		std::wstring CompetetiveRank;
		DWORD GameResourcesBaseAddress;
		bool Dormant;
		byte Flags;
		tDataStructs::tPlayerName PlayerName;
		DWORD RadarBaseAddress;
		DWORD RadarPointerBaseAddress;
	};
    struct tGeneralPlayerEntityFunctions
    {
		bool IsJumping(byte Flags);
		std::wstring GetCompetetiveRank(int CompetetiveRankNumber);
		void GetGeneralPlayerEntityInfo(tGeneralPlayerEntityInfo* GeneralPlayerEntityInfo, int PlayerNumber);
		D3DXVECTOR3 GetBonePosition(tGeneralPlayerEntity::tDataStructs::tBoneMatrix* BoneMatrix, int BoneNumber);
    } Functions;
};

extern tGeneralPlayerEntity pGeneralPlayerEntity;