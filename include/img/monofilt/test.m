clc
clear all
img=imread('test.bmp');
minWaveLength       =  4;
sigmaOnf            =  0.64;
mult                =  1.7;
region_num          =  8;
nscale              =  3;
orientWrap          =  0;
bh_n                =  4;
bw_n                =  5;
sh_n                =  1;
sw_n                =  1;
neigh               =  8;
radius              =  3;
NumTarget           =  10;
  [f1, h1f1, h2f1, A1,theta1, psi1] = monofilt(Tem_img, ...
            nscale, minWaveLength, mult, sigmaOnf, orientWrap);
       for v=1:nscale
        Tem_img=uint8((A1{v}-min(A1{v}(:)))./(max(A1{v}(:))-min(A1{v}(:))).*255);
      %LBPHIST=lbp_mbp(Tem_img,radius,neigh,MAPPING,'i');
       LBPHIST1=LBP3(Tem_img,mapping3);
   %    [LBPHIST]=LBP9_new(Tem_img,mapping9);
        matrix2=zeros(size(h1f1{v}));matrix3=zeros(size(h2f1{v}));
        matrix2(h1f1{v}>0)=0;matrix2(h1f1{v}<=0)=1;
   %   matrix2=matrix2(radius+1:end-radius,radius+1:end-radius);
        matrix3(h2f1{v}>0)=0;matrix3(h2f1{v}<=0)=1;
 %  matrix3=matrix3(radius+1:end-radius,radius+1:end-radius);
%         N_LBPHIST=matrix2*512+matrix3*256+double(LBPHIST);%for mapping = 0;
        N_LBPHIST=matrix2*128+matrix3*64+double(LBPHIST);% for uniform lbp;
        N_LBPHIST=uint16(N_LBPHIST);
        HIST(v).im = N_LBPHIST;
    end