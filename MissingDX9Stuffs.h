/**
 *  Added missing structures functions from DX9 SDK.
 *  I need to compile project without installing DX9 SDK.
 */

#include <d3d9types.h>

#define D3DXVECTOR3 D3DVECTOR
#define D3DXINLINE

typedef struct D3DXFLOAT16
{
#ifdef __cplusplus
public:
	D3DXFLOAT16() {};
	D3DXFLOAT16(FLOAT);
	D3DXFLOAT16(CONST D3DXFLOAT16&);

	// casting
	operator FLOAT ();

	// binary operators
	BOOL operator == (CONST D3DXFLOAT16&) const;
	BOOL operator != (CONST D3DXFLOAT16&) const;

protected:
#endif //__cplusplus
	WORD value;
} D3DXFLOAT16, *LPD3DXFLOAT16;

typedef struct D3DXVECTOR4
{
#ifdef __cplusplus
public:
	D3DXVECTOR4() {};
	D3DXVECTOR4(CONST FLOAT*);
	D3DXVECTOR4(CONST D3DXFLOAT16 *);
	D3DXVECTOR4(CONST D3DVECTOR& xyz, FLOAT w);
	D3DXVECTOR4(FLOAT x, FLOAT y, FLOAT z, FLOAT w);

	// casting
	operator FLOAT* ();
	operator CONST FLOAT* () const;

	// assignment operators
	D3DXVECTOR4& operator += (CONST D3DXVECTOR4&);
	D3DXVECTOR4& operator -= (CONST D3DXVECTOR4&);
	D3DXVECTOR4& operator *= (FLOAT);
	D3DXVECTOR4& operator /= (FLOAT);

	// unary operators
	D3DXVECTOR4 operator + () const;
	D3DXVECTOR4 operator - () const;

	// binary operators
	D3DXVECTOR4 operator + (CONST D3DXVECTOR4&) const;
	D3DXVECTOR4 operator - (CONST D3DXVECTOR4&) const;
	D3DXVECTOR4 operator * (FLOAT) const;
	D3DXVECTOR4 operator / (FLOAT) const;

	friend D3DXVECTOR4 operator * (FLOAT, CONST D3DXVECTOR4&);

	BOOL operator == (CONST D3DXVECTOR4&) const;
	BOOL operator != (CONST D3DXVECTOR4&) const;

public:
#endif //__cplusplus
	FLOAT x, y, z, w;
} D3DXVECTOR4, *LPD3DXVECTOR4;

typedef struct D3DXVECTOR2
{
#ifdef __cplusplus
public:
	D3DXVECTOR2() {};
	D3DXVECTOR2(CONST FLOAT *);
	D3DXVECTOR2(CONST D3DXFLOAT16 *);
	D3DXVECTOR2(FLOAT x, FLOAT y);

	// casting
	operator FLOAT* ();
	operator CONST FLOAT* () const;

	// assignment operators
	D3DXVECTOR2& operator += (CONST D3DXVECTOR2&);
	D3DXVECTOR2& operator -= (CONST D3DXVECTOR2&);
	D3DXVECTOR2& operator *= (FLOAT);
	D3DXVECTOR2& operator /= (FLOAT);

	// unary operators
	D3DXVECTOR2 operator + () const;
	D3DXVECTOR2 operator - () const;

	// binary operators
	D3DXVECTOR2 operator + (CONST D3DXVECTOR2&) const;
	D3DXVECTOR2 operator - (CONST D3DXVECTOR2&) const;
	D3DXVECTOR2 operator * (FLOAT) const;
	D3DXVECTOR2 operator / (FLOAT) const;

	friend D3DXVECTOR2 operator * (FLOAT, CONST D3DXVECTOR2&);

	BOOL operator == (CONST D3DXVECTOR2&) const;
	BOOL operator != (CONST D3DXVECTOR2&) const;


public:
#endif //__cplusplus
	FLOAT x, y;
} D3DXVECTOR2, *LPD3DXVECTOR2;

typedef struct D3DXVECTOR2_16F
{
#ifdef __cplusplus
public:
	D3DXVECTOR2_16F() {};
	D3DXVECTOR2_16F(CONST FLOAT *);
	D3DXVECTOR2_16F(CONST D3DXFLOAT16 *);
	D3DXVECTOR2_16F(CONST D3DXFLOAT16 &x, CONST D3DXFLOAT16 &y);

	// casting
	operator D3DXFLOAT16* ();
	operator CONST D3DXFLOAT16* () const;

	// binary operators
	BOOL operator == (CONST D3DXVECTOR2_16F&) const;
	BOOL operator != (CONST D3DXVECTOR2_16F&) const;

public:
#endif //__cplusplus
	D3DXFLOAT16 x, y;

} D3DXVECTOR2_16F, *LPD3DXVECTOR2_16F;

#ifdef __cplusplus
typedef struct D3DXMATRIX : public D3DMATRIX
{
public:
	D3DXMATRIX() {};
	D3DXMATRIX(CONST FLOAT *);
	D3DXMATRIX(CONST D3DMATRIX&);
	D3DXMATRIX(CONST D3DXFLOAT16 *);
	D3DXMATRIX(FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
		FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
		FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
		FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44);


	// access grants
	FLOAT& operator () (UINT Row, UINT Col);
	FLOAT  operator () (UINT Row, UINT Col) const;

	// casting operators
	operator FLOAT* ();
	operator CONST FLOAT* () const;

	// assignment operators
	D3DXMATRIX& operator *= (CONST D3DXMATRIX&);
	D3DXMATRIX& operator += (CONST D3DXMATRIX&);
	D3DXMATRIX& operator -= (CONST D3DXMATRIX&);
	D3DXMATRIX& operator *= (FLOAT);
	D3DXMATRIX& operator /= (FLOAT);

	// unary operators
	D3DXMATRIX operator + () const;
	D3DXMATRIX operator - () const;

	// binary operators
	D3DXMATRIX operator * (CONST D3DXMATRIX&) const;
	D3DXMATRIX operator + (CONST D3DXMATRIX&) const;
	D3DXMATRIX operator - (CONST D3DXMATRIX&) const;
	D3DXMATRIX operator * (FLOAT) const;
	D3DXMATRIX operator / (FLOAT) const;

	friend D3DXMATRIX operator * (FLOAT, CONST D3DXMATRIX&);

	BOOL operator == (CONST D3DXMATRIX&) const;
	BOOL operator != (CONST D3DXMATRIX&) const;

} D3DXMATRIX, *LPD3DXMATRIX;

#else //!__cplusplus
typedef struct _D3DMATRIX D3DXMATRIX, *LPD3DXMATRIX;
#endif //!__cplusplus
