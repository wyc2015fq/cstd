# 基于kernel的KNN 分类算法 - 家家的专栏 - CSDN博客





2013年12月24日 21:00:10[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：968








                
function [CRR,y_hat]=myNN(Test_Data,Train_Data,K)

%Train_data.X: M*M

%test_data.X: M*N

%Train_data.y: 1*M

%test_data.y: 1*N

Number_Samples=length(Test_Data.y);

Number_Train_Samples=length(Train_Data.y);

y_hat=zeros(1,Number_Samples);

for tmpC1 = 1:Number_Samples

%1NN    

%     [junk1,tmpInd]=min(Test_Data.X(tmpC1,:));

%     y_hat(tmpC1) = Train_Data.y(tmpInd);

%KNN

    [a,b]=sort(Test_Data.X(tmpC1,:));

    K_labels = Train_Data.y(b(1:K));

    [~,K] = size(K_labels);  

    class_count = zeros(1,max(Test_Data.y));  

    for i=1:K  

        class_index = K_labels(i);  

        class_count(class_index) = class_count(class_index) + 1;  

    end

    [~,y_hat(tmpC1) ]= max(class_count);

end

CRR=1-length(find((y_hat-Test_Data.y)~=0))/Number_Samples




参考网址：[http://blog.csdn.net/rk2900/article/details/9080821](http://blog.csdn.net/rk2900/article/details/9080821)



