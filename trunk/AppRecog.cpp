// -*- C++ -*-
/*!
 * @file AppRecog.cpp
 * @date $Date$
 *
 * $Id$
 */

#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>
#include "AppRecog.h"
//#include <boost/numeric/ublas/vector.hpp>
//#include <boost/numeric/ublas/io.hpp>

// typedef boost::numeric::ublas::vector<double> dvector;
// typedef boost::numeric::ublas::vector<double> dvector2;


using namespace cv;
using namespace std;

static const double PI = M_PIl;


int Model::loadModel(std::string fileName)
{
  std::string file("ModelFiles/");
  file += fileName;
  FileStorage fs(file, FileStorage::READ);

  if(fs.isOpened() == false)
    {
      cerr << "couldn't open " << file << endl;
      fs.release();
      return 1;
    }

  vertices_.clear();
  edges_.clear();
  circles_.clear();

  fs["ModelName"] >> modelName_;
  Mat vertices, edges, circles;
  fs["vertices"] >> vertices;
  fs["edges"] >> edges;
  fs["circles"] >> circles;

  if(vertices.rows > 0)
    {
      for(int i = 0; i < vertices.rows; i++)
        {
          vector<double> v;
          for(int j = 0; j < vertices.cols; j++)
            {
              v.push_back(vertices.at<double>(i,j));
            }
          vertice vv = vertice(v[0],v[1],v[2]);
	  vertices_.push_back(vv);
        }
    }

  if(edges.rows > 0)
    {
      for(int i = 0; i < edges.rows; i++)
	{
          vector<int> e;
          for(int j = 0; j < edges.cols; j++)
            {
              e.push_back(edges.at<int>(i,j));
            }
          edge ee = edge(e[0],e[1]);
	  edges_.push_back(ee);
	}
    }

  if(circles.rows > 0)
    {
      for(int i = 0; i < circles.rows; i++)
	{
          vector<double> c;
          for(int j = 0; j < circles.cols; j++)
            {
              c.push_back(circles.at<double>(i,j));
            }
          vertice cc = vertice(c[0],c[1],c[2]);
          circleEdge ce = circleEdge(cc,c[3]);
	  circles_.push_back(ce);
	}
    }

  fs.release();
  return 0;
}

Mat m_observation;
Model m_model;
Mat m_cameraMat;
Mat m_distCoeffs;

std::string AppearanceRecognition::ID2ModelName(int ModelID)
{
  std::ifstream ifs( "ModelFiles/ModelList.txt" );
  std::string str;
  string filename;
  while(getline(ifs, str)) {
    std::vector<std::string> results; //分割結果を格納する変数
    boost::split(results, str, boost::is_any_of(" ")); // 区切り
    stringstream ss;
    ss << results[0];
    int ID;
    ss >> ID;
    if (ModelID == ID)
      return results[1];

  }
  return "nothing";
}

