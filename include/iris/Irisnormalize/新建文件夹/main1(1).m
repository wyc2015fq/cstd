%%
%%%%%主函数

%%把图像读入，作为一个矩阵
Classnum=6;
snum=20;

row=30;
line=360;

Date=zeros(row,line,snum,Classnum);
Mask=zeros(row,line,snum,Classnum);

%%class 1
Date(:,:,1,1)=imread('F:\matwork\irisdata\S2001L01P.bmp');
Mask(:,:,1,1)=imread('F:\matwork\irisdata\S2001L01M.bmp');

Date(:,:,2,1)=imread('F:\matwork\irisdata\S2001L02P.bmp');
Mask(:,:,2,1)=imread('F:\matwork\irisdata\S2001L02M.bmp');

Date(:,:,3,1)=imread('F:\matwork\irisdata\S2001L03P.bmp');
Mask(:,:,3,1)=imread('F:\matwork\irisdata\S2001L03M.bmp');
 
Date(:,:,4,1)=imread('F:\matwork\irisdata\S2001L04P.bmp');
Mask(:,:,4,1)=imread('F:\matwork\irisdata\S2001L04M.bmp');
 
Date(:,:,5,1)=imread('F:\matwork\irisdata\S2001L05P.bmp');
Mask(:,:,5,1)=imread('F:\matwork\irisdata\S2001L05M.bmp');
 
Date(:,:,6,1)=imread('F:\matwork\irisdata\S2001L06P.bmp');
Mask(:,:,6,1)=imread('F:\matwork\irisdata\S2001L06M.bmp');
 
Date(:,:,7,1)=imread('F:\matwork\irisdata\S2001L07P.bmp');
Mask(:,:,7,1)=imread('F:\matwork\irisdata\S2001L07M.bmp');
 
Date(:,:,8,1)=imread('F:\matwork\irisdata\S2001L08P.bmp');
Mask(:,:,8,1)=imread('F:\matwork\irisdata\S2001L08M.bmp');
 
Date(:,:,9,1)=imread('F:\matwork\irisdata\S2001L09P.bmp');
Mask(:,:,9,1)=imread('F:\matwork\irisdata\S2001L09M.bmp');
 
Date(:,:,10,1)=imread('F:\matwork\irisdata\S2001L10P.bmp');
Mask(:,:,10,1)=imread('F:\matwork\irisdata\S2001L10M.bmp');
 
Date(:,:,11,1)=imread('F:\matwork\irisdata\S2001L11P.bmp');
Mask(:,:,11,1)=imread('F:\matwork\irisdata\S2001L11M.bmp');
 
Date(:,:,12,1)=imread('F:\matwork\irisdata\S2001L12P.bmp');
Mask(:,:,12,1)=imread('F:\matwork\irisdata\S2001L12M.bmp');
 
Date(:,:,13,1)=imread('F:\matwork\irisdata\S2001L13P.bmp');
Mask(:,:,13,1)=imread('F:\matwork\irisdata\S2001L13M.bmp');
 
Date(:,:,14,1)=imread('F:\matwork\irisdata\S2001L14P.bmp');
Mask(:,:,14,1)=imread('F:\matwork\irisdata\S2001L14M.bmp');
 
Date(:,:,15,1)=imread('F:\matwork\irisdata\S2001L15P.bmp');
Mask(:,:,15,1)=imread('F:\matwork\irisdata\S2001L15M.bmp');
 
Date(:,:,16,1)=imread('F:\matwork\irisdata\S2001L16P.bmp');
Mask(:,:,16,1)=imread('F:\matwork\irisdata\S2001L16M.bmp');
 
Date(:,:,17,1)=imread('F:\matwork\irisdata\S2001L17P.bmp');
Mask(:,:,17,1)=imread('F:\matwork\irisdata\S2001L17M.bmp');
 
Date(:,:,18,1)=imread('F:\matwork\irisdata\S2001L18P.bmp');
Mask(:,:,18,1)=imread('F:\matwork\irisdata\S2001L18M.bmp');
 
Date(:,:,19,1)=imread('F:\matwork\irisdata\S2001L19P.bmp');
Mask(:,:,19,1)=imread('F:\matwork\irisdata\S2001L19M.bmp');
 
Date(:,:,20,1)=imread('F:\matwork\irisdata\S2001L20P.bmp');
Mask(:,:,20,1)=imread('F:\matwork\irisdata\S2001L20M.bmp');
 
