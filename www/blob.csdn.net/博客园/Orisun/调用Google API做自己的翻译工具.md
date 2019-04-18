# 调用Google API做自己的翻译工具 - Orisun - 博客园







# [调用Google API做自己的翻译工具](https://www.cnblogs.com/zhangchaoyang/articles/1863890.html)





其实ubuntu下星际译王也不怎么好用，启动慢，词典不好管理。果断自己调用Google API做翻译工具，而且是在终端运行，快捷方便。

（1）编译java程序时指定依赖的第三方jar包

$ javac -cp /usr/local/lib/google-api-translate-java-0.4.jar MyTranslate.java

-cp是在指定classpath

事先下载google-api-translate-java-0.4.jar，放在/usr/local/lib/目录下。

（2）编写MANIFEST.MF文件

文件内容如下：

Manifest-Version: 1.0

Main-Class: MyTranslate

Created-By: Orisun

Class-Path: /usr/local/lib/google-api-translate-java-0.4.jar

注意冒号后面有一个空格。此文件放在与MyTranslate.class同一目录下。

（3）制作Jar包

jar cvfm translator.jar manifest.mf MyTranslate.class

（4）运行jar程序

 java -jar translator.jar ec chunk

"ec chunk"是该程序运行时需要的两个参数。其他程序不需要参数时可以不写。

（5）把程序做到自己的脚本中去

把translator.jar放到/usr/local/lib/下，

在~/.bashrc文件的最后添加两行：

alias cte='java -jar /usr/local/lib/translator.jar ce'

alias etc='java -jar /usr/local/lib/translator.jar ec'

（6）享用劳动成果

![](https://pic002.cnblogs.com/images/2010/103496/2010102821284127.png)

原代码：


import com.google.api.translate.Language;
import com.google.api.translate.Translate;

public class MyTranslate {
	public static void main(String args[]) {
		if(args.length != 2) {
			System.out.println("run like this:java -jar transtalor.jar  word");
			System.exit(1);
		}
		
		try {
			String languageFrom = null;
			String languageTo = null;
			if (args[0].equals("ce")) { // 中到英
				languageFrom = Language.CHINESE;
				languageTo = Language.ENGLISH;
			} else if (args[0].equals("ec")) { // 英到中
				languageFrom = Language.ENGLISH;
				languageTo = Language.CHINESE;
			} else {
				System.out.println("run like this:java -jar transtalor.jar  word");
				System.exit(1);
			}

			System.out.println(Translate.translate(args[1], languageFrom, languageTo));
		} catch (Exception ex) {
			System.out.println("UnkownError!");
		}		
	}
}














