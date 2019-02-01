# include "my_viewer.h"
# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
#include<sig\gs_math.h>
# include <vector>
# include <iostream>
#define PI 3.141592654f
using namespace std;
# include <sigogl/ws_run.h>
#define pie 3.14;

MyViewer::MyViewer(int x, int y, int w, int h, const char* l) : WsViewer(x, y, w, h, l)
{
	//double t1 = gs_time();
	//gsout << t1 << gsnl;

	_nbut = 0;
	_animating = false;
	manip = new SnManipulator;
	manip1 = new SnManipulator;
	manip2 = new SnManipulator;
	manip3 = new SnManipulator;
	manip4 = new SnManipulator;
	manip5 = new SnManipulator;
	manip6 = new SnManipulator;
	manip7 = new SnManipulator;
	manip8 = new SnManipulator;
	manip9 = new SnManipulator;

	add_ui();
	build_scene();

	//double t2 = gs_time();
	//gsout << t2 << gsnl;
	//gsout << (t2 - t1) << gsnl;
}

void MyViewer::add_ui()
{
	UiPanel *p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel("", UiPanel::HorizLeft);
	p->add(new UiButton("Transform", EvAnimate));
	p->add(new UiButton("Exit", EvExit)); p->top()->separate();
}

void MyViewer::add_model(SnShape* s, GsVec point, SnManipulator *manip)
{
	//SnManipulator* manip;
	SnGroup* g;
	SnLines* li;
	
	//manip = new SnManipulator;
	g = new SnGroup;
	li = new SnLines;
	GsMat m;
	m.translation(point);
	manip->initial_mat(m);

	li->color(GsColor::orange);
	g->add(s);
	g->add(li);
	manip->child(g);

	rootg()->add(manip);
}

//static model
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

void MyViewer::FT(SnLines* l, GsPnt firstpoint, GsPnt secondpoint, float angle, int s) {

	if (s >= 18) {
		return;
	}

	l->P.push().set(firstpoint);
	l->P.push().set(secondpoint);

	GsPnt newfirstpoint = secondpoint;

	GsPnt newlastpointleft(0.0f, 0.5f + 0.5f*s, 0.0f);
	GsPnt newlastpointright(0.0f, 0.5f + 0.5f*s, 0.0f);

	newlastpointleft.rotz(angle - PI / 150);
	FT(l, newfirstpoint, newlastpointleft, angle - PI / 150, s + 1);

	newlastpointright.rotz(angle + PI / 150);
	FT(l, newfirstpoint, newlastpointright, angle + PI / 150, s + 1);
}
void MyViewer::FT2(SnLines* l, GsPnt firstpoint, GsPnt secondpoint, float angle, int s) {

	if (s >= 18) {
		return;
	}
	l->P.push().set(firstpoint);
	l->P.push().set(secondpoint);
	l->line_width(2);

	GsPnt newfirstpoint = secondpoint;

	GsPnt newlastpointleft(0.0f, 0.5f + 0.5f*s, 0.0f);
	GsPnt newlastpointright(0.0f, 0.5f + 0.5f*s, 0.0f);

	newlastpointleft.rotz(angle - PI / 30);
	FT2(l, newfirstpoint, newlastpointleft, angle - PI / 30, s + 4);

	newlastpointright.rotz(angle + PI / 30);
	FT2(l, newfirstpoint, newlastpointright, angle + PI / 30, s + 4);
}

void MyViewer::FT3(SnLines* l, GsPnt firstpoint, GsPnt secondpoint, float angle, int s) {

	if (s >= 18) {
		return;
	}
	l->P.push().set(firstpoint);
	l->P.push().set(secondpoint);
	//l->line_width(2);

	GsPnt newfirstpoint = secondpoint;

	GsPnt newlastpointleft(0.0f, 0.3f + 0.3f*s, 0.0f);
	GsPnt newlastpointright(0.0f, 0.3f + 0.3f*s, 0.0f);

	newlastpointleft.rotz(angle - PI / 90);
	FT3(l, newfirstpoint, newlastpointleft, angle - PI / 90, s + 2);

	newlastpointright.rotz(angle + PI / 90);
	FT3(l, newfirstpoint, newlastpointright, angle + PI / 90, s + 2);
}

