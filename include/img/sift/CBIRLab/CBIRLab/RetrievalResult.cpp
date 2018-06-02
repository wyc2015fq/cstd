#include "StdAfx.h"
#include "RetrievalResult.h"

void RegionQuad::transformToRect(int width, int height)
{
	//注意默认图像中的原点在左上角
	int minX = width, maxX = 0, minY = height, maxY = 0;
	for(int i = 0; i < 4; i++)
	{
		if (points[i].x > maxX)
		{
			maxX = points[i].x;
		}
		if (points[i].x < minX)
		{
			minX = points[i].x;
		}
		if (points[i].y > maxY)
		{
			maxY = points[i].y;
		}
		if (points[i].y < minY)
		{
			minY = points[i].y;
		}
	}
	rectTopLeft.x = (minX < 0) ? 0 : minX;
	rectTopLeft.y = (minY < 0 ) ? 0 : minY;
	rectBottomRight.x = (maxX > width) ? width : maxX;
	rectBottomRight.y = (maxY > height) ? height : maxY;
}

CRetrievalResult::CRetrievalResult(void)
{
}

CRetrievalResult::~CRetrievalResult(void)
{
}

bool CRetrievalResult::initialize(CString path)
{
	imagePath = path;
	regionCount = 0;
	return true;
}
