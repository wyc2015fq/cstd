# linux 调试信息debug多参数，多颜色打印 - V__KING__的专栏 - CSDN博客





2017年03月24日 14:17:34[v__king__](https://me.csdn.net/V__KING__)阅读数：373








printf打印的信息可以是有颜色的，可以使多参数输入的

颜色设置：[http://blog.csdn.net/cy_cai/article/details/16943189](http://blog.csdn.net/cy_cai/article/details/16943189)

按等级printf打印：[http://www.cnblogs.com/mylinux/p/5906576.html](http://www.cnblogs.com/mylinux/p/5906576.html)

```
#include <stdio.h>  
#include <stdlib.h>  

#define DEBUG1( fmt,  arg  ... )  \  
    do{printf("[DEBUG] " fmt ,  ##arg );  }while(0);  
#define DEBUG2( fmt,  arg  ... ) \  
    do{printf("[%s: %s: line %d]" fmt ,\  
        __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  

#define PrintColor1( fmt,  arg  ... )   \  
    do{printf("\033[30m""[%s: %s: line %d]" fmt"\033[0m" ,\  
        __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  

#define PrintColor2( fmt,  arg  ... )   \  
    do{printf("\033[31m""[%s: %s: line %d]" fmt"\033[0m" ,\  
        __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  

#define PrintColor3( fmt,  arg  ... )   \  
    do{printf("\033[32m""[%s: %s: line %d]" fmt"\033[0m" ,\  
        __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  

#define PrintColor4( fmt,  arg  ... )   \  
    do{printf("\033[33m""[%s: %s: line %d]" fmt"\033[0m" ,\  
        __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  

#define PrintColor5( fmt,  arg  ... )   \  
    do{printf("\033[34m""[%s: %s: line %d]" fmt"\033[0m" ,\  
        __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  

#define PrintColor6( fmt,  arg  ... )   \  
    do{printf("\033[35m""[%s: %s: line %d]" fmt"\033[0m" ,\  
        __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  

#define PrintColor7( fmt,  arg  ... )    \  
    do{printf("\033[36m""[%s: %s: line %d]" fmt"\033[0m" ,\  
        __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  

int main()  
{  
    printf("I AM IN macro.c\n");  

    DEBUG1("I AM IN macro.c\n");  
    DEBUG2("I AM IN macro.c\n");  

    PrintColor1("I AM IN macro.c\n");  
    PrintColor2("I AM IN macro.c\n");  
    PrintColor3("I AM IN macro.c\n");     
    PrintColor4("I AM IN macro.c\n");  
    PrintColor5("I AM IN macro.c\n");     
    PrintColor6("I AM IN macro.c\n");  
    PrintColor7("I AM IN macro.c\n");  

    return 0;  
}
```



