/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef GL_RENDERER_H
# define GL_RENDERER_H

/** \file gl_renderer.h 
 * OpenGL shader-based render action
 */

# include <sig/sa_action.h>
# include <sig/sn_shape.h>
# include <sigogl/gl_context.h>

/*! \class GlRenderer gl_renderer.h
	\brief OpenGL 4 shader-based render action

	Will invoke the scene node methods for shader-based OpenGL 4 rendering.
	As scene traversal and rendering optimizations are added, it makes sense
	to use different instances for rendering different aspects of the application,
	each instance with different (or without) optimization. */
class GlRenderer : private SaAction
{  public :
	enum Mode { SceneTraversal };

   protected :
	GlContext* _context;
	Mode _mode;

   public :
	/*! Constructor requires a pointer to the (shared) GlContext to be used. 
		The given pointer will then be passed to the render methods. */
	GlRenderer ( GlContext* c );

	/*! Virtual destructor */
	virtual ~GlRenderer ();

	/*! Set the rendering optimization mode */
	void mode ( Mode m ) { _mode=m; }

	/*! Provides access to GsShareable::ref(). */
	void ref () { GsShareable::ref(); }

	/*! Provides access to GsShareable::unref(). */
	void unref () { GsShareable::unref(); }

	/*! Access the rendering context that will be passed to the scene */
	GlContext* glcontext () { return _context; }

	/*! Traverse all scene graph overriding the render mode of each shape node */
	void override_render_mode ( SnNode* n, gsRenderMode m );

	/*! Traverse all scene graph restoring the original render mode of each
		shape node, saved at the last override_render_mode() call */
	void restore_render_mode ( SnNode* n );

	/*! Initializes the matrix stack and the context transformations to identity. */
	void init () { init_matrix(); _context->projection(&GsMat::id); _context->localframe(&_matstack[0]); }

	/*! Initializes the matrix stack and the local frame, and set the projection transformation to p. */
	void init ( const GsMat* p ) 
	 { init_matrix(); _context->projection(p); _context->localframe(&_matstack[0]); }

	/*! Initializes the matrix stack and the context transformations. */
	void init ( const GsMat* p, const GsMat* l ) 
	 { init_matrix(*l); _context->projection(p); _context->localframe(&_matstack[0]); }

	/*! Calls the base class apply() method. */
	void apply ( SnNode* n );

   private :
	virtual bool shape_apply ( SnShape* s ) override;
	virtual void push_matrix () override;
	virtual void pop_matrix () override;
};

//================================ End of File =================================================

# endif // GL_RENDERER_H
