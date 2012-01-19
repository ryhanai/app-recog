// -*- C++ -*-
/*!
 * @file  AppRecogConsumer.cpp * @brief AppRecog Consumer component * $Date$ 
 *
 * $Id$ 
 */
#include "AppRecogConsumer.h"
#include <rtm/CORBA_SeqUtil.h>
#include <vector>
#include <stdlib.h>
#include <coil/Async.h>
#include <functional>



// Module specification
// <rtc-template block="module_spec">
static const char* apprecogconsumer_spec[] =
  {
    "implementation_id", "AppRecogConsumer",
    "type_name",         "AppRecogConsumer",
    "description",       "AppRecog Consumer component",
    "version",           "1.0.0",
    "vendor",            "JSK",
    "category",          "Generic",
    "activity_type",     "SPORADIC",
    "kind",              "AppRecogConsumer",
    "max_instance",      "10",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    ""
  };
// </rtc-template>

AppRecogConsumer::AppRecogConsumer(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_RecognitionServicePort("RecognitionService")

    // </rtc-template>
{
}

AppRecogConsumer::~AppRecogConsumer()
{
}


RTC::ReturnCode_t AppRecogConsumer::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers

  // Set OutPort buffer

  // Set service provider to Ports

  // Set service consumers to Ports
  m_RecognitionServicePort.registerConsumer("recogservice0", "RecognitionService", m_recognitionservice0);

  // Set CORBA Service Ports
  addPort(m_RecognitionServicePort);

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable

  // </rtc-template>
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t AppRecogConsumer::onFinalize()
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t AppRecogConsumer::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t AppRecogConsumer::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t AppRecogConsumer::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t AppRecogConsumer::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

RTC::ReturnCode_t AppRecogConsumer::onExecute(RTC::UniqueId ec_id)
{
  try
    {
      std::cout << std::endl;
      std::cout << "Command list: " << std::endl;
      std::cout << " getModelID        : get current model ID." << std::endl;
      std::cout << " setModelID [value]: set model ID." << std::endl;
      std::cout << "> ";
      std::string args;
      std::string::size_type pos;
      std::vector<std::string> argv;
      std::getline(std::cin, args);

      pos = args.find_first_of(" ");
      if (pos != std::string::npos)
	{
	  argv.push_back(args.substr(0, pos));
	  argv.push_back(args.substr(++pos));
	}
      else
	{
	  argv.push_back(args);
	}
      std::cout << argv[0] << std::endl;
      if (argv[0] == "setModelID" && argv.size() > 1)
	{
	  CORBA::Float val(atof(argv[1].c_str()));
	  m_recognitionservice0->setModelID(val);
	  std::cout << "Set ModelID: " << val << std::endl;

	  return RTC::RTC_OK;
	}
      if (argv[0] == "getModelID")
	{
	  std::cout << "Current ModelID: "
		    << m_recognitionservice0->getModelID() << std::endl;
	  return RTC::RTC_OK;
	}

      std::cout << "Invalid command or argument(s)." << std::endl;

    }
  catch (...)
    {
      std::cout << "No service connected." << std::endl;
    }
  return RTC::RTC_OK;
}
/*
RTC::ReturnCode_t AppRecogConsumer::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t AppRecogConsumer::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t AppRecogConsumer::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t AppRecogConsumer::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t AppRecogConsumer::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


extern "C"
{
 
  void AppRecogConsumerInit(RTC::Manager* manager)
  {
    coil::Properties profile(apprecogconsumer_spec);
    manager->registerFactory(profile,
                             RTC::Create<AppRecogConsumer>,
                             RTC::Delete<AppRecogConsumer>);
  }
  
};



