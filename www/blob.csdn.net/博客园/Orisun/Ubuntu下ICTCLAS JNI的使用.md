# Ubuntu下ICTCLAS JNI的使用 - Orisun - 博客园







# [Ubuntu下ICTCLAS JNI的使用](https://www.cnblogs.com/zhangchaoyang/articles/2108733.html)





首先下载[ICTCLAS2011_Linux_32_jni](http://ictclas.org/ictclas_feedback.aspx?packetid=53&packeturl=down/50/ICTCLAS50_Linux_RHAS_32_JNI.rar) ，解压后是ICTCLAS50_Linux_RHAS_32_JNI文件夹。

在Eclipse里面新建一个Java Project，把ICTCLAS50_Linux_RHAS_32_JNI/API下的ICTCLAS放到Java Project的src目录下，把ICTCLAS50_Linux_RHAS_32_JNI/API下的其他所有内容放到Java Project的根目录下，新建一下java class，取名Test。在ICTCLAS50_Linux_RHAS_32_JNI/Doc下有个ICTCLAS50的API使用说明，第22页是第一个JNI示例程序，把该程序的见容拷贝到我们的Test.java里面就可以了。或者直接使用下面的代码：

import ICTCLAS.I3S.AC.*;

public class Test {
	public static void main(String[] args) {
		try {
			ICTCLAS50 testICTCLAS50 = new ICTCLAS50();
			// 分词所需库（即Data文件夹）的路径
			String argu = ".";
			// 初始化
			if (testICTCLAS50.ICTCLAS_Init(argu.getBytes("GB2312")) == false) {
				System.out.println("Init Fail!");
				return;
			} else {
				System.out.println("Init Succeed!");
			}

			String sInput = "如果您对本网站的任何内容感兴趣请联系我们:Orisun@ymail.com。中华民国署";
			byte nativeBytes[] = testICTCLAS50.ICTCLAS_ParagraphProcess(
					sInput.getBytes("GB2312"), 0, 0);
			//System.out.println(nativeBytes.length);
			String nativeStr = new String(nativeBytes, 0, nativeBytes.length,
					"GB2312");
			//System.out.println("The result is ：" + nativeStr);
			String[] rest=nativeStr.split("\\s+");
			for(int i=0;i<rest.length;i++)
				System.out.println(rest[i]);
			testICTCLAS50.ICTCLAS_Exit();
		} catch (Exception e) {

		}
	}
}


运行Test.java，会遇到错误：no ICTCLAS50 in java.library.path，这时更改ICTCLAS.I3S.AC.ICTCLAS50.java，把System.loadLibrary("ICTCLAS50")改为System.load("/home/orisun/develop/workspace/ICTCLAS_Test/libICTCLAS50.so")----即libICTCLAS50.so的绝对路径，












