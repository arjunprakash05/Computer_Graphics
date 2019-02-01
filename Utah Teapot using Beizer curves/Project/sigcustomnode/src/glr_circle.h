# pragma once


# include <sigogl/glr_base.h>
//# include <sig/sn_shape.h>
# include <sigogl/gl_objects.h>

/*! \class GlrLines glr_lines.h
	Renderer for SnLines */
class GlrCircle : public GlrBase
 { protected :
	GlObjects _glo; // indices for opengl vertex arrays and buffers
	gsuint _psize;
   public :
	GlrCircle ();
	virtual ~GlrCircle ();
	virtual void init ( SnShape* s ) override;
	virtual void render ( SnShape* s, GlContext* c ) override;
};

//================================ End of File =================================================


