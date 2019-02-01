/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sigkin/kn_hoap_ik.h>
# include <sigkin/kn_coldet.h>
# include <sigkin/kn_skeleton.h>
# include <sig/sn_lines.h>
# include <sig/gs_euler.h>

//=============================== KnHoapIk ===============================

void KnHoapIk::_init () // private init method
 {
   _type = LeftLeg;

   _snlines = 0;

   int i;
   for ( i=0; i<6; i++ )
	{ _result[i]=0;
	  _joint[i]=0;
	}
   
   _d1 = _d2 = -1;
   
   _solve_rot_goal = true;
 }

KnHoapIk::KnHoapIk ()
 {
   _init ();
 }

KnHoapIk::KnHoapIk ( Type t, KnSkeleton* sk )
 {
   _init ();
   init ( t, sk );
 }

KnHoapIk::~KnHoapIk()
 { 
   if ( _snlines ) _snlines->unref();
 }

void KnHoapIk::lines ( SnLines* sl )
 { 
   if ( _snlines ) _snlines->unref(); // unref old
   if ( sl ) sl->ref(); // ref new
   _snlines = sl; // store new
 }

void KnHoapIk::_setjoint ( KnSkeleton* sk, int i, const char* jname, RotAxis ax )
 {
   KnJoint* j = sk->joint(jname);
   if ( !j ) gsout.fatal ( "HoapIk: joint [%s] not found", jname );
   if ( j->rot_type()!=KnJoint::TypeEuler ) gsout.fatal ( "HoapIk: joint [%s] is not of Euler type", jname );
   _joint[i] = j;
   _axis[i] = ax;
 }

void KnHoapIk::init ( Type t, KnSkeleton* sk )
 {
   _type = t;

   if ( _type==LeftLeg )
	{ _setjoint ( sk, 0, "LeftUpLegBase", Y );
	  _setjoint ( sk, 1, "LeftUpLeg", X );
	  _setjoint ( sk, 2, "LeftUpLeg", Z );
	  _setjoint ( sk, 3, "LeftLeg", X );
	  _setjoint ( sk, 4, "LeftFootBase", X );
	  _setjoint ( sk, 5, "LeftFootBase", Z );
	}
   else if ( _type==RightLeg )
	{ _setjoint ( sk, 0, "RightUpLegBase", Y );
	  _setjoint ( sk, 1, "RightUpLeg", X );
	  _setjoint ( sk, 2, "RightUpLeg", Z );
	  _setjoint ( sk, 3, "RightLeg", X );
	  _setjoint ( sk, 4, "RightFootBase", X );
	  _setjoint ( sk, 5, "RightFootBase", Z );
	}
   else if ( _type==LeftArm )
	{ _setjoint ( sk, 0, "LeftShoulder", X );
	  _setjoint ( sk, 1, "LeftUpArm", Y );
	  _setjoint ( sk, 2, "LeftUpArmRoll", Z );
	  _setjoint ( sk, 3, "LeftForeArm", X );
	  _setjoint ( sk, 4, "LeftHand", Z );
	  _setjoint ( sk, 5, "LeftHandIKEnd", NoAxis );
	}
   else // _type==RightArm
	{ _setjoint ( sk, 0, "RightShoulder", X );
	  _setjoint ( sk, 1, "RightUpArm", Y );
	  _setjoint ( sk, 2, "RightUpArmRoll", Z );
	  _setjoint ( sk, 3, "RightForeArm", X );
	  _setjoint ( sk, 4, "RightHand", Z );
	  _setjoint ( sk, 5, "RightHandIKEnd", NoAxis );
	}

   _d1 = mid()->offset().len();
   _d2 = end()->offset().len();

   if ( _type==LeftArm || _type==RightArm )
	{ _d1 += _joint[2]->offset().z; _d2 += _joint[4]->offset().z; }

   //gsout<<"Lens: "<<_d1<<", "<<_d2<<"; "<<(_d1+_d2)<<gsnl;
 }

void KnHoapIk::init ( Type t, KnJoint* j )
 {
   init ( t, j->skeleton() );
 }

void KnHoapIk::remove_joint_limits ()
 {
   if ( !_joint[0] ) return;
   _joint[0]->euler()->limits ( _axis[0], -gspi, gspi );
   _joint[1]->euler()->limits ( _axis[1], -gspi, gspi );
   _joint[2]->euler()->limits ( _axis[2], -gspi, gspi );
   _joint[3]->euler()->limits ( _axis[3], -gspi, gspi );
   _joint[4]->euler()->limits ( _axis[4], -gspi, gspi );
   if ( _axis[5]!=NoAxis ) _joint[5]->euler()->limits ( _axis[5], -gspi, gspi );
}

