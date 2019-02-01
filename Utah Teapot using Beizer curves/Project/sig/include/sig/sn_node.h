/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef SN_NODE_H
# define SN_NODE_H

/** \file sn_node.h 
 * base of all scene node classes
 */

# include <sig/gs.h>
# include <sig/gs_shareable.h>

//======================================= SnNode ====================================

template <typename X> class GsBuffer; // forward declaration

/*! \class SnNode sn_node.h
	\brief base of all scene node classes

	Defines a base class for all scene nodes.
	1. It cannot be directly instantiated by the user, and so there is no 
	   public constructor available.
	2. The scene graph was designed to be simple and small. All scene nodes
	   start with Sn (scene actions start with Sa, see sa_action.h).
	3. A reference counter is used for automatically deleting unused nodes,
	   see ref() and unref() methods in the parent class GsShareable. 
	4. Although a DAG is supported, each node has only information of one parent,
	   which is used to propagate its uptodate status up the hierarchy; thus,
	   the included auto update mechanism only fully works when nodes are not shared;
	   otherwise, an explict call to SnGroup::set_children_not_action_ready() to 
	   a common ancestor is needed.
	Each node has a type, among possible four types:
	1. a group manages a list of children nodes;
	2. a transform defines a transformation to affect children nodes;
	3. a shape defines a geometric shape to be rendered;
	4. an editor node handles events, a transformation, and children nodes. */
class SnNode : public GsShareable
{  public :
	enum Type { TypeGroup, TypeTransform, TypeShape, TypeEditor };

   private :
	friend class SnGroup;
	const char* _instance_name; // pointer to a static char containing the name of the derived class
	GsBuffer<void*>* _udata;
	gscenum _type;
	gscbool _visible;
   protected :
	gscbool _nodeuptodate;

   protected :
	/* Constructor requires the definition of the node type, and a const string
	   containing the name of the instantiated node. The convention for the class_name
	   is to take exactly the name of the class, for example "SnGroup", etc.
	   The type parameter defines how the node should behave when the scene graph
	   is traversed by an action and must be one of the four existing types. */
	SnNode ( Type t, const char* class_name );

	/*! This is a virtual destructor, so each derived class is responsible to 
		delete/unref its own internal data. */
	virtual ~SnNode ();
 
   public :
	/*! Return a const string with the name of the instantiated node class. */
	const char* instance_name () const { return _instance_name; }

	/*! Return the type of this node. The type is set at instantiation
		time and cannot be changed. */
	Type type () const { return (Type) _type; }

	/*! Get the visibility state of this node. Default is 1 (true). */
	bool visible () const { return _visible!=0; }

	/*! Change the visibility state of this node. */
	void visible ( bool b ) { _visible=b; }

	/*! Swap the visibility state of this node between 1 and 0. */
	void swap_visibility () { _visible=_visible?0:1; }

	/*! Attach a user data pointer to be kept within the node. Returns id for later retrieval. */
	int user_data ( void* pt );

	/*! Returns the attached user data respective to the given id, or null pointer if id is invalid. */
	void* user_data ( int id ) const;

	/*! Returns if the node is up to date */
	bool node_uptodate () const { return _nodeuptodate!=0; }

	/*! Method called right before rendering and other actions are executed on the node. 
		Nodes will use this method to update their internal data in preparation for
		rendering or some other action that will happen next. */
	virtual void update_node () {}
};

//================================ End of File =================================================

# endif  // SN_NODE_H

