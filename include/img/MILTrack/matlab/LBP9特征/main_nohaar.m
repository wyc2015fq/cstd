clc;clear;close all
%此函数与用haar特征的MIL的函数main函数有不同，不同的地方我用%￥￥￥￥￥￥￥￥￥￥￥标记出

%影响跟踪精度的主要参数为M、K(尤为重要)、正样本数、搜索样本数、学习率v(通过把学习率调节到接近1，如0.99，可大幅度提高跟踪质量，但这样会使特征均值和标准差的更新速度变慢，可能不利于表面在很短时间内变化较大的情况)
if_face_detect = 0;%是否进行人脸检测，为0则不进行人脸检测，用感兴趣区域选择函数
if_replace_clf = 0;%是否替换(分类能力差的)分类器,为0则不替换

work_path = pwd;
d1 = 'test_pic\*.bmp';
d2 = dir(d1);
num_pic = size(d2,1);%图片个数
M = 250;%候选特征数(候选分类器数)%￥￥￥￥￥￥￥￥￥￥￥
K = 60; %选出特征数(选出分类器数)%￥￥￥￥￥￥￥￥￥￥￥
beita1 = 50;%随机产生负样本用到的距离
beita2 = 80;%随机产生负样本用到的距离
gama  = 5; %随机产生正样本用到的距离
s = 30;%候选样本区域,要>=gama,且最好保证s > (num_fix_cr/10)/2
num_neg = 65;%负样本数
num_pos = 45;%正样本数
num_candidate_result =500;%搜索样本数,要>=num_fix_cr
num_fix_cr = 300;%要在原位置附近固定的样本数,目前取值为100、200和300，见函数randomSelectSample_for_result.m


MIU1_last_f = zeros(1,M);DELTA1_last_f = zeros(1,M);%高斯分布均值和标准差
MIU2_last_f = zeros(1,M);DELTA2_last_f = zeros(1,M);
v = 0.95;%学习率,0.85~1,v越小特征均值和标准差就更新得越快，v = 1则不更新特征均值和标准差，对于表面不变或者变化很缓慢的对象v应该取到接近1

