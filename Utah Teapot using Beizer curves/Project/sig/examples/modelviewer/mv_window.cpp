/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include "mv_window.h"

# include <sig/gs_scandir.h>
# include <sig/gs_box.h>
# include <sig/gs_string.h>
# include <sig/gs_model.h>
# include <sig/sn_primitive.h>
# include <sig/sn_group.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>
# include <sigogl/ui_dialogs.h>
# include <sigogl/ui_button.h>
# include <sigogl/ui_check_button.h>
//# include <sigogl/ui_radio_button.h>
//# include <sigogl/ui_color_chooser.h>
//# include <sigogl/ui_input.h>
//# include <sigogl/ui_output.h>

# include "mv_main.h"
# include "mv_scene.h"

//# define GS_USE_TRACE1 
# include <sig/gs_trace.h>

MvWindow::MvWindow ( int x, int y, int w, int h, const char *label ) : WsViewer(x,y,w,h,label)
{
	_last_crease_angle_deg = 35.0f;
	_last_normals_prec_deg =  1.0f;
	WsViewer::root ( new MvScene );
	build_ui();
}

MvWindow::~MvWindow ()
{
}

enum Cmd { CmdLoad, CmdSave, CmdNew, CmdExit, 
		   CmdSelectAll, CmdSelectNone, CmdModelSelected,
		   CmdViewModel, CmdViewNormals, CmdViewSelection, CmdViewWireframe,
		   CmdNotUsed};

void MvWindow::build_ui ()
{
	UiManager* uim = WsWindow::uim(); // retrieve UI manager of the window
	UiPanel* sp; // current sub panel

	UiPanel* p = uim->add_panel ( "", UiPanel::HorizLeft, 5, 5 );// p->color().ln.a=0;
	p->add ( new UiButton ( "File", sp=new UiPanel() ) );
	{	UiPanel* p = sp;
		p->add ( new UiButton ( "load- F2", CmdLoad ) ); uim->cmd_key(GsEvent::KeyF2,CmdLoad);
		p->add ( new UiButton ( "save (F3)", CmdSave ) );
		p->add ( new UiButton ( "new", CmdNew ) );
		p->add ( new UiButton ( "e&xit", CmdExit ) ); p->top()->separate();
	}
	p->add ( new UiButton ( "Select", sp=new UiPanel() ) );
	{	UiPanel* p = sp;
		p->add ( new UiButton ( "all", CmdSelectAll ) ); 
		p->add ( new UiButton ( "none", CmdSelectNone ) );
		p->add ( new UiButton ( "last one", CmdNotUsed ) );
		_spanelcmds = p->elements();
		_spanel=p;
	}
	p->add ( new UiButton ( "View", sp=new UiPanel() ) );
	{	UiPanel* p = sp;
		p->add ( _viewbut[0]=new UiCheckButton ( "model", CmdViewModel ) ); 
		p->add ( _viewbut[1]=new UiCheckButton ( "normals", CmdViewNormals ) );
		p->add ( _viewbut[2]=new UiCheckButton ( "selection", CmdViewSelection ) );
		//p->add ( _viewbut[3]=new UiCheckButton ( "statistics", CmdNotUsed ) ); p->top()->separate();
		p->add ( new UiButton ( "statistics", CmdNotUsed ) ); p->top()->separate();
	}
	p->add ( new UiButton ( "Action", sp=new UiPanel() ) );
	{	UiPanel* p = sp;
		p->add ( new UiButton ( "smooth", CmdNotUsed ) ); 
		p->add ( new UiButton ( "flat", CmdNotUsed ) );
		//p->add ( new UiButton ( "view statistics", CmdNotUsed ) );
	}
uim->build();
   /*uim->add_command_key ( key, cmd );
   uim->replace_command_key ( key, cmd );
   uim->clear_accelerator_keys ( key, cmd );
*/
 }

void MvWindow::add_model ( const char* fname )
{
	GsArray<const char*> multif;
	if ( !fname )
	{	fname = ui_select_file ( "Enter file to load:", _lastfile, "*.m;*.obj;*.s", &multif );
		if ( !fname ) return;
		_lastfile = fname;
	}

	GsStrings files;
	if ( multif.size() )
	{	ui_get_full_names ( fname, multif, files );
	}
	else
	{	files.push(fname);
	}

	int loaded=0;
	SnModel* lsmodel=0;
	for ( int i=0; i<files.size(); i++ )
	{	SnModel* sm = new SnModel;
		sm->ref();
		if ( sm->model()->load(files[i]) )
		{	loaded++;
			lsmodel=sm;
			root()->add_model ( sm );
			const char* mname = sm->model()->name.pt();
			_spanel->add ( new UiCheckButton ( mname, CmdModelSelected ) );
			if ( _spanel->elements()==_spanelcmds+1 ) _spanel->top()->separate();
		}
		sm->unref();
	}

	if ( loaded==0 )
	{	message().setf("Not found: [%s]",fname); return; }
	else
	{ //_spanel->changed(2);
		if ( loaded==1 ) message().setf("Loaded: %s",lsmodel->model()->filename);
		else message().setf("Loaded: %d models",loaded);
		_spanel->build();
		//if ( isdir ) update_selection(1);
		// else select_top();
		update_visibility();
		view_all();
	}
}

