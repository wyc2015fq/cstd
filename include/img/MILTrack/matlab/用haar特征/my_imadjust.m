function OutImage = my_imadjust(InImage)
%函数功能：增加灰度图像的对比度，等效于matlab自带函数imadjust,即等效于OutImage = imadjust(InImage);
%InImage为输入的灰度图像
%OutImage为增加对比度后的灰度图像

%以下代码和MATLAB自带函数stretchlim等同，即等效于lowhigh_in = stretchlim(InImage);%
nbins = 256;
tol = [.01 .99];
tol_low = tol(1);
tol_high = tol(2);
N = imhist(InImage,nbins);%求256个bin的直方图
cdf = cumsum(N)/sum(N);%cumsum(N)为求N中元素的累加和
low = find(cdf>tol_low, 1, 'first');%此句意思是返回cdf中的第一个大于tol_low的元素索引
high = find(cdf>=tol_high, 1, 'first');
if low==high
    lowhigh(:,1) = [1; nbins];
else
    lowhigh(:,1) = [low;high];
end
lowhigh_in = (lowhigh - 1)/(nbins-1);  % convert to range [0 1]
%上代码和MATLAB自带函数lowhigh_in = stretchlim(InImage);等同%


low_in   = lowhigh_in(1);
high_in  = lowhigh_in(2);
low_out  = 0;
high_out = 1;

temp1 = linspace(0,1,256);
temp1(:) = max(low_in,min(high_in,temp1));

temp2 = ((temp1-low_in)./(high_in-low_in));
temp2(:) = temp2.*(high_out-low_out) + low_out;

lut = im2uint8(temp2);%查找表所用到的表，即lut = uint8(round(temp2.*255));
OutImage = zeros(size(InImage,1),size(InImage,2));
OutImage= uintlut(InImage,lut);%如InImage中第100个位置的的元素值为a,则OutImage相同位置的元素值为lut(a+1)

