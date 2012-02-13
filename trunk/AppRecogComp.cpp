// -*- C++ -*-
/*!
 * @file AppRecogCo_p.cpp
 * @date $Date$
 *
 * $Id$
 */

#include <rtm/Manager.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "AppRecogComp.h"
#include <sys/time.h>

using namespace cv;

void MyModuleInit(RTC::Manager* manager)
{
  AppRecogInit(manager);
  RTC::RtcBase* comp;
  // Create a component
  comp = manager->createComponent("AppRecog");

  if (comp==NULL)
    {
      std::cerr << "Component create failed." << std::endl;
      abort();
    }

  return;
}

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
AppRecog::AppRecog(RTC::Manager* manager)
  // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_originalTImageIn("InputImage", m_originalTImage),
    m_outTImageOut("ResultImage", m_outTImage),
    m_RecognitionServicePort("Recognition"),
    TimedRecognitionResultOut("RecognitionResult", RecogResult)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
AppRecog::~AppRecog()
{
}


// Module specification
// <rtc-template block="module_spec">
static const char* apprecog_spec[] =
  {
    "implementation_id", "AppRecog",
    "type_name",         "AppRecog",
    "description",       "2D recognition using edges",
    "version",           "1.0.0",
    "vendor",            "JSK",
    "category",          "recognition",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "0",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.window_name", "Recognition result",
    "conf.default.calib_file", "camera_calib.yml",
    "conf.default.model_file", "parts.yml",
    "conf.default.adaptive_threshold_param", "5",
    "conf.default.detection_threshold", "90.0",
    "conf.default.x_lbound", "80.0",
    "conf.default.x_ubound", "560.0",
    "conf.default.y_lbound", "60.0",
    "conf.default.y_ubound", "420.0",
    "conf.default.scale_lbound", "2.75",
    "conf.default.scale_ubound", "3.00",
    "conf.default.debug_level", "0",
    // Widget
    // Constraints
    ""
  };
// </rtc-template>



RTC::ReturnCode_t AppRecog::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("InputImage", m_originalTImageIn);

  // Set OutPort buffer
  addOutPort("ResultImage", m_outTImageOut);
  addOutPort("RecognitionResult", TimedRecognitionResultOut);

  // Set service provider to Ports
  m_RecognitionServicePort.registerProvider("recogservice0", "RecognitionService", m_recogservice0);

  // Set service consumers to Ports

  // Set CORBA Service Ports
  addPort(m_RecognitionServicePort);

  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("window_name", m_window_name, "Recognition result");
  bindParameter("calib_file", m_calib_file, "camera_calib.yml");
  bindParameter("model_file", modelFile, "parts.yml");
  bindParameter("adaptive_threshold_param", m_adaptive_threshold_param, "5");
  bindParameter("detection_threshold", m_detection_threshold, "90.0");
  bindParameter("x_lbound", m_x_lbound, "80.0");
  bindParameter("x_ubound", m_x_ubound, "560.0");
  bindParameter("y_lbound", m_y_lbound, "60.0");
  bindParameter("y_ubound", m_y_ubound, "420.0");
  bindParameter("scale_lbound", m_scale_lbound, "2.75");
  bindParameter("scale_ubound", m_scale_ubound, "3.00");
  bindParameter("debug_level", m_debug_level, "0");


  // </rtc-template>
  return RTC::RTC_OK;
}

/*
  RTC::ReturnCode_t AppRecog::onFinalize()
  {
  return RTC::RTC_OK;
  }
*/

/*
  RTC::ReturnCode_t AppRecog::onStartup(RTC::UniqueId ec_id)
  {
  return RTC::RTC_OK;
  }
*/

/*
  RTC::ReturnCode_t AppRecog::onShutdown(RTC::UniqueId ec_id)
  {
  return RTC::RTC_OK;
  }
*/


