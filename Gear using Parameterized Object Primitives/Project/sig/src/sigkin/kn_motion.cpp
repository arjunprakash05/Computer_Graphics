/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <math.h>
# include <stdlib.h>
# include <string.h>

# include <sig/gs_vars.h>
# include <sigkin/kn_motion.h>
# include <sigkin/kn_posture.h>
# include <sigkin/kn_skeleton.h>

//============================= KnMotion ============================

KnMotion::KnMotion()
 {
   _name = 0;
   _filename = 0;
   _last_apply_frame = 0;
   _freq = 0;
   _userdata = 0;
 }

KnMotion::~KnMotion()
 {
   init ();
   delete[] _name;
   delete[] _filename;
   if ( _userdata ) _userdata->unref();
 }

void KnMotion::init()
 {
   while ( _frames.size()>0 ) _frames.pop().posture->unref();
   _last_apply_frame = 0;
 }

void KnMotion::compress ()
 {
   _frames.compress ();
   if ( channels() ) channels()->compress ();
 }

bool KnMotion::makeasref ( const GsArray<KnPosture*>& keypost, 
						   const GsArray<float>& keytime )
 {
   if ( keypost.size()<1 || keypost.size()!=keytime.size() ) return false;
   init ();

   // create the frames:
   int i;
   _frames.reserve ( keypost.size() );
   for ( i=0; i<keypost.size(); i++ )
	{ insert_frame ( i, keytime[i], keypost[i] );
	}
   compress ();
   return true;
 }

bool KnMotion::make ( const GsArray<KnPosture*>& keypost, 
					  const GsArray<float>& keytime )
 {
   if ( keypost.size()<1 || keypost.size()!=keytime.size() ) return false;

   // Make the "varying values" channels:
   GsArray<int> index;
   KnChannels* channels = new KnChannels;
   if ( !channels->make(keypost,&index) ) { delete channels; return false; }
   init ();

   // create the frames:
   int i, j;
   _frames.reserve ( keypost.size() );

   for ( i=0; i<keypost.size(); i++ )
	{
	  insert_frame ( i, keytime[i], new KnPosture(channels) );
	  for ( j=0; j<index.size(); j++ )
	   { if ( index[j]<0 ) continue;
		 _frames[i].posture->values[index[j]] = keypost[i]->values[j];
	   }
	}

   compress ();
   return true;
 }

bool KnMotion::insert_channel ( int pos, KnJointName jname, KnChannel::Type type, float* fvalues )
 {
   KnChannels* chs = channels();
   if ( !chs ) return false;

   // Insert channel:
   if ( !chs->insert(pos,type) ) return false;
   chs->get(pos).jname ( jname );
   
   // Insert values in all frames:
   int chsize = KnChannel::size(type);
   int fpos = chs->floatpos(pos);
   int frsize = _frames.size();
   int f, i;
   for ( f=0; f<frsize; f++ )
	{ 
	  _frames[f].posture->values.insert ( fpos, chsize );
	  for ( i=0; i<chsize; i++ )
	   _frames[f].posture->values[fpos+i] = fvalues? fvalues[i]:0.0f;
	}

   return true;
 }

bool KnMotion::remove_frame (int pos )
 {
   if ( pos<0 || pos>=_frames.size() ) return false;
   _frames.get(pos).posture->unref();
   _frames.remove(pos);
   return true;
 }

bool KnMotion::insert_frame ( int pos, float kt, KnPosture* p )
 {
   if ( pos<0 || pos>_frames.size() ) return false;
   _frames.insert ( pos );
   _frames[pos].keytime = kt;
   _frames[pos].posture = p;
   _frames[pos].posture->ref();
   _last_apply_frame = 0;
   return true;
 }

void KnMotion::dfjoints ( KnPostureDfJoints* dfjoints )
 {
   int f, frsize = _frames.size();
   for ( f=0; f<frsize; f++ )
	{ _frames[f].posture->dfjoints ( dfjoints );
	}
 }

int KnMotion::connect ( const KnSkeleton* s )
 {
   KnChannels* ch = channels();
   if ( !ch ) return 0;
   return ch->connect(s);
 }

int KnMotion::connect ( const KnPosture* p )
 {
   KnChannels* ch = channels();
   if ( !ch ) return 0;
   return ch->connect(p);
 }

void KnMotion::disconnect ()
 {
   KnChannels* ch = channels();
   if ( ch ) ch->disconnect();
 }

void KnMotion::apply_frame ( int f )
 {
   int lf = _frames.size()-1;
   if ( lf<0 ) return;
   GS_CLIP(f,0,lf);
   _frames[f].posture->apply();
 }

inline float _cubic ( float t, float tmin, float tmax )
 {
   t = (t-tmin)/(tmax-tmin);	// normalize t to [0,1]
   t=-(2.0f*(t*t*t)) + (3.0f*(t*t)); // cubic spline
   return t*(tmax-tmin) + tmin; // scale back
   // shape comparison with sine for graphmatica:
   // y=-(2.0*(x*x*x)) + (3.0*(x*x))
   // y=sin((x-0.5)*3)/2+0.5
 }

