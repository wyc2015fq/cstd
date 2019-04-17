# Ansj中文分词Java开发词性分类 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月21日 16:20:17[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4692








用ansj分词后，只提取满足提交的词性的单词。

Ansj下载地址：http://maven.ansj.org/org/ansj/ansj_seg/

辅助包NLP下载地址：http://maven.ansj.org/org/nlpcn/nlp-lang/

这两个包引入Java工程中即可，具体代码如下：



```java
package csc.ansj;

import java.util.Iterator;

import org.ansj.domain.Result;
import org.ansj.domain.Term;
import org.ansj.splitWord.analysis.ToAnalysis;

import csc.util.Logs;

public class ChnWordSeg {
	//中文分词
    public String chnSeg(String content){
    	String strResult="";
    	try{
    		//分词
        	Result segWords=ToAnalysis.parse(content);  	 
        	//提取分词
        	Iterator<Term> segTerms=segWords.iterator();
        	StringBuffer strbuf=new StringBuffer();
        	while (segTerms.hasNext()) { 
        		Term tm=segTerms.next();
        		String strNs=tm.getNatureStr();//获取词性
        		if(strNs=="null") continue;
        		char cns=strNs.charAt(0);//取词性第一个字母
        		//http://nlpchina.github.io/ansj_seg/content.html?name=词性说明
        		if(cns=='n' || cns=='t' || cns=='s' ||//名词、时间词、处所词
        				cns=='f' || cns=='v' || cns=='a' || //方位词、动词、形容词
        						cns=='b' || cns=='z' || cns=='r' ||//区别词、状态词、代词
        								strNs.equals("mq") || cns=='q' || cns=='d' ||//数词、数量词、副词
        										cns=='y'  || cns=='x' || strNs.equals("en")){//语气词、字符串x、英文
        				//介词p、连词c、助词u、叹词e、拟声词o、标点符号w、前缀h、后缀k不获取 ，数词m只获取其中mq数量词			 			
        			String strNm=tm.getName();
        			strbuf.append(strNm+"|");
        		}
        		//strbuf.append("\r\n");//换行
        	}
        	strResult=strbuf.toString();
        	strResult=strResult.substring(0,strResult.length()-1);//截取最后一个字符|
    	}catch (Exception e) {
			System.out.println(e.getMessage());
			Logs.writeLogs("中文分词失败!");
		} 	
    	return strResult;
    }
    
	public static void main(String args[]) {
		String str = "欢迎使用ansj_seg,(ansj中文分词)在这里如果你遇到什么问题都可以联系我.我一定尽我所能.帮助大家.ansj_seg更快,更准,更自由!" ;
		//System.out.println(ToAnalysis.parse(str).toString());
		ChnWordSeg cws=new ChnWordSeg();
		System.out.println(cws.chnSeg(str));
	}
}
```








