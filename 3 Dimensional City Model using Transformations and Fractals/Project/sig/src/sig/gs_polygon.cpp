/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/
 
# include <math.h>
# include <sig/gs_geo2.h>
# include <sig/gs_string.h>
# include <sig/gs_polygon.h>

//# define GS_USE_TRACE1 
//# define GS_USE_TRACE2 // arc
//# define GS_USE_TRACE3 // convex hull
# include <sig/gs_trace.h>
 
//=================================== GsPolygon =================================================

GsPolygon::GsPolygon ( int s, int c, bool o ) : GsArray<GsVec2> ( s, c ), _open(o)
{
}

GsPolygon::GsPolygon ( GsPnt2* pt, int s, int c, bool o ) : GsArray<GsVec2> ( pt, s, c ), _open(o)
{
}

GsPolygon::GsPolygon ( const char* values, int s, int c, bool o ) : GsArray<GsVec2> ( values, s, c ), _open(o)
{
}

GsPolygon::GsPolygon ( const GsPolygon& p ) : GsArray<GsVec2> ( p )
{ 
	_open = p._open;
}

void GsPolygon::setpoly ( const float* pt, int numv, bool open )
{
	if ( numv<0 ) return;
	sizeres ( 0, numv );
	for ( ; size()<numv; pt+=2 ) push().set(pt[0],pt[1]);
	_open = open;
}

bool GsPolygon::convex () const
{
	int n, i, i1, i2;

	if ( size()<3 ) return false; // degenerated polygon with 1 or 2 edges

	double o, ordering;

	for ( i=0; i<size(); i++ )
	{	i1 = (i+1)%size();
		i2 = (i+2)%size();
		ordering = ::ccw ( get(i), get(i1), get(i2) );
		if ( ordering!=0 ) break;
	}

	if ( ordering==0 ) return false; // not even a simple polygon

	for ( n=0; n<size(); n++ )
	{	i1 = (i+1)%size();
		i2 = (i+2)%size();
		o = ::ccw ( get(i), get(i1), get(i2) );
		if ( o*ordering<0 ) return false; // not a convex angle
		i++;
	}

	return true;
}

float GsPolygon::area () const // oriented: >0 if ccw
{
	float sum=0;
	int i, j;
  
	if ( size()<=2 ) return 0; // degenerated polygon with 1 or 2 edges

	for ( i=0; i<size(); i++ )
	{	j = vid(i+1);
		sum += get(i).x*get(j).y - get(j).x*get(i).y;
	}

	return sum/2.0f;
}

bool GsPolygon::contains ( const GsPnt2& p ) const
{
	bool cont=false;
	int i, j, s=size();
	for ( i=0, j=s-1; i<s; j=i++ )
	{	const GsVec2& a = get(i);
		const GsVec2& b = get(j);
		if ( a.y<=b.y )
		{	if ( a.y<p.y && b.y>=p.y ) // intercepts
			if ( p.x < a.x + (p.y-a.y) * (b.x-a.x) / (b.y-a.y) ) cont = !cont;
		}
		else if ( b.y<p.y && a.y>=p.y ) // intercepts
		{	if ( p.x < a.x + (p.y-a.y) * (b.x-a.x) / (b.y-a.y)) cont = !cont;
		}
	}
	return cont;
}

bool GsPolygon::contains ( const GsPolygon& pol ) const
{
	for ( int i=0; i<pol.size(); i++ )
	{	if ( !contains(pol[i]) ) return false; }
	return true;
}

GsPnt2 GsPolygon::sample ( char mode ) const
{
	GsPnt2 p;
	float ax, ay, bx, by;
	get_bounding_box ( ax, ay, bx, by );

	p.x = gs_random ( ax, bx );
	p.y = gs_random ( ay, by );
	if ( mode=='b' ) return p;

	while ( !contains(p) )
	{	p.x = gs_random ( ax, bx );
		p.y = gs_random ( ay, by );
	}
	return p;
}

int GsPolygon::has_in_boundary ( const GsPnt2& p, float ds ) const
{
	int i1, i2;
	for ( i1=0; i1<size(); i1++ )
	{	i2 = (i1+1)%size();
		if ( in_segment ( get(i1), get(i2), p, ds ) ) return i1;
	}
	return -1;
}

