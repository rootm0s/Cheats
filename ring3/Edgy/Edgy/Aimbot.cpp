#include "Aimbot.h"


void Aimbot::Tick(CGeneralEntity e) {
	//Do aimbot
	//If Aimbot Enables
	if (GetAsyncKeyState(0x01) & 0x8000) { // If Mouse1 is held down, start the aimbot
		if (e.LocalPlayer != 0) { // Checking if localplayer exists
			if (IsTargetStillValid(e)) { //Check for index localplayer = 0.  if != 0, update FOV.  If FOV > val, returns false and calls GetTarget.
				if (timer > 50.f) // Aimbot timed out
					return;
				float YawRecoilReductionFactor, PitchRecoilReductionFactor, AimspotRandomizer, CurveRandomizer;
				YawRecoilReductionFactor = GetRandomFloat(1.9, 2.05); // Randomized Y axis RCS
				PitchRecoilReductionFactor = GetRandomFloat(1.9, 2.05); // Randomized X axis RCS

				if (!InitialBoneSet || TargetIndex.Velocity.x > 0) { // Recalculate target's bone position if target moves or no bone has been set yet.
					if (TargetIndex.Velocity.x > 0 && RandomizedBone.x != NULL) { // Moving target, reassign bone position
						RandomizedBone = TargetIndex.GetBonePosition(8); // Recalculate bone position
						RandomizedBone.x += xOffset; // Assign original x offset 
						RandomizedBone.z += zOffset; // Assign original y offset 
					}
					else { // No initial bone position set, calculate it
						RandomizedBone = RandomizeBoneSpot(TargetIndex.GetBonePosition(8), 6, 8); // Calculate the bone position with a randomized x offset
						xOffset = RandomizedBone.x - TargetIndex.GetBonePosition(8).x; // Save previous x value in case entity starts moving
						zOffset = RandomizedBone.z - TargetIndex.GetBonePosition(8).z; // Save previous x value in case entity starts moving
						InitialBoneSet = true; // Bone position has been set
					}
				}

				if (e.isWeaponRifle() || e.isWeaponPistol()) { // Aimbot if current held weapon is a rifle or a pistol
					{
						if (aimbotTimer <= 0.999995f) { // Checking if aimbot should be curved or not, curve
							SetAngle(Curve(e.ViewAngles, CalcAngleRCS(e.LocalPos, RandomizedBone, e.PunchAngles, e.ViewAngles, YawRecoilReductionFactor, PitchRecoilReductionFactor)));
							Sleep(9);
						}
						else { // Not curved.
							SetAngle(SmoothAngle(e.ViewAngles, CalcAngleRCS(e.LocalPos, RandomizedBone, e.PunchAngles, e.ViewAngles, YawRecoilReductionFactor, PitchRecoilReductionFactor), 97 - (timer / 100.f)));
						}
					}
				}
				timer += 1; // Increment aimbot timer
				aimbotTimer += 0.05f; // Increment curve timer
				return;
			}
			else {
				timer = 0.f;
				aimbotTimer = 0.f;
				InitialBoneSet = false;
				e.SetAndValidate();
				TargetIndex = GetTarget(e);
			}
		}
		else {
			timer = 0.f;
			aimbotTimer = 0.f;
		}
	}
	else {
		timer = 0.f;
		aimbotTimer = 0.f;
		TargetIndex.LocalPlayer = 0;
		InitialBoneSet = false;
		DirectionSet = false;
	}
}

bool Aimbot::IsTargetStillValid(CGeneralEntity e) {
	if (!TargetIndex.SetAndValidateEntity()) { //If the target is dormant or dead or not valid (0) return false
		return false;
	}
	if (!TargetIndex.Spotted)
		return false;
	TargetIndex.FOV = GetFOV(e.ViewAngles, e.LocalPos, TargetIndex.GetBonePosition(6), e.LocalPos); // Calculate valid targets FOV
	//std::cout << "FOV: "<<TargetIndex.FOV<<"\n";
	//std::cout << "Distance: " << get3DDistance(TargetIndex.LocalPos, e.LocalPos);
	if (TargetIndex.FOV > 17)
		return false;
	return true;
}

