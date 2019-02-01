/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef KN_COLDET_H
# define KN_COLDET_H

//================================ KnColdet =================================================

# include <sig/gs_slot_map.h>
# include <sig/gs_shareable.h>

class KnJoint;
class KnSkeleton;
class CdManager;

/*! Manages collision detection between several articulated objects,
	including self-collisions. Based on the CdManager class.
	Note: this class does not allow inserting GsModels alone; instead,
	a single-joint KnSkeleton must be created to reference the model. */
class KnColdet : public GsShareable
 { private :
	class SkelData;
	GsSlotMap<SkelData> _skels;
	CdManager* _coldet;
	GsArray<KnJoint*> _joints; // contains all joints

   public :
	/*! Constructor */
	KnColdet ();

	/*! Destructor is public but be sure to respect ref()/unref() */
   ~KnColdet ();

	/*! Clears everything, ie, disconnects all skeletons */
	void init ();

	/*! Returns the total number of collision geometries being considered */
	int num_objects () { return _joints.size(); }

	/*! Connects and constructs internal structures for each collision
		geometry attached to the skeleton. It is assumed that the skeleton
		is in a collision-free posture, and so all initially colliding pairs
		are deactivated. The number of encountered colliding pairs is returned.
		The declared non-colliding-pairs in the skeleton are automatically deactivated.
		The id of the inserted skeleton can be retrieved from the KnSkeleton methods.
		Several calls to connect() can be done in order to add several skeletons.
		If the skeleton is already connected to any KnColdet object, it is "re-inserted".
		If pointer pairs is not null, the deactivated pairs are pushed in the array.
		If deactivate_adjacent is set to true (the default), deactivate_adjacent_joints()
		is automatically called. */
	int connect ( KnSkeleton* s, GsArray<KnJoint*>* pairs=0, bool deactivate_adjacent=true );

	/*! Updates the global transformation matrices of each collision geometry
		in s, which has to be connected. The number of updated geometries is returned. */
	int update ( KnSkeleton* s );

	/*! Updates the global transformation matrix of the given joint. This
		is a low level method that assumes j valid and connected to coldet */
	void update ( KnJoint* j );

	/*! Recursivelly updates the global transformation matrix of the given joint
		and all its children. Assumes j valid and connected to coldet */
	void update_subtree ( KnJoint* j );

	/*! Check for all collisions returning true if any collisions were found.
		All colliding pairs are returned in the giving array */
	bool collide ( GsArray<KnJoint*>& pairs );

	/*! Check for collisions returning true if any collisions were found. */
	bool collide ();

	/*! Output a report of all colliding pairs */
	void collide_report ( GsOutput& o );

	/*! Returns true if a pair is closer than toler. */
	bool collide_tolerance ( float toler );

	/*! Deactivate all pairs of adjacent joints in the skeleton hierarchy.
		This operation is (by default) performed by method connect(). */
	int deactivate_adjacent_joints ( KnSkeleton* kn );

	/*! Deactivate a pair of joints */
	bool deactivate ( KnJoint* j1, KnJoint* j2 );

	/*! Activate a pair of joints */
	bool activate ( KnJoint* j1, KnJoint* j2 );

	/*! Deactivate a joint */
	void deactivate ( KnJoint* j );

	/*! Activate a joint */
	void activate ( KnJoint* j );

	/*! Deactivate all joints in a skeleton */
	void deactivate ( KnSkeleton* s );

	/*! Activate all joints in a skeleton */
	void activate ( KnSkeleton* s );
	
	/*! Returns true if the given pair is deactivated and false otherwise */
	bool deactivated ( KnJoint* j1, KnJoint* j2 );

	/*! Counts and returns the number of deactivated pairs of joints */
	int count_deactivated_pairs ();

   private :
	void _addjoint ( int id, KnJoint* j );
};


//================================ End of File =================================================

# endif  // KN_COLDET_H
