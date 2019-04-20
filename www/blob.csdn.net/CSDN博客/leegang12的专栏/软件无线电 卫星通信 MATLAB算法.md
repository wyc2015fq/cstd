# 软件无线电 卫星通信  MATLAB算法 - leegang12的专栏 - CSDN博客
2013年03月07日 21:42:49[leegang12](https://me.csdn.net/leegang12)阅读数：1564
1、  matlab算法：  匹配滤波
rolloff=0.6;
filter=rcosine(1,16,'fir/sqrt',rolloff);
signallg=filter(filter,1,IQ);
2、 matlab算法：载波频偏算法，二次方四次方
fft = 1024;  
datalg=(I+1j.*Q).^4;
figure;  
plot(1:1024,abs(fft(datalg,fft))); 
title('FFT之后星座图')
grid on ;
3、matlab算法： Viterbi译码算法，码率1/2 ,1/3 ,1/4 , 2/3 ，硬译码，软译码，多电平译码。
   vitt=poly2trellis(5,[121 161]);         % 1/2
   vitt=poly2trellis(7,[133 171 181]);  % 1/3
   shendu=35;
    vittc = vitdec(lg,vitt,shendu,'cont','hard'); %
译码
