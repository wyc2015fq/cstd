function shape_curstage=randomshape(shape_ref,detec_bbox,num_samples)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%函数功能：随机形状初始化
%输入：
%shape_ref:真实的人脸形状
%detec_bbox：检测出的人脸框参数
%num_samples：样本个数
%输出：
%shape_curstage：初始化后的形状
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
num=size(shape_ref,1);
for i=1:num
    randnum=randint(1,1,[1,num_samples]);%样本中随机一个形状
    while(randnum==mod(i,num_samples))%如果随机的数等于本身，重复执行
        randnum=randint(1,1,[1,num_samples]);
    end
    randshape=shape_ref(randnum,:);%得到随机形状
    shape_curstage(i,:)=resetshape(randshape,detec_bbox(i,:));%利用检测出来的人脸框初始化形状
end
end