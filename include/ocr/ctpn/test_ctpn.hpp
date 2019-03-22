/*************************************************************************
    > File Name: test.cpp
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Thu 02 Mar 2017 09:47:58 AM CST
 ************************************************************************/

#include <iostream>
#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/net.hpp"
//#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
//#include "caffe/util/format.hpp"
#include "caffe/util/io.hpp"

#include "detector.h"

using caffe::Blob;
using caffe::Caffe;
using caffe::Datum;
using caffe::Net;

using namespace cv;
using namespace std;

int test_ctpn(){
	const char* fn = "E:/OCR_Line/demo_images/032.jpg";
	Mat img = imread(fn);
	vector< vector<float> > tline;
	int len = detect_tline(img, tline);
	return 0;
}
