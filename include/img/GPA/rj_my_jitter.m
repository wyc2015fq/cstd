%本程序的功能是抑制摄像机抖动(电子稳像)，用到的好像是灰度投影算法(GPA)
%适用于当前测试帧相对于当前参考帧最大平移20个像素(单方向)以内的情况，也就是说测试序列的前后两帧最大平移20个像素(单方向)以内的情况
%而且摄像头与场景不能太近，实质上离得太近的话场景内的前景会移动过快，也就是前后两帧的同一移动物体移动得幅度较大，所以会影响计算运动矢量
%参考文献：
%(1)一种基于灰度投影算法的电子稳像方法，朱娟娟 郭宝龙 冯宗哲
%(2)获取动态图像位移矢量的灰度投影算法的应用,赵红颖，熊经武,中国科学院长春光学精密机械及物理研究所，吉林长春
%(3)提高用于电子稳像的灰度投影算法精度的方法,钟 平，于前洋，王明佳，金 光(中国科学院长春光学精密机械与物理研究所，吉林长春130022)
clc;clear all;close all
offset_V = 20;%上下边缘去除量，注意：offset_V 和 offset_H要根据实际可能的最大抖动量适当取值，但不能太大，太大的话相关计算时移动位置的候选位置就少，所以会效果反而会降低，取20和30比较折中
offset_H = 30;%左右边缘去除量

