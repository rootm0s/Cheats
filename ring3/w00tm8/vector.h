#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)
extern float(*pfSqrt)(float x);
#define FastSqrt(x)			(*pfSqrt)(x)
#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define square( x ) ( x * x )
#define M_PI			3.14159265358979323846
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define FLOAT32_NAN_BITS     (unsigned long)0x7FC00000
#define FLOAT32_NAN          BitsToFloat( FLOAT32_NAN_BITS )
#define VEC_T_NAN FLOAT32_NAN
typedef float vec_t;
class Vector
{
public:

	vec_t x, y, z;

	Vector(void);
	Vector(vec_t X, vec_t Y, vec_t Z);
	explicit Vector(vec_t XYZ);

	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);

	bool IsValid() const;
	void Invalidate();

	vec_t   operator[](int i) const;
	vec_t&  operator[](int i);

	vec_t* Base();
	vec_t const* Base() const;

	inline void Zero();

	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;

	FORCEINLINE Vector& operator+=(const Vector& v);
	FORCEINLINE Vector& operator-=(const Vector& v);
	FORCEINLINE Vector& operator*=(const Vector& v);
	FORCEINLINE Vector& operator*=(float s);
	FORCEINLINE Vector& operator/=(const Vector& v);
	FORCEINLINE Vector& operator/=(float s);
	FORCEINLINE Vector& operator+=(float fl);
	FORCEINLINE Vector& operator-=(float fl);

	void Negate();

	inline vec_t Length() const;

	FORCEINLINE vec_t LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}

	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}

	vec_t NormalizeInPlace();
	Vector Normalized() const;

	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	FORCEINLINE bool WithinAABox(Vector const &boxmin, Vector const &boxmax);

	vec_t DistTo(const Vector& vOther) const;

	FORCEINLINE vec_t DistToSqr(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	void CopyToArray(float* rgfl) const;
	void MulAdd(const Vector& a, const Vector& b, float scalar);

	vec_t Dot(const Vector& vOther) const;

	Vector& operator=(const Vector& vOther);

	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;

	Vector operator-(void) const;
	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;
	Vector operator*(const Vector& v) const;
	Vector operator/(const Vector& v) const;
	Vector operator*(float fl) const;
	Vector operator/(float fl) const;

	Vector Cross(const Vector& vOther) const;

	Vector Min(const Vector& vOther) const;
	Vector Max(const Vector& vOther) const;
};


FORCEINLINE Vector ReplicateToVector(float x)
{
	return Vector(x, x, x);
}

inline Vector::Vector(void)
{

}



inline Vector::Vector(vec_t X, vec_t Y, vec_t Z)
{
	x = X; y = Y; z = Z;
}

inline Vector::Vector(vec_t XYZ)
{
	x = y = z = XYZ;
}

inline void Vector::Init(vec_t ix, vec_t iy, vec_t iz)
{
	x = ix; y = iy; z = iz;
}

inline void Vector::Zero()
{
	x = y = z = 0.0f;
}

inline void VectorClear(Vector& a)
{
	a.x = a.y = a.z = 0.0f;
}

