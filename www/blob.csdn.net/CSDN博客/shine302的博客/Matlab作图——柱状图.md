# Matlab作图——柱状图 - shine302的博客 - CSDN博客
2018年04月13日 21:32:21[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：350
类型一：
% Create data for childhood disease cases
measles = [75.6];
mumps = [80.12];
chickenPox = [4.7];
% Create a vertical bar chart using the bar function
figure
bar([measles' mumps'], 0.5)
% Set the axis limits
axis([0 3 0 100])
set(gca, 'XTick', 1:2)
set(gca,'XTickLabel',{'KNN','GCN'});
ylabel('准确度')
![](https://img-blog.csdn.net/20180413213147313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
类型2：
% data = [23.7,12.63,1.38,7.72;8.2,1.29,0.5,1.72;14.828.72,34.6,22.65;9.7,9.56,12.14,15.75;43.6,49.8,51.38,52.16];
data = [23.7,20.8,1.38,7.72;8.2,3,0.5,1.72;14.816.4,34.6,22.65;9.7,11.7,12.14,15.75;43.6,48.1,51.38,52.16];
%data = [41.6;76.2;80.8;]
bar(data,1);
axis([0 6 0.0 60]);
legend('Survery','RAPR','NANA','BJ',2);
set(gca,'XTickLabel',{'Work','School','Recreation','Shopping','Others'});
set(gcf,'color','white')
%xlabel('乘客活动');
%ylabel('乘客活动比重');
xlabel('passenger activity');
ylabel('passenger activity frequency');
%1
% applyhatch(gcf,'\-/x');
% [im_hatch,colorlist] = applyhatch_pluscolor(gcf,'.-/x',1,[],[],150);
% imwrite(im_hatch,'im_hatch.png','png')
![](https://img-blog.csdn.net/20180413213353812?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
