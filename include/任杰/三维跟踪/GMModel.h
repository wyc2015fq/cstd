#ifndef _GMMODEL_H_
#define _GMMODEL_H_

#include <stdlib.h>

// 高斯背景建模类
struct GMM {
public:
	// 背景建模函数, 建立背景模型时每帧图片调用一次该函数.
	void UpdateModel(const unsigned char* src, int step, int num);

	// 将背景建模权重, 均值, 方差结果导出到文本文档.
	void Export2File(const char* wf, const char* mf, const char* vf) const;

	// 前景检测和后期处理, 返回检测结果个数.
	int Detect(const unsigned char* src, int step, const unsigned char *pmask = NULL);

	// 当前背景图像.
	const double* Background(void) const {return b;}

	// 连通区域结果.
	const unsigned int* BlobRst(void) const {return blob;}

	// 场景高度.
	int SceneH(void) const {return row;}

	// 场景宽度.
	int SceneW(void) const {return col;}

public:

	GMM(double Afa  = 0.005, 
		double Thd  = 0.4, 
		double VirV = 5.0, 
		double VirW = 0.05, 

		double shadowp0 = 0.95, 
		double shadowp1 = 0.3, 
		double shadowp2 = 0.3, 
		double shadowp3 = 0.1, 

		int R   = 120, 
		int C   = 160, 
		int Num = 3, 
		int MinPts = 20, 

		bool IsRectLabel   = true, 
		bool IsRectShadowX = true, 
		bool IsRectShadowY = false, 
		bool IsRectShape   = true
		): 
	afa  (Afa), 
	thrd (Thd), 
	vir_v(VirV), 
	vir_w(VirW), 

	row (R), 
	col (C), 
	kind(Num), 
	min_pts(MinPts), 

	s(new int[row * col]),
	w(new double[row * col * kind]), 
	m(new double[row * col * kind * 3]), 
	v(new double[row * col * kind]), 
	b(new double[row * col * 3]), 

	blob(new unsigned int[col * row]), 
	label(new unsigned char[row * col]), 

	rlabel  (IsRectLabel), 
	rshadowx(IsRectShadowX), 
	rshadowy(IsRectShadowY), 
	rshape  (IsRectShape)
	{
		*(shadowp + 0) = shadowp0;
		*(shadowp + 1) = shadowp1;
		*(shadowp + 2) = shadowp2;
		*(shadowp + 3) = shadowp3;

		for (int i = 0; i < row * col * kind; ++i) 
		{
			*(w + i) = 0.0;
			*(v + i) = 0.0;
		}

		for (i = 0; i < row * col * kind * 3; ++i) 
		{
			*(m + i) = 0.0;
		}

		for (i = 0; i < row * col * 3; ++i) 
		{
			*(b + i) = 0.0;
		}

		for (i = 0; i < row * col; ++i) 
		{
			*(blob + i) = 0;
		}

		SetS();
	}

	~GMM() 
	{
		delete []label; label = NULL;
		delete [] blob; blob  = NULL;

		delete []b; b = NULL;
		delete []v; w = NULL;
		delete []m; m = NULL;
		delete []w; v = NULL;
		delete []s; s = NULL;
	}


private:
	double afa;
	double thrd;
	double vir_v;
	double vir_w;
	double shadowp[4];

	int row;
	int col;
	int kind; 

	unsigned int min_pts;

	int *s;
	double *w, *m, *v, *b;

	unsigned int *blob;
	unsigned char *label;

	bool rlabel, rshadowx, rshadowy, rshape;

private:
	// 禁用拷贝构造函数和赋值运算符.
	GMM(const GMM&);
	const GMM& operator = (const GMM&);

	// 初始化任意点当前已有的类别数，每一次重新建立背景模型都必须调用此函数.
	void SetS(void) 
	{
		for (int i = 0; i < row * col; ++i) 
		{
			*(s + i) = 1; 
		}
	}

	// 计算最大可能背景的函数, 每次检测前必须调用.
	void GetIb(void);

	// 前景检测.
	void DetectForeGround(const unsigned char* pt, int step, const unsigned char *pmask = NULL);

	// 空洞填补修正.
	void RectifyLabel(void);

	// 光照突变误检和阴影误检修正.
	void Rectify(const unsigned char *pt, int step);

	// 腐蚀, 膨胀修正.
	void RectifyShape(void);

	// 计算连通区域个数(检测结果团块个数).
	int RectLabel(void);

};

#endif