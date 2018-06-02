clear all;
clc;
load ordifilter2L;
[M,N]=size(ordifilterall);
SW=[];
SB=[];
cnsw=0;cnsb=0;
%%%测试adaboost利用前十张图片%%%
for pp=1:36      %%%%结果保存在SBNEW SWNEW
    for qq=(pp+1):37
        sw=[];
        sb=[];
       for k=1:4:16  %%%滤波水平二核的选择
           for h=1:2
%%%load OMNEW;%%%%获取一个水平方向二极分离偶极子（定序滤波）
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
    imgname=imgname(2:8);   
     allcell{i,1}=makeom(img,ordifilterall{k,h});%存放虹膜集合定序特征图（大小和原图一样）
     allcell{i,2}=imgname;   %存放虹膜集合图片名称
     allcell{i,3}=masimg;    %存放掩盖图片
end
        score=xor(allcell{pp,1},allcell{qq,1});
        morder1=find(allcell{pp,3}==255);
        morder2=find(allcell{qq,3}==255);
        score(morder1)=1;
        score(morder2)=1;
        dis=makeblock(score);  %特征图分块
        dis=single(dis);
        if  strcmp(allcell{pp,2}(2:6),allcell{qq,2}(2:6))==1     %%类内距离
%            swdis=makeblock(score);
           sw=[sw dis'];
           cnsw=cnsw+1
           swname{cnsw,1}=strcat(allcell{pp,2},allcell{qq,2});
           %SW=[SW;sw];
        else
            sb=[sb dis'];
            cnsb=cnsb+1
            sbname{cnsb,1}=strcat(allcell{pp,2},allcell{qq,2});
%             if dis<305
%                 indexi=allcell{i,2}
%                 indexj=allcell{j,2}
%             end
           % SB=[SB;sb];
        end
    end
       end
SW=[SW;sw];
SB=[SB;sb];
    end
end
            
           
                   
                    
            
        
        