%%class 2
Date(:,:,1,2)=imread('F:\matwork\irisdata\S2001R01P.bmp');
Mask(:,:,1,2)=imread('F:\matwork\irisdata\S2001R01M.bmp');
 
Date(:,:,2,2)=imread('F:\matwork\irisdata\S2001R02P.bmp');
Mask(:,:,2,2)=imread('F:\matwork\irisdata\S2001R02M.bmp');
 
Date(:,:,3,2)=imread('F:\matwork\irisdata\S2001R03P.bmp');
Mask(:,:,3,2)=imread('F:\matwork\irisdata\S2001R03M.bmp');
 
Date(:,:,4,2)=imread('F:\matwork\irisdata\S2001R04P.bmp');
Mask(:,:,4,2)=imread('F:\matwork\irisdata\S2001R04M.bmp');
 
Date(:,:,5,2)=imread('F:\matwork\irisdata\S2001R05P.bmp');
Mask(:,:,5,2)=imread('F:\matwork\irisdata\S2001R05M.bmp');
 
Date(:,:,6,2)=imread('F:\matwork\irisdata\S2001R06P.bmp');
Mask(:,:,6,2)=imread('F:\matwork\irisdata\S2001R06M.bmp');
 
Date(:,:,7,2)=imread('F:\matwork\irisdata\S2001R07P.bmp');
Mask(:,:,7,2)=imread('F:\matwork\irisdata\S2001R07M.bmp');
 
Date(:,:,8,2)=imread('F:\matwork\irisdata\S2001R08P.bmp');
Mask(:,:,8,2)=imread('F:\matwork\irisdata\S2001R08M.bmp');
 
Date(:,:,9,2)=imread('F:\matwork\irisdata\S2001R09P.bmp');
Mask(:,:,9,2)=imread('F:\matwork\irisdata\S2001R09M.bmp');
 
Date(:,:,10,2)=imread('F:\matwork\irisdata\S2001R10P.bmp');
Mask(:,:,10,2)=imread('F:\matwork\irisdata\S2001R10M.bmp');
 
Date(:,:,11,2)=imread('F:\matwork\irisdata\S2001R11P.bmp');
Mask(:,:,11,2)=imread('F:\matwork\irisdata\S2001R11M.bmp');
 
Date(:,:,12,2)=imread('F:\matwork\irisdata\S2001R12P.bmp');
Mask(:,:,12,2)=imread('F:\matwork\irisdata\S2001R12M.bmp');
 
Date(:,:,13,2)=imread('F:\matwork\irisdata\S2001R13P.bmp');
Mask(:,:,13,2)=imread('F:\matwork\irisdata\S2001R13M.bmp');
 
Date(:,:,14,2)=imread('F:\matwork\irisdata\S2001R14P.bmp');
Mask(:,:,14,2)=imread('F:\matwork\irisdata\S2001R14M.bmp');
 
Date(:,:,15,2)=imread('F:\matwork\irisdata\S2001R15P.bmp');
Mask(:,:,15,2)=imread('F:\matwork\irisdata\S2001R15M.bmp');
 
Date(:,:,16,2)=imread('F:\matwork\irisdata\S2001R16P.bmp');
Mask(:,:,16,2)=imread('F:\matwork\irisdata\S2001R16M.bmp');
 
Date(:,:,17,2)=imread('F:\matwork\irisdata\S2001R17P.bmp');
Mask(:,:,17,2)=imread('F:\matwork\irisdata\S2001R17M.bmp');
 
Date(:,:,18,2)=imread('F:\matwork\irisdata\S2001R18P.bmp');
Mask(:,:,18,2)=imread('F:\matwork\irisdata\S2001R18M.bmp');
 
Date(:,:,19,2)=imread('F:\matwork\irisdata\S2001R19P.bmp');
Mask(:,:,19,2)=imread('F:\matwork\irisdata\S2001R19M.bmp');
 
Date(:,:,20,2)=imread('F:\matwork\irisdata\S2001R20P.bmp');
Mask(:,:,20,2)=imread('F:\matwork\irisdata\S2001R20M.bmp');
 
%%class 3
Date(:,:,1,3)=imread('F:\matwork\irisdata\S2002L01P.bmp');
Mask(:,:,1,3)=imread('F:\matwork\irisdata\S2002L01M.bmp');
 
Date(:,:,2,3)=imread('F:\matwork\irisdata\S2002L02P.bmp');
Mask(:,:,2,3)=imread('F:\matwork\irisdata\S2002L02M.bmp');
 
