# 使用std::cout不能输出显示 - wishchinYang的专栏 - CSDN博客
2015年11月08日 13:25:44[wishchin](https://me.csdn.net/wishchin)阅读数：3946
         在测试一行函数时，出现std::cout不能输出的情况，找不到原因。
1.         只好从main函数第一行，开始测试，直到发现一个函数
o_Initer.ekf_filter(filter, x_k_k,p_k_k, sigma_a, sigma_alpha, sigma_image_noise, o_Initer.gSVOC );   
         函数体包含了：
```cpp
//f = class(f,'ekf_filter');
	if(freopen( "TraceDeg/Trace.txt", "w", stdout)== NULL)
		fprintf(stderr, "error redirecting stdout\n");
	m_shower.PrintMatrix(x_k_k);
	fclose(stdout);
	//end
```
         freopen 函数扰乱了重载，注销掉就可以了。        
2.fclose(stdout);语句的使用
     fclose(stdout);语句的使用导致 文件输出流 不能重定位 回到标准屏幕输出。需要注意。  
     修改为file.close();
    file.clear();语句。
