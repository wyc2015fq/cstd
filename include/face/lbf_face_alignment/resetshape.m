function shape=resetshape(shape_in,ref_location)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%函数功能：根据人脸检测的位置初始化人脸形状
%输入：
%shape_in：输入的形状（随机或者平均形状）
%ref_location：检测器检测到的位置
%输出：
%shape：初始化的人脸形状
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bbox(1)=ref_location(1);%left_x
bbox(2)=ref_location(3);%top_y
bbox(3)=ref_location(2)-ref_location(1);%width
bbox(4)=ref_location(4)-ref_location(3);%height

num_points = size(shape_in,2)/2;%人脸特征点数
width = max(shape_in(1:num_points)) - min(shape_in(1:num_points));%人脸宽
height= max(shape_in(num_points+1:end)) - min(shape_in(num_points+1:end));%人脸高
%把输入的形状归一化到1，然后放大到人脸框大小，最后对人脸形状进行平移。
shape = [bbox(3)/width*(shape_in(1:num_points)-min(shape_in(1:num_points)))+bbox(1),...
         bbox(4)/height*(shape_in(num_points+1:end)-min(shape_in(num_points+1:end)))+bbox(2)];
end
