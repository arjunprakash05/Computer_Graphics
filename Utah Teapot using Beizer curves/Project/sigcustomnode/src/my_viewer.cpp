
# include "my_viewer.h"
# include "sn_circle.h"
# include "glr_circle.h"
#include <sig/sn_primitive.h>
#include <sig/sn_transform.h>
#include <sig/sn_manipulator.h>
# include "glr_circle.h"
#include <fstream>
# include <sigogl/ui_button.h>
#include <iostream>
using namespace std;

MyViewer::MyViewer(int x, int y, int w, int h, const char* l) : WsViewer(x, y, w, h, l)
{
	double t1 = gs_time();
	add_ui();
	double t2 = gs_time();
	double t = t2 - t1;
	//gsout << t<<gsnl;
}

void MyViewer::add_ui()
{
	UiPanel *p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel("Basic Things::", UiPanel::HorizLeft);
	p->add(new UiButton(" Cage made up of Triangles! ", EvCPolygons));
	p->add(new UiButton(" Cage made up of Lines! ", EvCage));
	p->add(new UiButton(" Cage and Teapot!", Evall));
	//p->add(new UiButton(" TeapotShadow ", Evshadow));
	p->add(new UiButton(" Clear all! ", EvClear));
	p->add(new UiButton(" Exit! ", EvExit));
	
	p = uim->add_panel("Change Resolution of Teapot:: ", UiPanel::HorizLeft,UiPanel::Float, 0,20);

	double t3 = gs_time();
	p->add(new UiButton("|Teapot-Res(5)|", EvResol5));
	double t4 = gs_time();
	double ta = t4 - t3;
	//gsout << ta << gsnl;

	double t5 = gs_time();
	p->add(new UiButton("|Teapot-Res(30)|", EvResol30));
	double t6 = gs_time();
	double tb = t6 - t5;
	//gsout << tb << gsnl;

	double t7 = gs_time();
	p->add(new UiButton("|Teapot-Res(60)|", EvResol60));
	double t8 = gs_time();
	double tc = t8 - t7;
	//gsout << tc << gsnl;

	double t9 = gs_time();
	p->add(new UiButton("|Teapot-Res(100)|", EvResol100));
	double t10 = gs_time();
	double td = t10 - t9;
	//gsout << td<<gsnl;

	p = uim->add_panel("Row to Display or Hide Normal Vectors::", UiPanel::HorizLeft, UiPanel::Float, 0, 40);
	
	p->add(new UiButton("            |Display Normals|            ", EvTeapotNormals));
	p->add(new UiButton("            |Hide Normals|           ", EvRnormals));
	
}

int MyViewer::handle_keyboard(const GsEvent &e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;

	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
	default: gsout << "Key pressed: " << e.key << gsnl;
	}

	return 0;
}

int MyViewer::uievent(int e)
{
	switch (e)
	{
	
	case EvCage: Cage(); return 1;
	case Evall: all(); return 1;
	case EvCPolygons: CPolygons(); return 1;
	case EvResol5:Teapot(5);return 1;
	case EvResol30:Teapot(30);	return 1;
	case EvResol60:Teapot(60);	return 1;
	case EvResol100:Teapot(100); return 1;
	case EvTeapotNormals: TeapotNormals(15); return 1;
	case EvRnormals: Teapot(50); return 1;
	case EvClear: clear(); return 1;
	case EvExit: gs_exit();
	}

	return WsViewer::uievent(e);
}

void MyViewer::all() {
	//rootg()->remove_all();
	Cage();
	TeapotNoremove(20);
}

void MyViewer::clear() {
	rootg()->remove_all();
}