void KnHoapIk::update_base_up ()
 {
   if ( !base() ) return;
   if ( !base()->skeleton()->global_matrices_uptodate() ) base()->update_gmat_up();
 }
 
void KnHoapIk::update_base_end ()
 {
   if ( !base() ) return;

   if ( _type<2 ) // arms case
	{ _joint[0]->update_gmat_local();
	  _joint[1]->update_gmat_local();
	  _joint[2]->update_gmat_local();
	  _joint[3]->update_gmat_local();
	  _joint[4]->update_gmat_local();
	  _joint[5]->update_gmat_local();
	}
   else
	{ _joint[0]->update_gmat_local();
	  _joint[1]->update_gmat_local();
	  _joint[3]->update_gmat_local();
	  _joint[4]->update_gmat_local();
	}
}

void KnHoapIk::base_frame ( GsMat& bframe )
 {
   if ( !base() ) return;
   // bframe will be the same as : {base(0)->init_rot(); base(1)->init_rot(); bframe=base(1)->gmat();}
   // but we do this without changing joint values (without calling init_rot())
   GsMat jmat;
   quat2mat ( base()->quat()->prerot(), jmat );
   jmat.setl4 ( base()->offset() );
   bframe.mult ( jmat, base()->parent()->gmat() );
   // accumulate the base(1) offset without changing the current skeleton pose:
   bframe.lcombtrans ( base(1)->offset() );
 }

void KnHoapIk::set_local ( GsMat& goal )
 {
   GsMat bframe;
   base_frame ( bframe );
   bframe.invert();
   GsMat local;
   local.mult ( goal, bframe ); // put goal matrix in local frame
   goal = local;
 }

