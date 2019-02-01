/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/sn_lines.h>
# include <sig/sn_model.h>
# include <sig/sn_group.h>
# include <sig/sn_shape.h>
# include <sig/sn_manipulator.h>

# include "mv_scene.h"

# define GS_USE_TRACE1  // event tracing
# include <sig/gs_trace.h>

MvScene::MvScene () 
 {
   _lastselfac = _lastselmtl = -1;
 }

void MvScene::add_model ( SnModel* model )
 {
   SnManipulator* manip = new SnManipulator;
   SnGroup* group = new SnGroup;
   SnLines* lines = new SnLines;

   lines->visible ( false );
   manip->visible ( false );

   group->add ( model );
   group->add ( lines );
   manip->child ( group );
   add ( manip );
 }

# define CHECK(i)  if ( i<0 || i>=size() ) gsout.fatal("Invalid index when accessing scene item!");

bool MvScene::remove_model ( int i )
 {
   CHECK(i);
   remove ( i );
   return true;
 }

SnManipulator* MvScene::manipulator ( int i )
 {
   CHECK(i);
   return (SnManipulator*)get(i);
 }

SnGroup* MvScene::group ( int i )
 {
   return (SnGroup*)manipulator(i)->child();
 }

SnModel* MvScene::smodel ( int i )
 {
   return (SnModel*)group(i)->get(0);
 }

GsModel* MvScene::model ( int i )
 {
   return smodel(i)->model();
 }

void MvScene::reset_normals ( int i )
 {
   SnGroup* g = group(i);
   SnLines* slines = (SnLines*)g->get(1);
   slines->init ();
   if ( slines->visible() ) make_normals (i);
 }

void MvScene::make_normals ( int i )
 {
   SnGroup* g = group(i);
   const GsModel& m = *g->get<SnModel>(0)->cmodel();
   SnLines& l = *g->get<SnLines>(1);
   l.init ();

   if ( m.geomode()==GsModel::Empty ) return;

   GsBox box;
   m.get_bounding_box(box);
   float nsize = box.maxsize()/20.0f;

   int f, a, b, c, an, bn, cn;
   GsPnt center;
   GsVec normal;

   # define ADDNORM(c,n) l.push ( c, c+n*nsize )
   l.push ( GsColor::red );

   if ( m.geomode()==GsModel::Faces ) // there are no normals in N
	{ for ( f=0; f<m.F.size(); f++ )
	   { center = m.face_center(f);
		 ADDNORM ( center, m.face_normal(f) );
	   }
	}
   else if ( m.geomode()==GsModel::Flat ) // N.size()==F.size()
	{ for ( f=0; f<m.F.size(); f++ )
	   { center = m.face_center(f);
		 ADDNORM ( center, m.N[f] );
	   }
	}
   else if ( m.geomode()==GsModel::Smooth ) // N.size()==V.size()
	{ for ( f=0; f<m.N.size(); f++ )
	   { ADDNORM ( m.V[f], m.N[f] );
	   }
	}
   else if ( m.geomode()==GsModel::Hybrid ) // Fn.size()==F.size()
	{ for ( f=0; f<m.F.size(); f++ )
	   { a = m.F[f].a; b=m.F[f].b; c=m.F[f].c;
		 an = m.Fn[f].a; bn=m.Fn[f].b; cn=m.Fn[f].c;
		 ADDNORM ( m.V[a], m.N[an] );
 		 ADDNORM ( m.V[b], m.N[bn] );
 		 ADDNORM ( m.V[c], m.N[cn] );
	   }
	}
   else
	{ gsout.fatal("Unknown model geomode()!"); }

   # undef ADDNORM
 }

void MvScene::get_visibility ( int i, bool& model, bool& normals, bool& manip, bool& manbox )
 {
   model = normals = manip = false;
   SnManipulator* smanip = manipulator(i);
   SnGroup* group = (SnGroup*) smanip->child();
   SnModel* smodel = (SnModel*) group->get(0);
   SnLines* slines = (SnLines*) group->get(1);

   model = smodel->visible();
   normals = slines->visible();
   manip = smanip->visible();
   manbox = smanip->draw_box();
   //gsout<<smodel->shape().name<<": "<<model<<gspc<<normals<<gspc<<manip<<gsnl;
 }

void MvScene::set_visibility ( int i, int model, int normals, int manip, int manbox )
 {
   //gsout<<model<<gspc<<normals<<gspc<<manip<<gsnl;
   SnManipulator* smanip = manipulator(i);
   SnGroup* group = (SnGroup*) smanip->child();

   if ( manip>=0 ) { smanip->visible(manip!=0); smanip->update(); }

   if ( manbox>=0 ) { smanip->draw_box(manbox!=0); smanip->update(); }

   if (model>=0) { ((SnModel*)group->get(0))->visible(model!=0); }

   if ( normals>=0 ) 
	{ SnLines* slines = (SnLines*) group->get(1);
	  slines->visible(normals!=0);
	  if ( normals && slines->empty() ) make_normals ( i );
	}
 }

void MvScene::set_scene_visibility ( int model, int normals, int manips, int manbox )
 {
   //gsout<<model<<gspc<<normals<<gspc<<manip<<gsnl;
   for ( int i=0; i<size(); i++ )
	set_visibility(i,model,normals,manips, manbox);
 }

void MvScene::set_group_visibility ( int model, int normals, int manbox )
 {
   for ( int i=0; i<size(); i++ )
	{ if ( manipulator(i)->visible() ) set_visibility(i,model,normals,-1,manbox);
	}
 }

void MvScene::get_group_visibility ( int& model, int& normals, int& manbox )
 {
   model = normals = manbox = -1; // initially mark as undefined
   bool firstone=true;
   bool mo, no, ma, mb;
   for ( int i=0; i<size(); i++ )
	{ if ( manipulator(i)->visible() )
	   { get_visibility(i,mo,no,ma,mb);
		 if ( firstone ) // get first one
		  { model=mo; normals=no; manbox=mb; firstone=false;
gsout<<mo<<gsnl;
 }
		 else // update
		  { if ( model>=0 ) { if (model!=(int)mo) model=-1; }
			if ( normals>=0 ) { if (normals!=(int)no) normals=-1; }
			if ( manbox>=0 ) { if (manbox!=(int)mb) manbox=-1; }
			if ( model<0 && normals<0 && manbox<0 ) return; // undefined for both
		  }
	   }
	}
 }

//============================= EOF ===================================
