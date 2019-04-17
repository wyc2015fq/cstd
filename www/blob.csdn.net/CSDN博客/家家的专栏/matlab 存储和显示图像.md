# matlab 存储和显示图像 - 家家的专栏 - CSDN博客





2012年10月13日 10:34:21[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：703








for example: 



for i=1:100

img = reshape(ImgData(:,i),64,64);  

imwrite(img,[num2str(i),'.jpg']); %存储图像

imshow(img);   %显示图像

end



