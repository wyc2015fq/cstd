# CUDA vector 存储 - 家家的专栏 - CSDN博客





2015年04月02日 11:09:16[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1448










**转自：http://blog.csdn.net/xhz1234/article/details/7522454**

**[cpp]**[view
 plain](http://blog.csdn.net/xhz1234/article/details/7522454#)[copy](http://blog.csdn.net/xhz1234/article/details/7522454#)[print](http://blog.csdn.net/xhz1234/article/details/7522454#)[?](http://blog.csdn.net/xhz1234/article/details/7522454#)





- /*
- * Copyright 徐洪志(西北农林科技大学.信息工程学院).  All rights reserved.
- * Data: 2012-4-26
- */
- // 此程序是演示了vector型数据如何拷贝如显存
- #include <cutil_inline.h>
- #include <iostream>
- #include <vector>
- usingnamespace std;  
- int main(int argc, char** argv)  
- {  
-     CUT_DEVICE_INIT(argc, argv);  // 启动CUDA
- // vector-->Device-->vector
-     vector<int> i_vec;  // 保存输入的数据
-     vector<int> o_vec;  // 保存从显存拷贝回的数据
- int *gpu;  // 显存数据
- int width = 5, height = 3;  // 数据的宽度和高度
- int row, col;  
- size_t size = sizeof(int)*width*height;    
-     cutilSafeCall( cudaMalloc((void**) &gpu, size));  // 申请显存空间
-     cutilSafeCall( cudaMemset(gpu, 0, size));         // 初始化显存空间
- 
- // 内存vector数据赋值
- for(row = 0; row < height; ++row)  
-     {  
- for(col = 0; col < width; ++col)  
-         {  
-             i_vec.push_back(row+col);  
-             o_vec.push_back(0);  
-         }  
-     }  
- // 将内存数据拷贝入显存
-     cutilSafeCall( cudaMemcpy(gpu, &i_vec[0], size, cudaMemcpyHostToDevice));   
- // 将显存数据拷贝回内存
-     cutilSafeCall( cudaMemcpy(&o_vec[0], gpu, size, cudaMemcpyDeviceToHost));    
- // 打印内存数据
- for(row = 0; row < height; ++row)  
-     {  
- for(col = 0; col < width; ++col)  
-             cout << o_vec[row*width+col] << " ";  
-         cout << endl;  
-     }  




**[cpp]**[view
 plain](http://blog.csdn.net/xhz1234/article/details/7522454#)[copy](http://blog.csdn.net/xhz1234/article/details/7522454#)[print](http://blog.csdn.net/xhz1234/article/details/7522454#)[?](http://blog.csdn.net/xhz1234/article/details/7522454#)



- <span style="white-space:pre">  </span>cutilSafeCall( cudaFree(gpu));  // 释放显存空间




**[cpp]**[view
 plain](http://blog.csdn.net/xhz1234/article/details/7522454#)[copy](http://blog.csdn.net/xhz1234/article/details/7522454#)[print](http://blog.csdn.net/xhz1234/article/details/7522454#)[?](http://blog.csdn.net/xhz1234/article/details/7522454#)



- CUT_EXIT(argc, argv);  
- ;  




