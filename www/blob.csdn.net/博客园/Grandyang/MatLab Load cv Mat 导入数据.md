# MatLab Load cv::Mat 导入数据 - Grandyang - 博客园







# [MatLab Load cv::Mat 导入数据](https://www.cnblogs.com/grandyang/p/4695058.html)







我们有时候在项目中需要将OpenCV中的cv::Mat导入MatLab进行分析与处理，那么如果把数据转过去呢，我们的做法是首先将cv::Mat导出为txt文件，或者是yml文件，请参见我之前的博客[Write cv::Mat to a file](http://www.cnblogs.com/grandyang/p/4354952.html)。



导出的txt文件可以直接load进MatLab，如果数据是多维数组的话，只需用写几行代码来修改下即可，参见代码如下：

```
// If the size of 'im' is m by n by k,then the loaded 'data' is m by n*k, we need to split it
data = load('im.txt');
for i = 1:k
    newData(:,:,i) = data(:,i:k:end);
end
```



如果导出为.yml文件，那么就比较复杂，因为MatLab不支持直接加载.yml文件，需要自己写代码来加载。












