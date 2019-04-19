# Eclipse 导入项目乱码问题（中文乱码）-----有时要1个个java文件去修改编码格式才可以成功！！！ - xqhrs232的专栏 - CSDN博客
2018年09月29日 14:22:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：635
原文地址::[https://blog.csdn.net/YouAreMy_Sunshine/article/details/79759792](https://blog.csdn.net/YouAreMy_Sunshine/article/details/79759792)
1.修改项目编码格式：
  鼠标移到项目名，右键->Properties->Resource->Text file enCoding ->更改编码（GBK、UTF-8等）->Apply->OK>退出
2.修改整个Eclipse工作空间的编码
   Window->Preferences->General->Workspace->text file encoding ->更改编码（GBK、UTF-8等）->Apply->OK退出
3.修改某个特定类型的文件：如java文件类型(java Source File)，xml文件，js文件（javaScript Source File）的编码（不会受命名空间编码的影响）
  eclipse菜单栏->Window->Preferences->General->Content types->Text-> java Source File(或者选择其他文件类型->在Default enCoding中选择语言（没有选择则自己输入GBK、UTF-8等）->Update->Ok>退出
--------------------- 本文来自 YouAreMy_Sunshine 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/YouAreMy_Sunshine/article/details/79759792?utm_source=copy
//================================================================================
备注::
有时要1个个java文件去修改编码格式才可以成功！！！
