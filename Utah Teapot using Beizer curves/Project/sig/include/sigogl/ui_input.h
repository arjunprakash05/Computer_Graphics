/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/
/** \file ui.h
 * OpenGL minimalistic user interface
 */

# ifndef UI_INPUT_H
# define UI_INPUT_H

# include <sig/gs_string.h>
# include <sigogl/ui_element.h>

/*! An input element allows input of a single line text.
	UiElement colors: bg:input area, fg:input text, lb:label. */
class UiInput : public UiElement
 { private :
	gscbool _allev;   // if events for each key are to be generated
	gscbool _inuse;   // true while mouse cursor is over the element
	gsint16 _cursor;  // cursor position is the index of next character
	gsint16 _curini;  // current initial text display position
	gsint16 _event;   // event id
	GsRect  _ir;	  // input rect
	GsString _text;   // input text
    GsCharPt _restext; // text to restore when Esc is pressed
	void (*_usercb) ( UiInput* inp, const GsEvent& ev, char delc, void* udata );
	void* _userdata;

   protected :
	void setir ();

   public :
	UiInput ( const char* l, int ev, int x=0, int y=0, int mw=120, int mh=0, const char* t=0 );

	GsString& value () { return _text; }
	const GsString& cvalue () const { return _text; }
	void value ( const char* s ) { _text=s; }
	void all_events ( bool b ) { _allev=b; }
	const GsRect& input_rect () const { return _ir; }

	/*! Set a user callback that is called each time a key is pressed */
	void callback ( void(*cb)(UiInput*,const GsEvent&,char,void*), void* udata ) { _usercb=cb; _userdata=udata; }

	/*! Returns the associated callback data */
	void* userdata () const { return _userdata; }

	void set_style ( const UiStyle& s );

	// Overriden virtual methods:
	virtual void change_style ( const UiStyle& s ) override;
	virtual void move ( float dx, float dy ) override;
	virtual void resize ( float w, float h ) override;
	virtual void build () override;
	virtual void draw ( UiPanel* panel ) override;
	virtual int handle ( const GsEvent& e, UiManager* uim ) override;
	virtual void close () override;
};

//================================ End of File =================================================

# endif // UI_INPUT_H
