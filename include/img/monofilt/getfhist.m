
% function PH=getfhist(I)
%I为一幅图片，输出为最终直方图
I=imread('3.bmp');
%I1=imresize(I,[50,100]);
%I=imresize(I,[100 100]);
[f, h1f, h2f, A, theta, psi] = monofilt(I, 3, 4, 2, 0.65, 0);
%[f, h1f, h2f, A, theta, psi] = monofilt(I, 3, 4, 2, 0.65, 0);



MAPPING=getmapping(8,'u2');
fimal=cell(3,1);
for sc=1:3;
    A{sc}=imresize(A{sc},[52 102]);
    LBPMAP=lbp(A{sc},1,8,MAPPING,' ');%获得局部二值化后的图像
  %  LBPMAP=imresize(LBPMAP,[90 100]);
    theta{sc}=thtod(theta{sc});
    final{sc}{1}=double(LBPMAP)+theta{sc};%%获得其中一个模式的最终MBP图6*372*60
    
 %   final{sc}{1}=imresize(final{sc}{1},[90 100]);
 fimal{sc}{1}=oversample(final{sc}{1});
end

for t=1:3

    for p=1:372
        PH(1,((372*(t-1)+p-1)*60+1):((372*(t-1)+p)*60))=zhifang60(fimal{t}{1}{p});
         
    end
   
end



% subplot(1,3,1),imshow(mat2gray(final{3}{1}));
% subplot(1,3,2),imshow(mat2gray(final{2}{1}));
% subplot(1,3,3),imshow(mat2gray(final{1}{1}));
%%%%%%%%%%%%%%%%%%%%%%%%%%%对每个模式的最终MBP图分块


% B=cell(5,5);
% B=fenkuai(final{sc}{1},5,5);
% hist=cell(5,5);
% for i=1:5
%     for j=1:5
%         hist{i}{j}=zhifang(B{i}{j});%%算出每一块的直方图存入元胞hist
%     end
% end
% PEAH=cell(3,1);
% for sc=1:3;
%     evbh=cell(5,5);
%     evbh=toBeaH(final{sc}{1},5,5);%把某个模式图分成5*5块，计算其每个块的直方图,存入元胞evbh
%     PEAH{sc}{1}=evbh;%PEAH含有3个元胞，每个元胞里有25个小元胞，分别存储25个块的直方图
% end
% %%%%所有直方图串联，串联方式为3张第n块的直方图在一起
% 
% PH=zeros(1,19200);
% 
%     for i=1:5
%         for j=1:5
%             for  k=1:3
%                 PH(1,(256*(i-1)*5*3+256*(j-1)*3+(k-1)*256+1):(256*(i-1)*5*3+256*(j-1)*3+k*256))=PEAH{k}{1}{i}{j};
%                 %PH 为一幅图片最终直方图 19200
%             end
%         end
%     end
          