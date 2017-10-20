#pragma once
#include "main.h"
#ifndef CONCAT_IMPL
#define CONCAT_IMPL(x, y) x##y
#ifndef MACRO_CONCAT
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#ifndef PAD
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];
#endif
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CHudTexture
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CHudTexture
{
public:
	PAD(0x82);
	char    m_szCharacter;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CWeaponData
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CWeaponData
{
public:
	char                    m_szPrintName[32];                            // 0x0000
	PAD(0x158);
	int                             m_iBucket;                                                      // 0x0178
	int                             m_iBucketPosition;                                      // 0x017C
	int                             m_iClipSize;                                            // 0x0180
	int                             m_iDefaultClip1;                                        // 0x0184
	int                             m_iDefaultClip2;                                        // 0x0188
	PAD(0x4);
	int                             m_iWeight;                                                      // 0x0190
	int                             m_iRumble;                                                      // 0x0194
	PAD(0x78);
	char                    m_szEmpty[80];                                        // 0x0210
	char                    m_szSingleShot[80];                           // 0x0260
	PAD(0x140);
	char                    m_szReload[80];                                       // 0x03F0
	PAD(0x2E0);
	CHudTexture*    m_pWeaponIcon;                                          // 0x0720
	CHudTexture*    m_pWeaponSIcon;                                         // 0x0724
	CHudTexture*    m_pAmmoIcon;                                            // 0x0728
	PAD(0x2C);
	float                   m_flHeatPerShot;                                        // 0x0758
	int                             m_iWeaponPrice;                                         // 0x075C
	float                   m_flWeaponArmorRatio;                           // 0x0760
	float                   m_flMaxPlayerSpeed;                                     // 0x0764
	float                   m_flMaxPlayerSpeed2;                            // 0x0768
	int                             m_iCrosshairMinDist;                            // 0x076C
	int                             m_iCrosshairDeltaDist;                          // 0x0770
	float                   m_flPenetration;                                        // 0x0774
	int                             m_iDamage;                                                      // 0x0778
	float                   m_flRange;                                                      // 0x077C
	float                   m_flRangeModifier;                                      // 0x0780
	int                             m_iBullet;                                                      // 0x0784
	float                   m_flCycleTime;                                          // 0x0788
	PAD(0x184);
	char                    m_szViewModel[32];                            // 0x0910
	float                   m_flSpread;                                                     // 0x0930
	float                   m_flSpreadAlt;                                          // 0x0934
	float                   m_flInacurracyCrouch;                           // 0x0938
	float                   m_flInaccuracyCrouchAlt;                        // 0x093C
	float                   m_flInaccuracyStand;                            // 0x0940
	float                   m_flInaccuracyStandAlt;                         // 0x0944
	float                   m_flInaccuracyJump;                                     // 0x0948
	float                   m_flInaccuracyJumpAlt;                          // 0x094C
	float                   m_flInaccuracyLand;                                     // 0x0950
	float                   m_flInaccuracyLandAlt;                          // 0x0954
	float                   m_flInaccuracyLadder;                           // 0x0958
	float                   m_flInaccuracyLadderAlt;                        // 0x095C
	float                   m_flInaccuracyFire;                                     // 0x0960
	float                   m_flInaccuracyFireAlt;                          // 0x0964
	float                   m_flInaccuracyMove;                                     // 0x0968
	float                   m_flInaccuracyMoveAlt;                          // 0x096C
	PAD(0x10);
	float                   m_flRecoilAngle;                                        // 0x0980
	PAD(0x4);
	float                   m_flRecoilAngleVariance;                        // 0x0988
	PAD(0x4);
	float                   m_flRecoilMagnitude;                            // 0x0990
	PAD(0x4);
	float                   m_flRecoilMagnitudeVariance;            // 0x0998
	PAD(0x8);
	float                   m_flFlinchVelocityModifierLarge;        // 0x09A4
	float                   m_flFlinchVelocityModifierSmall;        // 0x09A8
	PAD(0x408);
	int                             m_iZoomLevels;                                          // 0x0DB4
	PAD(0x4);
	int                             m_iZoomFov1;                                            // 0x0DBC
	int                             m_iZoomFov2;                                            // 0x0DC0
	float                   m_flZoomTime0;                                          // 0x0DC4
	float                   m_flZoomTime1;                                          // 0x0DC8
	float                   m_flZoomTime2;                                          // 0x0DCC
	PAD(0x220);
};