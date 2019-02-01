/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# ifndef MV_SCENE_H
# define MV_SCENE_H

# include <sig/sn_group.h>
# include <sig/sn_model.h>

class SnManipulator;

// maintained scene graph :
//
//				 root
//	   |		  |		 |
//  manipulator	...	manipulator
//	   |					|
//	 group				group
//   |	   |			|	   |   
// model  normals	   model  normals

class MvScene : public SnGroup
 { private :
	int _lastselfac, _lastselmtl; // last selected items

   public :
	MvScene ();

	int models () const { return size(); }
	void add_model ( SnModel* model );
	bool remove_model ( int i );

	SnManipulator* manipulator ( int i );
	SnGroup* group ( int i );
	SnModel* smodel ( int i );
	GsModel* model ( int i );

	void reset_normals ( int i );
	void make_normals ( int i );

	void get_visibility ( int i, bool& model, bool& normals, bool& manip, bool& manbox );
	void set_visibility ( int i, int model, int normals, int manip, int manbox );

	void set_scene_visibility ( int model, int normals, int manips, int manbox );

	void set_group_visibility ( int model, int normals, int manbox );
	void get_group_visibility ( int& model, int& normals, int& manbox );
};

# endif // MV_SCENE_H