Date(:,:,3,3)=imread('F:\matwork\irisdata\S2002L03P.bmp');
Mask(:,:,3,3)=imread('F:\matwork\irisdata\S2002L03M.bmp');
 
Date(:,:,4,3)=imread('F:\matwork\irisdata\S2002L04P.bmp');
Mask(:,:,4,3)=imread('F:\matwork\irisdata\S2002L04M.bmp');
 
Date(:,:,5,3)=imread('F:\matwork\irisdata\S2002L05P.bmp');
Mask(:,:,5,3)=imread('F:\matwork\irisdata\S2002L05M.bmp');
 
Date(:,:,6,3)=imread('F:\matwork\irisdata\S2002L06P.bmp');
Mask(:,:,6,3)=imread('F:\matwork\irisdata\S2002L06M.bmp');
 
Date(:,:,7,3)=imread('F:\matwork\irisdata\S2002L07P.bmp');
Mask(:,:,7,3)=imread('F:\matwork\irisdata\S2002L07M.bmp');
 
Date(:,:,8,3)=imread('F:\matwork\irisdata\S2002L08P.bmp');
Mask(:,:,8,3)=imread('F:\matwork\irisdata\S2002L08M.bmp');
 
Date(:,:,9,3)=imread('F:\matwork\irisdata\S2002L09P.bmp');
Mask(:,:,9,3)=imread('F:\matwork\irisdata\S2002L09M.bmp');
 
Date(:,:,10,3)=imread('F:\matwork\irisdata\S2002L10P.bmp');
Mask(:,:,10,3)=imread('F:\matwork\irisdata\S2002L10M.bmp');
 
Date(:,:,11,3)=imread('F:\matwork\irisdata\S2002L11P.bmp');
Mask(:,:,11,3)=imread('F:\matwork\irisdata\S2002L11M.bmp');
 
Date(:,:,12,3)=imread('F:\matwork\irisdata\S2002L12P.bmp');
Mask(:,:,12,3)=imread('F:\matwork\irisdata\S2002L12M.bmp');
 
Date(:,:,13,3)=imread('F:\matwork\irisdata\S2002L13P.bmp');
Mask(:,:,13,3)=imread('F:\matwork\irisdata\S2002L13M.bmp');
 
Date(:,:,14,3)=imread('F:\matwork\irisdata\S2002L14P.bmp');
Mask(:,:,14,3)=imread('F:\matwork\irisdata\S2002L14M.bmp');
 
Date(:,:,15,3)=imread('F:\matwork\irisdata\S2002L15P.bmp');
Mask(:,:,15,3)=imread('F:\matwork\irisdata\S2002L15M.bmp');
 
Date(:,:,16,3)=imread('F:\matwork\irisdata\S2002L16P.bmp');
Mask(:,:,16,3)=imread('F:\matwork\irisdata\S2002L16M.bmp');
 
Date(:,:,17,3)=imread('F:\matwork\irisdata\S2002L17P.bmp');
Mask(:,:,17,3)=imread('F:\matwork\irisdata\S2002L17M.bmp');
 
Date(:,:,18,3)=imread('F:\matwork\irisdata\S2002L18P.bmp');
Mask(:,:,18,3)=imread('F:\matwork\irisdata\S2002L18M.bmp');
 
Date(:,:,19,3)=imread('F:\matwork\irisdata\S2002L19P.bmp');
Mask(:,:,19,3)=imread('F:\matwork\irisdata\S2002L19M.bmp');
 
Date(:,:,20,3)=imread('F:\matwork\irisdata\S2002L20P.bmp');
Mask(:,:,20,3)=imread('F:\matwork\irisdata\S2002L20M.bmp');
 
 
 
%%%class 4
Date(:,:,1,4)=imread('F:\matwork\irisdata\S2002R01P.bmp');
Mask(:,:,1,4)=imread('F:\matwork\irisdata\S2002R01M.bmp');
 
Date(:,:,2,4)=imread('F:\matwork\irisdata\S2002R02P.bmp');
Mask(:,:,2,4)=imread('F:\matwork\irisdata\S2002R02M.bmp');
 
Date(:,:,3,4)=imread('F:\matwork\irisdata\S2002R03P.bmp');
Mask(:,:,3,4)=imread('F:\matwork\irisdata\S2002R03M.bmp');
 
