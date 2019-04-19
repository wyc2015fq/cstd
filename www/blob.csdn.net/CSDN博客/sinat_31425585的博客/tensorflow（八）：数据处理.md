# tensorflow（八）：数据处理 - sinat_31425585的博客 - CSDN博客
2019年02月24日 22:34:00[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：51
**1、制作tfrecords**
使用tf.train.Example来对数据和标签进行封装，然后采用tf.python_io.TFRecordWriter方法进行写操作。
```python
import tensorflow as tf
import os
import PIL.Image as Image
cwd = './card_data_v1.0'
classes = {'four', 'nine', 'one', 'rectangle' }
writer = tf.python_io.TFRecordWriter('train.tfrecords')
for index, name in enumerate(classes):
    class_path = cwd + '/' name + '/'
    for img_name in os.listdir(class_path):
        img_path = class_path + img_name
        img = Image.open(img_path)
        img = img.resize((256, 256))
        img_raw = img.tobytes()
        example = tf.train.Example(features=tf.train.Features(feature={
            "label": tf.train.Feature(int64_list=tf.train.Int64List(value=[index])),
            "img_raw":tf.train.Feature(bytes_list=tf.train.BytesList(value=[img_raw]))
                
            }))
        writer.write(example.SerializeToString())
writer.close()
```
**2、数据载入**
先使用tf.train.string_input_producer将tfrecords文件输出到一个输入管道队列，然后采用tf.TFRecorderReader方法进行读取，最后用tf.parse_single_example进行解析，这样就可以将数据和类别标签解析出来。
注意 tf.train.string_input_producer格式为：
```python
tf.train.string_input_producer(file_name, shuffle=False, num_epochs=5)
```
第一个参数为列表类型的文件名队列，第二个参数为是否打乱顺序，第三个为迭代epoch次数
```cpp
def read_record(record_name):
    filename_queue = tf.train.string_input_producer([record_name])
    reader = tf.TFRecordReader()
    _, serialized_example = reader.read(filename_queue)
    features = tf.parse_single_example(serialized_example, features={
        "label": tf.FixedLenFeature([], tf.int64),
        "image_raw": tf.FixedLenFeature([], tf.string),
    })
    label = features["label"]
    image = features["image_raw"]
    image = tf.decode_raw(image, tf.uint8)
    image = tf.reshape(image, [256, 256, 3])
    label = tf.cast(label, tf.int32)
    return image, label
```
**3、生成一个batch**
使用tf.train.shuffle_batch或tf.train.batch方法，从数据中取出batch张图片和对应标签
```python
image_batch, label_batch = tf.train.shuffle_batch([image, label],
                                                   batch_size=20, capacity=30, 
                                                   min_after_dequeue=10)
```
最后，测试程序为：
```python
image, label = read_record("./card_data_v1.0/train.tfrecords")
print(image, label)
image_batch, label_batch = tf.train.shuffle_batch([image, label], batch_size=4, capacity=10, min_after_dequeue=5)
init = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init)
thread = tf.train.start_queue_runners(sess=sess)
for i in range(10):
    images, labels = sess.run([image_batch, label_batch])
    print("batch shape = ", image.shape, "labels = ", labels)
    print("label = ", labels)
    for j in range(4):
        plt.subplot(1, 4, j+1)
        plt.axis("off")
        plt.imshow(images[j])
```
这里要注意一下：tf.train.start_queue_runners，没有调用的话，整个系统处于"停滞"状态，因为只有当调用了tf.train.start_queue_runners之后，文件名队列才会被加载到内存中，计算单元就可以拿到数据进行计算了。
总结一下：
第一步：使用tf.train.string_input_producer建立队列；
第二步：使用reader.read读取文件；
第三步：调用tf.train.start_queue_runners，将文件名队列加载到内存中；
最   后：通过sess.run()获取读取图片的结果。
**参考资料：**
[[1] https://blog.csdn.net/xuan_zizizi/article/details/78431490](https://blog.csdn.net/xuan_zizizi/article/details/78431490)
[[2] https://blog.csdn.net/sinat_34474705/article/details/78966064](https://blog.csdn.net/sinat_34474705/article/details/78966064)
[3] 21个项目玩转深度学习
