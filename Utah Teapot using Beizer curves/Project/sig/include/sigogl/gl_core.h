/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef GL_CORE_H
# define GL_CORE_H

/** \file gl_core.h 
 * Main include file for OpenGL core functions version 4
 * For detailed OpenGL specification per version see http://www.opengl.org/registry/
 */

# include <sig/gs.h>
# include <sig/gs_color.h>

# ifdef GS_WINDOWS
	# include <sigogl/glcorearb.h> // this header file comes from the link above
	# include <sigogl/glcorearb_functions.h>
# else
	# include <sigogl/glcorearb.h> // this header file comes from the link above
	# include <sigogl/glcorearb_functions.h>
//	#include <GL/gl.h> // include headers or load sig OpenGL headers as in windows
# endif

# define GLTRANSPMAT GL_TRUE

//=============================== End of File ===============================

# endif // GL_CORE_H
