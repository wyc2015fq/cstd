# C++实现-特征码遍历



```cpp
#include <stdio.h>  
#include <stdlib.h>  
#include <windows.h>  

union Base     
{  
    DWORD   address;  
    BYTE    data[4];  
};

/************************************************************************/
/* 函数说明：根据特征码扫描基址 
/* 参数一：process 要查找的进程 
/* 参数二：markCode 特征码字符串,不能有空格 
/* 参数三：特征码离基址的距离，默认距离：1 
/* 参数四：findMode 扫描方式，找到特征码后，默认为：1 
/*                  0：往上找基址（特征码在基址下面） 
/*                  1：往下找基址（特征码在基址上面） 
/* 参数五：offset 保存基址距离进程的偏移，默认为：不保存 
/************************************************************************/
DWORD ScanAddress(HANDLE process, char *markCode,   
                  DWORD distinct = 1, DWORD findMode = 1,   
                  LPDWORD offset = NULL)  
{  
    //起始地址  
    const DWORD beginAddr = 0x00400000;  
    //结束地址  
    const DWORD endAddr = 0x7FFFFFFF;  
    //每次读取游戏内存数目的大小  
    const DWORD pageSize = 4096;  
  
    ////////////////////////处理特征码/////////////////////  
    //特征码长度不能为单数  
    if (strlen(markCode) % 2 != 0) return 0;  
    //特征码长度  
    int len = strlen(markCode) / 2;  
    //将特征码转换成byte型  
    BYTE *m_code = new BYTE[len];  
    for (int i = 0; i < len; i++){  
        char c[] = {markCode[i*2], markCode[i*2+1], '\0'};  
        *m_code = (BYTE)::strtol(c, NULL, 16);  
    }  
  
    /////////////////////////查找特征码/////////////////////  
    BOOL _break = FALSE;  
    //用来保存在第几页中的第几个找到的特征码  
    int curPage = 0;  
    int curIndex = 0;  
    Base base;  
    //每页读取4096个字节  
    BYTE page[pageSize];  
    DWORD tmpAddr = beginAddr;  
    while (tmpAddr <= endAddr - len){  
        ::ReadProcessMemory(process, (LPCVOID)tmpAddr, &page, pageSize, 0);  
        //在该页中查找特征码  
        for (int i = 0; i < pageSize; i++){  
            for (int j = 0; j < len; j++){  
                //只要有一个与特征码对应不上则退出循环  
                if (m_code[j] != page[i + j])break;  
                //找到退出所有循环  
                if (j == len - 1){  
                    _break = TRUE;  
                    if (!findMode){  
                        curIndex = i;  
                        base.data[0] = page[curIndex-distinct-4];  
                        base.data[1] = page[curIndex-distinct-3];  
                        base.data[2] = page[curIndex-distinct-2];  
                        base.data[3] = page[curIndex-distinct-1];  
                    }else{  
                        curIndex = i + j;  
                        base.data[0] = page[curIndex+distinct+1];  
                        base.data[1] = page[curIndex+distinct+2];  
                        base.data[2] = page[curIndex+distinct+3];  
                        base.data[3] = page[curIndex+distinct+4];  
                    }  
                    break;  
                }  
            }  
            if (_break) break;  
        }  
        if (_break) break;  
        curPage++;  
        tmpAddr += pageSize;  
    }  
    if(offset != NULL){  
        *offset = curPage * pageSize + curIndex + beginAddr;  
    }  
    return base.address;  
}  
  
/************************************************************************/ 
/* 函数说明：根据特征码扫描call地址 
/* 参数一：process 要查找的进程 
/* 参数二：markCode 特征码字符串,不能有空格 
/* 参数三：特征码离基址的距离，默认距离：1 
/* 参数四：findMode 扫描方式，找到特征码后，默认为：1 
/*                  0：往上找基址 
/*                  1：往下找基址 
/************************************************************************/
DWORD ScanCall(HANDLE process, char *markCode,   
               DWORD distinct = 1, DWORD findMode = 1)  
{  
    DWORD offset;  
    DWORD call = ScanAddress(process, markCode, distinct, findMode, &offset);  
    call += offset;  
    if(findMode) call = call + 5 + distinct;  
    else call = call - distinct;  
    return call;  
}  


//测试代码如下：
int main(int argc, char* argv[])  
{  

    HWND hGame = ::FindWindow("DxFirst", NULL);                //查找游戏窗口
    if(hGame == NULL) return FALSE;  
      
    DWORD processId;  
    HANDLE process;  
    ::GetWindowThreadProcessId(hGame, &processId);  
    process = ::OpenProcess(PROCESS_ALL_ACCESS, false, processId);

    //83C404C3CCCCA1                                    1           人物基址往下搜索  
    //C3CCCCCCCCCCCCCCCCCCCC8B442404A3ECA72001          0           人物基址往上搜索  
    //5557535152C6400801E8                              1            打怪call  
       
     //基址在特征码下面  
     DWORD addr = ScanAddress(process, "83C404C3CCCCA1");  
     printf("人物基址：%X\n",addr);  
       
     //基址在特征码上面  
     DWORD addr = ScanAddress(process, "C3CCCCCCCCCCCCCCCCCCCC8B442404A3ECA72001", 3, 0);  
    
    printf("人物基址：%X\n",addr);  
    DWORD call = ScanCall(process, "5557535152C6400801E8");  
    printf("call基址：%X\n",call);  
    ::CloseHandle(process);  
    return 0;  
}  
```