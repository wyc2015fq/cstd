# GoogLeNet_1×1的卷积核 - wsp_1138886114的博客 - CSDN博客





2018年08月03日 11:47:07[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：280
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)











- - - [1×1的卷积核](#11的卷积核)- - [1x1卷积核优化计算: 示例](#1x1卷积核优化计算-示例)
- [1x1卷积层代替全连接层的好处](#1x1卷积层代替全连接层的好处)
- [1x1卷积其它优点](#1x1卷积其它优点)
- [卷积核替换](#卷积核替换)
- [可变形卷积核(deformable convolution)](#可变形卷积核deformable-convolution)








### 1×1的卷积核

一般1×1的卷积核本质上并没有对图像做什么聚合操作，以为就是同一个$w$去乘以原图像上的每一个像素点，相当于做了一个scaling  

1×1卷积核最初是在Network in Network这个网络结构中提出来的。它用了比AlexNet更少的参数，达到了跟其一样的效果。 
- 1×1卷积核的作用
那么1×1卷积核有什么作用呢，如果当前层和下一层都只有一个通道那么1×1卷积核确实没什么作用。  

如果它们分别为m层和n层的话，1×1卷积核可以起到一个跨通道聚合的作用，所以进一步可以起到降维（或者升维）的作用，起到减少参数的目的。比如当前层为 $ x×x×m$ 即图像大小为($x×x$，特征层数为$m$)然后如果将其通过$1×1$的卷积核，特征层数为$n，$那么只要$n<m$ 这样就能起到降维的目的，减少之后步骤的运算量（当然这里不太严谨，需要考虑1×1卷积核本身的参数个数为m×n个）。换句话说，如果使用1x1的卷积核，这个操作实现的就是多个feature map的线性组合，可以实现feature map在通道个数上的变化。  

而因为卷积操作本身就可以做到各个通道的重新聚合的作用，所以1×1的卷积核也能达到这个效果。
##### 1x1卷积核优化计算: 示例

假设输入特征图的大小是 28x28x192；1x1 的卷积通道为64 ；3x3 的卷积通道为128；5x5 的卷积通道为32。 
![这里写图片描述](https://img-blog.csdn.net/20180803105708955?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 左边的卷积核参数计算：
192x(1x1x64) + 192x(3x3x128) + 192x(5x5x32) = 387072- 右图的卷积核计算：
（3x3 卷积核层加入通道为：96的1x1卷积核 ；5x5 的特征图后面加入通道为：16 的1x1卷积核 ）192x(1x1x64) + 【192x1x1x96+96x3x3x128】+ 【192x1x1x16+16x5x5x32】=157184
##### 1x1卷积层代替全连接层的好处

这样做其实有非常多的好处，比如上面的例子中输入是224x224x3 的图像，如果此时图像变得更大了，变成384x384大小的了，那么一开始按照32作为步长来进行卷积操作，最后还按照这个网络结构能得到一个[6×6×1000]层的，那么前面那个[6×6]有什么用呢，这个代表每一个位置上，其属于1000个分类结果中的打分，所以这在图像分割等领域等领域有着非常重要的作用  

##### 1x1卷积其它优点
- 实现了跨通道的信息组合，并增加非线性特征
使用1x1卷积核，实现降维和升维的操作其实就是channel间信息的线性组合变化，  

3*3 ,64channels的卷积核前面添加一个1*1 , 28channels的卷积核，就变成了3*3 , 28channels的卷积核，原来64个channels就可以理解为跨通道线性组合变成了28channels,这就是通道间的信息交互。  

因为1*1卷积核，可以在保持feature map尺度不变的（即不损失分辨率）的前提下大幅增加非线性特性（利用后接的非线性激活函数），把网络做的很deep ,增加非线性特性.
##### 卷积核替换
- 把一个5x5的拆成两个3x3的，或者把7x7的拆成1x7和7x1的:
感受野相同的情况下，参数量减少了也就是计算量减少了，增加了非线性
##### 可变形卷积核(deformable convolution)

卷积核一般都是长方形或正方形，但MSRA提出了一个相当反直觉的见解， 

认为卷积核的形状可以是变化的，变形的卷积核能让它只看感兴趣的图像区域 ，这样识别出来的特征更佳。 
![这里写图片描述](https://img-blog.csdn.net/20180803161913647?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

要做到这个操作，可以直接在原来的过滤器前面再加一层过滤器，这层过滤器学习的是下一层卷积核的位置偏移量（offset），这样只是增加了一层过滤器，或者直接把原网络中的某一层过滤器当成学习offset的过滤器，这样实际 

增加的计算量是相当少的，但能实现可变形卷积核，识别特征的效果更好。
```python
def __init__(self, filters, init_normal_stddev=0.01, **kwargs):

    self.filters = filters
    super(ConvOffset2D, self).__init__(
        self.filters * 2, (3, 3), padding='same', use_bias=False,
        kernel_initializer=RandomNormal(0, init_normal_stddev),
        **kwargs
    )
def call(self, x):
    """Return the deformed featured map"""

    #获取x大小，x大小为（b,h,w,c)，分别为batch_size,图片高度，图片宽度，特征图大小

    x_shape = x.get_shape()

    #调用普通卷积获得输出，输出结果为(b,h,w,2c)表示图片中每个像素需要偏移的量（x,y)
    offsets = super(ConvOffset2D, self).call(x)
     #reshape一下输出，方便后续操作，(b*c,h,w,2)表示共有b*c个图片，每个图片为h*w大小，每个像素对应2个方向
    # offsets: (b*c, h, w, 2)    
    offsets = self._to_bc_h_w_2(offsets, x_shape)
    #将原始输入也重新reshape一下方便后续操作
    # x: (b*c, h, w)
    x = self._to_bc_h_w(x, x_shape)
    #调用deform_conv.py中的函数根据原始图片与偏移量生成新图片数据。
    # X_offset: (b*c, h, w)
    x_offset = tf_batch_map_offsets(x, offsets)
    # x_offset: (b, h, w, c)
    x_offset = self._to_b_h_w_c(x_offset, x_shape)
    return x_offset
def compute_output_shape(self, input_shape):
    """Output shape is the same as input shape
    Because this layer does only the deformation part
    """
    return input_shape
@staticmethod
def _to_bc_h_w_2(x, x_shape):
    """(b, h, w, 2c) -> (b*c, h, w, 2)"""
    x = tf.transpose(x, [0, 3, 1, 2])
    x = tf.reshape(x, (-1, int(x_shape[1]), int(x_shape[2]), 2))
    return x
@staticmethod
def _to_bc_h_w(x, x_shape):
    """(b, h, w, c) -> (b*c, h, w)"""
    x = tf.transpose(x, [0, 3, 1, 2])
    x = tf.reshape(x, (-1, int(x_shape[1]), int(x_shape[2])))
    return x
@staticmethod
def _to_b_h_w_c(x, x_shape):
    """(b*c, h, w) -> (b, h, w, c)"""
    x = tf.reshape(
        x, (-1, int(x_shape[3]), int(x_shape[1]), int(x_shape[2]))
    )
    x = tf.transpose(x, [0, 2, 3, 1])
    return x

deform_conv.py主要代码：

def tf_flatten(a):
    """Flatten tensor"""
    return tf.reshape(a, [-1])

def tf_repeat(a, repeats, axis=0):
    """TensorFlow version of np.repeat for 1D"""
    # https://github.com/tensorflow/tensorflow/issues/8521
    assert len(a.get_shape()) == 1
    a = tf.expand_dims(a, -1)
    a = tf.tile(a, [1, repeats])
    a = tf_flatten(a)
    return a
def tf_repeat_2d(a, repeats):
    """Tensorflow version of np.repeat for 2D"""
    assert len(a.get_shape()) == 2
    a = tf.expand_dims(a, 0)
    a = tf.tile(a, [repeats, 1, 1])
    return a
def tf_map_coordinates(input, coords, order=1):
    """Tensorflow verion of scipy.ndimage.map_coordinates
    Note that coords is transposed and only 2D is supported
    Parameters
    ----------
    input : tf.Tensor. shape = (s, s)
    coords : tf.Tensor. shape = (n_points, 2)
    """
    assert order == 1
    coords_lt = tf.cast(tf.floor(coords), 'int32')
    coords_rb = tf.cast(tf.ceil(coords), 'int32')
    coords_lb = tf.stack([coords_lt[:, 0], coords_rb[:, 1]], axis=1)
    coords_rt = tf.stack([coords_rb[:, 0], coords_lt[:, 1]], axis=1)

    vals_lt = tf.gather_nd(input, coords_lt)
    vals_rb = tf.gather_nd(input, coords_rb)
    vals_lb = tf.gather_nd(input, coords_lb)
    vals_rt = tf.gather_nd(input, coords_rt)


    coords_offset_lt = coords - tf.cast(coords_lt, 'float32')
    vals_t = vals_lt + (vals_rt - vals_lt) * coords_offset_lt[:, 0]
    vals_b = vals_lb + (vals_rb - vals_lb) * coords_offset_lt[:, 0]
    mapped_vals = vals_t + (vals_b - vals_t) * coords_offset_lt[:, 1]
    return mapped_vals
def sp_batch_map_coordinates(inputs, coords):
    """Reference implementation for batch_map_coordinates"""
    coords = coords.clip(0, inputs.shape[1] - 1)
    mapped_vals = np.array([
        sp_map_coordinates(input, coord.T, mode='nearest', order=1)
        for input, coord in zip(inputs, coords)
    ])
    return mapped_vals
def tf_batch_map_coordinates(input, coords, order=1):
    """Batch version of tf_map_coordinates
    Only supports 2D feature maps
    Parameters
    ----------
    input : tf.Tensor. shape = (b, s, s)
    coords : tf.Tensor. shape = (b, n_points, 2)
    Returns
    -------
    tf.Tensor. shape = (b, s, s)
    """
    input_shape = tf.shape(input)
    batch_size = input_shape[0]
    input_size = input_shape[1]
    n_coords = tf.shape(coords)[1]
    coords = tf.clip_by_value(coords, 0, tf.cast(input_size, 'float32') - 1)

    #得到目标坐标左上角（left top）的整数坐标
    coords_lt = tf.cast(tf.floor(coords), 'int32')

    #得到又下角的整数坐标
    coords_rb = tf.cast(tf.ceil(coords), 'int32')

    #得到左下角的整数坐标
    coords_lb = tf.stack([coords_lt[..., 0], coords_rb[..., 1]], axis=-1)

    #得到右上角的整数坐标
    coords_rt = tf.stack([coords_rb[..., 0], coords_lt[..., 1]], axis=-1)

    #idx为索引展开，idx大小为（b*c*h*w)，形如(0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3)

    #b*c为5，h*w为4，总数为所有图片所有坐标总数

    idx = tf_repeat(tf.range(batch_size), n_coords)
    def _get_vals_by_coords(input, coords):

        #stack完后，每一个点表示一个坐标

        #形如

        #（0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3）

        # (3,2,1,2,3,1,2,3,0,0,0,3,2,1,1,2,3,2,0,0,2)

            # (3,2,1,0,0,2,0,3,1,2,3,0,0,2,3,0,1,2,0,2,3)

        indices = tf.stack([
            idx, tf_flatten(coords[..., 0]), tf_flatten(coords[..., 1])
        ], axis=-1)
        vals = tf.gather_nd(input, indices)
        vals = tf.reshape(vals, (batch_size, n_coords))
        return vals
     #以下为分别得到左上，左下，右上，右下四个点的像素值。
    vals_lt = _get_vals_by_coords(input, coords_lt)
    vals_rb = _get_vals_by_coords(input, coords_rb)
    vals_lb = _get_vals_by_coords(input, coords_lb)
    vals_rt = _get_vals_by_coords(input, coords_rt)
     #用双线性插值得到像素值。
    coords_offset_lt = coords - tf.cast(coords_lt, 'float32')
    vals_t = vals_lt + (vals_rt - vals_lt) * coords_offset_lt[..., 0]
    vals_b = vals_lb + (vals_rb - vals_lb) * coords_offset_lt[..., 0]
    mapped_vals = vals_t + (vals_b - vals_t) * coords_offset_lt[..., 1]
    return mapped_vals
def sp_batch_map_offsets(input, offsets):
    """Reference implementation for tf_batch_map_offsets"""
    batch_size = input.shape[0]
    input_size = input.shape[1]
    #生成grid，grid表示将一个图片的所有坐标变成两列，每一行两个元素表示x，y

    （grid的最后大小为(b*c,h*w,2)
    offsets = offsets.reshape(batch_size, -1, 2)
    grid = np.stack(np.mgrid[:input_size, :input_size], -1).reshape(-1, 2)
    grid = np.repeat([grid], batch_size, axis=0)

    #将原始坐标与坐标偏移量相加，得到目标坐标，coords的大小为(b*c,h*w,2)
    coords = offsets + grid

    #目标坐标需要在图片最大坐标范围内，将目标坐标进行切割限制
    coords = coords.clip(0, input_size - 1)
    #根据原始输入与目标坐标得到像素。
    mapped_vals = sp_batch_map_coordinates(input, coords)
    return mapped_vals
def tf_batch_map_offsets(input, offsets, order=1):
    """Batch map offsets into input
    Parameters
    ---------
    input : tf.Tensor. shape = (b, s, s)
    offsets: tf.Tensor. shape = (b, s, s, 2)
    Returns
    -------
    tf.Tensor. shape = (b, s, s)
    """
    input_shape = tf.shape(input)
    batch_size = input_shape[0]
    input_size = input_shape[1]
    offsets = tf.reshape(offsets, (batch_size, -1, 2))
    grid = tf.meshgrid(
        tf.range(input_size), tf.range(input_size), indexing='ij'
    )
    grid = tf.stack(grid, axis=-1)
    grid = tf.cast(grid, 'float32')
    grid = tf.reshape(grid, (-1, 2))
    grid = tf_repeat_2d(grid, batch_size)
    coords = offsets + grid
    mapped_vals = tf_batch_map_coordinates(input, coords)
    return mapped_vals
```





