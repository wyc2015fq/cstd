% function Label=jiance(W,M,V,I,IB,row,col);
clc
clear all
%d_name=strcat('E:\测试图片\railway','\*.bmp');

d_name=strcat('F:\extraction','\*.bmp');
D_name=dir(d_name);
row=240;col=320;
afa=0.005;
T1=0.4;%%%%%%%背景的阈值
load W_b.mat
load M_b.mat
load V_b.mat
for U=1:500
    tic
    U
    num = num2str(D_name(U).name);
    fname=strcat('E:\测试图片\railway\',num);
    I=imread(fname);
    I=imresize(I,[row col]);
    I=double(I);
Label=zeros(row,col);
for i=1:row
    for j=1:col
        r=0;
%         K=size(M{i,j},1);
%%%%%%%%%暂时改为这个
         index=find(W(i,j,:)==0);
         nnn=size(index,1);
         K=3-nnn;
%%%%%%%%%%%%%%%%%%%
        D=zeros(K,3);
         for k=1:K
             D(k,:)=abs([I(i,j,1) I(i,j,2) I(i,j,3)]-M{i,j}(k,:));
              if (D(k,1)<=2.5*V(i,j,k))&(D(k,2)<=2.5*V(i,j,k))&(D(k,3)<=2.5*V(i,j,k))
               r=1;
             end
         end        
        W1=zeros(1,K);
        W2=zeros(1,K);
        W1(1,:)=W(i,j,1:K)./V(i,j,1:K);
        W2=sort(W1);%%%%从小到大排序
        e=1;
        %%%%%%%从大到小
       for q=K:-1:1
           INdex=find(W1==W2(1,q));
           index(e)=min(INdex);
           e=e+1;
       end
       Sum=W(i,j,index(1));
        B=2;
       while (Sum<T1)&(B<K)
           Sum=Sum+W(i,j,index(B));
           B=B+1;
       end
       B=B-1;%%%%背景的B个高斯分布
       A=zeros(B,1);
       for k=1:B   
              if ((D(index(k),1)>2.5*V(i,j,index(k)))|(D(index(k),2)>2.5*V(i,j,index(k)))|(D(index(k),3)>2.5*V(i,j,index(k))))
               A(k)=1;
           end
       end
       if sum(A)==B 
           Label(i,j)=1;
       end
  %%%%如果匹配   
  if r==1
      x=0;
      for m=1:K
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
    W(i,j,:)=W(i,j,:)/sum(W(i,j,:));%%%%权重归一化
  %%%%%如果不匹配
  else 
      if K==3
     Index=find(W(i,j,:)./V(i,j,:)==min(W(i,j,:)./V(i,j,:)));
     Index=max(Index);
            W(i,j,Index)=0.05;
            M{i,j}(Index,:)=[I(i,j,1) I(i,j,2) I(i,j,3)];
            V(i,j,Index)=5;
      end
      if K==2
          W(i,j,3)=0.05;
          V(i,j,3)=5;
          M{i,j}(3,:)=[I(i,j,1) I(i,j,2) I(i,j,3)];
      end
      if K==1
          W(i,j,2)=0.05;
          V(i,j,2)=5;
          M{i,j}(2,:)=[I(i,j,1) I(i,j,2) I(i,j,3)];
      end
      W(i,j,:)=W(i,j,:)/sum(W(i,j,:));%%%%权重归一化
  end 
    end
end
%%%%%%%%%%%%
   fname=strcat('E:\测试图片\新建文件夹\',num);
   imwrite(Label,fname);
   toc
end