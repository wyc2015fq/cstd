#ifndef TRI_DIM_MODEL
#define TRI_DIM_MODEL

void CreateModel(uchar *binRst, int h, int w, int m, int n, const double *pTransMat, const double *pTInv, int SampOnCircle, int rLow, int rMid, int rHigh, int stdHeightLow, int stdHeightMid, int stdHeightHigh);

struct TriDimModel
{
public:
	const uchar* GetModel(int r, int c) const 
	{
		return m_Model3D + (r * m_WSubNum + c) * (m_Row * m_Col);
	}

	int GetModelSize(int r, int c) const
	{
		return *(m_ModelSize + r * m_WSubNum + c);
	}

	int GetHSubNum(void) const 
	{
		return m_HSubNum;
	}

	int GetWSubNum(void) const 
	{
		return m_WSubNum;
	}

public:
	TriDimModel(int h, int w, int subH, int subW, const double *mat, const double *matx, int sampNum, int rLow, int rMid, int rTop, int hLow, int hMid, int hTop):
	    m_Row(h), 
		m_Col(w), 
		m_HSubNum(subH), 
		m_WSubNum(subW), 
		m_Model3D(new uchar[(subH * subW) * (h * w)]), 
		m_ModelSize(new int[subH * subW])
		{
			if (NULL == m_Model3D || NULL == m_ModelSize)
			{
				exit(-1);
			}

			CreateModel(m_Model3D, 
				        h, 
						w, 
						subH, 
						subW, 
						mat, 
						matx, 
						sampNum, 
						rLow, 
						rMid, 
						rTop, 
						hLow, 
						hMid, 
						hTop);

			CalSize();
		}

	~TriDimModel()
	{
		delete []m_Model3D; m_Model3D = NULL;
		delete []m_ModelSize; m_ModelSize = NULL;
	}

private:
	void CalSize(void);

private:
	int m_Row;
	int m_Col;

	// 子块中心人体模型二值表示数据.
	int m_HSubNum;
	int m_WSubNum;

	uchar *m_Model3D; // 子区域中心人体3D模型二值表示序列头指针.
	int *m_ModelSize; // 子区域中心人体3D模型像素尺寸.
};

#endif