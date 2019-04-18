# Java实现算法导论中最近点对问题分治法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月07日 10:38:19[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4849
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









                
最近点对问题：给定平面上的N个点，找出距离最近的两个点。分治法：

             1 ）如果数组长度(即点的个数，一般≤3)在一定范围内时直接求出最近点，蛮力求解，递归退出条件；

             2）求出这些点的X坐标的中位数mid

             3）以mid为界将所有点分为两组，分表放在表T1、T2中

             4）将T1、T2表转换为数组S1、S2，并将S1、S2分别按X坐标升序排列

             5）求S1中的点的最近距离

             6） 求S2中点的最近距离

             7）求4、5中的两距离的最小值，记为d1

             8）在S1、S2中搜集距离中线(x=mid)小于d1的点，分别存放于表T3、T4中

             9）将表T3、T4转换为数组类型S3、S4，并将S3、S4分别按Y坐标升序排列

             10）求S3、S4两者之间可能的最近距离(与d1作比较)

重温分治法思想也是必要，策略是：对于一个规模为n的问题，若该问题可以容易地解决（比如说规模n较小）则直接解决，否则将其分解为k个规模较小的子问题，这些子问题互相独立且与原问题形式相同，递归地解这些子问题，然后将各子问题的解合并得到原问题的解。

分治法在每一层递归上都有三个步骤：

    step1 分解：将原问题分解为若干个规模较小，相互独立，与原问题形式相同的子问题；

    step2 解决：若子问题规模较小而容易被解决则直接解，否则递归地解各个子问题

    step3 合并：将各个子问题的解合并为原问题的解。





计算几何中最近点对求解问题的分治法，代码参考：



