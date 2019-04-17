# 特征点检测效果评估（matlab代码） - tostq的专栏 - CSDN博客





2015年09月24日 20:11:58[tostq](https://me.csdn.net/tostq)阅读数：6388标签：[特征提取																[matlab																[代码																[测试评估](https://so.csdn.net/so/search/s.do?q=测试评估&t=blog)
个人分类：[特征提取](https://blog.csdn.net/tostq/article/category/5839791)





最近老师给我一个课题方向，要求研究特征提取的方法，找到提取出来的特征精度最高，这里的最高是指在多帧运动变化图片里应用此算法都能找到固定位置的特征，而且这个特征不限，可以是边缘、角点或区域块等，然后根据不同特征、其运动变化及图像成像质量提供不同的算法，以达到最优。这个方向刚开始的确十分头大，完全不知道从什么着手，是边缘提取算法呢？还是先作目标分割，大概想一两天，决定先从特征点（包括角点）提取算法做起，先找到在特征点算法里非常优秀的，然后结合各种背景（噪声，运动变化）测试其变化。

这里主要的思路来自于一个仿射不变特征提取方法的检测：http://www.robots.ox.ac.uk/~vgg/research/affine/

这个网站上面主要提供了一个仿射不变特征提取方法的评估，它提供了八组图片，分别从噪声、模糊、视点变化、缩放、旋转、亮度变化来评估其仿射不变特征提取方法的效果。不过它主要是对相似区域进行提取，涉及到椭圆区域的仿射变换，而我主要进行点特征点提取的评估，要稍微简单点，当然也借鉴了其代码（这里面只有评估代码，没有特征提取算法的代码，不过提供了一系列算法的执行文件）

我首先要测试的sift算法对视点变化的评估，从上面的网站里下载了几个图片【img1.ppm、img2.ppm】、变化矩阵【H1to2p】及特征点提取文件【img1.haraff.sift】，我主要从特征向量匹配程度和实际匹配的位置进行比较。我的代码如下：



```
function [matchFeaIndex, matchFeaRatio,matchLocal1]=matchPointFea(file1,file2,H,image1,image2)
%   主要用特征点匹配效果测试：
%   用法：[matchFeaIndex, matchFeaRatio,matchLocal1]=matchPointFea('img1.haraff.sift','img2.haraff.sift','H1to2p','img1.ppm','img2.ppm');
%   输入: 
%       file1 - 图像1特征点检测文件         
%       file2 - 图像2特征点检测文件 
%       H - 图像1到图像2的仿射变换矩阵
%       image1 - 图像1
%       image2 - 图像2
%    输出:    
%       matchFeaIndex - 保存图像1中同图像2匹配的特征点在file2中的标号，若不匹配则为0。
%       matchFeaRatio - 对应于matchIndex，保存特征点匹配程度，不匹配则为0。
%       matchLocal1 - 保存图像2中特征点位置同经过仿射变换1之间的位置差别
%       matchLocal2 - 保存图像1中特征点位置同经过仿射变换2之间的位置差别（保留）
%    特征点检测文件格式： 
%    file1, file2:
%       x1 y1 a1 b1 c1 c2 c3 ...
%       x2 y2 a2 b2 c1 c2 c3 ...
%---------------------
%       第一行 保存描述子数目及大小
%       x, y - 特征点位置
%       a, b - 特征点大小及方向（主要针对于sift变换）
%       d1 d2 d3 ... - 特征描述子向量（如果小于，则无效）

close all;
%   loc存放位置及大小角度，des存放特征描述子向量,dimdesc存放描述子大小及数目
[des1, loc1, dimdesc1]=loadFeatures(file1);     
[des2, loc2, dimdesc2]=loadFeatures(file2);
%   导入图像
Im1=imread(image1);
Im2=imread(image2);
%   导入仿射矩阵文件
H=load(H);

fprintf(1,'numbers and length of descriptor in file1 %d %d\n',dimdesc1(1),dimdesc1(2));
fprintf(1,'numbers and length of descriptor in file2 %d %d\n',dimdesc2(1),dimdesc2(2));

if dimdesc1(2)>1 && dimdesc1(2)==dimdesc2(2) 
  fprintf(1,'%s, %s look like files with descriptors...\n',file1,file2);
else
    error('Different descriptor dimension in %s or %s files.',file1,file2);
end

% 计算两个特征向量的匹配程度可以通过向量空间余弦相似度来衡量.  
% 设置比值distRatio，保证所匹配的特征点具有显著相似度，即与第二相似度有较大差异.
distRatio = 0.6;   

% 在图像2中找到图像1中每一个匹配点.
des2t = des2';                          % 转置
desNum = dimdesc1(1);                   % 图像1特征点个数
matchFeaIndex=zeros(desNum,1);
matchFeaRatio=zeros(desNum,1);
matchNum=0;
for i = 1 : desNum
   dotprods = des1(i,:) * des2t;        % 计算乘积项
   [vals,indx] = sort(acos(dotprods));  % 排序余弦相似度
   % 找到最大余弦相似度.
   if (vals(1) < distRatio * vals(2))
      matchFeaIndex(i) = indx(1);
      matchFeaRatio(i) = vals(1);
      matchNum=matchNum+1;
   else
      matchFeaIndex(i) = 0;
   end
end

fprintf(1,'numbers of match descriptor is %d \n',matchNum);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 确定图像1中特征点变换到图像2中的位置，及图像2变换到图像1中的位置
% 要进行仿射变换测试，要求图像1和2必须大小相同
if size(Im1)==size(Im2)
HI=H(:, 1:3);   %图1到图2
H=inv(HI);      %图2到图1
fprintf(1,'Projecting 1 to 2...\n');
loc1t=projectPoints(loc1',HI);
loc1t=loc1t';
fprintf(1,'and 2 to 1...\n');
loc2t=projectPoints(loc2',H);
loc2t=loc2t';
ImageSize=size(Im1);
matchLocal1=matchLocal(loc1t,loc2,ImageSize,matchFeaIndex);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 画图
% showkeys(image1,loc1);      % 特征点显示
% showkeys(image2,loc2);

% 显示匹配图像过程
Im=appendimages(Im1,Im2); 
% Show a figure with lines joining the accepted matches.
figure('Position', [0 0 size(Im,2) size(Im,1)]);
colormap('gray');
imagesc(Im);
hold on;
cols1 = size(Im1,2);
for i = 1: size(des1,1)
  if (matchFeaIndex(i) > 0 && matchLocal1(i)<5)
    line([loc1(i,1) loc2(matchFeaIndex(i),1)+cols1], ...
         [loc1(i,2) loc2(matchFeaIndex(i),2)], 'Color', 'c');  %画对应线条
    plot(loc1(i,1),loc1(i,2),'r.'); %画特征点
    plot(loc2(matchFeaIndex(i),1)+cols1,loc2(matchFeaIndex(i),2),'g.'); %画特征点
  end
end
hold off;

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   点的投影运算
function loct=projectPoints(loc,H)
num=size(loc(1));   %特征点数目
loct=loc;
for i=1:num
    l1=[loc(i,1),loc(i,2),1];
    l1_2=H*l1';
    l1_2=l1_2/l1_2(3);
    loct(i,1)=l1_2(1);
    loct(i,2)=l1_2(2);
end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   加载描述子文件
function [des, loc, dimdes]=loadFeatures(file)
fid = fopen(file, 'r');
[dimdes, count]=fscanf(fid, '%d %d',[1 2]);
if count ~= 2
    error('Invalid keypoint file beginning.');
end
num = dimdes(1);    %描述子数目
len = dimdes(2);    %描述子长度
loc = double(zeros(num, 4));
des = double(zeros(num, len));
% 将描述子导入向量
for i = 1:num
    [vector, count] = fscanf(fid, '%f %f %f %f', [1 len+5]); %row col scale ori
    if count ~= (5+len)
        error('Invalid keypoint file format');
    end
    loc(i, :) = vector(1, 1:4);    
    descrip(1, :) = vector(1, 6:len+5);
    descrip = descrip / sqrt(sum(descrip.^2));
    des(i, :) = descrip(1, :);
end
fclose(fid);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   计算实际特征匹配的偏差位置
function matchLoc=matchLocal(loct,loc,ImageSize,matchFeaIndex)
matchLoc=ones(size(matchFeaIndex));     
for i=1:size(loct)
    if matchFeaIndex(i)~=0      %找到匹配的点
        %注意仿射变换，位置可能超过图像范围
        if loct(i,1)>0&&loct(i,1)<ImageSize(1)&&loct(i,2)>0&&loct(i,2)<ImageSize(2)...
                &&loc(matchFeaIndex(i),1)>0&&loc(matchFeaIndex(i),1)<ImageSize(1)&&loc(matchFeaIndex(i),2)>0&&loc(matchFeaIndex(i),2)<ImageSize(2)
            matchLoc(i)=(loct(i,1)-loc(matchFeaIndex(i),1))^2+(loct(i,2)-loc(matchFeaIndex(i),2))^2;
            matchLoc(i)=sqrt(matchLoc(i));  %这个偏差即是欧氏距离
        end
    end
end
matchLoc=100.*matchLoc./max(matchLoc); %计算百分比
end
```

![](https://img-blog.csdn.net/20150924202709777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




最后效果图，这里我稍微减少了匹配的特征点个数（原本匹配的点有300多个>_<），只是显示了效果最好的一些点。另外这里全部的代码，很快上传上去。](https://so.csdn.net/so/search/s.do?q=代码&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)](https://so.csdn.net/so/search/s.do?q=特征提取&t=blog)




