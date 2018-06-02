%
clear all
clc

use_haar = 1;
if use_haar ==1
    XX1 = load('E:\luobo\参考论文\AdaBoost\boost - 副本\haar+adaboost+cascade\F_Haar_0_20_4000_0104.mat');
    X1 = XX1.FEA(1:1000,:);
    % clear XX1;
    XX0 = load('E:\luobo\参考论文\AdaBoost\boost - 副本\haar+adaboost+cascade\F_Haar_neg_1-4226_0104.mat');
    X0 = XX0.FEA(1:3000,:);
    %clear XX0;
else
    XX1 = load('E:\luobo\download_files\piotr_toolbox_luobo\toolbox\channels\feature channels\F_pitor_left_0_20_4000.mat');
    X1 = XX1.FEA;
    XX0 = load('E:\luobo\download_files\piotr_toolbox_luobo\toolbox\channels\feature channels\F_pitor_neg659_8080_15445(1_5000).mat');
    X0 = XX0.FEA;
end
clear XX1 XX0;
X1 = double(X1);
X0 = double(X0);
Y = zeros(1,size(X1,1)+size(X0,1));
Y(1:size(X1,1)) = 1;

T = 500;%需要事先指定分类器个数
tic;
%
disp(['开始训练',num2str(T),'个弱分类器！！']);

pBoost=struct('nWeak',T,'verbose',1,'discrete',1,'pTree',struct('maxDepth',2));%应该自己指明pTree的类型
model = adaBoostTrain( X0, X1, pBoost);


%[Hypothesis,AlphaT,trainErrorRate,costTime,TPRate,FPRate]=trainAdaBoostLearner(X,Y,T);%利用AdaBoost训练出T个弱分类器
%[Hypothesis,left_Hypothesis,AlphaT,c]=new_Adaboost(X,Y,T);
%disp([num2str(T),'个弱分类器','训练结束！！']);



toc;

%备下份
MODEL = model;

disp('开始进行soft级联！！');

%function  [c_qt,r_t,F,D] = soft_cascade(X,Y,V,C)
%
%目的在于得到每一级弱分类器的排列顺序q_t、门限值r_t
%
% 首先利用给定的正负样本，调用AdaBoost，生成C{c1,c2,c3...cT}
% 将C里元素排序，赋门限值，则按顺序排列的c就构成强分类器。
% C{c1,c2,c3...cT}中
%ct=[Hypothesis,left_Hypothesis,right_Hypothesis,AlphaT,left_AlphaT,right_AlphaT]
%(深度2的决策树：根节点，左右子节点以及响应的权值（我没有用权值）);
%
%
%按顺序存储选取的每级弱分类器
H1 = zeros(T,3);
%l_H1 = zeros(T,3);
%r_H1 = zeros(T,3);
AT1 = zeros(T,1);
%l_T1 = zeros(T,1);
%r_T1 = zeros(T,1);

%初始化
a1 = sum(Y==0);%负样本个数
b1 = sum(Y==1);%正样本个数
N1 = a1+b1;%样本个数

P = b1;
N = a1;

%T = length(C); %本论文使用2048.
%T = 2048;
d = zeros(N1,1);% N*T，每t列是每个样本通过前t个分类器的最终响应！（直接就是累加和就行了，不用记录每一级的累加和）
p = 0; % 人脸拒绝分数
m = 0; %执行时间
A = 0; %目前为止用的负样本总数，重复使用的也计数,初值定为0
nn = 0;
nnn = 0;

