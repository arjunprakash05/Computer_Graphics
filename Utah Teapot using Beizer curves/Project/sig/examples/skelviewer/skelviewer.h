/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/
# pragma once

# include <sigkin/kn_skeleton.h>
# include <sigogl/ws_viewer.h>

// Viewer for this example:
class MySkelViewer : public WsViewer
{  public :
	enum MenuEv { EvPrint, EvExit };
	KnSkeleton* sk;
   public :
	MySkelViewer ( KnSkeleton* s, int x, int y, int w, int h, const char* l );
	virtual int uievent ( int e );
	void add_ui ();
	void print ();
};

