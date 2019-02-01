/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef SN_GROUP_H
# define SN_GROUP_H

/** \file sn_group.h 
 * groups scene nodes
 */

# include <sig/gs_array.h>
# include <sig/sn_node.h>

//======================================= SnGroup ====================================

/*! \class SnGroup sn_group.h
	\brief groups scene graph nodes

	SnGroup keeps a list of children. The group can be set or not
	to behave as a separator of the render state during action traversals.
	By default, it is not set as a separator. */   
class SnGroup : public SnNode
 { private :
	gscbool _separator;
	GsArray<SnNode*> _children;

   public :
	static const char* class_name;

   protected :
	/*! The destructor will remove all children in the subtree.
		Only accessible through unref(). */
	virtual ~SnGroup ();

   public :
	/*! Default constructor. PanelSeparator behavior is set to false. */
	SnGroup ();

	/*! Constructor to be used by derived classes, it receives the name of 
		the derived class and makes the same initializations as the default constructor. */
	SnGroup ( const char* class_name );

	/*! Sets the group separator behavior. If it is set to true, the
		render state is pushed to restored after the traversal of the
		group children. Mainly used to localize the effect of transformation
		matrices, applying only to the group children. */
	void separator ( bool b ) { _separator = (char)b; }

	/*! Returns the group separator behavior state. */
	bool separator () const { return _separator==1; }

	/*! Changes the capacity of the children array. If the requested capacity
		is smaller than the current size, nothing is done. */
	void capacity ( int c );

	/*! Compresses the children array. */
	void compress () { _children.compress(); }

	/*! Returns the number of children. */
	int size () const { return _children.size(); }

	/*! Get the child at position pos. If pos is invalid (as -1) the last
		child is returned, and if there are no children, 0 is returned. */
	SnNode* get ( int pos ) const;

	/*! Template version of get() with included type cast. */
	template <class Node> Node* get ( int pos ) const { return(Node*)get(pos); }

	/*! Get the last child. If there are no children, 0 is returned. */
	SnNode* top () const { return _children.empty()? 0:_children.top(); }

	/*! Template version of top() with included type cast. */
	template <class Node> Node* top () const { return(Node*)top(); }

	/*! Search for n and return its position, or returns -1 if not found. */
	int search ( SnNode *n ) const;

	/*! If pos<0 or pos>=num_children(), sn is appended. Otherwise, sn is inserted at pos
		with reallocation performed as needed. Method always returns sn. */
	SnNode* add ( SnNode *sn, int pos=-1 );

	/*! Template version of add() for allocating, adding a new node and then returning it. */
	template <class Node> Node* add_new ( int pos=-1 ) { return(Node*)add(new Node,pos); }

	/*! Removes one child.
		If the node removed is no more referenced by the scene graph, it is 
		deallocated together with all its sub-graph and 0 is returned. 0 is 
		also returned if the group has no children. If pos==-1 (the default)
		or pos is larger than the maximum child index, the last child is 
		removed. Otherwise, the removed node is returned. */
	SnNode *remove ( int pos=-1 );

	/*! Searches for the position of the given child pointer and removes it 
		with remove_child ( position ). */
	SnNode *remove ( SnNode *n );

	/*! Removes all children, calling the unref() method of each children. 
		The result is the same as calling remove_child() for each child. */
	void remove_all ();

	/*! Init does exacly the same as remove_all() */
	void init () { remove_all(); }

	/*! Removes child pos and insert sn in place. Same reference rules of 
		remove applies. Node sn is references and it will return the old node or 0
		in case the node is deleted due its ref counter reaching zero. If pos is 
		out of range, 0 is returned and nothing is done. */
	SnNode *replace ( int pos, SnNode *sn );
};

//================================ End of File =================================================

# endif  // SN_GROUP_H

