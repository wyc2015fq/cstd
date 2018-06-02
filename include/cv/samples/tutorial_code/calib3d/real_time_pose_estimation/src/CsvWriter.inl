#ifndef CSVWRITER_H
#define	CSVWRITER_H

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include "Utils.h"

using namespace std;
using namespace cv;

class CsvWriter {
public:
  CsvWriter(const string &path, const string &separator = " ");
  ~CsvWriter();
  void writeXYZ(const vector<CPoint3f> &list_points3d);
  void writeUVXYZ(const vector<CPoint3f> &list_points3d, const vector<CPoint2f> &list_points2d, const img_t &descriptors);

private:
  ofstream _file;
  string _separator;
  bool _isFirstTerm;
};

#endif
