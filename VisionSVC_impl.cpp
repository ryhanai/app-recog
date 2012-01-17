// -*-C++-*-
/*!
 * @file  VisionSVC_impl.cpp
 * @brief Service implementation code of Vision.idl
 *
 */

#include "VisionSVC_impl.h"

/*
 * Example implementational code for IDL interface Reconstruct3DService
 */
Reconstruct3DServiceSVC_impl::Reconstruct3DServiceSVC_impl()
{
  // Please add extra constructor code here.
}


Reconstruct3DServiceSVC_impl::~Reconstruct3DServiceSVC_impl()
{
  // Please add extra destructor code here.
}


/*
 * Methods corresponding to IDL attributes and operations
 */
void Reconstruct3DServiceSVC_impl::reconstruct()
{
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <void Reconstruct3DServiceSVC_impl::reconstruct()>"
#endif
}



// End of example implementational code

/*
 * Example implementational code for IDL interface RecognitionService
 */
RecognitionServiceSVC_impl::RecognitionServiceSVC_impl()
{
  // Please add extra constructor code here.
  m_modelID = -1;
  _ModelID = -1;
}


RecognitionServiceSVC_impl::~RecognitionServiceSVC_impl()
{
  // Please add extra destructor code here.
}


/*
 * Methods corresponding to IDL attributes and operations
 */
::CORBA::Long RecognitionServiceSVC_impl::getModelID()
{
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <::CORBA::Long RecognitionServiceSVC_impl::getModelID()>"
#endif
  return _ModelID;
}

void RecognitionServiceSVC_impl::setModelID(::CORBA::Long ModelID)
{
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <void RecognitionServiceSVC_impl::setModelID(::CORBA::Long ModelID)>"
#endif
  _ModelID = ModelID;
  m_modelID = ModelID;
}



// End of example implementational code

/*
 * Example implementational code for IDL interface RecognitionResultViewerService
 */
RecognitionResultViewerServiceSVC_impl::RecognitionResultViewerServiceSVC_impl()
{
  // Please add extra constructor code here.
}


RecognitionResultViewerServiceSVC_impl::~RecognitionResultViewerServiceSVC_impl()
{
  // Please add extra destructor code here.
}


/*
 * Methods corresponding to IDL attributes and operations
 */
void RecognitionResultViewerServiceSVC_impl::display(const Img::TimedMultiCameraImage& frame, const TimedRecognitionResult& pos)
{
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <void RecognitionResultViewerServiceSVC_impl::display(const Img::TimedMultiCameraImage& frame, const TimedRecognitionResult& pos)>"
#endif
}



// End of example implementational code



