clear all
clc
filter = filterBinomial1d( 1 ); % 半径r=1的1维二项式滤波器 
    filter2 = filter*filter';% 半径r=1的2维二项式滤波器

    bin_size = 4;
    nbin=6;
    sample_factor = 4;


if 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%直接读取图片，提取特征
    path1 = 'E:\luobo\databases\AFLW\AFLW归一化人脸库\left_face\imface_0_20\';
    tic;
    for i = 1:4000

        path2 = strcat(num2str(i),'.bmp');
        %img = imread(strcat(path1,path2));
        img = imread('1.bmp');

        [h,grad_m,luv] = getfeature_10(img,bin_size,nbin,sample_factor); 
         
        fea(:,:,1:3) = luv;
         fea(:,:,4) = grad_m;
         fea(:,:,5:10) = h; 
         
        %FEA(i,:) = [luv(:)',grad_m(:)',h(:)'];
        FEA(i,:) = fea(:)';
        clear h grad_m luv img;
        %c = fprintf(fid,'%.5f ',fea(:)');
        %c = fprintf(fid,'\n');
        fea = [];
    end
    %fclose('all');
   %toc;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end
save F_left_0_20_4000.mat FEA;












