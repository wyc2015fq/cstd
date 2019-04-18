# Ansj中文分词Java开发小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月29日 14:16:55[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4143








1、ansj中文分词器：https://github.com/NLPchina/ansj_seg




   基于n-Gram+CRF+HMM的中文分词的java实现.

   分词速度达到每秒钟大约200万字左右（mac air下测试），准确率能达到96%以上；

   目前实现了中文分词、中文姓名识别、用户自定义词典、关键字提取、自动摘要、关键字标记等功能；

   可以应用到自然语言处理等方面,适用于对分词效果要求高的各种项目。




2、http://maven.nlpcn.org/org/ansj/ansj_seg/

   下载ansj_seg-5.0.2-all-in-one.jar，引入eclipse工程；




3、代码：



```java
package cn.ansj;

import org.ansj.splitWord.analysis.ToAnalysis;

public class SplitWordsByAnsj {
	
	public static void main(String args[]) {
		String str = "欢迎使用ansj_seg,(ansj中文分词)在这里如果你遇到什么问题都可以联系我.我一定尽我所能.帮助大家.ansj_seg更快,更准,更自由!" ;
		System.out.println(ToAnalysis.parse(str));
	}
	
}
```





