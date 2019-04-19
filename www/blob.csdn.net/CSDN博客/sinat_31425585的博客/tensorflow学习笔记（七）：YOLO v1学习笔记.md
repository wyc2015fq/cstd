# tensorflow学习笔记（七）：YOLO v1学习笔记 - sinat_31425585的博客 - CSDN博客
2018年08月29日 15:28:18[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：339
1、网络结构
![](https://img-blog.csdn.net/20180826002425748?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                              图1 yolo v1 的网络结构（摘自文献[You only look once](http://172.16.24.181/www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Redmon_You_Only_Look_CVPR_2016_paper.pdf)）
       这里，所有卷积操作都是'SAME'方式，所以以步长为1的卷积操作过程中，不会影响输出feature map的width和height，feature map大小变化源自于卷积步长和pooling池化操作，而这两种因素都保留了feature map中元素与输入图像块之间的相对位置关系。因此，尺寸为448x448大小图像，经过一系列卷积层、下采样层之后，最终输出7x7大小feature map，feature map中每个cell对应于输入图像中图像块大小为：448/7 = 64，相当于将输入图像分割成7x7个图像块，就可以将图像与输出feature map对应起来，但是，由于网络的输入从224x224缩放到448x448，所以，实际上图像块大小为32x32，这里对应于论文中说的将图像分成SxS个格子。
        构建网络的代码为：
```python
def build_network(self, images, keep_prob=0.5, is_training=True, scope='yolo'):
        with tf.variable_scope(scope):
            with slim.arg_scope([slim.conv2d, slim.fully_connected],
                                activation_fn=leaky_relu(self.alpha),
                                weights_initializer=tf.truncated_normal_initializer(0.0, 0.01),
                                weights_regularizer=slim.l2_regularizer(0.0005)):
                # padding：上下左右、上下左右...
                net = tf.pad(images, np.array([[0, 0], [3, 3], [3, 3], [0, 0]]), name='pad_1')
                # 经过padding之后，相当于'SAME'方式的conv
                # c = 64, f = 7, s = 2   ==> 64 x 224 x 224
                net = slim.conv2d(net, 64, 7, 2, padding='VALID', scope='conv_2')
                # max-pooling
                # f = 2, c = 2, p = 'SAME' ==> 64 x 112 x 112
                net = slim.max_pool2d(net, 2, padding='SAME', scope='pool_3')
                # c = 192, f = 3, s = 1    ==> 192 x 112 x  112
                net = slim.conv2d(net, 192, 3, scope='conv_4')
                # max-pooling
                # f = 2, c = 2, p = 'SAME' ==> 192 x 56 x 56
                net = slim.max_pool2d(net, 2, padding='SAME', scope='pool_5')
                # 默认padding为'SAME'
                # c = 128, f = 1, s = 1   ==> 128 x 56 x 56
                net = slim.conv2d(net, 128, 1, scope='conv_6')
                # c = 256, f = 3, s = 1   ==> 256 x 56 x 56
                net = slim.conv2d(net, 256, 3, scope='conv_7')
                # c = 256, f = 1, s = 1   ==> 256 x 56 x 56
                net = slim.conv2d(net, 256, 1, scope='conv_8')
                # c = 512, f = 3, s = 1   ==> 512 x 56 x 56
                net = slim.conv2d(net, 512, 3, scope='conv_9')
                # f = 2, s = 2  ==> 512 x 28 x 28
                net = slim.max_pool2d(net, 2, padding='SAME', scope='pool_10')
                # c = 256, f = 1, s = 1   ==> 256 x 28 x 28
                net = slim.conv2d(net, 256, 1, scope='conv_11')
                # c = 512, f = 3, s = 1, ==> 512 x 28 x 28
                net = slim.conv2d(net, 512, 3, scope='conv_12')
                # c = 256, f = 1, s = 1, ==> 256 x 28 x 28
                net = slim.conv2d(net, 256, 1, scope='conv_13')
                # c = 512, f = 3, s = 1, ==> 512 x 28 x 28
                net = slim.conv2d(net, 512, 3, scope='conv_14')
                # c = 256, f = 1, s = 1, ==> 256 x 28 x 28
                net = slim.conv2d(net, 256, 1, scope='conv_15')
                # c = 512, f = 3, s = 1, ==> 512 x 28 x 28
                net = slim.conv2d(net, 512, 3, scope='conv_16')
                # c = 256, f = 1, s = 1, ==> 256 x 28 x 28
                net = slim.conv2d(net, 256, 1, scope='conv_17')
                # c = 512, f = 3, s = 1, ==> 512 x 28 x 28
                net = slim.conv2d(net, 512, 3, scope='conv_18')
                # c = 512, f = 1, s = 1, ==> 512 x 28 x 28
                net = slim.conv2d(net, 512, 1, scope='conv_19')
                # c = 1024, f = 3, s = 1, ==> 1024 x 28 x 28
                net = slim.conv2d(net, 1024, 3, scope='conv_20')
                # f = 2, s = 2,   ==> 1024 x 14 x 14
                net = slim.max_pool2d(net, 2, padding='SAME', scope='pool_21')
                # c = 512, f = 1, s = 1, ==> 512 x 14 x 14
                net = slim.conv2d(net, 512, 1, scope='conv_22')
                # c = 1024, f = 3, s = 1, ==> 1024 x 14 x 14
                net = slim.conv2d(net, 1024, 3, scope='conv_23')
                # c = 512, f = 1, s = 1, ==> 512 x 14 x 14
                net = slim.conv2d(net, 512, 1, scope='conv_24')
                # c = 1024, f = 3, s = 1, ==> 1024 x 14 x 14
                net = slim.conv2d(net, 1024, 3, scope='conv_25')
                # c = 1024, f = 3, s = 1, ==> 1024 x 14 x 14
                net = slim.conv2d(net, 1024, 3, scope='conv_26')
                # 相当于padding = 'SAME'的conv
                net = tf.pad(net, np.array([[0, 0], [1, 1], [1, 1], [0, 0]]), name='pad_27')
                # c = 1024, f = 3, s = 2  ==> 1024 x 7 x 7
                net = slim.conv2d(net, 1024, 3, 2, padding='VALID', scope='conv_28')
                # c = 1024, f = 3, s = 1, ==> 1024 x 7 x 7
                net = slim.conv2d(net, 1024, 3, scope='conv_29')
                # c = 1024, f = 3, s = 1, ==> 1024 x 7 x 7
                net = slim.conv2d(net, 1024, 3, scope='conv_30')
                # ==> 7 x 7 x 1024
                net = tf.transpose(net, [0, 3, 1, 2], name='trans_31')
                net = slim.flatten(net, scope='flat_32')
                net = slim.fully_connected(net, 512, scope='fc_33')
                net = slim.fully_connected(net, 4096, scope='fc_34')
                net = slim.dropout(net, keep_prob=keep_prob, is_training=is_training, scope='dropout_35')
                net = slim.fully_connected(net, self.output_size, activation_fn=None, scope='fc_36')
        return net
```
2、输出7x7x30
           YOLO最后输出的7x7x30中，7x7表示最后输出feature map大小，每一个位置对应于输入图像的一个cell，如图2所示。
![](https://img-blog.csdn.net/20180829150145167?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                   图2 cell对应第一个box信息（摘自deepsystems.io）
        每个cell对应于一个1x30的向量，前面10位对应于位置及置信度信息，由于每个cell对应两个box，而每个box对应于一个(x, y, w, h, c)，因此当前cell对应的第一个box信息如图2所示，当前cell对应第二个box信息如图3所示。
![](https://img-blog.csdn.net/20180829150634600?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                    图3 cell对应第二个box信息（摘自deepsystems.io）
            后面20位对应类别信息，是对于类别信息的编码，每个cell，对应于每个box都有一个类别编码值，如图4和5所示。
![](https://img-blog.csdn.net/20180829151200392?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                  图 4  box1的类别信息编码（摘自deepsystems.io）
![](https://img-blog.csdn.net/20180829151248692?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                        图5 box2的类别信息编码（摘自deepsystems.io）
        因此，7x7个cell对应于49x2=98个20x1的类别信息，如图6所示：
![](https://img-blog.csdn.net/20180829151516598?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                图6 7x7个cell对应的类别信息（摘自deepsystems.io）
3、检测过程
         如图7所示，首先，按照box的得分score（![](https://private.codecogs.com/gif.latex?)if score < threshold1(0.2), then Set score to zero）判断当前box中是否存在目标物体；然后，对box的得分score，按照从大到小的顺序进行排序；其次，采用NMS（非极大值抑制）策略对box进行进一步筛选；最后，将得分scorce值大于0的框显示出来，即最后检测结果。
![](https://img-blog.csdn.net/20180829152631101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                           图7 YOLO目标检测流程（摘自deepsystems.io）
代码：
```python
def main():
    parser = argparse.ArgumentParser()
    # 训练好的权重名
    parser.add_argument('--weights', default="YOLO_v.ckpt-10750", type=str)#YOLO_small.ckpt
    # 训练好权重所在路径
    parser.add_argument('--weight_dir', default='output', type=str)
    parser.add_argument('--data_dir', default="data", type=str)
    parser.add_argument('--gpu', default= '', type=str)
    args = parser.parse_args()
    os.environ['CUDA_VISIBLE_DEVICES'] = args.gpu # gpu
    yolo = YOLONet(False) # 网络结构
    # 权重路径+权重文件名
    weight_file = os.path.join(args.data_dir, args.weight_dir, args.weights)
    detector = Detector(yolo, weight_file) # 载入训练好的检测器
    # Detect Image
    imname = './test/1.jpg'
    detector.image_detector(imname)
if __name__ == '__main__':
    main()
```
    这个就是检测器的检测实现代码，我们再看:
```python
detector = Detector(yolo, weight_file)
detector.image_detector.image_detector(imname)
```
   检测器的主体部分：
```python
class Detector(object):
    def __init__(self, net, weight_file):
        self.net = net
        self.weights_file = weight_file
        self.classes = cfg.CLASSES
        self.num_class = len(self.classes)
        self.image_size = cfg.IMAGE_SIZE
        self.cell_size = cfg.CELL_SIZE
        self.boxes_per_cell = cfg.BOXES_PER_CELL
        self.threshold = cfg.THRESHOLD          # score阈值
        self.iou_threshold = cfg.IOU_THRESHOLD  # iou 阈值
        # 类别
        self.boundary1 = self.cell_size * self.cell_size * self.num_class
        # 每个cell对应两个box
        self.boundary2 = self.boundary1 + self.cell_size * self.cell_size * self.boxes_per_cell
        self.sess = tf.Session() # 声明会话
        self.sess.run(tf.global_variables_initializer()) # 变量初始化
        print('Restoring weights from: ' + self.weights_file)
        self.saver = tf.train.Saver()
        # 权重文件中读取训练好的权重
        self.saver.restore(self.sess, self.weights_file)
    def draw_result(self, img, result):
        colors = self.random_colors(len(result))
        for i in range(len(result)):
            x = int(result[i][1])
            y = int(result[i][2])
            w = int(result[i][3] / 2)
            h = int(result[i][4] / 2)
            color = tuple([rgb * 255 for rgb in colors[i]])
            cv2.rectangle(img, (x - w, y - h), (x + w, y + h), color, 3)
            cv2.putText(img, result[i][0], (x - w - 3, y - h - 15), cv2.FONT_HERSHEY_SIMPLEX, 2, color, 2)
            print(result[i][0],': %.2f%%' % (result[i][5]*100))
    def detect(self, img):
        img_h, img_w, _ = img.shape # 图像宽和高
        inputs = cv2.resize(img, (self.image_size, self.image_size))  # 缩放尺度至448x448
        inputs = cv2.cvtColor(inputs, cv2.COLOR_BGR2RGB).astype(np.float32)
        inputs = (inputs / 255.0) * 2.0 - 1.0  # 像素值归一化
        inputs = np.reshape(inputs, (1, self.image_size, self.image_size, 3))
        # 将图像作为输入，得到网络的输出结果
        result = self.detect_from_cvmat(inputs)[0]  
        # 检测结果还原到实际位置
        for i in range(len(result)):
            result[i][1] *= (1.0 * img_w / self.image_size) # 计算当前box在原来图像中大小
            result[i][2] *= (1.0 * img_h / self.image_size)
            result[i][3] *= (1.0 * img_w / self.image_size)
            result[i][4] *= (1.0 * img_h / self.image_size)
        return result
    # 对opencv的Mat数据进行检测
    def detect_from_cvmat(self, inputs):
        # 网络输出
        net_output = self.sess.run(self.net.logits, feed_dict={self.net.images: inputs})
        results = []
        for i in range(net_output.shape[0]):  # 网络的输出结果
            results.append(self.interpret_output(net_output[i])) # NMS
        return results
    def interpret_output(self, output):
        probs = np.zeros((self.cell_size, self.cell_size, self.boxes_per_cell, self.num_class))
        # 类别：boundary1：cell_size x cell_size x num_class
        class_probs = np.reshape(output[0:self.boundary1], (self.cell_size, self.cell_size, self.num_class))
        scales = np.reshape(output[self.boundary1:self.boundary2], (self.cell_size, self.cell_size, self.boxes_per_cell))
        # cell_size x cell_size x boxes_per_cell x 4：bnd box的四个坐标量
        boxes = np.reshape(output[self.boundary2:], (self.cell_size, self.cell_size, self.boxes_per_cell, 4))
        # 包含两个步骤：reshape  14x7  -> 2 x 7 x 7
        # 第二个步骤：transpose  2 x 7 x 7 -> 7 x 7 x 2
        offset = np.transpose(np.reshape(np.array([np.arange(self.cell_size)] * self.cell_size * self.boxes_per_cell),
                                         [self.boxes_per_cell, self.cell_size, self.cell_size]), (1, 2, 0))#7*7*2
        boxes[:, :, :, 0] += offset
        boxes[:, :, :, 1] += np.transpose(offset, (1, 0, 2))
        boxes[:, :, :, :2] = 1.0 * boxes[:, :, :, 0:2] / self.cell_size
        boxes[:, :, :, 2:] = np.square(boxes[:, :, :, 2:])
        boxes *= self.image_size
        for i in range(self.boxes_per_cell):
            for j in range(self.num_class):
                probs[:, :, i, j] = np.multiply(class_probs[:, :, j], scales[:, :, i])
        filter_mat_probs = np.array(probs >= self.threshold, dtype='bool')
        filter_mat_boxes = np.nonzero(filter_mat_probs) # 大于概率阈值
        boxes_filtered = boxes[filter_mat_boxes[0], filter_mat_boxes[1], filter_mat_boxes[2]]
        probs_filtered = probs[filter_mat_probs]
        classes_num_filtered = np.argmax(filter_mat_probs, axis=3)[filter_mat_boxes[0], filter_mat_boxes[1], filter_mat_boxes[2]]
        argsort = np.array(np.argsort(probs_filtered))[::-1] # 按照score进行排序
        boxes_filtered = boxes_filtered[argsort]  # 按照排序后的顺序调整box顺序
        probs_filtered = probs_filtered[argsort]  # 按照排序后的顺序调整score顺序
        classes_num_filtered = classes_num_filtered[argsort]
        for i in range(len(boxes_filtered)):
            if probs_filtered[i] == 0:
                continue
            for j in range(i + 1, len(boxes_filtered)): # 计算IOU，然后使用NMS
                if self.iou(boxes_filtered[i], boxes_filtered[j]) > self.iou_threshold:
                    probs_filtered[j] = 0.0
        filter_iou = np.array(probs_filtered > 0.0, dtype='bool') # score大于0的部分
        boxes_filtered = boxes_filtered[filter_iou]  # boxes
        probs_filtered = probs_filtered[filter_iou]  # scores
        classes_num_filtered = classes_num_filtered[filter_iou]  # 看最后还保存的类别
        result = []
        for i in range(len(boxes_filtered)):  # 将这些类别及位置返还
            result.append([self.classes[classes_num_filtered[i]], boxes_filtered[i][0], boxes_filtered[
                          i][1], boxes_filtered[i][2], boxes_filtered[i][3], probs_filtered[i]])
        return result
    # 计算交并比
    def iou(self, box1, box2):
        tb = min(box1[0] + 0.5 * box1[2], box2[0] + 0.5 * box2[2]) - \
            max(box1[0] - 0.5 * box1[2], box2[0] - 0.5 * box2[2])
        lr = min(box1[1] + 0.5 * box1[3], box2[1] + 0.5 * box2[3]) - \
            max(box1[1] - 0.5 * box1[3], box2[1] - 0.5 * box2[3])
        if tb < 0 or lr < 0:
            intersection = 0
        else:
            intersection = tb * lr
        return intersection / (box1[2] * box1[3] + box2[2] * box2[3] - intersection)
    def random_colors(self, N, bright=True):
        brightness = 1.0 if bright else 0.7
        hsv = [(i / N, 1, brightness) for i in range(N)]
        colors = list(map(lambda c: colorsys.hsv_to_rgb(*c), hsv))
        np.random.shuffle(colors)
        return colors
    # 视频检测
    def camera_detector(self, cap, wait=30):
        while(1):
            ret, frame = cap.read()
            result = self.detect(frame)
            self.draw_result(frame, result)
            cv2.imshow('Camera', frame)
            cv2.waitKey(wait)
            if cv2.waitKey(wait) & 0xFF == ord('q'):
                break
        cap.release()
        cv2.destroyAllWindows()
    # 图像检测
    def image_detector(self, imname, wait=0):
        image = cv2.imread(imname)
        result = self.detect(image)
        self.draw_result(image, result)
        cv2.imshow('Image', image)
        cv2.waitKey(wait)
```
    首先，将图片直接放到训练好的网络中，得到一个输出结果；然后，使用score阈值过滤掉得分较低的box；其次，使用NMS来对box做进一步筛选；最后，将结果还原到实际尺度，并显示输出结果。
     这里有一个使用YOLO1检测仪表的效果：[http://dwz.cn/2KZnHRKP](http://dwz.cn/2KZnHRKP)
4、训练
（1） 数据处理：
      这个部分代码位于utils\pascal_voc.py文件中，主要分为数据和标签两个部分：
```python
# 训练
    def next_batches(self, gt_labels, batch_size):
        # n x w x h x c
        images = np.zeros((batch_size, self.image_size, self.image_size, 3))
        # n x cell_size x cell_size x (class + 5)：输入只有一个位置
        labels = np.zeros((batch_size, self.cell_size, self.cell_size, self.num_class + 5))
        count = 0
        while count < batch_size:
            # 当前样本文件名
            imname = gt_labels[self.cursor]['imname']
            # 镜像标志
            flipped = gt_labels[self.cursor]['flipped']
            # 读取样本：gray -> normalize
            images[count, :, :, :] = self.image_read(imname, flipped)
            # 获取标签
            labels[count, :, :, :] = gt_labels[self.cursor]['label']
            # 读取下一个样本
            count += 1
            self.cursor += 1
            # 如果样本数目小于bacth_size
			# 将样本随机打乱顺序
            if self.cursor >= len(gt_labels):
                np.random.shuffle(gt_labels)
                self.cursor = 0
                self.epoch += 1
        return images, labels
```
    数据读写部分位于image_read，标签读取位于gt_labels
    数据读取步骤主要有：尺度缩放、灰度化、像素值归一化和镜像处理
```python
# 使用opencv接口读取样本图像
    def image_read(self, imname, flipped=False):
        image = cv2.imread(imname)
        # 保证尺度一致
        image = cv2.resize(image, (self.image_size, self.image_size))
        # 灰度化处理
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB).astype(np.float32)
        # 像素值归一化
        image = (image / 255.0) * 2.0 - 1.0
        # 镜像操作
        if flipped:
            image = image[:, ::-1, :]
        return image
```
标签读取步骤主要有：获取训练样本图片所在路径、读取标注文件中boungding box信息，并进行编码
```python
# 载入样本标签
    def load_labels(self, model):
        # 训练
        if model == 'train':
            # 样本数据所在路径
            self.devkil_path = os.path.join(cfg.PASCAL_PATH, 'VOCdevkit')
            self.data_path = os.path.join(self.devkil_path, 'VOC2007')
            txtname = os.path.join(self.data_path, 'ImageSets', 'Main', 'trainval.txt')
        # 测试
        if model == 'test':
            self.devkil_path = os.path.join(cfg.PASCAL_PATH, 'VOCdevkit')
            self.data_path = os.path.join(self.devkil_path, 'VOC2007')
            txtname = os.path.join(self.data_path, 'ImageSets', 'Main', 'test.txt')
        # 读取训练样本名
        with open(txtname, 'r') as f:
            self.image_index = [x.strip() for x in f.readlines()]
        gt_labels = []
        for index in self.image_index:
            # 读取bnd box信息，并进行编码， num：一张样本中目标物体数目
            label, num = self.load_pascal_annotation(index)
            if num == 0:
                continue
            imname = os.path.join(self.data_path, 'JPEGImages', index + '.jpg')
            gt_labels.append({'imname': imname, 'label': label, 'flipped': False})
        return gt_labels
```
    读取标注文件中bounding box信息实现代码位于load_pascal_annotation中：首先，读取样本图片及对应标注文件中目标物体的bounding box信息；然后，根据样本实际大小与送入网络中样本大小（448x448）之间比例，找到目标物体的对应位置；最后，根据目标物体中心与cell之间位置关系，对bounding box进行编码——距离目标物体中心最近的cell负责对当前目标物体进行检测，所以其实，每个样本对应于一个7x7x(num_class + 5)的矩阵。
```python
# 读取样本的标记
    def load_pascal_annotation(self, index):
        imname = os.path.join(self.data_path, 'JPEGImages', index + '.jpg')
        # 读取样本图像数据
        im = cv2.imread(imname)
        # 将样本的坐标归一化
        h_ratio = 1.0 * self.image_size / im.shape[0]
        w_ratio = 1.0 * self.image_size / im.shape[1]
        # 样本标签：cell_size x cell_size x (num_class + 5)
		# 每个cell需要预测(num_class + 5)个值
		# 分别对应：类别数目 + 4个坐标 + 1个置信度
		# 表明：当前样本属于某个类别的置信度及坐标位置
        label = np.zeros((self.cell_size, self.cell_size, self.num_class + 5))
        # 样本标记文件
        filename = os.path.join(self.data_path, 'Annotations', index + '.xml')
        # xml解析文件
        tree = ET.parse(filename)
        # 获取object属性
        objs = tree.findall('object')
        for obj in objs:
            # 获取object属性对应的子属性bndbox
			# bounding box
            bbox = obj.find('bndbox')
            # 建立bbox在输入image和feature map cell上对应位置关系
            x1 = max(min((float(bbox.find('xmin').text)) * w_ratio, self.image_size), 0)
            y1 = max(min((float(bbox.find('ymin').text)) * h_ratio, self.image_size), 0)
            x2 = max(min((float(bbox.find('xmax').text)) * w_ratio, self.image_size), 0)
            y2 = max(min((float(bbox.find('ymax').text)) * h_ratio, self.image_size), 0)
            # 查找类别名对应索引
            cls_ind = self.class_to_ind[obj.find('name').text.lower().strip()]
            # 中心位置，及宽、高
            boxes = [(x2 + x1) / 2.0, (y2 + y1) / 2.0, x2 - x1, y2 - y1]
            # bounding box 对应cell_size x cell_size网格中位置
            x_ind = int(boxes[0] * self.cell_size / self.image_size)
            y_ind = int(boxes[1] * self.cell_size / self.image_size)
			
            # 如果已经标记了，表明当前位置存在物体
            if label[y_ind, x_ind, 0] == 1:
                continue
            # 对当前cell进行标记
            label[y_ind, x_ind, 0] = 1            # 置信度
            label[y_ind, x_ind, 1:5] = boxes      # 坐标
            label[y_ind, x_ind, 5 + cls_ind] = 1  # 类别
        return label, len(objs)
```
（2） 损失函数
     这个部分位于yolo\yolo_net.py文件中：
```python
if is_training:
            self.labels = tf.placeholder(tf.float32, [None, self.cell_size, self.cell_size, 5 + self.num_class])
            self.loss_layer(self.logits, self.labels)
            self.total_loss = tf.losses.get_total_loss()
            tf.summary.scalar('total_loss', self.total_loss)
```
   具体loss在这个loss_layer中：
```python
# 定义损失层
    def loss_layer(self, predicts, labels, scope='loss_layer'):
        with tf.variable_scope(scope):
            # class
            # tf.reshape(tensor, shape, name=None)：将tensor变换为参数shape的形式
            # boundary1 = cell_size x cell_size x num_classes
            # N x cell_size x cell_size x num_classes -> [N, cell_size, cell_size, num_classes]
            predict_classes = tf.reshape(predicts[:, :self.boundary1], [self.batch_size, self.cell_size, self.cell_size, self.num_class])
            # bb：confidence
            # [N, cell_size, cell_size, boxes_per_cell]
            predict_scales = tf.reshape(predicts[:, self.boundary1:self.boundary2], [self.batch_size, self.cell_size, self.cell_size, self.boxes_per_cell])
            # (dx, dy, dw, dh)
            # [N, cell_size, cell_size, boxes_per_cell, 4]
            predict_boxes = tf.reshape(predicts[:, self.boundary2:], [self.batch_size, self.cell_size, self.cell_size, self.boxes_per_cell, 4])
            # 响应：batch_size * cell_size * cell_size * 1
            # [N, cell_size, cell_size, 1]
            response = tf.reshape(labels[:, :, :, 0], [self.batch_size, self.cell_size, self.cell_size, 1])
            # [N, cell_size, cell_size, 1, 4]
            boxes = tf.reshape(labels[:, :, :, 1:5], [self.batch_size, self.cell_size, self.cell_size, 1, 4])
            # tf.tile：张量扩展
            # tf.tile(raw, multiples=[a, b, c, d])
            # 将raw的第0维输入a次，第1维输入b次，第2维输入c次，第3维输入d次
            # [N, cell_size, cell_size, boxes_per_cell, 4]
            boxes = tf.tile(boxes, [1, 1, 1, self.boxes_per_cell, 1]) / self.image_size
            # 输入为：[N, cell_size, cell_size, boxes + class_num]
            # labels[:, :, :, 5:]为class对应编码
            classes = labels[:, :, :, 5:]
            # 初始化为一个常量: [cell_size, cell_size, boxes_per_cell]
            offset = tf.constant(self.offset, dtype=tf.float32)
            # [1, cell_size, cell_size, boxes_per_cell]
            offset = tf.reshape(offset, [1, self.cell_size, self.cell_size, self.boxes_per_cell])
            # [N, cell_size, cell_size, boxes_per_cell]
            offset = tf.tile(offset, [self.batch_size, 1, 1, 1])
            # shape为 [4, N, cell_size, cell_size, boxes_per_cell]
            predict_boxes_tran = tf.stack([1. * (predict_boxes[:, :, :, :, 0] + offset) / self.cell_size,
                                           1. * (predict_boxes[:, :, :, :, 1] + tf.transpose(offset, (0, 2, 1, 3))) / self.cell_size,
                                           tf.square(predict_boxes[:, :, :, :, 2]), # 开根号
                                           tf.square(predict_boxes[:, :, :, :, 3])])
            # shape为 [batch_size, 7, 7, 2, 4]
            # tf.transpose(input, [dimension_1, dimenaion_2,..,dimension_n]):
            # 这个函数主要适用于交换输入张量的不同维度用的
            # [N, cell_size, cell_size, boxes_per_cell, 4]
            predict_boxes_tran = tf.transpose(predict_boxes_tran, [1, 2, 3, 4, 0])
            # 计算IOU: 交并比
            iou_predict_truth = self.calc_iou(predict_boxes_tran, boxes)
            # calculate I tensor [BATCH_SIZE, CELL_SIZE, CELL_SIZE, BOXES_PER_CELL]
            # 计算iou_predict_truth在第3个维度上的最大值
            object_mask = tf.reduce_max(iou_predict_truth, 3, keep_dims=True)
            object_mask = tf.cast((iou_predict_truth >= object_mask), tf.float32) * response
            # calculate no_I tensor [CELL_SIZE, CELL_SIZE, BOXES_PER_CELL]
            noobject_mask = tf.ones_like(object_mask, dtype=tf.float32) - object_mask
            boxes_tran = tf.stack([1. * boxes[:, :, :, :, 0] * self.cell_size - offset,
                                   1. * boxes[:, :, :, :, 1] * self.cell_size - tf.transpose(offset, (0, 2, 1, 3)),
                                   tf.sqrt(boxes[:, :, :, :, 2]),
                                   tf.sqrt(boxes[:, :, :, :, 3])])
            # 参数中加上平方根是对 w 和 h 进行开平方操作，原因在论文中有说明
            # #shape为(4, batch_size, 7, 7, 2)
            boxes_tran = tf.transpose(boxes_tran, [1, 2, 3, 4, 0])
            # class_loss 分类损失
            class_delta = response * (predict_classes - classes)
            class_loss = tf.reduce_mean(tf.reduce_sum(tf.square(class_delta), axis=[1, 2, 3]), name='class_loss') * self.class_scale
            # object_loss 有目标物体存在的损失
            object_delta = object_mask * (predict_scales - iou_predict_truth)
            object_loss = tf.reduce_mean(tf.reduce_sum(tf.square(object_delta), axis=[1, 2, 3]), name='object_loss') * self.object_scale
            # noobject_loss 没有目标物体时的损失
            noobject_delta = noobject_mask * predict_scales
            noobject_loss = tf.reduce_mean(tf.reduce_sum(tf.square(noobject_delta), axis=[1, 2, 3]), name='noobject_loss') * self.noobject_scale
            # coord_loss 坐标损失 #shape 为 (batch_size, 7, 7, 2, 1)
            coord_mask = tf.expand_dims(object_mask, 4)
            # shape 为(batch_size, 7, 7, 2, 4)
            boxes_delta = coord_mask * (predict_boxes - boxes_tran)
            coord_loss = tf.reduce_mean(tf.reduce_sum(tf.square(boxes_delta), axis=[1, 2, 3, 4]), name='coord_loss') * self.coord_scale
            # 将所有损失放在一起
            tf.losses.add_loss(class_loss)
            tf.losses.add_loss(object_loss)
            tf.losses.add_loss(noobject_loss)
            tf.losses.add_loss(coord_loss)
            # 将每个损失添加到日志记录
            tf.summary.scalar('class_loss', class_loss)
            tf.summary.scalar('object_loss', object_loss)
            tf.summary.scalar('noobject_loss', noobject_loss)
            tf.summary.scalar('coord_loss', coord_loss)
            tf.summary.histogram('boxes_delta_x', boxes_delta[:, :, :, :, 0])
            tf.summary.histogram('boxes_delta_y', boxes_delta[:, :, :, :, 1])
            tf.summary.histogram('boxes_delta_w', boxes_delta[:, :, :, :, 2])
            tf.summary.histogram('boxes_delta_h', boxes_delta[:, :, :, :, 3])
            tf.summary.histogram('iou', iou_predict_truth)
```
对应于论文中定义的loss函数：
![](https://img-blog.csdn.net/20180829175920643?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这仅仅是自己学习的一个笔记，如果有地方不妥之处，欢迎大家批评指证，谢谢！
参考资料：
deepsystem.io：[https://goo.gl/eFcsTv](https://www.youtube.com/redirect?q=https%3A%2F%2Fgoo.gl%2FeFcsTv&redir_token=Q66oznHBO8FLtCLesAkSZEV5KHB8MTUzNTYxNDI4NUAxNTM1NTI3ODg1&event=video_description&v=L0tzmv--CGY)
Andrew NG的deeplearning.ai课程
[https://github.com/leeyoshinari/YOLO_v1](https://github.com/leeyoshinari/YOLO_v1)