Date(:,:,4,4)=imread('F:\matwork\irisdata\S2002R04P.bmp');
Mask(:,:,4,4)=imread('F:\matwork\irisdata\S2002R04M.bmp');
 
Date(:,:,5,4)=imread('F:\matwork\irisdata\S2002R05P.bmp');
Mask(:,:,5,4)=imread('F:\matwork\irisdata\S2002R05M.bmp');
 
Date(:,:,6,4)=imread('F:\matwork\irisdata\S2002R06P.bmp');
Mask(:,:,6,4)=imread('F:\matwork\irisdata\S2002R06M.bmp');
 
Date(:,:,7,4)=imread('F:\matwork\irisdata\S2002R07P.bmp');
Mask(:,:,7,4)=imread('F:\matwork\irisdata\S2002R07M.bmp');
 
Date(:,:,8,4)=imread('F:\matwork\irisdata\S2002R08P.bmp');
Mask(:,:,8,4)=imread('F:\matwork\irisdata\S2002R08M.bmp');
 
Date(:,:,9,4)=imread('F:\matwork\irisdata\S2002R09P.bmp');
Mask(:,:,9,4)=imread('F:\matwork\irisdata\S2002R09M.bmp');
 
Date(:,:,10,4)=imread('F:\matwork\irisdata\S2002R10P.bmp');
Mask(:,:,10,4)=imread('F:\matwork\irisdata\S2002R10M.bmp');
 
Date(:,:,11,4)=imread('F:\matwork\irisdata\S2002R11P.bmp');
Mask(:,:,11,4)=imread('F:\matwork\irisdata\S2002R11M.bmp');
 
Date(:,:,12,4)=imread('F:\matwork\irisdata\S2002R12P.bmp');
Mask(:,:,12,4)=imread('F:\matwork\irisdata\S2002R12M.bmp');
 
Date(:,:,13,4)=imread('F:\matwork\irisdata\S2002R13P.bmp');
Mask(:,:,13,4)=imread('F:\matwork\irisdata\S2002R13M.bmp');
 
Date(:,:,14,4)=imread('F:\matwork\irisdata\S2002R14P.bmp');
Mask(:,:,14,4)=imread('F:\matwork\irisdata\S2002R14M.bmp');
 
Date(:,:,15,4)=imread('F:\matwork\irisdata\S2002R15P.bmp');
Mask(:,:,15,4)=imread('F:\matwork\irisdata\S2002R15M.bmp');
 
Date(:,:,16,4)=imread('F:\matwork\irisdata\S2002R16P.bmp');
Mask(:,:,16,4)=imread('F:\matwork\irisdata\S2002R16M.bmp');
 
Date(:,:,17,4)=imread('F:\matwork\irisdata\S2002R17P.bmp');
Mask(:,:,17,4)=imread('F:\matwork\irisdata\S2002R17M.bmp');
 
Date(:,:,18,4)=imread('F:\matwork\irisdata\S2002R18P.bmp');
Mask(:,:,18,4)=imread('F:\matwork\irisdata\S2002R18M.bmp');
 
Date(:,:,19,4)=imread('F:\matwork\irisdata\S2002R19P.bmp');
Mask(:,:,19,4)=imread('F:\matwork\irisdata\S2002R19M.bmp');
 
Date(:,:,20,4)=imread('F:\matwork\irisdata\S2002R20P.bmp');
Mask(:,:,20,4)=imread('F:\matwork\irisdata\S2002R20M.bmp');
 
 
 
%%class 5
Date(:,:,1,5)=imread('F:\matwork\irisdata\S2003L01P.bmp');
Mask(:,:,1,5)=imread('F:\matwork\irisdata\S2003L01M.bmp');
 
Date(:,:,2,5)=imread('F:\matwork\irisdata\S2003L02P.bmp');
Mask(:,:,2,5)=imread('F:\matwork\irisdata\S2003L02M.bmp');
 
Date(:,:,3,5)=imread('F:\matwork\irisdata\S2003L03P.bmp');
Mask(:,:,3,5)=imread('F:\matwork\irisdata\S2003L03M.bmp');
 
Date(:,:,4,5)=imread('F:\matwork\irisdata\S2003L04P.bmp');
Mask(:,:,4,5)=imread('F:\matwork\irisdata\S2003L04M.bmp');
 
Date(:,:,5,5)=imread('F:\matwork\irisdata\S2003L05P.bmp');
Mask(:,:,5,5)=imread('F:\matwork\irisdata\S2003L05M.bmp');
 
