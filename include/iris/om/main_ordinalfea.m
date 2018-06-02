%%按块提取特征进行识别%%%%%%%%
clear all;
clc;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% load ordifilter2L;
% for k=1:4:16 %%k=13 % %%%滤波核的选择
%       for h=1:2 %%h=2 %
%  %%%load OMNEW;%%%%获取一个水平方向二极分离偶极子（定序滤波）
% imgset=dir('D:\我的文档\feat(2)\V2_iris\*.jpg');
% maskset=dir('D:\我的文档\feat(2)\V2_mask\*.jpg');
% numberset=length(imgset);
% allcell=cell(numberset,3);
% for i=1:numberset
%      imgfile=strcat('D:\我的文档\feat(2)\V2_iris\',imgset(i).name);
%      maskfile=strcat('D:\我的文档\feat(2)\V2_mask\',maskset(i).name);
%      masimg=imread(maskfile);
%      masimg=255-masimg;
%      img=imread(imgfile);
%      imgname=imgset(i).name;
%     imgname=imgname(2:6);   
%      allcell{i,1}=single(makeom(img,ordifilterall{k,h}));%存放虹膜集合定序特征图（大小和原图一样）
%      allcell{i,2}=(imgname);   %存放虹膜集合图片名称
%      allcell{i,3}=single(masimg);    %存放掩盖图片
% end
% %%%%%%%%%%%%产生训练集合，测试集合%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% numimg=length(allcell); %总的图片张数
% nameall=cell(numimg,1);
% numall=[];   %整个集合中每幅图片的编号
% for i=1:numimg
%     nameall{i,1}=allcell{i,2};
%     nameall{i,1}=strrep(nameall{i,1},'L','0');
%     nameall{i,1}=strrep(nameall{i,1},'R','1');
%     numall=[numall;str2double(nameall{i,1})];
%     temp=allcell{i,1};
% end
% classnum=unique(numall); %求出集合中图片的类别号（共552个类别）
% perimg=[];    %得出每个类别图片的张数
% for i=1:length(classnum)
%     temp=length(find(numall==classnum(i)));
%     perimg=[perimg;temp];
% end
% %%%%定义每个类别中选取到注册集中的图片张数
% cnt=2;%每个类别选两张*******************************************   
% cnting=1:cnt;
% gallerynum=zeros(length(perimg),cnt);
% gallerynum(1,:)=cnting;
% for i=1:(length(perimg)-1)
%        gallerynum(i+1,:)=gallerynum(i,:)+perimg(i);
% end
% gallerynum=gallerynum';
% gallerynum=gallerynum(:); %%需要选出的注册集合中的行号
% testnum=1:length(numall);
% testnum(gallerynum)=[];
% testnum=testnum';       %%%选出的测试集行号
% % %%save select2 gallerynum testnum %%%保存的测试集和注册集选出的行号
% galleryID=numall(gallerynum);
% testID=numall(testnum);
% save ID2 galleryID testID    %%%%保存的测试集和注册集选出的ID号
% galleryfea=allcell(gallerynum,1);
% galleryall{(ceil(k/4)-1)*2+h,1}=galleryfea;
% testfea=allcell(testnum,1);
% testall{(ceil(k/4)-1)*2+h,1}=testfea;
% gallerymask=allcell(gallerynum,3);
% testmask=allcell(testnum,3);
% clear allcell galleryfea testfea;
%            end
%   end
% %save set2 galleryfea testfea gallerymask testmask; %%%%保存测试集和注册集
%save gallerymask testmask;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 load ID2;
 load testall;
 load galleryall;
 load gmask;
 load tmask;
galleryperson=length(galleryID);
testperson=length(testID);
ra=0;
for tk=1:size(testID,1) 
    disall=[];
    for i=1:8
   dis=zeros(galleryperson,259);
    testsub=test{i,1}(tk);
    testing=testsub{1,1};
    tmask=testmask(tk);
    tmask=tmask{1,1};
    tmorder=(find(tmask==255));%测试图片被覆盖的区域
   for th=1:size(galleryID,1)
       gallerysub=gallery{i,1}(th);
       gallerying=gallerysub{1,1};
       gmask=gallerymask(th);
       gmask=gmask{1,1};
       gmorder=(find(gmask==255));%注册图片被覆盖的区域
       score=xor(testing,gallerying);
       score(tmorder)=1;
       score(gmorder)=1;
       dis(th,:)=single(makeblock(score));  %特征图分块
   end
   disall=[disall dis];
    end
    [value,index]=min(disall);
    if galleryID(index)==testID(tk)
        ra=ra+1;
    end    
end
rating=ra/testperson

  
