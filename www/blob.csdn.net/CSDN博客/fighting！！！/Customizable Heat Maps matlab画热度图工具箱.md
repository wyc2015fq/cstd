# Customizable Heat Maps  matlab画热度图工具箱 - fighting！！！ - CSDN博客
2018年05月29日 22:12:05[dujiahei](https://me.csdn.net/dujiahei)阅读数：1378
源地址：https://ww2.mathworks.cn/matlabcentral/fileexchange/24253-customizable-heat-maps?focused=6785671&tab=example
# Heatmap Examples
This scripts demonstrates the capabilities of the heatmap visualization function.
Copyright The MathWorks, Inc. 2014
## Contents
- [Load Data for Visualization](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#1)
- [Simple Heatmap](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#2)
- [Axes Ticks & Labels](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#3)
- [Heatmap Text Labels](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#7)
- [Zoom, Pan & Data Cursors](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#11)
- [Changing the Colormap](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#12)
- [Multiple Heatmaps on a Figure](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#16)
- [Forcing Color Levels](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#17)
- [Missing Values](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#20)
- [Colorbar](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#21)
- [Grid Lines](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples.html?access_key=#23)
## Load Data for Visualization
Import data set to visualize. The data set contains a matrix of electricity price differences between locations in New England.
load heatmapData
## Simple Heatmap
Generate heatmap only with no labels
clf
heatmap(spreads_small);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_01.png)
## Axes Ticks & Labels
The heatmap above can be made a lot more useful with labels on the columns and rows. We can do this with two more inputs for x and y labels. The labels can be numeric or cell arrays of strings
clf
heatmap(spreads_small, 1:15, labels_small);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_02.png)
Labels on the x-axis can be rotated to prevent overlap
clf
heatmap(spreads_small, labels_small, labels_small, [], 'TickAngle', 45);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_03.png)
By default, for larger heatmaps, not all ticks are shown. This can be forced with the ShowAllTicks option.
clf
heatmap(spreads_small, labels_small, labels_small, [], 'TickAngle', 45,...'ShowAllTicks', true);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_04.png)
The font size of the ticks can also be controlled with the TickFontSize option. This can help when trying to fit many tick labels on a heatmap.
clf
heatmap(spreads, labels, labels, [], 'TickAngle', 45,...'ShowAllTicks', true, 'TickFontSize', 6);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_05.png)
## Heatmap Text Labels
The heatmap image can be overlaid with text strings to either make the heatmap more descriptive or overlay another data set. The text labels can either be just turned on, turned on with a specific format or specified as another numeric matrix or cell array of strings
Turn on text labels with a format $xx.xx. See the documentation of sprintf for more information on format strings
clf
heatmap(spreads_small, labels_small, labels_small, '$%0.2f', 'TickAngle', 45);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_06.png)
A completely different matrix of data can be shown as text labels on top of the original heatmap, enabling you to overlay another dataset
% Convert matrix c_small into a cell array of formatted strings
clabel = arrayfun(@(x){sprintf('%0.2f',x)}, c_small);
clf
heatmap(spreads_small, labels_small, labels_small, clabel, 'TickAngle', 45);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_07.png)
Properties of text labels include FontSize and TextColor. TextColor can also be specified as a string 'xor' in which case a color will automatically be chosen for each label to contrast with the color on the image.
clf
heatmap(spreads_small, labels_small, labels_small, '$%0.2f', ...'TickAngle', 45, 'FontSize', 6, 'TextColor', 'w');
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_08.png)
## Zoom, Pan & Data Cursors
Heatmaps generated with heatmap are interactive, in that you can zoom and pan to explore the visualization. The tick labels will automatically update in response to zoom and pan events. Data cursors are also supported. The text shown in the data cursor is derived from the text labels used to display the data on the heatmap image.
## Changing the Colormap
Heatmaps, by default, use the colormap of the figure in which they are created. Therefore, changing the figure colormap will change that of the heatmap.
clf
heatmap(spreads);
snapnow
colormap cool
snapnow
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_09.png)![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_10.png)
heatmap includes two custom colormaps. The colormap money displays values of 0 as white and positive and negative values as shades of green or red. The colormap red displays values of 0 as white and positive values as different shades of red. You can also use your own colormaps with the Colormap option
clf
heatmap(spreads_small, [], [], '%0.2f', 'Colormap', 'money', ...'Colorbar', true);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_11.png)
The option ColorLevels lets you increase or decrease the number of distinct colors in the colormap.
clf
heatmap(spreads_small, [], [], '%0.2f', 'Colormap', 'money',...'Colorbar', true, 'ColorLevels', 5);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_12.png)
The colormap can also be constructed on a log-scale. This can be useful if your matrix values are not evenly distributed. Using a log-scale colormap will highlight the variation in the small values in your dataset.
clf
heatmap(spreads_small, [], [], '%0.2f', 'TextColor', 'w', ...'Colormap', 'copper', 'Colorbar', true);
snapnow
heatmap(spreads_small, [], [], '%0.2f', 'TextColor', 'w', ...'Colormap', 'copper', 'Colorbar', true, 'UseLogColormap', true);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_13.png)![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_14.png)
## Multiple Heatmaps on a Figure
The heatmap function can be used with multiple axes in a figure, such as with the subplot command. By default they will share the colormap of the figure. The option UseFigureColormap, if set to 0 or false, will make each heatmap use a different colormap.
clf
subplot(2,1,1);
heatmap(spreads_small, [], [], '%0.2f', 'Colormap', 'money', ...'UseFigureColormap', false, 'Colorbar', true);
title('Money Colormap');
subplot(2,1,2);
heatmap(spreads_small, [], [], '%0.2f', 'Colormap', 'copper', ...'UseFigureColormap', false, 'Colorbar', true, 'TextColor', 'w');
title('Copper Colormap');
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_15.png)
## Forcing Color Levels
A common use of heatmaps is to compare two matrices or tables using color to identify a change or difference. By default heatmap adjusts the colormap so that the minimum and maximum data values in the matrix map to the lowest and highest color. This is usually undesirable when comparing heatmaps where a consistent data-to-color mapping is needed. To aid in this effort, heatmap provides two inputs, MinColorValue and MaxColorValue, that control the minimum and maximum values of the data that map to the lowest and highest color levels. Here we use these to ensure comparable colors between two heatmaps.
The following two plots are heatmaps of two matrices with default color levels. Note that the colors are not consistent between them.
x = spreads(1:10,1:10);
y = x/2;
y([58 59 65 66]) = y([58 59 65 66])*2;
clf
heatmap(x,[],[],'%0.2f','ColorMap', @cool, 'Colorbar',true);
snapnow;
heatmap(y,[],[],'%0.2f','ColorMap', @cool, 'Colorbar',true);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_16.png)![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_17.png)
These two plots show the same heatmaps with consistent color levels. The mincolorvalue and maxcolorvalue have been set to the minimum and maximum of both matrices.
mincolor = min([x(:);y(:)]);
maxcolor = max([x(:);y(:)]);
heatmap(x,[],[],'%0.2f','ColorMap', @cool, 'Colorbar',true, ...'MinColorValue', mincolor, 'MaxColorValue', maxcolor);
snapnow
heatmap(y,[],[],'%0.2f','ColorMap', @cool, 'Colorbar',true, ...'MinColorValue', mincolor, 'MaxColorValue', maxcolor);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_18.png)![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_19.png)
## Missing Values
NaNs in the matrix are allowed but by default these get mapped to the lowest color value. The option NaNColor enables you to explicitly specify the color that NaN or missing values should take. In the following heatmap, they are set to black to distinguish them from the other elements that contain low values
close(gcf); clf
heatmap(c,[],[],[],'ColorMap', @cool, 'NaNColor', [0 0 0], 'colorbar', true);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_20.png)
## Colorbar
As seen above, a colorbar can be added to the figure using the Colorbar option. The labels for the colorbar will automatically be formatted using the formats for the text labels on the heatmap image. Notice the $ signs on the colorbar in the image below.
clf
heatmap(spreads, [], [], '$%0.2f', 'Colormap', 'money', ...'FontSize', 2, 'Colorbar', true);
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_21.png)
The value for the colorbar option can either be a simple true or false in which case the default colorbar will be drawn, or it can include a cell array of property-value pairs for the colorbar command.
heatmap(spreads, [], [], '$%0.2f', 'Colormap', 'money', ...'FontSize', 2, 'Colorbar', {'SouthOutside'});
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_22.png)
## Grid Lines
Grid lines can be added with a GridLines option which is a line specification like ':' for a dotted line
clf
heatmap(spreads_small, labels_small, labels_small, '%0.2f', ...'TickAngle', 45, 'GridLines', ':');
![](https://ww2.mathworks.cn/matlabcentral/mlc-downloads/downloads/submissions/24253/versions/7/previews/html/heatmap_examples_23.png)
