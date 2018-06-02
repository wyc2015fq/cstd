%%%%%%虹膜定序特征识别%%%%%%%%%
clear all;
clc;
% load  set;%第一个定序滤波核
% tfeature=testfea;
% gfeature=galleryfea;
load ID2;
load set2;%第二个定序滤波核
galleryperson=length(galleryID);
testperson=length(testID);
ra=0;
mdistance=zeros(galleryperson,1);
for k=1:size(testID,1)
    testing=testfea(k);
    testing=testing{1,1};
%     tmask=testmask(k);
%     tmask=tmask{1,1};
%     tmorder=(find(tmask==255));%测试图片被覆盖的区域
   for h=1:size(galleryID,1)
       gallery=galleryfea(h);
       gallery=gallery{1,1};
%        gmask=gallerymask(h);
%        gmask=gmask{1,1};
%        gmorder=(find(gmask==255));%注册图片被覆盖的区域
       score=xor(testing,gallery);
%       score(tmorder)=1;
%       score(gmorder)=1;
        score=score(:);
%         normval=length(score);
%         mdistance(h,1)=sum(score)/normval;
         mdistance(h,1)=sum(score);
   end
   [value,index]=min(mdistance);
   if galleryID(index)==testID(k)
       ra=ra+1;
   end
end
rate=ra/testperson
    
