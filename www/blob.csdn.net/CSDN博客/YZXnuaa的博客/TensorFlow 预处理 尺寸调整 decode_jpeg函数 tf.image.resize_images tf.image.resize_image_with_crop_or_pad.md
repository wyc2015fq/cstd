# TensorFlow 预处理 尺寸调整 decode_jpeg函数 tf.image.resize_images tf.image.resize_image_with_crop_or_pad - YZXnuaa的博客 - CSDN博客
2018年01月16日 13:31:55[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1387

其中： 
decode_jpeg函数为jpeg（jpg）图片解码的过程，对应的encode_jpeg函数为编码过程，编码后将图片重命名写入到指定的路径下。
**图像尺寸调整**
图像尺寸调整属于基础的图像几何变换，TensorFlow提供了几种尺寸调整的函数： 
tf.image.resize_images：将原始图像缩放成指定的图像大小，其中的参数method（默认值为ResizeMethod.BILINEAR）提供了四种插值算法，具体解释可以参考[图像几何变换（缩放、旋转）中的常用的插值算法](http://blog.csdn.net/chaipp0607/article/details/65658736)
tf.image.resize_image_with_crop_or_pad：剪裁或填充处理，会根据原图像的尺寸和指定的目标图像的尺寸选择剪裁还是填充，如果原图像尺寸大于目标图像尺寸，则在中心位置剪裁，反之则用黑色像素填充。
tf.image.central_crop：比例调整，central_fraction决定了要指定的比例，取值范围为(0，1]，该函数会以中心点作为基准，选择整幅图中的指定比例的图像作为新的图像。
