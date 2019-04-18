# TensorFlow的几种输入 - YZXnuaa的博客 - CSDN博客
2018年01月15日 18:26:40[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：938
本文列举了自己在最近用到过的几种TensorFlow输入方式，包括从原图读取，从CSV (TXT)读取，以及从bin file读取。
1, 从原图读取。
很多情况下我们的图片训练集就是原始图片本身，并没有像cifar dataset那样存成bin等格式。因此我们需要根据一个train_list列表，去挨个读取图片。这里我用到的方法是首先将train_list.txt中的image list(也就是每一行有图片的路劲和label组成)读入队列中，那么对每次dequeue的内容中可以提取当前图片的路劲和label
**[python]**[view plain](http://blog.csdn.net/jyshee/article/details/52566152#)[copy](http://blog.csdn.net/jyshee/article/details/52566152#)
- filename = os.path.join(data_dir, trainfilename)  
-     with open(filename) as fid:  
-         content = fid.read()  
-     content = content.split('\n')  
-     content = content[:-1]  
-     valuequeue = tf.train.string_input_producer(content,shuffle=True)  
-     value = valuequeue.dequeue()  
-     dir, labels = tf.decode_csv(records=value, record_defaults=[["string"], [""]], field_delim=" ")  
-     labels = tf.string_to_number(labels, tf.int32)  
然后从路劲dir中用tf.read_file进行读取，对读到的内容用tf.image.decode_png(或者tf.image.decode_jpeg)进行解析，得到image的tensor形式
**[python]**[view plain](http://blog.csdn.net/jyshee/article/details/52566152#)[copy](http://blog.csdn.net/jyshee/article/details/52566152#)
- imagecontent = tf.read_file(dir)  
-     image = tf.image.decode_png(imagecontent, channels=3, dtype=tf.uint8)  
-     image = tf.cast(image, tf.float32)  
-     rshape = tf.reshape(tf.reduce_mean(image, [0, 1]), [1, 1, 3]) #这里是对像素值归到128的均值，即对每个channel分别除以均值乘以128
-     image = image / rshape*128
最后就是crop以及batch了，这里省略了augment
**[python]**[view plain](http://blog.csdn.net/jyshee/article/details/52566152#)[copy](http://blog.csdn.net/jyshee/article/details/52566152#)
- image = tf.random_crop(image, [IMAGE_SIZE, IMAGE_SIZE, 3])  
- images, labels_batch = tf.train.shuffle_batch([image, labels], batch_size=batch_size, num_threads=6,  
-                                               capacity=3 * batch_size+3000, min_after_dequeue=3000)  
- return images,labels_batch   
2，从bin file 中读取
在官网的cifar的例子中就是从bin文件中读取的。bin文件需要以一定的size格式存储，比如每个样本的值占多少字节，label占多少字节，且这对于每个样本都是固定的，然后一个挨着一个存储。这样就可以使用tf.FixedLengthRecordReader 类来每次读取固定长度的字节，正好对应一个样本存储的字节（包括label）。并且用tf.decode_raw进行解析。
下面代码摘自cifar10_input.py。在用tf.decode_raw（注意decode时使用的数据格式最好与存储是的相同）得到record_bytes后，用tf.slice抽取里面的内容，第二个输入参数表示从第几个字节开始抽取，第三个参数表示抽取的字节数。代码中的reshape是根据图片存储是的shape格式有关，具体问题具体分析。
**[python]**[view plain](http://blog.csdn.net/jyshee/article/details/52566152#)[copy](http://blog.csdn.net/jyshee/article/details/52566152#)
- record_bytes = label_bytes + image_bytes  
- 
- reader = tf.FixedLengthRecordReader(record_bytes=record_bytes)  
- result.key, value = reader.read(filename_queue)  
- 
- # Convert from a string to a vector of uint8 that is record_bytes long.
- record_bytes = tf.decode_raw(value, tf.uint8)  
- 
- # The first bytes represent the label, which we convert from uint8->int32.
- label = tf.cast(  
-     tf.slice(record_bytes, [0], [label_bytes]), tf.int32)  
- 
- # The remaining bytes after the label represent the image, which we reshape
- # from [depth * height * width] to [depth, height, width].
- depth_major = tf.reshape(tf.slice(record_bytes, [label_bytes], [image_bytes]),  
-                          [result.depth, result.height, result.width])  
- # Convert from [depth, height, width] to [height, width, depth].
- uint8image = tf.transpose(depth_major, [1, 2, 0])  
最后自然还需要用shuffle_batch 生成batch。
3，从CSV (TXT)中读取
有的时候在数据量不是很大的时候，可以从CSV或者TXT文件进行读取。这中方法和第二种从bin文件中读取很类似。因为CSV (TXT)一般是一行存一个样本（包括样本值和label），因此使用tf.TextLineReader 类来每次读取一行，并使用tf.decode_csv来对每一行进行解析。
这里主要介绍一下 tf.decode_csv(records, record_defaults, field_delim=None, name=None)
首先records与第二种方法中相同，为reader读到的内容，这里为CSV (TXT)的一行。一般一行里面的值会用逗号或者空格隔开，这里第三个输入参数就是指定用什么来进行分割，默认为逗号。第二个输入参数是指定分割后每个属性的类型，比如分割后会有三列，那么第二个参数就应该是[['int32'], [], ['string']], 可见不指定类型（设为空[]）也可以。如果分割后的属性比较多，比如有100个，可以用[ []*100 ]来表示
col= tf.decode_csv(records, record_defaults=[ [ ]*100 ], field_delim=‘ ’, name=None)
返回的col是长度为100的list。
其他的操作与第二种方法类似，不必多说。
需要注意的是，当数据量比较大的时候，存成CSV或TXT文件要比BIN文件大的多，因此在TF中读取的速度也会慢很多。因此尽量不要读取大的CSV的方式来输入。
TF 官方还推荐了他自己的一种文件格式[standard TensorFlow format](https://www.tensorflow.org/versions/r0.10/how_tos/reading_data/index.html#standard-tensorflow-format), 我还 没用过，大家可以自己看看。
