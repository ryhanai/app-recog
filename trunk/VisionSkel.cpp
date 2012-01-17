// -*- C++ -*-
/*!
 *
 * THIS FILE IS GENERATED AUTOMATICALLY!! DO NOT EDIT!!
 *
 * @file VisionSkel.cpp 
 * @brief Vision server skeleton wrapper
 * @date Thu Nov 24 17:14:25 2011 
 *
 */

#include "VisionSkel.h"

#if defined ORB_IS_TAO
#  include "VisionC.cpp"
#  include "VisionS.cpp"
#elif defined ORB_IS_OMNIORB
#  include "VisionSK.cc"
#  include "VisionDynSK.cc"
#elif defined ORB_IS_MICO
#  include "Vision.cc"
#  include "Vision_skel.cc"
#elif defined ORB_IS_ORBIT2
#  include "Vision-cpp-stubs.cc"
#  include "Vision-cpp-skels.cc"
#else
#  error "NO ORB defined"
#endif

// end of VisionSkel.cpp
