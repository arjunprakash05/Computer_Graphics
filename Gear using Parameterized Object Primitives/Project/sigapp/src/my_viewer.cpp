# include "my_viewer.h"
# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
#include<sig\gs_math.h>
# include <sigogl/ws_run.h>
#define pie 3.14;

MyViewer::MyViewer(int x, int y, int w, int h, const char* l) : WsViewer(x, y, w, h, l)
{
	//double t1 = gs_time();
	//gsout << t1 << gsnl;

	_nbut = 0;
	_animating = false;
	add_ui();
	build_scene();

	//double t2 = gs_time();
	//gsout << t2 << gsnl;
	//gsout << (t2 - t1) << gsnl;
}

void MyViewer::add_ui()
{
	UiPanel *p, *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel("", UiPanel::HorizLeft);
	p->add(new UiButton("View", sp = new UiPanel()));
	{	UiPanel* p = sp;
	p->add(_nbut = new UiCheckButton("Normals", EvNormals));
	}
	p->add(new UiButton("Animate", EvAnimate));
	p->add(new UiButton("Exit", EvExit)); p->top()->separate();
}

void MyViewer::add_model(SnShape* s, GsVec p)
{
	SnManipulator* manip = new SnManipulator;
	GsMat m;
	m.translation(p);
	manip->initial_mat(m);

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);

	rootg()->add(manip);
}

//Make gear functions:
void MyViewer::make_gear_a(GsModel* m, double r1a, double r2a, double d) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = 0;
	double l1 = r1a;
	double l2 = r2a;
	double t = d;
	m->V.size(98);
	m->V[0].set(0, 0, 0);
	for (int i = 1; i < 49; i = i + 4)
	{
		m->V[i].set(l1*cos(theta), l1*sin(theta), 0.0);
		m->V[i + 1].set(l1*cos(theta + 15 * 3.14 / 180), l1*sin(theta + 15 * 3.14 / 180), 0.0);
		m->V[i + 2].set(l2*cos(theta + 15 * 3.14 / 180), l2*sin(theta + 15 * 3.14 / 180), 0.0);
		m->V[i + 3].set(l2*cos(theta + 30 * 3.14 / 180), l2*sin(theta + 30 * 3.14 / 180), 0.0);

		m->F.push().set(0, i, i + 1);
		m->F.push().set(0, i + 1, i);

		m->F.push().set(0, i + 2, i + 3);
		m->F.push().set(0, i + 3, i + 2);

		theta = theta + 30 * 3.14 / 180;
	}
	m->V[49].set(0, 0, 2.1f);
	for (int i = 50; i < 98; i = i + 4)
	{
		m->V[i].set(l1*cos(theta), l1*sin(theta), t);
		m->V[i + 1].set(l1*cos(theta + 15 * 3.14 / 180), l1*sin(theta + 15 * 3.14 / 180), t);
		m->V[i + 2].set(l2*cos(theta + 15 * 3.14 / 180), l2*sin(theta + 15 * 3.14 / 180), t);
		m->V[i + 3].set(l2*cos(theta + 30 * 3.14 / 180), l2*sin(theta + 30 * 3.14 / 180), t);

		m->F.push().set(49, i, i + 1);
		m->F.push().set(49, i + 1, i);

		m->F.push().set(49, i + 2, i + 3);
		m->F.push().set(49, i + 3, i + 2);

		theta = theta + 30 * 3.14 / 180;
	}

	for (int i = 0; i < 47; i = i + 1) {
		m->F.push().set(i + 50, i + 1, i + 2);
		m->F.push().set(i + 50, i + 2, i + 51);
	}

	m->F.push().set(49, 24, 1);
	m->F.push().set(26, 49, 1);
	
	}


void MyViewer::make_gear_b(GsModel* m, double r1b, double r2b, double d1) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = 0;
	double l1 = r1b;

	double l2 = r2b;

	double t = d1;
	m->V.size(50);
	m->V[0].set(0, 0, 0);
	for (int i = 1; i < 25; i = i + 4)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), 0.0);
		m->V[i + 1].set(l1*cos(theta + 30 * 3.14 / 180), l1*sin(theta + 30 * 3.14 / 180), 0.0);
		m->V[i + 2].set(l2*cos(theta + 30 * 3.14 / 180), l2*sin(theta + 30 * 3.14 / 180), 0.0);
		m->V[i + 3].set(l2*cos(theta + 60 * 3.14 / 180), l2*sin(theta + 60 * 3.14 / 180), 0.0);

		m->F.push().set(0, i, i + 1);
		m->F.push().set(0, i + 1, i);

		m->F.push().set(0, i + 2, i + 3);
		m->F.push().set(0, i + 3, i + 2);


		theta = theta + 60 * 3.14 / 180;
	}

	//m->V.size(25);
	m->V[25].set(0, 0, 3.1f);
	for (int i = 26; i < 50; i = i + 4)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), t);
		m->V[i + 1].set(l1*cos(theta + 30 * 3.14 / 180), l1*sin(theta + 30 * 3.14 / 180), t);
		m->V[i + 2].set(l2*cos(theta + 30 * 3.14 / 180), l2*sin(theta + 30 * 3.14 / 180), t);
		m->V[i + 3].set(l2*cos(theta + 60 * 3.14 / 180), l2*sin(theta + 60 * 3.14 / 180), t);

		m->F.push().set(25, i, i + 1);
		m->F.push().set(25, i + 1, i);


		m->F.push().set(25, i + 2, i + 3);
		m->F.push().set(25, i + 3, i + 2);


		theta = theta + 60 * 3.14 / 180;
	}
	for (int i = 0; i < 23; i = i + 1) {
		m->F.push().set(i + 26, i + 1, i + 2);
		m->F.push().set(i + 26, i + 2, i + 27);
	}

	m->F.push().set(49, 24, 1);
	m->F.push().set(26, 49, 1);


}

