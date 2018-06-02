clear all;
clc;
%%%%%%第二步骤：求出所有预处理后的图片的定序特征%%%%%%%%%%%
load ordifilter2L;
[M,N]=size(ordifilterall);
rate=zeros(M,N);
for k=1:M
    for h=1:N
%%%load OMNEW;%%%%获取一个水平方向二极分离偶极子（定序滤波）
imgset=dir('D:\我的文档\feat(2)\v2_iris\*.jpg');
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
     allcell{i,1}=makeom(img,ordifilterall{k,h});%存放虹膜集合定序特征图（大小和原图一样）
     allcell{i,2}=imgname;   %存放虹膜集合图片名称
     allcell{i,3}=masimg;    %存放掩盖图片
end
save allom3  allcell;
%%%%%%%%%%%%产生训练集合，测试集合%%%%%%%%%%%%%%%%%%%%%%%%%%
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
load ID2;
load set2;%第二个定序滤波核
galleryperson=length(galleryID);
testperson=length(testID);
ra=0;
mdistance=zeros(galleryperson,1);
for k=1:size(testID,1)
    testing=testfea(k);
    testing=testing{1,1};
    tmask=testmask(k);
    tmask=tmask{1,1};
    tmorder=(find(tmask==255));%测试图片被覆盖的区域
   for h=1:size(galleryID,1)
       gallery=galleryfea(h);
       gallery=gallery{1,1};
       gmask=gallerymask(h);
       gmask=gmask{1,1};
       gmorder=(find(gmask==255));%注册图片被覆盖的区域
       score=xor(testing,gallery);
       score(tmorder)=1;
       score(gmorder)=1;
        score=score(:);
        normval=length(score);
        mdistance(h,1)=sum(score)/normval;
   end
   [value,index]=min(mdistance);
   if galleryID(index)==testID(k)
       ra=ra+1;
   end
end
rate(k,h)=ra/testperson;
    end
end

     
     
     
     