void KnMotion::apply ( float t, KnMotion::InterpType itype, int* lastframe )
 {
   int fsize=_frames.size();
   if ( fsize<=0 ) return;
   if ( t<_frames[0].keytime ) return;
   
   if ( itype==CubicSpline )
	t = _cubic ( t, _frames[0].keytime, _frames.top().keytime );

   // optimize keytime search for sequenced calls with increasing t
   int fini=0;
   if ( lastframe ) _last_apply_frame = *lastframe;
   if ( _last_apply_frame>0 && _last_apply_frame<fsize )
	{ if ( t>_frames[_last_apply_frame].keytime )
	   fini=_last_apply_frame+1;
	}

   int i, f;
   for ( f=fini; f<fsize; f++ )
	{ if ( t<_frames[f].keytime ) break; }

   if ( f==_frames.size() ) { apply_frame(f-1); return; }

   f--;
   _last_apply_frame = f;
   if ( lastframe ) *lastframe = _last_apply_frame;

   float* fp1 = &(_frames[f].posture->values[0]);
   float* fp2 = &(_frames[f+1].posture->values[0]);
   // convert t to [0,1] according to the adjacent keytimes:
   t = (t-_frames[f].keytime) / (_frames[f+1].keytime-_frames[f].keytime);

   //gsout<<"t: "<<t<<" frames: "<<f<<gspc<<(f+1)<<"\n";
   int chfloats;
   KnChannels* chs = posture(0)->channels(); // recall that all postures share the same channels object
   int numchs = chs->size();

   float values[4]; // 4 is the max num of values per channel
   for ( i=0; i<numchs; i++ )
	{ KnChannel& ch = chs->get(i);
	  if ( ch.status()!=KnChannel::Disconnected )
	   { ch.interp ( fp1, fp2, t, values );
		 chfloats = ch.apply ( values );
	   }
	  else
	   { chfloats = ch.size();
	   }

	  fp1+=chfloats; fp2+=chfloats;
	}
 }

const char* KnMotion::interp_type_name ( InterpType type ) // static
 {
   switch ( type )
	{ case CubicSpline : return "CubicSpline";
	  default : return "Linear";
	}
 }

KnMotion::InterpType KnMotion::interp_type_name ( const char* type ) // static
 {
   if ( gs_compare(type,"CubicSpline")==0 ) return CubicSpline;
   return Linear;
 }

void KnMotion::operator = ( const KnMotion& m )
 {
   init();
   name(m.name());
   if ( !m.frames() ) return;

   KnChannels* chs = new KnChannels ( *m.channels() );
   KnPostureDfJoints* dfj=0;
   if ( m.posture(0)->dfjoints() )
	{ dfj = new KnPostureDfJoints;
	  *dfj = *(m.posture(0)->dfjoints());
	}

   int i, fsize = m._frames.size();
   for ( i=0; i<fsize; i++ )
	{ insert_frame ( i, m.keytime(i), new KnPosture ( *m.posture(i) ) );
	  posture(i)->dfjoints ( dfj );
	  posture(i)->channels ( chs );
	}
 }

void KnMotion::move_keytimes ( float startkt )
 {
   if ( _frames.size()==0 ) return;
   
   float diff = _frames[0].keytime-startkt;
   if ( diff==0 ) return;

   int i;
   for ( i=0; i<_frames.size(); i++ )
	{ _frames[i].keytime -= diff;
	}
 }

static float _correct ( float a1, float a2 )
 {
   if ( GS_DIST(a1,a2)<=gspi ) return a2;

   if ( a2>a1 )
	return _correct ( a1, a2-gs2pi );
   else
	return _correct ( a1, a2+gs2pi );
 }

int KnMotion::correct_euler_angles ()
 {
   int frsize = _frames.size();
   int count=0;
   
   if ( frsize<=1 ) return count;

   KnChannel::Type type;
   KnChannels* chs = channels();
   int chsize = chs->size();
   int i, j, p=0;

   for ( i=0; i<chsize; i++ )
	{ type = chs->get(i).type();
	  if ( type<KnChannel::XRot || type>KnChannel::ZRot )
	   { p += KnChannel::size ( type );
	   }
	  else
	   { count++;
		 for ( j=1; j<frsize; j++ )
		  _frames[j].posture->values[p] = _correct ( _frames[j-1].posture->values[p], _frames[j].posture->values[p] );
		 p++;
	   }
	}
   return count;
 }

void KnMotion::change_values ( int f1, int f2, int channel, float mfactor, const float* offset )
 {
   int frsize = _frames.size();
   if ( frsize==0 ) return;
   f2 = GS_BOUND(f2,0,(frsize-1));
   f1 = GS_BOUND(f1,0,f2);
   KnChannels* chs = channels();
   KnChannel& ch = chs->get(channel);
   int fp = chs->floatpos ( channel );
   for ( int f=f1; f<=f2; f++ )
	{ ch.change_values ( &_frames[f].posture->values[fp], mfactor, offset );
	}
 }

void KnMotion::mirror ( const char* left, const char* right, bool printerrors )
 {
   for ( unsigned int f=0; f<frames(); f++ )
	{ apply_frame(f);
	  posture(f)->mirror ( left, right, printerrors );
	}
 }

void KnMotion::append ( KnMotion* m, float deltakt )
 {
   float ikt = last_keytime()+deltakt;
   for ( gsuint f=0; f<m->frames(); f++ )
	{ add_frame ( m->keytime(f)+ikt, m->posture(f) );
	  m->posture(f)->channels ( channels() ); // share channel of first frame
	}
 }

bool KnMotion::append ( const char* filename, float deltakt )
 {
   KnMotion m;
   if ( !m.load(filename) ) return false;
   append ( &m, deltakt );
   return true;
 }

GsVars* KnMotion::userdata ()
 { 
   if ( !_userdata )
	{ _userdata = new GsVars;
	  _userdata->ref();
	}
   return _userdata;
 }
