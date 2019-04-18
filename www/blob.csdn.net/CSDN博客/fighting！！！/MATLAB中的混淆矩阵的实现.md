# MATLAB中的混淆矩阵的实现 - fighting！！！ - CSDN博客
2018年06月24日 11:16:57[dujiahei](https://me.csdn.net/dujiahei)阅读数：361
转自：https://blog.csdn.net/zhaomengszu/article/details/56283832
混淆矩阵的matlab代码实现：
actual：就是我们已知的label。
detected是我们通过模型预测得到的label
结合下面语句实现：
[pred,acc,preb] = svmpredict(double(testLabel), testData, model, '-b 1');

[html][view plain](https://blog.csdn.net/zhaomengszu/article/details/56283832#)[copy](https://blog.csdn.net/zhaomengszu/article/details/56283832#)
-  function confusion_matrix(actual,detected)  
-  [mat,order] = confusionmat(actual,detected);  
- 
-  %mat = rand(10);           %# A 5-by-5 matrix of random values from 0 to 1  
- % mat(3,3) = 0;            %# To illustrate  
- % mat(5,2) = 0;            %# To illustrate  
- imagesc(mat);            %# Create a colored plot of the matrix values  
- colormap(flipud(gray));  %# Change the colormap to gray (so higher values are  
-                          %#   black and lower values are white)  
- 
- textStrings = num2str(mat(:),'%0.02f');  %# Create strings from the matrix values  
- textStrings = strtrim(cellstr(textStrings));  %# Remove any space padding  
- 
- %% ## New code: ###  
- %idx = find(strcmp(textStrings(:), '0.00'));  
- %textStrings(idx) = {'   '};  
- %% ################  
- 
- [x,y] = meshgrid(1:5);   %# Create x and y coordinates for the strings  
- hStrings = text(x(:),y(:),textStrings(:),...      %# Plot the strings  
-                 'HorizontalAlignment','center');  
- midValue = mean(get(gca,'CLim'));  %# Get the middle value of the color range  
- textColors = repmat(mat(:) > midValue,1,3);  %# Choose white or black for the  
-                                              %#   text color of the strings so  
-                                              %#   they can be easily seen over  
-                                              %#   the background color  
- set(hStrings,{'Color'},num2cell(textColors,2));  %# Change the text colors  
- 
- set(gca,'XTick',1:5,...                         %# Change the axes tick marks  
-         'XTickLabel',{'Bob','Hyt','Maple','Study','Zm'},...  %#   and tick labels  
-         'YTick',1:5,...  
-         'YTickLabel',{'Bob','Hyt','Maple','Study','Zm'},...  
-         'TickLength',[0 0]);  
结果是：
![](https://img-blog.csdn.net/20170221152022588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhb21lbmdzenU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
