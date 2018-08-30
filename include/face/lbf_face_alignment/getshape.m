function realshape=getshape(pic_path)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%函数功能：
%输入：
%pic_path：样本所在路径
%输出：
%realshape：样本真实形状
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%读取样本的人脸形状
shapepath = strcat(pic_path(1:end-3), 'pts');%样本对应的pts路径
file = fopen(shapepath);
pts = textscan(file, '%f %f', 'HeaderLines', 3, 'CollectOutput', 2);%读取文本数据
realshape=pts{1};%人脸形状
fclose(file);%关闭文件
end