Date(:,:,6,5)=imread('F:\matwork\irisdata\S2003L06P.bmp');
Mask(:,:,6,5)=imread('F:\matwork\irisdata\S2003L06M.bmp');
 
Date(:,:,7,5)=imread('F:\matwork\irisdata\S2003L07P.bmp');
Mask(:,:,7,5)=imread('F:\matwork\irisdata\S2003L07M.bmp');
 
Date(:,:,8,5)=imread('F:\matwork\irisdata\S2003L08P.bmp');
Mask(:,:,8,5)=imread('F:\matwork\irisdata\S2003L08M.bmp');
 
Date(:,:,9,5)=imread('F:\matwork\irisdata\S2003L09P.bmp');
Mask(:,:,9,5)=imread('F:\matwork\irisdata\S2003L09M.bmp');
 
Date(:,:,10,5)=imread('F:\matwork\irisdata\S2003L10P.bmp');
Mask(:,:,10,5)=imread('F:\matwork\irisdata\S2003L10M.bmp');
 
Date(:,:,11,5)=imread('F:\matwork\irisdata\S2003L11P.bmp');
Mask(:,:,11,5)=imread('F:\matwork\irisdata\S2003L11M.bmp');
 
Date(:,:,12,5)=imread('F:\matwork\irisdata\S2003L12P.bmp');
Mask(:,:,12,5)=imread('F:\matwork\irisdata\S2003L12M.bmp');
 
Date(:,:,13,5)=imread('F:\matwork\irisdata\S2003L13P.bmp');
Mask(:,:,13,5)=imread('F:\matwork\irisdata\S2003L13M.bmp');
 
Date(:,:,14,5)=imread('F:\matwork\irisdata\S2003L14P.bmp');
Mask(:,:,14,5)=imread('F:\matwork\irisdata\S2003L14M.bmp');
 
Date(:,:,15,5)=imread('F:\matwork\irisdata\S2003L15P.bmp');
Mask(:,:,15,5)=imread('F:\matwork\irisdata\S2003L15M.bmp');
 
Date(:,:,16,5)=imread('F:\matwork\irisdata\S2003L16P.bmp');
Mask(:,:,16,5)=imread('F:\matwork\irisdata\S2003L16M.bmp');
 
Date(:,:,17,5)=imread('F:\matwork\irisdata\S2003L17P.bmp');
Mask(:,:,17,5)=imread('F:\matwork\irisdata\S2003L17M.bmp');
 
Date(:,:,18,5)=imread('F:\matwork\irisdata\S2003L18P.bmp');
Mask(:,:,18,5)=imread('F:\matwork\irisdata\S2003L18M.bmp');
 
Date(:,:,19,5)=imread('F:\matwork\irisdata\S2003L19P.bmp');
Mask(:,:,19,5)=imread('F:\matwork\irisdata\S2003L19M.bmp');
 
Date(:,:,20,5)=imread('F:\matwork\irisdata\S2003L20P.bmp');
Mask(:,:,20,5)=imread('F:\matwork\irisdata\S2003L20M.bmp');
 
 
 
%%class 6
Date(:,:,1,6)=imread('F:\matwork\irisdata\S2003R01P.bmp');
Mask(:,:,1,6)=imread('F:\matwork\irisdata\S2003R01M.bmp');
 
Date(:,:,2,6)=imread('F:\matwork\irisdata\S2003R02P.bmp');
Mask(:,:,2,6)=imread('F:\matwork\irisdata\S2003R02M.bmp');
 
Date(:,:,3,6)=imread('F:\matwork\irisdata\S2003R03P.bmp');
Mask(:,:,3,6)=imread('F:\matwork\irisdata\S2003R03M.bmp');
 
Date(:,:,4,6)=imread('F:\matwork\irisdata\S2003R04P.bmp');
Mask(:,:,4,6)=imread('F:\matwork\irisdata\S2003R04M.bmp');
 
Date(:,:,5,6)=imread('F:\matwork\irisdata\S2003R05P.bmp');
Mask(:,:,5,6)=imread('F:\matwork\irisdata\S2003R05M.bmp');
 
Date(:,:,6,6)=imread('F:\matwork\irisdata\S2003R06P.bmp');
Mask(:,:,6,6)=imread('F:\matwork\irisdata\S2003R06M.bmp');
 
