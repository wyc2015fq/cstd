# EKF优化：协方差coff公式、意义、SLAM中的Code优化 - wishchinYang的专栏 - CSDN博客
2015年11月13日 10:13:28[wishchin](https://me.csdn.net/wishchin)阅读数：2618
复习！复习！原文链接：[http://blog.csdn.net/goodshot/article/details/8611178](http://blog.csdn.net/goodshot/article/details/8611178)
![](https://img-blog.csdn.net/20151113101426526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
1.代码：Matlab相关系数的意义：
```cpp
Eigen::MatrixXf  correlation_matrix = corrcoef( LocM );
```
对行向量求相关系数 , 与列数无关，返回 cols()*cols() 矩阵...
翻译成Eigen：
还是自己写个函数吧//1.求协方差
```cpp
Eigen::MatrixXf CIcSearchM::cov(Eigen::MatrixXf &d1, Eigen::MatrixXf &d2)
{
    Eigen::MatrixXf  CovM(1,1);
    assert(1 ==d1.cols() && 1 ==d2.cols() &&d1.cols()==d2.cols()  );
    //求协方差
    float Ex =0;float Ey=0;
    for (int i=0;i< d1.rows();++i){
        Ex +=d1(i);
        Ey +=d2(i);
    }
    Ex /=d1.rows();
    Ey /=d2.rows();
    for (int i=0;i< d1.rows();++i){
        CovM(0) += (d1(i)-Ex)*(d2(i)-Ey);
    }
    CovM(0) /= d1.rows() -1;
    return CovM;
}
```
//2.写入方差矩阵
```cpp
//求矩阵的相关系数！
//返回矩阵A的列向量的相关系数矩阵//对行向量求相关系数 , 与行数无关，返回 cols()*cols() 矩阵...
Eigen::MatrixXf CIcSearchM::corrcoef(Eigen::MatrixXf &M)
{
	// C(i,j)/SQRT(C(i,i)*C(j,j)).//C is the covariation Matrix
	int Row= M.rows();
	int Col= M.cols();
	int Order= Col;//int Order= (std::max)(Row,Col);
	Eigen::MatrixXf Coef(Order,Order);
	for (int i=0;i<Order;++i){
		for (int j=0;j<Order;++j){
			Coef(i,j)= cov((Eigen::MatrixXf)M.col(i),(Eigen::MatrixXf)M.col(j))(0);
		}
	}
	return Coef;
}
```
**2.优化的代码**
使用Eigen计算1000维的方阵大概需要200ms的时间，相对于matlab默认开启GPU加速，时间上消耗的太多了。
参考：[比较OpenBLAS、Matlab、MKL、Eigen的基础计算性能](http://www.leexiang.com/the-performance-of-matrix-multiplication-among-openblas-intel-mkl-and-eigen)
优化的代码：
```cpp
//求矩阵的相关系数！一个原始公式的简化算法/优化算法
//返回矩阵A的列向量的相关系数矩阵//对行向量求相关系数 , 与行数无关，返回 cols()*cols() 矩阵...
Eigen::MatrixXf CIcSearchM::CorrcoefOpm(Eigen::MatrixXf &MI)
{
	Eigen::MatrixXf M =MI;
	// C(i,j)/SQRT(C(i,i)*C(j,j)).//C is the covariation Matrix
	//公式：
	//temp = mysample - repmat(mean(mysample), 10, 1);
	//result = temp' * temp ./ (size(mysample, 1) - 1)
	int Row= M.rows();
	int Col= M.cols();
	int Order= Col;//int Order= (std::max)(Row,Col);
	SYSTEMTIME sysP; 
	GetLocalTime( &sysP ); 
	int MileTsp = sysP.wSecond;
	int MileTP = sysP.wMilliseconds;
	Eigen::MatrixXf  CovM(Order,Order);//(1,Col);
	Eigen::MatrixXf  E_M(1,Col);
	//减去每一个维度的均值;确定一列为一个维度。
	//std::cout<< "Mat Src :"<<std::endl;m_Testor.print_EigenMat( M);
	for (int i =0;i< Col;++i)
	{
		//求均值
		E_M(i) =M.col(i).sum()/M.rows();
		//std::cout<< "E_M(i)" << E_M(i)<< std::endl;
		M.col(i) = M.col(i)- E_M(i);
		//
	}
	//SYSTEMTIME sysP2; 
	//GetLocalTime( &sysP ); 
	//int MileTsp2 = sysP.wSecond;
	//int MileTP2 = sysP.wMilliseconds;
	//int  DetaTp = MileTP2  - MileTP;
	//int DetaTsp = MileTsp2 -MileTsp;
	//std::cout<< "The Process time is :"<< DetaTsp<<"S"<< std::endl;
	//std::cout<< "The Process time is :"<< DetaTp<<"mS"<< std::endl;
	//std::cout<< "Mat E_M :"<<std::endl;m_Testor.print_EigenMat( M);
	CovM = M.transpose();
	//GetLocalTime( &sysP ); 
	//MileTsp2 = sysP.wSecond;
	//MileTP2 = sysP.wMilliseconds;
	//DetaTp = MileTP2  - MileTP;
	//DetaTsp = MileTsp2 -MileTsp;
	//std::cout<< "The Process time is :"<< DetaTsp<<"S"<< std::endl;
	//std::cout<< "The Process time is :"<< DetaTp<<"mS"<< std::endl;
	//std::cout<< "Mat CovM :"<<std::endl;m_Testor.print_EigenMat( CovM);
	CovM = CovM * M ;
	//GetLocalTime( &sysP ); 
	//MileTsp2 = sysP.wSecond;
	//MileTP2 = sysP.wMilliseconds;
	//DetaTp = MileTP2  - MileTP;
	//DetaTsp = MileTsp2 -MileTsp;
	//std::cout<< "The Process time is :"<< DetaTsp<<"S"<< std::endl;
	//std::cout<< "The Process time is :"<< DetaTp<<"mS"<< std::endl;
	//实现 ./ 函数 数值计算没有区别
	CovM = CovM /(Order-1)/(Order-1);
	//GetLocalTime( &sysP ); 
	//MileTsp2 = sysP.wSecond;
	//MileTP2 = sysP.wMilliseconds;
	//DetaTp = MileTP2  - MileTP;
	//DetaTsp = MileTsp2 -MileTsp;
	//std::cout<< "The Process time is :"<< DetaTsp<<"S"<< std::endl;
	//std::cout<< "The Process time is :"<< DetaTp<<"mS"<< std::endl;
	//std::cout<< "Mat CovM :"<<std::endl;m_Testor.print_EigenMat( CovM);
	//GetLocalTime( &sysP ); 
	//MileTsp2 = sysP.wSecond;
	//MileTP2 = sysP.wMilliseconds;
	//DetaTp = MileTP2  - MileTP;
	//DetaTsp = MileTsp2 -MileTsp;
	//std::cout<< "The Process time is :"<< DetaTsp<<"S"<< std::endl;
	//std::cout<< "The Process time is :"<< DetaTp<<"mS"<< std::endl;
	//遍历一次
	for (int i=0;i< Order;++i){
		for (int j=0;j<Order;++j){
			CovM(i,j) = sqrt(CovM(i,i)*CovM(j,j) );
		}
	}
	//GetLocalTime( &sysP ); 
	//MileTsp2 = sysP.wSecond;
	//MileTP2 = sysP.wMilliseconds;
	//DetaTp = MileTP2  - MileTP;
	//DetaTsp = MileTsp2 -MileTsp;
	//std::cout<< "The Process time is :"<< DetaTsp<<"S"<< std::endl;
	//std::cout<< "The Process time is :"<< DetaTp<<"mS"<< std::endl;
	//std::cout<< "Mat CovM :"<<std::endl;m_Testor.print_EigenMat( CovM);
	return CovM;
}
```
 稀疏矩阵可以加速到3ms，我去！终于可以实用了.....
