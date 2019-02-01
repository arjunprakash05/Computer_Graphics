/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef KN_HOAP_IK_H
# define KN_HOAP_IK_H

# include <sig/gs_mat.h>
# include <sig/gs_shareable.h>
# include <sig/sn_shape.h>

class SnLines;
class KnJoint;
class KnColdet;
class KnSkeleton;

//====================================== KnHoapIk =====================================

/*! Analytical IK solver for the HOAP3 */
class KnHoapIk : public GsShareable
{  public :
	enum Type { LeftArm, RightArm, LeftLeg, RightLeg }; // enum order must not change
	enum RotAxis { X=0, Y=1, Z=2, NoAxis };

   private :
	Type _type;				   // linkage type
	KnJoint *_joint[6];		   // shoulder/hips joints
	RotAxis _axis[6];			 // the axis of each joint
	float _result[6];			 // stores last results: 6 for leg, 5 for arm 
	float _d1, _d2;			   // lengths of the upper arm/leg and lower arm/leg
	bool _solve_rot_goal;		 // if true (default) will compute/test the 6DOF goal
	SnLines* _snlines;			// to draw things if required
	void _init ();				// private init method
	void _setjoint ( KnSkeleton* sk, int i, const char* jname, RotAxis ax );

   public :
	/*! Default constructor initializes the object, but it will only be ready for use
		after a succesfull call to a init() method */
	KnHoapIk ();

	/*! Constructor which initializes the solver for a given type and end-effector.
		The parameters are considered exactly as in the init(t,end) method */
	KnHoapIk ( Type t, KnSkeleton* sk );

	/*! Destructor */
	virtual ~KnHoapIk ();

	/*! Give a SnLines node to let the IK draw the elbow orbit circle
		and the base-end lines each time solve() is called.
		KnHoapIk will then maintain a reference to sl (calling sl->ref()).
		Set this pointer to null to stop drawing. */
	void lines ( SnLines* sl );

	/*! Access to the scene node used for drawing lines; can be null. */
	SnLines* lines() { return _snlines; }

	/*! Returns the current type */
	Type type () const { return _type; }

	/*! Retrieves base (shoulder or hip) joints, i can be: 0, 1, or 2. */
	KnJoint* base ( int i=0 ) { return _joint[i]; }

	/*! Retrieves the mid joint (elbow or knee) */
	KnJoint* mid () { return _joint[3]; }

	/*! Retrieves 1st or 2nd end joint for the foot (i can be: 0 or 1), or
		the ik target end joint for the arm (i has no effect). */
	KnJoint* end ( int i=0 ) { return _type<2? _joint[5]:_joint[4+i]; }

	/*! Init the IK */
	void init ( Type t, KnSkeleton* sk );

	/*! Init the IK from any given joint */
	void init ( Type t, KnJoint* j );

	/*! Set limits of the base, mid, end joints to the maximum range [-pi,pi] */
	void remove_joint_limits ();

	/*! If the skeleton is not globally uptodate, updates the global matrices of the
		joints prior to the base joint by calling _base->parent()->update_gmat_up() */
	void update_base_up ();
	
	/*! Calls update_gmat_local for the joints inbetween the base and end joints */
	void update_base_end ();

	/*! Returns the global matrix of the base joint of the IK. */
	void base_frame ( GsMat& bframe );

	/*! Transforms the goal matrix in global coordinates to local coordinates with:
		local = goal * base_frame().inverse(); */
	void set_local ( GsMat& goal );

	/*! Returns the length of the base-end linkage in maximum extension */
	float linkage_len () { return _d1+_d2; }

	/*! If true (default) the 6DOF pos/orientation are considered as goal in
		solve() methods. If set to false only the position is solved */
	void solve_rot_goal ( bool b ) { _solve_rot_goal=b; }

	/*! Returns true if the rotational goal constraint is being solved */
	bool solve_rot_goal () const { return _solve_rot_goal; }

	/*! Result enumerator used in the solve() methods - this must keep the same
		values as in KnIk::Result. */
	enum Result { Ok, NotReachable, NoMidFlexion, NoBaseSwing, 
				  NoBaseTwist, NoMidTwist, NoEndSwing, Collision, Undef };

	/*! Solve with **goal matrix in local coordinates**.
		By default both pos and orientation are solved, but solve_rot_goal() can change this.
		For converting global coordinates in local coordinates, use set_local().
		Collisions are not tested here and the results are not applied to the skeleton.
		Use apply_last_result() to apply the values to the skeleton.
		Make sure the global matrices are up to date. */
	Result solve ( const GsMat& goal, float oang=0 );
	
	/*! If coldet is null, this method is the same as the prior solve() method,
		except that **this method always call apply_last_result() in case of success**.
		When a valid coldet is given, this method also tests for collisions:
		when the IK can be solved respecting joint limits, the values are
		applied to the skeleton, the global matrices of the skeleton are updated,
		and a collision query is called, maybe leading to a Collision result. */
	Result solve ( const GsMat& goal, float oang, KnColdet* coldet );

	/*! Get a pointer to an array of 7 floats containing the 7 DOF values computed
		during the last call to solve(). The 7 values can come in two formats,
		according to how KnHoapIk was initialized:
		1. Base Swing/Twist, Mid Euler flexion, Mid Euler Twist, End Swing
		2. Base Swing/Twist, Mid Euler flexion, End Swing/Twist
		PS: the values can be edited, for instance, to set the desired end joint
		orientation when only the position is being solved. */
	float* last_result () { return _result; }

	/*! Apply to the skeleton joints the values found during the last call to solve() */
	void apply_last_result () { apply_values(_result); }

	/*! Get the 7 values from the current skeleton joints and put in the user
		provided float array of 6 values */
	void get_sk_values ( float values[6] );

	/*! Put in the skeleton joints the 6 given values */
	void apply_values ( const float values[6] );

	/*! Return the lower and upper limits of the given joint index.
		Note that i==5 is only valid for the leg cases. */
	void get_limits ( int i, float& lo, float& up );

	/*! Returns a string message describing the result parameter */
	static const char* message ( Result res );
};

//======================================= EOF =====================================

# endif // KN_HOAP_IK_H