for t = 1:num_pic
    tic
    pic_path = strcat(work_path,'\test_pic\',num2str(d2(t).name));%测试图片所在目录
    I = imread(pic_path);
    I_rgb=I;
    if ndims(I)==3,I = rgb2gray(I);end
    [pic_h,pic_w] = size(I);


    %     if t == 1%第一帧只用当前位置一个正样本
    %         tempnum_neg = num_neg;
    %         tempnum_pos = 1;
    %     else%用num_pos个正样本
    tempnum_neg = num_neg;
    tempnum_pos = num_pos;
    %     end
    totalnum = tempnum_neg + tempnum_pos;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % step1:检测人脸以及随机选择M个特征 %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if t==1
        if if_face_detect ==1
            fp1 = fopen('face_detect\face_detect.bat','w');
            fprintf(fp1,'%s\n','@echo off');
            path_for_bat = strcat('..\test_pic\',num2str(d2(t).name));
            fprintf(fp1,'%s %s','face_detect.exe',path_for_bat);
            fclose(fp1);

            cd(strcat(work_path,'\face_detect'));
            system('face_detect.bat');
            %clc;
            cd(work_path);

            fp2 = fopen('face_detect\facePoints.txt','r');
            facePoints = fscanf(fp2,'%f');%此为c++坐标系
            fclose(fp2);
            clc
            if facePoints(1) == -1,disp('第一帧没有检测到任何人脸，程序退出');break;end

            x1_f = facePoints(1);y1_f = facePoints(2);
            x3_f = facePoints(3);y3_f = facePoints(4);
            y1_f = y1_f - abs((y3_f - y1_f)/2);%扩大检测到的人脸(可以去掉)
            x2_f = x3_f;y2_f = y1_f;
            x4_f = x1_f;y4_f = y3_f;
            %imshow(I_rgb),axis on;
            X1 = round(x1_f+1);X2 = round(x2_f+1);X3 = round(x3_f+1);X4 = round(x4_f+1);%(X1,Y1)为矩形左上角点坐标
            Y1 = round(y1_f+1);Y2 = round(y2_f+1);Y3 = round(y3_f+1);Y4 = round(y4_f+1);
            Xm = round(((x2_f+1) + (x1_f+1))/2);%检测出来的矩形框的中心位置

            %Ym = round(((y3_f+1) + (y1_f+1))/2);
            %hold on
            %plot([X1;X2;X3;X4;X1],[Y1;Y2;Y3;Y4;Y1],'r');%人脸检测矩形框
            %plot(Xm,Ym,'r.', 'MarkerSize',10);
            %hold off
            %rusult_path=strcat('E:\result\',strtok(num2str(d2(t).name),'.'),'.jpg');%结果形状
            %f=getframe;f_r=f.cdata;
            %imwrite(f_r,rusult_path);
            %pause(0.1);
            plot_location(X1,Y1,X3,Y3,I_rgb,d2,t,1);%以下为保存跟踪结果，1表示红色
        else
            [Y1,X1,Y3,X3] = select_ROI(I_rgb);
            pause(0.1);
            close all;
        end

        patch_pos_sample = I(Y1:Y3,X1:X3);%第一帧的正样本
        [sample_h,sample_w] = size(patch_pos_sample);

        [MFeatures,NineBlocks_MFeatures] = randomMFeatures(sample_w,sample_h,M,K);%随机产生M个特征(类型、位置、尺度)，%￥￥￥￥￥￥￥￥￥￥￥
    end

    %%%%%%%%%%%%%%
    % step2:分类 %
    %%%%%%%%%%%%%%
    if t>=2

        YX_candidate = randomSelectSample_for_result(X1,Y1,sample_w,sample_h,pic_w,pic_h,s,num_candidate_result,num_fix_cr);
        [X1_result,Y1_result] = compute_reult(YX_candidate,sample_w,sample_h,SelectKFeatures,I,SelectMIU1,SelectDELTA1,SelectMIU2,SelectDELTA2);

        X1 = X1_result;
        Y1 = Y1_result;
        X3 = X1 + sample_w - 1;
        Y3 = Y1 + sample_h - 1;

        plot_location(X1,Y1,X3,Y3,I_rgb,d2,t,2);%以下为保存跟踪结果，2表示绿色

    end

    %%%%%%%%%%%%%%%%%%%%%
    % step3:随机产生样本 %
    %%%%%%%%%%%%%%%%%%%%%
    [YX_neg,YX_pos] = randomSelectSample(X1,Y1,sample_w,sample_h,pic_w,pic_h,beita1,beita2,gama,tempnum_neg,tempnum_pos,-1);%YX_neg,YX_pos只记录样本矩形框的左上角坐标，若第一帧和后面帧一样都不只用一个正样本，则此处是用-1代替原来的t

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % step4:计算每一个样本对应的M个候选弱分类器 %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    [h_candidateWeakClf,MIU1,DELTA1,MIU2,DELTA2] = candidateWeakClf(YX_neg,YX_pos,sample_w,sample_h,MFeatures,I,MIU1_last_f,DELTA1_last_f,MIU2_last_f,DELTA2_last_f,v,t);%￥￥￥￥￥￥￥￥￥￥￥，此函数内部有不同
    MIU1_last_f    = MIU1;
    DELTA1_last_f  = DELTA1;
    MIU2_last_f    = MIU2;
    DELTA2_last_f  = DELTA2;


    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % step5:从M个候选弱分类器中选择K个最佳弱分类器 %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    L_ind = SelectFeatures(h_candidateWeakClf,NineBlocks_MFeatures,tempnum_neg,tempnum_pos,K,M);%￥￥￥￥￥￥￥￥￥￥￥

    for k =1:size(L_ind,1)
        SelectKFeatures(k,1).StPos = MFeatures(L_ind(k),1).StPos;%￥￥￥￥￥￥￥￥￥￥￥
        SelectMIU1(1,k)            = MIU1(1,L_ind(k));
        SelectDELTA1(1,k)          = DELTA1(1,L_ind(k));
        SelectMIU2(1,k)            = MIU2(1,L_ind(k));
        SelectDELTA2(1,k)          = DELTA2(1,L_ind(k));
    end
    toc
end
