/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/sn_transform.h>

//# define GS_USE_TRACE1  // Const/Dest
# include <sig/gs_trace.h>

//======================================= SnTransform ====================================

const char* SnTransform::class_name = "SnTransform";

SnTransform::SnTransform ()
			:SnNode ( SnNode::TypeTransform, SnTransform::class_name )
 {
   GS_TRACE1 ( "Constructor" );
 }

SnTransform::SnTransform ( const GsMat& m )
			:SnNode ( SnNode::TypeTransform, SnTransform::class_name )
 {
   GS_TRACE1 ( "Constructor from GsMat" );
   _mat = m;
 }

SnTransform::~SnTransform ()
 {
   GS_TRACE1 ( "Destructor" );
 }

//======================================= EOF ====================================

