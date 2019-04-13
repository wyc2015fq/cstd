
# Tensorflow中padding的两种类型SAME和VALID - 机器学习的小学生 - CSDN博客


2017年10月29日 15:39:47[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：189个人分类：[tensorflow																](https://blog.csdn.net/xuluhui123/article/category/6934245)



**SAME**means that the output feature map has the same spatial dimensions as the input feature map. Zero padding is introduced to make
 the shapes match as needed, equally on every side of the input map.**VALID**means no padding.


Padding could be used in convolution and pooling operations.
Here, take pooling for example:
|down vote
|If you like ascii art:
|"VALID"|=
 without padding:
|inputs|:|1|2|3|4|5|6|7|8|9|10|11|(|12|13|)|||________________|||dropped|||_________________||
|"SAME"|=
 with zero padding:
|pad|||||pad|inputs|:|0|||1|2|3|4|5|6|7|8|9|10|11|12|13|||0|0|||________________|||||_________________|||||________________||
|In this example:
|Input width = 13
|Filter width = 6
|Stride = 5
|Notes:
|"VALID"|only ever drops the right-most columns (or bottom-most rows).
|"SAME"|tries to pad evenly left and right, but if the amount of columns to be added is odd, it will add the extra column to the right, as is the case in this example (the same logic applies vertically: there may be an extra row of zeros at the bottom).
|


The[TensorFlow Convolution](https://www.tensorflow.org/versions/r0.9/api_docs/python/nn.html)example gives an overview about the
 difference between`SAME`and`VALID`:
For theSAMEpadding,
 the output height and width are computed as:
out_height = ceil(float(in_height) / float(strides[1]))
out_width = ceil(float(in_width) / float(strides[2]))
And
For theVALIDpadding,
 the output height and width are computed as:
out_height = ceil(float(in_height - filter_height + 1) / float(strides[1](https://www.tensorflow.org/versions/r0.9/api_docs/python/nn.html)))
out_width = ceil(float(in_width - filter_width + 1) / float(strides[2]))


转载自：
[http://blog.csdn.net/jasonzzj/article/details/53930074](http://blog.csdn.net/jasonzzj/article/details/53930074)

