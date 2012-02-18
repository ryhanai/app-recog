// -*- C++ -*-
/*!
 * @file AppRecog.h
 * @date $Date$
 *
 * $Id$
 */

#pragma once


#include <iostream>
#include <cv.h>
#include <highgui.h>


typedef boost::tuple<double,double,double> vertice;
typedef boost::tuple<int,int> edge;
typedef std::pair<vertice,double> circleEdge;


class State
{
public:
  State (double x, double y, double theta, double scale) 
  {
    x_ = x; y_ = y; theta_ = theta; scale_ = scale;
  }
  State () { State (0.0, 0.0, 0.0, 0.0); }

  double x_;
  double y_;
  double theta_;
  double scale_;
};

class Model {
public:
  Model() { }

  void load(double (*vertices)[3], int nvertices, int (*edges)[2], int nedges) 
  {
    for (int i = 0; i < nvertices; i++) 
      {
	vertices_.push_back(boost::make_tuple(vertices[i][0],
					      vertices[i][1],
					      vertices[i][2]));
      }
    for (int i = 0; i < nedges; i++) 
      {
	edges_.push_back(boost::make_tuple(edges[i][0],
					   edges[i][1]));
      }
  }



  int loadModel(std::string fileName);

  std::string modelName_;
  std::vector<vertice> vertices_;
  std::vector<edge> edges_;
  std::vector<circleEdge> circles_;
};


class AppearanceRecognition
{
private:
  std::string edgeExtraction_;
  cv::Mat cameraMat_;
  cv::Mat distCoeffs_;
  Model model_;

public:
  AppearanceRecognition()
  {

  }
  ~AppearanceRecognition()
  {

  }

  void setCameraMat(cv::Mat cameraMat)
  {
    cameraMat_ = cameraMat;
  };

  void setDistCoeffs(cv::Mat distCoeffs)
  {
    distCoeffs_ = distCoeffs;
  }

  void setEdgeExtraction(std::string edgeExtraction)
  {
    edgeExtraction_ = edgeExtraction;
  }

  std::string ID2ModelName(int ModelID);

  void loadObjFile(std::string filename)
  {
    model_.loadModel(filename);
  };

  bool recognize (cv::Mat& observation, cv::Mat& output,
		  cv::Mat& rotation, cv::Mat& translation,
		  double detection_threshold,
		  double xlbound, double xubound,
		  double ylbound, double yubound,
		  double slbound, double subound,
		  int debug_level);

};

double optimize_pso(State& st, bool debug, double thresh, 
		    double xlbound, double xubound, double ylbound, double yubound,
		    double slbound, double subound);

void estimatePose(State st, Model model, cv::Mat cameraMat, cv::Mat distCoeffs,
		  cv::Mat &rotate, cv::Mat &transform);

void draw_result(cv::Mat& img, State& st, Model& model);
