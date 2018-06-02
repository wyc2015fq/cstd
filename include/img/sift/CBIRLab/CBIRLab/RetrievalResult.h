#pragma once
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <vector>

using namespace std;

class RegionQuad
{
public:
	//四边形的逆时针四个点坐标，分别对应坐上，左下，右下，右上
	CvPoint points[4];
	CvPoint rectTopLeft, rectBottomRight;

public:
	void transformToRect(int width, int height);
};

class CRetrievalResult
{
public:
	CRetrievalResult(void);
	~CRetrievalResult(void);
	bool initialize(CString path);

public:
	//图像路径
	CString imagePath;
	//检索区域个数
	int regionCount;
	//图像区域集合
	vector<RegionQuad> vpRegions;
};
