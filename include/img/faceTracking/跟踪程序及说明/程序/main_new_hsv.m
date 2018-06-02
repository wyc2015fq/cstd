clc;clear;

t=200;%图片帧数
A=25;%最大迭代次数
B=10;%扩大窗口范围,若运动过快且窗口太小则很可能使M00=0;
B1=5;
D=20;%环形区域尺寸,随背景的变化而取不同的值，D越大为零的像素点就越多,注意D若过大则probmap图像像素为全零
F=500;%更新直方图时的阈值


%(1)通过检测人脸求出初始人脸大小(开始)-----------------------------------
    N=2;%初始检测到N张人脸

    y_min(1)=150;
    y_max(1)=210;
    x_min(1)=145;
    x_max(1)=190;

%     D=0.5*(y_max(1)-y_min(1));
%     B=0.5*(y_max(1)-y_min(1));
%     B1=0.5*(y_max(1)-y_min(1));

    y_min(2)=185;
    y_max(2)=229;
    x_min(2)=250;
    x_max(2)=286;

    Y_CEN=zeros(10,N);X_CEN=zeros(10,N);
    for i=1:N
        y1_center(i)=(y_max(i)+y_min(i))/2;
        x1_center(i)=(x_max(i)+x_min(i))/2;

        y_center_qian(i)=y1_center(i);
        x_center_qian(i)=x1_center(i);

        S(i)=(y_max(i)-y_min(i))*(x_max(i)-x_min(i));%//目标区域面积
        Y_CEN(1,i)=y_center_qian(i);
        X_CEN(1,i)=x_center_qian(i);

    end
%   通过检测人脸求出初始人脸大小(结束)-----------------------------------


%(2)初始化第一帧目标颜色直方图(开始)------------------------------------
    I=imread('E:\pic\duo\1.bmp');
    for i=1:N
        histogram_h(:,1,i)=my_histogram_h(y_min(i),y_max(i),x_min(i),x_max(i),I,D);%//histogram_h:256*1*1
        histogram_s(:,1,i)=my_histogram_s(y_min(i),y_max(i),x_min(i),x_max(i),I,D);
        histogram_v(:,1,i)=my_histogram_v(y_min(i),y_max(i),x_min(i),x_max(i),I,D);

        linshi1(:,1,i)=histogram_h(:,1,i);%//更新直方图时用到
        linshi2(:,1,i)=histogram_s(:,1,i);
        linshi3(:,1,i)=histogram_v(:,1,i);
    end
%   初始化第一帧目标颜色直方图(开始)------------------------------------