d1 = 'F:\pub\bin\GPA\DOUDONG\test\*.bmp';
d2 = dir(d1);
numframe = length(d2);
frame_name = num2str(d2(1).name);
frame_path = strcat('F:\pub\bin\GPA\DOUDONG\test\',frame_name);
referenceframe_rgb = imread(frame_path);
[row,col,d] = size(referenceframe_rgb);
referenceframe = rgb2gray(referenceframe_rgb);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 参考帧的行投影和列投影以及余弦滤波 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%**********投影*********%
refprojrow = zeros(row,1);%存储参考帧的行投影
refprojrow     = sum(referenceframe,2);
refprojrowsum  = sum(refprojrow);
refprojrowmean = refprojrowsum / row;
refprojrow     = refprojrow - refprojrowmean;

refprojcol = zeros(1,col);%存储参考帧的列投影
refprojcol     = sum(referenceframe,1);
refprojcolsum  = sum(refprojcol);
refprojcolmean = refprojcolsum / col;
refprojcol     = refprojcol - refprojcolmean;


%**********对于边缘进行余弦滤波*********%
for i=1:row
    if (i<offset_V)||(i>row-offset_V)
        refprojrow(i)=refprojrow(i)*((1+cos(pi*(offset_V-1-i)/offset_V))/2);%或者改为：refprojrow(i)=refprojrow(i)*(1+cos(pi*(offset_V-1-i)/offset_V/2));哪个效果更好就用哪个，改的话以下类似的地方都改
    end
end

for j=1:col
    if (j<offset_H)||(j>col-offset_H)
        refprojcol(j)=refprojcol(j)*((1+cos(pi*(offset_H-1-j)/offset_H))/2);
    end
end



%%%%%%%%%%%%%%%%%%%
% 当前帧稳像主程序 %
%%%%%%%%%%%%%%%%%%%

for t = 2:numframe
    if mod(t,20) == 0
        clc;
        disp(strcat('正在进行稳像处理，已完成',num2str(round(t*100/numframe)),'%'));
    end
    frame_name = num2str(d2(t).name);
    frame_path = strcat('F:\pub\bin\GPA\DOUDONG\test\',frame_name);
    currentframe_rgb = imread(frame_path);
    currentframe = rgb2gray(currentframe_rgb);
    
    
    %**********投影*********%
    curprojrow = zeros(row,1);
    curprojrow     = sum(currentframe,2);
    curprojrowsum  = sum(curprojrow);
    curprojrowmean = curprojrowsum / row;
    curprojrow     = curprojrow - curprojrowmean;

    curprojcol = zeros(1,col);
    curprojcol     = sum(currentframe,1);
    curprojcolsum  = sum(curprojcol);
    curprojcolmean = curprojcolsum / col;
    curprojcol     = curprojcol - curprojcolmean;
    
    
    %**********余弦滤波*********%
    for i=1:row
        if (i<offset_V)||(i>row-offset_V)
            curprojrow(i)=curprojrow(i)*((1+cos(pi*(offset_V-1-i)/offset_V))/2);
        end
    end

    for j=1:col
        if (j<offset_H)||(j>col-offset_H)
            curprojcol(j)=curprojcol(j)*((1+cos(pi*(offset_H-1-j)/offset_H))/2);
        end
    end
    
    
    %**********互相关运算计算运动矢量*********%
    correlation_row = zeros(1,2*offset_V+1);
    extend_row = zeros(2*offset_V+1,1);
    refprojrow = [refprojrow;extend_row];
    curprojrow = [curprojrow;extend_row];
    for j=1:2*offset_V+1
        for i=1:row-2*offset_V
            correlation_row(j)=correlation_row(j)+(refprojrow(j+i-1)-curprojrow(offset_V+i))^2;
        end
    end
    [temp,min_index]=min(correlation_row); 
    dy=(offset_V+1)-min_index;%垂直抖动量(正负),即垂直方向运动矢量
    
    correlation_col = zeros(1,2*offset_H+1);
    extend_col = zeros(1,2*offset_H+1);
    refprojcol = [refprojcol extend_col];
    curprojcol = [curprojcol extend_col];
    for i=1:2*offset_H+1
        for j=1:col-2*offset_H
            correlation_col(i)=correlation_col(i)+(refprojcol(j+i-1)-curprojcol(offset_H+j))^2;
        end
    end
    [temp,min_index]=min(correlation_col); 
    dx=(offset_H+1)-min_index;%水平抖动量(正负)，即水平方向运动矢量
    
    
    %**********运动补偿*********%
    if     dy < 0 %dy < 0当前帧currentframe相对于参考帧referenceframe向上运动了|dy|(想不明白就画图)
        temp_sub   = zeros(abs(dy),col,d);
        tempframe_rgb  = [temp_sub;currentframe_rgb(1:(row - abs(dy)),:,:)];
    elseif dy > 0 %dy > 0当前帧currentframe相对于参考帧referenceframe向下运动了|dy|
        temp_sub   = zeros(abs(dy),col,d);
        tempframe_rgb  = [currentframe_rgb((1+abs(dy)):row,:,:);temp_sub];
    else          %dy = 0当前帧currentframe相对于参考帧referenceframe没有运动
        tempframe_rgb  = currentframe_rgb;
    end

    if     dx < 0 %dx < 0当前帧currentframe相对于参考帧referenceframe向左运动了|dx|(想不明白就画图)
        temp_sub   = zeros(row,abs(dx),d);
        tempframe_rgb  = [temp_sub tempframe_rgb(:,1:(col - abs(dx)),:)];
    elseif dx > 0 %dx > 0当前帧currentframe相对于参考帧referenceframe向右运动了|dx|
        temp_sub   = zeros(row,abs(dx),d);
        tempframe_rgb  = [tempframe_rgb(:,1+abs(dx):col,:) temp_sub];
    end
    compensateframe_rgb = tempframe_rgb;%当前补偿帧(稳定帧)
    
    
    %**********把当前稳定帧做为下一帧的参考帧*********%
    referenceframe_rgb = compensateframe_rgb;
    [row,col,d] = size(referenceframe_rgb);
    referenceframe = rgb2gray(referenceframe_rgb);
    
    %**投影**%
    refprojrow = zeros(row,1);%存储参考帧的行投影
    refprojrow     = sum(referenceframe,2);
    refprojrowsum  = sum(refprojrow);
    refprojrowmean = refprojrowsum / row;
    refprojrow     = refprojrow - refprojrowmean;

    refprojcol = zeros(1,col);%存储参考帧的列投影
    refprojcol     = sum(referenceframe,1);
    refprojcolsum  = sum(refprojcol);
    refprojcolmean = refprojcolsum / col;
    refprojcol     = refprojcol - refprojcolmean;

    %**余弦滤波**%
    for i=1:row
        if (i<offset_V)||(i>row-offset_V)
            refprojrow(i)=refprojrow(i)*((1+cos(pi*(offset_V-1-i)/offset_V))/2);
        end
    end

    for j=1:col
        if (j<offset_H)||(j>col-offset_H)
            refprojcol(j)=refprojcol(j)*((1+cos(pi*(offset_H-1-j)/offset_H))/2);
        end
    end

    
    resultframe_rgb = compensateframe_rgb(offset_V+1:row-offset_V,offset_H+1:col-offset_H,:);%对边缘进行截断
    fname_total = strcat('F:\pub\bin\GPA\DOUDONG\out\',frame_name);
    imwrite(resultframe_rgb,fname_total);
end
clc;


