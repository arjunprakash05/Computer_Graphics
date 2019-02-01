/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef KN_IK_SOLVER_H
# define KN_IK_SOLVER_H

# include <sigkin/kn_ik.h>
# include <sigkin/kn_hoap_ik.h>

/*! KnIkSolver is a generic interface for IK solvers */
class KnIkSolver
 { public:
	GsMat goal;
	KnColdet* coldet;
   public:
	KnIkSolver ()  { coldet=0; }
	virtual void ref ()=0;
	virtual void unref ()=0;
	virtual bool standardik ()=0;
	virtual bool init ( KnIk::Type t, KnJoint* end )=0;
	virtual void solve_rot_goal ( bool b )=0;
	virtual void set_local ()=0;
	virtual float orbit_angle ()=0;
	virtual float linkage_len ()=0;
	virtual KnJoint* base ()=0;
	virtual KnJoint* mid ()=0;
	virtual KnJoint* end ()=0;
	virtual KnIk::Result solve ()=0;
	virtual void apply_values ( const float values[7] )=0;
	virtual void apply_last_result ()=0;
};

/*! KnStdIkSolver inherits KnIk */
class KnStdIkSolver : public KnIkSolver, public KnIk
 { public:
	KnIkOrbitSearch osearch;
	bool orbitsearch;

   public:
	KnStdIkSolver ()  { orbitsearch=true; }
	virtual void ref () { KnIk::ref(); }
	virtual void unref () { KnIk::unref(); }
	void fixed_orbit ( float oang ) { orbitsearch=false; osearch.init=oang; }
	void search_orbit () { orbitsearch=true; }
	virtual bool standardik () { return true; }
	virtual bool init ( KnIk::Type t, KnJoint* end ) { return KnIk::init(t,end); }
	virtual void solve_rot_goal ( bool b ) { KnIk::solve_rot_goal(b); }
	virtual void set_local () { KnIk::set_local(goal); }
	virtual float orbit_angle () { return KnIk::orbit_angle(); }
	virtual float linkage_len () { return KnIk::linkage_len(); }
	virtual KnJoint* base () { return KnIk::base(); }
	virtual KnJoint* mid () { return KnIk::mid(); }
	virtual KnJoint* end () { return KnIk::end(); }

	virtual KnIk::Result solve ()
	 { if ( orbitsearch )
		return KnIk::solve(goal,osearch,coldet);
	   else
		return KnIk::solve(goal,osearch.init,coldet);
	 }

	virtual void apply_values ( const float values[7] ) { KnIk::apply_values(values); }
	virtual void apply_last_result () { KnIk::apply_last_result(); }
};

/*! Hoap solver version */
class KnHoapIkSolver : public KnIkSolver, public KnHoapIk
 { public:
	KnHoapIkSolver () {}
	virtual void ref () { KnHoapIk::ref(); }
	virtual void unref () { KnHoapIk::unref(); }
	virtual bool standardik () { return false; }
	virtual bool init ( KnIk::Type t, KnJoint* end ) { KnHoapIk::init ( (KnHoapIk::Type)t, end ); return true; }
	virtual void solve_rot_goal ( bool b ) { KnHoapIk::solve_rot_goal(b); }
	virtual void set_local () { KnHoapIk::set_local(goal); }
	virtual float orbit_angle () { return 0; }
	virtual float linkage_len () { return KnHoapIk::linkage_len(); }
	virtual KnJoint* base () { return KnHoapIk::base(); }
	virtual KnJoint* mid () { return KnHoapIk::mid(); }
	virtual KnJoint* end () { return KnHoapIk::end(); }

	virtual KnIk::Result solve ()
	 { return (KnIk::Result) KnHoapIk::solve(goal,0,coldet); }

	virtual void apply_values ( const float values[7] ) { KnHoapIk::apply_values(values); }
	virtual void apply_last_result () { KnHoapIk::apply_last_result(); }
};

//======================================= EOF =====================================

# endif // KN_IK_SOLVER_H