// Sorts through all players and finds the lowest FOV
CGeneralEntity Aimbot::GetTarget(CGeneralEntity e) {
	CGeneralEntity LowestFOVPlayer(0);
	TargetIndex = LowestFOVPlayer; // Set default targetindex to NULL localplayer
	float LowestFOV = FLT_MAX;
	for (int i = 0; i <= 128; i++) { // Loop through all entities
		CGeneralEntity temp(b.Read<DWORD>(I.ClientDLL + m_dwEntityList + (i*m_entityLoop))); // Temporary entity in loop
		temp.SetAndValidateEntity(); // Assign values to the entity
		if (temp.LocalPlayer == e.LocalPlayer) // NOT ME
			continue;
		if (!temp.SetAndValidateEntity())
			continue;
		if (temp.ClassID != 35)
			continue;
		if (!temp.Spotted) // SPOTTED CHECK
			continue;
		if (temp.TeamNumber == e.TeamNumber) // TEAM CHECK
			continue;

		float CurrentIndexFOV = GetFOV(e.ViewAngles, e.LocalPos, temp.GetBonePosition(6), e.LocalPos); // GET FOV OF THE TARGET
		if (CurrentIndexFOV < LowestFOV) { // IF THE ENTITY IN THIS LOOP < CURRENTMAXFOV, UPDATE CURRENT
			LowestFOV = CurrentIndexFOV;
			LowestFOVPlayer = temp;
		}
	}
	LowestFOVPlayer.FOV = LowestFOV;
	return LowestFOVPlayer;

}

D3DXVECTOR3 Aimbot::RandomizeBoneSpot(D3DXVECTOR3 Matrix, float randomx, float randomz) {
	Matrix.x += GetRandomFloat(randomx*-1, randomx);
	Matrix.z += GetRandomFloat(randomz*-1, randomz);
	return Matrix;
}

float Aimbot::GetFOV(D3DXVECTOR3 &angle, D3DXVECTOR3 &src, D3DXVECTOR3 &dst, D3DXVECTOR3 myPos) { //My view angles, my eye position, enemy eye position
	float fRealDistance, fPlayerDistance, fYawDifference, fPitchDifference;
	D3DXVECTOR3 targetAngles;
	calcAngle(src, dst, targetAngles);
	fPlayerDistance = get3DDistance(myPos, dst);
	fPitchDifference = sin(D3DXToRadian(angle.x - targetAngles.x)) * fPlayerDistance;
	fYawDifference = sin(D3DXToRadian(angle.y - targetAngles.y)) * fPlayerDistance;
	return sqrt(powf(fPitchDifference, 2.0) + powf(fYawDifference, 2.0));
}



