function [ error_of_image ] = compute_error(ground_truth_points, detected_points)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%函数功能：计算单个样本误差
%输入：
%ground_truth_points：真实形状
%detected_points：检测到的形状
%输出：
%error_of_image：单个样本误差
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    num_points = size(ground_truth_points,1);%特征点数目
    interocular_distance = norm(mean(ground_truth_points(37:42,:))-mean(ground_truth_points(43:48,:))); %眼睛中心距离
    sum=0;
    for j=1:num_points
        sum = sum+norm(detected_points(j,:)-ground_truth_points(j,:));%所有特征点误差和
    end
    error_of_image = sum/(num_points*interocular_distance);%单个图片的误差

end