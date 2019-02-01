# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
#include <sig/gs_model.h>
# include <sig/sn_primitive.h>
# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>
# include <sig/sn_manipulator.h>

# include <sig/sn_model.h>
# include <sig/sn_lines.h>

// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* _nbut;
	bool _animating;
	SnManipulator *manip1;
	SnManipulator *manip2;
	SnManipulator *manip3;
	SnManipulator *manip4;
	SnManipulator *manip5;
	SnManipulator *manip6;
	SnManipulator *manip7;
	SnManipulator *manip8;
	SnManipulator *manip9;
	SnManipulator* manip;
	SnTransform* _t3;
	SnTransform* _t3R;
	SnTransform* _t3RS;
	SnTransform* _t4;
	SnTransform* _t4R;
	SnTransform* _ty;
	SnTransform* _te;
	SnTransform* _tyS;
	SnTransform* _tF;
	SnTransform* _tF2;
	SnTransform* _tF3;
	SnTransform* _tF4;
	SnTransform* _tF5;
	SnTransform* _tF6;
	SnTransform* _tF7;
	SnTransform* _tF8;
	float _curang3;
	float _curang3R;
	float _curang3RS;
	float _curang4;
	float _curang4R;

   public :
	MyViewer ( int x, int y, int w, int h, const char* l );
	void add_ui ();
	GsVec point;
	
	
	void add_model ( SnShape* s, GsVec p, SnManipulator *manip );
	void add_model (SnShape* s, GsVec p);

	void shadow(GsModel * m);
	
	void build_scene ();
	void run_animation ();

	void FT(SnLines* l, GsPnt firstpoint, GsPnt secondpoint, float angle, int s);
	void FT2(SnLines* l, GsPnt firstpoint, GsPnt secondpoint, float angle, int s);
	void FT3(SnLines* l, GsPnt firstpoint, GsPnt secondpoint, float angle, int s);
	//void shell(SnLines* l, GsPnt firstpoint, GsPnt secondpoint, float angle, int s);

	void Rear(GsModel* m, double r1c, double r2c, double d2);
	void RearR(GsModel* m, double r1cR, double r2cR, double d2R);
	
	void Front(GsModel* m, double r1c, double r2c, double d2);
	void FrontR(GsModel* m, double r1cR, double r2cR, double d2R);

	void Ysign(GsModel* m);
	void Ysignal(GsModel* m);
	void extra(GsModel* m);

	virtual int handle_keyboard ( const GsEvent &e ) override;
	virtual int uievent ( int e ) override;
};

