# C++从文件中查找特定的字符串，并提取该字符串 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月23日 21:52:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：34









记录一个小技巧，使用C++从文件中查找特定标记的字符串，并提取该字符串。

用到了CString的方法，十分的简单，用于数据分析很方便。

这是我截取的压缩编码试验程序中的代码，通过这段代码可以提取X264输出的SSIM值



```cpp
FILE *fp_statfile=fopen("x264_output.txt","rb");
CString filecontent("");
//文件内容读入内存
while(!feof(fp_statfile)){
	filecontent.AppendChar(getc(fp_statfile));
}
//查找X264的SSIM数值
//X264特征字符串
CString featurestr("SSIM Mean Y:");
//查找，返回字符串位置
int paraloc=filecontent.Find(featurestr);
CString parastr;
//找到了的话
if(paraloc!=-1){
	//跳过特征字符串，提取5位
	parastr=filecontent.Mid(paraloc+featurestr.GetLength(),5);
	TRACE("%s\n",parastr);
}
fclose(fp_statfile);
```








