# Matlab梯度法图像锐化 - 家家的专栏 - CSDN博客





2011年07月12日 09:38:52[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：9067








 [I,map]=imread('img1.bmp');   %读入图像
imshow(I,map);                  %显示原图像
I=double(I);                      %转换为double类型
[Gx,Gy]=gradient(I);                 % 计算梯度
G=sqrt(Gx.*Gx+Gy.*Gy);           % 水平垂直差分
J=I; 
K=find(G>=7);                   %指定灰度级
J(K)=255;
figure,imshow(J,map);              % 显示处理后的图像



转自：[http://zhidao.baidu.com/question/160216024.html](http://zhidao.baidu.com/question/160216024.html)




|`01`|`function ruihua_Callback(hObject, eventdata, handles) `|
|----|----|
|`02`|`% hObject    handle to Untitled_1 (see GCBO) `|
|----|----|
|`03`|`% eventdata  reserved - to be defined in a future version of MATLAB `|
|----|----|
|`04`|`% handles    structure with handles and user data (see GUIDATA) `|
|----|----|
|`05`|`global img1; `|
|----|----|
|`06`|`axes(handles.axes1); `|
|----|----|
|`07`|`[fname,fpath]=uigetfile('*.bmp;*.jpg','open a file'); `|
|----|----|
|`08`|`filename=[fpath,fname]; `|
|----|----|
|`09`|`[I,map]=imread(filename); `|
|----|----|
|`10`|`imshow(I,map); `|
|----|----|
|`11`|`I=double(I); `|
|----|----|
|`12`|`[Gx,Gy]=gradient(I);       % 计算梯度 `|
|----|----|
|`13`|`G=sqrt(Gx.*Gx+Gy.*Gy);   % 注意是矩阵点乘 `|
|----|----|
|`14`|` `|
|----|----|
|`15`|`J1=G;                  % 第一种图像增强 `|
|----|----|
|`16`|` `|
|----|----|
|`17`|`J2=I;                   % 第二种图像增强 `|
|----|----|
|`18`|`K=find(G>=7); `|
|----|----|
|`19`|`J2(K)=G(K); `|
|----|----|
|`20`|` `|
|----|----|
|`21`|` `|
|----|----|
|`22`|`J3=I;                   % 第三种图像增强 `|
|----|----|
|`23`|`K=find(G>=7); `|
|----|----|
|`24`|`J3(K)=255; `|
|----|----|
|`25`|` `|
|----|----|
|`26`|`J4=I;                   % 第四种图像增强 `|
|----|----|
|`27`|`K=find(G<=7); `|
|----|----|
|`28`|`J4(K)=255; `|
|----|----|
|`29`|` `|
|----|----|
|`30`|` `|
|----|----|
|`31`|`J5=I;                   % 第五种图像增强 `|
|----|----|
|`32`|`K=find(G<=7); `|
|----|----|
|`33`|`J5(K)=0; `|
|----|----|
|`34`|`Q=find(G>=7); `|
|----|----|
|`35`|`J5(Q)=255; `|
|----|----|
|`36`|` `|
|----|----|
|`37`|`figure,                 %显示图形 `|
|----|----|
|`38`|`subplot(2,3,1),imshow(I,map); `|
|----|----|
|`39`|`title('原图像'); `|
|----|----|
|`40`|`subplot(2,3,2),imshow(J1,map); `|
|----|----|
|`41`|`title('第一种'); `|
|----|----|
|`42`|`subplot(2,3,3),imshow(J2,map); `|
|----|----|
|`43`|`title('第二种'); `|
|----|----|
|`44`|`subplot(2,3,4),imshow(J3,map); `|
|----|----|
|`45`|`title('第三种'); `|
|----|----|
|`46`|`subplot(2,3,5),imshow(J4,map); `|
|----|----|
|`47`|`title('第四种'); `|
|----|----|
|`48`|`subplot(2,3,6),imshow(J5,map); `|
|----|----|
|`49`|`title('第五种');`|
|----|----|
转自：[http://www.oschina.net/code/snippet_104512_4395](http://www.oschina.net/code/snippet_104512_4395)


