#include "algo.h"
#include "TriDimModel.h"
#include "..\三维标定\calibration.h"



// 监控区域3d模型.
void SamplePtsOnCircle(double *x, double *y, int num, double r);

void CalCoordinate(const double *pTInv, int m, int n, const double *stdX, const double *stdZ, double *x, double *z, int row = 3, int col = 51);

void TransDim(int index, int h, const double *pTransMat, int *xz3D, int stdHeightLow, int stdHeightMid, int stdHeightHigh, const double *x, const double *z, int row = 3, int col = 51);


// 在圆心(0, 0), 半径为r的圆周上逆时针均匀抽取(num + 1, 第一个点与最后一个点重合)个点, 抽取的第一个点坐标为(r, 0).
static inline void SamplePtsOnCircle(double *x, double *y, int num, double r)
{
	const double PI = 3.141592653589793;

	assert(NULL != x && NULL != y && num > 0 && r > 0);

	const double ANGLE_STEP = 2 * PI / num;

	// 首点与尾点重合.
	*(x + 0) = *(x + num) = r;
	*(y + 0) = *(y + num) = 0;

	double *px = x + 1;
	double *py = y + 1;

	for (int c = 1; c < num; ++c)
	{
		*px = r * cos(ANGLE_STEP * c);
		*py = r * sin(ANGLE_STEP * c);

		++px;
		++py;
	}
}

// 监控区域子区域中计算X, Z坐标序列. 矩阵pTransMat的维度为(3 * 4), stdX, stdZ, x, y具有相同的维度row * col(当前使用3 * 51).
static inline void CalCoordinate(const double *pTInv, int m, int n, const double *stdX, const double *stdY, double *x, double *y, int row, int col)
{
	assert(NULL != pTInv && NULL != stdX && NULL != stdY && NULL != x && NULL != y && 0 < row && 0 < col);

	const int F[] = {n, m};

	double t[2];

	DimTrans23(pTInv, F, t);

	for (int i = 0; i < row * col; ++i)
	{
		*(x + i) = *(stdX + i) + *(t + 0);
		*(y + i) = *(stdY + i) + *(t + 1);
	}
}

// 人模型某点(x, y)经变换矩阵转换后的三维模型坐标(x, y). pTransMat的维度为(3 * 4), x, u维度同为size(当前使用3 * 51)
static inline void TransDim(int index, int h, const double *pTransMat, int *xy3D, int stdHeightLow, int stdHeightMid, int stdHeightHigh, const double *x, const double *y, int row, int col)
{
	assert(-1 < index && -1 < h && NULL != pTransMat && NULL != xy3D && NULL != x && NULL != y && 0 < row && 0 < col);

	double tmpRst[3];

	const int DIMENSION = h < stdHeightLow ? 0 : (h < stdHeightMid ? 1 : 2);
	const double WORLD_SAMPLE[] = {*(x + DIMENSION * col + index), -h, *(y + DIMENSION * col + index), 1};

	MatMul(pTransMat, WORLD_SAMPLE, tmpRst, 3, 4, 1);

	*(xy3D + 0) = int(tmpRst[0] / tmpRst[2]);
	*(xy3D + 1) = int(tmpRst[1] / tmpRst[2]);
}

// 计算分块后各个子区域中心人模型3D变换后的平面二值图表示.
void CreateModel(uchar *binRst,           // 结果二值图序列(m * n幅二值图)头指针.
				 int h,                   // 图像高度.
				 int w,                   // 图像宽度.
				 int m,                   // 高度上的子块个数.
				 int n,                   // 宽度上的子块个数.
				 const double *pTransMat, // 3D变换矩阵.
				 const double *pTInv,     // 3D逆变换矩阵.
				 int SampOnCircle,        // 在圆上平均距离抽样的个数.
				 int rLow,                // 腿圆柱半径.
				 int rMid,                // 身体圆柱半径.
				 int rHigh,               // 头圆柱半径.
				 int stdHeightLow,        // 腿部高度.
				 int stdHeightMid,        // 身体高度.
				 int stdHeightHigh)       // 头部高度.
{
	assert(NULL != binRst && NULL != pTransMat);

	// 第一个抽样点和最后一个重合(+1), 圆柱分上下2个面抽样(*2), 共腿身头3个圆柱(*3).
	const int SAMP_NUM = (SampOnCircle + 1);

	double *x = new double[SAMP_NUM * 3];
	double *y = new double[SAMP_NUM * 3];
	double *p = new double[SAMP_NUM * 3];
	double *q = new double[SAMP_NUM * 3];

	if (NULL == x || NULL == y || NULL == p || NULL == q) exit(-1);

	SamplePtsOnCircle(x + SAMP_NUM * 0, y + SAMP_NUM * 0, SampOnCircle, rLow);  // 腿部圆柱抽样点.
	SamplePtsOnCircle(x + SAMP_NUM * 1, y + SAMP_NUM * 1, SampOnCircle, rMid);  // 身体圆柱抽样点.
	SamplePtsOnCircle(x + SAMP_NUM * 2, y + SAMP_NUM * 2, SampOnCircle, rHigh); // 头部圆柱抽样点.

	memset(binRst, 0, h * w * m * n * sizeof(uchar));

	int loc[2]; // 临时存放转换为3D模式之后的(x, y)坐标.

	for (int r = 0; r < m; ++r)
	{
		const int CUR_ROW_CTR = (h / m) / 2 + (h / m) * r;

		for (int c = 0; c < n; ++c)
		{
			const int CUR_COL_CTR = (w / n) / 2 + (w / n) * c;

			CalCoordinate(pTInv, CUR_ROW_CTR, CUR_COL_CTR, x, y, p, q, 3, SAMP_NUM);

			for (int i = 0; i < SampOnCircle + 1; ++i)
			{
				for (int j = 0; j < stdHeightHigh + 1; ++j)
				{
					TransDim(i, j, pTransMat, loc, stdHeightLow, stdHeightMid, stdHeightHigh, p, q, 3, SAMP_NUM);

					if (-1 < *(loc + 0) && *(loc + 0) < w && -1 < *(loc + 1) && *(loc + 1) < h) // 在二值图像边界内部.
					{
						*(binRst + *(loc + 1) * w + *(loc + 0)) = 255;
					}
				}
			}

			binRst += (h * w); // 指向下一副二值图的首指针.
		}
	}

	// 内存释放.
	delete []p; p = NULL;
	delete []q; q = NULL;
	delete []x; x = NULL;
	delete []y; y = NULL;
}

// 计算个各子块中的人体模型的尺寸.
void TriDimModel::CalSize()
{
	const uchar *bin = m_Model3D;
	int         *sum = m_ModelSize;

	memset(m_ModelSize, 0, m_HSubNum * m_WSubNum * sizeof(int));

	for (int i = 0; i < m_HSubNum * m_WSubNum; ++i)
	{
		for (int j = 0; j < m_Row * m_Col; ++j)
		{
			if (0 < *(bin + j))
			{
				++(*sum);
			}
		}

		++sum;
		bin += m_Row * m_Col;
	}
}