void MvWindow::select_top ()
{ 
	update_selection(0);
	((UiCheckButton*)_spanel->top())->value(1);
	root()->set_visibility(root()->size()-1,-1,-1,1,-1);
}

void MvWindow::update_selection ( int val ) // 1:sel all, 0:unselect, -1:upd scene sel from menu
{ 
	int e, i;
	UiCheckButton* but;

	for ( e=_spanelcmds; e<_spanel->elements(); e++ ) 
	{	but = (UiCheckButton*)_spanel->get(e);
		i = e-_spanelcmds;
		if ( val>=0 ) // select all or none according to val
		{	but->value(val);
			root()->set_visibility(i,-1,-1,val,-1);
		}
		else // update scene selection according to menu flags
		{	root()->set_visibility(i,-1,-1,but->value(),-1); 
		}
	}

//	update_visibility();
}

void MvWindow::update_visibility ()
{ 
	int mov, nov, selv;
	root()->get_group_visibility(mov,nov,selv);
	_viewbut[0]->value(mov);
	_viewbut[1]->value(nov);
	_viewbut[2]->value(selv);
}

//==================== virtuals ===================================

int MvWindow::uievent ( int cmd )
{ 
	int res = WsViewer::uievent ( cmd );
	if ( res ) return res;

   //   output->text().setf("event %d (%s)",++numev,cmd);

	switch ( cmd )
	{	case CmdExit: //if ( ui_ask ( "Do you really want to exit?" ) ) ws_exit(); break;
					 ws_exit(); break;

		case CmdLoad: add_model(); break;

		case CmdNew: 
		   { const char* buf = ui_input ( 
	"box <ra> <rb> <rc> 0\n"
	"sphere <radius> <nfaces>\n"
	"cylinder|capsule|ellipsoid <ra> <rb> <raxis> <nfaces>\n",
			0, "Describe Primitive" );//, WsWindow* win )}
 GsInput inp; inp.init(buf);
 GsPrimitive prim;
 inp>>prim;
gsout<<prim<<gsnl;
		SnModel* sm = new SnModel;
		sm->model()->make_primitive ( prim );
			root()->add_model ( sm );
			_spanel->add ( new UiCheckButton ( sm->model()->name, CmdModelSelected ) );
			//if ( _spanel->elements()==_spanelcmds+1 ) _spanel->top()->separate();

}break;

		case CmdModelSelected: update_selection(-1); break;
		case CmdSelectNone: update_selection(0); break;
		case CmdSelectAll : update_selection(1); break;

		case CmdViewModel:	 root()->set_group_visibility ( _viewbut[0]->value()?1:0, -1, -1 ); break;
		case CmdViewNormals:   root()->set_group_visibility ( -1, _viewbut[1]->value()?1:0, -1 ); break;
		case CmdViewSelection: root()->set_group_visibility ( -1, -1, _viewbut[2]->value()?1:0 ); break;

		return 0; // event not recognized
	}
	return 1; // since uievent() is not overriden it is ok to always return 1 here
}

int MvWindow::handle_keyboard ( const GsEvent& e )
{
	return WsViewer::handle_keyboard(e);
}

int MvWindow::handle_scene_event ( const GsEvent& e )
{
	if ( e.type==GsEvent::Push // &&
//		TheMainWin->ui_pick_face_but->value() &&
//		TheMaterialTab->cur_color()>=0 &&
//		TheMainWin->cur_model() 
)

// create a SaModelSelection? [Model,Face,etc]

	{
/*
	  GsModel& m = *TheMainWin->cur_model();
	  int f = m.pick_face ( e.ray );
	  if ( f>=0 )
	   { //_lastmtl=m.Fm[f]; _lastfac=f; // save here in case of undo
//		 m.Fm[f] = TheMaterialTab->cur_color();
		 TheMainWin->cur_smodel()->changed(true);
//		 TheMainWin->ui_viewer->render();
	   }
	  return 1;*/
	}
   return WsViewer::handle_scene_event(e);
}


