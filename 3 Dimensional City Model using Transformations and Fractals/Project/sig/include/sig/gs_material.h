/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

/** \file gs_material.h
 * specifies a material
 */

# ifndef GS_MATERIAL_H
# define GS_MATERIAL_H

# include <sig/gs_input.h> 
# include <sig/gs_output.h> 

# include <sig/gs_color.h>

/*! \class GsMaterial gs_material.h
	\brief specifies a material as defined by OpenGL

	Defines a material. */
class GsMaterial
{  public :
	GsColor ambient;   //!< default is 51,51,51,255 (equivalent to .2,.2,.2,1)
	GsColor diffuse;   //!< default is 204,204,204,255 (equivalent to: .8,.8,.8,1)
	GsColor specular;  //!< default is 127,127,127,255 (equivalent to: .5,.5,.5,1)
	GsColor emission;  //!< default is 0,0,0,255 (equivalent to: 0,0,0,1)
	float   shininess; //!< default: 10, can be in: [0,128]

   public :
	
	/*! Initializes with the default values. */
	GsMaterial ();

	/*! Set again the default values. Note that .2 is mapped
		to 51, and .8 to 204 in the GsColor format. */
	void init ();

	/*! Encodes the 4 colors of the material as 4 rgb vectors with 
		components in [0,1]. A pointer to vec is then returned. */
	float* encode_colors ( float vec[12] ) const;

	/*! Encodes in vec the shininess and the transparency 
		(diffuse.a in range [0,1]). A pointer to vec is then returned. */
	float* encode_params ( float vec[2] ) const;

	/*! Exact comparison operator == */
	friend bool operator == ( const GsMaterial& m1, const GsMaterial& m2 );

	/*! Exact comparison operator != */
	friend bool operator != ( const GsMaterial& m1, const GsMaterial& m2 );

	/*! Outputs in format: "amb ar ag ab aa dif dr dg db da spe gs gs sb sa emi er eg eb ea shi s tid i" */
	friend GsOutput& operator<< ( GsOutput& o, const GsMaterial& m );

	/*! Inputs from the output format (the keywords (all) can be omitted) */
	friend GsInput& operator>> ( GsInput& in, GsMaterial& m );
};

//============================== end of file ===================================

# endif // GS_MATERIAL_H
