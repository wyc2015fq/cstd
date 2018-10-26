#include "caffe.hpp"

int main(int argc, char** argv)
{
  LOG(FATAL) << "Deprecated. Use caffe test --model=... "
             "--weights=... instead.";
  return 0;
}
