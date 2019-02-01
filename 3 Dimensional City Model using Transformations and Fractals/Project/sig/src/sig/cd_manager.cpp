/*=======================================================================
   Copyright (c) 2014-2017 Marcelo Kallmann and contributors.
   This software is distributed under the Apache License, Version 2.0.
   All copies must contain the full copyright notice licence.txt located
   at the base folder of the distribution. 
  =======================================================================*/

# include <sig/cd_manager.h> 

//============================ DefaultImplementation =====================================

CdImplementation* CdManager::_defcdi = new CdImplementation; // empty implementaiton

void CdManager::set_default_implementation ( CdImplementation* cdi )
 {
   if ( _defcdi ) _defcdi->unref();
   _defcdi = cdi;
   _defcdi->ref ();
 }

//================================ CdManager =====================================

CdManager::CdManager ( CdImplementation* cdi )
 {
   _cdi = cdi? cdi:_defcdi;
   _cdi->ref();
 }
  
CdManager::~CdManager ()
 {
   _cdi->unref();
 }

void CdManager::set_implementation ( CdImplementation* cdi )
 {
   _cdi->unref();
   _cdi = cdi;
   _cdi->ref ();
 }

//=================================== EOF =====================================

