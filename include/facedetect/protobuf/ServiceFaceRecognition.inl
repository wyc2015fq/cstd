
#ifndef _SERVICEFACERECOGNITION_INL_
#define _SERVICEFACERECOGNITION_INL_

#define _SCL_SECURE_NO_WARNINGS
#undef min
#undef max
#ifdef __linux__
#include "./2.6.1/universal_report.pb.h"
#include "./2.6.1/universal_report.pb.cc"
#include "./2.6.1/human_face_report.pb.h"
#include "./2.6.1/human_face_report.pb.cc"
#else
#include "./2.7.0/universal_report.pb.h"
#include "./2.7.0/universal_report.pb.cc"
#include "./2.7.0/human_face_report.pb.h"
#include "./2.7.0/human_face_report.pb.cc"
#endif

#if 0
#include "libprotobuf1.inl"
#else
#pragma comment(lib,"libprotobuf.lib")
#pragma comment(lib,"libprotoc.lib")
#pragma comment(lib,"libprotobuf-lite.lib")
#endif


//#define LINE printf("line %d\n", __LINE__);
#define LOGOUT_INFO printf
#define LOGOUT_ERR  printf

#include "cstd.h"
#include <string>
#include <sstream>
using namespace std;

#include "facelib/facedetect/facedetect.h"

#include "Base64.inl"

#ifdef __linux__
typedef int64_t int64;
int64 GetMsecStamp()
{
  struct timeval tv;
  struct timezone tz;
  gettimeofday (&tv , &tz);  
  return tv.tv_sec * 1000 + tv.tv_usec/1000;
}
#else
#include <sys/timeb.h>
int64 GetMsecStamp()
{
	struct timeb rawtime;
	ftime(&rawtime);
	return rawtime.time * 1000 + rawtime.millitm;
}
#endif

//#include "human_face_report_performance_test.cc"

int test_protobuf1()
{
	char userFaceInfoStr[10] = { 0 };
	::rongyi::report::UniversalReportRequest universalReportRequest;
	int len1 = 1;
	LINE; printf("len1 = %d\n", len1);
	universalReportRequest.add_message_data(userFaceInfoStr, len1);
	std::string userFaceInfoStr1 = "1";
	universalReportRequest.add_message_data(userFaceInfoStr1);
	LINE;
	int len = universalReportRequest.ByteSize();
	LINE;
	char* data = (char*)malloc(len + 10);
	LINE;
	universalReportRequest.SerializeToArray(data, len);
	LINE;
	free(data);
	return len;
}

int test_protobuf2()
{
	char userFaceInfoStr[10] = { 0 };
	::rongyi::report::UniversalReportRequest universalReportRequest;
	int len1 = 0;
	universalReportRequest.add_message_data(userFaceInfoStr, len1);
	int len = universalReportRequest.ByteSize();
	char* data = (char*)malloc(len + 10);
	universalReportRequest.SerializeToArray(data, len);
	free(data);
	return len;
}

