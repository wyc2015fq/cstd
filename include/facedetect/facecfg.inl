
typedef struct facecfg_t
{
  bool   debug;			//DEBUG模式
  bool   videoShow;		//是否显示视频
  int8_t videoPort;		//视频源端口
  bool   trainSave;		//是否保存训练图片
  bool   facePlusPlus;	//是否调用FACE++人脸识别功能
  bool   recognizer;		//是否启用人脸识别
  int32_t trackTime;		//人脸位置跟踪时长
  int32_t predictTime;	//人脸记忆时长
  int32_t forecastMax;	//人脸预测阈值最大值
  int32_t forecastMin;	//人脸预测阈值最小值
  int32_t forecastValue;	//人脸预测阈值
  int32_t definition;		//人脸清晰度阈值
  int32_t pixelw;			//像素宽
  int32_t pixelh;			//像素高
  int32_t clearImageInterval; //删除抓拍图像的时间间隔(秒)
  bool  newver;
  int32_t paramVer;
  int32_t face_code;
  char host[64];
  char pichost[64];
  char mall_id[128];
  char terminal_id[128];
  char machine_no[128];
} facecfg_t;

int facecfg_init(facecfg_t* f)
{
  memset(f, 0, sizeof(*f));
  f->paramVer = (FILE_VER);
  f->newver = false;
  f->face_code = 0;
  f->debug = 0;			//关闭DEBUG
  f->videoShow = 0;		//是否显示视频
  f->videoPort = -1;		//视频源端口,-1代表自动检测
  f->trainSave = 0;		//是否保存训练图片
  f->facePlusPlus = 0;	//是否调用FACE++人脸识别功能
  f->recognizer = 0;		//是否启用人脸识别
  f->trackTime = 3;		//人脸位置跟踪时长
  f->predictTime = 100;	//人脸记忆时长
  f->forecastMax = 200;		//人脸预测阈值最大值
  f->forecastMin = 40;		//人脸预测阈值最小值
  f->forecastValue = 60;		//人脸预测阈值
  f->definition = 60;		//人脸清晰度阈值
  f->pixelw = 1280;			//像素宽
  f->pixelh = 960;			//像素高
	f->clearImageInterval = 604800; //清理抓拍人脸的时间间隔
  strcpy(f->host, "http://node.rongyi.com");
  strcpy(f->pichost, "http://state.hdp.rongyi.com");
  strcpy(f->mall_id, "51f9d7f731d6559b7d000163");
  strcpy(f->terminal_id, "ec2818a1-c359-407f-a374-7d3931920da6");
  strcpy(f->machine_no, "rongyi_android");
  return 0;
}

#include "parser/cjson.inl"

int facecfg_print(facecfg_t* f) {
  printf("trainSave :%d\n", f->trainSave);
  printf("facePlusPlus :%d\n", f->facePlusPlus);
  printf("recognizer :%d\n", f->recognizer);
  printf("trackTime :%d\n", f->trackTime);
  printf("predictTime :%d\n", f->predictTime);
  printf("forecastMin :%d\n", f->forecastMin);
  printf("forecastValue :%d\n", f->forecastValue);
  printf("host :%s\n", f->host);
  printf("pichost :%s\n", f->pichost);
  printf("mall_id :%s\n", f->mall_id);
  printf("terminal_id :%s\n", f->terminal_id);
  printf("machine_no :%s\n", f->machine_no);
  return 0;
}

