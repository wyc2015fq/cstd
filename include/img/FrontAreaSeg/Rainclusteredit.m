% Edit by hudalikm
% Time：2013.06.09
% Second edit time: 2013.07.04
% 这个方法的好处在于它不需要设置太多的参数，针对不同高度的人和不同高度的摄像机也有很好的鲁棒性，
% 参数几乎不用任何调整即可直接使用。缺点就是要求人不能有太大的动作，
% 例如举手等，而且如果拉着拉杆箱也不能处理，会在拉杆箱上有错误检测。如果有兴趣，
% 可以在这方面有所改进。
% 快速算法改进
% 参数有待验证
% 注意某些深陷点的计算，有可能超出边界

function [ClusterI Time_use]=Rainclusteredit(I,RN,RD)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 根据以左为先的原则，赋予不同的权重，减少循环次数 Edit by hudalikm.2013.09.05
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
OW=[8 6 4 2 1 3 5 7;
    7 8 6 4 2 1 3 5;
    5 7 8 6 4 2 1 3;
    3 5 7 8 6 4 2 1;
    1 3 5 7 8 6 4 2;
    2 1 3 5 7 8 6 4;
    4 2 1 3 5 7 8 6;
    6 4 2 1 3 5 7 8];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if ndims(I)==3
    I=rgb2gray(I);
end
[m,n]=size(I);
II=zeros(m+2,n+2);
II(2:m+1,2:n+1)=I;
I=II;
I=double(I);
index=find(I>10); % 找出图像中的非零点
[xq,yq]=ind2sub(size(I),index); % 找出前景点坐标位置
MaxValue=max(I(:)); % 找出图像最大值
FillValue=2*MaxValue+10; % 设置非前景区域填充值(黑色0区域填充值)，目的方便边界处理，相于与构造一个水池的围墙
I(I<10)=FillValue; % 非前景区域填充
sizeforeground=length(xq); % 找出前景点数目
RainNum=50*sizeforeground/25; % 雨滴数目，也就是总的外循环次数
disp(['雨滴数目：',num2str(RainNum)])
g=zeros(size(I)); % 测量函数
t_start=cputime;
NUMCOUNT=0;
for k=1:RainNum
    Rands=ceil(rand*sizeforeground); % 模拟雨滴的随机降落
    xs=xq(Rands); % 随机坐标生成
    ys=yq(Rands);
    W=RN;
    r=RD;
    orietind=zeros(1,8); % 邻域方向
    findt=0; % 用于指示是初始雨滴还是流下的雨滴
%     disp(['xs: ',num2str(xs),'  ys: ',num2str(ys)])
%     disp('')
    if mod(k,10000)==0
        NUMCOUNT=NUMCOUNT+1;
        disp(['第 ',num2str(NUMCOUNT),' 万水滴计算完毕！'])
    end
    while W>0 % 局部寻优
        d(1,8)=0;
        d(1)=I(xs-1,ys-1)+g(xs-1,ys-1)-(I(xs,ys)+g(xs,ys)); % 1
        d(2)=I(xs  ,ys-1)+g(xs  ,ys-1)-(I(xs,ys)+g(xs,ys)); % 2
        d(3)=I(xs+1,ys-1)+g(xs+1,ys-1)-(I(xs,ys)+g(xs,ys)); % 3
        d(4)=I(xs+1,ys  )+g(xs+1,ys  )-(I(xs,ys)+g(xs,ys)); % 4
        d(5)=I(xs+1,ys+1)+g(xs+1,ys+1)-(I(xs,ys)+g(xs,ys)); % 5
        d(6)=I(xs  ,ys+1)+g(xs  ,ys+1)-(I(xs,ys)+g(xs,ys)); % 6
        d(7)=I(xs-1,ys+1)+g(xs-1,ys+1)-(I(xs,ys)+g(xs,ys)); % 7
        d(8)=I(xs-1,ys  )+g(xs-1,ys  )-(I(xs,ys)+g(xs,ys)); % 8
        if findt==0 % 初始雨滴
            Index=find(d==min(d)); % 寻找最d中最小值的索引
            repnum=length(Index); % 最小值的个数
            Flg=Index(ceil(rand*repnum)); % 由于是初始雨滴，那么从最小值中随机挑选一个方向
            Value=d(Flg); % 雨滴流向的最小值
            if Value+r<0 %%%%%%%%%%%%%%%%%%%%%%%%%   减少循环的次数,增加 +r 
                                                 %   Edit by hudalikm.2013.09.05
                if Flg==1 % 方向1坐标跳转 
                    xt=xs-1;
                    yt=ys-1;
                elseif Flg==2 % 方向2坐标跳转 
                    xt=xs;
                    yt=ys-1;
                elseif Flg==3 % 方向3坐标跳转 
                    xt=xs+1;
                    yt=ys-1;
                elseif Flg==4 % 方向4坐标跳转 
                    xt=xs+1;
                    yt=ys;
                elseif Flg==5 % 方向5坐标跳转 
                    xt=xs+1;
                    yt=ys+1;
                elseif Flg==6 % 方向6坐标跳转 
                    xt=xs;
                    yt=ys+1;
                elseif Flg==7 % 方向7坐标跳转 
                    xt=xs-1;
                    yt=ys+1;
                elseif Flg==8 % 方向8坐标跳转 
                    xt=xs-1;
                    yt=ys;
                end
                xs=xt; % 坐标跳转
                ys=yt;
