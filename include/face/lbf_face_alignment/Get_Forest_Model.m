function [Delta,Wt,Forest_Model]=Get_Forest_Model(radius,imgTr,shape_curstage,num_points,num_samples_Aug,NumOfTree,max_depth,Deltas,num_samples,num_features)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%函数功能：建立森林模型
%输入：
%radius：局部区域半径
% imgTr:样本数据
%shape_curstage：当前形状
%num_points：特征点数目
%num_samples_Aug：扩大的样本数目
%NumOfTree：树的数目
%max_depth：最大层数
%Deltas：真实形状偏差
%num_samples：样本数目
%num_features：特征数目
%输出：
%Delta：预测的形状增量
%Wt：回归矩阵
%Forest_Model：森林模型
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    forests=cell(num_points,NumOfTree);%森林初始化
    BF=[];  %二值特征初始化
    for i_point = 1:num_points
        LBF=[];%局部二值特征初始化
        forest=cell(1,NumOfTree);
        tic;
        for i_tree=1:NumOfTree
            
            %%%%%%%%%%%%%%建树开始%%%%%%%%%%%%%%%%%%%%%
            %
            %随机选择部分样本，取1/10
            randsample=randperm(num_samples_Aug);
            idsample=randsample(1:ceil(num_samples_Aug/10));
            %}
            [pixel_diff_features,pixelpair]=Get_diff_features(radius,imgTr,shape_curstage,idsample,i_point,num_samples,num_features); %pixel_diff_features：像素差特征；pixelpair：特征对应的像素点
            Deltap=[Deltas(idsample,i_point) Deltas(idsample,i_point+num_points)];%样本在点i_point的偏差。
            rt=regtree(Deltap,max_depth,pixel_diff_features,pixelpair); %得到树模型
            %%%%%%%%%%%%%%建树完成%%%%%%%%%%%%%%%%%%%%%
            
            forest{i_tree}=rt;%保存n颗树，即森林
            %局部二值特征
            bincode=get_lbf(rt,num_samples,num_samples_Aug,radius,imgTr,shape_curstage,i_point,num_points,max_depth);
            LBF=[LBF,bincode];%所有样本在i_point的局部特征
        end
        toc;
        forests(i_point,:)=forest;%保存68个点森林
        BF=[BF,LBF];%串联所有局部特征
    end
    %toc;
    Forest_Model=forests;
    Wt=get_wt(BF,Deltas);%全局回归矩阵
    Delta=BF*Wt;%得到形状增量
end

function  bincode=get_lbf(rt,num_samples,num_samples_Aug,radius,imgTr,shape_curstage,i_point,num_points,max_depth)
          bincode=[];
          for ii=1:2^(max_depth-1)-1
          rt.cnodes(ii,:)=[2*ii,2*ii+1];
          end
          rt.id_leafnodes=2^(max_depth-1):2^max_depth-1;rt.num_leafnodes=2^(max_depth-1);
          for i_sample = 1:num_samples_Aug%对每一个训练样本
              i_Tr=i_sample-num_samples*(ceil(i_sample/num_samples)-1);
              x0=shape_curstage(i_sample,i_point); y0=shape_curstage(i_sample,i_point+num_points);%样本i_sample在点i_point的坐标
              bbox3=max(shape_curstage(i_sample,1:num_points))-min(shape_curstage(i_sample,1:num_points));%样本i_sample，人脸宽
              bbox3=bbox3*radius;
              bbox4=max(shape_curstage(i_sample,1+num_points:end))-min(shape_curstage(i_sample,1+num_points:end));%样本i_sample，人脸高
              bbox4=radius*bbox4;
              [height,width]=size(imgTr{i_Tr});%样本i_sample，宽高
              %%遍历树
              id_cnode=1;%遍历开始时的结点id
              while(1)
                    if id_cnode>=2^(max_depth-1)%判断当前结点是否是叶结点
                        j=find(rt.id_leafnodes==id_cnode);break;%如果是叶节点，找出它是第几个叶节点，并跳出循环，否则继续
                    end
                    feat=rt.feat(id_cnode,:);%当前结点中的特征参数（半径，角度）
                    ax=round(feat(1)*bbox3+x0);%像素点a的x坐标值
                    bx=round(feat(2)*bbox3+x0);%像素点b的x坐标值
                    ay=round(feat(3)*bbox4+y0);%像素点a的y坐标值
                    by=round(feat(4)*bbox4+y0);%像素点b的y坐标值
                    ax = max(1, min(ax, width));%给与坐标约束，不能超出图片坐标范围
                    ay = max(1, min(ay, height));
                    bx = max(1, min(bx, width));
                    by = max(1, min(by, height));
                    pdfeat=int16(imgTr{i_Tr}(ay,ax))-int16(imgTr{i_Tr}(by,bx));%得到像素差特征
                    cind = (pdfeat>rt.thresh(id_cnode)) + 1;%判断样本会落入当前结点的左结点还是右结点，大于为右结点，小于为左结点
                    id_cnode = rt.cnodes(id_cnode,cind);%得到当前结点子结点的id.
              end   
                qt=sparse(1,j,1,1,rt.num_leafnodes);%稀疏的局部二值特征
                bincode=[bincode;qt];
          end
end

function Wt=get_wt(BF,Deltas)
param = sprintf('-s 12 -p 0 -c %f -q heart_scale', 1/(size(BF, 1)));%设定参数
W_liblinear = zeros(size(BF, 2), size(Deltas, 2));%回归矩阵初始化

for o = 1:size(Deltas, 2)
    model = train(Deltas(:, o), BF, param);%得到模型
    W_liblinear(:, o) = model.w';
end

Wt = W_liblinear;
end

