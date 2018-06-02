#ifndef CAFFE_UTIL_FORMAT_H_
#define CAFFE_UTIL_FORMAT_H_

  // NOLINT(readability/streams)
  // NOLINT(readability/streams)




inline std::string format_int(int n, int numberOfLeadingZeros = 0 ) {
  std::ostringstream s;
  s << std::setw(numberOfLeadingZeros) << std::setfill('0') << n;
  return s.str();
}


#endif   // CAFFE_UTIL_FORMAT_H_
