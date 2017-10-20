#ifndef	Q_MATH_H
#define	Q_MATH_H

//Globals
#define M_PI 3.14159265358979323846f
#define ANG_CLIP( ang )		if( ang > 180.0f ) { ang -= 360.0f; } else if( ang <- 180.0f ) { ang += 360.0f; }
#define	GENTITYNUM_BITS		10
#define	MAX_GENTITIES		( 1 << GENTITYNUM_BITS )

struct Color;
struct Vector;
struct matrix3x4_t;
class CMath;

class CMath
{
public:
	BOOL WorldToScreen(CONST Vector& point, OUT Vector& screen);
	BOOL ScreenTransform(CONST Vector& point, OUT Vector& screen);
};
extern CMath cMath;

struct Vector
{
	float X;
	float Y;
	float Z;

	Vector ();
	Vector ( float _X, float _Y, float _Z );

	Vector operator+ ( const Vector &A );
	Vector operator+ ( const float A );
	Vector operator+= ( const Vector &A );
	Vector operator+= ( const float A );
	Vector operator- ( const Vector &A );
	Vector operator- ( const float A );
	Vector operator-= ( const Vector &A );
	Vector operator-= ( const float A );
	Vector operator* ( const Vector &A );
	Vector operator* ( const float A );
	Vector operator*= ( const Vector &A );
	Vector operator*= ( const float A );
	Vector operator/ ( const Vector &A );
	Vector operator/ ( const float A );
	Vector operator/= ( const Vector &A );
	Vector operator/= ( const float A );

	bool operator== ( const Vector &A );
	bool operator!= ( const Vector &A );

	float Size ();
	float Dot (const Vector V1,const Vector V2);

	bool IsValid ();
	//------- 2014.9.12
//	void Init(float ix=0.0f, float iy=0.0f, float iz=0.0f);
//	Vector	operator-(const Vector& v) const; //ÏòÁ¿¼õ·¨
	float LengthSqr(void) const
	{ 
		//CHECK_VALID(*this);
		return (X*X + Y*Y + Z*Z);		
	}

};

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t( 
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23 )
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}
	void Init( const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin )
	{
		m_flMatVal[0][0] = xAxis.X; m_flMatVal[0][1] = yAxis.X; m_flMatVal[0][2] = zAxis.X; m_flMatVal[0][3] = vecOrigin.X;
		m_flMatVal[1][0] = xAxis.Y; m_flMatVal[1][1] = yAxis.Y; m_flMatVal[1][2] = zAxis.Y; m_flMatVal[1][3] = vecOrigin.Y;
		m_flMatVal[2][0] = xAxis.Z; m_flMatVal[2][1] = yAxis.Z; m_flMatVal[2][2] = zAxis.Z; m_flMatVal[2][3] = vecOrigin.Z;
	}

	matrix3x4_t( const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin )
	{
		Init( xAxis, yAxis, zAxis, vecOrigin );
	}
	float *operator[]( int i )				
	{ 
		return m_flMatVal[i]; 
	}
	const float *operator[]( int i ) const	
	{ 
		return m_flMatVal[i]; 
	}
	float *Base()							
	{ 
		return &m_flMatVal[0][0]; 
	}
	const float *Base() const				
	{ 
		return &m_flMatVal[0][0]; 
	}

	float m_flMatVal[3][4];
};
#endif