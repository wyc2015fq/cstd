clear all;
clc;
%%%%建立测试集合和注册集合%%%%%%%%
load allom3;
numimg=length(allcell); %总的图片张数
nameall=cell(numimg,1);
numall=[];   %整个集合中每幅图片的编号
for i=1:numimg
    nameall{i,1}=allcell{i,2};
    nameall{i,1}=strrep(nameall{i,1},'L','0');
    nameall{i,1}=strrep(nameall{i,1},'R','1');
    numall=[numall;str2double(nameall{i,1})];
    temp=allcell{i,1};
end
classnum=unique(numall); %求出集合中图片的类别号（共552个类别）
perimg=[];    %得出每个类别图片的张数
for i=1:length(classnum)
    temp=length(find(numall==classnum(i)));
    perimg=[perimg;temp];
end
%%%%定义每个类别中选取到注册集中的图片张数
cnt=2;%每个类别选两张*******************************************   
cnting=1:cnt;
gallerynum=zeros(length(perimg),cnt);
gallerynum(1,:)=cnting;
for i=1:(length(perimg)-1)
       gallerynum(i+1,:)=gallerynum(i,:)+perimg(i);
end
gallerynum=gallerynum';
gallerynum=gallerynum(:); %%需要选出的注册集合中的行号
testnum=1:length(numall);
testnum(gallerynum)=[];
testnum=testnum';       %%%选出的测试集行号
save select2 gallerynum testnum %%%保存的测试集和注册集选出的行号
galleryID=numall(gallerynum);
testID=numall(testnum);
save ID2 galleryID testID    %%%%保存的测试集和注册集选出的ID号
galleryfea=allcell(gallerynum,1);
testfea=allcell(testnum,1);
gallerymask=allcell(gallerynum,3);
testmask=allcell(testnum,3);
save set2 galleryfea testfea gallerymask testmask; %%%%保存测试集和注册集
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%






    