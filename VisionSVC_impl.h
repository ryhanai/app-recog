// -*-C++-*-
/*!
 * @file  VisionSVC_impl.h
 * @brief Service implementation header of Vision.idl
 *
 */

#include "VisionSkel.h"


#ifndef VISIONSVC_IMPL_H
#define VISIONSVC_IMPL_H
 
/*
 * Example class implementing IDL interface Reconstruct3DService
 */
class Reconstruct3DServiceSVC_impl
 : public virtual POA_Reconstruct3DService,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~Reconstruct3DServiceSVC_impl();

 public:
   // standard constructor
   Reconstruct3DServiceSVC_impl();
   virtual ~Reconstruct3DServiceSVC_impl();

   // attributes and operations
   void reconstruct();

};

/*
 * Example class implementing IDL interface RecognitionService
 */
class RecognitionServiceSVC_impl
 : public virtual POA_RecognitionService,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~RecognitionServiceSVC_impl();

 public:
  int m_modelID;
  ::CORBA::Long _ModelID;

   // standard constructor
   RecognitionServiceSVC_impl();
   virtual ~RecognitionServiceSVC_impl();

   // attributes and operations
   ::CORBA::Long getModelID();
   void setModelID(::CORBA::Long ModelID);

};

/*
 * Example class implementing IDL interface RecognitionResultViewerService
 */
class RecognitionResultViewerServiceSVC_impl
 : public virtual POA_RecognitionResultViewerService,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~RecognitionResultViewerServiceSVC_impl();

 public:
   // standard constructor
   RecognitionResultViewerServiceSVC_impl();
   virtual ~RecognitionResultViewerServiceSVC_impl();

   // attributes and operations
   void display(const Img::TimedMultiCameraImage& frame, const TimedRecognitionResult& pos);

};



#endif // VISIONSVC_IMPL_H