void MyViewer::make_gear_c(GsModel* m, double r1c, double r2c, double d2) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = 0;
	double l1 = r1c;

	double l2 = r2c;

	double t = d2;
	m->V.size(194);
	m->V[0].set(0, 0, 0);
	for (int i = 1; i < 97; i = i + 4)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), 0.0);
		m->V[i + 1].set(l1*cos(theta + 7.5 * 3.14 / 180), l1*sin(theta + 7.5 * 3.14 / 180), 0.0);
		m->V[i + 2].set(l2*cos(theta + 7.5 * 3.14 / 180), l2*sin(theta + 7.5 * 3.14 / 180), 0.0);
		m->V[i + 3].set(l2*cos(theta + 15 * 3.14 / 180), l2*sin(theta + 15 * 3.14 / 180), 0.0);

		m->F.push().set(0, i, i + 1);
		m->F.push().set(0, i + 1, i);

		m->F.push().set(0, i + 2, i + 3);
		m->F.push().set(0, i + 3, i + 2);


		theta = theta + 15 * 3.14 / 180;
	}

	m->V[97].set(0, 0, 4.1f);
	for (int i = 98; i < 194; i = i + 4)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), t);
		m->V[i + 1].set(l1*cos(theta + 7.5 * 3.14 / 180), l1*sin(theta + 7.5 * 3.14 / 180), t);
		m->V[i + 2].set(l2*cos(theta + 7.5 * 3.14 / 180), l2*sin(theta + 7.5 * 3.14 / 180), t);
		m->V[i + 3].set(l2*cos(theta + 15 * 3.14 / 180), l2*sin(theta + 15 * 3.14 / 180), t);

		m->F.push().set(97, i, i + 1);
		m->F.push().set(97, i + 1, i);

		m->F.push().set(97, i + 2, i + 3);
		m->F.push().set(97, i + 3, i + 2);
		
		theta = theta + 15 * 3.14 / 180;
	}

	for (int i = 0; i < 95; i = i + 1) {
		m->F.push().set(i + 98, i + 1, i + 2);
		m->F.push().set(i + 98, i + 2, i + 99);
	}

	m->F.push().set(0, 1, 98);
	m->F.push().set(0, 98, 97);

}


void MyViewer::make_gear_d(GsModel* m, double r1a, double r2a, double d) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = 0;
	double l2 = r1a;
	double l1 = r2a;
	double t = d;
	m->V.size(74);
	m->V[0].set(0, 0, 0);
	for (int i = 1; i < 37; i = i + 3)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), 0.0);
		m->V[i + 1].set(l2*cos(theta + 15 * 3.14 / 180), l2*sin(theta + 15 * 3.14 / 180), 0.0);
		m->V[i + 2].set(l1*cos(theta + 30 * 3.14 / 180), l1*sin(theta + 30 * 3.14 / 180), 0.0);

		m->F.push().set(0, i, i + 1);
		m->F.push().set(0, i + 1, i);

		m->F.push().set(0, i + 1, i + 2);
		m->F.push().set(0, i + 2, i + 1);

		theta = theta + 30 * 3.14 / 180;
	}
	m->V[37].set(0, 0, 4);
	for (int i = 38; i < 74; i = i + 3)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), t);
		m->V[i + 1].set(l2*cos(theta + 15 * 3.14 / 180), l2*sin(theta + 15 * 3.14 / 180), t);
		m->V[i + 2].set(l1*cos(theta + 30 * 3.14 / 180), l1*sin(theta + 30 * 3.14 / 180), t);

		m->F.push().set(37, i, i + 1);
		m->F.push().set(37, i + 1, i);

		m->F.push().set(37, i + 1, i + 2);
		m->F.push().set(37, i + 2, i + 1);

		theta = theta + 30 * 3.14 / 180;
	}
	
	for (int i = 0; i < 35; i = i + 1) {
		m->F.push().set(i + 38, i + 1, i + 2);
		m->F.push().set(i + 38, i + 2, i + 39);
	}
	//m->F.push().set(0, i, i+1);
	//m->F.push().set(0, i+2, i+3);		
}


