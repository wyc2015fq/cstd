# tensorflow读取数据-tfrecord格式 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:10:05[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：42481

# 概述：
关于tensorflow读取数据，官网给出了三种方法：
1、供给数据：在tensorflow程序运行的每一步，让python代码来供给数据
2、从文件读取数据：建立输入管线从文件中读取数据
3、预加载数据：如果数据量不太大，可以在程序中定义常量或者变量来保存所有的数据。
这里主要介绍一种比较通用、高效的数据读取方法，就是tensorflow官方推荐的标准格式：tfrecord。
# tfrecord数据文件
tfrecord数据文件是一种将图像数据和标签统一存储的二进制文件，能更好的利用内存，在tensorflow中快速的复制，移动，读取，存储等。
tfrecord文件包含了tf.train.Example 协议缓冲区(protocol buffer，协议缓冲区包含了特征 Features)。你可以写一段代码获取你的数据， 将数据填入到Example协议缓冲区(protocol buffer)，将协议缓冲区序列化为一个字符串， 并且通过tf.python_io.TFRecordWriter class写入到TFRecords文件。tensorflow/g3doc/how_tos/reading_data/convert_to_records.py就是这样的一个例子。
tf.train.Example的定义如下：
```
message Example {
 Features features = 1;
};
message Features{
 map<string,Feature> featrue = 1;
};
message Feature{
    oneof kind{
        BytesList bytes_list = 1;
        FloatList float_list = 2;
        Int64List int64_list = 3;
    }
};
```
从上述代码可以看出，tf.train.Example中包含了属性名称到取值的字典，其中属性名称为字符串，属性的取值可以为字符串（BytesList）、实数列表（FloatList）或者整数列表（Int64List）。
# 代码实现
## 将数据保存为tfrecord格式
具体来说，首先需要给定tfrecord文件名称，并创建一个文件：
```
tfrecords_filename = './tfrecords/train.tfrecords'
writer = tf.python_io.TFRecordWriter(tfrecords_filename) # 创建.tfrecord文件，准备写入
```
之后就可以创建一个循环来依次写入数据：
```
for i in range(100):
        img_raw = np.random.random_integers(0,255,size=(7,30)) # 创建7*30，取值在0-255之间随机数组
        img_raw = img_raw.tostring()
        example = tf.train.Example(features=tf.train.Features(
                feature={
                'label': tf.train.Feature(int64_list = tf.train.Int64List(value=[i])),     
                'img_raw':tf.train.Feature(bytes_list = tf.train.BytesList(value=[img_raw]))
                }))
        writer.write(example.SerializeToString()) 
    writer.close()
```
example = tf.train.Example()这句将数据赋给了变量example（可以看到里面是通过字典结构实现的赋值），然后用writer.write(example.SerializeToString()) 这句实现写入。
值得注意的是赋值给example的数据格式。从前面tf.train.Example的定义可知，tfrecord支持整型、浮点数和二进制三种格式，分别是
```
tf.train.Feature(int64_list = tf.train.Int64List(value=[int_scalar]))
tf.train.Feature(bytes_list = tf.train.BytesList(value=[array_string_or_byte]))
tf.train.Feature(bytes_list = tf.train.FloatList(value=[float_scalar]))
```
例如图片等数组形式（array）的数据，可以保存为numpy array的格式，转换为string，然后保存到二进制格式的feature中。对于单个的数值（scalar），可以直接赋值。这里value=[×]的[]非常重要，也就是说输入的必须是列表（list）。当然，对于输入数据是向量形式的，可以根据数据类型（float还是int）分别保存。并且在保存的时候还可以指定数据的维数。
## 读取tfrecord数据
从TFRecords文件中读取数据， 首先需要用tf.train.string_input_producer生成一个解析队列。之后调用tf.TFRecordReader的tf.parse_single_example解析器。如下图：
![strip](https://upload-images.jianshu.io/upload_images/4685306-eae218123ff6abd2.gif?imageMogr2/auto-orient/strip)
AnimatedFileQueues.gif
解析器首先读取解析队列，返回serialized_example对象，之后调用tf.parse_single_example操作将Example协议缓冲区(protocol buffer)解析为张量。 MNIST的例子就使用了convert_to_records 所构建的数据。可以参考tensorflow/examples/how_tos/reading_data/fully_connected_reader.py。
下面代码实现了tfrecord数据的读取：
```
if __name__==’__main__’:
    tfrecords_filename = "train.tfrecords"
    test_write_to_tfrecords(tfrecords_filename)
    filename_queue = tf.train.string_input_producer([tfrecords_filename],) #读入流中
    reader = tf.TFRecordReader()
    _, serialized_example = reader.read(filename_queue)   #返回文件名和文件
    features = tf.parse_single_example(serialized_example,
                                       features={
                                           'label': tf.FixedLenFeature([], tf.int64),
                                           'img_raw' : tf.FixedLenFeature([], tf.string),
                                       })  #取出包含image和label的feature对象
    image = tf.decode_raw(features['img_raw'],tf.int64)
    image = tf.reshape(image, [7,30])
    label = tf.cast(features['label'], tf.int64)
    with tf.Session() as sess: #开始一个会话
        init_op = tf.initialize_all_variables()
        sess.run(init_op)
        coord=tf.train.Coordinator()
        threads= tf.train.start_queue_runners(coord=coord)
        for i in range(20):
            example, l = sess.run([image,label])#在会话中取出image和label
            img=Image.fromarray(example, 'RGB')#这里Image是之前提到的
            img.save('./'+str(i)+'_''Label_'+str(l)+'.jpg')#存下图片
            print(example, l)
        coord.request_stop()
        coord.join(threads)
```
程序中with tf.Session() as sess:创建了一个会话，其中特别要注意的就是以下两句，非常重要
```
coord=tf.train.Coordinator() #创建一个协调器，管理线程
       threads= tf.train.start_queue_runners(coord=coord) #启动QueueRunner, 此时文件名队列已经进队。
```
这两句实现的功能就是创建线程并使用QueueRunner对象来提取数据。简单来说：使用tf.train函数添加QueueRunner到tensorflow中。在运行任何训练步骤之前，需要调用tf.train.start_queue_runners函数，否则tensorflow将一直挂起。
tf.train.start_queue_runners 这个函数将会启动输入管道的线程，填充样本到队列中，以便出队操作可以从队列中拿到样本。这种情况下最好配合使用一个tf.train.Coordinator，这样可以在发生错误的情况下正确地关闭这些线程。如果你对训练迭代数做了限制，那么需要使用一个训练迭代数计数器，并且需要被初始化。详细解释可以参考链接[4]。
推荐的代码模板如下：
```
# Create the graph, etc.
init_op = tf.initialize_all_variables()
# Create a session for running operations in the Graph.
sess = tf.Session()
# Initialize the variables (like the epoch counter).
sess.run(init_op)
# Start input enqueue threads.
coord = tf.train.Coordinator()
threads = tf.train.start_queue_runners(sess=sess, coord=coord)
try:
    while not coord.should_stop():
        # Run training steps or whatever
        sess.run(train_op)
except tf.errors.OutOfRangeError:
    print 'Done training -- epoch limit reached'
finally:
    # When done, ask the threads to stop.
    coord.request_stop()
# Wait for threads to finish.
coord.join(threads)
sess.close()
```
在tf.train中要创建这些队列和执行入队操作，就要添加QueueRunner到一个使用tf.train.add_queue_runner函数的数据流图中。每个QueueRunner负责一个阶段，处理那些需要在线程中运行的入队操作的列表。一旦数据流图构造成功，tf.train.start_queue_runners函数就会要求数据流图中每个QueueRunner去开始它的线程运行入队操作。
在执行训练的时候，队列会被后台的线程填充好。如果设置了最大训练迭代数（epoch），在某些时候，样本出队的操作可能会抛出一个tf.OutOfRangeError的错误。这是因为tensorflow的队列已经到达了最大实际的最大迭代数，没有更多可用的样本了。这也是为何推荐代码模板需要用try..except ..finally结构来处理这种错误。
上面代码读取的是单个的image和label，而在tensorflow训练时，一般是采取batch的方式去读入数据。tensorflow提供了两种方式，一种是shuffle_batch（tf.train.shuffle_batch），这种主要是用在训练中，随机选取样本组成batch。另外一种就是按照数据在tfrecord中的先后顺序生成batch（tf.train.batch）。
这里采用tf.train.shuffle_batch方式：
```
# Creates batches of 32 images and 32 labels.
image_batch, label_batch = tf.train.shuffle_batch(
      [image, label],
      batch_size=3,
      num_threads=4,
      capacity=10,
      min_after_dequeue=10)
```
tf.train.shuffle_batch函数输入参数为：
- tensor_list: 进入队列的张量列表The list of tensors to enqueue.
- batch_size: 从数据队列中抽取一个批次所包含的数据条数The new batch size pulled from the queue.
- capacity: 队列中最大的数据条数An integer. The maximum number of elements in the queue.
- min_after_dequeue: 提出队列后，队列中剩余的最小数据条数Minimum number elements in the queue     after a dequeue, used to ensure a level of mixing of elements.
- num_threads: 进行队列操作的线程数目The number of threads enqueuing tensor_list.
- seed: 队列中进行随机排列的随机数发生器，似乎不常用到Seed for the random shuffling within the queue.
- enqueue_many: 张量列表中的每个张量是否是一个单独的例子，似乎不常用到Whether each tensor in tensor_list is a single example.
- shapes: (Optional) The shapes for each example. Defaults to the inferred shapes for tensor_list.
- name: (Optional) A name for the operations.
**值得注意的是，capacity>=min_after_dequeue+num_threads*batch_size。**
最后附上完整代码：
```
import tensorflow as tf
import numpy as np
import os
#=============================================================================#
# write images and label in tfrecord file and read them out
def encode_to_tfrecords(tfrecords_filename, data_num): 
    ''' write into tfrecord file '''
    if os.path.exists(tfrecords_filename):
        os.remove(tfrecords_filename)
    writer = tf.python_io.TFRecordWriter('./'+tfrecords_filename) # 创建.tfrecord文件，准备写入
    for i in range(data_num):
        img_raw = np.random.randint(0,255,size=(56,56))
        img_raw = img_raw.tostring()
        example = tf.train.Example(features=tf.train.Features(
                feature={
                'label': tf.train.Feature(int64_list = tf.train.Int64List(value=[i])),     
                'img_raw':tf.train.Feature(bytes_list = tf.train.BytesList(value=[img_raw]))
                }))
        writer.write(example.SerializeToString()) 
    writer.close()
    return 0
def decode_from_tfrecords(filename_queue, is_batch):
    reader = tf.TFRecordReader()
    _, serialized_example = reader.read(filename_queue)   #返回文件名和文件
    features = tf.parse_single_example(serialized_example,
                                       features={
                                           'label': tf.FixedLenFeature([], tf.int64),
                                           'img_raw' : tf.FixedLenFeature([], tf.string),
                                       })  #取出包含image和label的feature对象
    image = tf.decode_raw(features['img_raw'],tf.int64)
    image = tf.reshape(image, [56,56])
    label = tf.cast(features['label'], tf.int64)
    if is_batch:
        batch_size = 3
        min_after_dequeue = 10
        capacity = min_after_dequeue+3*batch_size
        image, label = tf.train.shuffle_batch([image, label],
                                                          batch_size=batch_size, 
                                                          num_threads=3, 
                                                          capacity=capacity,
                                                          min_after_dequeue=min_after_dequeue)
    return image, label
#=============================================================================#
if __name__=='__main__':
    # make train.tfrecord
    train_filename = "train.tfrecords"
    encode_to_tfrecords(train_filename,100)
##    # make test.tfrecord
    test_filename = 'test.tfrecords'
    encode_to_tfrecords(test_filename,10)
#    run_test = True
    filename_queue = tf.train.string_input_producer([train_filename],num_epochs=None) #读入流中
    train_image, train_label = decode_from_tfrecords(filename_queue, is_batch=True)
    filename_queue = tf.train.string_input_producer([test_filename],num_epochs=None) #读入流中
    test_image, test_label = decode_from_tfrecords(filename_queue, is_batch=True)
    with tf.Session() as sess: #开始一个会话
        init_op = tf.global_variables_initializer()
        sess.run(init_op)
        coord=tf.train.Coordinator()
        threads= tf.train.start_queue_runners(coord=coord)
        try:
            # while not coord.should_stop():
            for i in range(2):
                example, l = sess.run([train_image,train_label])#在会话中取出image和label
                print('train:')
                print(example, l) 
                texample, tl = sess.run([test_image, test_label])
                print('test:')
                print(texample,tl)
        except tf.errors.OutOfRangeError:
            print('Done reading')
        finally:
            coord.request_stop()
        coord.request_stop()
        coord.join(threads)
```
另一段tfrecord代码，这段代码实现了float，int和string三种类型数据tfrecord格式的编码和解码。特别注意的是，这里编码和解码时，指定了数据的维度。
```
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul 24 15:47:28 2017
@author: dl
"""
import tensorflow as tf
import numpy as np
writer = tf.python_io.TFRecordWriter('test.tfrecord')
for i in range(0, 2):
    a = np.random.random(size=(180)).astype(np.float32)
    a = a.data.tolist()
    b = [2016 + i, 2017+i]
    c = np.array([[0, 1, 2],[3, 4, 5]]) + i
    c = c.astype(np.uint8)
    c_raw = c.tostring()#这里是把ｃ换了一种格式存储
    print('  i:', i)
    print('  a:', a)
    print('  b:', b)
    print('  c:', c)
    example = tf.train.Example(features=tf.train.Features(
            feature = {'a':tf.train.Feature(float_list = tf.train.FloatList(value=a)),
                       'b':tf.train.Feature(int64_list = tf.train.Int64List(value = b)),
                       'c':tf.train.Feature(bytes_list = tf.train.BytesList(value = [c_raw]))}))
    serialized = example.SerializeToString()
    writer.write(serialized)
    print('   writer',i,'DOWN!')
writer.close()
filename_queue = tf.train.string_input_producer(['test.tfrecord'], num_epochs=None)
# create a reader from file queue
reader = tf.TFRecordReader()
_, serialized_example = reader.read(filename_queue)
# get feature from serialized example
features = tf.parse_single_example(serialized_example,
        features={
            'a': tf.FixedLenFeature([180], tf.float32),
            'b': tf.FixedLenFeature([2], tf.int64),
            'c': tf.FixedLenFeature([],tf.string)
        }
    )
a_out = features['a']
b_out = features['b']
c_out = features['c']
#c_raw_out = features['c']
#c_raw_out = tf.sparse_to_dense(features['c'])
#c_out = tf.decode_raw(c_raw_out, tf.uint8)
print( a_out)
print( b_out)
print( c_out)
a_batch, b_batch, c_batch = tf.train.shuffle_batch([a_out, b_out, c_out], batch_size=3, 
                                capacity=200, min_after_dequeue=100, num_threads=2)
print( a_batch)
print( b_batch)
print( c_batch)
```
## 附：tensorflowd协调器和队列运行器(Coordinator and QueueRunner)
![1240](https://upload-images.jianshu.io/upload_images/4685306-5110b06a6845c127.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
# 参考：
[1][http://www.2cto.com/kf/201702/604326.html](http://www.2cto.com/kf/201702/604326.html)
[2]这篇特别详细：[http://blog.csdn.net/u010358677/article/details/70544241](http://blog.csdn.net/u010358677/article/details/70544241)
[3][http://blog.csdn.net/lujiandong1/article/details/53376802](http://blog.csdn.net/lujiandong1/article/details/53376802)
[4]极客学院：[http://wiki.jikexueyuan.com/project/tensorflow-zh/how_tos/reading_data.html](http://wiki.jikexueyuan.com/project/tensorflow-zh/how_tos/reading_data.html)
[5][http://www.cnblogs.com/upright/p/6136265.html](http://www.cnblogs.com/upright/p/6136265.html)