RTC::ReturnCode_t AppRecog::onActivated(RTC::UniqueId ec_id)
{
  m_imageBuff = NULL;
  m_outImageBuff = NULL;

  std::cout << "calib_file: " << m_calib_file << std::endl;
  std::cout << "model_file_name: " << modelFile << std::endl;

  int width,height;
  cv::FileStorage cvfs(m_calib_file, cv::FileStorage::READ);

  if(!cvfs.isOpened())
    {
      RTC_ERROR(("Unable to open camera parameter file. [%s]",
                 m_calib_file.c_str()));
      std::cerr << "Could not open camera parameter." << std::endl;
      return RTC::RTC_ERROR;
    }
#if CV_MAJOR_VERSION == 2 && CV_MINOR_VERSION >= 2
  cvfs["image_width"] >> width;
  cvfs["image_height"] >> height;
  cvfs["camera_matrix"] >> m_cameraMat;
  cvfs["distortion_coefficients"] >> m_distCoeffs;
#else  // CV_MAJOR_VERSION == 2 && CV_MINOR_VERSION >= 2
  cv::FileNode node(cvfs.fs, NULL);
  width = node["image_width"];
  height = node["image_height"];
  cv::FileNode fn = node["camera_matrix"];
  cv::read(fn, m_cameraMat);
  fn = node["distortion_coefficients"];
  cv::read(fn, m_distCoeffs);
#endif  // CV_MAJOR_VE

  ar.loadObjFile(modelFile);

  // (4)print loaded data
  // cout << "width:" << width << endl;
  // cout << "height:" << height << endl;
  // cout << "camera_matrix:" << endl;
  // cout << cameraMat.cols << endl;
  // for(i=0;i<cameraMat.rows;i++){
  //   for(j=0;j<cameraMat.cols;j++){
  //     cout << cameraMat.at<float>(i,j) << ",";
  //   }
  //   cout << endl;
  // }
  // for(i=0;i<distCoeffs.rows;i++){
  //   for(j=0;j<distCoeffs.cols;j++){
  //     cout << distCoeffs.at<float>(i,j) << ",";
  //   }
  //   cout << endl;
  // }

  // char filename[] = "camera_calib_rhand.yaml";  // file name
  // // (1)create sample data
  // int width = 640;
  // float height = 480;
  // // (2)open file storage
  // cv::FileStorage   cvfs(filename,CV_STORAGE_WRITE);
  // // (3)write data to file
  // cv::write(cvfs,"width",width);
  // cv::write(cvfs,"height",height);
  // cv::WriteStructContext ws(cvfs, "camera_matrix", CV_NODE_SEQ);    // create node
  // cv::write(cvfs,"",cameraMat);
  // //cv::WriteStructContext ws(cvfs, "distortion_coefficients", CV_NODE_SEQ);    // create node
  // cv::write(cvfs,"",distCoeffs);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t AppRecog::onDeactivated(RTC::UniqueId ec_id)
{

  if(m_imageBuff != NULL){
    cvReleaseImage(&m_imageBuff);
    cvReleaseImage(&m_outImageBuff);
  }
  return RTC::RTC_OK;
}


RTC::ReturnCode_t AppRecog::onExecute(RTC::UniqueId ec_id)
{

  if (m_originalTImageIn.isNew()) {

    m_originalTImageIn.read();
    int width = m_originalTImage.data.image.width;
    int height = m_originalTImage.data.image.height;
    int nChannels = 3;

    // InPortとOutPortの画面サイズ処理およびイメージ用メモリの確保
    if( m_originalTImage.data.image.width != m_outTImage.data.image.width || m_originalTImage.data.image.height != m_outTImage.data.image.height)
      {
	m_outTImage.data.image.width = width;
	m_outTImage.data.image.height = height;

	// InPortのイメージサイズが変更された場合
	if(m_imageBuff != NULL)
	  {
	    cvReleaseImage(&m_imageBuff);
	    cvReleaseImage(&m_outImageBuff);
	  }
	m_imageBuff = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	m_outImageBuff = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
      }
    int c = 0;
    for (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
	for (int d = 0; d < nChannels; d++, c++){
	  const int off = y * m_imageBuff->widthStep + x * m_imageBuff->nChannels + (m_imageBuff->nChannels - d - 1);
	  m_imageBuff->imageData[off] = m_originalTImage.data.image.raw_data[c];
	}
      }
    }

    //set camera parameter
    if(m_originalTImage.data.intrinsic.matrix_element[0] != 0)
      {
	for (int i = 0; i < 5; i++)
	  {
	    m_outTImage.data.intrinsic.matrix_element[i]
	      = m_originalTImage.data.intrinsic.matrix_element[i];
	  }

	m_cameraMat = Mat::zeros(3,3,CV_32FC1);
	m_cameraMat.at<float>(0,0) = m_originalTImage.data.intrinsic.matrix_element[0];
	m_cameraMat.at<float>(0,1) = m_originalTImage.data.intrinsic.matrix_element[1];
	m_cameraMat.at<float>(1,1) = m_originalTImage.data.intrinsic.matrix_element[2];
 	m_cameraMat.at<float>(0,2) = m_originalTImage.data.intrinsic.matrix_element[3];
	m_cameraMat.at<float>(1,2) = m_originalTImage.data.intrinsic.matrix_element[4];
      }

    int dist_size = m_originalTImage.data.intrinsic.distortion_coefficient.length();

    if(dist_size != 0)
      {
	m_outTImage.data.intrinsic.distortion_coefficient.length(dist_size);
	for(int l = 0 ; l < dist_size; l++)
	  {
	    m_outTImage.data.intrinsic.distortion_coefficient[l] = m_originalTImage.data.intrinsic.distortion_coefficient[l];
	  }
	m_distCoeffs = Mat::zeros(1,dist_size, CV_32FC1);
	for(int l = 0 ; l < dist_size; l++)
	  {
	    m_distCoeffs.at<float>(0,l) = m_originalTImage.data.intrinsic.distortion_coefficient[l];
	  }
      }

    cv::Mat srcimg, dstimg;
    cv::Mat img(m_imageBuff);
    srcimg = m_imageBuff;
    cv::Mat rotation, translation;
    if(m_recogservice0.m_modelID != -1)
      {
	std::string ModelNameFile = ar.ID2ModelName(m_recogservice0.m_modelID);
	ar.loadObjFile(ModelNameFile);
	std::cout << "loadFile: " << ModelNameFile << std::endl;
	m_recogservice0.m_modelID = -1;
      }

    ar.setCameraMat(m_cameraMat);
    ar.setDistCoeffs(m_distCoeffs);
    ar.setEdgeExtraction("adaptive threshold");
    ar.recognize (srcimg, dstimg, rotation, translation,
    		  m_detection_threshold,
    		  m_x_lbound, m_x_ubound, m_y_lbound, m_y_ubound,
    		  m_scale_lbound, m_scale_ubound,
    		  m_debug_level>=1);
    cv::Vec3d v_transform(translation);

    vector<double> seq;
    int index = 0;
    for(int i = 0; i < 9; i++){
      seq.push_back(0);
    }

    for(int i = 0; i < rotation.rows ; i++)
      {
    	for(int j = 0; j < rotation.cols; j++)
    	  {
    	    seq[index] = rotation.at<double>(i,j);
    	    index++;
    	  }
      }
    // for(int i = 0; i< (int)seq.size() ; i++)
    //   {
    // 	std::cout << seq[i] << std::endl;
    //   }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    RecogResult.tm.sec = tv.tv_sec;
    RecogResult.tm.nsec = tv.tv_usec*1000;
    RecogResult.data.length(20);
    for(int i = 0; i < 20 ;i++)
      RecogResult.data[i] = 0;
    RecogResult.data[8] = seq[0];
    RecogResult.data[9] = seq[1];
    RecogResult.data[10] = seq[2];
    RecogResult.data[11] = v_transform[0];
    RecogResult.data[12] = seq[3];
    RecogResult.data[13] = seq[4];
    RecogResult.data[14] = seq[5];
    RecogResult.data[15] = v_transform[1];
    RecogResult.data[16] = seq[6];
    RecogResult.data[17] = seq[7];
    RecogResult.data[18] = seq[8];
    RecogResult.data[19] = v_transform[2];


    IplImage temp = dstimg;
    m_outImageBuff = &temp;

    //TimedCameraImage

    m_outTImage.data.image.format = Img::CF_RGB;
    m_outTImage.data.image.width = m_outImageBuff->width;
    m_outTImage.data.image.height = m_outImageBuff->height;
    const int t_num = m_outImageBuff->width * m_outImageBuff->height * m_outImageBuff->nChannels;
    m_outTImage.data.image.raw_data.length(t_num);
    c = 0;
    for (int y = 0; y < m_outImageBuff->height; y++){
      for (int x = 0; x < m_outImageBuff->width; x++){
	for (int d = 0; d < m_outImageBuff->nChannels; d++, c++){
	  const int off = y * m_outImageBuff->widthStep + x * m_outImageBuff->nChannels + (m_outImageBuff->nChannels - d - 1);
	  m_outTImage.data.image.raw_data[c] = (uchar) m_outImageBuff->imageData[off];
	}
      }
    }

    m_outTImageOut.write();
    TimedRecognitionResultOut.write();

    imshow(m_window_name, dstimg);
    waitKey(10);
  }

  return RTC::RTC_OK;
}

