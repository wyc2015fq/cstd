# 用ICTCLAS对复旦语料库分词 - Orisun - 博客园







# [用ICTCLAS对复旦语料库分词](https://www.cnblogs.com/zhangchaoyang/articles/2233059.html)





ICTCLAS提供C/C++,Java,C#接口。我发现在ubuntu上使用时，配置文件Configure.xml不起作用，因为不管<Tagger>设为On还Off，C版的总是加词性标注，而JNI版的总是不加词性标注。而<GranularityContorl>设为开或关结果都一样。

先给一个C++版的：

```
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/home/orisun/SoftWare/ICTCLAS50_Linux_RHAS_32_C/API/ICTCLAS50.h"

int main(int argc, char *argv[])
{
    char *initPath = "/home/orisun/master/ICTCLAS50_Linux_RHAS_32_C/API";
    if (!ICTCLAS_Init(initPath)) {
        printf("Init fails\n");
        return -1;
    }
    ICTCLAS_FileProcess(argv[1],argv[2],CODE_TYPE_UTF8, 1);        //argv[1]是待分词的文件，argv[2]是输出文件
    ICTCLAS_Exit();
    return 0;
}
```

注意怎么编译：

g++ ws.c -o ws -L/home/orisun/master/ICTCLAS50_Linux_RHAS_32_C/API -lICTCLAS50-D OS_LINUX

Java版的

/**
 * Author: Orisun
 * Date: Nov 1, 2011
 * FileName: WordSeg.java
 * Function: 用ICTCLAS进行中文分词.
 */

import java.io.File;

import ICTCLAS.I3S.AC.ICTCLAS50;

public class WordSeg {
	// 注意一次分词的数目不要太多，控制在一两千，否则会中途崩溃而退出。
	public void wordSeg(File srcFile) {
		if (srcFile.isDirectory()) {
			File[] childFiles = srcFile.listFiles();
			for (File child : childFiles) {
				wordSeg(child);
			}
		} else if (srcFile.isFile()) {
			try {
				ICTCLAS50 ictc = new ICTCLAS50();
				// 指定Configure.xml和Data directory的存储位置
				String argu = "/home/orisun/master/ICTCLAS50_Linux_RHAS_32_JNI/API";
				if (ictc.ICTCLAS_Init(argu.getBytes("GB2312")) == false) {
					System.out.println("Init Fail!");
					return;
				}
				// 加入用户词典
				String usrDict = "/home/orisun/master/ICTCLAS50_Linux_RHAS_32_JNI/API/userdict";
				byte[] dictB = usrDict.getBytes();
				ictc.ICTCLAS_ImportUserDictFile(dictB, 3);
				// 保存用户词典
				ictc.ICTCLAS_SaveTheUsrDic();
				// 对文件进行分词，分词后的结果保存在新文件中
				File parent = new File(srcFile.getParent() + "_ws/");
				parent.mkdir();
				String name = srcFile.getName();
				File ResFile = new File(parent, name);
				ResFile.createNewFile();
				// 这里的参数“3”指定了待分词的文件是UTF-8编码
				if (ictc.ICTCLAS_FileProcess(srcFile.getAbsolutePath()
						.getBytes(), 3, 0, ResFile.getAbsolutePath().getBytes()) == false) {
					System.out.println(srcFile + "文件分词失败.");
					return;
				}
				ictc.ICTCLAS_Exit(); // 释放分词组件资源
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args) {
		WordSeg inst=new WordSeg();
		File file=new File("/home/orisun/master/fudan_corpus/train/C39-Sports");
		inst.wordSeg(file);
	}
}

说明一下代码中添加的用户词典内容如下：

收稿@@v
盛唐@@n
二次@@adj
仿真@@n
与时俱进@@adv
原刊@@n
期号@@n
页号@@n
欧几里德@@n
地大物博@@adj

ICTCLAS5.0的文档中说“系统内核全新升级。支持多线程调用。”可是当我试图开两个线程来分别对训练集和测试集进行分词时，JVM发生致命错误。还没有尝试C/C++版的是否支持多线程。












