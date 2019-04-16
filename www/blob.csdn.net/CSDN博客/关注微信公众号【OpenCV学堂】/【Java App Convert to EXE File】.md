# 【Java App Convert to EXE File】 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2009年12月19日 10:44:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：1420标签：[java																[exe																[file																[descriptor																[application																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[Swing桌面开发](https://blog.csdn.net/jia20003/article/category/871637)





* ------------------------------------------------------------------------------------

Title:    TestJava.cpp

author: gloomy fish on 2009-09-16

------------------------------------------------------------------------------------
*/

#include <windows.h>

#include <stdio.h>

#include <stdlib.h>
/* for spawn
*/

#include <process.h>
/* for str* functions
*/

#include <string.h>

#include <TCHAR.H>

//-------------------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance,

                       HINSTANCE hPrevInstance,

                       LPTSTR     lpCmdLine,
int       nCmdShow)

{

// -- check wether application is running now....
    HANDLE hMutex = CreateMutex (NULL,                            
// No security descriptor
                            FALSE,                           
// Mutex object not owned
                            TEXT("TotallyBogusMutexObjectPlateUtility"));
// Object name

if (NULL
== hMutex)

      {
// Didn't expect this to happen!
         printf("Mutex is null");
return1;

      }
else

      {
// Not an error -- deal with success
if ( ERROR_ALREADY_EXISTS
== GetLastError() )

        {
return0;

        }

      }

// -- set up classpath & env path to launch
int ret;

    TCHAR envPath[3072]
="PATH=.;D://JDK6//bin;%path%";
int anErr
= _tputenv(envPath);
if (anErr
!=0) {

        ret = anErr;
return ret;

    }


char jvmArg[2048];

// append all necessary jars to set up classpath
    _tcscpy(jvmArg, _T("-cp .//test.jar"));
// _tcscat(jvmArg, _T(";D://JDK6//lib"));

// -- your application
    _tcscat(jvmArg, _T(" -Xms256m -Xmx1024m com.swing.ui.Test") );

// -- can't launch like this......
     #ifdef _DEBUG

         ret = _spawnlp(_P_NOWAIT,
"..//..//JDK6//bin//java.exe",
"java", jvmArg, NULL);
#else

         ret = _spawnlp(_P_NOWAIT,
"..//..//JDK6//bin//javaw.exe",
"java", jvmArg, NULL);
#endif

return ret;

}


// winXP, vc++6.0 compile successfully....](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=descriptor&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=exe&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




