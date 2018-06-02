% 该函数采用最为简单的方法进行跟踪，前提是前面聚类结果够准确，而且帧频够大
% Edit Time: 2013.08.06
% 参数意义：
% 输入：
%       Video                  输入的已经处理好的前景聚类图片
%       Frame                  每一帧的前景区域中心坐标，这里是一个结构体，代表五帧
%       Trajectory1            第一帧的坐标和标记
%       Trajectory2            第二帧的坐标和标记
%       Trajectory3            第三帧的坐标和标记
%       NumPeopleIn            统计某一段时间进去的人数
%       NumPeopleOut           统计某一段时间出来的人数
%       NumEmpty45             统计某一段时间内连续多帧没有目标出现的帧数，用于某些参数定时更新
%       FlgOut                 是否执行统计计数标记，每一帧开始时必须赋值为0
%       Startcoord             用于记录某一目标初次出现的时候的坐标，这里定义为一个1*2*100的三维矩阵
%       Endcoord               用于记录某一目标消失的时候的坐标，这里定义为一个1*2*100的三维矩阵
%       RE                     用于记录每一次出来的目标标记，测试用，用于确定是否正确跟踪
%       RI                     用于记录每一次进来的目标标记，测试用，用于确定是否正确跟踪
%       DIN                    进来的目标起始坐标和消失点坐标的距离，测试用
%       DE                     出去的目标起始坐标和消失点坐标的距离，测试用
%       DA                     所有能够被统计的目标的起始坐标和消失点坐标的距离           
%       CountsStartcoordIn     统计标识
%       CountsEndcoordOut      统计标识
% 输出：与输入同
% 

function [RE,RI,DIN,DE,DA,Trajectory1,Trajectory2,Trajectory3,Startcoord,Endcoord,NumPeopleOut,NumPeopleIn,NumEmpty45,Frame,CountsStartcoordIn,CountsEndcoordOut]=...
    TrackingAndCaculate(VideoFrame,Frame,Trajectory1,Trajectory2,Trajectory3,NumPeopleIn,NumPeopleOut,NumEmpty45,FlgOut,...
    Startcoord,Endcoord,RE,RI,DIN,DE,DA,CountsStartcoordIn,CountsEndcoordOut)

Threshold=50; % 距离阈值 2013.08.08
bg=VideoFrame; % 当前处理好的前景聚类图片
bg(VideoFrame>0)=1; % 二值化
L=bwlabel(bg); % 标记
L=logical(L); % 转换为逻辑型数据
S=regionprops(L,'centroid'); % 求中心点
Frame{5}=S; % 保存当前的坐标

NumFrame1=length(Frame{1}); % 求当前坐标的数目
NumFrame2=length(Frame{2});
NumFrame3=length(Frame{3});
NumFrame4=length(Frame{4});
NumFrame5=length(Frame{5});

for ij=1:NumFrame3   % 记录当前处理帧的坐标，并将标记符号标记为0
    Trajectory3(ij,1:2)=Frame{3}(ij).Centroid;
    Trajectory3(ij,3)=0;
end

