# Tensorflow迁移学习 - wsp_1138886114的博客 - CSDN博客





2018年08月02日 09:33:02[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：87标签：[Tensorflow迁移学习](https://so.csdn.net/so/search/s.do?q=Tensorflow迁移学习&t=blog)
个人分类：[深度学习](https://blog.csdn.net/wsp_1138886114/article/category/7729523)









### 一、迁移学习的应用价值

```
迁移学习的应用
    图像识别：图像增强，风格转移，对象检测，皮肤癌检测
    文字识别：Zero Shot翻译、情绪分类

应用价值
    复用现有知识数据，已有的大量工作不至于完全丢弃
    不需要在话费巨大代价重新采集和标定庞大数据（也可能无法获取数据）
    对于快速出现的新领域，能够快速迁移和应用，体现时效性优势
```

### 二、Tensorflow 迁移学习

```python
import glob
import os.path
import random
import numpy as np
import tensorflow as tf
from tensorflow.python.platform.gfile
```

##### 2.1参数设置

```
# 瓶颈点的节点个数
BOTTLENECK_TENSOR_SIZE = 200

# 瓶颈点的张量名称
BOTTLENECK_TENSOR_NAME = 'pool_3/reshape:0'

# 图像输入张量所对应的名称
JPEG_DATA_TENSOR_NAME = 'DecodeJpeg/contents:0'

# model info
MODEL_DIR = './model/'
MODEL_FILE = 'tensorflow_inception_gragh.pb'

# 缓存文件夹
CACHE_DIR = './tmp/bottleneck/'

# 图像输入文件夹
INPUT_DATA = './flower_data/'

# 验证和测试数据百分比
VALIDATION_PRECENTAGE = 10
TEST_PRECENTAGE = 10

# 定义神经网络参数
LEARNING_DATA = 0.01
STEPS = 4000
BATCH = 100
```

##### 2.2 数据集拆分

```python
def cteate_image_lists(testing_percentage,validation_percentage):
    result = {}    #得到的结果
    sub_dir = [x[0] for x in os.walk(INPUT_DATA)]
    is_root_dir = True
    for sub_dir in sub_dirs:
        if is_root_dir:
            is_root_dir = False
            continue

        #获取有效图片
        extensions = ['jpg','jpeg','JPG','JPEG']
        file_list = []
        dir_name = os.path.basename(sub_dir)
        for extension in extensions:
            file_glob = os.path.join(INPUT_DATA,dir_name,'*.'+extension)
            filr_list.extend(glob.glob(file_glob))
        if not file_list:
            continue

        #类别名
        label_name = dir_name.lower()
        training_images = []
        testing_images  = []
        validation_images = []

        for file_name in file_list:
            base_name = os.path.basename(file_name)
            #随机拆分数据集
            chance = np.random.randin(100)
            if chance < validation_percentage:
                validation_images.append(base_name)
            elif chance < (testing_percentage + validation_percentage):
                testing_images.append(base_name)
            else:
                training_images.append(base_name)

        #把结果放在字典中
        result[label_name] = {
            'dir': dir_name,
            'training':training_images,
            'testing':testing_images,
            'validation':validation_images
        }
    return result
```

##### 2.3 获取图像路径

```
# index图片的编号
def get_image_path(image_lists,image_dir,label_name,index,category):
    label_lists = image_lists[label_name]
    category_list = label_lists[category]
    mod_index = index % len(category_list)
    base_name = category_list[mod_index]
    sub_dir = label_lists('dir')

    #full_path
    full_path = os.path.join(images_dir,sub_dir,base_name)
    return

#get bottleneck path
def get_bottlenect_path(images_dir,label_name,index,category)
    return get_image_path(image_lists,CACHE_DIR,label_name,index,category)+'.txt'
```

##### 2.4 处理图片获取特征向量

```python
def run_bottlenect_on_image(sess,image_data,image_data_tensor,bottleneck_tensor):
    bottleneck_values = sess.run(bottleneck_tensor,
                                 feed_dict={image_data_tensor:image_data})
    bottleneck_values = np.squeeze(bottleneck_values)
    return bottleneck_values

def get_or_create_bottleneck(sess,image_lists,label_name,index,
                             category,jpeg_data_tensor,bottleneck_tensor):
    label_lists = image_lists(label_name)
    sub_dir = label_lists('dir')
    sub_dir_path = os.path.join(CACHE_DIR,sub_dir)
    if not os.path.exists(sub_dir_path):
        os.makedirs(sub_dir_path)
    bottleneck_path = get_bottlenect_path(image_lists,label_name,index,category)

    if not os.path.exists(bottleneck_path):
        image_path = get_image_path(image_lists,INPUT_DATA,label_name,index,category)
        image_data = gfile.FastGFile(image_path,'rb').read()
        bottleneck_values = run_bottlenect_on_image(sess,image_data,jpeg_data_tensor,
                                                    bottleneck_tensor)
        bottleneck_string = ','.join(str(x) for x in bottleneck_values)
        with open(bottleneck_path,'w') as bottleneck_file:
            bottleneck_file.write(bottleneck_string)
    else:
        with open(bottleneck_path,'r') as bottleneck_file:
            bottleneck_string = bottleneck_file.read()
        bottleneck_values = [float(x) for x in bottleneck_string.split(',')]
    return bottleneck_values
```

##### 2.5 数据集的获取

```python
def get_random_cache_bottlenecks(sess,n_classes,image_lists,how_many,category,
                                jpeg_data_tensor,bottleneck_tensor):
    bottlenecks = []
    ground_truths = []
    for i in range(how_many):
        label_index = random.randrange(n_classes)
        label_name = list(image_lists.keys())[label_index]
        image_index = random.randrange(65536)
        bottleneck = get_or_create_bottleneck(sess,image_lists,label_name,
                                              image_index,category,
                                              jpeg_data_tensor,bottleneck_tensor)
        ground_truth = np.zeros(n_classes,dtype = np.float32)
        ground_truth[label_index] = 1.0
        bottlenecks.append(bottleneck)
        ground_truths.append(ground_truth)
    return bottlenecks,ground_truths

def get_test_bottlenecks(sess,n_classes,image_lists,jpeg_data_tensor,bottleneck_tensor):
    bottlenecks = []
    ground_truths = []
    label_name_list = list(image_lists.keys())
    for label_index,label_name in enumerate(label_name_list):
        category = 'testing'
        for index,unused_base_name in enumerate(image_lists,[label_name],[category]):
            bottleneck = get_or_create_bottleneck(sess,image_lists,label_name,index,
                                                  category,jpeg_data_tensor,
                                                  bottleneck_tensor)
            ground_truth = np.zeros(n_classes,dtype=np.float32)
            ground_truth[label_index] = 1.0
            bottlenecks.append(bottleneck)
            ground_truths.append(ground_truth)
    return bottleneck,ground_truths
```

##### 2.6 主函数的定义

```python
def main(_):
    #读取所有图片
    image_lists = create_image_lists(TEST_PRECENTAGE,VALIDATION_PRECENTAGE)
    n_classes = len(image_lists.keys())

    with gfile.FastGFile(os.path.join(MODEL_DIR,MODEL_FILE),'rb') as f:
        graph_def = tf.GraphDef()
        graph_def.ParseFromString(f.read())
    bottleneck_tensor,jpeg_data_tensor = tf.import_graph_def(graph_def,
                                         return_elements=[BOTTLENECK_TENSOR_NAME,
                                                          JPEG_DATA_TENSOR_NAME])
    bottleneck_input = tf.placeholder(tf.float32,[BOTTLENECK_TENSOR_SIZE],
                                      name='BottleneckInputPlaceholder')                                                        
    ground_truth_input = tf.placeholder(tf.float32,[None,n_classes],
                                        name='GroundTruthInput')

    #定义全连接层
    with tf.name_scope('final_training_ops'):
        weights = tf.Variable(tf.truncated_normal([BOTTLENECK_TENSOR_SIZE,n_classes],
                              stddev=0.001))
        biases = tf.Variable(tf.zeros([n_classes]) + 0.001)
        logits = tf.matmul(bottleneck_input,weights) + biases #y = wx + b
        final_tensor = tf.nn.softmax(logits)

    #loss交叉熵
    cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=logits,
                    labels=ground_truth_input)
    cross_entropy_mean = tf.reduce_mean(cross_entropy)
    train_step = tf.train.GradientDescentOptimizer(LEARNING_DATA).\
                 minimize(cross_entropy_mean)

    #accuracy
    with tf.name_scope('evaluation'):
        correct_prediction = tf.equal(tf.argmax(final_tensor,1),
                                  tf.argmax(ground_truth_input,1))
        evaluation_step = tf.reduce_mean(tf.cast(correct_prediction,tf.float32)) 

    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        for i in range(STEPS):
            train_bottlenecks,train_ground_truth = get_random_cached_bottlenecks( 
                                                sess,n_classes,
                                                image_lists,BATCH,"training" 
                                                jpeg_data_tensor, 
                                                bottleneck_tensor) 
            sess.run(train_step,feed_dict={bottleneck_input:train_bottlenecks,
                                       ground_truth_input:train_ground_truth}) 

            #在训练集验证
            if i%100 ==0 or i+1 ==STEPS:
                validation_bottlenecks,validation_ground_truth = get_random_cached_bottlenecks(sess,n_classes,
                                                image_lists,BATCH,"training" 
                                                jpeg_data_tensor, 
                                                bottleneck_tensor) 
                validation_accuracy = sess.run(evaluation_step, 
                                                feed_dict={bottleneck_input:validation_bottlenecks, 
                                                           ground_truth_input:validation_ground_truth}) 
                print('Step{}valiation accuracy on random sampled{}example = {}%'.format(i,BATCH,valuation_accuracy*100)) 

        #在测试集验证
        test_bottlenecks,test_ground_truth = get_test_bottlenecks(sess,n_classes,
                                                image_lists,BATCH, 
                                                jpeg_data_tensor, 
                                                bottleneck_tensor) 
        test_accuracy = sess.run(evaluation_step, 
                                feed_dict={bottleneck_input:test_bottlenecks, 
                                           ground_truth_input:test_ground_truth}) 
        print('Fanal test accuracy ={}%'.format(test_accuracy*100)) 

if __name__ = "__main__" 
    tf.app.run()
```



