# 机器学习知识点(八)感知机模型Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年02月06日 10:50:03[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3248
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









###  感知机模型


假设输入数据集![1.png](http://7xnjry.com1.z0.glb.clouddn.com/2015/11/201511212762_7584.png)为表示是n个训练数据样本。输出y只有两个值(-1,+1)两个分类。那么感知机模型可以表示为以下函数:

![2.png](http://7xnjry.com1.z0.glb.clouddn.com/2015/11/201511212423_8437.png)


其中sign是符号函数，意义如下。w表示权值，b表示偏置。我们就是需要通过一定的策略和算法求出其w和b，就可以得到相关的感知机模型。


援引网上参考代码如下：



```java
package sk.ann;

import java.util.ArrayList;
import java.util.Arrays;

public class PerceptronClassifier {
		 /* 分类器参数
		 */
		private double[] w;//权值向量组
		private double b = 0;//阈值
		private double eta = 1;
		ArrayList<Point> arrayList;
		/**
		 * 初始化分类器，传入需要分组的数据
		 * @param arrayList  需要分类的点
		 */
		public PerceptronClassifier(ArrayList<Point> arrayList, double eta) {
			// 分类器初始化
			this.arrayList = arrayList;
			w = new double[arrayList.get(0).x.length];
			this.eta = eta;
		}
		public PerceptronClassifier(ArrayList<Point> arrayList) {
			// 分类器初始化
			this.arrayList = arrayList;
			w = new double[arrayList.get(0).x.length];
			this.eta = 1;
		}

		/**
		 * 进行分类计算
		 * @return 是否分类成功
		 */
		public boolean Classify() {
			boolean flag = false;
			while (!flag) {
				for (int i = 0; i < arrayList.size(); i++) {//所有训练集
					if (LearnAnswer(arrayList.get(i)) <= 0) {
						UpdateWAndB(arrayList.get(i));
						break;
					}
					if (i == (arrayList.size() - 1)) {
						flag = true;
					}
				}
			}
			System.out.println(Arrays.toString(w));
			System.out.println(b);
			return true;
		}

		/**
		 * 进行学习得到的结果
		 * @param point 需要进行学习的点，训练样本
		 * @return
		 */
		private double LearnAnswer(Point point) {
			System.out.println(Arrays.toString(w));
			System.out.println(b);
			return point.y * (DotProduct(w, point.x) + b);
		}

		/**
		 * 进行w更新
		 * @param point 需要根据样本来随机梯度下降来进行w和b更新
		 * @return 不需要返回值
		 */
		private void UpdateWAndB(Point point) {
			
			for (int i = 0; i < w.length; i++) {
				w[i] += eta * point.y * point.x[i];
			}
			b += eta * point.y;
			return;
		}

		/**
		 * 进行点乘
		 * @param x1 乘数
		 * @param x2 乘数
		 * @return 点乘的积
		 */
		private double DotProduct(double[] x1, double[] x2) {
			int len = x1.length;
			double sum = 0;
			for (int i = 0; i < len; i++) {
				sum += x1[i] * x2[i];
			}
			return sum;
		}

		/**
		 * 主程序进行检测
		 * @param args
		 */
		public static void main(String[] args) {
		    
			Point p1 = new Point(new double[] { 0,0,0,1 }, -1);
			Point p2 = new Point(new double[] { 1,0,0,0 }, 1);
			Point p3 = new Point(new double[] { 2,1,0,0 }, 1);
			Point p4 = new Point(new double[] { 2,1,0,1 }, -1);
			ArrayList<Point> list = new ArrayList<Point>();
			list.add(p1);
			list.add(p2);
			list.add(p3);
			list.add(p4);
			PerceptronClassifier classifier = new PerceptronClassifier(list);
			classifier.Classify();
		}
}
		/**
		 * 定义一个Point,里面包含两个部分，用来分类。x表示输入R维空间向量，y表示分类值，只有-1和+1两类
		 */
		class Point {
			double[] x = new double[2];
			double y = 0;

			Point(double[] x, double y) {
				this.x = x;
				this.y = y;
			}
			public Point() {
			}
		}
```
执行结果如下：



```
[0.0, 0.0, 0.0, 0.0]
0.0
[0.0, 0.0, 0.0, -1.0]
-1.0
[0.0, 0.0, 0.0, -1.0]
-1.0
[1.0, 0.0, 0.0, -1.0]
0.0
[1.0, 0.0, 0.0, -1.0]
0.0
[1.0, 0.0, 0.0, -1.0]
0.0
[1.0, 0.0, 0.0, -1.0]
0.0
[-1.0, -1.0, 0.0, -2.0]
-1.0
[-1.0, -1.0, 0.0, -2.0]
-1.0
[0.0, -1.0, 0.0, -2.0]
0.0
[0.0, -1.0, 0.0, -2.0]
0.0
[1.0, -1.0, 0.0, -2.0]
1.0
[1.0, -1.0, 0.0, -2.0]
1.0
[1.0, -1.0, 0.0, -2.0]
1.0
[1.0, -1.0, 0.0, -2.0]
1.0
[-1.0, -2.0, 0.0, -3.0]
0.0
[-1.0, -2.0, 0.0, -3.0]
0.0
[0.0, -2.0, 0.0, -3.0]
1.0
[0.0, -2.0, 0.0, -3.0]
1.0
[0.0, -2.0, 0.0, -3.0]
1.0
[2.0, -1.0, 0.0, -3.0]
2.0
[2.0, -1.0, 0.0, -3.0]
2.0
[2.0, -1.0, 0.0, -3.0]
2.0
[2.0, -1.0, 0.0, -3.0]
2.0
[0.0, -2.0, 0.0, -4.0]
1.0
[0.0, -2.0, 0.0, -4.0]
1.0
[0.0, -2.0, 0.0, -4.0]
1.0
[2.0, -1.0, 0.0, -4.0]
2.0
[2.0, -1.0, 0.0, -4.0]
2.0
[2.0, -1.0, 0.0, -4.0]
2.0
[2.0, -1.0, 0.0, -4.0]
2.0
[0.0, -2.0, 0.0, -5.0]
1.0
[0.0, -2.0, 0.0, -5.0]
1.0
[0.0, -2.0, 0.0, -5.0]
1.0
[2.0, -1.0, 0.0, -5.0]
2.0
[2.0, -1.0, 0.0, -5.0]
2.0
[2.0, -1.0, 0.0, -5.0]
2.0
[2.0, -1.0, 0.0, -5.0]
2.0
[0.0, -2.0, 0.0, -6.0]
1.0
[0.0, -2.0, 0.0, -6.0]
1.0
[0.0, -2.0, 0.0, -6.0]
1.0
[2.0, -1.0, 0.0, -6.0]
2.0
[2.0, -1.0, 0.0, -6.0]
2.0
[2.0, -1.0, 0.0, -6.0]
2.0
[2.0, -1.0, 0.0, -6.0]
2.0
[2.0, -1.0, 0.0, -6.0]
2.0
```
这个代码实现具有特定分类标记，如1和-1。





