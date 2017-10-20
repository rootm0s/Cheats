#include "stdafx.h"

struct tGlobalVars
{
    tGlobalVars();
    struct
    {
		DWORD CSPlayerResource;
		DWORD m_iCompetitiveRanking;
		DWORD m_iKills;
		DWORD m_iDeaths;
		DWORD m_bHasDefuser;
		DWORD m_iPlayerC4;
		DWORD m_ArmorValue;
        DWORD m_dwViewMatrix;
        DWORD m_dwEntityList;
        DWORD m_dwLocalPlayer;
        DWORD m_iHealth;
        DWORD m_vecOrigin;
        DWORD m_iTeamNum;
        DWORD m_lifeState;
        DWORD m_fFlags;
        DWORD m_dwBoneMatrix;
        DWORD m_vecPunch;
        DWORD m_bDormant;
        DWORD m_dwClientState;
        DWORD m_dwLocalPlayerIndex;
        DWORD m_iCrossHairID;
        DWORD m_dwViewAngle;
        DWORD m_iShotsFired;
        DWORD m_vecVelocity;
        DWORD m_vecViewOffset;
        DWORD m_dwRadarBase;
        DWORD m_dwRadarBasePointer;
        DWORD RadarName1;
        DWORD RadarName2;
        DWORD EntitySize;
    } Offsets;

    struct
    {

        std::wstring ReferenceWindowName;
        std::wstring OverlayWindowName;

    } OverlayWindowSettings;

    struct
    {
        bool Enabled;

        bool DrawEnemyBox;
        D3DCOLOR EnemyBoxColor;
        float EnemyBoxLineWidth;
        D3DCOLOR EnemyBoxOutlineColor;
        float EnemyBoxOutlineWidth;

        bool DrawFriendlyBox;
        D3DCOLOR FriendlyBoxColor;
        float FriendlyBoxLineWidth;
        D3DCOLOR FriendlyBoxOutlineColor;
        float FriendlyBoxOutlineWidth;

        bool DrawTargetBoxHighlight;
        D3DCOLOR TargetEnemyBoxColor;
        float TargetEnemyBoxLineWidth;
        D3DCOLOR TargetEnemyBoxOutlineColor;
        float TargetEnemyBoxOutlineWidth;

        bool DrawEnemySnapLines;
        D3DCOLOR EnemySnapLineColor;
        float EnemySnapLineWidth;
        D3DCOLOR EnemySnapLineOutlineColor;
        float EnemySnapLineOutlineWidth;

        bool DrawFriendlySnapLines;
        D3DCOLOR FriendlySnapLineColor;
        float FriendlySnapLineWidth;
        D3DCOLOR FriendlySnapLineOutlineColor;
        float FriendlySnapLineOutlineWidth;

        bool DrawTargetSnapLineHighlight;
        D3DCOLOR TargetEnemySnapLineColor;
        float TargetEnemySnapLineWidth;
        D3DCOLOR TargetEnemySnapLineOutlineColor;
        float TargetEnemySnapLineOutlineWidth;

        bool DrawEnemyHealthBar;
        D3DCOLOR EnemyHealthBarFrontColor;
        D3DCOLOR EnemyHealthBarBackColor;
        float EnemyHealthBarWidth;
        D3DCOLOR EnemyHealthBarOutlineColor;
        float EnemyHealthBarOutlineWidth;

        bool DrawFriendlyHealthBar;
        D3DCOLOR FriendlyHealthBarFrontColor;
        D3DCOLOR FriendlyHealthBarBackColor;
        float FriendlyHealthBarWidth;
        D3DCOLOR FriendlyHealthBarOutlineColor;
        float FriendlyHealthBarOutlineWidth;

        bool DrawEnemyNameText;
		bool DrawEnemyHealthText;
		bool DrawEnemyArmorText;
		bool DrawEnemyKillsText;
		bool DrawEnemyDeathsText;
		bool DrawEnemyKDRatioText;
		bool DrawEnemyRankText;
        D3DCOLOR EnemyInfoTextColor;
        D3DCOLOR EnemyInfoTextOutlineColor;
		bool DrawEnemyBombCarrierText;
		bool DrawEnemyDefuseKitCarrierText;

		bool DrawFriendlyNameText;
		bool DrawFriendlyHealthText;
		bool DrawFriendlyArmorText;
		bool DrawFriendlyKillsText;
		bool DrawFriendlyDeathsText;
		bool DrawFriendlyKDRatioText;
		bool DrawFriendlyRankText;
        D3DCOLOR FriendlyInfoTextColor;
        D3DCOLOR FriendlyInfoTextOutlineColor;
		bool DrawFriendlyBombCarrierText;
		bool DrawFriendlyDefuseKitCarrierText;

		bool DrawEnemyTargetBoneMarker;
		D3DCOLOR EnemyTargetBoneMarkerColor;
		float EnemyTargetBoneMarkerLineWidth;
		float EnemyTargetBoneMarkerSize;
		int EnemyTargetBoneMarkerType;
		D3DCOLOR EnemyTargetBoneMarkerOutlineColor;
		float EnemyTargetBoneMarkerOutlineWidth;

		bool DrawFriendlyTargetBoneMarker;
		D3DCOLOR FriendlyTargetBoneMarkerColor;
		float FriendlyTargetBoneMarkerLineWidth;
		float FriendlyTargetBoneMarkerSize;
		int FriendlyTargetBoneMarkerType;
		D3DCOLOR FriendlyTargetBoneMarkerOutlineColor;
		float FriendlyTargetBoneMarkerOutlineWidth;

        bool DrawRecoilMarker;
        D3DCOLOR RecoilMarkerColor;
        float RecoilMarkerLineWidth;
        float RecoilMarkerSize;
        int RecoilMarkerType;
        D3DCOLOR RecoilMarkerOutlineColor;
        float RecoilMarkerOutlineWidth;

    } EspSettings;

    struct
    {
        bool Enabled;
        float SmoothPercent;
        bool EnableRandomYawRecoilReductionFactor;
        float MinRandomYaw;
        float MaxRandomYaw;
        float StaticYaw;
        bool EnableRandomPitchRecoilReductionFactor;
        float MinRandomPitch;
        float MaxRandomPitch;
        float StaticPitch;
        int TargetBone;
        int IgnoreShotCount;
        int ActivationKey;
        bool EnableTimeout;
        int Timeout;
        bool EnableFire;
        bool TargetJumpingPlayers;
    } AimbotSettings;

    struct
    {
        bool Enabled;
        int ActivationKey;
        int FirstShotDelay;
        int FollowUpDelay;
        bool EnableTimeout;
		bool EnableRecoilLimit;
		float YawRecoilLimit;
		float PitchRecoilLimit;
        int Timeout;
    } TriggerbotSettings;

	struct
	{
		bool Enabled;
		int ActivationKey;
	} BhopSettings;

    struct
    {
        std::wstring TargetProcessName;
        std::wstring ClientModuleName;
        std::wstring EngineModuleName;
        std::wstring SettingsPath;
    } IOSettings;

	std::wstring debuglog = L"DEBUG LOG:\n";
	std::wstring Version = L"PP Multi-Hack V: 1.2";
    bool KILLALL;
};

extern tGlobalVars pGlobalVars;