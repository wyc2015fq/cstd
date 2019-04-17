# MFC程序显示控制台输出 - DoubleLi - 博客园






有的时候，我们用c写的一些东西，可能需要MFC作为UI输入参数进行测试，但是程序里有大量的printf操作，这就需要MFC程序启动的时候同时打开

一个控制台，用于标准输出 

查询网络，大致方法有以下两种

1,添加一个全局函数，内容如下，再程序启动的时候调用


- 



void InitConsoleWindow()


- 



{


- 



int nCrt = 0;


- 



FILE* fp;


- 



AllocConsole();


- 



nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);


- 



fp = _fdopen(nCrt, "w");


- 



*stdout = *fp;


- 



setvbuf(stdout, NULL, _IONBF, 0);


- 



}











2,在程序启动的地方添加以下代码，MFC程序添加到InitInstance函数中


- 



FILE *stream;


- 



	AllocConsole();


- 



	SetConsoleTitle(_T("debug console"));


- 



	freopen_s(&stream,"CONOUT$", "w", stdout);


- 











