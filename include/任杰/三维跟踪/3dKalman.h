#ifndef _KALMAN_H_
#define _KALMAN_H_


// 常量定义.
const double T = 1.0 / 30.0;

const double F[] = 
{
	1, 0, T, 0, 
	0, 1, 0, T, 
	0, 0, 1, 0, 
	0, 0, 0, 1
};

const double FT[] = 
{
	1, 0, 0, 0, 
	0, 1, 0, 0, 
	T, 0, 1, 0, 
	0, T, 0, 1
};

const int H[] = 
{
	1, 0, 0, 0, 
	0, 1, 0, 0
};

const int HT[] = 
{
	1, 0, 
	0, 1, 
	0, 0, 
	0, 0
};

const int I[] = 
{
	1, 0, 0, 0, 
	0, 1, 0, 0, 
	0, 0, 1, 0, 
	0, 0, 0, 1
};

const double Q[] = 
{
	1, 0, 0, 0, 
	0, 1, 0, 0, 
	0, 0, 1, 0, 
	0, 0, 0, 1
};

const double INIT_V = 0;
const double INIT_P = 1000;

struct kalman
{
public:
	// 构造函数.
	kalman(double cx, double cy, double vx = INIT_V, double vy = INIT_V) 
	{
		init(cx, cy, vx, vy);
	}

	// 获取当前预测x结果.
	double GetPredX(void) const
	{
		return *(predx + 0);
	}

	// 获取当前预测y结果.
	double GetPredY(void) const
	{
		return *(predx + 1);
	}

	// 获取速度方差x.
	double GetVX() const 
	{
		return *(predv + 0);
	}

	// 获取速度方差y.
	double GetVY() const 
	{
		return *(predv + 5);
	}

	// 获取当前移动速率.
	double GetSpeed(void) const 
	{
		return sqrt(*(rectx + 2) * *(rectx + 2) + *(rectx + 3) * *(rectx + 3));
	}

	// 状态预测(xNew: 2 * 1).
	void predict(void)
	{
		double T[16], T0[16];

		// 位置预测.
		MatMul(F, rectx, predx, 4, 4, 1);

		// 速度预测.
		MatMul(F, rectv, T, 4, 4, 4);
		MatMul(T, FT, T0, 4, 4, 4);
		MatAdd(T0, Q, predv, 4, 4);
	}

	// 状态更新(xReal: 2 * 1).
	void update(const double *matT, const double* xReal)
	{
		double S[4], SINV[4], T[16], T0[8], T1[4], R[4], T2[2], K[8], E[2];

		// R
		calR(matT, xReal, S);
		MAT_INV22(S, SINV);

		*(S + 0) = *(SINV + 0); *(S + 1) = *(SINV + 2);
		*(S + 2) = *(SINV + 1); *(S + 3) = *(SINV + 3);

		MatMul(SINV, S, R, 2, 2, 2);

		// S
		MatMul(H, predv, T0, 2, 4, 4);
		MatMul(T0, HT, T1, 2, 4, 2);
		MatAdd(T1, R, S, 2, 2);

		// SINV
		MAT_INV22(S, SINV);

		// K
		MatMul(predv, HT, T0, 4, 4, 2);
		MatMul(T0, SINV, K, 4, 2, 2);

		// E
		MatMul(H, predx, T2, 2, 4, 1);
		MatSub(xReal, T2, E, 2, 1);

		// rectx
		MatMul(K, E, T1, 4, 2, 1);
		MatAdd(predx, T1, rectx, 4, 1);

		// rectv
		MatMul(K, H, T, 4, 2, 4);
		MatSub(I, T, T, 4, 4);
		MatMul(T, predv, rectv, 4, 4, 4);
	}

private:
	// 初始化.
	void init(double cx, double cy, double vx, double vy)
	{
		*(rectx + 0) = cx;
		*(rectx + 1) = cy;

		*(rectx + 2) = vx;
		*(rectx + 3) = vy;

		*(rectv +  0) = INIT_P; *(rectv +  1) =      0; *(rectv +  2) =      0; *(rectv +  3) =      0;
		*(rectv +  4) =      0; *(rectv +  5) = INIT_P; *(rectv +  6) =      0; *(rectv +  7) =      0;
		*(rectv +  8) =      0; *(rectv +  9) =      0; *(rectv + 10) = INIT_P; *(rectv + 11) =      0;
		*(rectv + 12) =      0; *(rectv + 13) =      0; *(rectv + 14) =      0; *(rectv + 15) = INIT_P;
	}

	// 扰动计算.
	void calR(const double *matT, const double *xReal, double *r)
	{
		const double M[] = 
		{
			*(matT + 0), *(matT + 2), *(matT + 3), 
			*(matT + 4), *(matT + 6), *(matT + 7), 
			*(matT + 8), *(matT +10), *(matT +11)
		}; 

		const double A = *(M + 0) * *(xReal + 0) + *(M + 1) * *(xReal + 1) + *(M + 2);
		const double B = *(M + 3) * *(xReal + 0) + *(M + 4) * *(xReal + 1) + *(M + 5);
		const double C = *(M + 6) * *(xReal + 0) + *(M + 7) * *(xReal + 1) + *(M + 8);

		*(r + 0) = (*(M + 0) * C - *(M + 6) * A) / (C * C);
		*(r + 1) = (*(M + 1) * C - *(M + 7) * A) / (C * C);
		*(r + 2) = (*(M + 3) * C - *(M + 6) * B) / (C * C);
		*(r + 3) = (*(M + 4) * C - *(M + 7) * B) / (C * C);
	}

private:
	// 预测结果.
	double predx[4 * 1];
	double predv[4 * 4];

	// 预测矩阵.
	double rectx[4 * 1];
	double rectv[4 * 4];
};

#endif