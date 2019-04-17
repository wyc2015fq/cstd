# C++ 调节PCM音频音量大小 - DoubleLi - 博客园






在用解码器解码音频数据得到PCM音频数据块之后，可以在将数据送给声卡播放之前调节其音量大小，具体的实现函数如下：





**[cpp]**[view plain](http://blog.csdn.net/weixinhum/article/details/34864625#)[copy](http://blog.csdn.net/weixinhum/article/details/34864625#)



- void RaiseVolume(char* buf, UINT32 size, UINT32 uRepeat, double vol)//buf为需要调节音量的音频数据块首地址指针，size为长度，uRepeat为重复次数，通常设为1，vol为增益倍数,可以小于1  
- {  
- if (!size)  
-     {  
- return;  
-     }  
- for (int i = 0; i < size;)  
-     {  
- signed long minData = -0x8000; //如果是8bit编码这里变成-0x80  
- signed long maxData = 0x7FFF;//如果是8bit编码这里变成0xFF  
- 
- signed short wData = buf[i + 1];  
-         wData = MAKEWORD(buf[i], buf[i + 1]);  
- signed long dwData = wData;  
- 
- for (int j = 0; j < uRepeat; j++)  
-         {  
-             dwData = dwData * vol;  
- if (dwData < -0x8000)  
-             {  
-                 dwData = -0x8000;  
-             }  
- else if (dwData > 0x7FFF)  
-             {  
-                 dwData = 0x7FFF;  
-             }  
-         }  
-         wData = LOWORD(dwData);  
-         buf[i] = LOBYTE(wData);  
-         buf[i + 1] = HIBYTE(wData);  
-         i += 2;  
-     }  
- }  










