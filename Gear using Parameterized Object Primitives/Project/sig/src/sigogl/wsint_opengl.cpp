/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include "wsint.h"
# include <sig/gs_var.h>
# include <sig/gs_output.h>
# include <sigogl/gl_core.h>
# include <sigogl/gl_resources.h>

//# define GS_USE_TRACE_COUNTER
//# define GS_USE_TRACE1 // OGL version
# include <sig/gs_trace.h>

//=========================== OpenGL Function Loading ==========================================

# ifdef GS_WINDOWS
	# define GetProcPt(s) wglGetProcAddress(s)
# else
	#include <GL/glx.h> // include default one or load everyfunction as in windows
# endif

static unsigned WsiOglLoaded=0;

// GetProc is called from wsi_opengl_glinit.inc
static void *GetProc(const char *name)
{
	void *p = (void*)GetProcPt(name);
	if ( !p || (long(p)>=-1 && long(p)<=0x3) ) // cover failure values that may be returned
	{	HMODULE module = LoadLibraryA("opengl32.dll"); // will not load multiple times but increment reference count of handle
		p = !module? 0: (void*)GetProcAddress(module, name);
		if (!p) { gsout<<"COULD NOT LOAD: "<<name<<gsnl; } else WsiOglLoaded++;
	}
	else WsiOglLoaded++;
	return p;
}

# undef PFNGLDECLTYPE
# define PFNGLDECLTYPE
# undef glcorearb_functions_h
# include <sigogl/glcorearb_functions.h>

# include <sigogl/gl_tools.h>
# include <sigogl/glr_base.h>

unsigned wsi_ogl_loaded ()
{
	// If the shader support is already solved, the shared OpenGL context and its shaders 
	// are already available, so do not load everything again and instead just return:
	return WsiOglLoaded;
}

static void loadogl ( int n )
{
	int c=0; // counter of how many functions were loaded
	# include "wsint_opengl_glinit.inc" // counter will be used for early return
}

void wsi_load_ogl ()
{
	// All our OpenGL contexts are shared, so do not load functions twice:
	if ( WsiOglLoaded>0 ) return;

	// Attach ogl renderer instantiators to the classes derived from SnShape:
	GlrBase::init ();

	// Load core OpenGL functions:
	int n = 400; // Number of OGL functions to load
	const GsVar* v = GlResources::configuration("oglfuncs");
	if (v) { n=v->geti(); if(n<300) n=300; } // use custom value, up to a limit to avoid certain crash
	loadogl ( n );

	// Declare shaders:
	GlResources::declare_default_shaders();

	GS_TRACE1("OGL  Version: "<<gl_version());
	GS_TRACE1("GLSL Version: "<<glsl_version());
}

//================================ End of File =================================================
