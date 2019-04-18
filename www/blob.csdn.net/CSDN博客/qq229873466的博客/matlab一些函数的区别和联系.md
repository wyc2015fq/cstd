# matlab一些函数的区别和联系 - qq229873466的博客 - CSDN博客

2016年06月24日 19:40:29[qq229873466](https://me.csdn.net/qq229873466)阅读数：268


线性卷积: conv(x,h); 

截断的线性卷积: filter(h,1,x);

圆周卷积: cconv(x,h,N)  = ifft(fft(x,N).*fft(h,N));

bode(tf(b,a),t); 

等价于 

[y w] = freqs(b,a,L);

semilogx(w,20*log10(abs(y)));

semilogx(w,rad2deg(angle(y)));

[yi,ti]=impulse(tf(b,a),t);

freqz(yi,1,L/2);

等价于 

Y = fft(yi);

plot(linspace(0,1,L/2+1),20*log10(abs(Y(1:L/2+1))));

plot(linspace(0,1,L/2+1),rad2deg(angle(Y(1:L/2+1))));


