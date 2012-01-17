// -*- C++ -*-
/*!
 * @file  Vision.cpp * @brief Vision Manipulate component * $Date$ 
 *
 * $Id$ 
 */
#include "Vision.h"

// Module specification
// <rtc-template block="module_spec">
static const char* vision_spec[] =
  {
    "implementation_id", "Vision",
    "type_name",         "Vision",
    "description",       "Vision Manipulate component",
    "version",           "0.1",
    "vendor",            "JSK",
    "category",          "Generic",
    "activity_type",     "SPORADIC",
    "kind",              "Vision",
    "max_instance",      "10",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    ""
  };
// </rtc-template>

Vision::Vision(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_RecognitionServicePort("RecognitionService")

    // </rtc-template>
{
}

Vision::~Vision()
{
}


RTC::ReturnCode_t Vision::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers

  // Set OutPort buffer

  // Set service provider to Ports
  m_RecognitionServicePort.registerProvider("recognitionservice0", "RecognitionService", m_recognitionservice0);

  // Set service consumers to Ports

  // Set CORBA Service Ports
  addPort(m_RecognitionServicePort);

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable

  // </rtc-template>
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t Vision::onFinalize()
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t Vision::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


extern "C"
{
 
  void VisionInit(RTC::Manager* manager)
  {
    coil::Properties profile(vision_spec);
    manager->registerFactory(profile,
                             RTC::Create<Vision>,
                             RTC::Delete<Vision>);
  }
  
};



