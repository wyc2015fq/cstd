# CxImage内存方式转换图像 - DoubleLi - 博客园






最近，处于项目需要，需要将Bmp转换为JPEG格式。以前做过，采用的是GDI+的方式，该方式有一个极大地缺陷为无法实现跨平台处理。闲话少说，进入正题。



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678112#)[copy](http://blog.csdn.net/augusdi/article/details/8678112#)



- CxImage cxImageBmp(pRGBBuffer, dwRGBSize, CXIMAGE_FORMAT_BMP);  
- 
- cxImageBmp.Save("D:\\ttt.jpg",CXIMAGE_FORMAT_JPG);  



该方式很正常，顺利将RGB格式24位BMP图像转化为本地JPG文件并保存下来。



但是采用以下内存转换方式，却失败了。



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678112#)[copy](http://blog.csdn.net/augusdi/article/details/8678112#)



- BYTE* pJpgDataTemp = new BYTE[1024 * 1024];  
- 
- int lJpgSize = 0;  
- 
- bool b = cxImageBmp.Encode(pJpgDataTemp, lJpgSize, CXIMAGE_FORMAT_JPG);  



一方面，函数返回值为false；另一方面，数据长度,lJpgSize为0。



后来，经过以下修改，成功。



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678112#)[copy](http://blog.csdn.net/augusdi/article/details/8678112#)



- BYTE* pJpgDataTemp =NULL;  
- 
- int lJpgSize = 0;  
- 
- bool b = cxImageBmp.Encode(pJpgDataTemp,lJpgSize,CXIMAGE_FORMAT_JPG);  




为什么？下面我们从CxImage::Encode()定义中进行查看：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678112#)[copy](http://blog.csdn.net/augusdi/article/details/8678112#)



- bool CxImage::Encode(BYTE * &buffer, long &size, DWORD imagetype)  
- {  
- if (buffer!=NULL)  
- 
- {  
- strcpy(info.szLastError,"the buffer must be empty");  
- return false;  
- }  
- 
- 
- CxMemFile file;  
- file.Open();  
- if(Encode(&file,imagetype))  
- {  
- buffer=file.GetBuffer();  
- size=file.Size();  
- return true;  
- }  
- 
- 
- return false;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)



真相大白！









