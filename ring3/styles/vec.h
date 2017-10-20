#pragma once

typedef float matrix3x4[3][4];
#include "sse69.h"

#define pi 3.14159265358979323846
#define rad(a) a*(pi/180)
#define deg(a) a*(180/pi)

class Vector
{
public:
	Vector(float a = 0,float b = 0,float c = 0) { x = a; y = b; z = c; }

	inline void operator=(Vector a) { x = a.x; y = a.y; z = a.z; }
	inline bool operator!() { return !x && !y && !z; }
	inline bool operator==(Vector a) { return x == a.x && y == a.y && z == a.z; }
	inline bool operator!=(Vector a) { return x != a.x || y != a.y || z != a.z; }
	inline float& operator[](int i) { return ((float*)this)[i]; }
	inline Vector operator+(Vector a) { return Vector(x+a.x,y+a.y,z+a.z); }
	inline Vector operator-(Vector a) { return Vector(x-a.x,y-a.y,z-a.z); }
	inline Vector operator*(Vector a) { return Vector(x*a.x,y*a.y,z*a.z); }
	inline Vector operator/(Vector a) { return Vector(x/a.x,y/a.y,z/a.z); }
	inline void operator+=(Vector a) { *this = Vector(x+a.x,y+a.y,z+a.z); }
	inline void operator-=(Vector a) { *this = Vector(x-a.x,y-a.y,z-a.z); }
	inline void operator*=(Vector a) { *this = Vector(x*a.x,y*a.y,z*a.z); }
	inline void operator/=(Vector a) { *this = Vector(x/a.x,y/a.y,z/a.z); }
	inline Vector operator*(float a) { return Vector(x*a,y*a,z*a); }
	inline Vector operator/(float a) { return Vector(x/a,y/a,z/a); }
	inline void operator*=(float a) { *this = Vector(x*a,y*a,z*a); }
	inline void operator/=(float a) { *this = Vector(x/a,y/a,z/a); }
	inline float Length() { return sqrt2(x*x+y*y+z*z); }
	inline float Dot(Vector a) { return x*a.x+y*a.y+z*a.z; }
	inline Vector Cross(Vector a) { return Vector(y*a.z - z*a.y,z*a.x - x*a.z,x*a.y - y*a.x); }

	inline Vector Angle(Vector* up = 0)
	{
		if (!x && !y)
			return Vector(0,0,0);

		float roll = 0;

		if (up)
		{
			Vector left = (*up).Cross(*this);

			roll = deg(atan2f(left.z,(left.y * x) - (left.x * y)));
		}

		return Vector(deg(atan2f(-z,sqrt2(x*x+y*y))),deg(atan2f(y,x)),roll);
	}
	inline Vector Forward()
	{
		float sp,sy,cp,cy;

		SinCos(rad(x),sp,cp);
		SinCos(rad(y),sy,cy);

		return Vector(cp*cy,cp*sy,-sp);
	}
	inline Vector Right()
	{
		float sp,sy,sr,cp,cy,cr;

		SinCos(rad(x),sp,cp);
		SinCos(rad(y),sy,cy);
		SinCos(rad(z),sr,cr);

		return Vector(1*sr*sp*cy+-1*cr*-sy,-1*sr*sp*sy+-1*cr*cy,-1*sr*cr);
	}
	inline Vector Up()
	{
		float sp,sy,sr,cp,cy,cr;

		SinCos(rad(x),sp,cp);
		SinCos(rad(y),sy,cy);
		SinCos(rad(z),sr,cr);

		return Vector(cr*sp*cy+-sr*-sy,cr*sp*sy+-sr*cy,cr*cp);
	}

	float x,y,z;
};