bool AppearanceRecognition::recognize (Mat& observation, Mat& output,
				       Mat& rotation, Mat& translation,
				       bool debug)
{
    Mat hsvimg;
    cvtColor(observation, hsvimg, CV_BGR2HSV);
    std::vector<cv::Mat> planes;
    cv::split(hsvimg, planes);

    // Mat hedge;
    // adaptiveThreshold(planes[0], hedge, 255,
    // 		      ADAPTIVE_THRESH_GAUSSIAN_C,
    // 		      THRESH_BINARY, 5, 5);

    // int hedgeThresh = 10;
    // Mat chedge;
    // blur(planes[0], chedge, Size(3,3));
    // Canny(chedge, chedge, hedgeThresh, hedgeThresh*3, 3);
    // bitwise_not(chedge, chedge);

    int edgeThresh = 22;
    Mat iedge;

    if (edgeExtraction_ == "canny") {
	blur(planes[2], iedge, Size(3,3));
	Canny(iedge, iedge, edgeThresh, edgeThresh*3, 3);
	bitwise_not(iedge, iedge);
    } else {
      adaptiveThreshold(planes[2], iedge, 255,
			ADAPTIVE_THRESH_GAUSSIAN_C,
			THRESH_BINARY, 15, 7);
    }

    Mat dtf, dtf_norm;
    distanceTransform(iedge, dtf, CV_DIST_L2, 5);
    normalize(dtf, dtf_norm, 0.0, 255.0, CV_MINMAX, NULL);

    // GaussianBlur(ciedge, dtf_norm, Size(19,19), 19);
    // for (int y = 0; y < ciedge.rows; y++) {
    // 	for (int x = 0; x < ciedge.cols; x++) {
    // 	    double val1 = dtf_norm.data[dtf_norm.step*y + x];
    // 	    double val2 = ciedge.data[ciedge.step*y + x];
    // 	    dtf_norm.data[dtf_norm.step*y +x] = (val1+val2)/2;
    // 	}
    // }
    //add(dtf, ciedge, dtf_norm);
    //normalize(dtf_norm, dtf_norm, 0.0, 255.0, CV_MINMAX, NULL);

    State stmax;

    m_observation = dtf_norm;
    m_model = model_;
    m_cameraMat = cameraMat_;
    m_distCoeffs = distCoeffs_;

    extern unsigned long current_utime();
    unsigned long start = current_utime();

    double detect_threshold = 90.0;
    //double confidence = optimize_bruteforce(stmax, debug, detect_threshold);

    double confidence = optimize_pso(stmax, false, detect_threshold);
    Mat temp;
    estimatePose(stmax, model_, cameraMat_, distCoeffs_, temp, translation);

    cv::Rodrigues(temp, rotation);

    unsigned long end = current_utime();
    std::cout << "processing time: " << end - start << "[usec]" << std::endl;
    std::cout << "rotation: " << rotation << std::endl;
    std::cout << "translation: " << translation << std::endl;
    std::cout << "confidence: " << confidence << std::endl;
    std::cout << "scale: " << stmax.scale_ << endl;

    if (confidence > detect_threshold) {
      draw_result(observation, stmax, model_);
      draw_result(dtf_norm, stmax, model_);
    }

    if (debug) {
      imshow("intensity edge", iedge);
      imshow("distance transform", dtf_norm);
    }

    observation.copyTo(output);
    return confidence > detect_threshold;
}


#if 0
string getExtension(string s) {
    boost::regex r("\\.[^\\.]+$");
    boost::smatch m;

    if (boost::regex_search(s, m, r) ) {
        // cout << "found(pos =" << m.position() << endl;
        return m.str();
    }

    string empty_string = "";
    return empty_string;
}
#endif


static inline void trans(Point& p, State& st, Point& q) {
    double l = st.scale_;
    double theta = st.theta_;
    double c = cos(theta);
    double s = sin(theta);
    q.x = st.x_ + l*c*p.x - l*s*p.y;
    q.y = st.y_ + l*s*p.x + l*c*p.y;
}

static inline double eval_edge(Point& p0, Point& p1, Mat& dtf) {
    double d = 0.0;
    int c = dtf.channels();
    double l = sqrt(pow(p0.x-p1.x, 2) + pow(p0.y-p1.y, 2));
    int n = l / 12;
    double dx = (p1.x-p0.x)/n;
    double dy = (p1.y-p0.y)/n;
    for (int i = 0; i < n; i++) {
    	double x = p0.x + dx*i;
    	double y = p0.y + dy*i;
        if (0 <= y && y < dtf.rows && 0 <= x && x < dtf.cols) {
    	    double v = dtf.data[dtf.step * (int)y + (int)x * c];
    	    d -= v;
        }
    }
    return d/n;
}

static inline double eval_circle(Point& center, double r, State& st, Mat& dtf) {
  double d = 0.0;
  int c = dtf.channels();

  Point p, q;
  int n = 12;

  for (int i = 0; i < n; i++) {
    double theta = 2.0*PI*i/n;
    p.x = center.x + r*cos(theta);
    p.y = center.y + r*sin(theta);
    trans(p, st, q);
    double x = q.x;
    double y = q.y;
    if (0 <= y && y < dtf.rows && 0 <= x && x < dtf.cols) {
      double v = dtf.data[dtf.step * (int)y + (int)x * c];
      d -= v;
    }

  }

  return d/n;
}

