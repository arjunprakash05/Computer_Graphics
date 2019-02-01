/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

/** \file ui_manager.h
 * OpenGL minimalistic user interface
 */

# ifndef UI_MANAGER_H
# define UI_MANAGER_H

# include <sig/gs_event.h>
# include <sig/gs_string.h>
# include <sigogl/ui_panel.h>
# include <sigogl/ui_style.h>

# include <sig/sn_text.h>
# include <sig/sn_editor.h>

class WsWindow;
class GlRenderer;
class UiCheckButton;

/*! UiManager manages event handling and drawing of a collection of ui elements.
	Normally UiManager is directly controller by a WsWindow. 
	It derives SnEditor so that it can also be inserted in a scenegraph as 
	a 3D user interface, but this is left as future work, by translating
	scene events to ui window-coordinates events to be processed. */
class UiManager : public SnEditor
{  public :
	static const char* class_name; //<! Points to string containing UiManager

	/*! UI events are negative, starting at -500, in order to not conflict with user's events,
		which should be positive starting at 0. Range from -500 to -101 is reserved for internal
		UI events starting as follows: UiManager:-500, WsViewer:-400, dialog functions:-300. */
	enum ManagerCmd {	MCmdPanelDetached=-500,	//!< notify menu panel is becoming detached from parent
						MCmdPannelWillClose		//!< notify user clicked on a panel's close button
					};

   protected :
	WsWindow* _win;		// if not null will point to the attached window that will receive the UI events
	UiPanel* _dialog;	// if not null will point to dialog in operation receiving all current events
	UiElement* _focus;	// if not null will point to element in focus and receiving all current events
	UiElement* _eventelem; // if not null will point to element in focus and receiving all current events
	int _lastevent;		// the last event generated by the UI
	int _submenus;		// number of submenus currently open
	int _w, _h;			// size of the user interface viewport
	GsMat _projmat;		// projection matrix sent to the renderer for setting window coordinates
	float _rbx, _rby, _rbmx, _rbmy;
	UiPanel* _rbpanel;	// panel to open on a right button click, if not null
	GsArrayRef<UiPanel> _rbpanelsubs; // panels open by the right-button menu
	GsArrayRef<UiPanel> _panels; // top-level panels attached to the manager
	bool _display;
	bool _newevent;
	gscenum _changed; // 0:no change, 1:needs draw, 2:needs build
	SnGroup* group() const { return (SnGroup*)SnEditor::child(); }
	struct CmdKey { int key, cmd; gscbool alt, ctrl, shift; };
	GsArray<CmdKey> _cmdkeys;

   public : // the following methods can be called by the user:
	UiManager ( WsWindow* w );

	/*! The destructor will free all internal element elements. 
		It will honor the ref/unref methods of all inserted elements so that
		elements will self destroy if no other references to them exist. */
   ~UiManager ();

	/*! Clears any existing user interface associated with this manager */
	void init ();

	/*! Add ui panel to the manager. Label string l may be a null pointer indicating no label to be used. */
	UiPanel* add_panel ( const char* l, UiPanel::Format f=UiPanel::Vertical, UiPanel::Dock d=UiPanel::Float, int x=0, int y=0 );
	UiPanel* add_panel ( const char* l, UiPanel::Format f, int x, int y ) { return add_panel(l,f,UiPanel::Float,x,y); }
	void add ( UiPanel* p );

	/*! Will set the top element, if a panel, to activate only in response to a right-button event.
		If a previous rbutton panel already exists, it will be removed and unref() called. */
	void set_rbutton_panel ();
	void hide_rbutton_panel ();
	UiPanel* rbutton_panel () const { return _rbpanel; }
	bool rbutton_panel_active () const { return _rbpanel? _rbpanel->active():false; }

	/*! Associates key combination with event generation. No tests for duplicated definitions are performed. */
	void cmd_key ( int key, int cmd, gscbool alt=0, gscbool ctrl=0, gscbool shift=0 );

	/*! Access to top-level panels */
	UiPanel* top () { return _panels.top(); }
	UiPanel* get ( int i ) { return _panels.get(i); }
	int search ( UiPanel* p ) { return _panels.lsearch(p); }
	int panels () const { return _panels.size(); }
	void remove ( int pos );

	/*! Removes the top element. The removal honors the ref/unref method of the
		element's GsShareable base class and the element may self-destroy during the removal process */
	void pop ();

	/*! Dimensions of the ui area in window coordinates */
	int w () const { return _w; }
	int h () const { return _h; }

	/* Updates the style of all panels to the current one and marks for rebuild all managed panels. */
	void update_style ();

	/* Method build is usually to be called once after all UI elements are added. Optional
	   parameters allow specification of the range to build; if end<0, the maximum index is used.
	   A call to build will always hierachically build all elements.
	   A flag is maintained so that it is autmatically called before each render call when needed. */
	void build ( int ini=0, int end=-1 );

	/*! If the new state c is higher than the current one, mark the changed state to c.
		State number means: 0 for no updates needed, 1 needs redraw, 2 needs rebuild. */
	void changed ( gsbyte c );

	/*! Will display and process events for the given panel in exclusive focus. 
		Events generated by the panel that are <0 will cause the panel to close and
		only then this method will return. Normal (>=0) events are sent to the associated window.
		The panel's ref/unref methods from base class GsShareable are honored so that the panel
		will self destroy itself if not other reference to it exists. */
	void run_dialog ( UiPanel* p );

	/*! Returns the current dialog panel, or null if no panel is being displayed */
	UiPanel* dialog() const { return _dialog; }

	/*! Returns the last event received, negative (special dialog events) or positive */
	int last_event() const { return _lastevent; }

	/*! Returns the element which generated the last event, or null if not available. 
		Even when a non-null pointer is returned, it is the user responsibility to
		ensure that the generating element still exists when accessing its pointer. */
	UiElement* event_element () const { return _eventelem; }

   public : // UI manipulation methods:
	void focus ( UiElement* w );
	UiElement* focus () const { return _focus; }
	bool send_to_top ( UiPanel* p );
	bool send_to_back ( UiPanel* p );
	void open_submenu ( UiPanel* p );
	void close_submenu ();
	void close_submenus ( UiPanel* skip=0 );
    bool is_open_submenu ( UiPanel* p );
    int submenus () const { return _submenus; }
	void uievent ( int e, UiElement* evel, bool closesubs=true );

   public : // the following methods are to be called by the owner window:
	void render ( GlRenderer* r );
	const GsMat& projection () const { return _projmat; }
	int handle ( const GsEvent& e );
	void resize ( int w, int h );
};

//================================ End of File =================================================

# endif // UI_MANAGER_H
