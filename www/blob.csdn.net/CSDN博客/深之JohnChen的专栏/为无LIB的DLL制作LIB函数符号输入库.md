# 为无LIB的DLL制作LIB函数符号输入库 - 深之JohnChen的专栏 - CSDN博客

2005年12月20日 13:51:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1720标签：[dll																[library																[vc++																[c++builder																[工具																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=c++builder&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[VC](https://blog.csdn.net/byxdaz/article/category/7211574)


本文介绍了在VC中针对无LIB时的DLL隐式链接,制作可供VC++使用的LIB函数符号输入库。具体步骤如下：

 一、使用VC++的工具DUMPBIN将DLL中的导出函数表导出到一定义(.DEF)文件
EXAMPLE： 
DUMPBIN VideoDeCoder.dll /EXPROTS /OUT:VideoDeCoder.def 二、将导出的.DEF文件整理为一符合.DEF个数的函数导出文件
EXAMPLE：VideoDeCoder.DEF 文件内容如下Dump of file VideoDeCoder.dll                                  
                                                                       
        File Type: DLL                                                 
                                                                       
          Section contains the following exports for VideoDeCoder.dll  
                                                                       
                   0 characteristics                                   
            3D49E48F time date stamp Fri Aug 02 09:46:55 2002          
                0.00 version                                           
                   1 ordinal base                                      
                  11 number of functions                               
                  11 number of names                                   
                                                                       
            ordinal hint RVA      name                                 
                                                                       
                  1    0 00010F60 _TM_ClearDecoderBuff@4               
                  2    1 00010E80 _TM_CloseDecoder@4                   
                  3    2 00010F00 _TM_DecodePicture@4                  
                  4    3 00010ED0 _TM_DecodePictureHeader@4            
                  5    4 00010FD0 _TM_GetFileEnd@4                     
                  6    5 00011030 _TM_GetUValue@4                      
                  7    6 00011060 _TM_GetVValue@4                      
                  8    7 00011000 _TM_GetYValue@4                      
                  9    8 00010E10 _TM_OpenDecoder@8                    
                 10    9 00010F30 _TM_ReturnType@4                     
                 11    A 00010F90 _TM_SetFileEnd@8                     
                                                                       
          Summary                                                      
                                                                       
                2000 .data                                             
                1000 .rdata                                            
                1000 .reloc                                            
               15000 .text 按照以下方法整理:
1)添加LIB说明 LIBRARY "VideoDeCoder"            ;"xx"为DLL名称
DESCRIPTION "VideoDeCoder library"  2)去掉导出函数说明端以外的内容,在LIB说明下添加 "EXPROTS" 说明导出函数 LIBRARY "VideoDeCoder"
DESCRIPTION "VideoDeCoder library"

EXPORTS
            ordinal hint RVA      name                          
                                                                
                  1    0 00010F60 _TM_ClearDecoderBuff@4            
                  2    1 00010E80 _TM_CloseDecoder@4            
                  3    2 00010F00 _TM_DecodePicture@            
                  4    3 00010ED0 _TM_DecodePictureH            
                  5    4 00010FD0 _TM_GetFileEnd@4              
                  6    5 00011030 _TM_GetUValue@4               
                  7    6 00011060 _TM_GetVValue@4               
                  8    7 00011000 _TM_GetYValue@4               
                  9    8 00010E10 _TM_OpenDecoder@8             
                 10    9 00010F30 _TM_ReturnType@4              
                 11    A 00010F90 _TM_SetFileEnd@8 3)将所有的函数放至行首,去掉 "hint" 和 "RVA" 数据,留下函数的序号 "ordinal" ,在序号前加上 "@" 符号 形成 "_导出函数名@参数字节和 @序号" 此种格式(__stdcall 方式调用导出的函数符号是 "函数名称@参数字节和"). 最后形成.DEF文件如下: LIBRARY "VideoDeCoder"
  DESCRIPTION "VideoDeCoder library"

  EXPORTS
      
  TM_ClearDecoderBuff@4             @1    
  TM_CloseDecoder@4                 @2    
  TM_DecodePicture@4                @3    
  TM_DecodePictureHeader@4          @4    
  TM_GetFileEnd@4                   @5    
  TM_GetUValue@4                    @6    
  TM_GetVValue@4                    @7    
  TM_GetYValue@4                    @8    
  TM_OpenDecoder@8                  @9    
  TM_ReturnType@4                  @10    
  TM_SetFileEnd@8                  @11  三、使用VC++的LIB工具,带/DEF:(.def文件名) /MACHINE:IX86(80X86机器),就输出符合VC++格式的的LIB文件了. 
EXAMPLE:LIB /DEF:VideoDeCoder.def /MACHINE:IX86 四、接时带上LIB文件链接;注意的是当有些动态库DUMPBIN的只有函数名,无"@nn"的参数格式,如C++Builder写的DLL,输出就只有函数名符号,链接时就会报错: error LNK2002:unresolved external symbol "functionname@nn" 提示程序中引入的函数符号无法识别,这时只要将DEF文件中相应的函数名称改为functionname@nn方式,重新建立 LIB,重新链接即可。            