void MyViewer::make_gear_e(GsModel* m, double r1b, double r2b, double d1) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = 30;
	double l2 = r1b;
	double l1 = r2b;
	double t = d1;
	m->V.size(38);
	m->V[0].set(0, 0, 0);
	for (int i = 1; i < 19; i = i + 3)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), 0.0);
		m->V[i + 1].set(l2*cos(theta + 30 * 3.14 / 180), l2*sin(theta + 30 * 3.14 / 180), 0.0);
		m->V[i + 2].set(l1*cos(theta + 60 * 3.14 / 180), l1*sin(theta + 60 * 3.14 / 180), 0.0);

		m->F.push().set(0, i, i + 1);
		m->F.push().set(0, i + 1, i);

		m->F.push().set(0, i + 1, i + 2);
		m->F.push().set(0, i + 2, i + 1);

		theta = theta + 60 * 3.14 / 180;
	}
	m->V[19].set(0, 0, 4);
	for (int i = 20; i < 38; i = i + 3)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), t);
		m->V[i + 1].set(l2*cos(theta + 30 * 3.14 / 180), l2*sin(theta + 30 * 3.14 / 180), t);
		m->V[i + 2].set(l1*cos(theta + 60 * 3.14 / 180), l1*sin(theta + 60 * 3.14 / 180), t);

		m->F.push().set(19, i, i + 1);
		m->F.push().set(19, i + 1, i);

		m->F.push().set(19, i + 1, i + 2);
		m->F.push().set(19, i + 2, i + 1);

		theta = theta + 60 * 3.14 / 180;
	}

	for (int i = 0; i < 17; i = i + 1) {
		m->F.push().set(i + 20, i + 1, i + 2);
		m->F.push().set(i + 20, i + 2, i + 21);
	}
	//m->F.push().set(0, i, i+1);
	//m->F.push().set(0, i+2, i+3);		
}


void MyViewer::make_gear_f(GsModel* m, double r1c, double r2c, double d2) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = 15;
	double l2 = r1c;
	double l1 = r2c;
	double t = d2;
	m->V.size(146);
	m->V[0].set(0, 0, 0);
	for (int i = 1; i < 73; i = i + 3)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), 0.0);
		m->V[i + 1].set(l2*cos(theta + 7.5 * 3.14 / 180), l2*sin(theta + 7.5 * 3.14 / 180), 0.0);
		m->V[i + 2].set(l1*cos(theta + 15 * 3.14 / 180), l1*sin(theta + 15 * 3.14 / 180), 0.0);

		m->F.push().set(0, i, i + 1);
		m->F.push().set(0, i + 1, i);

		m->F.push().set(0, i + 1, i + 2);
		m->F.push().set(0, i + 2, i + 1);

		theta = theta + 15 * 3.14 / 180;
	}
	m->V[73].set(0, 0, 5);
	for (int i = 74; i < 146; i = i + 3)
	{

		m->V[i].set(l1*cos(theta), l1*sin(theta), t);
		m->V[i + 1].set(l2*cos(theta + 7.5 * 3.14 / 180), l2*sin(theta + 7.5 * 3.14 / 180), t);
		m->V[i + 2].set(l1*cos(theta + 15 * 3.14 / 180), l1*sin(theta + 15 * 3.14 / 180), t);

		m->F.push().set(73, i, i + 1);
		m->F.push().set(73, i + 1, i);

		m->F.push().set(73, i + 1, i + 2);
		m->F.push().set(73, i + 2, i + 1);

		theta = theta + 15 * 3.14 / 180;
	}

	for (int i = 0; i < 71; i = i + 1) {
		m->F.push().set(i + 74, i + 1, i + 2);
		m->F.push().set(i + 74, i + 2, i + 75);
	}
	//m->F.push().set(0, i, i+1);
	//m->F.push().set(0, i+2, i+3);		
}