d32=Frame_Euler(Frame{2},Frame{3}); % 两帧之间的欧氏距离

 [Value32 Index32]=min(d32); % 按列求最小值,其中行数表示后一帧图像的目标数目，列数表示前一帧图像的目标数目，返回的索引是对第3帧的索引号
 
 if NumFrame1==0 && NumFrame2==0 && NumFrame3~=0 % 前两帧为空，第三帧(当前处理帧)为非空的时候，视之为开始标记
     for ij=1:NumFrame3
         CountsStartcoordIn=CountsStartcoordIn+1; % 标记标识，采用累加的方式进行标记
         Startcoord(1,:,CountsStartcoordIn)=Frame{3}(ij).Centroid; % 保存新增目标的起始坐标
         Trajectory3(ij,1:2)=Frame{3}(ij).Centroid; % 记录当前处理帧的目标坐标
         Trajectory3(ij,3)=CountsStartcoordIn; % 标记
     end
     if NumFrame4~=NumFrame3 && NumFrame3==NumFrame5  % 修改时间 2013.08.12 增加对帧不同的判断
         Frame{4}=[];
         Frame{1}=Frame{2};
         Frame{2}=Frame{3};
         Frame{3}=Frame{5};
         Frame{4}=Frame{5};
         
         Trajectory1=Trajectory2;
         Trajectory2=Trajectory3;
         Trajectory3=[];
     else
         Frame{1}=Frame{2};
         Frame{2}=Frame{3};
         Frame{3}=Frame{4};
         Frame{4}=Frame{5};
         
         Trajectory1=Trajectory2;
         Trajectory2=Trajectory3;
         Trajectory3=[];
     end
 elseif NumFrame1+NumFrame2+NumFrame3~=0 % 当三帧中有一帧非空，即执行相应的处理
     if isempty(d32) % 如果d32为空
        if NumFrame2~=0 % 当第二帧非空，第三帧为空的时候，说明这时最后一批走出的目标
            for ijh=1:NumFrame2
                disp('注意目标走出的情况,这是最后一人')
                CountsEndcoordOut=CountsEndcoordOut+1; % 走出图像的计数，用于确定消失点坐标存放的行
                FlgOut=1; % 指示标记赋值为1，表示有目标消失
                Endcoord(1,:,Trajectory2(ijh,3))=Trajectory2(ijh,1:2); % 保存当前消失目标的消失坐标
                disp(['CountsEndcoordOut: ',num2str(CountsEndcoordOut),'  Trajectory2(IndexList(ij),1:2): ',num2str(Trajectory2(ijh,3))])
            end
        end
    elseif ~isempty(d32)  % 第二帧和第三帧均为非空的时候
        TempList32=Index32;
        UniqueList32=unique(Index32); % 由于Index32中的内容反映行，表示第三帧的目标位置，而Index32的列索引反映的是第二帧目标位置，这里提取唯一的行信息
        NU32=size(UniqueList32,2); % 行索引的数目，其实就是第三帧的目标数目
        for ij=1:NU32 % 在第三帧中寻找最佳匹配的目标，并做标记，这里已经包含了目标不增不减的情况
            IndexEqual=find(TempList32==UniqueList32(ij)); % 逐行寻找
            NumEqual=sum(IndexEqual~=0);
            if NumEqual==1 % 只有一个匹配目标
                if NumFrame3==1 % 当第三帧只有一个目标时候，由于无法构成二维的矩阵所以单独处理
                    DisMatch=d32(IndexEqual,Index32(IndexEqual)); % 新增阈值处理2013.08.07
                    if DisMatch<Threshold % 新增阈值处理2013.08.07
                        Trajectory3(IndexEqual,3)=Trajectory2(Index32(IndexEqual),3);
                    end
                else % 第二帧目标数目为1或者两帧目标数目都不为1
                    DisMatch=d32(Index32(IndexEqual),IndexEqual); % 新增阈值处理2013.08.07
                    if DisMatch<Threshold % 新增阈值处理2013.08.07
                        Trajectory3(Index32(IndexEqual),3)=Trajectory2(IndexEqual,3);
                    end
                end                
            elseif NumEqual>1 % 存在多个匹配，寻找最佳匹配
                TempValue=zeros(1,NumEqual);
                for ijk=1:NumEqual % 寻找匹配的最小值
                    TempValue(1,ijk)=d32(Index32(IndexEqual(ijk)),IndexEqual(ijk));
                end
                [CValue CIndex]=min(TempValue); % 最佳匹配
                if CValue<Threshold % 新增阈值处理2013.08.07
                    Trajectory3(Index32(IndexEqual(CIndex)),3)=Trajectory2(IndexEqual(CIndex),3); % 从多个匹配中寻找最佳匹配
                end
            end
        end
        
        NumMatch3=sum((Trajectory3(:,3)~=0)); % 第三帧和第二帧匹配的目标数目        
        TempLeftList3=Trajectory3;
        
        if NumMatch3<NumFrame3 % 当匹配数目小于第三帧目标数目，表示有新出现的目标
            IndexList3=find(Trajectory3(:,3)==0); % 寻找新出现的目标在Trajectory3中对应的行
            Nempty3=size(IndexList3,1); % 新出现目标的数目
            for ij=1:Nempty3 % 对新出现的目标做标记，并保存其起始坐标
                CountsStartcoordIn=CountsStartcoordIn+1;
                Trajectory3(IndexList3(ij),3)=CountsStartcoordIn;
                Startcoord(1,:,CountsStartcoordIn)=Trajectory3(IndexList3(ij),1:2);
            end
        end
        if NumMatch3<NumFrame2 % 当匹配的数目小于第二帧目标数目的时候，表示有目标消失
            IndexList32=Trajectory2(:,3); 
            for ij=1:NumFrame3 % 寻找消失的目标标记
                IndexList32(IndexList32==TempLeftList3(ij,3))=[];
            end
            IndexList=IndexList32;
            for ij=1:size(IndexList,1) % 记录消失目标的消失点坐标
                CountsEndcoordOut=CountsEndcoordOut+1;
                FlgOut=1;
                Tij=find(Trajectory2(:,3)==IndexList(ij));
                Endcoord(1,:,Trajectory2(Tij,3))=Trajectory2(Tij,1:2);
                disp(['CountsEndcoordOut: ',num2str(CountsEndcoordOut),'  Trajectory2(IndexList(ij),3): ',num2str(Trajectory2(Tij,3))])
            end
        end
        %             if NumMatch3==NumFrame3 && NumFrame3==NumFrame2
        %                 for ij=1:NumFrame3
        %                     Trajectory3(ij,3)=Trajectory2(Index32(ij),3);
        %                 end
        %             else
        %                 disp('没考虑到的情况')
        %             end
    end
    if FlgOut==1 % 如果有目标消失
        if NumFrame3>0 % 第三帧还有目标
            TempList3=Trajectory3(:,3);
            TempList2=Trajectory2(:,3);
            for ij=1:NumFrame3 % 找出消失的目标标记
                TempList2(TempList2==TempList3(ij))=[];
            end
        elseif NumFrame3==0 % 第三帧没有目标
            TempList2=Trajectory2(:,3);
        end
        
        NOut=size(TempList2,1); % 消失的目标数目
        for ij=1:NOut
            DStartEnd=norm(Startcoord(:,:,TempList2(ij))-Endcoord(:,:,TempList2(ij))); % 计算消失目标的起始点和消失点的距离
            DA=[DA DStartEnd]; % 保存距离，仅用于测试
            TStart=Startcoord(:,:,TempList2(ij)); % 起始点坐标
            TEnd=Endcoord(:,:,TempList2(ij)); % 消失点坐标
            if DStartEnd>100 && TEnd(1)>200 && TStart(1)<40 % 进的判断
                NumPeopleIn=NumPeopleIn+1;  % 进来的人数统计               
                RI=[RI TempList2(ij)]; % 标识保存，测试用 2013.08.08
                DIN=[DIN DStartEnd]; % 距离保存，测试用 
            end
            if DStartEnd>100 && TEnd(1)<40 && TStart(1)>200 % 出的判断
                NumPeopleOut=NumPeopleOut+1; % 出去的人数统计
                RE=[RE TempList2(ij)]; % 标识保存，测试用 2013.08.08
                DE=[DE DStartEnd]; % 距离保存，测试用
            end
        end
    end
    
    if NumFrame4~=NumFrame3 && NumFrame3==NumFrame5 % 对于第三帧目标数目和第四帧目标数目不同，第三帧目标数目和第五帧目标数目相同，则删除第四张
        Frame{4}=[];  % 删除第四张
        Frame{1}=Frame{2}; % 更新
        Frame{2}=Frame{3};
        Frame{3}=Frame{5};
        Frame{4}=Frame{5};
        
        Trajectory1=Trajectory2; % 更新
        Trajectory2=Trajectory3;
        Trajectory3=[];
        
    else
        Frame{1}=Frame{2}; % 一般情况下的更新
        Frame{2}=Frame{3};
        Frame{3}=Frame{4};
        Frame{4}=Frame{5};
        
        Trajectory1=Trajectory2; % 一般情况下的更新
        Trajectory2=Trajectory3;
        Trajectory3=[];
    end
else
    disp('连续三帧为0，初始化')
    if NumFrame3==0 && NumFrame4==0 && NumFrame5==0 || NumFrame1==0 && NumFrame2==0 && NumFrame3==0  % 统计连续三帧为0的情况，当出现数目大于一定数目时，对某些参数进行初始化
        NumEmpty45=NumEmpty45+1; % 统计连续三帧为0的次数
        if NumEmpty45>10 %　当次数大于一定数值的时候，对Startcoord,Endcoord,CountsStartcoord,CountsStartcoord,NumEmpty45进行更新
            Startcoord=zeros(1,2,100); % 要设置定时更新
            CountsStartcoordIn=0; 
            CountsEndcoordOut=0;
            Endcoord=zeros(1,2,100); % 要设置定时更新
            NumEmpty45=0;
        end
    end
    Frame{1}=Frame{2}; % 更新
    Frame{2}=Frame{3};
    Frame{3}=Frame{4};
    Frame{4}=Frame{5};
    
    Trajectory1=Trajectory2; % 更新
    Trajectory2=Trajectory3;
    Trajectory3=[];
end


    






