# memset和wmemset用法记录 - DoubleLi - 博客园






char cBuffer[50] ;
TCHAR wBuffer[50];

1、将cBuffer初始化为空格
memset(cBuffer, ' ', sizeof(cBuffer));第三个参数为字节数
cBuffer[49] = 0;
2、将wBuffer初始化为空格
wmemset(wBuffer, _T(' '), sizeof(wBuffer)/sizeof(TCHAR));第三个参数为字符个数
wBuffer[49] = 0;
3、使用memset清空宽字符数组 

memset(wBuffer, 0, sizeof(wBuffer));第三个参数为字节数



4、memset(wBuffer, ' ', sizeof(wBuffer));这个不能初始化为空格，而是数组每一位高低字节全部赋值为空格









