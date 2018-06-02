function [img,realshapes,save_path]=resizeimg(dbnames,mode)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%函数功能：对图片进行人脸抠出及尺寸缩放
%输入：
%dbnames：数据库名
%mode：模式
%输出：
%img：样本数据
%realshapes：样本真实形状
%save_path：样本保存路径
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
num_points=68;
work_path = pwd;
imgpathlists=[];
ifresize=1;%是否对图片进行缩小处理，1为是
for i=1:length(dbnames)
    imgpathlistfile = strcat(work_path,'\300-w\',mode,'\', dbnames{i}, '\Path_Images.txt');%数据库路径存储文本
    imgpathlist = textread(imgpathlistfile, '%s', 'delimiter', '\n');%数据库路径
    imgpathlists=[imgpathlists;imgpathlist];
    fprintf(strcat(num2str(dbnames{i}),':',num2str(length(imgpathlist)),'\n'));%显示该数据库的样本数。
end
    %变量初始化
    num_samples=length(imgpathlists);
    img=cell(1,num_samples);
    realshapes=zeros(num_samples,num_points*2);

for i_sample=1:num_samples
    pic_path = imgpathlists{i_sample};%样本路径
    img_rgb = im2uint8(imread(pic_path));%读取数据
    realshape=getshape(pic_path);%读取形状
    [height,width,a]=size(img_rgb);%图片宽高
    minx=min(realshape(:,1));maxx=max(realshape(:,1));
    miny=min(realshape(:,2));maxy=max(realshape(:,2));
    w=maxx-minx;h=maxy-miny;%人脸宽，高
    leftx=max(1,floor(minx-w/4));rightx=min(width,floor(maxx+w/4));%左右边界赋值，给与相应约束。
    topy=max(1,floor(miny-h/4));bottomy=min(height,floor(maxy+h/4));%上下边界赋值，给与相应约束。
    img_color=img_rgb(topy:bottomy,leftx:rightx,:);%截取人脸。
    realshape=[realshape(:,1)-leftx,realshape(:,2)-topy];%对应的形状做相应变化。
    clear img_rgb;
        %
        %训练样本做灰度化，测试样本保持彩色图
        if strcmp(mode,'train')
        if ndims(img_color)==3, img_color=rgb2gray(img_color);end
        end
        %}
        %
        %缩小
     if ifresize
        [height,width,a]=size(img_color);
        size_fac=ceil(height/300);%缩小倍数
        if size_fac==1
            img_inter=img_color;%不变
            realshape_inter=realshape;
        else
            img_inter=imresize(img_color,1/size_fac,'bilinear');%以size_fac倍数缩小
            realshape_inter=realshape/size_fac;
        end
     else
         img_inter=img_color;%不变
         realshape_inter=realshape;
     end
        %}
        %
     img{i_sample}=img_inter;
     realshapes(i_sample,:)=realshape_inter(:);
     save_path{i_sample}=strcat(pic_path(length(work_path)+8:end-8),num2str(i_sample,'%04d'),'.jpg');%设置图片保存路径
     
     %
     %保存图片
     imwrite(img_inter,save_path{i_sample});
     %保存坐标
     fp_w=fopen(strcat(save_path{i_sample}(1:end-4),'.pts'),'w');
     fprintf(fp_w,'%s %d\n','version:',1);
     fprintf(fp_w,'%s %d\n','n_points:',num_points);
     fprintf(fp_w,'%s\n','{');
     fprintf(fp_w,'%f %f\n',realshape_inter');
     fprintf(fp_w,'%s','}');
     fclose(fp_w);
     %}
     clear img_color;
     clear img_inter;
end
end