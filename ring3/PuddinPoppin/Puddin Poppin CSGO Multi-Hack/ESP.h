#include "stdafx.h"

struct tESP
{
	std::wstring InfoText;
	D3DCOLOR InfoTextColor;
	D3DCOLOR InfoTextOutlineColor;
	tLocalPlayerEntity::tLocalPlayerEntityInfo LocalPlayerEntityInfo;
	tGeneralPlayerEntity::tGeneralPlayerEntityInfo GeneralPlayerEntityInfo;
    struct tDataStructs
    {
        struct tPlayerSkeleton
        {
            struct
            {
                D3DXVECTOR2 ScreenPosition[4];
            } Limbs[4];

            struct
            {
                D3DXVECTOR2 ScreenPosition[7];
            } Spine;
            bool Valid;
        };
    };
    struct tESPFunctions
    {
        D3DXVECTOR2 WorldToScreen(D3DXVECTOR3 EnemyPosition, tLocalPlayerEntity::tDataStructs::tViewMatrix ViewMatrix);
        tESP::tDataStructs::tPlayerSkeleton ParsePlayerSkeleton();
        void DrawBox(int PlayerNumber);
		void DrawSnapLines(int PlayerNumber);
		void DrawBones(int PlayerNumber);
		void DrawHealthBar();
		void DrawInfoText(int PlayerNumber);
		void DrawTargetBoneMarker();
		void DrawRecoilMarker();
        bool OnScreen(D3DXVECTOR2 ScreenPosition);
        void Tick();
    } Functions;
};

extern tESP pESP;