void GsPolygon::circle_approximation ( const GsPnt2& center, float radius, int nvertices )
{
	GsVec2 p;
	float ang=0, incang = ((float)GS_2PI) / nvertices;
	size(nvertices); // reserve memory
	size(0);
	while ( nvertices>0 )
	{	p.set ( radius*sinf(ang), radius*cosf(ang) );
		p += center;
		push ( p );
		ang += incang;
		nvertices--;
	}
	_open = 0;
}

void GsPolygon::square ( const GsPnt2& center, float radius )
{
	size(0);
	push().set ( center.x-radius, center.y-radius );
	push().set ( center.x+radius, center.y-radius );
	push().set ( center.x+radius, center.y+radius );
	push().set ( center.x-radius, center.y+radius );
}

float GsPolygon::perimeter () const
{
	if ( size()<2 ) return 0;
	int i;
	float len=0;
	for ( i=1; i<size(); i++ ) len += dist ( cget(i-1), cget(i) );
	if ( !_open ) len += dist ( cget(i), cget(0) );
	return len;
}

GsPnt2 GsPolygon::interpolate_along_edges ( float t ) const
{
	int ilast, i1, i2=0;
	float len1=0, len2=0;

	GsVec2 v;
	if ( size()==0 ) return v;
	if ( size()==1 || t<0 ) return cget(0);
	ilast = _open? size()-1:0;

	v = cget(0);
	if ( t<=0 ) return v;

	for ( i1=0; i1<size(); i1++ )
	{	if ( i1==size()-1 )
		{	if ( _open ) break;
			i2 = 0;
		}
		else i2 = i1+1;
		len1 = len2;
		len2 += dist ( cget(i1), cget(i2) );
		if ( t<len2 ) break;
	}
   
	if ( _open )
	{	if ( i1==ilast ) return cget(ilast); }
	else
	{	if ( i2==0 ) return cget(0); }

	len2 -= len1;
	t -= len1;
	t /= len2;
	v = mix ( cget(i1), cget(i2), t );
	return v;
}

void GsPolygon::resample ( float maxlen )
{
	if ( maxlen<=0 ) return;
	GsVec2 v;
	int i, j, isub;
	float len, nsub;
	GsPolygon psamp ( 0, size(), open() );
	for ( i=0; i<size(); i++ )
	{	if ( i+1==size() && _open ) { psamp.push()=top(); break; }
		const GsPnt2& p1 = get(i);
		const GsPnt2& p2 = get ( (i+1)%size() );
		psamp.push() = p1;
		len = dist(p1,p2);
		if ( len>maxlen )
		{	nsub = len/maxlen;
			isub = int(nsub);
			if ( nsub!=float(isub) ) isub++;
			len = len/float(isub);
			v = p2 - p1;
			v.len(len);
			for ( j=1; j<isub; j++ ) psamp.push()=p1+(v*float(j));
		}
	}
	adopt ( psamp );
}

void GsPolygon::remove_duplicate_vertices ( float epsilon )
{
	int i1=0;
	epsilon *= epsilon;
	while ( i1<size() )
	{	if ( _open && i1+1==size() ) break;
		if ( dist2(get(i1),get((i1+1)%size()))<=epsilon ) remove(i1);
		else i1++;
	}
}

void GsPolygon::remove_collinear_vertices ( float epsilon )
{
	int i=0, i1, i2;
	while ( i<size() )
	{	if ( _open && i>=size()-2 ) break;
		i1 = (i+1)%size();
		i2 = (i+2)%size();
		if ( gs_point_line_dist ( get(i).x, get(i).y,
								get(i1).x, get(i1).y,
								get(i2).x, get(i2).y )<=epsilon )
			remove(i1);
		else
		i++;
	}
}

GsVec2 GsPolygon::centroid () const
{
	GsVec2 c;
	for ( int i=0; i<size(); i++ ) c += get(i);
	c /= (float)size();
	return c;
}

void GsPolygon::translate ( const GsVec2& dv )
{
	for ( int i=0; i<size(); i++ ) set ( i, get(i)+dv );
}

void GsPolygon::rotate ( const GsPnt2& center, float radians )
{
	int i;
	float s = sinf ( radians );
	float c = cosf ( radians );

	for ( i=0; i<size(); i++ )
	{	GsVec2& v = (*this)[i];
		v.rot ( center, s, c );
	}
}

void GsPolygon::scale ( float s )
{
	for ( int i=0; i<size(); i++ ) set ( i, get(i)*s );
}

