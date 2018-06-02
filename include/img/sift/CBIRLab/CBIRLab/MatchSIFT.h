#pragma once
#include "MySIFT.h"
#include "RetrievalResult.h"

/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.49

class CMatchSIFT
{
public:
	CMatchSIFT(void);
	~CMatchSIFT(void);
	bool initialize(CMySIFT* imgSIFT1, CMySIFT* imgSIFT2);
	bool matchTwoImage(CRetrievalResult& retrievalRes);

public:
	//为了显示两张图在一块的
	CMySIFT* imgSIFT1, *imgSIFT2;
	int n1, n2;
};
