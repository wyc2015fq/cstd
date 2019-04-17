# 图像搜索目标框---Matlab中regionprops的使用示例 - 家家的专栏 - CSDN博客





2015年12月15日 19:16:50[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3780








图像搜索目标框








版权声明：本文为shaoxiaohu原创文章，欢迎转载，请注明出处，谢谢。




有这样一幅图，

![](https://img-blog.csdn.net/20141019215738500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhb3hpYW9odTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



我们想获取其中的连通区域，可以使用以下代码：





**[plain]**[view
 plain](http://blog.csdn.net/shaoxiaohu1/article/details/40272531#)[copy](http://blog.csdn.net/shaoxiaohu1/article/details/40272531#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- src_img_name = 'blue_sky_white_clound_002594.jpg';  
- img = imread(src_img_name);  
- 
- % get binary image  
- gray_img = rgb2gray(img);  
- T = graythresh(gray_img);  
- bw_img = im2bw(gray_img, T);  
- 
- % find the largest connected region  
- img_reg = regionprops(bw_img,  'area', 'boundingbox');  
- areas = [img_reg.Area];  
- rects = cat(1,  img_reg.BoundingBox);  





显示所有连通区域，





**[plain]**[view
 plain](http://blog.csdn.net/shaoxiaohu1/article/details/40272531#)[copy](http://blog.csdn.net/shaoxiaohu1/article/details/40272531#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- % show all the largest connected region  
- figure(1),  
- imshow(bw_img);  
- for i = 1:size(rects, 1)  
-     rectangle('position', rects(i, :), 'EdgeColor', 'r');  
- end  

![](https://img-blog.csdn.net/20141019220051706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhb3hpYW9odTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




显示最大连通区域，





**[plain]**[view
 plain](http://blog.csdn.net/shaoxiaohu1/article/details/40272531#)[copy](http://blog.csdn.net/shaoxiaohu1/article/details/40272531#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- [~, max_id] = max(areas);  
- max_rect = rects(max_id, :);  
- 
- % show the largest connected region  
- figure(2),   
- imshow(bw_img);  
- rectangle('position', max_rect, 'EdgeColor', 'r');  

![](https://img-blog.csdn.net/20141019215712953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhb3hpYW9odTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






