#pragma once

#include "stdafx.h"

class CMySIFT
{
public:
	CMySIFT(void);
	~CMySIFT(void);
	bool initialize(CString img_filePath, bool isBlur = false);
	bool cloneSiftFeature(CMySIFT& sift);

public:
	IplImage* img;
	struct feature* feat;
	int num;
};