/*
  RTC::ReturnCode_t AppRecog::onAborting(RTC::UniqueId ec_id)
  {
  return RTC::RTC_OK;
  }
*/

/*
  dsRTC::ReturnCode_t AppRecog::onError(RTC::UniqueId ec_id)
  {
  return RTC::RTC_OK;
  }
*/

/*
  RTC::ReturnCode_t AppRecog::onReset(RTC::UniqueId ec_id)
  {
  return RTC::RTC_OK;
  }
*/

/*
  RTC::ReturnCode_t AppRecog::onStateUpdate(RTC::UniqueId ec_id)
  {
  return RTC::RTC_OK;
  }
*/

/*
  RTC::ReturnCode_t AppRecog::onRateChanged(RTC::UniqueId ec_id)
  {
  return RTC::RTC_OK;
  }
*/


extern "C"
{

  void AppRecogInit(RTC::Manager* manager)
  {
    coil::Properties profile(apprecog_spec);
    manager->registerFactory(profile,
                             RTC::Create<AppRecog>,
                             RTC::Delete<AppRecog>);
  }

};


int main (int argc, char** argv)
{
  RTC::Manager* manager;
  manager = RTC::Manager::init(argc, argv);

  // Initialize manager
  manager->init(argc, argv);

  // Set module initialization proceduer
  // This procedure will be invoked in activateManager() function.
  manager->setModuleInitProc(MyModuleInit);

  // Activate manager and register to naming service
  manager->activateManager();

  // run the manager in blocking mode
  // runManager(false) is the default.
  manager->runManager();

  // If you want to run the manager in non-blocking mode, do like this
  // manager->runManager(true);

  return 0;
}
