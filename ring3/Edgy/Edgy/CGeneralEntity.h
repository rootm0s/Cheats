#ifndef CGENERALENTITY_H
#define CGENERALENTITY_H
#include <Windows.h>
#include "Offsets.h"
#include "Interfaces.h"
#include <d3dx9.h>

struct tDataStructs {
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
};

typedef struct
{
	float flMatrix[4][4];
}WorldToScreenMatrix_t;

/*
				General entity.
NON-LOCAL USE:
				PRE:
						Construct a CGeneralEntity object
						Set object's LocalPlayer
				POST:
						Call SetAndValidate
*/
class CGeneralEntity {
public:
	CGeneralEntity(); // Default constructor
	CGeneralEntity(DWORD); //NON-LOCAL constructor
	~CGeneralEntity(); // Destructor

	bool SetAndValidate();
	bool SetAndValidateEntity();
	void getClassID();
	D3DXVECTOR3 GetBonePosition(int);

	DWORD LocalPlayer, BoneMatrixAddress, ClassID;
	BYTE Flags;
	bool Dormant, LifeState, Spotted;
	float FOV;
	int TeamNumber, localPlayerWeaponID;
	WorldToScreenMatrix_t WorldToScreenMatrix;
	tDataStructs::tBoneMatrix BoneMatrix;
	D3DXVECTOR3 ViewAngles, VecOrigin, VecView, LocalPos, PunchAngles, Velocity /*VecOrigin + VecView*/;
	
	//D3DXVECTOR3 vecForce;

#pragma region WEAPON CHECKS
	bool isNonAimWeapon();
	int getWeaponId();
	DWORD getCurrentWeapon();
	int getWeapEnt();
	DWORD getHandleWeap();
	bool isWeaponKnife();
	bool isWeaponGrenade();
	bool isWeaponBomb();
	bool isWeaponTaser();
	bool isWeaponShotgun();
	bool isWeaponRifle();
	bool isWeaponSniper();
	bool isWeaponPistol();
#pragma endregion
};

#endif