void MyViewer::Cage() {
	//Teapot(10);
	double c1 = gs_time();
	SnLines *l;
	l = new SnLines;
	l->color(GsColor::darkblue);
	GsVec V[512];
	rootg()->remove_all();
	float allpoints[512][3];
	float value;
	ifstream in;
	in.open("teapotCGA.bpt.txt");
	if (!in)
	{
		gsout << "no file";
		exit(1);
	}
	int i = 0; int j = 0;
	while (in >> value)
	{
		if (value == 3 || value == 32)
		{continue;}
		if (j < 3) {
			allpoints[i][j] = value; j++;
		}
		else {
			i++; j = 0; allpoints[i][j] = value; j++;
		}
	}
	int n;
	for (int i = 0; i < 512; i++)
	{
		V[i].set(allpoints[i][0], allpoints[i][1], allpoints[i][2]);
	}
	for (int NP = 0; NP < 32; NP++) {
		for (int c = 0; c <= 12; c = c + 4) {
			l->push(V[c + NP * 16], V[c + 1 + NP * 16]);
			l->push(V[c + 1 + NP * 16], V[c + 2 + NP * 16]);
			l->push(V[c + 2 + NP * 16], V[c + 3 + NP * 16]);
			n++;

		}
		for (int r = 0; r <= 3; r = r + 1) {
			l->push(V[r + NP * 16], V[r + 4 + NP * 16]);
			l->push(V[r + 4 + NP * 16], V[r + 8 + NP * 16]);
			l->push(V[r + 8 + NP * 16], V[r + 12  + NP * 16]);
			n++;
		}
	}
	//gsout << "no of time push is called:" << n;
	rootg()->add(l);
	double c2 = gs_time();
	double c = c2 - c1;
	//gsout << "cagelines" << c;
}

void MyViewer::CPolygons() {
	double c1 = gs_time();
	rootg()->remove_all();
	SnModel *model;
	model = new SnModel;
	GsModel *m = model->model();
	model->color(GsColor::lightblue);
	m->V.size(512);
	float allpoints[512][3]; 
	float value;
	ifstream in;
	in.open("teapotCGA.bpt.txt");
	if (!in)
	{
		gsout << "no file";
		exit(1);
	}
	int i = 0; int j = 0;
	while (in >> value)
	{
		if (value == 3 || value == 32)
		{continue;}
		if (j < 3) {
			allpoints[i][j] = value; j++;}
		else {
			i++; j = 0; allpoints[i][j] = value;
			j++;}
	}
	int nt=0;
	for (int i = 0; i < 512; i++)
	{
		m->V[i].set(allpoints[i][0], allpoints[i][1], allpoints[i][2]);
	}
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j <= 2; j++) {
			for (int k = 0; k <= 2; k++) {
				m->F.push().set(k + 4 * j + 16 * i, k + 5 + 4 * j + 16 * i, k + 4 + 4 * j + 16 * i);
				m->F.push().set(k + 4 * j + 16 * i, k + 4 + 4 * j + 16 * i, k + 5 + 4 * j + 16 * i);
				m->F.push().set(k + 4 * j + 16 * i, k + 1 + 4 * j + 16 * i, k + 5 + 4 * j + 16 * i);
				m->F.push().set(k + 4 * j + 16 * i, k + 5 + 4 * j + 16 * i, k + 1 + 4 * j + 16 * i);
				nt++;
			}
		}
	}
	//gsout << "no of triangles:"<<nt;
	rootg()->add(model);
	double c2 = gs_time();
	double c = c2 - c1;

	//gsout << "cagetrianlgestime" << c;
}