void Aimbot::calcAngle(D3DXVECTOR3 &src, D3DXVECTOR3 &dst, D3DXVECTOR3 &angles)
{
	double delta[3] = { (src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

	angles[0] = (float)(atan(delta[2] / hyp) * M_RADPI);
	angles[1] = (float)(atan(delta[1] / delta[0]) * M_RADPI);
	angles[2] = 0.0f;

	if (delta[0] >= 0.0) { angles[1] += 180.0f; }
}

D3DXVECTOR3 Aimbot::CalcAngleRCS(D3DXVECTOR3 PlayerPosition, D3DXVECTOR3 EnemyPosition, D3DXVECTOR3 PunchAngle, D3DXVECTOR3 ViewOffset, float YawRecoilReductionFactor, float PitchRecoilReductionFactor)
{
	D3DXVECTOR3 AimAngle;
	D3DXVECTOR3 Delta = { (PlayerPosition.x - EnemyPosition.x), (PlayerPosition.y - EnemyPosition.y), ((PlayerPosition.z + ViewOffset.z) - EnemyPosition.z) };
	float hyp = sqrt(Delta.x * Delta.x + Delta.y * Delta.y);
	AimAngle.x = atanf(Delta.z / hyp) * 57.295779513082f - PunchAngle.x * YawRecoilReductionFactor;
	AimAngle.y = atanf(Delta.y / Delta.x) * 57.295779513082f - PunchAngle.y * PitchRecoilReductionFactor;
	AimAngle.z = 0.0f;
	if (Delta.x >= 0.0)
	{
		AimAngle.y += 180.0f;
	}
	ClampAngle(AimAngle);
	//Velocity compensation
	return AimAngle;
}

D3DXVECTOR3 Aimbot::VelocityCompensation(D3DXVECTOR3 &Position, D3DXVECTOR3 Velocity) {
	Position.x += Velocity.x/500;
	Position.y += Velocity.y/500;
	Position.z += Velocity.z/500;
	return Position;
}

//Credits to dude719
D3DXVECTOR3 Aimbot::Curve(D3DXVECTOR3 src, D3DXVECTOR3 dst) { //My View Angles, Aim Angles
	D3DXVECTOR3 delta = src - dst;
	ClampAngle (delta);

	float randValPt1 = 30.0f + GetRandomFloat(0.0f, 15.0f);
	float finalRandValPt1 = 2.f / randValPt1; // between 0.333 and 0.666 1.f
	D3DXVECTOR3 point1 = src + (delta * finalRandValPt1);
	ClampAngle(point1);

	float randValPt2 = 50.0f + GetRandomFloat(0.0f, 15.0f);
	float finalRandValPt2 = .2f / randValPt2; // between 0.018 and 0.025 .4
	D3DXVECTOR3 point2 = dst * (1.0f + finalRandValPt2);
	ClampAngle(point2);

	D3DXVECTOR3 CurvedAngle = CubicInterpolate(src, point1, point2, dst, aimbotTimer); // Bug here, at certain viewangles the cursor snaps in a random direction.
	//std::cout << CurvedAngle.y << "\n";
	return CurvedAngle;
}

int getPt(int n1, int n2, float perc)
{
	int diff = n2 - n1;

	return n1 + (diff * perc);
}

D3DXVECTOR3 Aimbot::SmoothAngle(D3DXVECTOR3 &SourceAngle, D3DXVECTOR3 &DestinationAngle, float SmoothPercent)
{
	if (SmoothPercent < 0.f)
	{
		SmoothPercent = 0.f;
	}
	D3DXVECTOR3 SmoothedAngle;
	SmoothedAngle = DestinationAngle - SourceAngle;
	ClampAngle(SmoothedAngle);
	SmoothedAngle = SourceAngle + SmoothedAngle / 100.f * (100.f - SmoothPercent);
	ClampAngle(SmoothedAngle);
	return SmoothedAngle;
}

void Aimbot::SetAngle(D3DXVECTOR3 ViewAngle) {
	WriteProcessMemory(b.hProcess, (LPVOID)(I.EnginePointer + m_dwViewAngles), &ViewAngle, sizeof(D3DXVECTOR3), 0);
}

void Aimbot::MakeVector(D3DXVECTOR3 &pfIn, D3DXVECTOR3 &pfOut)
{
	float pitch;
	float yaw;
	float tmp;

	pitch = (float)(pfIn[0] * M_PI / 180);
	yaw = (float)(pfIn[1] * M_PI / 180);
	tmp = (float)cos(pitch);

	pfOut[0] = (float)(-tmp * -cos(yaw));
	pfOut[1] = (float)(sin(yaw)*tmp);
	pfOut[2] = (float)-sin(pitch);
}

float Aimbot::get3DDistance(D3DXVECTOR3 src, D3DXVECTOR3 dst) {
	return sqrt(powf(src[0] - dst[0], 2.f) + powf(src[1] - dst[1], 2.f) + powf(src[2] - dst[2], 2.f));
}

float Aimbot::GetRandomFloat(float min, float max)
{
	std::random_device Random;
	std::mt19937 RandomGenerator(Random());
	std::uniform_real<float> RandomDistribute(min, max);
	return RandomDistribute(RandomGenerator);
}

void Aimbot::ClampAngle(D3DXVECTOR3 &ViewAngle)
{
	if (ViewAngle.x > 89.0f && ViewAngle.x <= 180.0f)
	{
		ViewAngle.x = 89.0f;
	}
	if (ViewAngle.x > 180.f)
	{
		ViewAngle.x -= 360.f;
	}
	if (ViewAngle.x < -89.0f)
	{
		ViewAngle.x = -89.0f;
	}
	if (ViewAngle.y > 180.f)
	{
		ViewAngle.y -= 360.f;
	}
	if (ViewAngle.y < -180.f)
	{
		ViewAngle.y += 360.f;
	}
	if (ViewAngle.z != 0.0f)
	{
		ViewAngle.z = 0.0f;
	}
}

bool Aimbot::WorldToScreen(D3DXVECTOR3 &vOrigin, D3DXVECTOR3 &vScreen, CGeneralEntity e)
{

	float w = e.WorldToScreenMatrix.flMatrix[3][0] * vOrigin[0] + e.WorldToScreenMatrix.flMatrix[3][1] * vOrigin[1] + e.WorldToScreenMatrix.flMatrix[3][2] * vOrigin[2] + e.WorldToScreenMatrix.flMatrix[3][3]; //Calc the angle in comparison to the player's cam
	vScreen.z = 0;
	if (w > 0.01)
	{
		float inversew = 1 / w;
		vScreen.x = (ScreenWidth / 2) + (0.5 * ((e.WorldToScreenMatrix.flMatrix[0][0] * vOrigin[0] + e.WorldToScreenMatrix.flMatrix[0][1] * vOrigin[1] + e.WorldToScreenMatrix.flMatrix[0][2] * vOrigin[2] + e.WorldToScreenMatrix.flMatrix[0][3]) * inversew) * ScreenWidth + 0.5);
		vScreen.y = (ScreenHeight / 2) - (0.5 * ((e.WorldToScreenMatrix.flMatrix[1][0] * vOrigin[0] + e.WorldToScreenMatrix.flMatrix[1][1] * vOrigin[1] + e.WorldToScreenMatrix.flMatrix[1][2] * vOrigin[2] + e.WorldToScreenMatrix.flMatrix[1][3]) * inversew) * ScreenHeight + 0.5);
		return true;
	}
	return false;
}