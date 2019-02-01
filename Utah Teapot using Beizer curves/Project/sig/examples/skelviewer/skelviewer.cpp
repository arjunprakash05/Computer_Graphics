/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include "skelviewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_dialogs.h>
# include <sigogl/ws_dialog.h>
# include <sigogl/ws_run.h>

# include <sigkin/kn_scene.h>

MySkelViewer::MySkelViewer ( KnSkeleton* s, int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{ 
	sk=s;
	add_ui ();
}

int MySkelViewer::uievent ( int e )
{ 
	switch ( e )
	{	case EvExit: ws_exit(); return 1;
		case EvPrint: print(); return 1;
	}
	return WsViewer::uievent(e);
}

// main function to create this test:
void MySkelViewer::add_ui ()
{
	UiPanel* p = uim()->add_panel ( 0, UiPanel::Vertical, UiPanel::Left ); // ww TopBar has resizing problem!
	p->add ( new UiButton ( "print", EvPrint ) );
	p->add ( new UiButton ( "exit", EvExit ) );
	uim()->build();
}

void MySkelViewer::print ()
{
	int i;
	const GsArray<KnJoint*>& j = sk->joints();
	sk->update_global_matrices();

	for ( i=0; i<j.size(); i++ )
	{	gsout << "Joint ["<<j[i]->name()<<"]\n";
		gsout << " Offset: " << j[i]->offset() << gsnl <<
			   " Pos: " << GsVec(j[i]->pos()->value()) << gsnl <<
			   " Rot: " << j[i]->rot()->value() << gsnl <<
			   " Global Mat:\n" << j[i]->gmat() <<
			   " Local Mat:\n" << j[i]->lmat();
		gsout<<gsnl;
	}
}

int main ( int argc, char **argv )
{
	// Choose a skeleton to load:
	GsStrings s;
	s.push ( "../data/gskel/gskel.s" ); // 0
	s.push ( "../data/arms/onearm.s" ); // 1
	s.push ( "../data/arms/twoarm.s" ); // 2
	s.push ( "../data/arms/torso.s" );  // 3
	int choice = 1;
	if ( choice<0 ) ui_choice ( "Select skeleton:", s );
	if ( choice<0 ) return 1;

	// load skeleton:
	KnSkeleton* sk = new KnSkeleton;
	const char* skfile = s[choice];
	if ( !sk->load(skfile) ) gsout.fatal("could not load %s",skfile);

	// create my scene and viewer:
	KnScene* sc = new KnScene; // KnScene derives SnGroup
	sc->connect ( sk ); // creates a scene for my skeleton
	if ( sk->visgeos()== 0 ) // if no geometries show the skeleton:
		sc->set_visibility ( 1/*skel*/, 0/*visgeo*/, 0/*colgeo*/, 0/*axis*/ );
	MySkelViewer* viewer = new MySkelViewer(sk,-1,-1,640,480,"skeleton test");
	viewer->root ( sc );
	viewer->view_all ();

	// show the window:
	viewer->show();

	// let it run:
	ws_run();

	return 1;
}
