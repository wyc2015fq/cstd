#include <stdio.h>
#include "arcsoft_fsdk_age_estimation.h"
#include "arcsoft_fsdk_face_detection.h"
#include "arcsoft_fsdk_gender_estimation.h"
#include "merror.h"
#include "assert.h"

#pragma comment(lib,"libarcsoft_fsdk_age_estimation.lib")
#pragma comment(lib,"libarcsoft_fsdk_face_detection.lib")
#pragma comment(lib,"libarcsoft_fsdk_gender_estimation.lib")

#define MAXIMUM_FACE_NUMBER 10
#define WORKBUF_SIZE        (30*1024*1024)
#define APPID		"Ds3AyzWVhxTWTNRv1Zo6QfsMEaMESMThgAPee2PpzYVM"			//APPID
#define ASAE_AgeSDKKey  "EQThhSTUgNMNNnGF5VPmo3MPPMXVpAWMK6qsiMbTfCdi"			//AgeSDKKey
#define ASAE_GenderSDKKey  "EQThhSTUgNMNNnGF5VPmo3MWYknhyuocRWdptUY6UfDb"			//GenderSDKKey
#define ASAE_FDSDKKey  "EQThhSTUgNMNNnGF5VPmo3LeQwxXudUKcrFzKM3uQCD5"			//FDSDKKey
#define ASAE_FRSDKKey  "EQThhSTUgNMNNnGF5VPmo3LmaMDgXoDr6yatvWn19QyF"			//FRSDKKey
#define ASAE_FTSDKKey  "EQThhSTUgNMNNnGF5VPmo3LXFYhNv9huP2zYn2aH8sN3"			//FTSDKKey

#define AGE_ESTIMATION_STATICIMAGE 
//#define AGE_ESTIMATION_PREVIEW 
#define nullptr NULL
/* define global variables for age estimation */
MHandle						AgeEngine = nullptr;
MHandle						GenderEngine = nullptr;
MHandle						FDEngine = nullptr;
ASVLOFFSCREEN				AImageInfo = { 0 };
ASAE_FSDK_AGEFACEINPUT		AgeFaceInput;
ASGE_FSDK_GENDERFACEINPUT		GenderFaceInput;
ASAE_FSDK_AGERESULT			AgeResult;
ASGE_FSDK_GENDERRESULT			GenderResult;
MByte *						pAgeWorkMem = nullptr;
MByte *						pGenderWorkMem = nullptr;
MByte *						pFDWorkMem = nullptr;

