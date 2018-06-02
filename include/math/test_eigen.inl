
#include <Eigen/Dense>
#include <iostream>

template <typename T>
static void matrix_mul_matrix(T* p1, int iRow1, int iCol1, T* p2, int iRow2, int iCol2, T* p3)
{
	if (iRow1 != iRow2) return;

	//列优先 
	//Eigen::Map< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> > map1(p1, iRow1, iCol1);
	//Eigen::Map< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> > map2(p2, iRow2, iCol2);
	//Eigen::Map< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> > map3(p3, iCol1, iCol2);

	//行优先 
	Eigen::Map< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> > map1(p1, iRow1, iCol1);
	Eigen::Map< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> > map2(p2, iRow2, iCol2);
	Eigen::Map< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> > map3(p3, iCol1, iCol2);

	map3 = map1 * map2;
}

#include "cstd.h"

#include <Eigen/Cholesky>
int test_eigen()
{
	{
		int n = 325;
		Eigen::MatrixXd a = Eigen::MatrixXd::Random(n, n);
		Eigen::MatrixXd b = Eigen::MatrixXd::Random(n, n), bt;
		Eigen::MatrixXd c = Eigen::MatrixXd::Random(n, n);
		int i, times = 10;
		bt = b.transpose();
		a = b*bt;
		b = Eigen::MatrixXd::Random(n, 1);
		Eigen::MatrixXd x;
		x  = a.llt().solve(b);
		{
			utime_start(_start_time);
			for (i = 0; i<times; ++i) {
				c = a.inverse();
			}
			printf("%fms\n", 1000 * utime_elapsed(_start_time) / times);
		}

	}
	//1. 矩阵的定义 
	Eigen::MatrixXd m(2, 2), mm(2, 2);
	m = Eigen::MatrixXd::Random(500, 500);
	m = m*m;
	m.triangularView<Eigen::Upper>().adjoint().solveInPlace(mm);
	Eigen::Vector3d vec3d_1;
	Eigen::Vector4d vec4d(1.0, 2.0, 3.0, 4.0);

	//2. 动态矩阵、静态矩阵 
	Eigen::MatrixXd matrixXd;
	Eigen::Matrix3d matrix3d;

	//3. 矩阵元素的访问 
	m(0, 0) = 1;
	m(0, 1) = 2;
	m(1, 0) = m(0, 0) + 3;
	m(1, 1) = m(0, 0) * m(0, 1);
	std::cout << m << std::endl << std::endl;

	//4. 设置矩阵的元素 
	m << -1.5, 2.4,
		6.7, 2.0;
	std::cout << m << std::endl << std::endl;
	int row = 4;
	int col = 5;
	Eigen::MatrixXf matrixXf(row, col);
	matrixXf << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20;
	std::cout << matrixXf << std::endl << std::endl;
	matrixXf << Eigen::MatrixXf::Identity(row, col);
	std::cout << matrixXf << std::endl << std::endl;

	//5. 重置矩阵大小 
	Eigen::MatrixXd matrixXd1(3, 3);
	m = matrixXd1;
	std::cout << m.rows() << " " << m.cols() << std::endl << std::endl;

	//6. 矩阵运算 
	m << 1, 2, 7,
		3, 4, 8,
		5, 6, 9;
	std::cout << m << std::endl;
	matrixXd1 = Eigen::Matrix3d::Random();
	m += matrixXd1;
	std::cout << m << std::endl << std::endl;
	m *= 2;
	std::cout << m << std::endl << std::endl;
	std::cout << -m << std::endl << std::endl;
	std::cout << m << std::endl << std::endl;

	//7. 求矩阵的转置、共轭矩阵、伴随矩阵 
	std::cout << m.transpose() << std::endl << std::endl;
	std::cout << m.conjugate() << std::endl << std::endl;
	std::cout << m.adjoint() << std::endl << std::endl;
	std::cout << m << std::endl << std::endl;
	m.transposeInPlace();
	std::cout << m << std::endl << std::endl;

	//8. 矩阵相乘、矩阵向量相乘
	m = Eigen::MatrixXd::Random(500, 500);
	m = m*m;
	Eigen::MatrixXd aa = m*m;
	std::cout << m*m << std::endl << std::endl;
	vec3d_1 = Eigen::Vector3d(1, 2, 3);
	std::cout << m * vec3d_1 << std::endl << std::endl;
	std::cout << vec3d_1.transpose()*m << std::endl << std::endl;

	//9. 矩阵的块操作 
	std::cout << m << std::endl << std::endl;
	std::cout << m.block(1, 1, 2, 2) << std::endl << std::endl;
	std::cout << m.block<1, 2>(0, 0) << std::endl << std::endl;
	std::cout << m.col(1) << std::endl << std::endl;
	std::cout << m.row(0) << std::endl << std::endl;

	//10. 向量的块操作 
	Eigen::ArrayXf arrayXf(10);
	arrayXf << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;
	std::cout << vec3d_1 << std::endl << std::endl;
	std::cout << arrayXf << std::endl << std::endl;
	std::cout << arrayXf.head(5) << std::endl << std::endl;
	std::cout << arrayXf.tail(4) * 2 << std::endl << std::endl;

	//11. 求解矩阵的特征值和特征向量 
	Eigen::Matrix2f matrix2f;
	matrix2f << 1, 2, 3, 4;
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix2f> eigenSolver(matrix2f);
	if (eigenSolver.info() == Eigen::Success) {
		std::cout << eigenSolver.eigenvalues() << std::endl << std::endl;
		std::cout << eigenSolver.eigenvectors() << std::endl << std::endl;
	}

	//12. 类Map及动态矩阵的使用 
	int array1[4] = { 1, 2, 3, 4 };
	int array2[4] = { 5, 6, 7, 8 };
	int array3[4] = { 0, 0, 0, 0 };
	matrix_mul_matrix(array1, 2, 2, array2, 2, 2, array3);
	for (int i = 0; i < 4; i++)
		std::cout << array3[i] << std::endl;


	return 0;
}

#define USESYS
#include "lib.inl"