void MyViewer::make_gear_g(GsModel* m, double r1f, double r2f, double d) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = -10;
	double l2 = r1f;
	double l1 = r2f;
	double t = d;
	m->V.size(16);
	m->V[0].set(0, 0, 0);
		
	for (int i = 1; i <= 5; i = i + 2)
	{
			m->V[i].set(l1*cos(theta), l1*sin(theta), 0.0);
			m->V[i + 1].set(l2*cos(theta + 15 * 3.14 / 180), l2*sin(theta + 15 * 3.14 / 180), 0.0);
			m->V[i + 2].set(l1*cos(theta + 30 * 3.14 / 180), l1*sin(theta + 30 * 3.14 / 180), 0.0);

			m->F.push().set(0, i, i + 1);
			m->F.push().set(0, i + 1, i);

			m->F.push().set(0, i + 1, i + 2);
			m->F.push().set(0, i + 2, i + 1);

			theta = theta + 30 * 3.14 / 180;
			//if (theta > 90) { break; }
		}
		m->V[8].set(0, 0, 2.2f);
		theta = -10;
		for (int i = 9; i <= 13; i = i + 2)
		{

			m->V[i].set(l1*cos(theta), l1*sin(theta), t);
			m->V[i + 1].set(l2*cos(theta + 15 * 3.14 / 180), l2*sin(theta + 15 * 3.14 / 180), t);
			m->V[i + 2].set(l1*cos(theta + 30 * 3.14 / 180), l1*sin(theta + 30 * 3.14 / 180), t);

			m->F.push().set(8, i, i + 1);
			m->F.push().set(8, i + 1, i);

			m->F.push().set(8, i + 1, i + 2);
			m->F.push().set(8, i + 2, i + 1);

			theta = theta + 30 * 3.14 / 180;
		}
	
	for (int i = 0; i <6; i = i + 1) {
		m->F.push().set(i + 9, i + 1, i + 2);
		m->F.push().set(i + 9, i + 2, i + 10);
	}
	m->F.push().set(0, 1, 9);
	m->F.push().set(0, 9, 8); 
	m->F.push().set(15, 7, 8);
	m->F.push().set(0, 8, 7);
}
void MyViewer::make_gear_h(GsModel* m, double r1d, double r2d, double d3) {
	SnModel* _model;
	_model = new SnModel;
	double l2 = r1d;
	double l1 = r2d;
	double t = d3;
	m->V.size(78);
	double k = 0.2;
	m->V[0].set(0.0, 0.0, 0.0);
	m->V[36].set(0.0, l2 / 2, 0.0);
	m->V[37].set(11.0, 0.0, 0.0);
	m->V[38].set(11.0, l2 / 2, 0.0);
	for (int i = 1; i <= 29; i = i + 7)
	{
		m->V[i].set(1.2 + k, 0.0, 0.0);
		m->V[i + 1].set(2 + k, 0.0, 0.0);
		m->V[i + 2].set(2 + k, l2 / 2, 0.0);
		m->V[i + 3].set(2 + k, l2, 0.0);
		m->V[i + 4].set(1.5 + k, l2 + 0.5, 0.0);
		m->V[i + 5].set(1.2 + k, l2, 0.0);
		m->V[i + 6].set(1.2 + k, l2 / 2, 0.0);
		m->F.push().set(i, i + 1, i + 3);
		m->F.push().set(i, i + 3, i + 5);
		m->F.push().set(i + 5, i + 3, i + 4);

		m->F.push().set(i, i + 3, i + 1);
		m->F.push().set(i, i + 5, i + 3);
		m->F.push().set(i + 5, i + 4, i + 3);
		if (i != 1)
		{
			m->F.push().set(i - 6, i, i + 6);
			m->F.push().set(i - 6, i + 6, i - 5);

			m->F.push().set(i - 6, i + 6, i);
			m->F.push().set(i - 6, i - 5, i + 6);
		}
		k = k + 2;
		m->F.push().set(0, 1, 7);
		m->F.push().set(0, 7, 36);
		m->F.push().set(30, 37, 38);
		m->F.push().set(30, 38, 31);
	}
	// back face
	k = 0.0;
	m->V[39].set(0.0, 0.0, 4.0);
	m->V[40].set(0.0, l2 / 2, 4.0);
	m->V[76].set(11.0, 0.0, 4.0);
	m->V[77].set(11.0, l2 / 2, 4.0);
	m->F.push().set(0, 40, 36);
	m->F.push().set(0, 39, 40);
	m->F.push().set(37, 38, 77);
	m->F.push().set(37, 77, 76);
	for (int i = 41; i <= 69; i = i + 7)
	{
		m->V[i].set(1.2 + k, 0.0, t);
		m->V[i + 1].set(2 + k, 0.0, t);
		m->V[i + 2].set(2 + k, l2 / 2, t);
		m->V[i + 3].set(2 + k, l2, t);
		m->V[i + 4].set(1.5 + k, l2 + 0.5, t);
		m->V[i + 5].set(1.2 + k, l2, t);
		m->V[i + 6].set(1.2 + k, l2 / 2, t);
		m->F.push().set(i, i + 1, i + 3);
		m->F.push().set(i, i + 3, i + 5);
		m->F.push().set(i + 5, i + 3, i + 4);
		if (i != 41)
		{
			m->F.push().set(i - 6, i, i + 6);
			m->F.push().set(i - 6, i + 6, i - 5);
		}
		k = k + 2;
		m->F.push().set(39, 41, 47);
		m->F.push().set(39, 47, 40);
		m->F.push().set(70, 76, 77);
		m->F.push().set(70, 77, 71);
	}

	for (int i = 6; i < 35; i = i + 7) {
		m->F.push().set(i + 41, i, i + 1);
		m->F.push().set(i + 40, i, i + 41);
		m->F.push().set(i, i + 39, i - 1);
		m->F.push().set(i, i + 41, i + 39);
		m->F.push().set(i - 2, i - 1, i + 39);
		m->F.push().set(i - 2, i + 39, i + 38);
		m->F.push().set(i - 3, i - 2, i + 38);
		m->F.push().set(i - 3, i + 38, i + 37);
	}

	m->F.push().set(0, 37, 76);
	m->F.push().set(0, 76, 39);
	m->F.push().set(0, 76, 37);
	m->F.push().set(0, 39, 76);

	/* // previous 5 triangles
	SnModel* _model;
	_model = new SnModel;
	double l2 = r1d;
	double l1 = r2d;
	double t = d;
	m->V.size(22);
	double k = 0;
	m->V[0].set(0.0, 0.0, 0.0);

	for (int i = 1; i < 11; i = i + 2)
	{
		m->V[i].set(k + l1, 0.0, 0.0);
		m->V[i + 1].set(k + l1/2, l2, 0.0);
		if (i != 1) {
			m->F.push().set(i-2, i , i + 1);
			m->F.push().set(i-2, i + 1, i);
		}
		k = k + l1;
	}
	m->F.push().set(0, 1, 2);
	m->F.push().set(0, 2, 1);
	m->V[11].set(0.0, 0.0, 2.0);
	k = 0;
	for (int i = 12; i < 22; i = i + 2)
	{
		m->V[i].set(k + l1, 0.0, t);
		m->V[i + 1].set(k + l1 / 2, l2, t);
		if (i != 12) {
			m->F.push().set(i - 2, i, i + 1);
			m->F.push().set(i - 2, i + 1, i);
		}
		k = k + l1;
	}
	m->F.push().set(9, 20, 11);
	m->F.push().set(9, 11, 0);
	m->F.push().set(0, 11, 2);
	m->F.push().set(13, 2, 11);
	m->F.push().set(9, 10, 20);
	m->F.push().set(21, 20, 10);
	m->F.push().set(1, 12, 2);
	m->F.push().set(13, 2, 12);
	for (int i = 0; i <= 6; i = i + 2) {
		m->F.push().set(i + 1 , i + 12, i + 4);
		m->F.push().set(i +15, i + 4, i + 12);

		m->F.push().set(i + 3, i + 4, i + 14);
		m->F.push().set(i + 15, i + 14, i + 4);
		//gsout << i + 1 << "," << i + 4 << "," << i + 12 << gsnl;
	}
	m->F.push().set(11, 12, 13);
	m->F.push().set(11, 13, 12);*/
}

