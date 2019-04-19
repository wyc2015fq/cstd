# Matlab批量读取元胞数组中数据保存到txt文件中（WIDER FACE数据集中，读取人脸标注矩形框信息） - 数据之美的博客 - CSDN博客
2017年02月26日 02:29:43[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：541
# WIDER FACE:http://mmlab.ie.cuhk.edu.hk/projects/WIDERFace/
1、文件说明：
"wider_face_split.zip" 文件包含三个子文件: 
wider_face_train.mat, wider_face_val.mat, andwider_face_test.mat.
其中，每个子文件中元胞数组：
The 'event_list'包含61个种类的文件夹名
The 'face_bbx_list' 包含每个种类中图片中人脸的标注矩形框
The 'file_list' 包含每个种类中图片名
标注的矩形框的格式如下：
<x_left y_top width height>.
参考地址：[http://mmlab.ie.cuhk.edu.hk/projects/WIDERFace/support/example/README.txt](http://mmlab.ie.cuhk.edu.hk/projects/WIDERFace/support/example/README.txt)
2、  要求
导出数据为如下格式
格式：event_list\file_list\face_bbx_list
注：
event_list、file_list、face_bbx_list三个元胞数组里面的内容是一一对应的，切记要一一对应。
对应图片：[http://mmlab.ie.cuhk.edu.hk/projects/WIDERFace/](http://mmlab.ie.cuhk.edu.hk/projects/WIDERFace/)
matlab程序：
**[plain]**[view plain](http://blog.csdn.net/xzzppp/article/details/51888603#)[copy](http://blog.csdn.net/xzzppp/article/details/51888603#)
- <span style="font-size:24px;"><span style="font-size:24px;">%功能描述，读取'wider_face_train.mat'文件中存储的文件夹名、文件图片名及标注矩形框位置（每个图片一行）  
- clc;clear;  
- wider_face_val = load('wider_face_val.mat')  %加载'wider_face_val.mat'文件  
- fid = fopen('val.txt','w');  %以只读方式打开文件'val.txt'  
- for i = 1:61  %文件夹个数  
-     num_j = length(wider_face_val.file_list{i,1})  %每个文件夹中图片个数  
-     for j = 1:num_j  %遍历每个文件夹中图片  
-         %[num_k, num_un] = size(wider_face_val.face_bbx_list{i,1}{j,1})  %取每个图片中人脸个数，即标注矩形框个数  
-         fprintf(fid, '%s%s', wider_face_val.event_list{i,1},'\');  %写入文件夹名  
-         fprintf(fid, '%s%s ', wider_face_val.file_list{i,1}{j,1},'.jpg');  %写入图片名  
-         fprintf(fid, '%g ', wider_face_val.face_bbx_list{i,1}{j,1});  %写入标注矩形框坐标  
-         fprintf(fid, '\n');  %换行  
-     end  
- end  
- fclose(fid);  %关闭文件</span></span>  
**[plain]**[view plain](http://blog.csdn.net/xzzppp/article/details/51888603#)[copy](http://blog.csdn.net/xzzppp/article/details/51888603#)
- <span style="font-size:24px;"><span style="font-size:24px;">%功能描述，读取'wider_face_train.mat'文件中存储的文件夹名、文件图片名及标注矩形框位置（每个人脸矩形框一行）  
- clc;clear;  
- wider_face_train = load('wider_face_train.mat')  %加载'wider_face_train.mat'文件  
- fid = fopen('train.txt','w');  %以只读方式打开文件'train.txt'  
- for i = 1:61  %文件夹个数  
-     num_j = length(wider_face_train.file_list{i,1})  %每个文件夹中图片个数  
-     for j = 1:num_j  %遍历每个文件夹中图片  
-         [num_k, num_un] = size(wider_face_train.face_bbx_list{i,1}{j,1})  %取每个图片中人脸个数，即标注矩形框个数  
-         if num_k == 1  
-             fprintf(fid, '%s%s', wider_face_train.event_list{i,1},'\');  %写入文件夹名  
-             fprintf(fid, '%s%s ', wider_face_train.file_list{i,1}{j,1},'.jpg');  %写入图片名  
-             fprintf(fid, '%g ', wider_face_train.face_bbx_list{i,1}{j,1});  %写入标注矩形框坐标  
-             fprintf(fid, '\n');  %换行  
-         else  
-             for k = 1:num_k  %遍历每个图片中标注框  
-                 %a =  wider_face_train.face_bbx_list{i,1}{j,1}( k, :);  
-                 fprintf(fid, '%s%s', wider_face_train.event_list{i,1},'\');  
-                 fprintf(fid, '%s%s ', wider_face_train.file_list{i,1}{j,1},'.jpg');  
-                 fprintf(fid, '%g ', wider_face_train.face_bbx_list{i,1}{j,1}( k, :));  %写入该图片中第k个标注框位置  
-                 fprintf(fid, '\n');  
-             end  
-         end  
-     end  
- end  
- fclose(fid);  %关闭文件</span></span>  
**[plain]**[view plain](http://blog.csdn.net/xzzppp/article/details/51888603#)[copy](http://blog.csdn.net/xzzppp/article/details/51888603#)
- <span style="font-size:24px;">%功能描述：读取'wider_face_train.mat'文件中存储的图片名及标注矩形框位置（每个人脸矩形框一行）  
- %格式形如“name.jpg x y x+w y+h”,且x,y,w,h都为整数    
- clc;clear;  
- wider_face_train = load('wider_face_train.mat');  
- fid = fopen('11.txt','w');  
- for i = 1:61  
-     num_j = length(wider_face_train.file_list{i,1});  
-     for j = 1:num_j  
-         [num_k, num_un] = size(wider_face_train.face_bbx_list{i,1}{j,1});  
-         if num_k == 1  
-             %fprintf(fid, '%s%s', wider_face_val.event_list{i,1},'\');  
-             fprintf(fid, '%s%s ', wider_face_train.file_list{i,1}{j,1},'.jpg');  
-             fprintf(fid, '%g ', int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 1 )));  
-             fprintf(fid, '%g ', int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 2 )));  
-             fprintf(fid, '%g ', int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 1 ) + wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 3 )));  
-             fprintf(fid, '%g', int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 2 ) + wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 4 )));  
-             fprintf(fid, '\n');  
-         else  
-             for k = 1:num_k  
-                 %fprintf(fid, '%s%s', wider_face_val.event_list{i,1},'\');  
-                 fprintf(fid, '%s%s ', wider_face_train.file_list{i,1}{j,1},'.jpg');  
-                 fprintf(fid, '%g ', int16(wider_face_train.face_bbx_list{i,1}{j,1}(  k, 1 )));  
-                 fprintf(fid, '%g ', int16(wider_face_train.face_bbx_list{i,1}{j,1}(  k, 2 )));  
-                 fprintf(fid, '%g ', int16(wider_face_train.face_bbx_list{i,1}{j,1}(  k, 1 ) + wider_face_train.face_bbx_list{i,1}{j,1}(  k, 3 )));  
-                 fprintf(fid, '%g', int16(wider_face_train.face_bbx_list{i,1}{j,1}(  k, 2 ) + wider_face_train.face_bbx_list{i,1}{j,1}(  k, 4 )));  
-                 fprintf(fid, '\n');  
-             end  
-         end  
-     end  
- end  
- fclose(fid);</span>  
**[plain]**[view plain](http://blog.csdn.net/xzzppp/article/details/51888603#)[copy](http://blog.csdn.net/xzzppp/article/details/51888603#)
- <span style="font-size:24px;">%功能描述：读取'wider_face_train.mat'文件中存储的图片名及标注矩形框位置（每个人脸矩形框一行）  
- %格式形如“name.jpg x y w h”,且x,y,w,h都为整数  
- %要求：去掉x,y小于0，且w,h为0的人脸框  
- clc;clear;  
- wider_face_train = load('wider_face_val.mat');  
- fid = fopen('val_chenqi.txt','w');  
- for i = 1:61  
-     num_j = length(wider_face_train.file_list{i,1});  
-     for j = 1:num_j  
-         [num_k, num_un] = size(wider_face_train.face_bbx_list{i,1}{j,1});  
-         if num_k == 1  
-             %fprintf(fid, '%s%s', wider_face_val.event_list{i,1},'\');  
-             if int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 3 ))==0 | int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 4 ))==0  
-                 continue;  
-             end  
-             a1 = int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 1 ));  
-             if a1 < 0  
-                 a1 = 0;  
-             end  
-             b1 = int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 2 ));  
-             if b1 < 0  
-                 b1 = 0;  
-             end  
-             fprintf(fid, '%s%s ', wider_face_train.file_list{i,1}{j,1},'.jpg');  
-             fprintf(fid, '%g ', a1 );  
-             fprintf(fid, '%g ', b1 );  
-             fprintf(fid, '%g ', int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 3 )));  
-             fprintf(fid, '%g',  int16(wider_face_train.face_bbx_list{i,1}{j,1}( num_k, 4 )));  
-             fprintf(fid, '\n');  
-         else  
-             for k = 1:num_k  
-                 %fprintf(fid, '%s%s', wider_face_val.event_list{i,1},'\');  
-                 if int16(wider_face_train.face_bbx_list{i,1}{j,1}( k, 3 ))==0 | int16(wider_face_train.face_bbx_list{i,1}{j,1}( k, 4 ))==0  
-                     continue;  
-                 end  
-                 a2 = int16(wider_face_train.face_bbx_list{i,1}{j,1}( k, 1 ));  
-                 if a2 < 0  
-                     a2 = 0;  
-                 end  
-                 b2 = int16(wider_face_train.face_bbx_list{i,1}{j,1}( k, 2 ));  
-                 if b2 < 0  
-                     b2 = 0;  
-                 end  
-                     fprintf(fid, '%s%s ', wider_face_train.file_list{i,1}{j,1},'.jpg');  
-                     fprintf(fid, '%g ', a2 );  
-                     fprintf(fid, '%g ', b2 );  
-                     fprintf(fid, '%g ', int16(wider_face_train.face_bbx_list{i,1}{j,1}(  k, 3 )));  
-                     fprintf(fid, '%g',  int16(wider_face_train.face_bbx_list{i,1}{j,1}(  k, 4 )));  
-                     fprintf(fid, '\n');  
-             end  
-         end  
-     end  
- end  
- fclose(fid);</span>  