void MyViewer::Rear(GsModel* m, double r1c, double r2c, double d2) {
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

	m->V[97].set(0, 0, 1.0f);
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
void MyViewer::RearR(GsModel* m, double r1cR, double r2cR, double d2R) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = 0;
	double l1 = r1cR;

	double l2 = r2cR;

	double t = d2R;
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

	m->V[97].set(0, 0, 1.0f);
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
	//shadow(m);

}
void MyViewer::Front(GsModel* m, double r1c, double r2c, double d2) {
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

	m->V[97].set(0, 0, 1);
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
void MyViewer::FrontR(GsModel* m, double r1cR, double r2cR, double d2R) {
	
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	double theta = 0;
	double l1 = r1cR;

	double l2 = r2cR;

	double t = d2R;
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

	m->V[97].set(0, 0, 1.0f);
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

void MyViewer::Ysign(GsModel* m) {
	SnModel* _model;
	_model = new SnModel;
	//m = _model->model();
	m->V.size(7);
	m->V[0].set(8.5f, 2, 10);
	m->V[1].set(9.5f, 2, 10);
	m->V[2].set(9.5f, 10, 10);
	m->V[3].set(8.5f, 10, 10);
	m->V[4].set(9, 9, 10);
	m->V[5].set(12, 13, 10);
	m->V[6].set(6, 13, 10);

	m->F.push().set(0, 1, 2);
	m->F.push().set(0, 2, 3);
	m->F.push().set(4, 5, 6);
	shadow(m);	
	//gsout<< "total triangles on base:" << c;
	//double t1 = gs_time();
	//_model->color(GsColor::darkred);
	//rootg()->add(_model);
	//double t2 = gs_time();
	//double t = t2 - t1;
	//gsout << "shadowing time" << t;

}
void MyViewer::Ysignal(GsModel* m) {
	SnLines *l;
	l = new SnLines;
	l->color(GsColor::white);
	l->line_width(4);
	GsVec V[4];

		V[0].set(39, -3, 20.4f);
		V[1].set(39, -1, 20.4f);
		V[2].set(40.5f, 1, 20.4f);
		V[3].set(37.5f, 1, 20.4f);

		l->push(V[0], V[1]);
		l->push(V[1], V[2]);
		l->push(V[1], V[3]);
	
	rootg()->add(l);
}
void MyViewer::shadow(GsModel * m) {
	SnModel* _model;
	_model = new SnModel;
	GsModel* newM = _model->model();
	int index = 0;
	int dis = 193;
	int c = 0;
	newM->V.size(7);
	GsVec l(40.0f, 500.0f, 180.0f);
	for (int it = 0; it <= m->V.size() - 1; it++) {
		float cordinatex = (l(0) - l(2)*(l(0) - m->V[it].x) / (l(2) - m->V[it].z))+30.0f;
		float cordinatey = (l(1) - l(2)*(l(1) - m->V[it].y) / (l(2) - m->V[it].z))+15.0f;
		newM->V[it].set(cordinatex, cordinatey, 0.0f);
		index++;
	}
		newM->F.push().set(0, 1, 2);
		newM->F.push().set(0, 2, 3);
		newM->F.push().set(4, 5, 6);

	//gsout<< "total triangles on base:" << c;
	//double t1 = gs_time();
	_model->color(GsColor::darkred);
	rootg()->add(_model);
	//double t2 = gs_time();
	//double t = t2 - t1;
	//gsout << "shadowing time" << t;
}
void MyViewer::build_scene()

{
	//double t1 = gs_time();
	//gsout << t1 << gsnl;

	//   Example
	SnModel* _model;
	
	//  c,f
	//double r1c = 2.5;
	//double r2c = 1.0;
	double r1c = 2.2;
	double r2c = 0.7;

	double r1cR = 2.5;
	double r2cR = 2.5;

	double d2 = 1.0;
	//double d2 = 0.3;
	double d2R = 0.3;


	SnLines* nl = new SnLines;
	nl->color(GsColor::green);
	//nl->line_width(0.5f);

	SnLines* nl1 = new SnLines;
	nl1->color(GsColor::darkgreen);
	//nl->line_width(0.5f);

	SnLines* nl2 = new SnLines;
	nl2->color(GsColor::green);
	//nl->line_width(0.5f);
	

	SnGroup* gF = new SnGroup();
	gF->separator(true);
	GsPnt sA(0.0f, 0.0f, 0.0f);
	GsPnt sB(0.0f, 0.5f, 0.0f);
	FT(nl, sA, sB, 0, 1);
		_tF = new SnTransform();
	gF->add(_tF);
	gF->add(nl);
	_tF->get().translation(43.0f, -11.0f, 20.0f);
	rootg()->add(gF);

	SnGroup* gF2 = new SnGroup();
	gF2->separator(true);
	FT(nl, sA, sB, 0, 1);
	_tF2 = new SnTransform();
	gF2->add(_tF2);
	gF2->add(nl);
	_tF2->get().translation(-55.0f, -11.0f, 20.0f);
	rootg()->add(gF2);

	SnGroup* gF3 = new SnGroup();
	gF3->separator(true);
	GsPnt sA2(0.0f, 0.0f, 0.0f);
	GsPnt sB2(0.0f, 0.5f, 0.0f);
	FT2(nl1, sA2, sB2, 0, 1);
	_tF3 = new SnTransform();
	gF3->add(_tF3);
	gF3->add(nl1);
	_tF3->get().translation(-39.0f, -11.0f, 20.0f);
	rootg()->add(gF3);

	SnGroup* gF4 = new SnGroup();
	gF4->separator(true);
	FT2(nl1, sA2, sB2, 0, 1);
	_tF4 = new SnTransform();
	gF4->add(_tF4);
	gF4->add(nl1);
	_tF4->get().translation(25.0f, -11.0f, 20.0f);
	rootg()->add(gF4);
	
	SnGroup* gF5 = new SnGroup();
	gF5->separator(true);
	FT2(nl1, sA2, sB2, 0, 1);
	_tF5 = new SnTransform();
	gF5->add(_tF5);
	gF5->add(nl1);
	_tF5->get().translation(-7.0f, -11.0f, 20.0f);
	rootg()->add(gF5);

	SnGroup* gF6 = new SnGroup();
	gF6->separator(true);
	GsPnt sA3(0.0f, 0.0f, 0.0f);
	GsPnt sB3(0.0f, 0.3f, 0.0f);
	FT3(nl2, sA2, sB2, 0, 1);
	_tF6 = new SnTransform();
	gF6->add(_tF6);
	gF6->add(nl2);
	_tF6->get().translation(-23.0f, -11.0f, 20.0f);
	rootg()->add(gF6);

	SnGroup* gF7 = new SnGroup();
	gF7->separator(true);
	FT3(nl2, sA2, sB2, 0, 1);
	_tF7 = new SnTransform();
	gF7->add(_tF7);
	gF7->add(nl2);
	_tF7->get().translation(9.0f, -11.0f, 20.0f);
	rootg()->add(gF7);
		

	//wheel rear
	_model = new SnModel;
	_model->color(GsColor::green);
	Rear(_model->model(), r1c, r2c, d2);
	// put in scene graph:
	SnGroup *g3 = new SnGroup;
	g3->separator(true);
	g3->add(_t3 = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g3->add(_model); 
	_t3->get().translation(-45.0f, -8.5f, 10.0f);
	//_t3->get().rotx(5.0f);
	
	// wheel rear back
	_model = new SnModel;
	_model->color(GsColor::red);
	RearR(_model->model(), r1cR, r2cR, d2R);
	// put in scene graph:
	SnGroup *g3R = new SnGroup;
	g3R->separator(true);
	g3R->add(_t3R = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g3R->add(_model); //   1
	_t3R->get().translation(-45.0f, -8.5f, 10.0f);

	/*
	//   3 Rear Shadow
	_model = new SnModel;
	_model->color(GsColor::red);
	shadow(_model->model());
	// put in scene graph:
	SnGroup *g3RS = new SnGroup;
	g3RS->separator(true);
	g3RS->add(_t3RS = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g3RS->add(_model); //   1
	_t3RS->get().translation(-45.0f, -18.5f, 2.5f);
	//_t3RS->get().translation(0,0,0.5f);
	*/

	// wheel front
	_model = new SnModel;
	_model->color(GsColor::green);
	Front(_model->model(), r1c, r2c, d2);
	// put in scene graph:
	SnGroup *g4 = new SnGroup;
	g4->separator(true);
	g4->add(_t4 = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g4->add(_model); //   1
	_t4->get().translation(-35.0f, -8.5f, 10.0f);
	
	// wheel front Rear
	_model = new SnModel;
	_model->color(GsColor::red);
	FrontR(_model->model(), r1cR, r2cR, d2R);
	// put in scene graph:
	SnGroup *g4R = new SnGroup;
	g4R->separator(true);
	g4R->add(_t4R = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	g4R->add(_model); //   1
	_t4R->get().translation(-35.0f, -8.5f, 10.0f);


	//   Sign Y
	_model = new SnModel;
	_model->color(GsColor::red);
	Ysign(_model->model());
	// put in scene graph:
	SnGroup *gy = new SnGroup;
	gy->separator(true);
	gy->add(_ty = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	gy->add(_model); //   1
	_ty->get().translation(30.0f, -11.0f, 10.0f);

	//   Sign shadow Y
	_model = new SnModel;
	_model->color(GsColor::red);
	Ysignal(_model->model());
	// put in scene graph:
	SnGroup *gyS = new SnGroup;
	gyS->separator(true);
	gyS->add(_tyS = new SnTransform); // _t1 is a member variable, to save a direct access pointer to the transformation
	gyS->add(_model); //   1
	_tyS->get().translation(30.0f, -1.0f, 22.0f);
	
	SnPrimitive *p;
	
	//Big Box
	p = new SnPrimitive(GsPrimitive::Box, 58, 1, 20.0f);
	p->prim().material.diffuse = GsColor::darkgray;
	add_model(p, GsVec(-6, -12, 1.0f));

	//Big Box up
	p = new SnPrimitive(GsPrimitive::Box, 58, 24, -0.1f);
	p->prim().material.diffuse = GsColor::lightblue;
	add_model(p, GsVec(-6, 16, -0.2f));
	
	
	//building 1
	p = new SnPrimitive(GsPrimitive::Box, 4.5, 15, 1.5);
	p->prim().material.diffuse = GsColor::black;
	add_model(p, GsVec(-24, 5, 0));

	
	//roof B1
	p = new SnPrimitive(GsPrimitive::Box, 2.5, 1.5, 1.5);
	p->prim().material.diffuse = GsColor::black;
	add_model(p, GsVec(-24, 21.5f, 0));

	//antenna B1
	p = new SnPrimitive(GsPrimitive::Box, 0.2f, 2.0, 1.5);
	p->prim().material.diffuse = GsColor::black;
	add_model(p, GsVec(-24, 25, 0));
	
	//window B1
	p = new SnPrimitive(GsPrimitive::Box, 0.8f, 1.4f, 0.5);
	p->prim().material.diffuse = GsColor::gray;
	for (int j = 18; j >= 0; j = j - 6) {
		for (int i = 21; i < 28; i = i + 2) {
			add_model(p, GsVec(-i, j, 1.3f));
		}
	}
	for (int i = 21; i<28; i = i + 2) {
		add_model(p, GsVec(-i, -6, 1.3f));
	}

	//building 2
	p = new SnPrimitive(GsPrimitive::Box, 3.5, 10, 2.5);
	p->prim().material.diffuse = GsColor::blue;
	add_model(p, GsVec(-17.1f, 0.2f, 1));

	//window B2
	p = new SnPrimitive(GsPrimitive::Box, 2.4f, 0.4f, 0.5);
	p->prim().material.diffuse = GsColor::darkgray;
	for (int i = 8; i > -9; i = i - 2) {
		add_model(p, GsVec(-17, i, 3.3f));
	}
	//roof B2
	p = new SnPrimitive(GsPrimitive::Box, 1.5, 0.5f, 2.5);
	p->prim().material.diffuse = GsColor::blue;
	add_model(p, GsVec(-17.1f, 10.5f, 1));

	//antenna B2
	p = new SnPrimitive(GsPrimitive::Box, 0.4f, 1, 1.5);
	p->prim().material.diffuse = GsColor::blue;
	add_model(p, GsVec(-17.1f, 11, 1));


	//building 3
	p = new SnPrimitive(GsPrimitive::Box, 5.5, 18, 2.5);
	p->prim().material.diffuse = GsColor::brown;
	add_model(p, GsVec(-8.6f, 8, -1));
	
	//window B3
	p = new SnPrimitive(GsPrimitive::Box, 0.4f, 16, 0.2f);
	p->prim().material.diffuse = GsColor::red;
	for (int i = 5; i < 14; i = i + 2) {
		add_model(p, GsVec(-(i)+0.4f, 8, 2));
	}

	//Flat window B3
	p = new SnPrimitive(GsPrimitive::Box, 4.9f, 0.4f, 0.2f);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(-8.6f, 25, 2));

	//Antenna B3
	p = new SnPrimitive(GsPrimitive::Box, 0.2f, 3, 2.5);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(-8.6f, 27, -1));


	//building 4
	p = new SnPrimitive(GsPrimitive::Box, 6, 12, 4);
	p->prim().material.diffuse = GsColor::lightblue;
	add_model(p, GsVec(2, 2, 0));

	//Roof sphere on B4
	p = new SnPrimitive(GsPrimitive::Sphere, 4);
	p->prim().material.diffuse = GsColor::lightblue;
	add_model(p, GsVec(2, 15.3f, 0));
	
	
	//window B4
	p = new SnPrimitive(GsPrimitive::Box, 1, 1, 0.5);
	p->prim().material.diffuse = GsColor::darkgray;
	for (int j = -7; j < 14; j = j + 3) {
		for (int i = 4; i < 11; i = i + 3) {
			add_model(p, GsVec(i-5, j, 4.1f));
			//add_model(p, GsVec(7, 10, 1.3f));
			//add_model(p, GsVec(10, 10, 1.3f));
		}
	}


	
	//building 5
	p = new SnPrimitive(GsPrimitive::Box, 5.5, 14, 2.5);
	p->prim().material.diffuse = GsColor::darkgreen;
	add_model(p, GsVec(12.5f, 4.2f, 1));
	
	//window B5
	p = new SnPrimitive(GsPrimitive::Box, 4.4f, 0.4f, 0.5);
	p->prim().material.diffuse = GsColor::darkgray;
	for (int i = 18; i >-8; i = i - 2) {
		add_model(p, GsVec(12.5f, i-2, 3.3f));
	}
	//antenna B5
	p = new SnPrimitive(GsPrimitive::Cylinder, 2.5f, 1.5f, 3.0);
	p->prim().material.diffuse = GsColor::green;
	add_model(p, GsVec(12.5f, 19, 1.0f));

	//building 6
	p = new SnPrimitive(GsPrimitive::Box, 6, 17, 2.5);
	p->prim().material.diffuse = GsColor::orange;
	add_model(p, GsVec(-34, 7, 0));

	//antenna B6
	p = new SnPrimitive(GsPrimitive::Box, 0.2f, 1.0, 1.5);
	p->prim().material.diffuse = GsColor::gray;
	add_model(p, GsVec(-38.5f, 25.0f, 1));
	add_model(p, GsVec(-36.0f, 25.0f, 1));
	add_model(p, GsVec(-34.0f, 25.0f, 1));
	add_model(p, GsVec(-32.0f, 25.0f, 1));
	add_model(p, GsVec(-29.5f, 25.0f, 1));

	//window B6
	p = new SnPrimitive(GsPrimitive::Box, 1, 1, 0.5);
	p->prim().material.diffuse = GsColor::darkgray;
	for (int j = -13; j < 20; j = j + 3) {
		for (int i = -32; i < -25; i = i + 3) {
			add_model(p, GsVec(i - 5, j+5, 2.5f));
			//add_model(p, GsVec(7, 10, 1.3f));
			//add_model(p, GsVec(10, 10, 1.3f));
		}
	}

	//building 7
	p = new SnPrimitive(GsPrimitive::Box, 5.5, 13, 2.5);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(-45, 3, -1));

	//window B7
	p = new SnPrimitive(GsPrimitive::Box, 0.4f, 12, 0.2f);
	p->prim().material.diffuse = GsColor::cyan;
	for (int i = -31; i < -22; i = i + 2) {
		add_model(p, GsVec((i)-17.8f, 3, 2.5f));
	}


	//antenna B7
	p = new SnPrimitive(GsPrimitive::Box, 4, 0.7f, 1.5);
	p->prim().material.diffuse = GsColor::cyan;
	add_model(p, GsVec(-44.7f, 16.5f, 2));
	p = new SnPrimitive(GsPrimitive::Box, 3, 0.7f, 1.5);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(-44.7f, 18.0f, 2));
	p = new SnPrimitive(GsPrimitive::Box, 2.5, 0.7f, 1.5);
	p->prim().material.diffuse = GsColor::cyan;
	add_model(p, GsVec(-44.7f, 19.5f, 2));
	p = new SnPrimitive(GsPrimitive::Box, 2, 0.7f, 1.5);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(-44.7f, 21.0f, 2));
	p = new SnPrimitive(GsPrimitive::Box, 1, 0.7f, 1.5);
	p->prim().material.diffuse = GsColor::cyan;
	add_model(p, GsVec(-44.7f, 22.5f, 2));

	//building 8
	p = new SnPrimitive(GsPrimitive::Box, 5, 13, 4);
	p->prim().material.diffuse = GsColor::darkblue;
	add_model(p, GsVec(22.3f, 3, 0));
	
	//antenna B8
	p = new SnPrimitive(GsPrimitive::Box, 0.2f, 2.0, 1.5);
	p->prim().material.diffuse = GsColor::darkred;
	add_model(p, GsVec(18.5f, 18, 2));
	add_model(p, GsVec(20.5f, 18, 2));
	add_model(p, GsVec(22.5f, 18, 2));
	add_model(p, GsVec(24.5f, 18, 2));
	add_model(p, GsVec(26.5f, 18, 2));

	//window B8
	p = new SnPrimitive(GsPrimitive::Box, 1.5, 1, 0.5);
	p->prim().material.diffuse = GsColor::darkred;
	for (int j = -7; j < 17; j = j + 3) {
		for (int i = 4; i < 11; i = i + 3) {
			add_model(p, GsVec(i + 15, j, 4.1f));
			//add_model(p, GsVec(7, 10, 1.3f));
			//add_model(p, GsVec(10, 10, 1.3f));
		}
	}
	//building 9
	p = new SnPrimitive(GsPrimitive::Box, 5, 15, 1.5);
	p->prim().material.diffuse = GsColor::cyan;
	add_model(p, GsVec(32, 4.5f, 0));


	//roof B9
	p = new SnPrimitive(GsPrimitive::Box, 1.5f, 1.5f, 1.5);
	p->prim().material.diffuse = GsColor::cyan;
	add_model(p, GsVec(32, 21, 0));

	//antenna B9
	p = new SnPrimitive(GsPrimitive::Box, 0.3f, 0.3f, 1.5);
	p->prim().material.diffuse = GsColor::cyan;
	add_model(p, GsVec(32, 22.5f, 0));
	
	//window B9
	p = new SnPrimitive(GsPrimitive::Box, 1, 2.7f, 0.5);
	p->prim().material.diffuse = GsColor::yellow;
	for (int j = 18; j >= 0; j = j - 6) {
		for (int i = 21; i < 28; i = i + 2) {
			add_model(p, GsVec(i+8, j-4, 1.3f));
		}
	}
	


	// Building 5
	p = new SnPrimitive(GsPrimitive::Box, 5.6f, 11, 1);
	p->prim().material.diffuse = GsColor::darkgreen;
	add_model(p, GsVec(-54.0f, 1.0f, 0));

	//window B8
	p = new SnPrimitive(GsPrimitive::Box, 1.2f, 1, 1);
	p->prim().material.diffuse = GsColor::darkred;
	for (int j = -7; j < 14; j = j + 3) {
		for (int i = 4; i < 11; i = i + 3) {
			add_model(p, GsVec(i - 62, j-0.9f, 0.3f));
			//add_model(p, GsVec(7, 10, 1.3f));
			//add_model(p, GsVec(10, 10, 1.3f));
		}
	}
	//antenna B6
	p = new SnPrimitive(GsPrimitive::Box, 0.2f, 1.0, 0.5);
	p->prim().material.diffuse = GsColor::green;
	add_model(p, GsVec(-52.1f, 13.0f, 1));
	add_model(p, GsVec(-55.1f, 13.0f, 1));
	add_model(p, GsVec(-58.1f, 13.0f, 1));

	p = new SnPrimitive(GsPrimitive::Box, 4.0f, 0.5f, 0.5);
	p->prim().material.diffuse = GsColor::darkgreen;
	add_model(p, GsVec(-55.1f, 14.0f, 1));
	

	//building 2
	p = new SnPrimitive(GsPrimitive::Box, 5.5, 10, 2.5);
	p->prim().material.diffuse = GsColor::blue;
	add_model(p, GsVec(42.0f, 0, 1));

	//window B3
	p = new SnPrimitive(GsPrimitive::Box, 0.4f, 8, 0.2f);
	p->prim().material.diffuse = GsColor::red;
	for (int i = 5; i < 14; i = i + 2) {
		add_model(p, GsVec(-(i)+50.9f, 0, 4));
	}
	//antenna B2
	p = new SnPrimitive(GsPrimitive::Box, 4.0f, 3, 1.5);
	p->prim().material.diffuse = GsColor::darkred;
	add_model(p, GsVec(42.0f,10, 1));
	p = new SnPrimitive(GsPrimitive::Box, 2.0f, 1, 1.5);
	p->prim().material.diffuse = GsColor::blue;
	add_model(p, GsVec(42.0f, 14, 1));


	//partition on road

	p = new SnPrimitive(GsPrimitive::Box, 2, 0.3f, 0.1f);
	p->prim().material.diffuse = GsColor::yellow;
	for (int i = -57; i < 46; i = i + 3) {
		add_model(p, GsVec(i, -10, 10.0f));
	}
	rootg()->add(g3);
	rootg()->add(g3R);
	//rootg()->add(g3RS);
	rootg()->add(g4);
	rootg()->add(g4R);
	rootg()->add(gy);
		
	
	//R1
	p = new SnPrimitive(GsPrimitive::Cylinder, 0.33f, 0.33f, 3.0);
	p->prim().material.diffuse = GsColor::blue;
	//add_model(p, GsVec(4, 0, 0));
	add_model(p, GsVec(-45.0f, -5.5f, 10.1f), manip);

	//R3
	p = new SnPrimitive(GsPrimitive::Cylinder, 0.33f, 0.33f, 3.0);
	p->prim().material.diffuse = GsColor::blue;
	//add_model(p, GsVec(4, 0, 0));
	add_model(p, GsVec(-40.0f, -5.5f, 10.1f), manip2);

	//R4
	p = new SnPrimitive(GsPrimitive::Cylinder, 0.33f, 0.33f, 3.8f);
	p->prim().material.diffuse = GsColor::blue;
	//add_model(p, GsVec(4, 0, 0));
	add_model(p, GsVec(-35.0f, -4.5f, 10.1f), manip3);

	//R2
	p = new SnPrimitive(GsPrimitive::Cylinder, 0.4f, 0.4f, 2.0);
	p->prim().material.diffuse = GsColor::blue;
	//add_model(p, GsVec(4, 0, 0));
	add_model(p, GsVec(-42.5f, -2.7f, 10.1f), manip4);

	//J1
	p = new SnPrimitive(GsPrimitive::Box, 5.5f, 0.3f, 0.5);
	p->prim().material.diffuse = GsColor::darkblue;
	add_model(p, GsVec(-40.0f, -2.5f, 10), manip5);

	//J2
	p = new SnPrimitive(GsPrimitive::Box, 2.9f, 0.2f, 0.5);
	p->prim().material.diffuse = GsColor::darkblue;
	add_model(p, GsVec(-37.5f, -3.5f, 10),manip6);

	//J3
	p = new SnPrimitive(GsPrimitive::Box, 1.8f, 0.2f, 0.5);
	p->prim().material.diffuse = GsColor::darkblue;
	add_model(p, GsVec(-41.3f, -4.5f, 10),manip7);

	//J4
	p = new SnPrimitive(GsPrimitive::Box, 2.7f, 0.3f, 0.5);
	p->prim().material.diffuse = GsColor::darkblue;
	add_model(p, GsVec(-42.3f, -8.2f, 10),manip8);
	
	//J5
//	p = new SnPrimitive(GsPrimitive::Box, 1.25, 0.2f, 0.5);
	p = new SnPrimitive(GsPrimitive::Box, 0.5f, 0.2f, 1.7f);

	p->prim().material.diffuse = GsColor::darkblue;
	add_model(p, GsVec(-35.0f, -0.8f, 10), manip9);
	
	//double t2 = gs_time();
	//gsout << t2 << gsnl;
	//gsout << (t2 - t1) << gsnl;
	
	}


// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation()
{
	//double t1 = gs_time();
	//gsout << t1 << gsnl;
	//int ind = gs_random(0, rootg()->size() - 1); // pick one child
	//SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	
	if (_animating) return; // avoid recursive calls
	_animating = true;

	//int ind = gs_random(0, rootg()->size() - 1); // pick one child
	
	_curang3 = 0;
	_curang3R = 0;
	_curang3RS = 0;

	_curang4 = 0;
	_curang4R = 0;

	double frdt = 1.0 / 30.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double t = 0, lt = 0, t0 = gs_time();
	do // run for a while:
	{
		while (t - lt<frdt) t = gs_time() - t0; // wait until it is time for next frame
		lt = t;

		//  rear 
		GsVec tr3;
		_t3->get().getrans(tr3);
		_t3->get().rotz(GS_TODEG(_curang3));
		_t3->get().setrans(tr3);
		_curang3 -= 0.00025f;

		_t3->get().getrans(tr3);
		if (tr3.x < 22.0f)
			tr3 += GsVec(0.08f, 0.0f, 0.0f);
		if (tr3.x > 22.0f) {
			tr3 = GsVec(-45.0f, -8.5f, 10.0f);
		}
		_t3->get().setrans(tr3);
		
		//  rear back
		GsVec tr3R;
		_t3R->get().getrans(tr3R);
		_t3R->get().rotz(GS_TODEG(_curang3R));
		_t3R->get().setrans(tr3R);
		_curang3R -= 0.00025f;

		_t3R->get().getrans(tr3);
		if (tr3R.x < 22.0f)
			tr3R += GsVec(0.08f, 0.0f, 0.0f);
		if (tr3R.x > 22.0f) {
			tr3R = GsVec(-45.0f, -8.5f, 10.0f);
		}
		_t3R->get().setrans(tr3R);

		/*
		//  3 Rear Shadow
		GsVec tr3RS;
		_t3RS->get().getrans(tr3RS);
		_t3RS->get().rotz(GS_TODEG(_curang3RS));
		_t3RS->get().setrans(tr3RS);
		_curang3RS -= 0.00025f;

		_t3RS->get().getrans(tr3RS);
		if (tr3RS.x < 22.0f)
			tr3RS += GsVec(0.08f, 0.0f, 0.0f);
		if (tr3RS.x > 22.0f) {
			tr3RS = GsVec(-45.0f, -18.5f, 1.0f);
		}
		_t3RS->get().setrans(tr3RS);
		*/

		//  front
		GsVec tr4;
		_t4->get().getrans(tr4);
		_t4->get().rotz(GS_TODEG(_curang4));
		_t4->get().setrans(tr4);
		_curang4 -= 0.00025f;

		_t4->get().getrans(tr4);
		if (tr4.x < 32.0f)
			tr4 += GsVec(0.08f, 0.0f, 0.0f);
		if (tr4.x > 32.0f) {
			tr4 = GsVec(-35.0f, -8.5f, 10.0f);
		}
		_t4->get().setrans(tr4);

		//  front back
		GsVec tr4R;
		_t4R->get().getrans(tr4R);
		_t4R->get().rotz(GS_TODEG(_curang4R));
		_t4R->get().setrans(tr4R);
		_curang4R -= 0.00025f;

		_t4R->get().getrans(tr4);
		if (tr4R.x < 32.0f)
			tr4R += GsVec(0.08f, 0.0f, 0.0f);
		if (tr4R.x > 32.0f) {
			tr4R = GsVec(-35.0f, -8.5f, 10.0f);
		}
		_t4R->get().setrans(tr4R);
		
		
		//R1
		GsMat m = manip->mat();	
		m.e14 = m.e14 + 0.08f;
		if (m.e14>=22.0f){
		m.e14 = m.e14 - 67.0f;
		}
		manip->initial_mat(m);

		//R3
		GsMat m2 = manip2->mat();
		m2.e14 = m2.e14 + 0.08f;
		if (m2.e14 >= 27.0f) {
			m2.e14 = m2.e14 - 67.0f;
		}
		manip2->initial_mat(m2);
		
		//R4
		GsMat m3 = manip3->mat();
		m3.e14 = m3.e14 + 0.08f;
		if (m3.e14 >= 32.0f) {
			m3.e14 = m3.e14 - 67.0f;
		}
		manip3->initial_mat(m3);

		//R2
		GsMat m4 = manip4->mat();
		m4.e14 = m4.e14 + 0.08f;
		if (m4.e14 >= 24.5f) {
			m4.e14 = m4.e14 - 67.0f;
		}
		manip4->initial_mat(m4);


		//J1
		GsMat m5 = manip5->mat();
		m5.e14 = m5.e14 + 0.08f;
		if (m5.e14 >= 27.0f) {
			m5.e14 = m5.e14 - 67.0f;
		}
		manip5->initial_mat(m5);
		
		//J2
		GsMat m6 = manip6->mat();
		m6.e14 = m6.e14 + 0.08f;
		if (m6.e14 >= 29.5f) {
			m6.e14 = m6.e14 - 67.0f;
		}
		manip6->initial_mat(m6);
		
		//J3
		GsMat m7 = manip7->mat();
		m7.e14 = m7.e14 + 0.08f;
		if (m7.e14 >= 25.7f) {
			m7.e14 = m7.e14 - 67.0f;
		}
		manip7->initial_mat(m7);
		
		//J4
		GsMat m8 = manip8->mat();
		m8.e14 = m8.e14 + 0.08f;
		if (m8.e14 >= 24.7f) {
			m8.e14 = m8.e14 - 67.0f;
		}
		manip8->initial_mat(m8);

		//J5
		GsMat m9 = manip9->mat();
		m9.e14 = m9.e14 + 0.08f;
		if (m9.e14 >= 31.5f) {
			m9.e14 = m9.e14 - 67.0f;
		}
		manip9->initial_mat(m9);


		render(); // notify it needs redraw
		ws_check(); // redraw now
	} while (true);// m.e24 > 0 );
	_animating = false;
	
	
	//double t2 = gs_time();
	//gsout << t2 << gsnl;
	//gsout << (t2 - t1) << gsnl;

}

int MyViewer::handle_keyboard(const GsEvent &e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;

	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
//	case 'n': { bool b = !_nbut->value(); _nbut->value(b);show_normals(b); return 1; }
	default: gsout << "Key pressed: " << e.key << gsnl;
	}

	return 0;
}

int MyViewer::uievent(int e)
{
	switch (e)
	{
	//case EvNormals: show_normals(_nbut->value()); return 1;
	case EvAnimate: run_animation(); return 1;
	case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
