#ifndef __SOURCESDK_RESOURCE_H__
#define __SOURCESDK_RESOURCE_H__

#ifdef _MSC_VER
#pragma once
#endif

namespace ValveSDK
{
	namespace resource
	{
		/************************************************************************/
		/* Bone Index defines                                                   */
		/************************************************************************/
		enum BoneList
		{
			BONE_PELVIS,
			BONE_SPINE1,
			BONE_SPINE2,
			BONE_SPINE3,
			BONE_SPINE4,
			BONE_NECK,
			BONE_L_CLAVICLE,
			BONE_L_UPPER_ARM,
			BONE_L_FOREARM,
			BONE_L_HAND,
			BONE_HEAD,
			BONE_FORWARD,
			BONE_R_CLAVICLE,
			BONE_R_UPPER_ARM,
			BONE_R_FOREARM,
			BONE_R_HAND,
			BONE_WEAPON,
			BONE_WEAPON_SLIDE,
			BONE_WEAPON_R_HAND,
			BONE_WEAPON_L_HAND,
			BONE_WEAPON_CLIP1,
			BONE_WEAPON_CLIP2,
			BONE_SILENCER,
			BONE_R_THIGH,
			BONE_R_CALF,
			BONE_R_FOOT,
			BONE_L_THIGH,
			BONE_L_CALF,
			BONE_L_FOOT,
			BONE_L_FORETWIST,
			BONE_L_CALFTWIST,
			BONE_R_CALFTWIST,
			BONE_L_THIGHTWIST,
			BONE_R_THIGHTWIST,
			BONE_L_UPARMTWIST,
			BONE_R_UPARMTWIST,
			BONE_R_FORETWIST,
			BONE_R_TOE,
			BONE_L_TOE,
			BONE_R_FINGER01,
			BONE_R_FINGER02,
			BONE_R_FINGER03,
			BONE_R_FINGER04,
			BONE_R_FINGER05,
			BONE_R_FINGER06,
			BONE_R_FINGER07,
			BONE_R_FINGER08,
			BONE_R_FINGER09,
			BONE_R_FINGER10,
			BONE_R_FINGER11,
			BONE_R_FINGER12,
			BONE_R_FINGER13,
			BONE_R_FINGER14,
			BONE_L_FINGER01,
			BONE_L_FINGER02,
			BONE_L_FINGER03,
			BONE_L_FINGER04,
			BONE_L_FINGER05,
			BONE_L_FINGER06,
			BONE_L_FINGER07,
			BONE_L_FINGER08,
			BONE_L_FINGER09,
			BONE_L_FINGER10,
			BONE_L_FINGER11,
			BONE_L_FINGER12,
			BONE_L_FINGER13,
			BONE_L_FINGER14,
			BONE_L_FINGER15,
			BONE_R_FINGER15,
			BONE_MAX
		};
		/************************************************************************/
		/* Hit box Index defines                                                */
		/************************************************************************/
		enum HitboxList
		{
			HITBOX_PELVIS,
			HITBOX_L_THIGH,
			HITBOX_L_CALF,
			HITBOX_L_FOOT,
			HITBOX_R_THIGH,
			HITBOX_R_CALF,
			HITBOX_R_FOOT,
			HITBOX_SPINE1,
			HITBOX_SPINE2,
			HITBOX_SPINE3,
			HITBOX_NECK,
			HITBOX_HEAD,
			HITBOX_L_UPPERARM,
			HITBOX_L_FOREARM,
			HITBOX_L_HAND,
			HITBOX_R_UPPERARM,
			HITBOX_R_FOREARM,
			HITBOX_R_HAND,
			HITBOX_L_CLAVICLE,
			HITBOX_R_CLAVICLE,
			HITBOX_HELMET,
			HITBOX_SPINE4,
			HITBOX_MAX,
		};
		/************************************************************************/
		/* ItemSchemaIndex defines                                              */
		/************************************************************************/
		enum ItemSchemaIndex_e
		{
			ITEM_NONE = 0,
			WEAPON_DEAGLE = 1,
			WEAPON_DUALS = 2,
			WEAPON_FIVE7 = 3,
			WEAPON_GLOCK = 4,
			WEAPON_AK47 = 7,
			WEAPON_AUG = 8,
			WEAPON_AWP = 9,
			WEAPON_FAMAS = 10,
			WEAPON_G3SG1 = 11,
			WEAPON_GALIL = 13,
			WEAPON_M249 = 14,
			WEAPON_M4A1 = 16,
			WEAPON_MAC10 = 17,
			WEAPON_P90 = 19,
			WEAPON_UMP45 = 24,
			WEAPON_XM1014 = 25,
			WEAPON_BIZON = 26,
			WEAPON_MAG7 = 27,
			WEAPON_NEGEV = 28,
			WEAPON_SAWEDOFF = 29,
			WEAPON_TEC9 = 30,
			WEAPON_TASER = 31,
			WEAPON_P2000 = 32,
			WEAPON_MP7 = 33,
			WEAPON_MP9 = 34,
			WEAPON_NOVA = 35,
			WEAPON_P250 = 36,
			WEAPON_SCAR20 = 38,
			WEAPON_SG553 = 39,
			WEAPON_SCOUT = 40,
			WEAPON_KNIFE_T = 42,
			WEAPON_FLASH = 43,
			WEAPON_HE = 44,
			WEAPON_SMOKE = 45,
			WEAPON_MOLOTOV = 46,
			WEAPON_DECOY = 47,
			WEAPON_INC = 48,
			WEAPON_C4 = 49,
			WEAPON_KNIFE_CT = 59,
			WEAPON_M4A1S = 60,
			WEAPON_USPS = 61,
			WEAPON_CZ75 = 63,
			WEAPON_MAX = 64,
			WEAPON_KNIFE_BAYONET = 500,
			WEAPON_KNIFE_FLIP = 505,
			WEAPON_KNIFE_GUT = 506,
			WEAPON_KNIFE_KARAMBIT = 507,
			WEAPON_KNIFE_M9BAYONET = 508,
			WEAPON_KNIFE_HUNTSMAN = 509,
			WEAPON_KNIFE_BUTTERFLY = 515,
		};
		/************************************************************************/
		/* ItemSchemaWeaponType defines                                         */
		/************************************************************************/
		enum ItemSchemaWeaponType
		{
			WEAPON_TYPE_INVALID = -1,
			WEAPON_TYPE_PISTOLS,
			WEAPON_TYPE_SHOTGUN,
			WEAPON_TYPE_SMG,
			WEAPON_TYPE_RIFLE,
			WEAPON_TYPE_MG,
			WEAPON_TYPE_SNIPER,
			WEAPON_TYPE_GRENADE,
			WEAPON_TYPE_KNIFE,
		};
	}
}

#endif