%                 findt=findt+1;
            else
                g(xs,ys)=g(xs,ys)+min(r,W);
                W=W-r;
%                 findt=findt+1;
            end
        else % 非初始雨滴
            td=d==min(d);
            Index=find(td); % 找出最小值所在的方向
            repnum=length(Index); % 找出最小值的个数
            NSFlg=Index; % 最小值方向保存
            Indicate=find(NSFlg==Flg); % 找出上一步最小值方向相同的最小值方向，其中Flg是上一步的方向
            DirectInd=0; % 用于指示最小值方向之值是否为0
            if ~isempty(Indicate) % 如果有与上一步最小值方向相同的方向，优先选择该方向
                Flg=NSFlg(Indicate); % 找出最小值方向
                Value=d(Flg); % 找出最小值
                if Value==0 %　如果最小值等于０，则DirectInd=1
                    DirectInd=1;
                end
            elseif repnum>1 % 如果与上一部方向不同，且最小值个数不止一个的时候，则随机选取一个方向作为流动方向(0和小于0的情况是否考虑)
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % 方向不同，以左为先，Edit by hudalikm.2013.09.05
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % 将代码
                %                 Flg=Index(ceil(rand*repnum)); % 随机选取方向
                %                 Value=d(Flg); % 对应最小值方向的最小值
                % 修改为
                tod=OW(Flg,:).*td; 
                [Vtod Flg]=max(tod);
                Value=d(Flg);
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            else % 与上一步方向不同，而且最小值只有一个
                [Value,Flg]=min(d);
            end
            if DirectInd~=1 % 假如最小值不是0
                if Value+r<0 % 那么当最小值小于0时，执行坐标跳转 %%%%%%%%%%%%%%%%%% 减少循环的次数，增加 +r
                                                                               % Edit by hudalikm.2013.09.05
                    if Flg==1
                        xt=xs-1;
                        yt=ys-1;
                    elseif Flg==2
                        xt=xs;
                        yt=ys-1;
                    elseif Flg==3
                        xt=xs+1;
                        yt=ys-1;
                    elseif Flg==4
                        xt=xs+1;
                        yt=ys;
                    elseif Flg==5
                        xt=xs+1;
                        yt=ys+1;
                    elseif Flg==6
                        xt=xs;
                        yt=ys+1;
                    elseif Flg==7
                        xt=xs-1;
                        yt=ys+1;
                    elseif Flg==8
                        xt=xs-1;
                        yt=ys;
                    end
                    xs=xt;
                    ys=yt;
%                     findt=findt+1;
                else % 当最小值大于0时衡量函数g改变
                    g(xs,ys)=g(xs,ys)+min(r,W);
                    W=W-r;
%                     findt=findt+1;
                end
            else % 当最小值为0(但是只是针对与上一步方向相同的0值处理，与上一步方向不同的0值处理则不在此列)
                    if Flg==1
                        xt=xs-1;
                        yt=ys-1;
                    elseif Flg==2
                        xt=xs;
                        yt=ys-1;
                    elseif Flg==3
                        xt=xs+1;
                        yt=ys-1;
                    elseif Flg==4
                        xt=xs+1;
                        yt=ys;
                    elseif Flg==5
                        xt=xs+1;
                        yt=ys+1;
                    elseif Flg==6
                        xt=xs;
                        yt=ys+1;
                    elseif Flg==7
                        xt=xs-1;
                        yt=ys+1;
                    elseif Flg==8
                        xt=xs-1;
                        yt=ys;
                    end
                    xs=xt;
                    ys=yt;
%                     findt=findt+1;
            end
        end
        findt=findt+1; % 标记当前雨滴寻优的步数，用于指示是否是初始雨滴
        %         disp(['第 ',num2str(findt),'个循环'])
    end
    %         disp(['第 ',num2str(k),'个水滴计算完毕'])
end
end_time=cputime-t_start;
disp(['消耗时间：',num2str(end_time),' second,大约为：',num2str(round(end_time/60)),' 分钟!'])
ClusterI=g;
Time_use=end_time;
end