```java
package cn.ansj;

import java.util.ArrayList;
import java.util.Random;
import java.util.Set;
import java.util.TreeSet;

public class NPointPair {
	/**
	 * 最近点问题
	 * @param S
	 */
	public static dcPoint[] closestPoint(dcPoint [] S){
		
		dcPoint[] result = new dcPoint[2];	
		/**
		 * 0.首先，解决该问题的边界，当数组长度在一定范围内时直接求出最近点，蛮力求解 
		 */
		double dmin = Double.POSITIVE_INFINITY;
		double tmpmin = 0;
		if(S.length <= 20){
			for(int i = 0; i < S.length; i ++){
				for(int j = i + 1; j < S.length; j ++){
					tmpmin = Math.sqrt(Math.pow(S[i].getX() - S[j].getX(), 2)) + Math.pow(S[i].getY() - S[j].getY(), 2);
					if(tmpmin < dmin){
						dmin = tmpmin;
						result[0] = S[i];
						result[1] = S[j];
					}
				}
			}
			return result;
		}
		
		/**
		 *1.求所有点在X坐标的中位数 
		*/
		int minX = (int) Double.POSITIVE_INFINITY;		//保证假设的初始最小值足够大
		int maxX = (int) Double.NEGATIVE_INFINITY;		//保证假设的初始最大值足够小
		for(int i = 0; i < S.length; i++){
			if(S[i].getX() < minX)
				minX = S[i].getX();
			if(S[i].getX() > maxX)
				maxX = S[i].getX();
		}
		int midX = (minX + maxX)/2;
		
		/**
		 * 2.以midX为界将所有点分成两组分别存放在两个表中
		 */
		ArrayList T1 = new ArrayList();
		ArrayList T2 = new ArrayList();
		for(int i = 0; i < S.length; i++){
			if(S[i].getX() <= midX)		//是否要=号？
				T1.add(S[i]);
			if(S[i].getX() > midX)
				T2.add(S[i]);
		}
		
		/**
		 * 3.将两张表转化为数组类型，并分别按X坐标升序排列
		 */
		dcPoint [] S1 = new dcPoint[T1.size()];
		dcPoint [] S2 = new dcPoint[T2.size()];
		
		T1.toArray(S1);
		T2.toArray(S2);
		
		mergeSort(S1, "x");		//按X坐标升序排列
		mergeSort(S2, "x");		//按X坐标升序排列
		
		/**
		 * 4.求S1中的最近距离的两个点
		 */
		dcPoint[] result1 = new dcPoint[2];
		result1 = closestPoint(S1);
		
		/**
		 * 5.求S2中的最近距离的两个点
		 */
		dcPoint[] result2 = new dcPoint[2];
		result2 = closestPoint(S2);
		
		/**
		 * 6.求两最近距离的最小值
		 */
		double d1 = Math.sqrt(Math.min(Math.pow(result1[0].getX() - result1[1].getX(), 2) + Math.pow(result1[0].getY() - result1[1].getY(), 2), 
				Math.pow(result2[0].getX() - result2[1].getX(), 2) + Math.pow(result2[0].getY() - result2[1].getY(), 2)));
		
		if(Math.pow(result1[0].getX() - result1[1].getX(), 2) + Math.pow(result1[0].getY() - result1[1].getY(), 2) < 
				Math.pow(result2[0].getX() - result2[1].getX(), 2) + Math.pow(result2[0].getY() - result2[1].getY(), 2))
			result = result1;
		else
			result = result2;
		
		/**
		 * 7.在S1、S2中收集距离中线小于d1的点，分别存放于两个表中
		 */
		ArrayList T3 = new ArrayList();
		ArrayList T4 = new ArrayList();
		for(int i = 0; i < S1.length; i++){
			if(midX - S1[i].getX() < d1)
				T3.add(S1[i]);
		}
		for(int i = 0; i < S2.length; i++){
			if(S2[i].getX() - midX < d1){
				T4.add(S2[i]);
			}
		}
		
		/**
		 * 8.分别将表T3、T4转换为数组类型的S3、S4，并将其分别按Y坐标升序排列
		 */
		dcPoint [] S3 = new dcPoint [T3.size()];
		dcPoint [] S4 = new dcPoint [T4.size()];
		T3.toArray(S3);
		T4.toArray(S4);
		
		mergeSort(S3, "y");
		mergeSort(S4, "y");
		
		/**
		 * 求解S3、S4两者之间可能的更近（相比于d1）距离 , 以及构成该距离的点
		 */
		double d =  Double.POSITIVE_INFINITY;
		for(int i = 0; i < S3.length; i ++){
			for(int j = 0; j < S4.length; j ++){
				if(Math.abs(S3[i].getY() - S4[j].getY()) < d1){
					double tmp = Math.sqrt(Math.pow(S3[i].getX() - S4[j].getX(), 2) + Math.pow(S3[i].getY() - S4[j].getY(), 2));
					if(tmp < d){
						d = tmp;
						result[0] = S3[i];
						result[1] = S4[j];
					}
				} 
			}
		}
		
		return result;
	}
	
	private static void mergeSort(dcPoint[] a, String property){
		dcPoint[] tempArray = new dcPoint[a.length];
		mergeSort(a, tempArray, 0, a.length - 1, property);
	}
	
	private static void mergeSort(dcPoint[] a, dcPoint [] tempArray, int left, int right, String property){
		if(left < right){
			int center = (left + right) >> 1;
			//分治
			mergeSort(a, tempArray, left, center, property);
			mergeSort(a, tempArray, center + 1, right, property);
			//合并
			merge(a, tempArray, left, center + 1, right, property);
		}
	}
	
	private static void merge(dcPoint [] a, dcPoint [] tempArray, int leftPos, int rightPos, int rightEnd, String property){
		int leftEnd = rightPos - 1;
		int numOfElements = rightEnd - leftPos + 1;
		
		int tmpPos = leftPos;		//游标变量, 另两个游标变量分别是leftPos 和 rightPos
		
		while(leftPos <= leftEnd && rightPos <= rightEnd){
			if(property.equals("x")){
				if(a[leftPos].getX() <= a[rightPos].getX())
					tempArray[tmpPos++] = a[leftPos++];
				else
					tempArray[tmpPos++] = a[rightPos++];
			}else if(property.equals("y")){
				if(a[leftPos].getY() <= a[rightPos].getY())
					tempArray[tmpPos++] = a[leftPos++];
				else
					tempArray[tmpPos++] = a[rightPos++];
			}else
				throw new RuntimeException();
		}
		
		while(leftPos <= leftEnd)
			tempArray[tmpPos++] = a[leftPos++];
		while(rightPos <= rightEnd)
			tempArray[tmpPos++] = a[rightPos++];
		
		//将排好序的段落拷贝到原数组中
		System.arraycopy(tempArray, rightEnd-numOfElements+1, a, rightEnd-numOfElements+1, numOfElements);
	}
	
	public static void main(String[] args) {
		
		Set<dcPoint> testData = new TreeSet<dcPoint>();
		
		Random random = new Random();
		
		int x = 0;
		int y = 0;
		
		for(int i = 0;i < 50;i++){
			x = random.nextInt(500);
			y = random.nextInt(500);
			System.out.println("x:" + x + "  y:" + y);
			testData.add(new dcPoint(x, y));
		}
		
		dcPoint [] S = new dcPoint[testData.size()];
		S = (dcPoint[]) testData.toArray(S);
		
		for(int i = 0; i < S.length; i ++){
			System.out.println("(" + S[i].getX() + ", " + S[i].getY() + ")");
		}
		
		System.out.println(testData.size());
		
		dcPoint [] result = new dcPoint [2];
		
		result = closestPoint(S);
		
		System.out.println("最近的两点分别是(" + result[0].getX() + ", " + result[0].getY() 
				+ ") 和 (" + result[1].getX() + ", " + result[1].getY() + "), 最近距离为：" 
				+ Math.sqrt(Math.pow(result[0].getX() - result[1].getX(), 2) + Math.pow(result[0].getY() - result[1].getY(), 2)));
		
	}
}
```

