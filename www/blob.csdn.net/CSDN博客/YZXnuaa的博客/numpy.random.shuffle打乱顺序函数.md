# numpy.random.shuffle打乱顺序函数 - YZXnuaa的博客 - CSDN博客
2018年04月03日 09:59:56[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1186
在做将caffe模型和预训练的参数转化为tensorflow的模型和预训练的参数，以便微调，遇到如下函数：
**[python]**[view plain](https://blog.csdn.net/jasonzzj/article/details/53932645#)[copy](https://blog.csdn.net/jasonzzj/article/details/53932645#)
- def gen_data(source):  
- whileTrue:  
-         indices = range(len(source.images)) # indices = the number of images in the source data set
-         random.shuffle(indices)  
- for i in indices:  
-             image = np.reshape(source.images[i], (28, 28, 1))  
-             label = source.labels[i]  
- yield image, label  
         之前卑鄙陋寡闻，不知道这个用法，按照字面上的意思是打乱，那么这里就应该是让训练数据集中的数据打乱顺序，然后一个挨着一个地(for i in indices)生成训练数据对。下面就从docs.scipy.org中查到的random.shuffle的用法：
- numpy.random.shuffle(x)
Modify a sequence in-place by shuffling its contents.
Returns:
None
举例
python>>>>>> arr=np.arange()>>> np.random.shuffle(arr)>>> arr[1 7 5 2 9 4 3 6 0 8]
This function only shuffles the array along the first index of a multi-dimensional array（多维矩阵中，只对第一维（行）做打乱顺序操作）:
python>>>>>> arr=np.arange().reshape((,))>>> np.random.shuffle(arr)>>> arrarray([[3, 4, 5],       [6, 7, 8],       [0, 1, 2]])This function only shuffles the array along the first index of a multi-dimensional array:
参考：·[1] https://docs.scipy.org/doc/numpy/reference/generated/numpy.random.shuffle.html#numpy-random-shuffle
            [2] https://github.com/ethereon/caffe-tensorflow/blob/master/examples/mnist/finetune_mnist.py
