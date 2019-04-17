# MatLab GUI Load .mat File 导入mat文件 - Grandyang - 博客园







# [MatLab GUI Load .mat File 导入mat文件](https://www.cnblogs.com/grandyang/p/5150922.html)







在MatLab中，我们用GUI时，有时候需要导入mat格式的图片，但是在GUI中调用load和在命令行里调用load不一样，在命令行里调用load('im.mat')，加载进去是uint8的矩阵，但是我们在GUI中写代码加进去，默认得到的是一个struct结构体，如果我们此时强行用imshow来显示导入的数据，会出现如下错误：

??? Error using ==> iptcheckinput
Function IMAGEDISPLAYVALIDATEPARAMS expected its first input, I,
to be one of these types:

  double, single, uint8, uint16, uint32, int8, int16, int32, logical

Instead its type was struct.



为了取出其中的图片数据，可浪费了我好几个小时候，终于找到了方法，这样我们就可以在GUI中的axes中显示出图片了，参见如下：



```
% Load im.mat
filename = uigetfile('*.mat');
img = cell2mat(struct2cell(load(filename)));
axes(handles.axes_1);
imshow(img);
```














