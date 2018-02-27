// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLPLANE_H
#define	ZLPLANE_H

#include <zl-util/ZLVec3D.h>

//================================================================//
// ZLPlane2D
//================================================================//
class ZLPlane2D {
	public:

	ZLVec2D	mNorm;
	ZLReal	mDist;

	//----------------------------------------------------------------//
	void	Flip			();
	void	Init			( const ZLVec2D& p1, const ZLVec2D& p2 );	// From a line segment
	void	Init			( const ZLVec2D& n, ZLReal d );
	void	Init			( ZLReal xn, ZLReal yn, ZLReal d );
			ZLPlane2D		() {};
			~ZLPlane2D		() {};
};

//================================================================//
// ZLPlane3D
//================================================================//
class ZLPlane3D {
	public:

	ZLVec3D	mNorm;
	ZLReal	mDist;

	//----------------------------------------------------------------//
	void	Flip			();
	void	Init			( const ZLVec3D& p, const ZLVec3D& n );							// From a point and a normal
	void	Init			( const ZLVec3D& p1, const ZLVec3D& p2, const ZLVec3D& p3 );	// From a triangle; N = |( p2 - p1 ) x ( p3 - p1 )|
	void	Init			( ZLReal xn, ZLReal yn, ZLReal zn, ZLReal d );
			ZLPlane3D		() {};
			~ZLPlane3D		() {};
};

#endif
