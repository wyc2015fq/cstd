
# Keras之inception_v3使用 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Keras之inception_v3使用](https://www.cnblogs.com/eniac1946/p/7809263.html)
|Posted on|2017-11-09 14:35|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7809263)|[收藏](#)
|一、安装
|必要：tensorflow，Keras
|首次运行需要安装：
|1）下载模型权重   inception_v3_weights_tf_dim_ordering_tf_kernels.h5
|路径见前一篇
|2）安装h5py
|pip install h5py
|3）安装PIL
|遇到pip无法安装，以pillow替代，见Stack Overflow
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171109142248434-207780966.png)
|二、参数说明
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171109142530747-1358843702.png)
|分类结果：
|ImageNet的1000种object，对应模型分类结果的1000 classes：
|[text: imagenet 1000 class id to human readable labels](https://gist.github.com/yrevar/942d3a0ac09ec9e5eb3a)
|[https://github.com/cjyanyi/keras_deep_learning_tutorial/blob/master/imagenet1000_clsid_to_human.txt](https://github.com/cjyanyi/keras_deep_learning_tutorial/blob/master/imagenet1000_clsid_to_human.txt)
|三、代码示例
|import numpy as np
from keras.preprocessing import image
from keras.applications import inception_v3
　  img = image.load_img("xxx.jpg", target_size=(299, 299))
　  input_image = image.img_to_array(img)
　  input_image /= 255.
　  input_image -= 0.5
　  input_image *= 2.
　  \# Add a 4th dimension for batch size (Keras)
　  input_image = np.expand_dims(input_image, axis=0)
    \# Run the image through the NN
    predictions = model.predict(input_image)
    \# Convert the predictions into text
    predicted_classes = inception_v3.decode_predictions(predictions, top=1)
    imagenet_id, name, confidence = predicted_classes[0][0]
    print("This is a {} with {:-4}% confidence!".format(name, confidence * 100))
|input_image 是一个默认大小：1*299*299*3  的4维向量（列表）







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