double eval_likelihood(State& st) {
    double lk = 0.0;
    Point p0, p1, q0, q1;
    vector<vertice> vertices = m_model.vertices_;
    vector<edge> edges = m_model.edges_;

    for (vector<edge>::iterator it = edges.begin();
    	 it != edges.end(); it++) {

    	vertice v0 = vertices[it->get<0>()];
    	vertice v1 = vertices[it->get<1>()];
    	Point p0, p1;
    	p0.x = v0.get<0>(); p0.y = v0.get<1>();
    	p1.x = v1.get<0>(); p1.y = v1.get<1>();

    	// vector<Point3f> objectPoints;
    	// Mat rvec;
    	// Mat tvec;
    	// vector<Point2f> imagePoints;
    	// rvec[0] = 0; rvec[1] = 0; rvec[2] = 0;
    	// projectPoints(objectPoints, rvec, tvec, m_cameraMat, m_distCoeffs, imagePoints);

    	trans(p0, st, q0);
    	trans(p1, st, q1);
	lk += eval_edge(q0, q1, m_observation);
    }

    Point cp;
    for (vector<pair<vertice,double> >::iterator it = m_model.circles_.begin();
    	 it != m_model.circles_.end(); it++) {

      cp.x = it->first.get<0>();
      cp.y = it->first.get<1>();
      lk += eval_circle(cp, it->second, st, m_observation);
    }

    // cout << "LK: " << lk << endl;
    return max(100.0+lk, 10.0);
}

double eval_likelihood_caller(int num_vars, double x[]) {
    State st(x[0], x[1], x[2], x[3]);
    return eval_likelihood(st);
}

void draw_result(Mat& img, State& st, Model& model) {
    Point p0, p1, q0, q1;

    vector<vertice> vertices = model.vertices_;
    vector<edge> edges = model.edges_;

    for (vector<edge>::iterator it = edges.begin();
	 it != edges.end(); it++) {

	vertice v0 = vertices[it->get<0>()];
	vertice v1 = vertices[it->get<1>()];
	Point p0, p1;
	p0.x = v0.get<0>(); p0.y = v0.get<1>();
	p1.x = v1.get<0>(); p1.y = v1.get<1>();
	trans(p0, st, q0);
	trans(p1, st, q1);
	line(img, q0, q1, Scalar(255, 0, 0), 2);
    }

    Point cp, cq;
    for (vector<pair<vertice,double> >::iterator it = m_model.circles_.begin();
    	 it != m_model.circles_.end(); it++) {
      cp.x = it->first.get<0>();
      cp.y = it->first.get<1>();
      trans(cp, st, cq);
      double R = it->second * st.scale_;
      circle(img, cq, R, Scalar(0, 0, 255), 2);
    }
}

void loadCameraParameter(string calibFile, Mat& cameraMat, Mat& distCoeffs) {
    int width, height;
    FileStorage cvfs(calibFile, CV_STORAGE_READ);
    FileNode node(cvfs.fs, NULL);
    width = node["image_width"];
    height = node["image_height"];
    FileNode fn = node[string("camera_matrix")];
    read(fn[0], cameraMat);
    read(fn[1], distCoeffs);
}

