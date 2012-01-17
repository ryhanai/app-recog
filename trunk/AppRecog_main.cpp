#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <time.h>

#include "AppRecog.h"

using namespace std;
using namespace cv;

extern unsigned long current_utime();

int main (int argc, char** argv) {
    // char* imgfile = argv[1];
    // Mat srcimg = imread(imgfile, 3);

    // Vec3f circle;
    // Mat gray_image;

    // cv::Mat dstimg;
    // cv::RotatedRect match_rect;
    // cv::Vec3f match_circle;

    // cv::Mat cameraMat;
    // cv::Mat distCoeffs;

    // std::string calib_file = "../script/camera_calib_lhand.yaml";
    // std::cout << "calib_file: " << calib_file << std::endl;

    // int width,height;
    // cv::FileStorage cvfs(calib_file, CV_STORAGE_READ);
    // cv::FileNode node(cvfs.fs, NULL); // Get Top Node
    // width = node["image_width"];
    // height = node["image_height"];
    // cv::FileNode fn = node[string("camera_matrix")];
    // cv::read(fn[0], cameraMat);
    // cv::read(fn[1], distCoeffs);

    // cv::Mat rotation, translation;
    // //mi_match_proc(srcimg, dstimg, match_rect, match_circle, 1, cameraMat, distCoeffs, "distance_transform", 7);
    // recognize(srcimg, dstimg, "partsA", cameraMat, distCoeffs, rotation, translation, "canny", true);

    // imshow("dst", dstimg);
    // waitKey(0);

    // cerr << "finished" << endl;
    return 0;
}