GsVec2 GsPolygon::south_pole ( int* index ) const
{
	int i;
	GsVec2 p;
	if ( size()==0 ) return p;
   
	p=get(0); if (index) *index=0; 

	for ( i=1; i<size(); i++ )
		if ( get(i).y<p.y )
		{	p=get(i); if (index) *index=i; }

	return p;
}

struct WV { int i; float a; void set ( int n, float f ) { i=n; a=f; } };

static int sWrap ( GsPolygon& data, int ini, const GsPnt2& o, const GsVec2& v, GsArray<WV>& a )
{
	int i, imin;
	float angmin = 100.0f;
	float ang;

	a.size(1);
	a[0].set(0,angmin);
	int n = data.size();
	for ( i=ini; i<n; i++ )
	{	ang = angle ( v, data[i]-o );
		if ( ang<=angmin )
		{	angmin=ang; imin=i;
			if ( GS_NEXT(a.top().a,angmin,0.00001f) )
			{	a.push().set(imin,angmin); }
			else
			{	a.size(1); a[0].set(imin,angmin); }
		}
	}

	// if there are collinear points, pick the farthest one:
	if ( a.size()>1 )
	{	imin = a[0].i;
		if ( imin==0 ) return 0;
		float d, dmax = dist2(o,data[imin]);
		for ( i=1; i<a.size(); i++ )
		{	if ( a[i].i==0 ) return 0;
			d = dist2 ( o, data[a[i].i] );
			if ( d>dmax ) { dmax=d; imin=a[i].i; }
		}
	}

	GS_TRACE3 ( "Min index:"<<imin<<" ang:"<<angmin<<" point:"<<p[imin] );
	return imin;
}

void GsPolygon::convex_hull ( GsPolygon& hull ) const
{
	if ( size()<=3 ) { hull=*this; return; }

	GS_TRACE3("Convex Hull Starting");

	hull.reserve(size());
	hull.size(0);

	int i0;
	GsPolygon data ( *this );
	GsPnt2 sp = data.south_pole(&i0);
	if ( i0>0 ) { data[i0]=data[0]; data[0]=sp; i0=0; } // sp is now at 0 pos
	GS_TRACE3("South pole: "<<sp);
	hull.push(sp);

	GsVec2 v = GsVec2::i;
	int nexti, ini=1;
	GsArray<WV> buffer;
	GsPnt2 o = sp;
	
	int count = size()+1;
	while ( count-- ) // this is a protection, the loop should break when i==i0
	{
		nexti = sWrap ( data, ini, o, v, buffer );
		if ( nexti==i0 ) break; // back to south pole
		ini = 0;
		v = data[nexti]-o; v.normalize();
		o = data[nexti];
		if ( dist2(hull.top(),o)>gsmall ) hull.push()=o;
		data[nexti] = data.pop(); // remove point from data set
		if ( data.size()==1 ) break; // only south pole left
	}
	if ( count==0 ) gsout.warning ( "Forced loop break in GsPolygon::convex_hull()!" );

	// could remove collinear pnts here...

	GS_TRACE3("Result: "<<pol);
}

int GsPolygon::pick_vertex ( const GsPnt2& p, float epsilon, bool first ) const
{
	int i, imin=-1;
	if ( size()==0 ) return imin;

	float dist, distmin;
	distmin = dist2(get(0),p);
	imin = 0;
	if ( first ) epsilon*=epsilon;

	for ( i=1; i<size(); i++ )
	{	dist = dist2(get(i),p);
		if ( first ) { if (dist<=epsilon) return i; }
		if ( dist<distmin ) { distmin=dist; imin=i; }
	}

	if ( distmin<=epsilon )
		return imin;
	else
		return -1;
}

int GsPolygon::pick_edge ( const GsPnt2& p, float epsilon, float& dist2 ) const
{ 
	int i, i2, iresult;
	double d, t;

	iresult=-1;
	dist2=-1;

	int s = size();
	for ( i=0; i<s; i++ )
	{	i2 = (i+1)%s;
		if ( i2==0 && open() ) break;
		if ( in_segment(cget(i),cget(i2),p,epsilon,d,t) )
		{	if ( dist2<0 || d<dist2 )
			{	iresult = i;
				dist2 = (float)d;
			}
		}
	}

	return iresult;
}