// usually we get here precision of 10E-5 */
KnHoapIk::Result KnHoapIk::solve ( const GsMat& goal, float oang )
 {  
   /* Notation:
	  m = mid point in base frame
	  e = end point in base frame
	  c = center of the orbit circle in base frame
	  r = orbit circle radius
	  u,v = orbit angle frame axis
	  d1 = distance from the base to the mid joint
	  d2 = distance from the mid joint to the end joint
	  dist = distance from the base to the goal point
	  midr = the found flexion rotation of the mid joint
	  basest = vector where (x,y)=found swing and z=sound twist of the base joint
   */

   // End point:
   GsPnt e ( goal.e14, goal.e24, goal.e34 );

   //gsout<<"\nsolve:\n"<<goal;
   //gsout<<"d1:"<<_d1<<" d2:"<<_d2<<gsnl;

   if ( _type<2 && goal.e34<-1.0f ) return NotReachable; // arms will not reach most of -Z subspace

   // 1. get the lengths of the limbs and check goal distance:
   //	(offsets must be along one single axis!)
   float d1 = _d1;
   float d2 = _d2;
   float d2d2 = d2*d2;
   float d1d1 = d1*d1;
   float dist = e.len();
   float distdist = dist*dist;

   if ( dist>d1+d2 || dist==0 ) return NotReachable; // not reachable

   // 2. Find the flexion of the mid joint:
   float k = ( d1d1 + d2d2 - distdist ) / ( 2.0f * d2 * d1 );
   float midr = gspi - acosf( k ); // the 2 solutions are: gspi+-acos(k)

   GsVec axis;
   const float xlimin=-1.0f;
   const float xlimax=+10.0f;
   const float oanglim=GS_TORAD(25.0f);
   const float handr4=GS_TORAD(22.0f);

   if ( _type==LeftArm )
	{ if ( -e.x<=xlimin )
	   { oang=oanglim; _result[4]=-handr4; }
	  else if ( -e.x>=xlimax )
	   { oang=gspidiv2; _result[4]=-gspidiv2; }
	  else
	   { float t = ((-e.x-xlimin)/(xlimax-xlimin));
		 oang = gs_cubicmix ( oanglim, gspidiv2, t );
		 _result[4] = gs_cubicmix ( -handr4, -gspidiv2, t );
	   }
	  oang = gspidiv2-oang;
	  axis = GsVec::i;
	  midr = -midr;
	  axis = axis + e; // same as axis*goal, for a goal with id in the 3x3 submat
	}
   else if ( _type==RightArm )
	{ if ( e.x<=xlimin )
	   { oang=oanglim; _result[4]=handr4; }
	  else if ( e.x>=xlimax )
	   { oang=gspidiv2; _result[4]=gspidiv2; }
	  else
	   { float t = ((e.x-xlimin)/(xlimax-xlimin));
		 oang = gs_cubicmix ( oanglim, gspidiv2, t );
		 _result[4] = gs_cubicmix ( handr4, gspidiv2, t );
	   }
	  oang = oang-gspidiv2;
	  axis = -GsVec::i;
	  midr = -midr;
	  axis = axis + e; // same as axis*goal, for a goal with id in the 3x3 submat
	}
   else
	{ oang=0;
	  axis = GsVec::k;
	  axis = axis * goal; // project axis to orbit circle
	}

   axis.normalize();
   _result[3] = midr; // Store mid flexion

   // 3. Specify the orbit angle frame (u,v) and mid position m:
   GsVec n = e/dist; // n is the unit vector pointing from the base to the end joint
   GsVec u = axis + n*dot(-axis,n); // u is the projection of the Y axis in the mid's rotation circle
   u.normalize();
   GsVec v = cross ( u, n ); // v is orthogonal to n and u
   float cosa = (d2d2-d1d1-distdist) / (-2.0f*d1*dist);
   float sina = (d2*sinf(gspi-midr)) / dist;
   GsPnt c = (cosa*d1)*n; // c is the center of the circle in local coords
   float r = d1 * GS_ABS(sina); // r is the circle radius
   GsPnt m = c + r * ( u*cosf(oang) + v*sinf(oang) ); // mid pos in _base coords

   // Draw things if required:
//MatChange
   if ( _snlines )
	{ GsMat bframe;
	  base_frame ( bframe );
	  GsPnt basep ( bframe[12], bframe[13], bframe[14] ); // _base joint in global coords
	  GsMat bframe3x3 = bframe;
	  bframe3x3.setl4(0,0,0,1.0f);
	  n = e/dist; // unit vector pointing from the base to the end joint
	  SnLines& lines = *_snlines;
	  lines.init();
	  lines.push ( GsColor::cyan );
	  lines.push ( basep, e*bframe ); // _base-end line
	  lines.push ( GsColor::red );
	  lines.push ( c*bframe, (c+u)*bframe ); // u vector (red)
	  lines.push ( GsColor::green );
	  lines.push ( c*bframe, (c+v)*bframe ); // v vector (green)
	  lines.push ( GsColor::cyan );
	  lines.push_circle_approximation ( c*bframe, u*r*bframe3x3, n*bframe3x3, 60 ); // orbit circle
	  lines.push ( GsColor::cyan );
	  lines.push ( c*bframe, m*bframe ); // circenter-mid line
	  lines.push ( GsColor::magenta );
	  lines.push ( basep, m*bframe ); // _base-mid line
	  lines.push ( m*bframe, e*bframe );
	}

   GsMat M;
   float rx, ry, rz;

   // 4. Find the mid joint target frame:
   if ( _type==RightArm ) 
	{
	  GsVec mx = u*cosf(oang-gspidiv2) + v*sinf(oang-gspidiv2);
	  GsVec mz=m; mz.normalize();
	  M.setl1 ( mx );
	  M.setl2 ( cross(mx,mz) );
	  M.setl3 ( mz );
	  if ( (d1+d2-dist)<1.0E-5f ) 
	   { rx=ry=0; rz=GS_TORAD(30.0f); }
	  else
	   { gs_angles_zyx ( M, rx, ry, rz, 'C' ); } // decompose M in joint angles
	  _result[0] = rx;
	  _result[1] = ry;
	  _result[2] = -rz;
	}
   else if ( _type==LeftArm )
	{
	  GsVec mx = u*cosf(oang-gspidiv2) + v*sinf(oang-gspidiv2); 
	  GsVec mz=m; mz.normalize();
	  M.setl1 ( mx );
	  M.setl2 ( cross(mx,mz) );
	  M.setl3 ( mz );
	  if ( (d1+d2-dist)<1.0E-5f ) 
	   { rx=ry=0; rz=-GS_TORAD(30.0f); }
	  else
	   { gs_angles_zyx ( M, rx, ry, rz, 'C' ); } // decompose M in joint angles
	  _result[0] = rx;
	  _result[1] = ry;
	  _result[2] = -rz;
	}
   else
	{ GsVec mx=v; 
	  GsVec my=-m; my.normalize();
	  M.setl1 ( mx );
	  M.setl2 ( my );
	  M.setl3 ( cross(mx,my) );
	  gs_angles_xzy ( M, rx, ry, rz, 'C' ); // decompose M in joint angles
	  _result[0] = ry;
	  _result[1] = rx;
	  _result[2] = rz;
	}

   if ( _type==RightArm ) 
	{
	  //_result[4] = ...
	}
   else if ( _type==LeftArm ) 
	{
	  //_result[4] = ...
	}
   else if ( _solve_rot_goal )
	{ // 5. Find the end joint 2-DOF final rotation to match goal.
	  // Will now solve the following equation to find the end joint rotation E:
	  // E * T2 * R * T1 * M = goal  =>  E * X = goal  =>  E = goal * X^-1
	  GsMat R;  R.rotx ( midr );	   // mid joint flexion
	  GsMat T1; T1.setrans(0,-_d1,0);  // upper limb translation
	  GsMat T2; T2.setrans(0,-_d2,0);  // lower limb trnaslation
	  GsMat X = T2 * R * T1 * M;	   // composition of all known rotations
	  GsMat E = goal * X.inverse();	// final local rotation, must have no translation
	  gs_angles_yzx ( E, rx, ry, rz, 'C' ); // decompose E, ry must be zero
	  _result[4] = rx;
	  _result[5] = rz;
   }

   if ( !_joint[0]->euler()->inlimits(_axis[0],_result[0]) ) return NoBaseSwing;
   if ( !_joint[1]->euler()->inlimits(_axis[1],_result[1]) ) return NoBaseSwing;
   if ( !_joint[2]->euler()->inlimits(_axis[2],_result[2]) ) return NoBaseTwist;

   if ( !_joint[3]->euler()->inlimits(_axis[3],_result[3]) ) return NoMidFlexion;

   if ( _type<2 )
	{ if ( !_joint[4]->euler()->inlimits(_axis[4],_result[4]) ) return NoMidTwist; }
   else if ( _solve_rot_goal )
	{ if ( !_joint[4]->euler()->inlimits(_axis[4],_result[4]) ) return NoEndSwing; 
	  if ( !_joint[5]->euler()->inlimits(_axis[5],_result[5]) ) return NoEndSwing; 
	}

   //if (_type<2)gsout<<"IK OK\n";
   return Ok;
 }

