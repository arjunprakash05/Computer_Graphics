/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/gs_string.h>
# include <sig/sn_model.h>
# include <sig/sn_lines.h>
# include <sig/sn_group.h>

# include <sigogl/ws_run.h>

# include "mv_main.h"
# include "mv_window.h"
# include "mv_scene.h"

MvApp* App=0;

MvApp::MvApp ()
 {
   window = new MvWindow ( -1,-1,640,480, "SIG Model Viewer");
   scene = window->root();
 }

int main ( int argc, char **argv )
 {
   App = new MvApp;

   if ( argc>1 )
	{ //App->window->open ( argv[1] );
	}

	gs_show_console();
	//App->window->add_model ( "../../", "bodie.obj" );
	//App->window->add_model ( "../../models/dir", "obj" );
	//App->window->add_model ( "../../models/adobemodel/MODEL.obj" );
	//App->window->add_model ( "../../models/manmodel/manmodel.obj" );
	//App->window->add_model ( "../../models/toonkid/toonkid.obj" );
	App->window->add_model ( "../../models/playground.obj" );

	//App->window->add_model ( "../../models/mh/mhman.obj" );
	//App->window->add_model ( "../../models/test/texsquare.obj" );

   App->window->cmd ( WsViewer::VCmdAxis );
   App->window->view_all();

   App->window->show();

   ws_run();

   return 0;
 }