void estimatePose(State st, Model model, cv::Mat cameraMat, cv::Mat distCoeffs, cv::Mat &rotate, cv::Mat &transform){
  vector<Point3f> objectPoints;
  vector<Point2f> imagePoints;

  vector<vertice> vertices = model.vertices_;
  for(uint i = 0; i < vertices.size(); i++)
    {
      Point3f temp;
      temp.x = vertices[i].get<0>();
      temp.y = vertices[i].get<1>();
      temp.z = vertices[i].get<2>();

      temp.y *= -1;

      objectPoints.push_back(temp);
    }
  cv::Mat OP(objectPoints);

  Point p,q;
  vertice v;
  for(uint i = 0; i < vertices.size(); i++)
    {
      v = vertices[i];
      p.x = v.get<0>(); p.y = v.get<1>();
      trans(p,st,q);
      imagePoints.push_back(q);
    }

  cv::Mat IP(imagePoints);

  solvePnP( OP, IP, cameraMat, distCoeffs, rotate, transform);
}

State optimize_bruteforce(bool debug) {
    //State st(140,350,-PI/6.0,2.1);
    State st;
    State stmax;
    double lkmax = -DBL_MAX;
    int mx = 50;
    int my = 40;
    int n = 90;
    int o = 0;
    //int cx = 240;
    //int cy = 320;
    int cx = 320;
    int cy = 240;
    int dx = 5;
    int dy = 5;

    for (int i = -mx; i <= mx; i++) {
	st.x_ = cx + i*dx;
	for (int j = -my; j <= my; j++) {
	    st.y_ = cy + j*dy;
	    for (int k = 0; k < n; k++) {
		st.theta_ = 2.0*PI*k/n;
		for (int l = -o; l <= o; l++) {
		    st.scale_ = 2.7*expf(0.04*l);
		    double lk = eval_likelihood(st);
		    if (lk > lkmax) {
			lkmax = lk; stmax = st;
		    }
		}
	    }
	}
    }

    cout << "MAX" << endl;
    cout << stmax.x_ << "," << stmax.y_ << ","
	 << stmax.theta_ << ", " << stmax.scale_ << "," << lkmax << endl;
    return stmax;
}

#include <ctime>
#include <cmath>
#include <queue>
#include "simpsolib.h"

using namespace simpsolib;

double optimize_pso_main(vector<double>& lower_range, vector<double>& upper_range, State& st, bool debug) {
    State stmax;
    int number_runs = 10; // 40 by default
    //ga parms
    // int ga_pop = 20;
    // int ga_iters = 100;
    //sh & sa parms
    // int sh_sa_iters = ga_pop * ga_iters;
    // int sh_sa_starts = 5;
    //pso parms
    int pso_pop = 40;
    int pso_iters = 100;
    float phi_p = 1.49445;
    float phi_g = 1.49445;
    float omega = 0.729;

    int nvars = lower_range.size();

    vector<double> result(nvars);
    simpsolib::EvalFN pso_eval_fn((char *)"likelihood", nvars, lower_range, upper_range, eval_likelihood_caller);
    double value = run_pso(pso_eval_fn, number_runs, pso_pop, pso_iters, phi_p, phi_g, omega, result, debug);

    st.x_ = result[0];
    st.y_ = result[1];
    st.theta_ = result[2];
    st.scale_ = result[3];
    return value;
}

double optimize_pso(State& st, bool debug, double thresh=85.0) {
    int nvars = 4;

    vector<double> lower_range(nvars);
    vector<double> upper_range(nvars);
    lower_range[0] = 180.0;
    upper_range[0] = 460.0;
    lower_range[1] = 100.0;
    upper_range[1] = 380.0;
    lower_range[2] = 0.0;
    upper_range[2] = PI;
    lower_range[3] = 2.75;
    upper_range[3] = 3.00;
    double value;

    // value = optimize_pso_main(lower_range, upper_range, st, debug);
    // if (value > threh) { return value; }

    lower_range[0] = 80.0;
    upper_range[0] = 560.0;
    lower_range[1] = 60.0;
    upper_range[1] = 420.0;
    value = optimize_pso_main(lower_range, upper_range, st, debug);
    if (value > thresh) { return value; }

    return value;
}


//#define BOOST_PYTHON_STATIC_LIB
//#include <boost/python.hpp>
//
//BOOST_PYTHON_MODULE( libpiece_recog )
//{
//    using namespace boost::python;
//    def("recognize", &recognize);
//}
