clc
clear all
%d_name=strcat('E:\测试图片\railway','\*.bmp');
d_name=strcat('F:\extraction','\*.jpg');

D_name=dir(d_name);
row=240;col=320;
% row=167;col=250;
W=zeros(row,col,3);
M=cell(row,col);
V=zeros(row,col,3); 
afa=0.005;
for U=1:20
    U
    tic
    num = num2str(D_name(U).name);
    fname=strcat('F:\extraction\',num);
    I=imread(fname);
    I=imresize(I,[row col]);
    I=double(I);
    if U==1
        for i=1:row
            for j=1:col
              W(i,j,1)=1;
              V(i,j,1)=5;
              M{i,j}(1,:)=[I(i,j,1) I(i,j,2) I(i,j,3)];
              k=1;
            end
        end
    end
    if U>1
        for i=1:row
             for j=1:col
                 k=size(M{i,j},1);%%%%%%高斯模型的个数
 %%%%%%%%%%%计算当前像素点与各个高斯模型间的距离
                 for m=1:k
                     D(m,:)=abs([I(i,j,1) I(i,j,2) I(i,j,3)]-M{i,j}(m,:));
                 end
                  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                  if k==1
                      if ((D(1,1)<=2.5*V(i,j,1))&(D(1,2)<=2.5*V(i,j,1))&(D(1,3)<=2.5*V(i,j,1)))%%%%%%%RGB三通道的差值都要小于2.5倍方差
                          W(i,j,1)=(1-afa)*W(i,j,1)+afa;
                          theta=afa/W(i,j,1);
                          M{i,j}(1,:)=(1-theta)*M{i,j}(1,:)+theta*[I(i,j,1) I(i,j,2) I(i,j,3)];
                          V(i,j,1)=(abs((1-theta)*V(i,j,1)^2+theta*D(1,:)*D(1,:)'))^(0.5);
                      else
                          W(i,j,1)=(1-afa)*W(i,j,1); 
                          W(i,j,2)=0.05;
                          V(i,j,2)=5;
                          M{i,j}(2,:)=[I(i,j,1) I(i,j,2) I(i,j,3)];
                      end
                  else
         %%%%%%%%%将高斯模型按权重比方差的大小从大到小排序
                      W1=zeros(1,k);
                      W2=zeros(1,k);
                      W1(1,:)=W(i,j,1:k)./V(i,j,1:k);
                      W2=sort(W1);%%%%从小到大排序
                      e=1;
        %%%%%%%从大到小
                     for q=k:-1:1
                         INdex=find(W1==W2(1,q));
                         index(e)=min(INdex);
                         e=e+1;
                     end
         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                     x=0;
                     for m=1:k
                        if ((D(index(m),1)<=2.5*V(i,j,index(m)))&(D(index(m),2)<=2.5*V(i,j,index(m)))&(D(index(m),3)<=2.5*V(i,j,index(m))))&(x==0)
                            x=1;
                            W(i,j,index(m))=(1-afa)*W(i,j,index(m))+afa;
                            theta=afa/W(i,j,index(m));
                            M{i,j}(index(m),:)=(1-theta)*M{i,j}(index(m),:)+theta*[I(i,j,1) I(i,j,2) I(i,j,3)];
                            V(i,j,index(m))=(abs((1-theta)*V(i,j,index(m))^2+theta*D(index(m),:)*D(index(m),:)'))^(0.5);
                        else
                            W(i,j,index(m))=(1-afa)*W(i,j,index(m)); 
                        end
                     end
            %%%%%如果不匹配
                   if x==0
                       if k==3
                          Index=find(W(i,j,:)./V(i,j,:)==min(W(i,j,:)./V(i,j,:)));
                          W(i,j,Index)=0.05;
                          M{i,j}(Index,:)=[I(i,j,1) I(i,j,2) I(i,j,3)];
                          V(i,j,Index)=5;
                       end
                       if k==2
                          W(i,j,3)=0.05;
                          V(i,j,3)=5;
                          M{i,j}(3,:)=[I(i,j,1) I(i,j,2) I(i,j,3)];
                       end
                   end
                  end
                 W(i,j,:)=W(i,j,:)/sum(W(i,j,:));%%%%权重归一化 
             end
        end
    end
%      %%%%%%%%显示最大可能的背景
     for i=1:row
         for j=1:col
             index=find(W(i,j,:)~=0);
             Index=find(W(i,j,index)./V(i,j,index)==max(W(i,j,index)./V(i,j,index)));
             IB(i,j,:)=M{i,j}(Index,:);
         end
     end
     %%%%%%%%%%
    toc
end
figure;imshow(IB)
% save W_b W;
% save M_b M;
% save V_b V;





