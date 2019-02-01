
# include "my_viewer.h"

# include <sigogl/ws_run.h>

int main ( int argc, char** argv )
{
	double m1 = gs_time();
	MyViewer* v = new MyViewer ( -1, -1, 640, 480, "SnCircle Test" );
	v->cmd ( WsViewer::VCmdAxis );
	
	v->view_all ();
	v->show ();
	double m2 = gs_time();
	double m = m2 - m1;
	//gsout << "total time:" << m;
	ws_run ();
	
	return 1;
}
