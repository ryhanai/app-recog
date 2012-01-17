// -*-C++-*-
/*!
 * @file  ImgSVC_impl.h
 * @brief Service implementation header of Img.idl
 *
 */

#include "BasicDataTypeSkel.h"

#include "ImgSkel.h"

#ifndef IMGSVC_IMPL_H
#define IMGSVC_IMPL_H
 
/*!
 * @class CameraCaptureServiceSVC_impl
 * Example class implementing IDL interface Img::CameraCaptureService
 */
class CameraCaptureServiceSVC_impl
 : public virtual POA_Img::CameraCaptureService,
   public virtual PortableServer::RefCountServantBase
{
 private:
   CORBA::Long counter;
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~CameraCaptureServiceSVC_impl();

 public:
  /*!
   * @brief standard constructor
   */
  CameraCaptureServiceSVC_impl();
  /*!
   * @brief destructor
   */
   virtual ~CameraCaptureServiceSVC_impl();

   // attributes and operations
   void take_one_frame();
   void take_multi_frames(CORBA::Long num);
   void start_continuous();
   void stop_continuous();
	
   CORBA::Long GetCounter() const { return counter; }
   void DecrimentCounter(){ 	
     if (counter > 0){ counter--; } 
   }
};



#endif // IMGSVC_IMPL_H


