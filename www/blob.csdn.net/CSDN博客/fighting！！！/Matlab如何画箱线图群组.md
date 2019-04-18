# Matlab如何画箱线图群组 - fighting！！！ - CSDN博客
2018年05月26日 20:59:43[dujiahei](https://me.csdn.net/dujiahei)阅读数：4075
转自：http://blog.sina.com.cn/s/blog_46acd6f50101a6wj.html
![Matlab如何画箱线图群组](http://s13.sinaimg.cn/mw690/46acd6f5tx6BuHdLB6c0c&690)
使用Matlab如何画出上面的箱线图群组？
示例：
data = rand(20,24)
month = repmat({'jan' 'feb' 'mar' 'apr' 'may' 'jun' 'jul' 'aug' 'sep' 'oct' 'nov' 'dec'},1,2);
simobs = [repmat({'sim'},1,12),repmat({'obs'},1,12)];
boxplot(data,{month,simobs},'colors',repmat('rb',1,12),'factorgap',[5 2],'labelverbosity','minor');
另一种方法是利用不同坐标使用hold on在同一幅图上画箱线图
% Boxplot for the observed temperature from January to December
Temp_O = [Jan_O, Feb_O, Mar_O, Apr_O, May_O, Jun_O, Jul_O, Aug_O, Sep_O, Oct_O, Nov_O, Dec_O];
position_O = 1:1:12; 
% Define position for 12 Month_O boxplots 
box_O = boxplot(Temp_O,'colors','b','positions',position_O,'width',0.18);
set(gca,'XTickLabel',{' '})  % Erase xlabels
hold on  % Keep the Month_O boxplots on figure overlap the Month_S boxplots  
% Boxplot for the simulated temperature from January to December
Temp_S = [Jan_S, Feb_S, Mar_S, Apr_S, May_S, Jun_S, Jul_S, Aug_S, Sep_S, Oct_S, Nov_S, Dec_S];
position_S = 1.3:1:12.3;  % Define position for 12 Month_S boxplots 
box_S = boxplot(Temp_S,'colors','r','positions',position_S,'width',0.18);  
hold off   % Insert texts and labels 
![Matlab如何画箱线图群组](http://s5.sinaimg.cn/mw690/001ifd9bty6EUJT8kRKe4&690)
以上两种方法均可画多组的箱线图
