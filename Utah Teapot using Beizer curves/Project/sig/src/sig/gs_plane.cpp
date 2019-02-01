/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/gs_plane.h>

//============================== GsPlane ======================================

// Important: Static initializations cannot use other statically-initialized
// variables ( as GsVec::i, ... ), because we cannot know the order that they
// will be initialized by the compiler.

const GsPlane GsPlane::XY ( GsVec::null, GsVec::i, GsVec::j );
const GsPlane GsPlane::XZ ( GsVec::null, GsVec::i, GsVec::k );
const GsPlane GsPlane::YZ ( GsVec::null, GsVec::j, GsVec::k );

GsPlane::GsPlane () : coords ( GsVec::k ), coordsw(0)
{
}

GsPlane::GsPlane ( const GsVec& center, const GsVec& normal )
{
	set ( center, normal );
}

GsPlane::GsPlane ( const GsVec& p1, const GsVec& p2, const GsVec& p3 )
{
	set ( p1, p2, p3 );
}

bool GsPlane::set ( const GsVec& center, const GsVec& normal )
{
	coords = normal;
	coordsw = -dot(normal,center);
	float n = normal.norm();
	if (n==0.0) return false;
	coords/=n; coordsw/=n;
	return true;
}

bool GsPlane::set ( const GsVec& p1, const GsVec& p2, const GsVec& p3 )
{
	GsVec normal = cross ( p2-p1, p3-p1 );
	return set ( p1, normal );
}

bool GsPlane::parallel ( const GsVec& p1, const GsVec& p2, float ds ) const
{
	float fact = dot ( coords, p1-p2 );
	return GS_NEXTZ(fact,ds);
}

/*! Returns p as the intersection between plane and infinite line {p1,p2};
	(0,0,0) is returned if they are parallel. */
GsVec GsPlane::intersect ( const GsVec& p1, const GsVec& p2, float *t ) const
{
	float fact = dot ( coords, p1-p2 );
	if ( fact==0.0 ) return GsVec::null;
	float k = (coordsw+dot(coords,p1)) / fact;
	if (t) *t=k;
	return mix ( p1, p2, k );
}
