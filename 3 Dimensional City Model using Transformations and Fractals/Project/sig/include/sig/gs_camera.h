/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef GS_CAMERA_H
# define GS_CAMERA_H

/** \file gs_camera.h 
 * Manages camera transformations
 */

# include <sig/gs_vec.h>
# include <sig/gs_quat.h>

class GsMat;
class GsBox;

/*! \class GsCamera gs_camera.h
	\brief Manages camera transformations

	GsCamera contains the parameters to define a camera transformation.
	Attention: if znear is too small inconsistencies in the rendering will appear;
	a minimal value of 0.1 should be considered. */
class GsCamera
{  public :
	GsPnt  eye;	//!< position of the eye, default is (0,0,1).
	GsPnt  center; //!< position where the eye is looking to, default is (0,0,0).
	GsVec  up;	 //!< the up vector orients the camera around the eye-center vector, default is (0,1,0)
	float  fovy;   //!< the y field of view in radians. Default is pi/3 (60deg), range is [0.01,pi].
	float  znear;  //!< must be >0, default is 0.1.
	float  zfar;   //!< must be >0, default is 1000.
	float  aspect; //!< normally is set to the screen width/heigh, default is 1.
	float  scale;  //!< scale factor applied to the scene before the camera
	GsQuat rotation; //!< rotation applied to the scene before the camera
	GsVec  translation; //!< translation applied to the scene before the camera

   public :
	
	/*! Initialize the camera with the default parameters, see init(). */
	GsCamera ();

	/*! Copy constructor. */
	GsCamera ( const GsCamera &c );

	/*! Set the parameters to their default values, which are :
		eye=(0,0,2), center=(0,0,0), up=(0,1,0), fovy=60deg, znear=0.1, zfar=1000, aspect=1 */
	void init ();

	/*! Returns in p the projection matrix relative to the camera transformation,
		and in m the model transformation, the final "camview" matrix should be m*p */
	void getmat ( GsMat& m, GsMat& p ) const;

	/*! Returns the "camview mat" c=m*p by calling the other getmat() method */
	void getmat ( GsMat& c ) const;

	/*! Gets the 3d ray (p1,p2) which projects exactly in the given window point
		according to the camera current parameters. Points p1 and p2 lye in the 
		near and far planes respectively. Window points are considered to be
		in normalized coordinates, ranging between [-1,1]. */
	void getray ( float winx, float winy, GsPnt& p1, GsPnt& p2 ) const;

	/*! Output camview data values in format keyword1 value \n keyword2 value ...
		(keywords are: eye, center, up, etc*/
	friend GsOutput& operator<< ( GsOutput& out, const GsCamera& c );

	/*! Input camview data. Not all keywords are required to exist. The routine
		returns when a non-keyword entry is found (which is 'ungetted' in inp). */
	friend GsInput& operator>> ( GsInput& inp, GsCamera& c );
};

//================================ End of File =================================================

# endif // GS_CAMERA_H

