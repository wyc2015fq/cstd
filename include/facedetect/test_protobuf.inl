#include "cstd.h"
#define _SCL_SECURE_NO_WARNINGS

#define LINE printf("line %d\n", __LINE__)
#define andprintf printf
#include "protobuf/ServiceFaceRecognition.inl"
//int ServiceFaceRecognition(const char* m_strCurrentTerminalID, const char* m_strCurrentMallID, int num, facedata_t* f, char* buf, int maxbuflen);

#include "facedetect/rongyihttp.inl"

int test_protobuf() {
	faceobj_t f[100] = {0};
	buf_t bf[1] = { 0 };
	int len = ServiceFaceRecognition("aaaa", "bbbb", 100, f, bf);
#if 1
	const char* url = "http://192.168.10.194:28082/screen_detector_log?";
	str_t strResponse[1] = { 0 };
	int http_post_mem(const char* url, const HttpParam* cValueParam, int ValueParamNum, const uchar* postdata, int datalen, str_t* strResponse);
	HttpParam cValueParam[1] = { "Content-Type", "application/octet-stream" };
	http_post_mem(url, cValueParam, 1, (uchar*)bf->data, bf->len, strResponse);
	printf("strResponse: %s\n", strResponse->s);
#endif
	return 0;
}
