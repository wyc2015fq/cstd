
#include "caffe/libcaffe.cpp"
#include "caffe/classification/libClassification.cpp"


#if 1
#ifdef _DEBUG
//#pragma comment(lib,"libprotocd.lib")
#pragma comment(lib,"libprotobufd.lib")
#pragma comment(lib,"libprotobuf-lited.lib")
#pragma comment(lib,"libopenblas.lib")
#else
//#pragma comment(lib,"libprotocd.lib")
#pragma comment(lib,"libprotobuf.lib")
#pragma comment(lib,"libprotobuf-lite.lib")
#pragma comment(lib,"libopenblas.lib")
#endif
#endif
cv::fastFree;