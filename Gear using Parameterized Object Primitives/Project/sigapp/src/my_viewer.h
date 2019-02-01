# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
#include <sig/gs_model.h>
# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>

// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* _nbut;
	bool _animating;
	SnTransform* _t1;
	SnTransform* _t2;
	SnTransform* _t3;
	SnTransform* _t4;
	SnTransform* _t5;
	SnTransform* _t6;
	SnTransform* _t7;
	SnTransform* _t8;
	SnTransform* _t9;
	SnTransform* _t10;
	//SnTransform* _t11;

	float _curang1;
	float _curang2;
	float _curang3;
	float _curang4;
	float _curang5;
	float _curang6;
	float _curang7;
	float _curang8;
	float _curang9;
	float _curang10;

   public :
	MyViewer ( int x, int y, int w, int h, const char* l );
	void add_ui ();
	void add_model ( SnShape* s, GsVec p );
	void build_scene ();
	void show_normals ( bool b );
	void run_animation ();

	//void make_gear(GsModel* m, const GsPnt& c, float radius);
	
	void make_gear_a(GsModel* m, double r1a, double r2a, double d);
	void make_gear_b(GsModel* m, double r1b, double r2b, double d1);
	void make_gear_c(GsModel* m, double r1c, double r2c, double d2);
	
	void make_gear_d(GsModel* m, double r1a, double r2a, double d);
	void make_gear_e(GsModel* m, double r1b, double r2b, double d1);
	void make_gear_f(GsModel* m, double r1c, double r2c, double d2);
	
	void make_gear_g(GsModel* m, double r1f, double r2f, double d);
	void make_gear_h(GsModel* m, double r1d, double r2d, double d3);
	void make_gear_i(GsModel* m, double r1e, double r2e, double d3);
	void make_gear_j(GsModel* m, double r1o, double r2o, double d);
	
	//void make_gear_k(GsModel* m, double r1d, double r2d, double d);
	//void make_gear_try(GsModel* m, double c, double r, double d);
	//void make_gear_try(GsModel* m, long center[3] = {15,15.0,0}, double r1,double r2, double d);
	//void make_gear_g1(GsModel* m, double c1, double r1);
	//void make_gear_c(GsModel* m, double c, double r);
	//void make_gear_b(GsModel* m);
	//void make_primitive(GsModel* m, const GsPnt& c, float radius);
	virtual int handle_keyboard ( const GsEvent &e ) override;
	virtual int uievent ( int e ) override;
};

