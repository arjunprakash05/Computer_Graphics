/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/sn_poly_editor.h>
# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>
# include <sigogl/ws_run.h>

// Viewer for this example:
class PolyEdViewer : public WsViewer
{  public :
	enum MenuEv { EvPrint, EvHelp, EvExit };
	SnPolyEditor* polyed;
   public :
	PolyEdViewer ( int x, int y, int w, int h, const char* l );
	void add_ui ();
	void print ();
	virtual int uievent ( int e ) override;
};

PolyEdViewer::PolyEdViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{ 
	root ( polyed=new SnPolyEditor );
}

const char* Msg = "Keys:\n"
				  "Esc: switch edition mode\n"
				  "d: change drawing mode\n"
				  "s: change selection type";

int PolyEdViewer::uievent ( int e )
{
	switch ( e )
	{	case EvPrint: print(); break;
		case EvHelp:  output_pos(0,40); output(output().len()>0?0:Msg); break;
		case EvExit:  ws_exit(); break;
		default:	  return WsViewer::uievent(e);
	}
	return 1;
}

void PolyEdViewer::print ()
{
	GsPolygons& polys = *polyed->polygons();

	gsout<<"Polygons being edited:\n\n";

	// The output function of GsPolygons can be used:
	// gsout<<polys<<gsnl<<gsnl;

	// Here is an example of how to access each vertex for a custom output:
	GsPnt2 vertex; // GsPnt2 is the same as (it is a typedef of) GsVec2
	for ( int i=0; i<polys.size(); i++ )
	{	gsout << "Polygon " << i << ":\n";
		for ( int j=0; j<polys[i].size(); j++ )
		{	vertex = polys[i][j];
			gsout<< gs_round(vertex.x,0.01f) << "," << gs_round(vertex.y,0.01f) << " ";
		}
		gsout<<gsnl<<gsnl;
	}
}

void PolyEdViewer::add_ui ()
{
	UiPanel* p = uim()->add_panel ( 0, UiPanel::HorizLeft, UiPanel::Top ); // ww TopBar has resizing problem!
	p->add ( new UiButton ( "print", EvPrint ) );
	p->add ( new UiButton ( "help", EvHelp ) );
	p->add ( new UiButton ( "exit", EvExit ) );
	uim()->build();
}

int main ( int argc, char** argv )
{
	PolyEdViewer* viewer = new PolyEdViewer(-1,-1,640,480,"SnPolyEditor test");
	viewer->add_ui ();
	viewer->cmd ( WsViewer::VCmdPlanar );
	viewer->cmd ( WsViewer::VCmdAxis );
	viewer->show();

	ws_run();
	return 0;
}