void MyViewer::Teapot(int discretization){
	rootg()->remove_all();
	float value;
	ifstream in;
	in.open("teapotCGA.bpt.txt");
	if (!in) {
		gsout << "no file!!!";
		exit(1);
	}
	float allpoints[1536];
	int c = 0;
	while (in >> value) {
		if (value != 3 && value != 32) {
			allpoints[c++] = value;
		}
	}
	int element = 0;
	int dis;
	dis = discretization - 1;
	for (int a = 0; a < 32; a++) {

		float CPoints[4][4][3];
		int index = 0;
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				CPoints[i][j][0] = allpoints[element++];
				CPoints[i][j][1] = allpoints[element++];
				CPoints[i][j][2] = allpoints[element++];
			}
		}
		SnModel* _model;
		_model = new SnModel;
		GsModel* m = _model->model();
		m->V.size(discretization*discretization);
		float x, y, z;
		float mult = 1.0;
		float divi = float(dis);
		for (int i = 0; i <= dis; i++) {
			for (int j = 0; j <= dis; j++) {
				x = y = z = 0.0;
				float tu = mult*i / divi;
				float tv = mult*j / divi;
				float tU[4], tV[4];
				tU[0] = (1 - tu) * (1 - tu) * (1 - tu);
				tU[1] = 3 * tu * (1 - tu) * (1 - tu);
				tU[2] = 3 * tu * tu * (1 - tu);
				tU[3] = tu * tu * tu;
				tV[0] = (1 - tv) * (1 - tv) * (1 - tv);
				tV[1] = 3 * tv * (1 - tv) * (1 - tv);
				tV[2] = 3 * tv * tv * (1 - tv);
				tV[3] = tv * tv * tv;
				for (int k = 0; k <= 3; k++) {
					for (int l = 0; l <= 3; l++) {
						x = x + tU[k] * tV[l] * CPoints[k][l][0];
						y = y + tU[k] * tV[l] * CPoints[k][l][1];
						z = z + tU[k] * tV[l] * CPoints[k][l][2];
					}
				}
				m->V[index++].set(x, y, z);
			}
		}
		for (int i = 0; i < (index - (dis + 2)); i++) {
			m->F.push().set(i, i + 1, i + dis + 2);
			m->F.push().set(i, i + dis + 2, i + dis + 1);
		}
		//double t1 = gs_time();
		shadow(m, discretization);
		//double t2 = gs_time();
		//double t = t2 - t1;
		//gsout << "shadowing time" << t;
		_model->color(GsColor::red);
		rootg()->add(_model);
	}
	return;
}

void MyViewer::TeapotNoremove(int discretization) {
	//rootg()->remove_all();
	float value;
	ifstream in;
	in.open("teapotCGA.bpt.txt");
	if (!in) {
		gsout << "no file!!!";
		exit(1);
	}
	float allpoints[1536];
	int c = 0;
	while (in >> value) {
		if (value != 3 && value != 32) {
			allpoints[c++] = value;
		}
	}
	int element = 0;
	int dis;
	dis = discretization - 1;
	for (int a = 0; a < 32; a++) {

		float CPoints[4][4][3];
		int index = 0;
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				CPoints[i][j][0] = allpoints[element++];
				CPoints[i][j][1] = allpoints[element++];
				CPoints[i][j][2] = allpoints[element++];
			}
		}
		SnModel* _model;
		_model = new SnModel;
		GsModel* m = _model->model();
		m->V.size(discretization*discretization);
		float x, y, z;
		float mult = 1.0;
		float divi = float(dis);
		for (int i = 0; i <= dis; i++) {
			for (int j = 0; j <= dis; j++) {
				x = y = z = 0.0;
				float tu = mult*i / divi;
				float tv = mult*j / divi;
				float tU[4], tV[4];
				tU[0] = (1 - tu) * (1 - tu) * (1 - tu);
				tU[1] = 3 * tu * (1 - tu) * (1 - tu);
				tU[2] = 3 * tu * tu * (1 - tu);
				tU[3] = tu * tu * tu;
				tV[0] = (1 - tv) * (1 - tv) * (1 - tv);
				tV[1] = 3 * tv * (1 - tv) * (1 - tv);
				tV[2] = 3 * tv * tv * (1 - tv);
				tV[3] = tv * tv * tv;
				for (int k = 0; k <= 3; k++) {
					for (int l = 0; l <= 3; l++) {
						x = x + tU[k] * tV[l] * CPoints[k][l][0];
						y = y + tU[k] * tV[l] * CPoints[k][l][1];
						z = z + tU[k] * tV[l] * CPoints[k][l][2];
					}
				}
				m->V[index++].set(x, y, z);
			}
		}
		for (int i = 0; i < (index - (dis + 2)); i++) {
			m->F.push().set(i, i + 1, i + dis + 2);
			m->F.push().set(i, i + dis + 2, i + dis + 1);
		}
		//double t1 = gs_time();
		shadow(m, discretization);
		//double t2 = gs_time();
		//double t = t2 - t1;
		//gsout << "shadowing time" << t;
		_model->color(GsColor::red);
		rootg()->add(_model);
	}
	return;
}