void GsPolygon::ear_triangulation ( GsArray<int>& tris, float prec ) const
{
	bool ok;
	int a1, a2, a3, b, size;
	double ccw;
	tris.size(0); 

	if ( GsPolygon::size()<3 ) return;
	GsPolygon pol(*this);
	GsArray<int> im(pol.size()); // index mapping array
	for ( int i=0; i<im.size(); i++ ) im[i]=i;

	pol.remove_collinear_vertices ( prec );

	while ( pol.size()>3 )
	{	size = pol.size();
		for ( a2=pol.size()-1; a2>=0; a2-- )
		{	a1 = pol.vid(a2-1);
			a3 = pol.vid(a2+1);

			if ( next(pol[a1],pol[a2],prec) || next(pol[a2],pol[a3],prec) )
			{	pol.remove(a2); /*maxangi=-2;*/ break; }

			//note: this is using a non-robust geometric primitive
			ccw = gs_ccw ( pol[a1].x, pol[a1].y, pol[a2].x, pol[a2].y, pol[a3].x, pol[a3].y );
			if ( ccw<=0 ) continue; // not ccw

			ok = true;
			for ( b=0; b<pol.size(); b++ )
			{	if ( b==a1 || b==a2 || b==a3 ) continue;
				if ( gs_in_triangle_interior ( pol[a1].x, pol[a1].y, pol[a2].x, pol[a2].y,
											   pol[a3].x, pol[a3].y, pol[b].x, pol[b].y ) )
				{	ok=false; break; }
			}

			if ( ok )
			{	tris.push()=im[a1]; tris.push()=im[a2]; tris.push()=im[a3];
				pol.remove(a2); im.remove(a2);
				break;
			}
		}

		if ( size==pol.size() ) // no diagonals worked so pick shortest ccw one (may happen due self-intersections)
		{	float d2, mind2=-1;
			int mina2=pol.size()-1;
			for ( a2=mina2; a2>=0; a2-- )
			{	a1 = pol.vid(a2-1);
				a3 = pol.vid(a2+1);
				if ( gs_ccw ( pol[a1].x, pol[a1].y, pol[a2].x, pol[a2].y, pol[a3].x, pol[a3].y )<=0 ) continue; // not ccw
				d2 = dist2 ( pol[a1], pol[a3] );
				if ( d2<mind2 || mind2<0 ) { mind2=d2; mina2=a2; }
			}
			if ( mind2>=0 )
			{	a2=mina2; a1=pol.vid(a2-1); a3=pol.vid(a2+1);
				tris.push()=im[a1]; tris.push()=im[a2]; tris.push()=im[a3];
				pol.remove(a2); im.remove(a2);
				break;
			}
		}

		if ( size==pol.size() ) break; // loop protection
	}

	tris.push()=im[0]; tris.push()=im[1]; tris.push()=im[2];
}

void GsPolygon::get_bounding_box ( float& minx, float& miny, float& maxx, float& maxy ) const
{
	int s = size();
	if ( s==0 )
	{	minx=miny=1.0f; maxx=maxy=0; // define an "empty box"
		return;
	}
   
	int i;
	minx = maxx = get(0).x;
	miny = maxy = get(0).y;
	for ( i=1; i<s; i++ )
	{	GS_UPDMIN ( minx, get(i).x ); GS_UPDMAX ( maxx, get(i).x );
		GS_UPDMIN ( miny, get(i).y ); GS_UPDMAX ( maxy, get(i).y );
	}
}

//================================ configs =================================================

void GsPolygon::get_configuration ( float& x, float& y, float& a ) const
{
	GsVec2 c = centroid();
	x=c.x; y=c.y;
	a = oriangle ( GsVec2::i, get(0)-c );
	if ( a<0 ) a += gs2pi;
}

void GsPolygon::set_configuration ( float x, float y, float a )
{
	GsVec2 c = centroid();
	GsVec2 nc(x,y);

	translate ( nc-c );

	float b = oriangle ( GsVec2::i, get(0)-nc );
	if ( b<0 ) b += gs2pi;
	a = a-b;

	rotate ( nc, a );
}

GsOutput& operator<< ( GsOutput& out, const GsPolygon& p )
{
	out << ( p.open()? "polyline " : "polygon " );
	return out << (const GsArray<GsPnt2>&)p;
}

GsInput& operator>> ( GsInput& inp, GsPolygon& p )
{
	p.open ( false );
	if ( inp.check()==GsInput::String )
	{	inp.get();
		if ( inp.ltoken().len()>4 && inp.ltoken()[4]=='l' ) p.open ( true );
	}

   return inp >> (GsArray<GsPnt2>&)p;
}

//================================ End of File =================================================