q = zeros(T,1);
r = zeros(T,1);
Q = zeros(T,1);
QQ = zeros(T,T);tic;
V = zeros(1,T);
boostrp_flag = 0;
%开始训练soft级联，求解T个弱分类器排列顺序，以及每个弱分类器的阈值。
for t = 1:T %通过训练分类器的个数T为循环结束条件，也可以使用其他条件，如最终检测率或最终误检测率
    t_qt = 0;%记录计算时间

    tic

    V(t) = 0.01;%每一级允许的最大人脸漏检率,
    %根据soft论文给定的算法来求解，给定k值（k值保证v向量元素之和等于1-D，因此需要根据D,T和a值，提前计算出来），alpha值，
    %if a<0
    %   V(t) = k*exp(-alpha*(1-t/T));
    %else
    %   V(t) = k*exp(alpha*t/T);
    %end

    p = p+V(t); %累加漏检率，漏检的人脸数可以累加，比如每一级规定最多漏检10个人脸，第1级漏检了8个，则第二级可以漏检12个，以此类推。？？可以累加（这是论文中说的）
    a(t) = sum(Y==0);%第t级的负样本数
    b(t) = sum(Y==1);
    %N(t) = a(t) + b(t); %第t级的总样本数

    A = A+a(t);

    %Q = zeros(T,1);
    Q(t) = 0;
    %Q_t = -1000;
    %q_t = 1;
    ar_1 = 0;
    ar_2 = 0;
    tt =T-t+1;%控制求解q_t的循环次数，第t轮，分类器集合里还剩下tt个分类器，故循环tt次，找出最佳分类器（下标为q_t）
    %if t==1
    %    d(:,1) = 0;
    %else
    % for jj=1:t-1
    %   c(jj,:) = tree2(H1(jj,:),l_H1(jj,:),r_H1(jj,:),...
    %      AT1(jj),l_T1(jj),r_T1(jj),X,Y);
    %   d(:,jj) = d(:,jj)+c(jj,:)';%不能这样求解d，因为每一级的响应样本会发生改变，不能直接for循环。应该在大循环中每次求解！！下标也错误！！
    % end
    %end
    d = d(1:P+N,1);
    %该循环用来求解q_t
    for j = 1:tt

        ids(1:size(X1,1),:) = forestInds(X1,model.thrs(:,j),model.fids(:,j),model.child(:,j),1e5);
        ids(size(X1,1)+1:size(X1,1)+size(X0,1),:) = forestInds(X0,model.thrs(:,j),model.fids(:,j),model.child(:,j),1e5);
        %ids是叶子节点的编号
        hs =  model.hs(ids,j);

        f_tij = d + hs;
        clear ids hs;
        ar_1 = sum(f_tij.*Y')/b(t);
        ar_2 = sum(f_tij.*(1-Y)')/a(t);
        QQ(t,j) = ar_1 - ar_2;

        if j==1 %如此为Q(t)赋初值，比直接赋初值靠谱些
            Q(t) = QQ(t,j);
            q_t = 1;
        end
        if Q(t) < QQ(t,j)  %Q_t < Q(j,:) %找出最大的结果
            Q(t) = QQ(t,j); %Q_t = Q(j,:);
            q_t = j;%该处有问题！！！！q_t应是原来的编号。（该编号是为了找到分类器，后面紧接着就利用q_t,提取出了这一步的最佳分类器，所以是不是初始分类器集合中的编号，无所谓！）
        end

    end

    % toc;

    q(t) = q_t;%存储每一级在集合里的序号（存不存无所谓！没有真正的意义）

    disp(['第',num2str(t),'个分类器选取的特征索引号为：',num2str(model.fids(1,q_t)),',',num2str(model.fids(2,q_t)),',',num2str(model.fids(3,q_t))]);

    %按顺序存储每一级弱分类器
    Model.fids(:,t) = model.fids(:,q_t);
    Model.thrs(:,t) = model.thrs(:,q_t);
    Model.child(:,t)= model.child(:,q_t);
    Model.hs(:,t) = model.hs(:,q_t);
    Model.weights(:,t) = model.weights(:,q_t);
    Model.depth(:,t) = model.depth(:,q_t);


    %t_qt = time;
    %更新样本响应

    ids(1:size(X1,1),:) = forestInds(X1,model.thrs(:,q_t),model.fids(:,q_t),model.child(:,q_t),1e5);
    ids(size(X1,1)+1:size(X1,1)+size(X0,1),:) = forestInds(X0,model.thrs(:,q_t),model.fids(:,q_t),model.child(:,q_t),1e5);
    hs =  model.hs(ids,q_t);

    %注意此处更新样本响应时，应该更新d的size。
    d = d + hs;
    clear ids hs;

    if 0
        %求解第t级对应的门限值r_t
        %更新阈值时，采用的方法与保证正样本通过个数的方法类似
        %该阈值应该不是之前认为的特征值的变化

        % tempH(i)=sum(AlphaT.*h);
        % 这里的d应该相当于 tempH，所以d应该是矩阵，行数为样本数
        % sorttemp=sort(tempH(1:600)); %对600个正样本进行排序
        % 应对d进行排序，按照失去规定的正样本数，确定门限值。（可以反过来，计算保证通过的正样本数，得到阈值）


        %最多允许误判的人脸数为：p*b(t),则要求正确判定的人脸最少为（1-p）*b(t);
        %num(t) = b(t)-floor(p*b(t)); %每一级要求通过的最少人脸数
        num(t) = floor(p*b(t)); %每一级允许漏检的最大人脸数，非负数
        %sorttemp=sort(d(1:b(t)),'descend'); %对所有的人脸响应值进行降序排序
        sorttemp=sort(d(1:b(t)),'ascend'); %对所有的正样本（人脸）响应值进行升序排序

        %为r_t赋初值
        if num(t)>0
            r_t = sorttemp(num(t)); %保证通过的最少人脸数 %直接使用最多误判人脸数，也保证了该门限值也是满足条件（误判人脸数至多为p*b(t)）的门限值中最大的
        else
            r_t =  sorttemp(1)-0.0001;%让门限值比最小的阈值还小
            %r_t =  sorttemp(b(t));%让门限值为最小的正样本阈值
        end

        pred = (d <=r_t).*Y'; %统计响应值小于阈值r_t的样本
        %pred = (d >=r_t).*Y';

        %确定最终的r_t值
        %调整门限值，在保证不等式成立的情况下，r_t取值最大
        i=num(t);
        while(sum(pred) > num(t))
            i=i-1;
            if i<=0
                r_t =  sorttemp(1)-0.0001;%对于不去掉人脸的情况，这时门限值可以这样赋值吗？？
                %r_t =  sorttemp(b(t));
                break;
            else
                r_t = sorttemp(i);
            end
            pred = (d <=r_t).*Y';
        end

        r(t) = r_t;

        p = p-sum((d <=r_t).*Y')/b(t); %更新允许去掉人脸的分数
        if p<0 %漏检率应为非负数，其实理论上讲，p没有负值的可能，可在实际计算中，会出现负值。（应该是计算有问题，应该修改！！）
            p=0;
        end


        %sum_pred = 0;
        %r_t = X(1,Hypothesis(q_t,3));
        %R = X(:,Hypothesis(q_t,3)); %阈值比较
        %for j = 1:N
        %   pred = d<=R(j);
        %   sum_pred = sum(pred.*Y');
        %if sum_pred < p*b
        %    if r_t < R(j)
        %        r_t = R(j);
        %    end
        %end
        %end
    end

    sorttemp=sort(d(1:b(t)),'ascend');
    r_t = (sorttemp(ceil(b(t)*0.01)-1)+sorttemp(ceil(b(t)*0.01)))/2;
    %sorttemp=sort(d(1:b(t)),'descend');
    r(t) = r_t;
    clear sorttemp;

    %更新未排序弱分类器集合,去掉已选取的弱分类器
    model.fids(:,q_t) = [];
    model.thrs(:,q_t) = [];
    model.child(:,q_t) = [];
    model.hs(:,q_t) = [];
    model.weights(:,q_t) = [];
    model.depth(:,q_t) = [];

    % m = m+t_qt*a(t);%更新计算总时间
    m = m+1*a(t);%论文中说时间函数为常数1

    output = d < r(t);%前t级弱分类器的最终输出
    output = ~output;
    %为什么output总是为1？？r(t)的得出不是会保证正样本的通过率，但负样本没有定义，每次负样本的响应怎么都会大于选取的门限值？？偶然？？

    TRUE(t) = sum(output'.*Y)/P;
    FALSE(t) = sum(output'.*(~Y))/N;
    DD(t) = sum(output'.*Y)/b1;
    FF(t) = sum(output'.*(~Y))/a1;
    %disp(['第',num2str(t),'个分类器分类正确率为：',num2str(TRUE(t)),'！！  ','False Positive率为：',num2str(FALSE(t)),'！！']);
    disp(['           分类正确率为：',num2str(TRUE(t)),'！！  ','False Positive率为：',num2str(FALSE(t)),'！！']);
    disp(['前',num2str(t),'个分类器分类正确率为：',num2str(DD(t)),'！！  ','FP率为：',num2str(FF(t)),'！！']);
    disp(['通过的正样本个数为：',num2str(sum(output'.*Y)),'！',]);
    disp(['现有负样本个数为：',num2str(N),'通过的负样本个数为：',num2str(sum(output'.*(~Y))),'！  ']);
    fprintf('\n');
    %这里更新样本，也应该注意，每级输出累加和的size

    %X = X(~mark,:); %去掉d<r(t)的样本，正负样本只要满足条件。
    X1 = X1(output(1:P),:);
    X0 = X0(output(1+P:N+P),:);
    Y = Y(output);
    d = d(output); %更新d的size.

    clear output;

    if boostrp_flag == 0
        if isempty(X0)
            disp('负样本为空！！训练结束！！');
            break;
        end
    else
        if t>2
            nn = nn+1;
            XX0  = load ('E:\luobo\download_files\piotr_toolbox_luobo\toolbox\channels\feature channels\F_pitor_neg659_8080_15445(5001_10000).mat');
            if 1000+(nn-1)*1000 <= size(XX0.FEA,1)
                X00 = XX0.FEA(1+(nn-1)*1000:1000+(nn-1)*1000,:);
                boostrp_flag = boostrp_flag +1;
            else
                clear XX0;
                nnn = nnn+1;
                XX0  = load ('E:\luobo\download_files\piotr_toolbox_luobo\toolbox\channels\feature channels\F_pitor_neg659_8080_15445(10001_15000).mat');
                if 1000+(nnn-1)*1000 < size(XX0.FEA,1)
                    X00 = XX0.FEA(1+(nnn-1)*1000:1000+(nnn-1)*1000,:);
                    boostrp_flag = boostrp_flag +1;
                else
                    X00 = XX0.FEA(1+(nnn-1)*1000:1000+(nnn-1)*1000,:);
                    boostrp_flag = 1;
                end

            end
            clear XX0;
            X00 = double(X00);
            HS_x = 0;
            for nweak = 1:size(Model.fids,2)
                ids = forestInds(X00,Model.thrs(:,nweak),Model.fids(:,nweak),Model.child(:,nweak),1e5);
                hs =  Model.hs(ids,nweak);
                HS_x = HS_x+hs;
            end
            d = [d;HS_x];
            Y = [Y,zeros(1,size(X00,1))];
            X0 = [X0;double(X00)];
            %num_F(t) = num_F(t)+ size(X00,1);
            %num_FF = num_FF+ size(X00,1);
            a1 = a1 + size(X00,1);
            clear X00;
            boostrp_flag = boostrp_flag-1;
            %F(t+1) = 1;
            %FF(t+1) = num_F(t)/num_FF;
            %break;
        end
    end
    P = sum(Y==1);
    N = sum(Y==0);



end
toc;

if t<T
    q = q(1:t);

    Model.fids = Model.fids(:,1:t);
    Model.thrs = Model.thrs(:,1:t);
    Model.child= Model.child(:,1:t);
    Model.hs = Model.hs(:,1:t);
    Model.weights = Model.weights(:,1:t);
    Model.depth = Model.depth(:,1:t);
end

[selectFeaSequence,hypoFeaList,overlapNum] = analysisHypothesisFeature_soft( Model);
disp(['选择的特征总数：',num2str(length(hypoFeaList(:,1)))]);
%disp(['重复特征个数：',num2str(overlapNum)]);
disp(['选择的特征为：',num2str(selectFeaSequence)]);



