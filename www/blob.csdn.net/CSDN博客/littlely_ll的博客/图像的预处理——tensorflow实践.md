# 图像的预处理——tensorflow实践 - littlely_ll的博客 - CSDN博客





2018年02月05日 22:34:05[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：853








# 图像预处理

```
import matplotlib.pyplot as plt
import tensorflow as tf

#tf.gfile.FastGFile读取或保存图像文件
image_raw_data = tf.gfile.FastGFile(".../input_data/cat.jpeg",'r').read()

with tf.Session() as sess:
    #图形解码(可以解码jpeg, png，编码为encode_jpeg)
    img_data = tf.image.decode_jpeg(image_raw_data)
    print(img_data.eval())
    plt.imshow(img_data.eval())
    plt.show()

import numpy as np
with tf.Session() as sess:
    #设定图片大小，method有4中插值,分别为0,1,2,3
    resized = tf.image.resize_images(img_data, [300, 300], method=0)
    print("Digital dtype: %s" % resized.dtype)
    # TensorFlow的函数处理图片后存储的数据是float32格式的，需要转换成uint8才能正确打印图片。
    cat = np.asarray(resized.eval(), dtype="uint8")
    print(resized.get_shape())

    plt.imshow(cat)
    plt.show()

#图形剪切或填充
with tf.Session() as sess:
    croped = tf.image.resize_image_with_crop_or_pad(img_data, 300,200)
    padded = tf.image.resize_image_with_crop_or_pad(img_data, 700, 500)
    plt.imshow(croped.eval())
    plt.show()
    plt.imshow(padded.eval())
    plt.show()

with tf.Session() as sess:
    central_cropped = tf.image.central_crop(img_data, 0.5)
    plt.imshow(central_cropped.eval())
    plt.show()

#图形翻转变换
with tf.Session() as sess:
    #上下翻转
    flipped = tf.image.flip_up_down(img_data)
    #左右反转
    flipped1 = tf.image.flip_left_right(img_data)
    #对角翻转
    transposed = tf.image.transpose_image(img_data)

    plt.imshow(flipped.eval())
    plt.show()
    plt.imshow(flipped1.eval())
    plt.show()
    plt.imshow(transposed.eval())
    plt.show()

##图形色彩调整

#调整亮度
with tf.Session() as sess:
    adjusted = tf.image.adjust_brightness(img_data, -0.5)
    adjusted1 = tf.image.adjust_brightness(img_data, 0.5)
    adjusted2 = tf.image.random_brightness(img_data, max_delta=0.5)

    plt.imshow(adjusted.eval())
    plt.show()
    plt.imshow(adjusted1.eval())
    plt.show()
    plt.imshow(adjusted2.eval())
    plt.show()

#调整对比度
with tf.Session() as sess:
    adjusted = tf.image.adjust_contrast(img_data, -5)
    adjusted1 = tf.image.adjust_contrast(img_data, 5)
    adjusted2 = tf.image.random_contrast(img_data, 0,5)

    plt.imshow(adjusted.eval())
    plt.show()
    plt.imshow(adjusted1.eval())
    plt.show()
    plt.imshow(adjusted2.eval())
    plt.show()

#调整色相
with tf.Session() as sess:
    adjusted = tf.image.adjust_hue(img_data, 0.1)
    adjusted1 = tf.image.adjust_hue(img_data, 0.5)
    adjusted2 = tf.image.adjust_hue(img_data, 0.9)
    adjusted3 = tf.image.random_hue(img_data, max_delta=0.4)

    plt.imshow(adjusted.eval())
    plt.show()
    plt.imshow(adjusted1.eval())
    plt.show()
    plt.imshow(adjusted2.eval())
    plt.show()
    plt.imshow(adjusted3.eval())
    plt.show()

#调整饱和度
with tf.Session() as sess:
    adjusted = tf.image.adjust_saturation(img_data, -5)
    adjusted1 = tf.image.adjust_saturation(img_data, 5)
    adjusted2 = tf.image.random_saturation(img_data,3,7)

    plt.imshow(adjusted.eval())
    plt.show()
    plt.imshow(adjusted1.eval())
    plt.show()
    plt.imshow(adjusted2.eval())
    plt.show()

#处理标注框
with tf.Session() as sess:         
    boxes = tf.constant([[[0.05, 0.05, 0.4, 0.6]]])
    begin, size, bbox_for_draw = tf.image.sample_distorted_bounding_box(tf.shape(img_data), bounding_boxes=boxes)
    batched = tf.expand_dims(tf.image.convert_image_dtype(img_data, tf.float32), 0) 
    image_with_box = tf.image.draw_bounding_boxes(batched, bbox_for_draw)
    distorted_image = tf.slice(img_data, begin, size)

    plt.imshow(image_with_box[0].eval())
    plt.show()
    plt.imshow(distorted_image.eval())
    plt.show()
```