int ServiceFaceRecognition(const char* terminal_mac, const char* MallID, int num, faceobj_t* f, buf_t* bf)
{
	//test_protobuf1();
	static int64 int64Index = 0;
	string strBase64Out;

	char tempBuf[32] = {0};

	::rongyi::face::log::UserFaceInfo userFaceInfo;
	::rongyi::face::log::CapturedUserFace *captureUserFace = NULL;
	::rongyi::face::log::UserProfile *userProfile = NULL;
	::rongyi::face::log::ReportID  reportId;

	int64 msec = GetMsecStamp();

	reportId.set_terminal_mac(terminal_mac);
	reportId.set_timestamp(msec);
	reportId.set_index(int64Index);
	++int64Index;

	CBase64_Encode(reportId.SerializeAsString(), strBase64Out);

	userFaceInfo.set_timestamp(msec);
	userFaceInfo.set_mall_id(MallID);
	userFaceInfo.set_terminal_mac(terminal_mac);
	LOGOUT_INFO("strId[%s]", strBase64Out.c_str());	
	userFaceInfo.set_id(strBase64Out);

	int iIndex = 0;
	for (iIndex; iIndex < num; ++iIndex, ++f) {
		userProfile = new ::rongyi::face::log::UserProfile();
		captureUserFace = userFaceInfo.add_face();

		userProfile->set_begin_age(f->age);
		userProfile->set_end_age(f->age);
		userProfile->set_gender((f->gender == -1) ?
			::rongyi::face::log::GENDER_UNKNOWN
			: (::rongyi::face::log::Gender)f->gender);

		memset(tempBuf, 0x00, sizeof(tempBuf));
		//_itoa(f->rowId, tempBuf, 10);
    sprintf(tempBuf, "%d", f->id);  
		userProfile->set_face_code(tempBuf);

		userProfile->set_expression((f->smile == -1) ?
			::rongyi::face::log::UserExpressionUnknown 
			: (::rongyi::face::log::UserExpression)f->smile);
		userProfile->set_wear_glasses(f->glass);
		userProfile->set_age_confidence(-1);
		userProfile->set_gender_confidence(-1);
		userProfile->set_expression_confidence(-1);

		captureUserFace->set_allocated_profile(userProfile);

		captureUserFace->set_enter_time(f->enter_time);
		captureUserFace->set_leave_time(f->left_time);

    captureUserFace->set_position(f->sideface<0
      ? ::rongyi::face::log::FacePositionOthers
      : (::rongyi::face::log::FacePosition)(f->sideface));

		// TODO: parese image
		captureUserFace->set_face_image(f->image->data, f->image->len);
		captureUserFace->set_image_mime_type("image/jpeg");
		
		// if the the size of serialize of userFaceInfo exceed 1M, need brak
		int iSize = userFaceInfo.SerializeAsString().size();
		//LOGOUT_INFO("userFaceInfo data len [%d]\n", iSize);
		if (iSize > (1 << 20)) { 
			userFaceInfo.mutable_face()->RemoveLast();
			iSize = userFaceInfo.SerializeAsString().size();
			//LOGOUT_INFO("userFaceInfo data len [%d] exceed 1M\n", iSize);
			break;
		}

		//printf("%d", f->rowId);		rowIdFields = rowIdFields + ss.str() + ",";		ss.str("");
	}

  int len = 0;
	::rongyi::report::UniversalReportRequest universalReportRequest;
  len = userFaceInfo.ByteSize();
#if 1
  //LINE;
	char* userFaceInfoStr = (char*)malloc(len);
	userFaceInfo.SerializeToArray(userFaceInfoStr, len);
	//bool bTT = userFaceInfo.ParseFromString(userFaceInfoStr);
	//LOGOUT_INFO("test xxx[%d].", bTT);
	
  //LINE;
	universalReportRequest.set_id(strBase64Out);
	universalReportRequest.set_timestamp(GetMsecStamp());
	universalReportRequest.set_message_name("rongyi.face.log.UserFaceInfo");
  //LINE;
	universalReportRequest.add_message_data(userFaceInfoStr, len);
  //LINE;
  free(userFaceInfoStr);
#else
	string userFaceInfoStr;
	userFaceInfo.SerializeToString(&userFaceInfoStr);
	//bool bTT = userFaceInfo.ParseFromString(userFaceInfoStr);
	//LOGOUT_INFO("test xxx[%d].", bTT);
	
  LINE;
	universalReportRequest.set_id(strBase64Out);
	universalReportRequest.set_timestamp(GetMsecStamp());
	universalReportRequest.set_message_name("rongyi.face.log.UserFaceInfo");
	universalReportRequest.add_message_data(userFaceInfoStr);
#endif

  LINE;
  len = universalReportRequest.ByteSize();
  bfsetsize(bf, len);
	universalReportRequest.SerializeToArray(bf->data, len);

#if 0
  LINE;
	string strUrl = "http://192.168.10.194:28080/screen_detector_log?";
	string strResponse;
	RongyiCHttpClient http;
	int ret = http.PostSpcifyHead(strUrl,
		serializedStr,
		strResponse,
		"Content-Type: application/octet-stream");
	if (0 != ret) {
		LOGOUT_ERR("url:[%s], ret:[%d]", strUrl.c_str(), ret);
		return;
	}

	strBase64Out.clear();
	CBase64_Encode(serializedStr, strBase64Out);
	LOGOUT_INFO("the base64 is [%s]", strBase64Out.c_str());
#endif
	return len;
}



#endif // _SERVICEFACERECOGNITION_INL_
