/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/
 
# include <sig/gs_event.h>
# include <sig/sn_lines.h>
# include <sig/sn_model.h>
# include <sig/sn_group.h>

# include <sigogl/ws_viewer.h>
# include <sigogl/ws_run.h>

class CamViewer;
class ObjViewer;

// use some global pointers to make things easier:
CamViewer *TheCamViewer=0;
ObjViewer *TheObjViewer=0;

//=========================== CamViewer =============================

class CamViewer : public WsViewer
{  public :
	SnLines *Ray;
	SnLines *Pnt;
   public :
	CamViewer ( SnNode* n, int x, int y, int w, int h );
   ~CamViewer () { ws_exit(); } // for consistency when one window closes the application ends
};

CamViewer::CamViewer ( SnNode* n, int x, int y, int w, int h ) : WsViewer(x,y,w,h)
{
	rootg()->add ( Ray=new SnLines );
	rootg()->add ( Pnt=new SnLines );
	rootg()->add ( n );
	cmd ( WsViewer::VCmdAxis );
	view_all();
	show();
}

//=========================== ObjViewer =============================

class ObjViewer : public WsViewer
{  public :
	float clen;
	ObjViewer ( SnNode* n, int x, int y, int w, int h );
   ~ObjViewer () { ws_exit(); } // for consistency when one window closes the application ends
	virtual int handle ( const GsEvent& e );
	virtual int handle_scene_event ( const GsEvent& e );
};

ObjViewer::ObjViewer ( SnNode* n, int x, int y, int w, int h ) : WsViewer(x,y,w,h)
{
	clen = 1.0f;
	root ( n );
	cmd ( WsViewer::VCmdAxis );
	view_all();
	show();
}

int ObjViewer::handle ( const GsEvent& e )
{
	GsMat M, P;
	GsCamera cam = camera();
	cam.getmat(M,P);
	M.invert();

	TheCamViewer->render();

	if ( e.type==GsEvent::Keyboard )
	{ 
		if ( e.key==GsEvent::KeyEsc ) ws_exit();

		if ( e.key==GsEvent::KeySpace )
		{	render();
			return 1;
		}
	}

	return WsViewer::handle(e);
}

int ObjViewer::handle_scene_event ( const GsEvent& e )
{
	GsVec p1, p2;
	p1 = e.ray.p1;
	p2 = e.ray.p2;

	// another way of getting the ray is:
	// GsCamera cam;
	// cam = camera ();
	// cam.getray ( e.mouse.x, e.mouse.y, p1, p2 );

	TheCamViewer->Ray->init ();
	TheCamViewer->Ray->push ( GsColor::cyan );
	TheCamViewer->Ray->push ( p1, p2 );

	TheCamViewer->Pnt->init ();
	TheCamViewer->Pnt->push ( GsColor::magenta );
	TheCamViewer->Pnt->push_cross ( e.mousep, clen );

	return 1; // tell that I used the event
}

//=========================== main =============================

int main ( int argc, char** argv )
{
	GsModel* model = new GsModel;
	const char* s = "../data/models/lamp.m";
	float clen = 5.0f;

	if ( !model->load(s) )
	{	model->make_capsule ( GsVec(-10,0,0), GsVec(10,0,0), 4, 4, 20, true );
		GsMaterial m;
		m.diffuse = GsColor::lightblue;
		model->set_one_material ( m );
		clen = 0.8f;
	}

	SnGroup* g = new SnGroup;
	g->add ( new SnModel(model) );

	int sw, sh, ww=400, wh=320, sep=20;
	ws_screen_resolution ( sw, sh );
	int x = (sw-sep-2*ww)/2;
	TheCamViewer = new CamViewer ( g, x, -1, ww, wh );
	TheObjViewer = new ObjViewer ( g, x+ww+sep, -1, ww, wh );
	TheObjViewer->clen = clen;
	ws_run();
}
