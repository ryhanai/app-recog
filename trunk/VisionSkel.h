// -*- C++ -*-
/*!
 *
 * THIS FILE IS GENERATED AUTOMATICALLY!! DO NOT EDIT!!
 *
 * @file VisionSkel.h 
 * @brief Vision server skeleton header wrapper code
 * @date Thu Nov 24 17:14:25 2011 
 *
 */

#ifndef VISIONSKEL_H
#define VISIONSKEL_H



#include <rtm/config_rtc.h>
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#if   defined ORB_IS_TAO
#  include "VisionC.h"
#  include "VisionS.h"
#elif defined ORB_IS_OMNIORB
#  if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#    undef USE_stub_in_nt_dll
#  endif
#  include "Vision.hh"
#elif defined ORB_IS_MICO
#  include "Vision.h"
#elif defined ORB_IS_ORBIT2
#  include "/Vision-cpp-stubs.h"
#  include "/Vision-cpp-skels.h"
#else
#  error "NO ORB defined"
#endif

#endif // VISIONSKEL_H
