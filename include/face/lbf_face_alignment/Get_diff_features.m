function [pixel_diff_features,pixelpair]=Get_diff_features(radius,imgTr,shape_curstage,idsample,i_point,num_samples,num_features)%,tf2)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%函数功能：获得差值特征
%输入：
%radius：圆域半径
%imgTr：训练样本数据
%shape_curstage：当前样本形状
%idsample：根节点样本集
%i_point：当前特征点
%num_samples：样本数
%num_features：样本特征数
%输出：
%pixel_diff_features 差值特征
%pixelpair：差值特征对应的角度与极半径
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       
    [pixel_coordinate,pixelpair]=Get_Coordinate(radius,num_features);
    
    for i_sample = 1:length(idsample)%对每一个训练样本
        i_Tr=idsample(i_sample)-num_samples*(ceil(idsample(i_sample)/num_samples)-1);%样本的序号
        imgTr_gray=imgTr{i_Tr};
        [height,width]=size(imgTr_gray);
        shape=shape_curstage(idsample(i_sample),:);num_points=size(shape_curstage,2)/2;
        bbox3=max(shape(1:num_points))-min(shape(1:num_points));%当前人脸宽
        bbox4=max(shape(1+num_points:end))-min(shape(1+num_points:end));%当前人脸高
        x0=shape_curstage(idsample(i_sample),i_point); y0=shape_curstage(idsample(i_sample),i_point+68);%当前点坐标
        %得到当前样本的1000个像素的实际坐标及灰度值
        pixel_coordinate_x=pixel_coordinate(1:length(pixel_coordinate)/2)*bbox3;%相对坐标x
        pixel_coordinate_y=pixel_coordinate(1+length(pixel_coordinate)/2:end)*bbox4;
     
        pixel_coordinate_x=round(pixel_coordinate_x+x0);%绝对坐标x
        pixel_coordinate_y=round(pixel_coordinate_y+y0);
        
        pixel_coordinate_x=max(min(pixel_coordinate_x,width),1);%坐标约束
        pixel_coordinate_y=max(min(pixel_coordinate_y,height),1);
        
        pixel_a_x=pixel_coordinate_x(1:length(pixel_coordinate_x)/2);    %前一半像素点的x坐标值
        pixel_b_x=pixel_coordinate_x(1+length(pixel_coordinate_x)/2:end);%后一半像素点的x坐标值
        pixel_a_y=pixel_coordinate_y(1:length(pixel_coordinate_y)/2);    %前一半像素点的y坐标值
        pixel_b_y=pixel_coordinate_y(1+length(pixel_coordinate_y)/2:end);%后一半像素点的y坐标值
        
        pixel_diff_features(i_sample,:)=int16(imgTr_gray(pixel_a_y + (pixel_a_x-1)*height))-...
                                        int16(imgTr_gray(pixel_b_y + (pixel_b_x-1)*height));%像素差特征
    end
end

function [pixel_coordinate,pixelpair]=Get_Coordinate(radius,num_features)
%函数功能：
%获得局部半径里像素坐标
%先用极坐标表示，后转成直角坐标
        radius_grid = ([0:1/30:1]');%把半径均分
        angles_grid = 2*pi*[0:1/36:1]';%把角度均分
        [radiuspairs, anglepairs] = getproposals(num_features,radius_grid,angles_grid);%得到随机的半径对及角度对（极坐标）
        pixelpair=[anglepairs radiuspairs];%像素对
        angles_cos = cos(anglepairs);%x方向为cos
        angles_sin = sin(anglepairs);%y方向为sin
        pixel_coordinate=[(angles_cos(:, 1).*radiuspairs(:, 1)*radius)',(angles_cos(:, 2).*radiuspairs(:, 2)*radius)',...
                          (angles_sin(:, 1).*radiuspairs(:, 1)*radius)',(angles_sin(:, 2).*radiuspairs(:, 2)*radius)'];%转成直角坐标
end

function [radiuspairs, anglepairs] = getproposals(num_features,radius_grid,angles_grid)

num_radius = length(radius_grid);%半径个数
num_angles = length(angles_grid);%角度个数
%num_radius*num_angles：角度与半径的组合数
Pro_a = randperm(num_radius*num_angles);%对组合情况随机排列
Pro_b = randperm(num_radius*num_angles);

exc = Pro_a == Pro_b;%判断a,b相应位置是否相等

Pro_a = Pro_a(exc == 0);%剔除a=b的那种情况
Pro_b = Pro_b(exc == 0);

Pro_a_choose = Pro_a(1:num_features);%选择要求的特征数目
Pro_b_choose = Pro_b(1:num_features);

id_radius_a = floor((Pro_a_choose - 1)/num_angles) + 1;%对应的半径id
id_radius_b = floor((Pro_b_choose - 1)/num_angles) + 1;

id_angles_a = mod(Pro_a_choose, num_angles) + 1;%对应的角度id
id_angles_b = mod(Pro_b_choose, num_angles) + 1;

radiuspairs = [radius_grid(id_radius_a) radius_grid(id_radius_b)];%半径对
anglepairs  = [angles_grid(id_angles_a) angles_grid(id_angles_b)];%角度对
end