void MyViewer::TeapotNormals(int discretization) {
	rootg()->remove_all();
	float value;
	ifstream in;
	in.open("teapotCGA.bpt.txt");
	if (!in) {
		gsout << "File not correct";
		exit(1);
	}
	float allpoints[1536];
	int c = 0;
	while (in >> value) {
		if (value != 3 && value != 32) {
			allpoints[c++] = value;
		}
	}
	int element = 0;
	
	for (int np = 0; np < 32; np++) {
		float CPoints[4][4][3];
		int index = 0;
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				CPoints[i][j][0] = allpoints[element++];
				CPoints[i][j][1] = allpoints[element++];
				CPoints[i][j][2] = allpoints[element++];
			}
		}
		float x, y, z;
		
		int dis;
		dis = discretization - 1;
		float finalV[225][3];
		float mult = 1.0;
		float divi = float(dis);
		
		for (int i = 0; i <= dis; i++) {
			for (int j = 0; j <= dis; j++) {
				x = y = z = 0.0;
				float tu = mult*i / divi;
				float tv = mult*j / divi;
				float tU[4], tV[4];
				tU[0] = (1 - tu) * (1 - tu) * (1 - tu);
				tU[1] = 3 * tu * (1 - tu) * (1 - tu);
				tU[2] = 3 * tu * tu * (1 - tu);
				tU[3] = tu * tu * tu;
				tV[0] = (1 - tv) * (1 - tv) * (1 - tv);
				tV[1] = 3 * tv * (1 - tv) * (1 - tv);
				tV[2] = 3 * tv * tv * (1 - tv);
				tV[3] = tv * tv * tv;
				for (int k = 0; k <= 3; k++) {
					for (int l = 0; l <= 3; l++) {
						x = x + tU[k] * tV[l] * CPoints[k][l][0];
						y = y + tU[k] * tV[l] * CPoints[k][l][1];
						z = z + tU[k] * tV[l] * CPoints[k][l][2];
					}
				}

				finalV[index][0] = x;
				finalV[index][1] = y;
				finalV[index][2] = z;
				index++;
			}
		}
		int finalVcount = 0;
		SnModel* _model;
		_model = new SnModel;
		_model->color(GsColor::red);
		
		GsModel* m = _model->model();
		m->V.size(225);
		for (int i = 0; i < index; i++) {
			m->V[finalVcount++].set(finalV[i][0], finalV[i][1], finalV[i][2]);
		}
		for (int i = 0; i < (index - (discretization + 1)); i++) {

			m->F.push().set(i, i + 1, i + (discretization + 1));
			m->F.push().set(i, i + (discretization + 1), i + 1);
			m->F.push().set(i, i + (discretization + 1), i + discretization);
			m->F.push().set(i, i + discretization, i + (discretization + 1));

		}
		Normalfunction(m, discretization);
		//shadow(m, resol);
		rootg()->add(_model);
	}
	return;
}