KnHoapIk::Result KnHoapIk::solve ( const GsMat& goal, float oang, KnColdet* coldet )
 {
   Result res = solve ( goal, oang );
   if ( res!=Ok ) return res;
   apply_last_result ();
   if ( !coldet ) return res;
   coldet->update ( base()->skeleton() );
   return coldet->collide()? Collision:Ok;
 }

void KnHoapIk::get_sk_values ( float values[6] )
 {
   if ( !base() ) return;
   values[0] = _joint[0]->euler()->value ( _axis[0] );
   values[1] = _joint[1]->euler()->value ( _axis[1] );
   values[2] = _joint[2]->euler()->value ( _axis[2] );
   values[3] = _joint[3]->euler()->value ( _axis[3] );
   values[4] = _joint[4]->euler()->value ( _axis[4] );
   values[5] = _type<2? 0 : _joint[5]->euler()->value ( _axis[5] );
 }

void KnHoapIk::apply_values ( const float values[6] )
 {
   if ( !base() ) return;
   _joint[0]->euler()->value ( _axis[0], values[0] );
   _joint[1]->euler()->value ( _axis[1], values[1] );
   _joint[2]->euler()->value ( _axis[2], values[2] );
   _joint[3]->euler()->value ( _axis[3], values[3] );
   _joint[4]->euler()->value ( _axis[4], values[4] );
   if ( _solve_rot_goal&&( _type>=2 )) _joint[5]->euler()->value ( _axis[5], values[5] );
 }

void KnHoapIk::get_limits ( int i, float& lo, float& up )
 {
   _joint[i]->euler()->getlimits ( _axis[0], lo, up );
 }

const char* KnHoapIk::message ( Result res )
 {
   switch (res)
	{ case Ok : return "Ok";
	  case NotReachable : return "Not Reachable";
	  case NoBaseSwing : return "No Base Swing";
	  case NoBaseTwist : return "No Base Twist";
	  case NoMidFlexion : return "No Mid Flexion";
	  case NoMidTwist : return "No Mid Twist";
	  case NoEndSwing : return "No End Swing";
	  case Collision : return "Collision";
	  default: break; // it seems gcc needs this
	}
   return "Undef";
 }

//======================================= EOF =====================================

