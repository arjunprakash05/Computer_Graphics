/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/gs_material.h>
# include <sig/gs_string.h>

//# define GS_USE_TRACE1 
# include <sig/gs_trace.h>

//================================ GsMaterial =================================================

GsMaterial::GsMaterial () :
			ambient  (  51,  51,  51, 255 ),
			diffuse  ( 204, 204, 204, 255 ),
			specular ( 127, 127, 127, 255 ),
			emission (   0,   0,   0, 255 )
{
	shininess = 10;
}

void GsMaterial::init () 
{ 
	ambient.set  (  51,  51,  51, 255 );
	diffuse.set  ( 204, 204, 204, 255 );
	specular.set ( 127, 127, 127, 255 );
	emission.set (   0,   0,   0, 255 );
	shininess = 10;
}

float* GsMaterial::encode_colors ( float vec[12] ) const
{ 
	ambient.get3 (vec);
	diffuse.get3 (vec+3);
	specular.get3 (vec+6);
	emission.get3 (vec+9);
	return vec;
}

float* GsMaterial::encode_params ( float vec[2] ) const
{
	vec[0] = shininess; 
	vec[1] = float(diffuse.a)/255.0f;
	return vec;
}

bool operator == ( const GsMaterial& m1, const GsMaterial& m2 )
{
	return ( m1.ambient==m2.ambient &&
			 m1.diffuse==m2.diffuse &&
			 m1.specular==m2.specular &&
			 m1.emission==m2.emission &&
			 m1.shininess==m2.shininess );
}

bool operator != ( const GsMaterial& m1, const GsMaterial& m2 )
{
	return m1==m2? false:true;
}

GsOutput& operator<< ( GsOutput& o, const GsMaterial& m )
{
	o << "amb " << m.ambient  << 
		 " dif " << m.diffuse  << 
		 " spe " << m.specular << 
		 " emi " << m.emission << 
		 " shi " << m.shininess;
	return o;
}

GsInput& operator>> ( GsInput& in, GsMaterial& m )
{
	in.get();

	if ( in.ltype()!=GsInput::String ) // compact format, no texid
	{	in.unget();
		in >> m.ambient >> m.diffuse  >>
			  m.specular >> m.emission >>  m.shininess;
	}
	else
	{	in >> m.ambient;
		in.get(); in >> m.diffuse;
		in.get(); in >> m.specular;
		in.get(); in >> m.emission;
		in.get(); in >> m.shininess;
	}

	return in;
}

//============================== end of file ===============================
