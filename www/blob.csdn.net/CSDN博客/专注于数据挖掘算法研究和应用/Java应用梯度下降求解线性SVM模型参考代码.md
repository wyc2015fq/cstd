# Java应用梯度下降求解线性SVM模型参考代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月21日 16:55:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2586








下面的代码是参考网上的，直接执行，主要是为了后续进一步掌握SVM原理而发布。

两个基本原理还是要去掌握：SVM原理和梯度下降法。

1）SVM分类器：

支持向量机，因其英文名为support vector machine，故一般简称SVM，通俗来讲，它是一种二类分类模型，其基本模型定义为特征空间上的间隔最大的线性分类器，其学习策略便是间隔最大化，最终可转化为一个凸二次规划问题的求解。


我自己的理解是：特征空间上寻找一个最优平面来分类，这个最优的求解其实就是多维的约束规划问题。




2）梯度下降法：

梯度下降法，就是利用负梯度方向来决定每次迭代的新的搜索方向，使得每次迭代能使待优化的目标函数逐步减小。梯度下降法是2范数下的最速下降法。 最速下降法的一种简单形式是：x(k+1)=x(k)-a*g(k),其中a称为学习速率，可以是较小的常数。g（k）是x(k)的梯度。


我自己的理解是：切线求导数，梯度下降法是求解SVM的一种方法。




在实际文本分类中，怎么求解SVM，应该要根据实际来选择方法，如拉格朗日、对偶、核函数等，如果理解超平面比较复杂的话，可以用二维平面及其点到直线的距离来抽象理解多维度超平面空间的分类。






```java
package sk.svm;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.StringTokenizer;

//梯度下降法
public class SimpleSvm {
	
	private int exampleNum;
	private int exampleDim;
	private double[] w;
	private double lambda;
	private double lr = 0.001;//0.00001
	private double threshold = 0.001;
	private double cost;
	private double[] grad;
	private double[] yp;
	public SimpleSvm(double paramLambda){
		lambda = paramLambda;		
	}
	
	private void CostAndGrad(double[][] X,double[] y){//梯度求解
		cost =0;
		for(int m=0;m<exampleNum;m++){
			yp[m]=0;
			for(int d=0;d<exampleDim;d++){
				yp[m]+=X[m][d]*w[d];
			}
			
			if(y[m]*yp[m]-1<0){
				cost += (1-y[m]*yp[m]);
			}
			
		}
		
		for(int d=0;d<exampleDim;d++){
			cost += 0.5*lambda*w[d]*w[d];
		}
		

		for(int d=0;d<exampleDim;d++){
			grad[d] = Math.abs(lambda*w[d]);	
			for(int m=0;m<exampleNum;m++){
				if(y[m]*yp[m]-1<0){
					grad[d]-= y[m]*X[m][d];
				}
			}
		}				
	}
	
	private void update(){
		for(int d=0;d<exampleDim;d++){
			w[d] -= lr*grad[d];
		}
	}
	
	public void Train(double[][] X,double[] y,int maxIters){
		exampleNum = X.length;
		if(exampleNum <=0) {
			System.out.println("num of example <=0!");
			return;
		}
		exampleDim = X[0].length;
		w = new double[exampleDim];
		grad = new double[exampleDim];
		yp = new double[exampleNum];
		
		for(int iter=0;iter<maxIters;iter++){
			
			CostAndGrad(X,y);
			System.out.println("cost:"+cost);
			if(cost< threshold){
				break;
			}
			update();	
		}
	}
	private int predict(double[] x){
		double pre=0;
		for(int j=0;j<x.length;j++){
			pre+=x[j]*w[j];
		}
		if(pre >=0)//这个阈值一般位于-1到1
			return 1;
		else return -1;
	}
	
	public void Test(double[][] testX,double[] testY){
		int error=0;
		for(int i=0;i<testX.length;i++){
			if(predict(testX[i]) != testY[i]){
				error++;
			}
		}
		System.out.println("total:"+testX.length);
		System.out.println("error:"+error);
		System.out.println("error rate:"+((double)error/testX.length));
		System.out.println("acc rate:"+((double)(testX.length-error)/testX.length));
	}
	
	
	
	public static void loadData(double[][]X,double[] y,String trainFile) throws IOException{
		
		File file = new File(trainFile);
		RandomAccessFile raf = new RandomAccessFile(file,"r");
		StringTokenizer tokenizer,tokenizer2; 

		int index=0;
		while(true){
			String line = raf.readLine();
			
			if(line == null) break;
			tokenizer = new StringTokenizer(line," ");
			y[index] = Double.parseDouble(tokenizer.nextToken());
			while(tokenizer.hasMoreTokens())
			{
				tokenizer2 = new StringTokenizer(tokenizer.nextToken(),":");
				int k = Integer.parseInt(tokenizer2.nextToken());
				double v = Double.parseDouble(tokenizer2.nextToken());
				X[index][k] = v;				
			}	
			X[index][0] =1;
			index++;		
		}
	}
	
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		double[] y = new double[400];
		double[][] X = new double[400][11];
		String trainFile = "D:\\tmp\\train_bc";
		loadData(X,y,trainFile);
		
		
		SimpleSvm svm = new SimpleSvm(0.0001);
		svm.Train(X,y,7000);
		
		double[] test_y = new double[283];
		double[][] test_X = new double[283][11];
		String testFile = "D:\\tmp\\test_bc";
		loadData(test_X,test_y,testFile);
		svm.Test(test_X, test_y);
		
	}
}
```


源代码和数据集下载：https://github.com/linger2012/simpleSvm





