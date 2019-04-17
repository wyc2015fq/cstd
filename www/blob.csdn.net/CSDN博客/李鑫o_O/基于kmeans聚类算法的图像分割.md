# 基于kmeans聚类算法的图像分割 - 李鑫o_O - CSDN博客





置顶2016年03月10日 19:54:19[hustlx](https://me.csdn.net/HUSTLX)阅读数：6175








# Kmeans之前已经讲过了，其图像分割只不过是把之前的高斯数对换成图像二维像素点，彩色图像每个像素点有rgb三个分量，灰度图像只有一个分量。

## 1编程实现



```cpp
<span style="font-size:14px;">clear;clc;close all;
data=imread('src1.bmp');
imshow(data)
[m,n,c]=size(data);
[mu,pattern]=k_mean_Seg(data,2);
for x=1:m
    for y=1:n
        if pattern(x,y,1)==1
            data(x,y,1)=0;
            data(x,y,2)=0;
            data(x,y,3)=255;
        elseif pattern(x,y,1)==2
            data(x,y,1)=0;
            data(x,y,2)=255;
            data(x,y,3)=0;
        elseif pattern(x,y,1)==3
            data(x,y,1)=255;
            data(x,y,2)=0;
            data(x,y,3)=0;
        else
            data(x,y,1)=255;
            data(x,y,2)=255;
            data(x,y,3)=0;
        end
    end
end
figure;
imshow(data);   
 
function [num,mask]=k_mean_Seg(src,k)
src=double(src);
img=src;        
src=src(:);      
mi=min(src);     
src=src-mi+1;    
L=length(src);
 
m=max(src)+1;
hist=zeros(1,m);
histc=zeros(1,m);
for i=1:L
  if(src(i)>0)
      hist(src(i))=hist(src(i))+1;
  end;
end
ind=find(hist);
hl=length(ind);
num=(1:k)*m/(k+1);
while(true)
  prenum=num;
  for i=1:hl
      c=abs(ind(i)-num);
      cc=find(c==min(c));
      histc(ind(i))=cc(1);
  end
  for i=1:k,
      a=find(histc==i);
      num(i)=sum(a.*hist(a))/sum(hist(a));
  end
  if(num==prenum)
      break;
  end;
 
end
L=size(img);
mask=zeros(L);
for i=1:L(1),
for j=1:L(2),
  c=abs(img(i,j)-num);
  a=find(c==min(c)); 
  mask(i,j)=a(1);
end
end
num=num+mi-1;  </span>
```






## 2结果展示



## ![lip_image013](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200601725-1790834414.png)![lip_image014](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200606694-1624895718.png)



a)原图b)N=2

![lip_image015](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200610835-1862697049.png)![lip_image016](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200613319-173499058.png)

c)N=3d)N=4

对于灰度图像，用data=rgb2gray(data); 转化成为成为灰度图像后面的显示换成如下：

for x=1:m

for y=1:n

if pattern(x,y)==1

data(x,y)=0;

elseif pattern(x,y)==2

data(x,y,1)=80;

elseif pattern(x,y)==3

data(x,y)=180;

else

data(x,y)=255;

end

end

end

![lip_image017](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200618725-1867586912.png)![lip_image018](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200621569-876252892.png)

a)原图b)N=2

![lip_image019](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200624491-1615422611.png)![lip_image020](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200628600-794693542.png)

c)N=3d)N=4




