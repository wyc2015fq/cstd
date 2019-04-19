# cout 与wcout - Ms.Fox的专栏 - CSDN博客
2017年12月05日 16:06:54[lulu-lu](https://me.csdn.net/smbluesky)阅读数：249
std:: cout是C++的标准输出流对象，对应的unicode对象是std:: wcout。
根据测试，在unicode字符集下编译，std:: cout可以输出多字节字符串，但不能输出unicode字符串（std:: cout打印unicode字符串只能输出一个16进制数）；
无论是在unicode字符集下编译还是在多字节字符集下编译，std:: wcout均能输出多字节字符串和unicode字符串。
-  CTime time = CTime::GetCurrentTime();  
-  CString date = time.Format(_T("%Y-%m-%d %X"));
-  wcout << date.GetString() << endl;  
 //cout << date.GetString() << endl; 
