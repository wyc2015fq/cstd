# Nebula3学习笔记(6): IO实战, ZIP解压缩程序 - 逍遥剑客 - CSDN博客
2008年07月08日 16:23:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2379标签：[io																[string																[application																[recursion																[file																[path](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=recursion&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
上一次熟悉了IO系统后, 写个程序来练练手. 
正好这次看到App命名空间, 正好熟悉一下ConsoleApplication的用法. 因为Nebula3内置了ZipFileSystem, 但不支持压缩, 只支持解压缩, 就试着写了一个命令行的unzip.exe, 算是对之前所学的一个总结. 
没想解压缩就像拷贝文件一样简单! 因为当zip文件挂载到IO系统后, 可以像本地文件一样使用其中的文件, 呵呵.
 1:  9: 10: #include 
11: #include 
12: 13: using namespace Util
14: 15: 18: 19:  argc   argv
20: 21: 	CmdLineArgs argsargc argv
22: 	UnZipApp app
23: 	appSetCompanyName
24: 	appSetAppName
25: 	appSetCmdLineArgsargs
26: 	 appOpen
27: 28: 		appRun
29: 		appClose
30: 31: 	system
32: 	appExit
33:  1:  9: #pragma once
10: #include 
11: #include 
12: 13:  UnZipApp   AppConsoleApplication
14: 15: 16: 	UnZipApp
17: 18: 		 bool Open
20: 		  Run
22: 23: 24: 		 UnZipDirUtilString dir
26: 27: 	UtilString zipFileName
28: 	UtilString sourcePath
29: 	UtilString targetPath
30:  1:  9: #include 
10: 11: 12: UnZipAppUnZipApp
13: 14: 15: 16: bool UnZipAppOpen
17: 18: 	 ConsoleApplicationOpen
19: 20: 				 argsHasArg
22: 23: 			n_printf
24: 			n_printf
25: 			 false
26: 27: 28: 		UtilString zipFile
29: 		zipFile  argsGetString
30: 				sourcePath  UtilString  zipFile
32: 		bool fileValid  ioServerMountZipArchivesourcePath
33: 		 fileValid
34: 35: 						sourcePath  UtilString  zipFile
37: 			fileValid  ioServerMountZipArchivesourcePath
38: 			 fileValid
39: 40: 				n_error
41: 				 false
42: 43: 44: 		zipFileName  zipFileExtractFileName
45: 		zipFileNameStripFileExtension
46: 		sourcePath  sourcePathExtractDirName  
47: 48: 				targetPath  argsGetString
50: 		 targetPathLength    targetPath  
51: 					targetPath  UtilString  targetPath
53: 54: 55: 					targetPath  UtilString  targetPath
57: 58: 		targetPath  
59: 		 sourcePath  targetPath
60: 61: 			n_printf
62: 			 false
63: 64: 		 true
65: 66: 	 false
67: 68: 69:  UnZipAppRun
70: 71: 	UnZipDirzipFileName
72: 73: 74:  UnZipAppUnZipDir UtilString dir 
75: 76: 		ioServerCreateDirectorytargetPath  dir
78: 		UtilArrayUtilString listFile  ioServerListFilessourcePath  dir 
80: 	 IndexT i   i  listFileSize i
81: 82: 		UtilString curFile  targetPath  dir    listFilei
83: 		ioServerCopyFilesourcePath  dir    listFilei curFile
84: 		n_printf curFileAsCharPtr
85: 86: 		UtilArrayUtilString listDir  ioServerListDirectoriessourcePath  dir 
88: 	 IndexT i   i  listDirSize i
89: 90: 		UtilString curDir  dir    listDiri
91: 		n_printf targetPath  curDirAsCharPtr
92: 		UnZipDircurDir
93: 94: 
调试参数: ![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Nebula3_6_Unzip_Args.JPG)
运行结果: 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Nebula3_6_Unzip2.JPG)
