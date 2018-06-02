
#include "faceplusplus.inl"

int test_faceplusplus()
{
  const char* file = "F:/pub/bin/img2/lena.jpg";
  faceobj_t fa[1] = {0};
	curl_global_init(CURL_GLOBAL_ALL);
  int ret = faceplusplus_file(file, fa);
	return 0;
}