```java
package cn.ansj;

public class dcPoint implements Cloneable, Comparable<dcPoint>{
	public dcPoint() {
		x = 0;
		y = 0;
	}

	public dcPoint(int x, int y) {
		this.x = x;
		this.y = y;
	}

	public void setX(int x) {
		this.x = x;
	}

	public void setY(int y) {
		this.y = y;
	}

	public int getX() {
		return x;
	}

	public int getY() {
		return y;
	}

	private int x;
	private int y;

	@Override
	public int compareTo(dcPoint o) {
		if(x == o.getX() && y == o.getY())
			return 0;
		else 
			return 1;
	}
}
```


执行结果：



```
x:347  y:149
x:78  y:95
x:435  y:277
x:291  y:346
x:314  y:30
x:421  y:264
x:428  y:72
x:274  y:237
x:296  y:404
x:10  y:217
x:149  y:230
x:172  y:152
x:76  y:434
x:336  y:488
x:403  y:441
x:442  y:89
x:4  y:328
x:368  y:375
x:12  y:297
x:455  y:296
x:388  y:13
x:402  y:244
x:410  y:310
x:247  y:269
x:417  y:481
x:261  y:279
x:478  y:39
x:364  y:2
x:391  y:410
x:268  y:485
x:118  y:195
x:11  y:456
x:167  y:380
x:405  y:309
x:243  y:324
x:343  y:195
x:45  y:266
x:70  y:375
x:487  y:183
x:282  y:400
x:466  y:128
x:498  y:310
x:35  y:457
x:150  y:272
x:258  y:433
x:23  y:44
x:485  y:146
x:301  y:23
x:71  y:124
x:383  y:146
(347, 149)
(78, 95)
(435, 277)
(291, 346)
(314, 30)
(421, 264)
(428, 72)
(274, 237)
(296, 404)
(10, 217)
(149, 230)
(172, 152)
(76, 434)
(336, 488)
(403, 441)
(442, 89)
(4, 328)
(368, 375)
(12, 297)
(455, 296)
(388, 13)
(402, 244)
(410, 310)
(247, 269)
(417, 481)
(261, 279)
(478, 39)
(364, 2)
(391, 410)
(268, 485)
(118, 195)
(11, 456)
(167, 380)
(405, 309)
(243, 324)
(343, 195)
(45, 266)
(70, 375)
(487, 183)
(282, 400)
(466, 128)
(498, 310)
(35, 457)
(150, 272)
(258, 433)
(23, 44)
(485, 146)
(301, 23)
(71, 124)
(383, 146)
50
最近的两点分别是(405, 309) 和 (410, 310), 最近距离为：5.0990195135927845
```