Date(:,:,7,6)=imread('F:\matwork\irisdata\S2003R07P.bmp');
Mask(:,:,7,6)=imread('F:\matwork\irisdata\S2003R07M.bmp');
 
Date(:,:,8,6)=imread('F:\matwork\irisdata\S2003R08P.bmp');
Mask(:,:,8,6)=imread('F:\matwork\irisdata\S2003R08M.bmp');
 
Date(:,:,9,6)=imread('F:\matwork\irisdata\S2003R09P.bmp');
Mask(:,:,9,6)=imread('F:\matwork\irisdata\S2003R09M.bmp');
 
Date(:,:,10,6)=imread('F:\matwork\irisdata\S2003R10P.bmp');
Mask(:,:,10,6)=imread('F:\matwork\irisdata\S2003R10M.bmp');
 
Date(:,:,11,6)=imread('F:\matwork\irisdata\S2003R11P.bmp');
Mask(:,:,11,6)=imread('F:\matwork\irisdata\S2003R11M.bmp');
 
Date(:,:,12,6)=imread('F:\matwork\irisdata\S2003R12P.bmp');
Mask(:,:,12,6)=imread('F:\matwork\irisdata\S2003R12M.bmp');
 
Date(:,:,13,6)=imread('F:\matwork\irisdata\S2003R13P.bmp');
Mask(:,:,13,6)=imread('F:\matwork\irisdata\S2003R13M.bmp');
 
Date(:,:,14,6)=imread('F:\matwork\irisdata\S2003R14P.bmp');
Mask(:,:,14,6)=imread('F:\matwork\irisdata\S2003R14M.bmp');
 
Date(:,:,15,6)=imread('F:\matwork\irisdata\S2003R15P.bmp');
Mask(:,:,15,6)=imread('F:\matwork\irisdata\S2003R15M.bmp');
 
Date(:,:,16,6)=imread('F:\matwork\irisdata\S2003R16P.bmp');
Mask(:,:,16,6)=imread('F:\matwork\irisdata\S2003R16M.bmp');
 
Date(:,:,17,6)=imread('F:\matwork\irisdata\S2003R17P.bmp');
Mask(:,:,17,6)=imread('F:\matwork\irisdata\S2003R17M.bmp');
 
Date(:,:,18,6)=imread('F:\matwork\irisdata\S2003R18P.bmp');
Mask(:,:,18,6)=imread('F:\matwork\irisdata\S2003R18M.bmp');
 
Date(:,:,19,6)=imread('F:\matwork\irisdata\S2003R19P.bmp');
Mask(:,:,19,6)=imread('F:\matwork\irisdata\S2003R19M.bmp');
 
Date(:,:,20,6)=imread('F:\matwork\irisdata\S2003R20P.bmp');
Mask(:,:,20,6)=imread('F:\matwork\irisdata\S2003R20M.bmp');
 



%%%%%%划分成重叠的子区域，然后定序编码
binum=300;
anum=16;
bnum=64;
fnum=12;
Code = zeros(anum,bnum*binum*fnum,snum,Classnum);
newMask = zeros(anum,bnum*binum,snum,Classnum);
tic
for c=1:Classnum
    for i=1:snum
        [Code(:,:,i,c) newMask(:,:,i,c)]=OMcode1(Date(:,:,i,c),Mask(:,:,i,c));
    end
end   
toc

%%%%%%%%%%%%%%%%%
%%%%同类的汉明距离
%%%%%%%%%%%%%%%%%
dnum1=snum*(snum-1)/2;
Dist1=zeros(binum*fnum,dnum1,Classnum);

tic

for c=1:Classnum
    k=0;
    for i=1:(snum-1)
        for j=(i+1):snum
            k=k+1;
            Dist1(:,k,c)=mixHD1(Code(:,:,i,c),newMask(:,:,i,c),Code(:,:,j,c),newMask(:,:,j,c));
        end
    end
end
           
toc

%%%%%%%%%%%%%%%%%
%%%%%%%%不同类的汉明距离
zhuhe=Classnum*(Classnum-1)/2;
Dist2=zeros(binum*fnum,snum,snum,zhuhe);

tic
k=0;
for c1=1:(Classnum-1)
    for c2=(c1+1):Classnum
        k=k+1;
        for i=1:snum
            for j=1:snum
                Dist2(:,i,j,k)=mixHD1(Code(:,:,i,c1),newMask(:,:,i,c1),Code(:,:,j,c2),newMask(:,:,j,c2));
            end
        end
    end
end
toc
















