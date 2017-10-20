#include <emmintrin.h>
#include <xmmintrin.h>

inline float sqrt2(float sqr)
{
	float root = 0;

	__asm
	{
		sqrtss xmm0,sqr
		movss root,xmm0
	}

	return root;
}

inline void SinCos(const float rad, float &sin, float &cos)
{
	const __m128 _ps_fopi = _mm_set_ss(1.27323954473516f);

	const __m128 _ps_0p5 = _mm_set_ss(0.5f);
	const __m128 _ps_1   = _mm_set_ss(1.0f);

	const __m128 _ps_dp1 = _mm_set_ss(-0.7851562f);
	const __m128 _ps_dp2 = _mm_set_ss(-2.4187564849853515625e-4f);
	const __m128 _ps_dp3 = _mm_set_ss(-3.77489497744594108e-8f);

	const __m128 _ps_sincof_p0 = _mm_set_ss(2.443315711809948e-5f);
	const __m128 _ps_sincof_p1 = _mm_set_ss(8.3321608736e-3f);
	const __m128 _ps_sincof_p2 = _mm_set_ss(-1.6666654611e-1f);
	const __m128 _ps_coscof_p0 = _mm_set_ss(2.443315711809948e-5f);
	const __m128 _ps_coscof_p1 = _mm_set_ss(-1.388731625493765e-3f);
	const __m128 _ps_coscof_p2 = _mm_set_ss(4.166664568298827e-2f);

	const __m128i _pi32_1  = _mm_set1_epi32(1);
	const __m128i _pi32_i1 = _mm_set1_epi32(~1);
	const __m128i _pi32_2  = _mm_set1_epi32(2);
	const __m128i _pi32_4  = _mm_set1_epi32(4);

	const __m128 _mask_sign_raw = *(__m128*)&_mm_set1_epi32(0x80000000);
	const __m128 _mask_sign_inv = *(__m128*)&_mm_set1_epi32(~0x80000000);


	__m128  xmm3 = _mm_setzero_ps();
	__m128i emm0, emm2, emm4;

	__m128 sign_bit_cos, sign_bit_sin;

	__m128 x, y, z;
	__m128 y1,  y2;

	__m128 a = _mm_set_ss(rad);

	x = _mm_and_ps(a, _mask_sign_inv);
	y = _mm_mul_ps(x, _ps_fopi);

	emm2 = _mm_cvtps_epi32(y);
	emm2 = _mm_add_epi32(emm2, _pi32_1);
	emm2 = _mm_and_si128(emm2, _pi32_i1);
	y    = _mm_cvtepi32_ps(emm2);

	emm4 = emm2;

	emm0 = _mm_and_si128(emm2, _pi32_4);
	emm0 = _mm_slli_epi32(emm0, 29);
	__m128 swap_sign_bit_sin = _mm_castsi128_ps(emm0);

	emm2 = _mm_and_si128(emm2, _pi32_2);
	emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());
	__m128 poly_mask = _mm_castsi128_ps(emm2);

	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp1));
	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp2));
	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp3));

	emm4 = _mm_sub_epi32(emm4, _pi32_2);
	emm4 = _mm_andnot_si128(emm4, _pi32_4);
	emm4 = _mm_slli_epi32(emm4, 29);

	sign_bit_cos = _mm_castsi128_ps(emm4);
	sign_bit_sin = _mm_xor_ps(_mm_and_ps(a, _mask_sign_raw), swap_sign_bit_sin);

	z = _mm_mul_ps(x, x);

	y1 = _mm_mul_ps(_ps_coscof_p0, z);
	y1 = _mm_add_ps(y1, _ps_coscof_p1);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_add_ps(y1, _ps_coscof_p2);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_sub_ps(y1, _mm_mul_ps(z, _ps_0p5));
	y1 = _mm_add_ps(y1, _ps_1);

	y2 = _mm_mul_ps(_ps_sincof_p0, z);
	y2 = _mm_add_ps(y2, _ps_sincof_p1);
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_add_ps(y2, _ps_sincof_p2);
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_mul_ps(y2, x);
	y2 = _mm_add_ps(y2, x);

	xmm3 = poly_mask;

	__m128 ysin2 = _mm_and_ps(xmm3, y2);
	__m128 ysin1 = _mm_andnot_ps(xmm3, y1);

	sin = _mm_cvtss_f32(_mm_xor_ps(_mm_add_ps(ysin1, ysin2), sign_bit_sin));
	cos = _mm_cvtss_f32(_mm_xor_ps(_mm_add_ps(_mm_sub_ps(y1, ysin1), _mm_sub_ps(y2, ysin2)), sign_bit_cos));
}