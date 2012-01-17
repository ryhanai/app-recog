// -*- C++ -*-
/*!
 *
 * THIS FILE IS GENERATED AUTOMATICALLY!! DO NOT EDIT!!
 *
 * @file VisionStub.h 
 * @brief Vision client stub header wrapper code
 * @date Thu Nov 24 17:14:25 2011 
 *
 */

#ifndef VISIONSTUB_H
#define VISIONSTUB_H



#include <rtm/config_rtc.h>
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#if   defined ORB_IS_TAO
#  include "VisionC.h"
#elif defined ORB_IS_OMNIORB
#  if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#    undef USE_stub_in_nt_dll
#  endif
#  include "Vision.hh"
#elif defined ORB_IS_MICO
#  include "Vision.h"
#elif defined ORB_IS_ORBIT2
#  include "Vision-cpp-stubs.h"
#else
#  error "NO ORB defined"
#endif

#endif // VISIONSTUB_H
