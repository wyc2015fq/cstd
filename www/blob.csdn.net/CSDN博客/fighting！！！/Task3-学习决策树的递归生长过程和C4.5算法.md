# Task3 - 学习决策树的递归生长过程和C4.5算法 - fighting！！！ - CSDN博客
2019年01月18日 11:46:13[dujiahei](https://me.csdn.net/dujiahei)阅读数：81

任务3：参考自己较为喜欢的课本（如西瓜书），理解决策树递归生长过程和C4.5算法。完成下面的作业（该内容也放到了QQ群，）：
![](https://img-blog.csdnimg.cn/20190117225128524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
根据以上表格，把特征选择过程和最终形成的决策树写出来（可通过在纸上写拍照或者用Latex），并且思考决策树的终止条件都用到了哪些。分析没有用到的终止条件自己是否理解了？
参考：【1】[https://blog.csdn.net/gumpeng/article/details/51397737](https://blog.csdn.net/gumpeng/article/details/51397737)
【2】[https://www.cnblogs.com/gispathfinder/p/5770217.html](https://www.cnblogs.com/gispathfinder/p/5770217.html)
【3】[https://www.cnblogs.com/hermione1985/p/6750209.html](https://www.cnblogs.com/hermione1985/p/6750209.html)
【4】[https://cloud.tencent.com/developer/article/1082509](https://cloud.tencent.com/developer/article/1082509)
【5】[https://www.cnblogs.com/mfrbuaa/p/4730248.html](https://www.cnblogs.com/mfrbuaa/p/4730248.html)
【6】[http://www.cnblogs.com/yanfengfree/p/3606579.html](http://www.cnblogs.com/yanfengfree/p/3606579.html)
【7】[https://blog.csdn.net/shenxiaoming77/article/details/51602976](https://blog.csdn.net/shenxiaoming77/article/details/51602976)
手算实在辛苦 2333，我改用了matlab来完成。但是单次计算可以，多次计算有些问题，我觉得是part 函数那里的初始化变量有问题，待会儿接着搞。
经过修改后的信息增益函数如下：
```
%% 计算增益
% INPUT: group_value,Data,Decis_value 
% OUTPUT: ev,infoV
 %Data 要传入两列，变量和标签
function [ev,infoV]=InfoGainRCalculatePart(group_value,Data,Decis_value,r)
ev=0;%变量熵
infoV=0;
for j=1:length(group_value)
    Decis_value(:,2)=0;
    location=find(Data(:,1)==group_value(j));
    variable_value_lable=Data(location,end);%变量值对应标签
    ev_value=0;%变量值熵
    %         info=0;%划分带来的信息
    for k=1:length(Decis_value)
        Decis_value(k,2)=sum((variable_value_lable==Decis_value(k,1)),1);%计算变量值对应标签每个值数量
        if Decis_value(k,2)~=0
            ev_value=ev_value+(-(Decis_value(k,2)/length(variable_value_lable))*log2(Decis_value(k,2)/length(variable_value_lable)));%计算变量的不确定性
        end
    end
    ev=ev+(length(variable_value_lable)/r)*ev_value;
    infoV=infoV+(-(length(variable_value_lable)/r)*log2((length(variable_value_lable)/r)));
end
```
首先load数据，将所有特征数值化：
```
clc
clear all
% RawData=importdata('C4.5_homework.txt');
% RawData=dlmread('C4.5_homework.txt');
[Day,Outl,Temp,Hum,Wind,Decis]=textread('C4.5_homework.txt','%s %s %d %d %s %s');
% 特征数值化
for i=1:14
    %outlook
   if strcmp(Outl{i},'Sunny') 
       RawData(i,1)=1;
   elseif strcmp(Outl{i},'Overcast') 
       RawData(i,1)=2;
   elseif strcmp(Outl{i},'Rain') 
       RawData(i,1)=3;
   end
   %Temp
   RawData(i,2)=Temp(i);
   % Humidity
   RawData(i,3)=Hum(i);
   % Wind
   if strcmp(Wind{i},'Strong') 
       RawData(i,4)=1;
   else 
       RawData(i,4)=2;  
   end
   %Decis
   if strcmp(Decis{i},'Yes') 
       RawData(i,5)=1;
   else 
       RawData(i,5)=0;  
   end
end
```
### 第一层节点
计算标签，或者说最终决定的信息熵：
```
[r,c]=size(RawData);
for iGroup=1:c
    RawGroup_value{iGroup}=unique(RawData(:,iGroup));
end
Decis_value=RawGroup_value{end};
% Decis 的信息熵
e=0;%Decis 的信息熵
for iDec=1:length(Decis_value)
    lableNum(iDec,1)=sum((RawData(:,end)==Decis_value(iDec,1)),1);%计算标签每个值数量
    e=e+(-(lableNum(iDec,1)/r)*log2(lableNum(iDec,1)/r));%计算随机标签的不确定性
end  
% 以上计算正确 e=0.9403
```
计算离散特征的信息增益率：
```
Data=RawData;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup));
end
% 计算增益率
% input: c,group_value,Data,Decis_value
Ev=zeros(c-1,1);%变量熵
InfoV=zeros(c-1,1);
in_gain=[];%信息增益
gain_ration=[];%信息增益率
 
% outlook 信息增益率
    i=1;
    %Data 要传入两列，变量和标签
    [Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);
    in_gain(i,1)=e-Ev(i); %0.246 计算正确
    gain_ration(i,1)=in_gain(i,1)/InfoV(i); % 0.1564 计算正确
 
 % wind 信息增益率
     i=4;
      [Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);    
    in_gain(i,1)=e-Ev(i);
     gain_ration(i,1)=in_gain(i,1)/InfoV(i); %0.048 计算正确
```
因此，outlook 信息增益率是0.1564，wind信息增益率是0.048。
计算连续特征的信息增益率
```
% 对连续值的处理
% Temp
TempSplitAll=[64.5,66.5,70.5,72,77.5,80.5,84];
 i=2;
for iTemp=1:length(TempSplitAll) 
TempSplit=TempSplitAll(iTemp);
IndTemp0=find(RawData(:,i)<TempSplit);
Data(:,i)=1;
Data(IndTemp0,i)=0;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup));
end
% temp 信息增益率
  [Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);    
    in_gain(i,1)=e-Ev(i);
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    % 连续特征需要减去一个常数 log2(N-1)/|D|（N是连续特征的取值个数，D是训练数据数目
    % temp continuous_temperature_cost_1 = np.log2(dataset['temperature'].nunique() - 1)/ num_datasets 
    if i==2
    ContinuCost=log2(length(RawGroup_value{i})-1)/length(Decis); % !!! 之前理解有误 0.2471 计算正确
    in_gain(i,1)=in_gain(i,1)-ContinuCost;
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    end    
 TempInGain(iTemp)=in_gain(i,1);
 TempGainRation(iTemp)=gain_ration(i,1);
end
 
% 对连续值的处理
% Humidity
i=3;
HumSplitAll=[67.5,70,80,90,92.5,95.5];
for iHum=1:length(HumSplitAll) 
HumSplit=HumSplitAll(iHum);
IndHum0=find(RawData(:,i)<HumSplit);
Data(:,i)=1;
Data(IndHum0,i)=0;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup)); % !!! 修改Data数据后，记得修改group_value
end
% Humidity 信息增益率
 
  [Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);    
    in_gain(i,1)=e-Ev(i);
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    % 连续特征需要减去一个常数 log2(N-1)/|D|（N是连续特征的取值个数，D是训练数据数目
    % temp continuous_temperature_cost_1 = np.log2(dataset['temperature'].nunique() - 1)/ num_datasets 
    if i==3
    ContinuCost=log2(length(RawGroup_value{i})-1)/length(Decis); % !!! 之前理解有误 0.2471 计算正确
    in_gain(i,1)=in_gain(i,1)-ContinuCost;
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    end    
 HumInGain(iHum)=in_gain(i,1);
 HumGainRation(iHum)=gain_ration(i,1);
end
```
temperature的信息增益率为 -0.537111487165885    -0.400194442157365    -0.214581622307969    -0.249447317916535    -0.257234149033146    -0.328994596615627    -0.360155581015254；
humidity的信息增益率为：-0.448712547113111    -0.448712547113111    -0.125868679910130    -0.219213337823796    -0.344730433373644    -0.448712547113111    0.0635086282979060。
从以上数据来看，outlook的信息增益率最大，所以将outlook定做第一个节点。
### 第二层节点
对outlook下的sunny进行划分：
```
% outlook==1（sunny）的数据
iSunny=find(RawData(:,1)==1);
DataSunny=RawData(iSunny,:);
[r,c]=size(DataSunny);
for iGroup=1:c
    RawGroup_value{iGroup}=unique(DataSunny(:,iGroup));
end
Decis_value=RawGroup_value{end};
% Decis 的信息熵
e=0;%Decis 的信息熵
for iDec=1:length(Decis_value)
    lableNum(iDec,1)=sum((DataSunny(:,end)==Decis_value(iDec,1)),1);%计算标签每个值数量
    e=e+(-(lableNum(iDec,1)/r)*log2(lableNum(iDec,1)/r));%计算随机标签的不确定性 0.9710 对
end  
Data=DataSunny;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup));
end
```
![](https://img-blog.csdnimg.cn/20190119122359443.png)
计算wind信息增益率
```
% 计算增益率
% input: c,group_value,Data,Decis_value
Ev=zeros(c-1,1);%变量熵
InfoV=zeros(c-1,1);
in_gain=[];%信息增益
gain_ration=[];%信息增益率
%wind
i=4;
[Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);
in_gain(i,1)=e-Ev(i);
gain_ration(i,1)=in_gain(i,1)/InfoV(i); % InfoV=0.9710,Ev(i)=0.9510计算正确  0.0206
% 手算检验一下
% entropy_2_dataset =0.97095059445466858;
% entropy_2_wind=-2/5*(1/2*log2(1/2)+1/2*log2(1/2))-3/5*(1/3*log2(1/3)+2/3*log2(2/3));
% IV_2_wind=0.970950594455;
% entropy_ratio_2_wind = (entropy_2_dataset - entropy_2_wind) / IV_2_wind;
```
wind的信息增益率是0.0206；
计算Temperature的信息增益率，分裂点：70.5，73.5，77.5
![](https://img-blog.csdnimg.cn/20190119125059210.png)
```
%Temp
TempSplitAll=[70.5,73.5,77.5];
TempInGain=[];
TempGainRation=[];
 i=2;
for iTemp=1:length(TempSplitAll) 
TempSplit=TempSplitAll(iTemp);
IndTemp0=find(DataSunny(:,i)<TempSplit);
Data(:,i)=1;
Data(IndTemp0,i)=0;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup));
end
% temp 信息增益率
  [Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);    
    in_gain(i,1)=e-Ev(i);
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    % 连续特征需要减去一个常数 log2(N-1)/|D|（N是连续特征的取值个数，D是训练数据数目
    % temp continuous_temperature_cost_1 = np.log2(dataset['temperature'].nunique() - 1)/ num_datasets 
    if i==2
    ContinuCost=log2(length(RawGroup_value{i})-1)/length(Decis); % !!! 之前理解有误 0.2471 计算正确
    in_gain(i,1)=in_gain(i,1)-ContinuCost;
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    end    
 TempInGain(iTemp)=in_gain(i,1);
 TempGainRation(iTemp)=gain_ration(i,1);
end
```
得到的增益率：0.248045412414873    -0.126560557804885    0.285406850510734。
计算Humidity的增益，分裂点77.5
![](https://img-blog.csdnimg.cn/20190119125542389.png)
```
i=3;
HumSplitAll=77.5;
 HumInGain=[];
HumGainRation=[];
for iHum=1:length(HumSplitAll) 
HumSplit=HumSplitAll(iHum);
IndHum0=find(DataSunny(:,i)<HumSplit);
Data(:,i)=1;
Data(IndHum0,i)=0;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup)); % !!! 修改Data数据后，记得修改group_value
end
% Humidity 信息增益率
 
  [Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);    
    in_gain(i,1)=e-Ev(i);
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    % 连续特征需要减去一个常数 log2(N-1)/|D|（N是连续特征的取值个数，D是训练数据数目
    % temp continuous_temperature_cost_1 = np.log2(dataset['temperature'].nunique() - 1)/ num_datasets 
    if i==3
    ContinuCost=log2(length(RawGroup_value{i})-1)/length(Decis); % !!! 之前理解有误 0.2471 计算正确
    in_gain(i,1)=in_gain(i,1)-ContinuCost;
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    end    
 HumInGain(iHum)=in_gain(i,1);
 HumGainRation(iHum)=gain_ration(i,1);
end
```
得到Humidity的增益率0.8834.
因此，Humidity的增益率最大，所以选择Humidity作为Sunny下的节点。
### Overcast 下的分类结果都为yes，所以不需要分叉。
### Rain分叉
```
%Rain分叉
% outlook==3（Rain）的数据
iRain=find(RawData(:,1)==3);
DataRain=RawData(iRain,:);
[r,c]=size(DataRain);
for iGroup=1:c
    RawGroup_value{iGroup}=unique(DataRain(:,iGroup));
end
Decis_value=RawGroup_value{end};
% Decis 的信息熵
e=0;%Decis 的信息熵
for iDec=1:length(Decis_value)
    lableNum(iDec,1)=sum((DataRain(:,end)==Decis_value(iDec,1)),1);%计算标签每个值数量
    e=e+(-(lableNum(iDec,1)/r)*log2(lableNum(iDec,1)/r));%计算随机标签的不确定性 0.9710 对
end  
Data=DataRain;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup));
end
```
![](https://img-blog.csdnimg.cn/20190119130340787.png)
计算wind
```
% 计算增益率
% input: c,group_value,Data,Decis_value
Ev=zeros(c-1,1);%变量熵
InfoV=zeros(c-1,1);
in_gain=[];%信息增益
gain_ration=[];%信息增益率
%Wind
i=4;
[Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);
in_gain(i,1)=e-Ev(i);
gain_ration(i,1)=in_gain(i,1)/InfoV(i);
```
得到信息增益率1.
计算Temperature，分裂点66.5，70.5，73
![](https://img-blog.csdnimg.cn/20190119130721186.png)
```
%Temp
TempSplitAll=[66.5,70.5,73];
TempInGain=[];
TempGainRation=[];
 i=2;
for iTemp=1:length(TempSplitAll) 
TempSplit=TempSplitAll(iTemp);
IndTemp0=find(DataRain(:,i)<TempSplit);
Data(:,i)=1;
Data(IndTemp0,i)=0;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup));
end
% temp 信息增益率
  [Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);    
    in_gain(i,1)=e-Ev(i);
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    % 连续特征需要减去一个常数 log2(N-1)/|D|（N是连续特征的取值个数，D是训练数据数目
    % temp continuous_temperature_cost_1 = np.log2(dataset['temperature'].nunique() - 1)/ num_datasets 
    if i==2
    ContinuCost=log2(length(RawGroup_value{i})-1)/length(Decis); % !!! 之前理解有误 0.2471 计算正确
    in_gain(i,1)=in_gain(i,1)-ContinuCost;
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    end    
 TempInGain(iTemp)=in_gain(i,1);
 TempGainRation(iTemp)=gain_ration(i,1);
end
```
得到Temperature的信息增益率：0.248045412414873    -0.126560557804885    0.0389144733339529。
计算Humidity，分裂点75
![](https://img-blog.csdnimg.cn/20190119131010857.png)
```
%humidity
i=3;
HumSplitAll=75;
 HumInGain=[];
HumGainRation=[];
for iHum=1:length(HumSplitAll) 
HumSplit=HumSplitAll(iHum);
IndHum0=find(DataRain(:,i)<HumSplit);
Data(:,i)=1;
Data(IndHum0,i)=0;
for iGroup=1:c
    group_value{iGroup}=unique(Data(:,iGroup)); % !!! 修改Data数据后，记得修改group_value
end
% Humidity 信息增益率
 
  [Ev(i),InfoV(i)]=InfoGainRCalculatePart(group_value{i},Data(:,[i,end]),Decis_value,r);    
    in_gain(i,1)=e-Ev(i);
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    % 连续特征需要减去一个常数 log2(N-1)/|D|（N是连续特征的取值个数，D是训练数据数目
    % temp continuous_temperature_cost_1 = np.log2(dataset['temperature'].nunique() - 1)/ num_datasets 
    if i==3
    ContinuCost=log2(length(RawGroup_value{i})-1)/length(Decis); % !!! 之前理解有误 0.2471 计算正确
    in_gain(i,1)=in_gain(i,1)-ContinuCost;
     gain_ration(i,1)=in_gain(i,1)/InfoV(i);
    end    
 HumInGain(iHum)=in_gain(i,1);
 HumGainRation(iHum)=gain_ration(i,1);
end
```
得到Humidity的信息增益率 0.346986805518179。
综上，Rain节点下选择Wind作为节点。
于是得到决策树如下图：
![](https://img-blog.csdnimg.cn/20190119131654457.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
