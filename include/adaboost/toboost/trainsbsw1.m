    %%%该程序是测试程序，只用了一个定序滤波来提取人脸特征，用来验证adboost选取特征程序的正确性%%
    %clear all;
    clc;
    filename1=['D:\我的文档\liuhaimei\mat\castrainpca'];
    filename1=load(filename1);
   trainall=filename1.trainhistall;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%
    PerclassNum=4; %类内训练样本中每人有4张图片
    ClassNum=100;  %训练集合中共有300人
    AllNum=PerclassNum*ClassNum; 
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%
    PerclassNum1=4; %类间图片数
    ClassNum1=100; 
    AllNum1=PerclassNum1*ClassNum1;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      计算类内
    disp('计算类内距离')
    featurelength=size(trainall,2);
    swdistance=[]; % 类内距离矩阵
    for i=1:ClassNum
     sample=trainall(((i-1)*PerclassNum+1):i*PerclassNum,:); % 每一类样本数据 
     count=0;
       sw=[];
     for j=1:(PerclassNum-1)
         for m=(j+1):PerclassNum
               count= count+1;
               sampledistance=abs(sample(j,:)-sample(m,:)); %  
                sw=[sw;sampledistance];
         end
     end
             swdistance=[swdistance;sw];  %类内两张图片的距离
    end
    filesave=strcat('D:\我的文档\MATLAB\swdislbp','.mat');
    save(filesave,'swdistance');
% %     clear swdistance sample sampledistance;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      % 计算类间
%       disp('计算类间距离')
% %       sbdistance=zeros(45*16,featurelength); 
%         sbdistance=[];
%         counts_class=0;
%          n=0;
%        for k=1:(ClassNum1-1)
%           TP=trainall(((k-1)*PerclassNum1+1):k*PerclassNum1,:);
%          for h=(k+1):ClassNum1
%              counts_bclass=0;
%              counts_class=counts_class+1;
%              TQ=trainall(((h-1)*PerclassNum1+1):h*PerclassNum1,:);
%              for m=1:PerclassNum1
%                  for tk=1:PerclassNum1
%                   counts_bclass= counts_bclass+1;
%                    n=n+1;
%                  sbdistance=[sbdistance;abs(TP(m,:)-TQ(tk,:))];
%                  end
%              end
%          end
%        end
%      filesave2=strcat('D:\我的文档\MATLAB\sbdislbp','.mat');
%      save(filesave2,'sbdistance');

    