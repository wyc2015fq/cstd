# MATLAB批量读取文件夹下的图像，并对图像进行裁剪为指定大小，旋转，缩放，加噪声处理，rgb2hsv，批量保存 - 站在巨人的肩膀上coding - CSDN博客





2018年01月25日 17:44:52[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1896








看完标题，我们可以把这个任务当成两个子任务来实现：一是先批量读取指定文件夹下的图像，二是对读取的每一

帧图形裁剪；再就是如何设置，才可以裁剪出多张指定的大小？三是如何对处理后的图像进行批量保存?现在让我们来

一探究竟。

step1：批量读取指定文件夹下的图像：

```cpp
file_path='F:\学术&工程\ObjectDetect\全景图像集\';  %图像文件夹路径
img_path_list=dir(strcat(file_path,'*.jpg'));%获取该文件夹下所有jpg格式的图像,是一个结构体，有data,name等成员
img_num=length(img_path_list);%获取图像的数量
if img_num >0 %文件夹下有图
    for j=1:img_num  %逐一读取图像
        image_name=img_path_list(j).name;%图像名
        image=imread(strcat(file_path,image_name));%拼接图像名
        fprintf('%d %s\n',j,strcat(file_path,image_name));%显示正在处理的图像名
        %TODO
        %图像处理过程
    end
end
```

好了，到此为止我们已经实现了批量读取指定路径下所有图像，那么，如果我们还要继续读取指定目录下的子文件夹中

的图像呢？我们又该怎么办呢？



```cpp
p=genpath('F:\学术&工程\ObjectDetect\全景图像集');%获取指定文件夹下所有子文件的路径
        %这些路径存在于字符串p中，以';'分割
length_p=size(p,2);%字符串p的长度
path={};%建立一个元胞数组，数组的每个单元中包含一个目录
temp=[];
for i=1:length_p %寻找分隔符';'，一旦找到，则将路径temp写入path数组中
    if p(i)~=';'
        temp=[temp p(i)];
    else
        temp=[temp '\'];%在路径的后面加上\
        path=[path;temp];
        temp=[];
    end
end
clear p length_p temp
%至此，获得了指定文件夹及其所有子文件夹，接下来就是逐一文件夹读取图像，利用上面的path数组，将路径取出来
file_num=size(path,1);%获取子文件夹的个数
for i=1:file_num
    file_path=path{i}; %获取每个文件夹路径
    img_path_list=dir(strcat(file_path,'*.jpg')); %获取每个文件夹下的图像列表
    img_num=length(img_path_list);%获取文件夹下图像的数量
    if img_num > 0
        for j=1:img_num
            image_name=img_path_list(j).name;%获取图像名
            image=imread(strcat(file_path,image_name));%读取文件夹下的每一帧图像
            fprintf('%d %d %s\n',i,j,strcat(file_path,image_name));%显示正在读取的图像
             %TODO
             %图像处理过程：裁剪，加噪声等
        end
    end
end
```
好了，到现在为止，我们已经学会了如何递归的读取指定文件夹下的所有图片了，基础都打好了，现在我们开始第二步，

对读取的图像进行处理

step2：如何对读取的图像进行处理呢？我们来看，这就是因人而异了，有的需要裁剪，有的需要加噪声，有的需要

处理后并保存到另外一个文件夹，因需求的不同在代码上也有些许差别，但是大体操作还是差不多的呢！好啦，现在

我们一起来看看吧：



```cpp
p=genpath('F:\学术&工程\ObjectDetect\全景图像集');%获取指定文件夹下所有子文件的路径
        %这些路径存在于字符串p中，以';'分割
length_p=size(p,2);%字符串p的长度
path={};%建立一个元胞数组，数组的每个单元中包含一个目录
temp=[];
for i=1:length_p %寻找分隔符';'，一旦找到，则将路径temp写入path数组中
    if p(i)~=';'
        temp=[temp p(i)];
    else
        temp=[temp '\'];%在路径的后面加上\
        path=[path;temp];
        temp=[];
    end
end
clear p length_p temp
%至此，获得了指定文件夹及其所有子文件夹，接下来就是逐一文件夹读取图像，利用上面的path数组，将路径取出来
file_num=size(path,1);%获取子文件夹的个数
for i=1:file_num
    file_path=path{i}; %获取每个文件夹路径
    img_path_list=dir(strcat(file_path,'*.jpg')); %获取每个文件夹下的图像列表
    img_num=length(img_path_list);%获取文件夹下图像的数量
    if img_num > 0
        for j=1:img_num
            image_name=img_path_list(j).name;%获取图像名
            image=imread(strcat(file_path,image_name));%读取文件夹下的每一帧图像
            fprintf('%d %d %s\n',i,j,strcat(file_path,image_name));%显示正在读取的图像
             %TODO
             %图像处理过程
             %裁剪，加噪声
             crop_1=imcrop(image,[0,0,40,40]);
             crop_2=imcrop(image,[80,80,100,100]);
             crop_3=imcrop(image,[50,50,200,200]);
             figure
             imshow(crop_1);
             %imshow(crop_2);
             %imshow(crop_3);
        end
    end
end
```

这里，我们实现了对读入的图像做简单的裁剪，可以裁剪多种尺寸，crop_1;crop_2;crop_3.....裁剪多张。指定裁剪的范围。

对于一张分辨率很高的图像，我们可以指定多个位置，从多个位置开始裁剪，裁剪制定大小，可裁剪成6部分，三行两列，

保证中间的能很好地裁剪到。

A. 想要加噪声的话，添加这两句话进去即可：

```cpp
%加噪声
             noise_1 = imnoise(image,'salt & pepper',0.1); %椒盐
             noise_2 = imnoise(image,'gaussian',0,10) ;%gauss 
             figure
             imshow(noise_1);
```

B. 想要对照片旋转指定角度的话，在处理部分添加这几句话即可：



```cpp
%D. 旋转处理，指定旋转角度
              %旋转后，空余部分的填充如何实现？
              %imrotate(),绕中心旋转？还有绕指定顶点旋转？
              image_rotate_30=imrotate(image,30,'bilinear','loose');
              image_rotate_60=imrotate(image,60,'bilinear','crop');
              figure
              imshow(image_rotate_30);
              figure
              imshow(image_rotate_60);%图像旋转操作
```

C. 想要对图像做缩放的话，貌似直接resize就可以啦



```cpp
%C. 图像缩放
             %按指定倍数缩放
              upSamping= imresize(image,1.5);%放大1.5倍
              downSamping= imresize(image,0.5);%缩小到原来的0.5
            %  imshow(upSamping);
            %  imshow(downSamping);
              %按指定大小缩放
              s_fix_size=imresize(image,[100,100]);
              figure
              imshow(s_fix_size);
              figure
              imshow(image);
```
D. 想要对图像做旋转操作的话，可以使用以下代码：

```cpp
%D. 旋转处理，指定旋转角度
              %旋转后，空余部分的填充如何实现？
              %imrotate(),绕中心旋转？还有绕指定顶点旋转？
              rotate_30=imrotate(image,30,'bilinear','loose');
              rotate_60=imrotate(image,60,'bilinear','crop');
              figure
              imshow(rotate_30);
              figure
              imshow(rotate_60);
              %第一个参数指的是旋转实现的方法，有'nearest' 'bilinear' 'bicubic'，三种
              %就是       最邻近插值法   双线性插值法 三次卷积插值法
              %不同的插值方法得到的旋转图像有细微的差别，默认最近邻插值，图像有一定失真
              %失真主要是因为matlab在计算每个点的新坐标的时候得到的数值不是整数，要去整所造成的
              %第二个参数BBox分为crop和loose两种
```
E. 想要对图像做镜像的话，可以使用以下代码：



```cpp
%E. 图像镜像处理,垂直镜像和水平镜像
              %底层代码实现
              [height,width,dim]=size(image);  
              tform_x=maketform('affine',[-1 0 0;0 1 0;width 0 1]);  
              mirror_x=imtransform(image,tform_x,'nearest');%mirror_x中存储的是经过水平镜像变换后的图像
              figure
              imshow(mirror_x);  %水平镜像
                
              tform_y=maketform('affine',[1 0 0;0 -1 0;0 height 1]);  
              mirror_y=imtransform(image,tform_y,'nearest');%C中存储的是经过竖直镜像变换后的图像
              figure  
              imshow(mirror_y);%垂直镜像
```

好了，图像处理的话我们也做的差不多了,做了这么多处理，那么我们的样本数据就扩充了很多啦！是不是so happy呢？

不过不要高兴的太早哦！接下来我们来谈一谈如何对刚刚处理过的图像做批量的保存。

经过第二步之后，代码量有点多了呢，我们来看看整体代码是怎样的呢？



```
p=genpath('F:\学术&工程\ObjectDetect\全景图像集');%获取指定文件夹下所有子文件的路径
        %这些路径存在于字符串p中，以';'分割
length_p=size(p,2);%字符串p的长度
path={};%建立一个元胞数组，数组的每个单元中包含一个目录
temp=[];
for i=1:length_p %寻找分隔符';'，一旦找到，则将路径temp写入path数组中
    if p(i)~=';'
        temp=[temp p(i)];
    else
        temp=[temp '\'];%在路径的后面加上\
        path=[path;temp];
        temp=[];
    end
end
clear p length_p temp
%至此，获得了指定文件夹及其所有子文件夹，接下来就是逐一文件夹读取图像，利用上面的path数组，将路径取出来
file_num=size(path,1);%获取子文件夹的个数
for i=1:file_num
    file_path=path{i}; %获取每个文件夹路径
    img_path_list=dir(strcat(file_path,'*.jpg')); %获取每个文件夹下的图像列表
    img_num=length(img_path_list);%获取文件夹下图像的数量
    if img_num > 0
        for j=1:1%img_num  %遍历全部照片时候取消这个注释，我这里为了代码测试，没有全部遍历
            image_name=img_path_list(j).name;%获取图像名
            image=imread(strcat(file_path,image_name));%读取文件夹下的每一帧图像
            fprintf('%d %d %s\n',i,j,strcat(file_path,image_name));%显示正在读取的图像
             %TODO
             %图像处理过程
             %A. 裁剪
             image_crop_1=imcrop(image,[0,0,40,40]);
             image_crop_2=imcrop(image,[80,80,100,100]);
             image_crop_3=imcrop(image,[50,50,200,200]);
           %  figure
            % imshow(image_crop_1);
            % imshow(image_crop_2);
            % imshow(image_crop_3);
             %B. 加噪声
             noise_1 = imnoise(image,'salt & pepper',0.1); %椒盐
             noise_2 = imnoise(image,'gaussian',0,10) ;%gauss 
            % figure
            % imshow(noise_1);
             %C. 图像缩放
             %按指定倍数缩放
              upSamping= imresize(image,1.5);%放大1.5倍
              downSamping= imresize(image,0.5);%缩小到原来的0.5
            %  imshow(upSamping);
            %  imshow(downSamping);
              %按指定大小缩放
              s_fix_size=imresize(image,[100,100]);
              figure
              imshow(s_fix_size);
              figure
              imshow(image);
              %D. 旋转处理，指定旋转角度
              %旋转后，空余部分的填充如何实现？
              %imrotate(),绕中心旋转？还有绕指定顶点旋转？
              rotate_30=imrotate(image,30,'bilinear','loose');
              rotate_60=imrotate(image,60,'bilinear','crop');
              figure
              imshow(rotate_30);
              figure
              imshow(rotate_60);
              %第一个参数指的是旋转实现的方法，有'nearest' 'bilinear' 'bicubic'，三种
              %就是       最邻近插值法   双线性插值法 三次卷积插值法
              %不同的插值方法得到的旋转图像有细微的差别，默认最近邻插值，图像有一定失真
              %失真主要是因为matlab在计算每个点的新坐标的时候得到的数值不是整数，要去整所造成的
              %第二个参数BBox分为crop和loose两种
              
              %E. 图像镜像处理,垂直镜像和水平镜像
              %底层代码实现
              [height,width,dim]=size(image);  
              tform_x=maketform('affine',[-1 0 0;0 1 0;width 0 1]);  
              mirror_x=imtransform(image,tform_x,'nearest');%mirror_x中存储的是经过水平镜像变换后的图像
              figure
              imshow(mirror_x);  %水平镜像
                
              tform_y=maketform('affine',[1 0 0;0 -1 0;0 height 1]);  
              mirror_y=imtransform(image,tform_y,'nearest');%C中存储的是经过竖直镜像变换后的图像
              figure  
              imshow(mirror_y);%垂直镜像
                
              %%%*********************
              %  做不同的处理，处理后的图像存入各自指定的文件夹，并且
              %%%*********************
             
        end
    end
end
```

以上就是我们读入图像及对图像做我们想要的处理相关代码哦！！！

step3:批量保存我们刚刚处理过的图像，废话不多说，上代码：（针对不同的处理，文件保存到不同的文件夹下，

便于管理）

A：镜像图像保存



```cpp
% 镜像图像保存
                mirror_path='F:\学术&工程\ObjectDetect\数据处理演示\mirror_x\'; 
               mirror_file=img_path_list(i).name; 
               mirror_pathfile=fullfile(mirror_path,mirror_file); 
               imwrite(mirror_x,mirror_pathfile,'jpg')
```

B：加噪声C:D:E:等方式保存的方法是一样的，只不过需要指定不同的文件路径进行保存。

至此，在深度学习处理数据集的过程中，我们的数据增强工作样本扩充的工作就完成啦！！！！我们再来看看我们的代码吧~



```cpp
p=genpath('F:\学术&工程\ObjectDetect\数据处理演示\全景图像集');%获取指定文件夹下所有子文件的路径
        %这些路径存在于字符串p中，以';'分割
length_p=size(p,2);%字符串p的长度
path={};%建立一个元胞数组，数组的每个单元中包含一个目录
temp=[];
for i=1:length_p %寻找分隔符';'，一旦找到，则将路径temp写入path数组中
    if p(i)~=';'
        temp=[temp p(i)];
    else
        temp=[temp '\'];%在路径的后面加上\
        path=[path;temp];
        temp=[];
    end
end
clear p length_p temp
%至此，获得了指定文件夹及其所有子文件夹，接下来就是逐一文件夹读取图像，利用上面的path数组，将路径取出来
file_num=size(path,1);%获取子文件夹的个数
for i=1:file_num
    file_path=path{i}; %获取每个文件夹路径
    img_path_list=dir(strcat(file_path,'*.jpg')); %获取每个文件夹下的图像列表
    img_num=length(img_path_list);%获取文件夹下图像的数量
    if img_num > 0
        for j=1:1%img_num  %遍历全部照片时候取消这个注释，我这里为了代码测试，没有全部遍历
            image_name=img_path_list(j).name;%获取图像名
                        % 注意，这里的图像读取并不是按照图像的编号顺序读取
            image=imread(strcat(file_path,image_name));%读取文件夹下的每一帧图像
            fprintf('%d %d %s\n',i,j,strcat(file_path,image_name));%显示正在读取的图像
             %TODO
             %图像处理过程
             %A. 裁剪
             image_crop_1=imcrop(image,[0,0,40,40]);
             image_crop_2=imcrop(image,[80,80,100,100]);
             image_crop_3=imcrop(image,[50,50,200,200]);
           %  figure
            % imshow(image_crop_1);
            % imshow(image_crop_2);
            % imshow(image_crop_3);
             %B. 加噪声
             noise_1 = imnoise(image,'salt & pepper',0.1); %椒盐
             noise_2 = imnoise(image,'gaussian',0,10) ;%gauss 
            % figure
            % imshow(noise_1);
             %C. 图像缩放
             %按指定倍数缩放
              upSamping= imresize(image,1.5);%放大1.5倍
              downSamping= imresize(image,0.5);%缩小到原来的0.5
            %  imshow(upSamping);
            %  imshow(downSamping);
              %按指定大小缩放
              s_fix_size=imresize(image,[100,100]);
              figure
              imshow(s_fix_size);
              figure
              imshow(image);
              %D. 旋转处理，指定旋转角度
              %旋转后，空余部分的填充如何实现？
              %imrotate(),绕中心旋转？还有绕指定顶点旋转？
              rotate_30=imrotate(image,30,'bilinear','loose');
              rotate_60=imrotate(image,60,'bilinear','crop');
              figure
              imshow(rotate_30);
              figure
              imshow(rotate_60);
              %第一个参数指的是旋转实现的方法，有'nearest' 'bilinear' 'bicubic'，三种
              %就是       最邻近插值法   双线性插值法 三次卷积插值法
              %不同的插值方法得到的旋转图像有细微的差别，默认最近邻插值，图像有一定失真
              %失真主要是因为matlab在计算每个点的新坐标的时候得到的数值不是整数，要去整所造成的
              %第二个参数BBox分为crop和loose两种
              
              %E. 图像镜像处理,垂直镜像和水平镜像
              %底层代码实现
              [height,width,dim]=size(image);  
              tform_x=maketform('affine',[-1 0 0;0 1 0;width 0 1]);  
              mirror_x=imtransform(image,tform_x,'nearest');%mirror_x中存储的是经过水平镜像变换后的图像
              figure
              imshow(mirror_x);  %水平镜像
                
              tform_y=maketform('affine',[1 0 0;0 -1 0;0 height 1]);  
              mirror_y=imtransform(image,tform_y,'nearest');%C中存储的是经过竖直镜像变换后的图像
              figure  
              imshow(mirror_y);%垂直镜像
               
             % imwrite(mirror_x,['F:\学术&工程\ObjectDetect\数据处理演示\全景图像集\mirror_x\','r', num2str(i) '.jpg']);
             % 这里的'r'会添加到文件名当中，文件名为ri.jpg
             % 如果程序会遍历其子文件夹，那么文件就不要放在这里
             %%%*********************
              %  做不同的处理，处理后的图像存入各自指定的文件夹，并且
              %%%*********************
              
              % 镜像图像保存
                mirror_path='F:\学术&工程\ObjectDetect\数据处理演示\mirror_x\'; 
               mirror_file=img_path_list(i).name; 
               mirror_pathfile=fullfile(mirror_path,mirror_file); 
               imwrite(mirror_x,mirror_pathfile,'jpg');
             
        end
    end
end
```


注意：上述代码会递归查找指定文件路径下的子文件夹的图片，如果不想递归查找，请用下面的代码框架，然后将图

像处理部分原封不动的粘贴进去就可以了。代码如下：



```cpp
file_path='F:\学术&工程\ObjectDetect\全景图像集\';  %图像文件夹路径
img_path_list=dir(strcat(file_path,'*.jpg'));%获取该文件夹下所有jpg格式的图像,是一个结构体，有data,name等成员
                                %img_path_list只包含文件名，不包含具体路径
img_num=length(img_path_list);%获取图像的数量
if img_num >0 %文件夹下有图
    for j=1:img_num  %逐一读取图像
        image_name=img_path_list(j).name;%图像名
        image=imread(strcat(file_path,image_name));%拼接图像名
        fprintf('%d %s\n',j,strcat(file_path,image_name));%显示正在处理的图像名
        %TODO
        %图像处理过程
    end
end
```


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~··2018.2.1更新~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

最新想着还可以将图像转换到HSV空间，做下数据集的扩充，增强颜色纹理信息使之更明显，那么代码该如何实现呢？我们看看：



```cpp

```
%从RGB空间到HSV空间转换,以及查看H,S,V分量的Matlab程序实现
a=imread('Lenna_std.png'); 
hv=rgb2hsv(a); %rgb2hsv

scale=rand*(1.2-0.8)+0.8;%hsv三个分量随机变成原始值的0.8~1.2

hsv=rgb2hsv(image)*scale;

scale=sprintf('%d\n',scale);

disp(scale);

figure;

subplot(1,2,1);imshow(image);title('原始图像');

subplot(1,2,2);imshow(hsv);title('hsv空间图像');%可以通过下面的程序看一幅图的HSV三个通道 H=hv(:,:,1);S=hv(:,:,2);V=hv(:,:,3);figure;subplot(1,2,1);imshow(a);title('原始图像'); subplot(1,2,2);imshow(hv);title('HSV空间I图像');figure;subplot(1,3,1);imshow(H);title('HSV空间H分量图像');subplot(1,3,2);imshow(S);title('HSV空间S分量图像');subplot(1,3,3);imshow(V);title('HSV空间V分量图像')


上面的程序不仅仅将rgb转换到了hsv而且还将hsv各个通道中的值显示了出来。

完整的hsv变换及图像保存方法代码为：



```cpp
file_path='F:\学术&工程\ObjectDetect\电力线图像集\原始图像\';  %图像文件夹路径
img_path_list=dir(strcat(file_path,'*.jpg'));%获取该文件夹下所有jpg格式的图像,是一个结构体，有data,name等成员
                                %img_path_list只包含文件名，不包含具体路径
img_num=length(img_path_list);%获取图像的数量
if img_num >0 %文件夹下有图
    for j=1:img_num  %逐一读取图像
        image_name=img_path_list(j).name;%图像名
        image=imread(strcat(file_path,image_name));%拼接图像名
        fprintf('%d %s\n',j,strcat(file_path,image_name));%显示正在处理的图像名
        %TODO
        %图像处理过程
        
        %rgb2hsv,并将hsv各个通道的值变为原来的0.8到1.2倍。
        scale=rand*(1.2-0.8)+0.8;
        hsv_image=rgb2hsv(image)*scale;
        scale=sprintf('%d\n',scale);
        disp(scale);
        figure;
        subplot(1,2,1);imshow(image);title('原始图像');
        subplot(1,2,2);imshow(hsv_image);title('hsv空间图像');
        
        %%保存
         hsv_path='F:\学术&工程\ObjectDetect\电力线图像集\数据集扩充\hsv_image'; 
         hsv_file=img_path_list(j).name; 
         hsv_pathfile=fullfile(hsv_path,hsv_file); 
         imwrite(hsv_image,hsv_pathfile,'jpg');
    end
end
```


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



接下来开始几千几万张图像的标定工作了，想想就觉得小激动。。。。，下一篇文章更新标签软件的使用方法，

并说明如何用Python编程修改xml文件中指定属性的文本内容，敬请期待~~~






