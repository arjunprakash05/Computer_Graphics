# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
#include <sig/sn_primitive.h>
#include <sig/sn_transform.h>
#include <sig/sn_manipulator.h>

# include <sigogl/ui_button.h>	
# include <sigogl/ws_viewer.h>

// Viewer for this example:
class MyViewer : public WsViewer
{
protected:
	enum MenuEv {Evall,EvExit, EvCage, EvCPolygons, EvTeapotNormals, EvClear, EvResol5, EvResol30, EvResol60, EvResol100, EvRnormals};
public:
	MyViewer(int x, int y, int w, int h, const char* l);
	
	void add_ui();
	void Cage();
	void CPolygons();
	void shadow(GsModel * m, int discretization);
	void Teapot(int discretization);
	void TeapotNormals(int discretization);
	void clear();
	void all();
	virtual int handle_keyboard(const GsEvent &e) override;
	virtual int uievent(int e) override;
	UiCheckButton* _viewbut[4];
	UiPanel* _spanel;
	int _spanelcmds;
	void TeapotNoremove(int discretization);
	void MyViewer::Normalfunction(GsModel *m, int discretization);
};

