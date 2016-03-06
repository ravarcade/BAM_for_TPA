/**
 *  Added missing structures functions from DX9 SDK.
 *  I need to compile project without installing DX9 SDK.
 */


#include <d3d9.h>
#include "MissingDX9Stuffs.h"

D3DXMATRIX* D3DXMatrixMultiply(
	_Inout_       D3DXMATRIX *pOut,
	_In_    const D3DXMATRIX *pM1,
	_In_    const D3DXMATRIX *pM2
	)
{
	const D3DXMATRIX &M1 = *pM1;
	const D3DXMATRIX &M2 = *pM2;
	D3DXMATRIX &mResult = *pOut;

	// Cache the invariants in registers
	float x = M1.m[0][0];
	float y = M1.m[0][1];
	float z = M1.m[0][2];
	float w = M1.m[0][3];
	// Perform the operation on the first row
	mResult.m[0][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
	mResult.m[0][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
	mResult.m[0][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
	mResult.m[0][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
	// Repeat for all the other rows
	x = M1.m[1][0];
	y = M1.m[1][1];
	z = M1.m[1][2];
	w = M1.m[1][3];
	mResult.m[1][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
	mResult.m[1][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
	mResult.m[1][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
	mResult.m[1][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
	x = M1.m[2][0];
	y = M1.m[2][1];
	z = M1.m[2][2];
	w = M1.m[2][3];
	mResult.m[2][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
	mResult.m[2][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
	mResult.m[2][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
	mResult.m[2][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
	x = M1.m[3][0];
	y = M1.m[3][1];
	z = M1.m[3][2];
	w = M1.m[3][3];
	mResult.m[3][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
	mResult.m[3][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
	mResult.m[3][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
	mResult.m[3][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
	return pOut;
}

D3DXINLINE
D3DXMATRIX::D3DXMATRIX(CONST FLOAT* pf)
{
#ifdef D3DX_DEBUG
	if (!pf)
		return;
#endif

	memcpy(&_11, pf, sizeof(D3DXMATRIX));
}

D3DXINLINE
D3DXMATRIX::D3DXMATRIX(CONST D3DMATRIX& mat)
{
	memcpy(&_11, &mat, sizeof(D3DXMATRIX));
}

/*
D3DXINLINE
D3DXMATRIX::D3DXMATRIX(CONST D3DXFLOAT16* pf)
{
#ifdef D3DX_DEBUG
	if (!pf)
		return;
#endif
	D3DXFloat16To32Array(&_11, pf, 16);
}
*/

D3DXINLINE
D3DXMATRIX::D3DXMATRIX(FLOAT f11, FLOAT f12, FLOAT f13, FLOAT f14,
FLOAT f21, FLOAT f22, FLOAT f23, FLOAT f24,
FLOAT f31, FLOAT f32, FLOAT f33, FLOAT f34,
FLOAT f41, FLOAT f42, FLOAT f43, FLOAT f44)
{
	_11 = f11; _12 = f12; _13 = f13; _14 = f14;
	_21 = f21; _22 = f22; _23 = f23; _24 = f24;
	_31 = f31; _32 = f32; _33 = f33; _34 = f34;
	_41 = f41; _42 = f42; _43 = f43; _44 = f44;
}



// access grants
D3DXINLINE FLOAT&
D3DXMATRIX::operator () (UINT iRow, UINT iCol)
{
	return m[iRow][iCol];
}

D3DXINLINE FLOAT
D3DXMATRIX::operator () (UINT iRow, UINT iCol) const
{
	return m[iRow][iCol];
}


// casting operators
D3DXINLINE
D3DXMATRIX::operator FLOAT* ()
{
	return (FLOAT *)&_11;
}

D3DXINLINE
D3DXMATRIX::operator CONST FLOAT* () const
{
	return (CONST FLOAT *) &_11;
}


// assignment operators
D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator *= (CONST D3DXMATRIX& mat)
{
	D3DXMatrixMultiply(this, this, &mat);
	return *this;
}

D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator += (CONST D3DXMATRIX& mat)
{
	_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
	_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
	_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
	_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
	return *this;
}

D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator -= (CONST D3DXMATRIX& mat)
{
	_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
	_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
	_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
	_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
	return *this;
}

D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator *= (FLOAT f)
{
	_11 *= f; _12 *= f; _13 *= f; _14 *= f;
	_21 *= f; _22 *= f; _23 *= f; _24 *= f;
	_31 *= f; _32 *= f; _33 *= f; _34 *= f;
	_41 *= f; _42 *= f; _43 *= f; _44 *= f;
	return *this;
}

D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator /= (FLOAT f)
{
	FLOAT fInv = 1.0f / f;
	_11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
	_21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
	_31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
	_41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
	return *this;
}


// unary operators
D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator + () const
{
	return *this;
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator - () const
{
	return D3DXMATRIX(-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44);
}


// binary operators
D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator * (CONST D3DXMATRIX& mat) const
{
	D3DXMATRIX matT;
	D3DXMatrixMultiply(&matT, this, &mat);
	return matT;
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator + (CONST D3DXMATRIX& mat) const
{
	return D3DXMATRIX(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
		_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
		_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
		_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator - (CONST D3DXMATRIX& mat) const
{
	return D3DXMATRIX(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
		_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
		_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
		_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator * (FLOAT f) const
{
	return D3DXMATRIX(_11 * f, _12 * f, _13 * f, _14 * f,
		_21 * f, _22 * f, _23 * f, _24 * f,
		_31 * f, _32 * f, _33 * f, _34 * f,
		_41 * f, _42 * f, _43 * f, _44 * f);
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator / (FLOAT f) const
{
	FLOAT fInv = 1.0f / f;
	return D3DXMATRIX(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
		_21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
		_31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
		_41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
}


D3DXINLINE D3DXMATRIX
operator * (FLOAT f, CONST D3DXMATRIX& mat)
{
	return D3DXMATRIX(f * mat._11, f * mat._12, f * mat._13, f * mat._14,
		f * mat._21, f * mat._22, f * mat._23, f * mat._24,
		f * mat._31, f * mat._32, f * mat._33, f * mat._34,
		f * mat._41, f * mat._42, f * mat._43, f * mat._44);
}


D3DXINLINE BOOL
D3DXMATRIX::operator == (CONST D3DXMATRIX& mat) const
{
	return 0 == memcmp(this, &mat, sizeof(D3DXMATRIX));
}

D3DXINLINE BOOL
D3DXMATRIX::operator != (CONST D3DXMATRIX& mat) const
{
	return 0 != memcmp(this, &mat, sizeof(D3DXMATRIX));
}