void MyViewer::make_gear_i(GsModel* m, double r1e, double r2e, double d3) {
	SnModel* _model;
	_model = new SnModel;
	double l2 = r1e;
	double l1 = r2e;
	double t = d3;
	m->V.size(22);
	double k = 0;
	m->V[0].set(0.0, 0.0, 0.0);
	for (int i = 1; i < 11; i = i + 2)
	{
		m->V[i].set(k + l1, 0.0, 0.0);
		m->V[i + 1].set(k + l1 / 2, l2, 0.0);
		if (i != 1) {
			m->F.push().set(i - 2, i, i + 1);
			m->F.push().set(i - 2, i + 1, i);
			}
		k = k + l1;
	}
	m->F.push().set(0, 1, 2);
	m->F.push().set(0, 2, 1);
	
	m->V[11].set(0.0, 0.0, 4.0);
	k = 0;
	for (int i = 12; i < 22; i = i + 2)
	{
		m->V[i].set(k + l1, 0.0, t);
		m->V[i + 1].set(k + l1 / 2, l2, t);
		if (i != 12) {
			m->F.push().set(i - 2, i, i + 1);
			m->F.push().set(i - 2, i + 1, i);
		}
			k = k + l1;
	}
	m->F.push().set(9, 20, 11);
	m->F.push().set(9, 11, 0);
	m->F.push().set(0, 11, 2);
	m->F.push().set(13, 2, 11);
	m->F.push().set(9, 10, 20);
	m->F.push().set(21, 20, 10);
	m->F.push().set(2, 12, 1);
	m->F.push().set(2, 13, 12);
	for (int i = 0; i <= 6; i = i + 2) {
		m->F.push().set(i + 1, i + 12, i + 4);
		m->F.push().set(i + 15, i + 4, i + 12);
		m->F.push().set(i + 3, i + 4, i + 14);
		m->F.push().set(i + 15, i + 14, i + 4);
	}
	m->F.push().set(11, 12, 13);
	m->F.push().set(11, 13, 12);
}

