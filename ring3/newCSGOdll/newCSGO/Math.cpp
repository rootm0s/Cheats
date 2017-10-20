#include	"stdafx.h"

CMath cMath;

Vector::Vector ()
{
	this->X = 0.0f;
	this->Y = 0.0f;
	this->Z = 0.0f;
}

Vector::Vector ( float _X, float _Y, float _Z )
{
	this->X = _X;
	this->Y = _Y;
	this->Z = _Z;
}

Vector Vector::operator+ ( const Vector &A )
{
	return Vector( this->X + A.X, this->Y + A.Y, this->Z + A.Z );
}

Vector Vector::operator+ ( const float A )
{
	return Vector( this->X + A, this->Y + A, this->Z + A );
}

Vector Vector::operator+= ( const Vector &A )
{
	this->X += A.X;
	this->Y += A.Y;
	this->Z += A.Z;
	return *this;
}

Vector Vector::operator+= ( const float A )
{
	this->X += A;
	this->Y += A;
	this->Z += A;
	return *this;
}

Vector Vector::operator- ( const Vector &A )
{
	return Vector( this->X - A.X, this->Y - A.Y, this->Z - A.Z );
}

Vector Vector::operator- ( const float A )
{
	return Vector( this->X - A, this->Y - A, this->Z - A );
}

Vector Vector::operator-= ( const Vector &A )
{
	this->X -= A.X;
	this->Y -= A.Y;
	this->Z -= A.Z;
	return *this;
}

Vector Vector::operator-= ( const float A )
{
	this->X -= A;
	this->Y -= A;
	this->Z -= A;
	return *this;
}

Vector Vector::operator* ( const Vector &A )
{
	return Vector( this->X * A.X, this->Y * A.Y, this->Z * A.Z );
}

Vector Vector::operator* ( const float A )
{
	return Vector( this->X * A, this->Y * A, this->Z * A );
}

Vector Vector::operator*= ( const Vector &A )
{
	this->X *= A.X;
	this->Y *= A.Y;
	this->Z *= A.Z;
	return *this;
}

Vector Vector::operator*= ( const float A )
{
	this->X *= A;
	this->Y *= A;
	this->Z *= A;
	return *this;
}

Vector Vector::operator/ ( const Vector &A )
{
	return Vector( this->X / A.X, this->Y / A.Y, this->Z / A.Z );
}

Vector Vector::operator/ ( const float A )
{
	return Vector( this->X / A, this->Y / A, this->Z / A );
}

Vector Vector::operator/= ( const Vector &A )
{
	this->X /= A.X;
	this->Y /= A.Y;
	this->Z /= A.Z;
	return *this;
}

Vector Vector::operator/= ( const float A )
{
	this->X /= A;
	this->Y /= A;
	this->Z /= A;
	return *this;
}

bool Vector::operator== ( const Vector &A )
{
	if ( this->X == A.X 
		&& this->Y == A.Y
		&& this->Z == A.Z )
		return true;

	return false;
}

bool Vector::operator!= ( const Vector &A )
{
	if ( this->X != A.X 
		|| this->Y != A.Y
		|| this->Z != A.Z )
		return true;

	return false;
}

bool Vector::IsValid ()
{
	return *this != Vector( 0, 0, 0 );
}

FLOAT fScreenX, fScreenY;

BOOL CMath::ScreenTransform(CONST Vector& point, OUT Vector& screen)
{
	const D3DXMATRIX& worldToScreen = pEngine->WorldToScreenMatrix();
//	const D3DXMATRIX& worldToScreen = cfn.WorldToScreenMatrix();

	screen.X = worldToScreen.m[0][0] * point.X + worldToScreen.m[0][1] * point.Y + worldToScreen.m[0][2] * point.Z + worldToScreen.m[0][3];
	screen.Y = worldToScreen.m[1][0] * point.X + worldToScreen.m[1][1] * point.Y + worldToScreen.m[1][2] * point.Z + worldToScreen.m[1][3];

	FLOAT w;
	w = worldToScreen.m[3][0] * point.X + worldToScreen.m[3][1] * point.Y + worldToScreen.m[3][2] * point.Z + worldToScreen.m[3][3];
	screen.Z = 0.0f;

	BOOL behind = FALSE;
	if(w < 0.001f)
	{
		behind = TRUE;
		screen.X *= 100000;
		screen.Y *= 100000;
	}
	else
	{
		FLOAT invw = 1.0f / w;
		screen.X *= invw;
		screen.Y *= invw;
	}

	return behind;
}

BOOL CMath::WorldToScreen(CONST Vector &origin, OUT Vector &screen)
{
	BOOL bRet = FALSE;
	
	if(!ScreenTransform(origin, screen))
	{
		INT width, height;
		pEngine->GetScreenSize(width, height);

		fScreenX = width / 2;
		fScreenY = height / 2;

		fScreenX += 0.5f * screen.X * width + 0.5f;
		fScreenY -= 0.5f * screen.Y * height + 0.5f;

		screen.X = fScreenX;
		screen.Y = fScreenY;

		bRet = TRUE;
	}

	return bRet;
}
