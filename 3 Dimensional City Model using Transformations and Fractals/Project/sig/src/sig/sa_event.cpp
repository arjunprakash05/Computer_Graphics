/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/sn_editor.h>
# include <sig/sa_event.h>

//# define GS_USE_TRACE1 // constructor / destructor
//# define GS_USE_TRACE2 // closest editor selection
# include <sig/gs_trace.h>

//================================== SaEvent ====================================

void SaEvent::apply ( SnNode* n )
{
	_hits.size ( 0 );
	SaAction::apply ( n );

	if ( _hits.size()>0 ) // sort and call editor closest to viewer
	{	GS_TRACE2 ( "Hits: "<<_hits.size() );
		int i, imin = 0;
		float tmin = _hits[0].t;
		for ( i=1; i<_hits.size(); i++ )
		{	if ( _hits[i].t<tmin ) { tmin=_hits[i].t; imin=i; } }
		GS_TRACE2 ( "min index: "<<imin );
		_hits[imin].ed->handle_event ( _hits[imin].ev, _hits[imin].t );
	}
}

bool SaEvent::editor_apply ( SnEditor* m )
{
	if ( !m->visible() ) return true;

	push_matrix ();
	mult_matrix ( m->mat() );

	GsMat mat = _matstack.top();
	mat.invert();

	GsEvent e = _ev;
	e.ray.p1 = mat*e.ray.p1;
	e.ray.p2 = mat*e.ray.p2;
	e.lray.p1 = mat*e.lray.p1;
	e.lray.p2 = mat*e.lray.p2;
	e.mousep = mat*e.mousep;
	e.lmousep = mat*e.lmousep;

	float t;
	int res = m->check_event ( e, t );
	if ( _result==0 && res ) _result=res;

	if ( res==2 ) // event must be handled now
	{	GS_TRACE2 ( "Priority Hit t="<<t );
		m->handle_event ( e, t );
		_hits.size(0); // ensure no sorting is done
		return false; // interrupt traversal
	}
	else if ( res==1 ) // event can be handled: save it
	{	GS_TRACE2 ( "Hit t="<<t );
		_hits.push();
		_hits.top().ev = e;
		_hits.top().ed = m;
		_hits.top().t = t;
	}

	pop_matrix ();
	return true;
}

//======================================= EOF ====================================
