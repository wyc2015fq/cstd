# Matlab——折线图 - shine302的博客 - CSDN博客
2018年05月14日 20:12:17[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：1164
% abc = [2,4,6,8,10,12,14,16];
% cba = [7.15,7.15,16.375,24.15,30.3,564,2256,10156];
abc = [2,4,6,8,10];
cba = [7.15,16.375,24.15,30.3,564];
% Create the pie chart in position 1 of a 2x2 grid
figure
% Create the line plot in position 4 of a 2x2 grid
subplot(1, 1, 1)
plot(abc,cba,'r-o','MarkerFaceColor','red');
hold on;
% legend('Weekend','Weekday',1)
set(gca,'XTickLabel',{'20000','40000','60000','80000','100000'},'FontSize',12)
% set(gca,'XTick',1:1:7);
xlabel('Average number of requests per hour')
ylabel('Worst-case response time')
% title('Sports activity ')
axis([1 11 0.0 600])
grid on;
set(gcf,'color','white')
![](https://img-blog.csdn.net/20180514201150809?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
