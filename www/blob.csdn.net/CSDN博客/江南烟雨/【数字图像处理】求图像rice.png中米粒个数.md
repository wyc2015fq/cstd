# 【数字图像处理】求图像rice.png中米粒个数 - 江南烟雨 - CSDN博客
2011年10月27日 10:22:12[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：15701
 图像处理课程上老师布置的任务，要求求出图片rice.png中米粒的个数及其各米粒的大小。
rice.png:
![](http://hi.csdn.net/attachment/201112/9/0_13234378796q65.gif)
大体步骤是：首先进行边缘检测，然后进行填充。然后进行开运算，可以使一些轻微连着的米粒分开来。然后是遍历图片，把各米粒进行标号，从1开始从小到大标号。每个米粒的各像素点值相同，第i个米粒的各像素点值均为i。其中采用了队列，用数组模拟。最大的米粒标号便是米粒的总个数。最后，遍历一遍图像数组，便可求出各米粒的面积。
参考代码(matlab实现)：
```
clear;
close all;
I = imread('rice.png');
[width,height] = size(I);
J = edge(I,'canny');
%figure,imshow(J);
K = imfill(J,'holes');
%figure,imshow(K);
SE = strel('disk',3);%用于膨胀腐蚀及开闭运算等操作的结构元素对象
%对图像实现开运算，开运算一般能平滑图像的轮廓，消弱狭窄的部分，去掉细的突出。
L = imopen(K,SE);
figure,imshow(L);
L = uint8(L);%把L由logic类型转化为uint8类型
for i = 1:height
    for j = 1:width
        if L(i,j) == 1
            L(i,j) = 255;%把白色像素点像素值赋值为255
        end
    end
end
MAXSIZE = 999999;
Q = zeros(MAXSIZE,2);%用数组模拟队列,存储像素点坐标
front = 1;%指明队头的位置
rear = 1;%指明队尾的下一个位置；front=rear表示队空
flag = 0;%米粒的标号
for i = 1:height
    for j = 1:width
        if L(i,j) == 255%白色像素点入队列
            if front == rear%队列空，找到新米粒，米粒标号加一
                flag = flag+1;
            end
            L(i,j) = flag;%给白色像素赋值为米粒的标号
            Q(rear,1) = i;
            Q(rear,2) = j;
            rear = rear+1;%队尾后移
            while front ~= rear
                %队头出队
                temp_i = Q(front,1);
                temp_j = Q(front,2);
                front = front + 1;
                %把队头位置像素点8连通邻域中未作标记的白色像素点入队,并加上米粒标号
                %左上角的像素点
                if L(temp_i - 1,temp_j - 1) == 255
                    L(temp_i - 1,temp_j - 1) = flag;
                    Q(rear,1) = temp_i - 1;
                    Q(rear,2) = temp_j - 1;
                    rear = rear + 1;
                end
                %正上方的像素点
                if L(temp_i - 1,temp_j) == 255
                    L(temp_i - 1,temp_j) = flag;
                    Q(rear,1) = temp_i - 1;
                    Q(rear,2) = temp_j;
                    rear = rear + 1;
                end
                %右上方的像素点
                if L(temp_i - 1,temp_j + 1) == 255
                    L(temp_i - 1,temp_j + 1) = flag;
                    Q(rear,1) = temp_i - 1;
                    Q(rear,2) = temp_j + 1;
                    rear = rear + 1;
                end
                %正左方的像素点
                if L(temp_i,temp_j - 1) == 255
                    L(temp_i,temp_j - 1) = flag;
                    Q(rear,1) = temp_i;
                    Q(rear,2) = temp_j - 1;
                    rear = rear + 1;
                end
                %正右方的像素点
                if L(temp_i,temp_j + 1) == 255
                    L(temp_i,temp_j + 1) = flag;
                    Q(rear,1) = temp_i;
                    Q(rear,2) = temp_j + 1;
                    rear = rear + 1;
                end
                %左下方的像素点
                if L(temp_i + 1,temp_j - 1) == 255
                    L(temp_i + 1,temp_j - 1) = flag;
                    Q(rear,1) = temp_i + 1;
                    Q(rear,2) = temp_j - 1;
                    rear = rear + 1;
                end
                %正下方的像素点
                if L(temp_i + 1,temp_j) == 255
                    L(temp_i + 1,temp_j) = flag;
                    Q(rear,1) = temp_i + 1;
                    Q(rear,2) = temp_j;
                    rear = rear + 1;
                end
                %右下方的像素点
                if L(temp_i + 1,temp_j + 1) == 255
                    L(temp_i + 1,temp_j + 1) = flag;
                    Q(rear,1) = temp_i + 1;
                    Q(rear,2) = temp_j + 1;
                    rear = rear + 1;
                end
            end
        end
    end
end
figure,imshow(L);
RiceNumber = flag;%记录米粒的总个数
disp('米粒的总个数:')
RiceNumber
RiceArea = zeros(1,RiceNumber);%记录各米粒的大小
for i = 1:height
    for j = 1:width
        if L(i,j) ~= 0
            RiceArea(L(i,j)) = RiceArea(L(i,j)) + 1;
        end
    end
end
disp('各米粒的大小(按照从上往下，从左往右的顺序)：')
RiceArea
```
运行结果：
边缘检测，填充，开运算之后的结果：
![](http://hi.csdn.net/attachment/201112/9/0_13234377807kWL.gif)
可以看出有些轻微连着的米粒被分开了。
进行标号后的结果：
![](http://hi.csdn.net/attachment/201112/9/0_1323437792ep6p.gif)
米粒是从上到下从左到右依次编号的。从图中米粒的明暗程度可以看出来。
最终结果：
*米粒的总个数:*
*RiceNumber =    69各米粒的大小(按照从上往下，从左往右的顺序)：RiceArea =  Columns 1 through 20   212   146   197   179   224   431   208   182   147   189   236   214   207   202   138   188   158   202   210   181  Columns 21 through 40   186   211   199   227   203   223   224   227   249   254   225   187   183   200   232   199   210   200   198   183  Columns 41 through 60   230   268   233   235   193   206   240   158   207   215   238   200   239   227   207   254   237   203   211   236  Columns 61 through 69   219   152   225   238   169   223   215   195   195*
 有一个比较简单的方法是直接调用bwlabel函数，进行连通域标记。不过老师要求尽量不要调用这些库函数，自己把他实现出来，这样理解的深刻一些，更好地提高编程能力。
```python
clear;
close all;
I=imread('rice.png');
%imshow(I);
J=edge(I,'canny');
%figure,imshow(J);
K=imfill(J,'holes');
figure,imshow(K);
SE=strel('disk',3);%用于膨胀腐蚀及开闭运算等操作的结构元素对象
%对图像实现开运算，开运算一般能平滑图像的轮廓，消弱狭窄的部分，去掉细的突出。
L=imopen(K,SE);
figure,imshow(L);
M=bwlabel(L,4);%8联通域标记
figure,imshow(M,[]);
%矩阵M的大小
[m,n]=size(M);
num=0;%米粒的个数
for i=1:m
    for j=1:n
        if M(i,j)>num%最大的label值即联通域的个数
            num=M(i,j);
        end
    end
end
area=zeros(1,num);%记录各个米粒的大小
for i=1:m
    for j=1:n
        if M(i,j)>0%不是背景，在联通域里面
            area(1,M(i,j))=area(1,M(i,j))+1;%对应的米粒大小加一
        end
    end
end
disp('米粒个数：');
disp(num);
disp('各个米粒的大小');
area
```
