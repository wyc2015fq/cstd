function [Pt Pt_new]=isample_traverse(forestmodel,Radius,shape_curstage,i_stage,num_points,NumOfTree,imgTe_gray,bbox3,bbox4,max_depth)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%函数功能：测试样本遍历森林模型，获得样本的高维二值特征
%输入：
%forestmodel：森林模型
%Radius：圆域半径
%shape_curstage:当前样本形状
%i_stage：当前阶段
%num_points：特征点数
%NumOfTree：森林中树的数目
%imgTe_gray：测试样本的灰度图数据
%bbox3,bbox4：当前人脸宽高
%输出：
%Pt：二值特征
%阶段-->点-->树
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
radius=Radius(i_stage);
Pt=[];Pt_new=[];
for i_point = 1:num_points
    forest=forestmodel{i_stage}(i_point,:);%当前点森林模型
    x0=shape_curstage(i_point); y0=shape_curstage(i_point+num_points);%当前点坐标
    [pt pt_new]=getlbf(forest,radius,NumOfTree,imgTe_gray,x0,y0,bbox3,bbox4,max_depth);%LBF
    Pt=[Pt,pt];%BF 
    Pt_new=[Pt_new,pt_new];
end
end

function [Qt,Qt_new]=getlbf(forest,radius,NumOfTree,imgTe_gray,x0,y0,bbox3,bbox4,max_depth)
    %与训练的一样，不再注释。
    bbox3=bbox3*radius;bbox4=radius*bbox4;
    [height,width]=size(imgTe_gray);
    Qt=[];Qt_new=[];
     for ii=1:2^(max_depth-1)-1
         cnodes(ii,:)=[2*ii,2*ii+1];
     end
     num_leafnodes=2^(max_depth-1);
     id_leafnodes=2^(max_depth-1):2^max_depth-1;
     
    for i_tree = 1:NumOfTree
        tree=forest{i_tree};
        %%遍历树
        id_cnode=1;
        while(1)
           if id_cnode>=2^(max_depth-1)
              j=find(id_leafnodes==id_cnode);break;
           end
           feat=tree.feat(id_cnode,:);
           ax=round(feat(1)*bbox3+x0);%像素点a的x坐标值
           bx=round(feat(2)*bbox3+x0);%像素点b的x坐标值
           ay=round(feat(3)*bbox4+y0);%像素点a的y坐标值
           by=round(feat(4)*bbox4+y0);%像素点b的y坐标值
           ax = max(1, min(ax, width));
           ay = max(1, min(ay, height));
           bx = max(1, min(bx, width));
           by = max(1, min(by, height));
           pdfeat=int16(imgTe_gray(ay,ax))-int16(imgTe_gray(by,bx));
           cind = (pdfeat>tree.thresh(id_cnode)) + 1;
           id_cnode = cnodes(id_cnode,cind);
        end   
        qt=sparse(1,j,1,1,num_leafnodes);
        Qt=[Qt,qt];Qt_new=[Qt_new,j];
    end
end