void MyViewer::Normalfunction(GsModel *m, int discretization) {
	SnLines *nl;
	nl = new SnLines;
	float rowdis1[3], rowdis2[3];
	float coldis1[3], coldis2[3];
	int dis;
	dis = discretization -1;
	int nol = 0;
	for (int rc = 0; rc < (dis +1) - 1; rc++) {
		for (int cc = 0; cc < (dis + 1) - 1; cc++) {
				if (cc == (dis + 1) - 1) {
				m->V[rc*(dis + 1) + cc - 1].get(rowdis1);
				m->V[rc*(dis + 1) + cc].get(rowdis2);
				m->V[(rc + 1)*(dis + 1) + cc].get(coldis1);
				m->V[rc*(dis + 1) + cc].get(coldis2);
				
				GsVec rowv1(rowdis1), rowv2(rowdis2);
				GsVec colv1(coldis1), colv2(coldis2);
				GsVec pv;
				pv.cross((colv1 - colv2), (rowv1 - rowv2));
				pv.normalize();
				pv.len(float(0.1));
				GsVec pt = m->V[rc*(dis + 1) + cc];
				GsVec pvf = pv;
				nl->push(pt, pt + pvf);
				nol++;
			}
			else if (rc == (dis + 1) - 1) {
				m->V[rc*(dis+1) + cc + 1].get(rowdis1);
				m->V[rc*(dis+1) + cc].get(rowdis2);
				m->V[rc*(dis+1) + cc].get(coldis1);
				m->V[(rc - 1)*(dis+1) + cc].get(coldis2);
				
				GsVec rowv1(rowdis1), rowv2(rowdis2);
				GsVec colv1(coldis1), colv2(coldis2);
				GsVec pv;
				pv.cross((colv2 - colv1), (rowv1 - rowv2));
				pv.normalize();
				GsVec pt = m->V[rc*(dis+1) + cc];
				GsVec pvf = pv;
				nl->push(pt, pt + pvf);
				nol++;
			}
			else if ((rc == (dis+1) - 1) && (cc == (dis+1) - 1)) {
				m->V[rc*(dis+1) + cc].get(rowdis1);
				m->V[rc*(dis+1) + cc - 1].get(rowdis2);
				m->V[rc*(dis+1) + cc].get(coldis1);
				m->V[(rc - 1)*(dis+1) + cc].get(coldis2);
				
				GsVec rowv1(rowdis1), rowv2(rowdis2);
				GsVec colv1(coldis1), colv2(coldis2);
				GsVec pv;
				pv.cross((rowv2 - rowv1), (colv2 - colv1));
				pv.normalize();
				GsVec pt = m->V[rc*(dis+1) + cc];
				GsVec pvf = pv;
				nl->push(pt, pt + pvf);
				nol++;
			}
			else {
				m->V[rc*(dis+1) + cc + 1].get(rowdis1);
				m->V[rc*(dis+1) + cc].get(rowdis2);
				m->V[(rc + 1)*(dis+1) + cc].get(coldis1);
				m->V[rc*(dis+1) + cc].get(coldis2);
				
				GsVec rowv1(rowdis1), rowv2(rowdis2);
				GsVec colv1(coldis1), colv2(coldis2);
				GsVec pv;
				pv.cross((rowv1 - rowv2), (colv1 - colv2));
				pv.normalize();
				GsVec pt = m->V[rc*(dis+1) + cc];
				GsVec pvf = pv;
				nl->push(pt, pt + pvf);
				nol++;
			}
		}
	}
	//gsout<<"no of normal vectors"<< nol;
	nl->color(GsColor::green);
	rootg()->add(nl);
}

void MyViewer::shadow(GsModel * m, int discretization) {
	SnModel* _model;
	_model = new SnModel;
	GsModel* newM = _model->model();
	int index = 0;
	int dis = discretization - 1;
	int c=0;
	newM->V.size(discretization*discretization);
	GsVec l(40.0f, 110.0f, 180.0f);
	for (int it = 0; it <= m->V.size() - 1; it++) {
		float cordinatex = l(0) - l(2)*(l(0) - m->V[it].x) / (l(2) - m->V[it].z);
		float cordinatey = l(1) - l(2)*(l(1) - m->V[it].y) / (l(2) - m->V[it].z);
		newM->V[it].set(cordinatex, cordinatey, 0.0f);
		index++;
	}
	//gsout << index << gsnl;
	for (int i = 0; i < (index - (dis + 2)); i++) {
		newM->F.push().set(i, i + 1, i + dis + 2);
		newM->F.push().set(i, i + dis + 2, i + dis + 1);
		c = c++;
	}
	//gsout<< "total triangles on base:" << c;
	//double t1 = gs_time();
	_model->color(GsColor::gray);
	rootg()->add(_model);
	//double t2 = gs_time();
	//double t = t2 - t1;
	//gsout << "shadowing time" << t;
}