void MyViewer::make_gear_j(GsModel* m, double r1o, double r2o, double d) 
{
	SnModel* _model;
	_model = new SnModel;
	double theta = 60;
	double l1 = r1o;
	double l2 = r2o;
	double t = d;
	m->V.size(84);
	m->V[0].set(0, 0, 0);
	for (int i = 1; i < 42; i = i + 7)
	{
		m->V[i].set(l1*cos(theta), l1*sin(theta), 0.0);
		m->V[i + 1].set((l1 / 1.2)*cos(theta), (l1 / 1.2)*sin(theta), 0.0);
		m->V[i + 2].set(l1*cos(theta + 30 * 3.14 / 180), l1*sin(theta + 30 * 3.14 / 180), 0.0);
		m->V[i + 3].set((l1 / 1.2)*cos(theta + 30 * 3.14 / 180), (l1 / 1.2)*sin(theta + 30 * 3.14 / 180), 0.0);

		m->V[i + 4].set(l2*cos(theta + 30 * 3.14 / 180), l2*sin(theta + 30 * 3.14 / 180), 0.0);
		m->V[i + 5].set(l2*cos(theta + 60 * 3.14 / 180), l2*sin(theta + 60 * 3.14 / 180), 0.0);
		m->V[i + 6].set((l1 / 1.2)*cos(theta + 60 * 3.14 / 180), (l1 / 1.2)*sin(theta + 60 * 3.14 / 180), 0.0);

		m->F.push().set(i + 1, i, i + 3);
		m->F.push().set(i, i + 2, i + 3);
		m->F.push().set(i + 3, i + 4, i + 6);
		m->F.push().set(i + 4, i + 5, i + 6);

		m->F.push().set(i + 1, i+3, i);
		m->F.push().set(i, i + 3, i + 2);
		m->F.push().set(i + 3, i + 6, i + 4);
		m->F.push().set(i + 4, i + 6, i + 5);

		theta = theta + 60 * 3.14 / 180;
	}
	//m->V[42].set(0, 0, 2);
	for (int i = 43; i < 84; i = i + 7)
	{
		m->V[i].set(l1*cos(theta), l1*sin(theta), t);
		m->V[i + 1].set((l1 / 1.2)*cos(theta), (l1 / 1.2)*sin(theta), t);
		m->V[i + 2].set(l1*cos(theta + 30 * 3.14 / 180), l1*sin(theta + 30 * 3.14 / 180), t);
		m->V[i + 3].set((l1 / 1.2)*cos(theta + 30 * 3.14 / 180), (l1 / 1.2)*sin(theta + 30 * 3.14 / 180), t);

		m->V[i + 4].set(l2*cos(theta + 30 * 3.14 / 180), l2*sin(theta + 30 * 3.14 / 180), t);
		m->V[i + 5].set(l2*cos(theta + 60 * 3.14 / 180), l2*sin(theta + 60 * 3.14 / 180), t);
		m->V[i + 6].set((l1 / 1.2)*cos(theta + 60 * 3.14 / 180), (l1 / 1.2)*sin(theta + 60 * 3.14 / 180), t);

		m->F.push().set(i + 1, i, i + 3);
		m->F.push().set(i, i + 2, i + 3);
		m->F.push().set(i + 3, i + 4, i + 6);
		m->F.push().set(i + 4, i + 5, i + 6);

		m->F.push().set(i + 1, i+3, i);
		m->F.push().set(i, i + 3, i + 2);
		m->F.push().set(i + 3, i + 6, i + 4);
		m->F.push().set(i + 4, i + 6, i + 5);

		theta = theta + 60 * 3.14 / 180;
	}

	/*for (int i = 1; i <= 40; i = i + 1) {
		m->F.push().set(i + 41, i, i + 3);
		m->F.push().set(i + 41, i + 3, i + 42);
	}*/
}

void MyViewer::build_scene()