inline Vector& Vector::operator=(const Vector& vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

inline vec_t& Vector::operator[](int i)
{
	return ((vec_t*)this)[i];
}

inline vec_t Vector::operator[](int i) const
{
	return ((vec_t*)this)[i];
}



inline vec_t* Vector::Base()
{
	return (vec_t*)this;
}

inline vec_t const* Vector::Base() const
{
	return (vec_t const*)this;
}

inline bool Vector::IsValid() const
{
	return (x == x && y == y && z == z);
}

inline void Vector::Invalidate()
{
	x = y = z;
}

inline bool Vector::operator==(const Vector& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Vector::operator!=(const Vector& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z);
}

FORCEINLINE void VectorCopy(const Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

inline void Vector::CopyToArray(float* rgfl) const
{
	rgfl[0] = x; rgfl[1] = y; rgfl[2] = z;
}

inline void Vector::Negate()
{
	x = -x; y = -y; z = -z;
}

FORCEINLINE Vector& Vector::operator+=(const Vector& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator-=(const Vector& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator*=(const Vector& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

FORCEINLINE Vector& Vector::operator/=(const Vector& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

FORCEINLINE void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

FORCEINLINE void VectorMultiply(const Vector& a, vec_t b, Vector& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

FORCEINLINE void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

inline void VectorScale(const Vector& in, vec_t scale, Vector& result)
{
	VectorMultiply(in, scale, result);
}

FORCEINLINE void VectorDivide(const Vector& a, vec_t b, Vector& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

FORCEINLINE void VectorDivide(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

inline void Vector::MulAdd(const Vector& a, const Vector& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
}

inline void VectorLerp(const Vector& src1, const Vector& src2, vec_t t, Vector& dest)
{
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
}

FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline vec_t Vector::Dot(const Vector& vOther) const
{
	return DotProduct(*this, vOther);
}

inline void CrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

//inline vec_t DotProductAbs(const Vector& v0, const Vector& v1)
//{
//	return abs(v0.x * v1.x) + abs(v0.y * v1.y) + abs(v0.z * v1.z);
//}

inline vec_t VectorLength(const Vector& v)
{
	return (vec_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline vec_t Vector::Length(void) const
{
	return VectorLength(*this);
}

inline vec_t VectorNormalize(Vector& v)
{
	vec_t l = v.Length();

	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f; v.z = 1.0f;
	}

	return l;
}

FORCEINLINE float VectorNormalizer(float * v)
{
	return VectorNormalize(*(reinterpret_cast<Vector *>(v)));
}
inline vec_t Vector::NormalizeInPlace()
{
	return VectorNormalize(*this);
}

bool Vector::WithinAABox(Vector const &boxmin, Vector const &boxmax)
{
	return (
		(x >= boxmin.x) && (x <= boxmax.x) &&
		(y >= boxmin.y) && (y <= boxmax.y) &&
		(z >= boxmin.z) && (z <= boxmax.z)
		);
}

inline vec_t Vector::DistTo(const Vector& vOther) const
{
	Vector delta;
	VectorSubtract(*this, vOther, delta);
	return delta.Length();
}

inline Vector Vector::Min(const Vector& vOther) const
{
	return Vector(x < vOther.x ? x : vOther.x,
		y < vOther.y ? y : vOther.y,
		z < vOther.z ? z : vOther.z);
}

inline Vector Vector::Max(const Vector& vOther) const
{
	return Vector(x > vOther.x ? x : vOther.x,
		y > vOther.y ? y : vOther.y,
		z > vOther.z ? z : vOther.z);
}

inline Vector Vector::operator-(void) const
{
	return Vector(-x, -y, -z);
}

inline Vector Vector::operator+(const Vector& v) const
{
	Vector res;
	VectorAdd(*this, v, res);
	return res;
}

inline Vector Vector::operator-(const Vector& v) const
{
	Vector res;
	VectorSubtract(*this, v, res);
	return res;
}

inline Vector Vector::operator*(float fl) const
{
	Vector res;
	VectorMultiply(*this, fl, res);
	return res;
}

inline Vector Vector::operator*(const Vector& v) const
{
	Vector res;
	VectorMultiply(*this, v, res);
	return res;
}

inline Vector Vector::operator/(float fl) const
{
	Vector res;
	VectorDivide(*this, fl, res);
	return res;
}

inline Vector Vector::operator/(const Vector& v) const
{
	Vector res;
	VectorDivide(*this, v, res);
	return res;
}

inline Vector operator*(float fl, const Vector& v)
{
	return v * fl;
}

inline Vector Vector::Cross(const Vector& vOther) const
{
	Vector res;
	CrossProduct(*this, vOther, res);
	return res;
}

inline vec_t Vector::Length2D(void) const
{
	return (vec_t)::sqrtf(x * x + y * y);
}

inline vec_t Vector::Length2DSqr(void) const
{
	return (x * x + y * y);
}

inline Vector CrossProduct(const Vector& a, const Vector& b)
{
	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline void VectorMin(const Vector& a, const Vector& b, Vector& result)
{
	result.x = min(a.x, b.x);
	result.y = min(a.y, b.y);
	result.z = min(a.z, b.z);
}

inline void VectorMax(const Vector& a, const Vector& b, Vector& result)
{
	result.x = max(a.x, b.x);
	result.y = max(a.y, b.y);
	result.z = max(a.z, b.z);
}


class QAngleByValue;
class QAngle
{
public:
	// Members
	float x, y, z;

	// Construction/destruction
	QAngle(void);
	QAngle(float X, float Y, float Z);
	//      QAngle(RadianEuler const &angles);      // evil auto type promotion!!!

	// Allow pass-by-value
	operator QAngleByValue &()              { return *((QAngleByValue *)(this)); }
	operator const QAngleByValue &() const  { return *((const QAngleByValue *)(this)); }

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	void Random(float minVal, float maxVal);

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// equality
	bool operator==(const QAngle& v) const;
	bool operator!=(const QAngle& v) const;

	// arithmetic operations
	QAngle& operator+=(const QAngle &v);
	QAngle& operator-=(const QAngle &v);
	QAngle& operator*=(float s);
	QAngle& operator/=(float s);

	// Get the vector's magnitude.
	float   Length() const;
	float   LengthSqr() const;

	// negate the QAngle components
	//void  Negate();

	// No assignment operators either...
	QAngle& operator=(const QAngle& src);

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// copy constructors

	// arithmetic operations
	QAngle  operator-(void) const;

	QAngle  operator+(const QAngle& v) const;
	QAngle  operator-(const QAngle& v) const;
	QAngle  operator*(float fl) const;
	QAngle  operator/(float fl) const;
#else

private:
	// No copy constructors allowed if we're in optimal mode
	QAngle(const QAngle& vOther);

#endif
};

//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------
inline QAngle::QAngle(void)
{
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
	// Initialize to NAN to catch errors
	x = y = z = VEC_T_NAN;
#endif
#endif
}

inline QAngle::QAngle(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
	CHECK_VALID(*this);
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------
inline void QAngle::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
	CHECK_VALID(*this);
}

inline void QAngle::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	CHECK_VALID(*this);
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator=(const QAngle &vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------
inline bool QAngle::operator==(const QAngle& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool QAngle::operator!=(const QAngle& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator+=(const QAngle& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}

inline QAngle& QAngle::operator-=(const QAngle& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

inline QAngle& QAngle::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}

inline QAngle& QAngle::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* QAngle::Base()
{
	return (float*)this;
}

inline float const* QAngle::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline float& QAngle::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

inline float QAngle::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float QAngle::Length() const
{
	CHECK_VALID(*this);
	return (float)FastSqrt(LengthSqr());
}


inline float QAngle::LengthSqr() const
{
	CHECK_VALID(*this);
	return x * x + y * y + z * z;
}


//-----------------------------------------------------------------------------
// arithmetic operations (SLOW!!)
//-----------------------------------------------------------------------------
#ifndef VECTOR_NO_SLOW_OPERATIONS

inline QAngle QAngle::operator-(void) const
{
	return QAngle(-x, -y, -z);
}

inline QAngle QAngle::operator+(const QAngle& v) const
{
	QAngle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline QAngle QAngle::operator-(const QAngle& v) const
{
	QAngle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline QAngle QAngle::operator*(float fl) const
{
	QAngle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline QAngle QAngle::operator/(float fl) const
{
	QAngle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline QAngle operator*(float fl, const QAngle& v)
{
	return v * fl;
}

#endif // VECTOR_NO_SLOW_OPERATIONS


//QANGLE SUBTRAC
inline void QAngleSubtract(const QAngle& a, const QAngle& b, QAngle& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

//QANGLEADD
inline void QAngleAdd(const QAngle& a, const QAngle& b, QAngle& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

