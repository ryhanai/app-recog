// -*-C++-*-
/*!
 * @file  ImgSVC_impl.cpp
 * @brief Service implementation code of Img.idl
 *
 */

#include "ImgSVC_impl.h"

/*
 * Example implementational code for IDL interface Img::CameraCaptureService
 */
CameraCaptureServiceSVC_impl::CameraCaptureServiceSVC_impl()
 : counter(0)
{
  // Please add extra constructor code here.
}


CameraCaptureServiceSVC_impl::~CameraCaptureServiceSVC_impl()
{
  // Please add extra destructor code here.
}


/*
 * Methods corresponding to IDL attributes and operations
 */
void CameraCaptureServiceSVC_impl::take_one_frame()
{
  counter = 1; 
}

void CameraCaptureServiceSVC_impl::take_multi_frames(CORBA::Long num)
{
  counter = num;
}

void CameraCaptureServiceSVC_impl::start_continuous()
{
  counter = -1;
}

void CameraCaptureServiceSVC_impl::stop_continuous()
{
  counter = 0;
}



// End of example implementational code



