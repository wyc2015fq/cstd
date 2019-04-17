# Ansj中文分词Java开发自定义和过滤词库 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月23日 15:39:05[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：10865








Ansj中文分词应用时，需要自定义词库，比如城中村，分词成城、中、村，需自定义词库，有时，也需要过滤单词。具体代码如下，可以结合执行结果看代码效果。

1、过滤词库



```java
package csc.ansj;

import org.ansj.domain.Result;
import org.ansj.recognition.impl.FilterRecognition;
import org.ansj.splitWord.analysis.ToAnalysis;

public class AnsjWordFilter {
	public static void main(String[] args) {
		String str = "不三不四，您好！欢迎使用ansj_seg,深圳有没有城中村这里有宽带吗?(ansj中文分词)在这里如果你遇到什么问题都可以联系我.我一定尽我所能.帮助大家.ansj_seg更快,更准,更自由!" ;
		//过滤词性和词汇
		FilterRecognition fitler = new FilterRecognition();
		//http://nlpchina.github.io/ansj_seg/content.html?name=词性说明
		fitler.insertStopNatures("w"); //过滤标点符号词性
		fitler.insertStopNatures("null");//过滤null词性
		fitler.insertStopNatures("m");//过滤m词性
		fitler.insertStopWord("不三不四"); //过滤单词
		fitler.insertStopRegex("城.*?"); //支持正则表达式
		Result modifResult = ToAnalysis.parse(str).recognition(fitler); //过滤分词结果
		System.out.println(modifResult.toString());
	}
}
/*
 * 您好/l,欢迎/v,使用/v,ansj/en,seg/en,深圳/ns,有/v,没/d,有/v,村/n,这里/r,有/v,宽带/nz,吗/y,ansj/en,中文/nz,分词/n,在/p,这里/r,如果/c,你/r,遇到/v,什么/r,问题/n,都/d,可以/v,联系/v,我/r,我/r,一定/d,尽/v,我/r,所/u,能/v,帮助/v,大家/r,ansj/en,seg/en,更/d,快/a,更/d,准/a,更/d,自由/a
 */
```


2、自定义词库，可以设置歧义词等



```java
package csc.ansj;

import org.ansj.app.summary.SummaryComputer;
import org.ansj.domain.Result;
import org.ansj.domain.Term;
import org.ansj.library.UserDefineLibrary;
import org.ansj.splitWord.analysis.ToAnalysis;
import org.nlpcn.commons.lang.tire.domain.Forest;
import org.nlpcn.commons.lang.tire.domain.Value;
import org.nlpcn.commons.lang.tire.library.Library;

public class AnsjWordDefine {
	
	public static void main(String[] args) throws Exception {
		String str = "不三不四，您好！欢迎使用ansj_seg,深圳有没有城中村这里有宽带吗?(ansj中文分词)在这里如果你遇到什么问题都可以联系我.我一定尽我所能.帮助大家.ansj_seg更快,更准,更自由!" ;
		
		// 增加新词,中间按照'\t'隔开
		UserDefineLibrary.insertWord("城中村", "userDefine", 1000);//自定义词汇、自定义词性
		UserDefineLibrary.insertWord("ansj中文分词", "userDefine", 1001);
		Result terms = ToAnalysis.parse(str);
		System.out.println("增加自定义词库:" + terms.toString());
		
		// 删除词语,只能删除.用户自定义的词典.
		UserDefineLibrary.removeWord("ansj中文分词");
		UserDefineLibrary.removeWord("城中村");
		terms = ToAnalysis.parse(str);
		System.out.println("删除自定义词库:" + terms.toString());
		
		// 歧义词
		Value value = new Value("济南下车", "济南", "n", "下车", "v");
		System.out.println(ToAnalysis.parse("我经济南下车到广州.中国经济南下势头迅猛!"));
		Library.insertWord(UserDefineLibrary.ambiguityForest, value);
		System.out.println(ToAnalysis.parse("我经济南下车到广州.中国经济南下势头迅猛!"));
		
		// 多用户词典
		String str1 = "神探夏洛克这部电影作者.是一个dota迷";
		System.out.println(ToAnalysis.parse(str1));
		// 两个词汇 神探夏洛克 douta迷
		Forest dic1 = new Forest();
		Library.insertWord(dic1, new Value("神探夏洛克", "define", "1000"));
		Forest dic2 = new Forest();
		Library.insertWord(dic2, new Value("dota迷", "define", "1000"));
		System.out.println(ToAnalysis.parse(str1, dic1, dic2));
	}
}
/*
 增加自定义词库:不三不四/i,，/w,您好/l,！/w,欢迎/v,使用/v,ansj/en,_,seg/en,,,深圳/ns,有/v,没/d,有/v,城中村/userDefine,这里/r,有/v,宽带/nz,吗/y,?,(,ansj中文分词/userDefine,),在/p,这里/r,如果/c,你/r,遇到/v,什么/r,问题/n,都/d,可以/v,联系/v,我/r,./m,我/r,一定/d,尽/v,我/r,所/u,能/v,./m,帮助/v,大家/r,./m,ansj/en,_,seg/en,更/d,快/a,,,更/d,准/a,,,更/d,自由/a,!
删除自定义词库:不三不四/i,，/w,您好/l,！/w,欢迎/v,使用/v,ansj/en,_,seg/en,,,深圳/ns,有/v,没/d,有/v,城中/ns,村/n,这里/r,有/v,宽带/nz,吗/y,?,(,ansj/en,中文/nz,分词/n,),在/p,这里/r,如果/c,你/r,遇到/v,什么/r,问题/n,都/d,可以/v,联系/v,我/r,./m,我/r,一定/d,尽/v,我/r,所/u,能/v,./m,帮助/v,大家/r,./m,ansj/en,_,seg/en,更/d,快/a,,,更/d,准/a,,,更/d,自由/a,!
我/r,经济/n,南/f,下/f,车/n,到/v,广州/ns,./m,中国/ns,经济/n,南/f,下/f,势头/n,迅猛/a,!
我/r,经/p,济南/n,下车/v,到/v,广州/ns,./m,中国/ns,经济/n,南/f,下/f,势头/n,迅猛/a,!
神/n,探/v,夏洛克/nr,这部/r,电影/n,作者/n,./m,是/v,一个/m,dota/en,迷/v
神探夏洛克/define,这部/r,电影/n,作者/n,./m,是/v,一个/m,dota迷/define
 */
```