/* initialize the engine and other variables */
MInt32 InitAgeEstimate()
{
	MInt32 res = MOK;
	AgeFaceInput.lFaceNumber = 0;
  GenderFaceInput.lFaceNumber = 0;
	AgeFaceInput.pFaceRectArray = new MRECT[MAXIMUM_FACE_NUMBER];
	GenderFaceInput.pFaceRectArray = new MRECT[MAXIMUM_FACE_NUMBER];
	if (0 == AgeFaceInput.pFaceRectArray)
		return MERR_NO_MEMORY;
	AgeFaceInput.pFaceOrientArray = new MInt32[MAXIMUM_FACE_NUMBER];
	GenderFaceInput.pFaceOrientArray = new MInt32[MAXIMUM_FACE_NUMBER];
	if (0 == AgeFaceInput.pFaceOrientArray)
		return MERR_NO_MEMORY;

	MInt32 nScale = 22;
	MInt32 nMaxFace = 100;
	pAgeWorkMem = new MByte[WORKBUF_SIZE];
	pGenderWorkMem = new MByte[WORKBUF_SIZE];
	pFDWorkMem = new MByte[WORKBUF_SIZE];
	res = ASAE_FSDK_InitAgeEngine(APPID, ASAE_AgeSDKKey, pAgeWorkMem, WORKBUF_SIZE, &AgeEngine);
	res = ASGE_FSDK_InitGenderEngine(APPID, ASAE_GenderSDKKey, pGenderWorkMem, WORKBUF_SIZE, &GenderEngine);
	res = AFD_FSDK_InitialFaceEngine(APPID, ASAE_FDSDKKey, pFDWorkMem, WORKBUF_SIZE, &FDEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
  assert(AgeEngine && GenderEngine && FDEngine);
	return res;
}
/* release the engine and other memory handles */
MInt32 UnInitAgeEstimate()
{
	MInt32 res = MOK;
	res = ASAE_FSDK_UninitAgeEngine(AgeEngine);
	if (AgeFaceInput.pFaceRectArray != nullptr)
	{
		delete[] AgeFaceInput.pFaceRectArray;
		AgeFaceInput.pFaceRectArray = nullptr;
	}
	if (AgeFaceInput.pFaceOrientArray != nullptr)
	{
		delete[] AgeFaceInput.pFaceOrientArray;
		AgeFaceInput.pFaceOrientArray = nullptr;
	}
	if (pAgeWorkMem != nullptr)
	{
		delete[] pAgeWorkMem;
		pAgeWorkMem = nullptr;
	}
	return res;
}
/* print sdk version */
void PrintVersionInfo()
{
	const ASAE_FSDK_Version * pVersionInfo = nullptr;
	pVersionInfo = ASAE_FSDK_GetVersion(AgeEngine);
	printf("%d %d %d %d\n", pVersionInfo->lCodebase, pVersionInfo->lMajor, pVersionInfo->lMinor, pVersionInfo->lBuild);
	printf("%s\n", pVersionInfo->Version);
	printf("%s\n", pVersionInfo->BuildDate);
	printf("%s\n", pVersionInfo->CopyRight);
}

#include "cstd.h"
#include "str.h"
#include "cfile.h"
#include "img/imgio.inl"

const char* age_tag[] = {
    "region1_5",
    "region6_10",
    "region11_15",
    "region16_20",
    "region21_25",
    "region26_30",
    "region31_35",
    "region36_40",
    "region41_45",
    "region46_50",
    "region51_55",
    "region56_60",
    "region61_65",
    "region66_70",
    "region71_75",
    "region76_80",
};
const char* gender_tag[] = {
  "Female",
  "Male",
};
int str_to_label(const char* str, const char** strs, int len, int delims) {
  int i;
  for (i=0; i<len; ++i) {
    const char* p = str;
    const char const * s = strs[i];
    int l = strlen(s);
    for (;*p;++p) {
      if (delims==*p && delims==p[l+1] && 0==_memicmp(p+1, s, l)) {
        return i;
      }
    }
  }
    printf("error : %s unkown label\n", str);
  if (1) {
    int asdf=0;
  }
  return -1;
}

double fsqr(double x) {
  return x*x;
}

#ifdef AGE_ESTIMATION_PREVIEW
/* get each preview frame and save it to "pImageInfo", which is a "ASVLOFFSCREEN" struct. return "MOK" if succeed. */
MInt32 GetPreviewData(ASVLOFFSCREEN * pImageInfo)
{
	MInt32 res = MOK;

	/* get frame data. add your code here */
	/* ... ... */

	return res;
}

/* using ArcSoft face tracking library to detect faces in the input image and save the face results to "pFaceInput".
return "MOK" if succeed. */
MInt32 FaceTrack(ASVLOFFSCREEN * pImageInfo, ASAE_FSDK_AGEFACEINPUT * pFaceInput)
{
	MInt32 res = MOK;

	/* add your code here */
	/* ... ... */

	return res;
}
#endif

/* estimate age  */
MInt32 AgeEstimate()
{
	MInt32 res = MOK;

#ifdef AGE_ESTIMATION_PREVIEW
	res = ASAE_FSDK_AgeEstimation_Preview(AgeEngine, &AImageInfo, &AgeFaceInput, &AgeResult);
#endif

	return res;
}

int main()
{
	/* initialize the engine and other variables */
	MRESULT nRet = MERR_UNKNOWN;
	nRet = InitAgeEstimate();
	if (nRet != MOK)
	{
		printf("InitAgeEngine failed , errorcode is %d \n", nRet);
		return -1;
	}

	/* print version info */
	PrintVersionInfo();
  
#ifdef AGE_ESTIMATION_STATICIMAGE
  /* image data acquisition */
  vstr_t sv[1] = {0};
  img_t im[1] = {0};
  int acc = 0, detcnt = 0;
  int cnt = 0;
  int accage = 0;
  float age_err_sum=0;
  float age_err_abssum = 0;
  float age_err_sqsum = 0;
  int gender_id = 0;
  int age_id = 0;
  int i=0;
  
  
  /* load static image and save it to "pImageInfo", which is a "ASVLOFFSCREEN" struct. return "MOK" if succeed. */

  static int inited = 0;
  if (!inited) {
    const char* list_file = "list.txt";
    sys_chdir("D:/code/cnn/gender");
    DataSize size;
    int delims = '\\';
    vstr_load(list_file, sv);
    size = dataSize(sv->n, 3, 64, 64);
  }
  
        double tage=0;
        double tgender=0;
	MInt32 res = MOK;
  for (i=sv->n*0.9; i<sv->n; ++i) {
    int delims = '\\';
    ++cnt;
    gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
    age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
      float age = age_id*5. + 3;
    if (gender_id>=0 && imread(sv->v[i].s, 3, 1, im)) {
      AImageInfo.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
      AImageInfo.i32Width = im->w;
      AImageInfo.i32Height = im->h;
      AImageInfo.ppu8Plane[0] = im->data;
      AImageInfo.pi32Pitch[0] = im->s;
      
      /* face detection */
      LPAFD_FSDK_FACERES	FaceRes = nullptr;
      nRet = AFD_FSDK_StillImageFaceDetection(FDEngine, &AImageInfo, &FaceRes);
      if (FaceRes->nFace>0) {
        AgeFaceInput.lFaceNumber = 1;
        AgeFaceInput.pFaceOrientArray[0] = FaceRes->lfaceOrient[0];
        AgeFaceInput.pFaceRectArray[0] = FaceRes->rcFace[0];
        GenderFaceInput.lFaceNumber = 1;
        GenderFaceInput.pFaceOrientArray[0] = FaceRes->lfaceOrient[0];
        GenderFaceInput.pFaceRectArray[0] = FaceRes->rcFace[0];
        
        // A 11624 / 13672 / 15828  G: 85.0 FDR: 86.4 64.2 EM: 7.5 EAM: 10.1 EV: 251.5 31.0 16.0
{utime_start(_start_time);
        res = ASAE_FSDK_AgeEstimation_StaticImage(AgeEngine, &AImageInfo, &AgeFaceInput, &AgeResult);
tage += utime_elapsed(_start_time);}
{utime_start(_start_time);
        res = ASGE_FSDK_GenderEstimation_StaticImage(GenderEngine, &AImageInfo, &GenderFaceInput, &GenderResult);
tgender += utime_elapsed(_start_time);}
        int age1 = AgeResult.pAgeResultArray[0];
        acc += (!gender_id)==GenderResult.pGenderResultArray[0];
        accage += fabs(age - age1)<10;
        age_err_sum += (age - age1);
        age_err_abssum += fabs(age - age1);
        age_err_sqsum += fsqr(age - age1);
        ++detcnt;
        
        if (nRet == MOK)
        {
          printf("A %d / %d / %d  G: %3.1f FDR: %3.1f %3.1f EM: %3.1f EAM: %3.1f EV: %3.1f %3.1f %3.1f\n",
            acc, detcnt, cnt, acc*100./detcnt, detcnt*100./(cnt), accage*100./ detcnt,
            age_err_sum/detcnt, age_err_abssum/detcnt, age_err_sqsum/detcnt, tage*1000./detcnt, tgender*1000./detcnt);
        }
      }
    }
  }

  printf("\n");
#endif

#ifdef AGE_ESTIMATION_PREVIEW
	/* preview data acquisition */
	while (MOK == GetPreviewData(&AImageInfo)) 
	{
		/* face tracking */
		nRet = FaceTrack(&AImageInfo, &AgeFaceInput);
		if (nRet != MOK)
		{
			printf("FaceDetect: nRet=%d\n", nRet);
			return nRet;
		}

		/* age estimation */
		/* no matter "AgeFaceInput->lFaceNumber" is greater than or equal to 0, next sentence must be excuted. */
		nRet = AgeEstimate();
		if (nRet != MOK)
		{
			printf("AgeEstimate: nRet=%d\n", nRet);
			return nRet;
		}

		/* print age estimation result */
		if (AgeFaceInput.lFaceNumber > 0)
		{
			for (int i = 0; i < AgeFaceInput.lFaceNumber; i++)
			{
				printf("Age: %d, ", AgeResult.pAgeResultArray[i]);
			}
			printf("\n");
		}
	}
#endif

	/* release the engine and other memory handles */
	nRet = UnInitAgeEstimate();
	return nRet;
}
