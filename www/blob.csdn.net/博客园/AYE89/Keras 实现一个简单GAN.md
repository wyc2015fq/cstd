
# Keras 实现一个简单GAN - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Keras 实现一个简单GAN](https://www.cnblogs.com/eniac1946/p/8005083.html)
|Posted on|2017-12-08 14:00|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8005083)|[收藏](#)
|Keras 实现一个简单GAN
|代码中需提供：
|Loss Function  参见|[Keras](https://github.com/fchollet/keras/blob/master/keras/losses.py)|或者 Tensorflow 文档
|model_param_matrix   反向调整的模型参数/参数矩阵
|epoch 迭代轮数
|W 以及调整的方式
|import numpy as np
from keras.preprocessing import image
from keras.applications import inception_v3
from keras import backend as K
from PIL import Image
import tensorflow as tf
\#Prepare the input
\# Load the image
    img = image.load_img("name.png", target_size=(299, 299))
    original_image = image.img_to_array(img)
    \# Scale the image so all pixel intensities are between [-1, 1] as the model expects
    original_image /= 255.
    original_image -= 0.5
    original_image *= 2.
    \# Add a 4th dimension for batch size (as Keras expects)
    original_image = np.expand_dims(original_image, axis=0)
    \# Create a copy of the input image to process
    processed_image = np.copy(original_image)
    \# How much to update the hacked image in each iteration
    learning_rate = 0.01
    \# Define the cost function.
    cost_function = \#Loss Function\#
    \# We'll ask Keras to calculate the gradient based on the input image and the currently predicted class
    \#BP
    gradient_function = K.gradients(cost_function, model_param_matrix)[0]
    \# Create a Keras function that we can call to calculate the current cost and gradient
    grab_cost_and_gradients_from_model = K.function([model_input_layer, K.learning_phase()],
                                                    [cost_function, gradient_function])
    cost = 0.0
    epoch = 1000
    for iter in range(epoch):
        \# Check how close the image is to our target class and grab the gradients we
        \# can use to push it one more step in that direction.
        \# Note: It's really important to pass in '0' for the Keras learning mode here!
        \# Keras layers behave differently in prediction vs. train modes!
        cost, gradients = grab_cost_and_gradients_from_model([processed_image, 0])
        \# Adjust the params according to gradients (GD)
        W -= gradients * learning_rate
        print("Model's predicted likelihood that the image is a XXX: {:.8}%".format(cost * 100))







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
