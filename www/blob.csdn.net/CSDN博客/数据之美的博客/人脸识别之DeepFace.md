# 人脸识别之DeepFace - 数据之美的博客 - CSDN博客
2017年03月24日 19:00:21[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2793
该论文为2016年的一篇ECCV，ADiscriminative Feature Learning Approach for Deep Face Recognition。还是深圳先进院乔宇老师组的一篇。不管是思想还是效果都非常的棒。
论文思想：
![](https://img-blog.csdn.net/20161123184329434)
整体流程就是读取输入的图像，然后做个前向传播，应用softmax loss+center loss学习到DiscriminativeFeatures，然后进行标签的预测。
         softmaxloss可以实现将2个类别分开，center loss可以实现减少类类距离，增加类间距离的效果，有点类似，线性判别分析LDA（Linear Discriminant Analysis）的思想。在效果上优于contrastiveloss和triplet loss，解决了这2个方法在采样问题中的trick。
         其中，softmaxloss公式如下：
![](https://img-blog.csdn.net/20161123184410389)
center loss公式如下：
![](https://img-blog.csdn.net/20161123184427309)
总体的损失函数就是下面的式子：
![](https://img-blog.csdn.net/20161123184455793)
实际的mnist测试效果如下图所示：
左图为不加center_loss的效果，右图为加了center_loss的效果。可见效果确实是不错的。
![](https://img-blog.csdn.net/20161123184513375)
最后，测试2张人脸图像的相似性，直接计算cosine distance，简单的理解，2个特征向量越相似，夹角就会越小，cos值就会越大，也就是2个人脸越相似。
![](https://img-blog.csdn.net/20170110100232912)
注意事项：
因为必须首先确保matcaffe的正确安装，linux下安装成功的标志为在cafferoot/matlab/+caffe/目录下有caffe_.mexa64存在。同理，windows为caffe\Build\x64\Release\matcaffe\+caffe\private下有caffe_.mexw64存在。
直接跑测试的demo的话，原版的caffe就可以跑，因为虽然作者使用了center_loss，但是在检测的过程中，只用到了前向传播，在face_deploy.prototxt中是没有center_loss这个layer的。
跑训练的话就需要编译作者提供的caffe了，里面用到了center_loss，原版的caffe是不带的，或者自己加进去，重新编译下。可以看到在face_train_test.prototxt中是包含center_loss这一层的。
实验结果：
测试图像如下，大小为816*816，
![](https://img-blog.csdn.net/20161123184534403)
matlab源码运行效果：
![](https://img-blog.csdn.net/20161123184551063)
本人改好的c语言release下运行效果（8G内存+XeonE3-1225）
![](https://img-blog.csdn.net/20161123184615450)
本人改好的CUDA下运行效果（titanX）
![](https://img-blog.csdn.net/20161123184632330)
官方程序链接：[https://github.com/ydwen/caffe-face](https://github.com/ydwen/caffe-face)
[http://ydwen.github.io/](http://ydwen.github.io/)
人脸识别训练：
该训练最重要的其实还是数据集的准备，也就是先期数据的预处理，所有的图片都要crop为112*96的大小，因为这里是使用的caffe自带的train进行训练的。
修改face_train_test.prototxt中的相关参数，
18行：source的位置，训练的图片，格式为路径/图片名  类别（从0开始）
39行：source的位置，测试的图片，格式为路径/图片名  类别
1146行：num_output，输出的个数，CASIA有10575个，但是有3个和LFW一样，所以这里作者用了10572个
1175行：num_output，输出的个数，CASIA有10575个，但是有3个和LFW一样，所以这里作者用了10572个
1180行：loss_weight，这里作者程序中给的是0.008，论文中验证的是0.003最佳，可以结合自己的数据集实际测试，本人测试效果差距不大，该参数为center_loss的乘积参数
然后下面的脚本开始训练，
```
cd $CAFFE-FACE_ROOT
./build/tools/caffe train -solver face_example/face_solver.prototxt -gpu X,Y
```
ps:CASIA-WebFace和LFW重复照片的对应关系为，
![](https://img-blog.csdn.net/20170316124151880)
LFW识别率测试：
由于作者提供了其LFW数据集提取出来的特征矩阵，所以，本人也对其LFW识别率进行了测试。
首先，LFW的这个测试，包含了6000对测试比对，其中3000对为同一个人的比对，3000对不同人之间的比对，下面贴一个本人写的matlab小程序。
```
%directed by watersink2016.12.26
%any question,send me watersink2016@gmail.com
clc;clear;
pair = importfile('pair.txt',1, 6001);
pic1=pair(:,1);
num1=pair(:,2);
pic2_num2=pair(:,3);
num2=pair(:,4);
LFW_Feature=load('LFW_Feature');
similarone=[];
similartwo=[];
for i=2:6001
%    第一行不做处理
i
str1='';
str2='';
if  length(pic2_num2{i})<4
    %处理同一个人
    str1=strcat(strcat(strcat(pic1{i},'_'),num2str(num1{i},'%04d')),'.jpg');
    str2=strcat(strcat(strcat(pic1{i},'_'),num2str(str2num(pic2_num2{i}),'%04d')),'.jpg');
else
    %处理不同人
    str1=strcat(strcat(strcat(pic1{i},'_'),num2str(num1{i},'%04d')),'.jpg');
    str2=strcat(strcat(strcat(pic2_num2{i},'_'),num2str(num2{i},'%04d')),'.jpg');
end
    numnum1=find(strcmp(LFW_Feature.list,str1)==1);
    numnum2=find(strcmp(LFW_Feature.list,str2)==1);
    similar=dot(LFW_Feature.feature(:,numnum1),LFW_Feature.feature(:,numnum2))/norm(LFW_Feature.feature(:,numnum1))...
        /norm(LFW_Feature.feature(:,numnum2));
    if length(pic2_num2{i})<4
        similarone=[similarone;similar];
    else
        similartwo=[similartwo;similar];
    end
end
%画最后相似性得分图
figure,plot(similarone);
xlabel('category');ylabel('similarity');title('oneself to oneself');
figure,plot(similartwo);
xlabel('category');ylabel('similarity');title('oneself to otherselves');
%测试不同阈值下的最终识别率
similiarityAll=[];
for threshold=0:0.0001:1
    threshold
    numpos=0;
    numneg=0;
    for i=1:size(similarone,1)
        if similarone(i)>=threshold
            numpos=numpos+1;
        else
            numneg=numneg+1;
        end
         if similartwo(i)<threshold
             numpos=numpos+1;
         else
             numneg=numneg+1;
        end
    end
    similiarity=numpos/(numpos+numneg);
    similiarityAll=[similiarityAll;similiarity];
    
end
bestthreshold=find(similiarityAll==max(similiarityAll));
figure,plot([0:0.0001:1],similiarityAll),hold on,plot(bestthreshold/10000,max(similiarityAll),'*r');
xlabel('threshold');ylabel('similarity');title('different threshold generate different similiarity');
```
![](https://img-blog.csdn.net/20161226101320671)
![](https://img-blog.csdn.net/20161226101329906)
![](https://img-blog.csdn.net/20161226101335938)
上面第一个图为，3000对自己和自己的图像进行比对的cos距离值，第二幅图为自己和别人的图片进行比对的cos距离值，第三幅图为不同阈值（0-1）下的识别率，红色标记的*表示为最佳的识别率。
本人测试的最终的最佳识别率为0.9875，该识别率下的阈值为
    0.4164
    0.4165
    0.4166
    0.4167
    0.4395
    0.4396
    0.4397
    0.4398
    0.4399
    0.4400
    0.4401
    0.4402
    0.4403
和作者论文中的0.9928还是有个小差距的。可能出错的地方可能就是本人处理数据的某些细节可能和作者的不一样把，或者这里提供的不是最好的那个模型，而是一个供开源测试的模型吧，本人猜测。这里，希望可以得到大神的指正。
程序和所需文件下载链接：[http://download.csdn.net/detail/qq_14845119/9721049](http://download.csdn.net/detail/qq_14845119/9721049)
