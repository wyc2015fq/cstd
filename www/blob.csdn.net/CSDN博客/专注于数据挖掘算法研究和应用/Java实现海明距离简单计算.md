# Java实现海明距离简单计算 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月18日 14:35:08[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4614








文本相似度比较有很多方法，如余弦夹角算法、欧式距离、Jaccard相似度、最长公共子串、编辑距离等，海明距离是其中之一。

在信息编码中，两个合法代码对应位上编码不同的位数称为码距，又称海明距离。


n位的码字可以用n维空间的超立方体的一个顶点来表示。两个码字之间的海明距离就是超立方体两个顶点之间的一条边，而且是这两个顶点之间的最短距离。


对海明距离的应用，最多的是在海量短文本去重上，性能优，主要方法就是对文本进行向量化，或者说把文本的特征抽取出来映射成编码，然后再对编码进行异或计算出海明距离。

曾遇到的一个场景是：从庞大的记录文本中，判断文本的相似度并做分类。一般做法就是两两比较相似度，这时对相似度计算算法的耗时要求就比较高，一旦数据量庞大，就能立见高下。有兴趣的，可以试验下cos、欧式、海明三者在文本相似度判断方面的性能，比如2000万记录的文本。

本文这里的代码只是简单的模拟了海明距离的计算过程，参考如下：



```java
package sk.ml;

/*
 * 功能：计算两个文本海明距离
```

```java
先字符串二进制，再统计差异位数
```

```java
* 作者：Jason.F
 * 时间：2017年1月18日
 */

public class HammingDistance {
	public static void main(String[] args) {
		
		String strA="大数据和人工智能";
		String strB="小数据和人工智能";
		String strAB=StrToBinstr(strA);
		String strBB=StrToBinstr(strB);
		//对两个二进制字符串，字符数相同，统计差异数
		int count=0;
		int len=strAB.length();
		for(int i=0;i<len;i++){
			if(strAB.charAt(i)!=strBB.charAt(i)) count++;
		}
		System.out.println("海明距离是："+count);
	}
	
	//将字符串转换成二进制字符串，以空格相隔
	public static String StrToBinstr(String str) {

		char[] strChar=str.toCharArray();
		String result="";
		for(int i=0;i<strChar.length;i++){
			result +=Integer.toBinaryString(strChar[i]);
		}
		return result;
	}
}
```
执行结果：
`海明距离是：4`




