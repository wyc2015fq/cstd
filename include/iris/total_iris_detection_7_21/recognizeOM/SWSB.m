clear all;
clc;
%%%%%%第一步骤：先将feat与mask中两张图片进行与处理,并写入新的文件夹中%%%%%%%%
%原图像路径%%%%
% orimgset=dir('D:\我的文档\feat(2)\V2_normalize\*.jpg');
% numberset=length(orimgset);
% %覆盖图像路径
% covimgset=dir('D:\我的文档\feat(2)\V2_mask\*.jpg');
% % savefile='D:\我的文档\feat(2)\V2_iris';
% for i=1:numberset
%     orifile=strcat('D:\我的文档\feat(2)\V2_normalize\',orimgset(i).name);
%     covfile=strcat('D:\我的文档\feat(2)\V2_mask\',covimgset(i).name);
%     orimg=imread(orifile);
%     coimg=imread(covfile);
%     fimg=makeom(orimg,coimg);
%     imname=orimgset(i).name;
%    savefile=strcat('D:\我的文档\feat(2)\V2_iris\',num2str(imname));
%    imwrite(fimg,savefile);
% end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%第二步骤：求出所有预处理后的图片的定序特征%%%%%%%%%%%
load OM3;%%%%获取一个水平方向二极分离偶极子（定序滤波）
imgset=dir('D:\我的文档\feat(2)\V2_iris\*.jpg');
maskset=dir('D:\我的文档\feat(2)\V2_mask\*.jpg');
numberset=length(imgset);
allcell=cell(numberset,3);
for i=1:numberset
     imgfile=strcat('D:\我的文档\feat(2)\V2_iris\',imgset(i).name);
     maskfile=strcat('D:\我的文档\feat(2)\V2_mask\',maskset(i).name);
     masimg=imread(maskfile);
     masimg=255-masimg;
     img=imread(imgfile);
     imgname=imgset(i).name;
    imgname=imgname(2:6);   
     allcell{i,1}=makeom(img,ordifilter);%存放虹膜集合定序特征图（大小和原图一样）
     allcell{i,2}=imgname;   %存放虹膜集合图片名称
     allcell{i,3}=masimg;    %存放掩盖图片
end
save allom3  allcell;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%第三步骤：求类内和类间对汉明距离%%%%%%%%%%%%%
% load allom2;
% numimg=length(allcell);
% sw=[];
% sb=[];
% %minsb=1000;
% for i=1:(numimg-1)
%     for j=(i+1):numimg
% %           score=xor(allcell{i,1},allcell{j,1});
% %           morder1=find(allcell{i,3}==255);
% %           morder2=find(allcell{j,3}==255);
% %           score=score(:);
% %           score(morder1)=1;
% %           score(morder2)=1;
% %           dis=sum(score)/length(score);
% %           dis=single(dis);
% %           clear score morder1 morder2 ;
%       if  strcmp(allcell{i,2},allcell{j,2})==1 
%           score=xor(allcell{i,1},allcell{j,1});
%           morder1=find(allcell{i,3}==255);
%           morder2=find(allcell{j,3}==255);
%           score=score(:);
%           score(morder1)=1;
%           score(morder2)=1;
%           dis=sum(score)/length(score);
%           dis=single(dis);
%           clear score morder1 morder2 ;
%           sw=[sw;dis];
%       else
%           score=xor(allcell{i,1},allcell{j,1});
%           morder1=find(allcell{i,3}==255);
%           morder2=find(allcell{j,3}==255);
%           score=score(:);
%           score(morder1)=1;
%           score(morder2)=1;
%           dis=sum(score)/length(score);
%           dis=single(dis);
%           if dis<0.3945
%              % minsb=dis;
% %               indexi=allcell{i,2};
% %               indexj=allcell{j,2};
%                  indexi=i;
%                  indexj=j;
%           end         
%          sb=[sb;dis];
%       end         
%     end
% end
% % % save sw sw;
% % save sb sb;
% % save index indexi indexj;

     
     
     
     