tic;
%(3)【在线更新】多人camshift跟踪(开始)----------------------------------
    for k=2:t
        k
        for  ii=1:N
            %求概率分布图像(开始)---------------------------
            filename = sprintf('E:/pic/duo/%.1d.bmp',k);
            I1=imread(filename);
            I1_hsv=rgb2hsv(I1);

            h=I1_hsv(:,:,1);s=I1_hsv(:,:,2);v=I1_hsv(:,:,3);
            h=h*255;s=s*255;v=v*255;
            h=uint8(h);s=uint8(s);v=uint8(v);
            [m,n]=size(h);
            probmap_h = zeros(m,n);probmap_s = zeros(m,n);probmap_v= zeros(m,n);
            for i=1:m
                for j=1:n
                        probmap_h(i,j)=histogram_h(h(i,j)+1,1,ii);
                        probmap_s(i,j)=histogram_s(s(i,j)+1,1,ii);
                        probmap_v(i,j)=histogram_v(v(i,j)+1,1,ii);
                end
            end
            probmap=probmap_h.*probmap_s.*probmap_v;
            probmap=bwmorph(probmap,'dilate');
            %figure,imshow(probmap),axis on;
            %求概率分布图像(结束)---------------------------


            %迭代求重心(开始)------------------------------
            count=0;
            rowcenter = 0;
            colcenter = 0;
            rowcenterold = 10;
            colcenterold = 10;

            aa=y_min(ii);bb=y_max(ii);cc=x_min(ii);dd=x_max(ii);
            gao=bb-aa;kuan=dd-cc;

            %while  (((abs(rowcenter - rowcenterold) > 1) && (abs(colcenter - colcenterold) > 1)) || (count < A))
            %while  (((abs(rowcenter - rowcenterold) > 1) && (abs(colcenter - colcenterold) > 1)))
            while  (count < A)%//只用count < A一个条件可以防止进去局部极小值，缺点是可能增加了运算量
                    ymin=floor(aa-B);ymax=floor(bb+B1);xmin=floor(cc-B);xmax=floor(dd+B);%//扩大搜索窗口
                    rowcenterold = rowcenter;colcenterold = colcenter;

                    if ymin<1,a=ymax-ymin;ymin=1;ymax=ymin+a;if ymax>m,ymax=m;end;end%//防止越界
                    if ymax>m,b=ymax-ymin;ymax=m;ymin=ymax-b;if ymin<1,ymin=1;end;end
                    if xmin<1,c=xmax-xmin;xmin=1;xmax=xmin+c;if xmax>n,xmax=n;end;end
                    if xmax>n,d=xmax-xmin;xmax=n;xmin=xmax-d;if xmin<1,xmin=1;end;end

                    [rowcenter colcenter M00 probmap] = zhongxin(ymin,ymax,xmin,xmax,probmap,I1,linshi1(:,1,ii),linshi2(:,1,ii),linshi3(:,1,ii));

                    aa = floor(rowcenter - gao/2);%//加上检测后此处的gao和kuan应改为人脸目标的实际尺寸
                    bb = floor(rowcenter + gao/2);
                    cc = floor(colcenter - kuan/2);
                    dd = floor(colcenter + kuan/2);

                    count = count + 1;
            end
            %迭代求重心(结束)------------------------------

            if abs(rowcenter-y_center_qian(ii))>60||abs(colcenter-x_center_qian(ii))>60%//防止迭代后中心坐标误差过大
                rowcenter=y_center_qian(ii);
                colcenter=x_center_qian(ii);

                ymin=floor(rowcenter-(ymax-ymin)/2);
                ymax=floor(rowcenter+(ymax-ymin)/2);
                xmin=floor(colcenter-(xmax-xmin)/2);
                xmax=floor(colcenter+(xmax-xmin)/2);
            end

            aa = floor(rowcenter - gao/2);
            bb = floor(rowcenter + gao/2);
            cc = floor(colcenter - kuan/2);
            dd = floor(colcenter + kuan/2);

            ymin=floor(aa-B);ymax=floor(bb+B1);xmin=floor(cc-B);xmax=floor(dd+B);

            if ymin<1,a=ymax-ymin;ymin=1;ymax=ymin+a;if ymax>m,ymax=m;end;end%防止越界
            if ymax>m,b=ymax-ymin;ymax=m;ymin=ymax-b;if ymin<1,ymin=1;end;end
            if xmin<1,c=xmax-xmin;xmin=1;xmax=xmin+c;if xmax>n,xmax=n;end;end
            if xmax>n,d=xmax-xmin;xmax=n;xmin=xmax-d;if xmin<1,xmin=1;end;end

            y_min(ii)=aa;y_max(ii)=bb;x_min(ii)=cc;x_max(ii)=dd;

            y_center=floor(rowcenter);
            x_center=floor(colcenter);

            %画矩形框(开始)--------------------------------
            y_start=y_center-gao/2;x_start=x_center-kuan/2;
            if ii==1
                close;
                figure,imshow(I1),axis on;
                %figure,imshow(probmap),axis on;
                hold on;
                rectangle('Position',[x_start,y_start,kuan,gao],'Curvature',[0,0],'LineWidth',2,'LineStyle','-','EdgeColor','r');
                hold off;
                %pause(0.05);
            end
            if ii==2
                hold on;
                rectangle('Position',[x_start,y_start,kuan,gao],'Curvature',[0,0],'LineWidth',2,'LineStyle','-','EdgeColor','b');
                hold off;
                pause(0.05);
            end
            %画矩形框(结束)--------------------------------
            y_center_qian(ii)=y_center;
            x_center_qian(ii)=x_center;

            num_cen=mod(k-1,10);
            num_cen=num_cen+1;
            Y_CEN(num_cen,ii)=y_center_qian(ii);
            X_CEN(num_cen,ii)=x_center_qian(ii);


            %更新直方图(开始)----------------------------------
            y_geng_min=floor(y_start);
            y_geng_max=floor(y_start+gao);
            x_geng_min=floor(x_start);
            x_geng_max=floor(x_start+kuan);

            linshi_histo1=my_histogram_h(y_geng_min,y_geng_max,x_geng_min,x_geng_max,I1,D);
            linshi_histo2=my_histogram_s(y_geng_min,y_geng_max,x_geng_min,x_geng_max,I1,D);
            linshi_histo3=my_histogram_v(y_geng_min,y_geng_max,x_geng_min,x_geng_max,I1,D);

            Dh=0;Ds=0;Dv=0;
            for i=1:256
                Dh=Dh+linshi_histo1(i)*log2(max(linshi_histo1(i),0.00001)/max(linshi1(i,1,ii),0.00001));%//加0.00001是为了防止分母为零
                Ds=Ds+linshi_histo2(i)*log2(max(linshi_histo2(i),0.00001)/max(linshi1(i,1,ii),0.00001));
                Dv=Dv+linshi_histo3(i)*log2(max(linshi_histo3(i),0.00001)/max(linshi1(i,1,ii),0.00001));
            end
            if Dh<=F
                histogram_h(:,1,ii)=linshi_histo1;
            end
            if Ds<=F
                histogram_s(:,1,ii)=linshi_histo2;
            end
            if Dv<=F
                histogram_v(:,1,ii)=linshi_histo3;
            end
            %更新直方图(结束)----------------------------------

            %判断跟丢或消失(开始)--------------------------------
            dis_cen=0;
            num_dis=0;
            if mod(k,10)==0
                for u=2:10
                    %dis_cen=sqrt((Y_CEN(u,ii)-Y_CEN(u-1,ii))^2+(X_CEN(u,ii)-X_CEN(u-1,ii))^2);
                    dis_cen1=abs(Y_CEN(u,ii)-Y_CEN(u-1,ii));
                    dis_cen2=abs(X_CEN(u,ii)-X_CEN(u-1,ii));
                    %if dis_cen<=3
                    if dis_cen1<=10&&dis_cen2<=10
                        num_dis=num_dis+1;
                    end
                end
                if  M00/S(ii)<0.1&&num_dis==5
                    kk=num2str(k);
                    iii=num2str(ii);
                    st=strcat('第',kk,'帧时：','第',iii,'人可能失跟或消失');
                    disp(st);
                end
            end
            %判断跟丢或消失(开始)--------------------------------

%             if ii==2
%             kk=num2str(k);
%             rusult_path=strcat('E:\result\',kk);
%             saveas(gcf,rusult_path,'jpg');
%             end
        end

    end

%  【在线更新】多人camshift跟踪(结束)----------------------------------
tt=toc