{
	//double t1 = gs_time();
	//gsout << t1 << gsnl;

	// Gear Example
	SnModel* _model;
	// gear a,d
	double r1a = 5.3;
	double r2a = 3.3;
	double d = 2.0;
	
	//gear  b,e
	double r1b = 2.8;
	double r2b = 1.5;
	double d1 = 3.0;
	
	//gear c,f
	double r1c = 9.0;
	double r2c = 6.5;
	double d2 = 4.0;

	//h (left linear)
	double r1d = 2.0;
	double r2d = 1.5;

	//i (right linear)
	double r1e = 2.5;
	double r2e = 2.5;
	double d3 = 4.0;

	// gear j (biggest)
	double r1o = 28.0;
	double r2o = 25.0;

	//for gear g
	double r1f = 4.7;
	double r2f = 3.0;

	
	// GEAR 1
	_model = new SnModel;
	_model->color(GsColor::red);
	make_gear_a(_model->model(), r1a, r2a, d);
	// put in scene graph:
	SnGroup *g1 = new SnGroup;
	g1->separator(true);
	g1->add(_t1 = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g1->add(_model); // gear 1
	_t1->get().translation(0, 0.0f, 0.8f);
	//_t1->get().transpose();
	
	// GEAR 2
	_model = new SnModel;
	_model->color(GsColor::blue);
	make_gear_b(_model->model(), r1b, r2b, d1);
	// put in scene graph:
	SnGroup *g2 = new SnGroup;
	g2->separator(true);
	g2->add(_t2 = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g2->add(_model); // gear 1
	_t2->get().translation(-6.7f, 0.0f, 2);

	// GEAR 3
	_model = new SnModel;
	_model->color(GsColor::green);
	make_gear_c(_model->model(), r1c, r2c, d2);
	// put in scene graph:
	SnGroup *g3 = new SnGroup;
	g3->separator(true);
	g3->add(_t3 = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g3->add(_model); // gear 1
	_t3->get().translation(0.0f, 12.5f, 0);
	
	// GEAR 4
	_model = new SnModel;
	_model->color(GsColor::red);
	make_gear_d(_model->model(), r1a, r2a, d);
	// put in scene graph:
	SnGroup *g4 = new SnGroup;
	g4->separator(true);
	g4->add(_t4 = new SnTransform);
	g4->add(_model); // gear 1
	_t4->get().translation(18.0f, 0.0f, 0.8f);
	
	// GEAR 5
	_model = new SnModel;
	_model->color(GsColor::blue);
	make_gear_e(_model->model(),r1b, r2b, d1);
	// put in scene graph:
	SnGroup *g5 = new SnGroup;
	g5->separator(true);
	g5->add(_t5 = new SnTransform);
	g5->add(_model); // gear 1
	_t5->get().translation(24.2f, 0.2f, 2);

	// GEAR 6
	_model = new SnModel;
	_model->color(GsColor::green);
	make_gear_f(_model->model(),r1c, r2c, d2);
	// put in scene graph:
	SnGroup *g6 = new SnGroup;
	g6->separator(true);
	g6->add(_t6 = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g6->add(_model); // gear 1
	_t6->get().translation(18.3f, 12.8f, 0);
	
	// GEAR 7
	_model = new SnModel;
	_model->color(GsColor::darkgreen);
	make_gear_g(_model->model(), r1f, r2f, d);
	// put in scene graph:
	SnGroup *g7 = new SnGroup;
	g7->separator(true);
	g7->add(_t7 = new SnTransform);
	g7->add(_model);
	_t7->get().translation(9.0f, -0.2f, 1.5f);

	// GEAR 8
	_model = new SnModel;
	_model->color(GsColor::blue);
	make_gear_h(_model->model(), r1d, r2d, d3);
	// put in scene graph:
	SnGroup *g8 = new SnGroup;
	g8->separator(true);
	g8->add(_t8 = new SnTransform);
	g8->add(_model);
	_t8->get().translation(-8.3f, -6.7f, 0.0f);

	// GEAR 9
	_model = new SnModel;
	_model->color(GsColor::blue);
	make_gear_i(_model->model(), r1e, r2e, d3);
	// put in scene graph:
	SnGroup *g9 = new SnGroup;
	g9->separator(true);
	g9->add(_t9 = new SnTransform);
	g9->add(_model);
	_t9->get().translation(9.2f, -6.2f, 0.0f);
	
	/*
		// GEAR 10
	_model = new SnModel;
	_model->color(GsColor::red);
	make_gear_j(_model->model(), r1o, r2o, d);
	// put in scene graph:
	SnGroup *g10 = new SnGroup;
	g10->separator(true);
	g10->add(_t10 = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g10->add(_model); // gear 1
	_t10->get().translation(8.9f, 7.0f, 0);
	*/

	rootg()->add(g1);
	rootg()->add(g2);
	rootg()->add(g3);
	rootg()->add(g4);
	rootg()->add(g5);
	rootg()->add(g6);
	rootg()->add(g7);
	rootg()->add(g8);
	rootg()->add(g9);

	//rootg()->add(g10);
	//original 
	/*
	SnPrimitive* p;
	
	p = new SnPrimitive(GsPrimitive::Box, 1, 3, 1);
	p->prim().material.diffuse = GsColor::yellow;
	add_model(p, GsVec(0, 0, 0));

	p = new SnPrimitive(GsPrimitive::Sphere, 2);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(-4, 0, 0));

	p = new SnPrimitive(GsPrimitive::Cylinder, 1.0, 1.0, 1.5);
	p->prim().material.diffuse = GsColor::blue;
	add_model(p, GsVec(4, 0, 0));

	p = new SnPrimitive(GsPrimitive::Capsule, 1, 1, 3);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(8, 0, 0));

	p = new SnPrimitive(GsPrimitive::Ellipsoid, 2.0, 0.5);
	p->prim().material.diffuse = GsColor::green;
	add_model(p, GsVec(-8, 0, 0));
	*/
	
	//double t2 = gs_time();
	//gsout << t2 << gsnl;
	//gsout << (t2 - t1) << gsnl;

	}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation()
{
	//double t1 = gs_time();
	//gsout << t1 << gsnl;
	
	if (_animating) return; // avoid recursive calls
	_animating = true;

	int ind = gs_random(0, rootg()->size() - 1); // pick one child
	SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	GsMat m = manip->mat();

	_curang1 = 0;
	_curang2 = 0;
	_curang3 = 0;
	_curang4 = 0;
	_curang5 = 0;
	_curang6 = 0;
	_curang7 = 0;
	//_curang8 = 0;
	//_curang9 = 0;
	//_curang10 = 0;

	double frdt = 1.0 / 30.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double t = 0, lt = 0, t0 = gs_time();
	do // run for a while:
	{
		while (t - lt<frdt) t = gs_time() - t0; // wait until it is time for next frame
		lt = t;

		//gear 1
		GsVec tr1;
		_t1->get().getrans(tr1);
		_t1->get().rotz(GS_TODEG(_curang1));
		_t1->get().setrans(tr1);
		_curang1 += 0.0005f;

		//gear 2
		GsVec tr2;
		_t2->get().getrans(tr2);
		_t2->get().rotz(GS_TODEG(_curang2));
		_t2->get().setrans(tr2);
		_curang2 -= 0.0010f;

		//gear 3
		GsVec tr3;
		_t3->get().getrans(tr3);
		_t3->get().rotz(GS_TODEG(_curang3));
		_t3->get().setrans(tr3);
		_curang3 -= 0.00025f;
		
		//gear 4
		GsVec tr4;
		_t4->get().getrans(tr4);
		_t4->get().rotz(GS_TODEG(_curang4));
		_t4->get().setrans(tr4);
		_curang4 += 0.0005f;

		//gear 5
		GsVec tr5;
		_t5->get().getrans(tr5);
		_t5->get().rotz(GS_TODEG(_curang5));
		_t5->get().setrans(tr5);
		_curang5 -= 0.0010f;

		//gear 6
		GsVec tr6;
		_t6->get().getrans(tr6);
		_t6->get().rotz(GS_TODEG(_curang6));
		_t6->get().setrans(tr6);
		_curang6 -= 0.00025f;
		
		//gear 7
		GsVec tr7;
		_t7->get().getrans(tr7);
		_t7->get().rotz(GS_TODEG(_curang7));
		_t7->get().setrans(tr7);
		_curang7 -= 0.0005f;

		//gear 8
		GsVec tr8;
		_t8->get().getrans(tr8);
		if (tr8.x < -2.0f)
		tr8 += GsVec(0.115f, 0.0f, 0.0f);
		if (tr8.x > -2.0f) {
			//tr8 = GsVec(-9.2f, -6.3f, 0.0f);
			tr8 = GsVec(-8.1f, -6.7f, 0.0f);
		}
		_t8->get().setrans(tr8);
		//_curang8 += 0.0005f;
				
		//gear 9
		GsVec tr9;
		_t9->get().getrans(tr9);
		//20.4
		if (tr9.x < 16.7f)
			tr9 += GsVec(0.134f, 0.0f, 0.0f);
		if (tr9.x > 16.7f) {
			//tr9 = GsVec(10.3f, -7.0f, 0);
			tr9 = GsVec(9.2f, -6.2f, 0);
		}
		_t9->get().setrans(tr9);
		//_curang9 += 0.0005f;

		//gear 10 (outer most) // Comment before submitting..
		/*
		GsVec tr10;
		_t10->get().getrans(tr10);
		_t10->get().rotz(GS_TODEG(_curang10));
		_t10->get().setrans(tr10);
		_curang10 += 0.0005f;
		*/
		render(); // notify it needs redraw
		ws_check(); // redraw now
	} while (true);// m.e24 > 0 );
	_animating = false;
	
	//original
	/*
	if (_animating) return; // avoid recursive calls
	_animating = true;

	int ind = gs_random(0, rootg()->size() - 1); // pick one child
	SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	GsMat m = manip->mat();

	double frdt = 1.0 / 30.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double t = 0, lt = 0, t0 = gs_time();
	do // run for a while:
	{
		while (t - lt<frdt) t = gs_time() - t0; // wait until it is time for next frame
		double yinc = (t - lt)*v;
		if (t>2) yinc = -yinc; // after 2 secs: go down
		lt = t;
		m.e24 += (float)yinc;
		if (m.e24<0) m.e24 = 0; // make sure it does not go below 0
		manip->initial_mat(m);
		render(); // notify it needs redraw
		ws_check(); // redraw now
	} while (m.e24>0);
	_animating = false;
	*/
	//double t2 = gs_time();
	//gsout << t2 << gsnl;
	//gsout << (t2 - t1) << gsnl;

}

void MyViewer::show_normals(bool b)
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.
	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for (int k = 0; k<r->size(); k++)
	{
		SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if (!b) { l->visible(false); continue; }
		l->visible(true);
		if (!l->empty()) continue; // build only once
		l->init();
		if (s->instance_name() == SnPrimitive::class_name)
		{
			GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face(fn);
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for (int i = 0; i<m.F.size(); i++)
			{
				const GsVec& a = m.V[m.F[i].a]; l->push(a, a + (*n++)*f);
				const GsVec& b = m.V[m.F[i].b]; l->push(b, b + (*n++)*f);
				const GsVec& c = m.V[m.F[i].c]; l->push(c, c + (*n++)*f);
			}
		}
	}
}

int MyViewer::handle_keyboard(const GsEvent &e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;

	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
	case 'n': { bool b = !_nbut->value(); _nbut->value(b); show_normals(b); return 1; }
	default: gsout << "Key pressed: " << e.key << gsnl;
	}

	return 0;
}

int MyViewer::uievent(int e)
{
	switch (e)
	{
	case EvNormals: show_normals(_nbut->value()); return 1;
	case EvAnimate: run_animation(); return 1;
	case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