bool facecfg_load(facecfg_t* f, const char* sFile) {
	cJSON *pJsRoot, *pJsItem;

	pJsRoot = cJSON_OpenFile(sFile);
	if (!pJsRoot)
	{
		return false;
	}
  
	pJsItem = cJSON_GetObjectItem(pJsRoot, "paramVer");
	if (!pJsItem || pJsItem->valueint != f->paramVer)
	{
		f->newver = true;
		cJSON_Delete(pJsRoot);
		return false;
	}
  
  pJsItem = cJSON_GetObjectItem(pJsRoot, "debug");
  if (pJsItem)
  {
    f->debug = pJsItem->valueint ? true : false;
  }
  
  pJsItem = cJSON_GetObjectItem(pJsRoot, "face_code");
  if (pJsItem)
  {
    f->face_code = pJsItem->valueint;
  }

	pJsItem = cJSON_GetObjectItem(pJsRoot, "videoShow");
	if (pJsItem)
	{
		f->videoShow = pJsItem->valueint ? true : false;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "videoPort");
	if (pJsItem)
	{
		f->videoPort = pJsItem->valueint;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "trainSave");
	if (pJsItem)
	{
		f->trainSave = pJsItem->valueint ? true : false;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "facePlusPlus");
	if (pJsItem)
	{
		f->facePlusPlus = pJsItem->valueint ? true : false;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "recognizer");
	if (pJsItem)
	{
		f->recognizer = pJsItem->valueint ? true : false;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "trackTime");
	if (pJsItem)
	{
		f->trackTime = pJsItem->valueint;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "predictTime");
	if (pJsItem)
	{
		f->predictTime = pJsItem->valueint;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "forecastMax");
	if (pJsItem)
	{
		f->forecastMax = pJsItem->valueint;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "forecastMin");
	if (pJsItem)
	{
		f->forecastMin = pJsItem->valueint;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "forecastValue");
	if (pJsItem)
	{
		f->forecastValue = pJsItem->valueint;
	}

	//pJsItem = cJSON_GetObjectItem(pJsRoot, "definition");
	//if (pJsItem)
	//{
	//	definition = pJsItem->valueint;
	//}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "pixelw");
	if (pJsItem)
	{
		f->pixelw = pJsItem->valueint;
	}

	pJsItem = cJSON_GetObjectItem(pJsRoot, "pixelh");
	if (pJsItem)
	{
		f->pixelh = pJsItem->valueint;
	}
  
  pJsItem = cJSON_GetObjectItem(pJsRoot, "clearImageInterval");
  if (pJsItem)
  {
    f->clearImageInterval = pJsItem->valueint;
  }
  
  pJsItem = cJSON_GetObjectItem(pJsRoot, "host");
  if (pJsItem)
  {
    if (pJsItem->valuestring && strlen(pJsItem->valuestring)>0)
      strncpy(f->host, pJsItem->valuestring, countof(f->host));
  }
  
  pJsItem = cJSON_GetObjectItem(pJsRoot, "pichost");
  if (pJsItem)
  {
    if (pJsItem->valuestring && strlen(pJsItem->valuestring)>4)
      strncpy(f->pichost, pJsItem->valuestring, countof(f->pichost));
  }
  
  pJsItem = cJSON_GetObjectItem(pJsRoot, "mall_id");
  if (pJsItem)
  {
    if (pJsItem->valuestring && strlen(pJsItem->valuestring)>0) {
    strncpy(f->mall_id, pJsItem->valuestring, countof(f->mall_id));
    }
  }
  
  pJsItem = cJSON_GetObjectItem(pJsRoot, "terminal_id");
  if (pJsItem)
  {
    if (pJsItem->valuestring && strlen(pJsItem->valuestring)>0) {
      strncpy(f->terminal_id, pJsItem->valuestring, countof(f->terminal_id));
    }
  }
  
  
  pJsItem = cJSON_GetObjectItem(pJsRoot, "machine_no");
  if (pJsItem)
  {
    if (pJsItem->valuestring && strlen(pJsItem->valuestring)>0)
      strncpy(f->machine_no, pJsItem->valuestring, countof(f->machine_no));
  }

	cJSON_Delete(pJsRoot);
  return false;
}

bool facecfg_save(facecfg_t* f, const char* sFile) {
  cJSON *pJsRoot, *pJsItem;
  
  pJsRoot = cJSON_CreateObject();
  
  pJsItem = cJSON_CreateNumber(f->paramVer);
  cJSON_AddItemToObject(pJsRoot, "paramVer", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->debug);
  cJSON_AddItemToObject(pJsRoot, "debug", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->videoShow);
  cJSON_AddItemToObject(pJsRoot, "videoShow", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->videoPort);
  cJSON_AddItemToObject(pJsRoot, "videoPort", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->face_code);
  cJSON_AddItemToObject(pJsRoot, "face_code", pJsItem);

  pJsItem = cJSON_CreateNumber(f->trainSave);
  cJSON_AddItemToObject(pJsRoot, "trainSave", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->facePlusPlus);
  cJSON_AddItemToObject(pJsRoot, "facePlusPlus", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->recognizer);
  cJSON_AddItemToObject(pJsRoot, "recognizer", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->trackTime);
  cJSON_AddItemToObject(pJsRoot, "trackTime", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->predictTime);
  cJSON_AddItemToObject(pJsRoot, "predictTime", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->forecastMax);
  cJSON_AddItemToObject(pJsRoot, "forecastMax", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->forecastMin);
  cJSON_AddItemToObject(pJsRoot, "forecastMin", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->forecastValue);
  cJSON_AddItemToObject(pJsRoot, "forecastValue", pJsItem);
  
  //pJsItem = cJSON_CreateNumber(f->definition);
  //cJSON_AddItemToObject(pJsRoot, "definition", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->pixelw);
  cJSON_AddItemToObject(pJsRoot, "pixelw", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->pixelh);
  cJSON_AddItemToObject(pJsRoot, "pixelh", pJsItem);
  
  pJsItem = cJSON_CreateNumber(f->clearImageInterval);
  cJSON_AddItemToObject(pJsRoot, "clearImageInterval", pJsItem);
  
  pJsItem = cJSON_CreateString(f->host);
  cJSON_AddItemToObject(pJsRoot, "host", pJsItem);
  
  pJsItem = cJSON_CreateString(f->pichost);
  cJSON_AddItemToObject(pJsRoot, "pichost", pJsItem);
  
  pJsItem = cJSON_CreateString(f->mall_id);
  cJSON_AddItemToObject(pJsRoot, "mall_id", pJsItem);
  
  pJsItem = cJSON_CreateString(f->terminal_id);
  cJSON_AddItemToObject(pJsRoot, "terminal_id", pJsItem);
  
  pJsItem = cJSON_CreateString(f->machine_no);
  cJSON_AddItemToObject(pJsRoot, "machine_no", pJsItem);

  cJSON_SaveFile(sFile, pJsRoot);
	cJSON_Delete(pJsRoot);
  return false;
}
