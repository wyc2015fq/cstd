# Faster RCNN代码详解（三）：数据处理的整体结构 - AI之路 - CSDN博客





2018年05月20日 08:31:18[AI之路](https://me.csdn.net/u014380165)阅读数：3510








在上一篇博客中介绍了Faster RCNN网络结构的构建：[Faster RCNN代码详解（二）：网络结构构建](https://blog.csdn.net/u014380165/article/details/80303812)。网络结构是Faster RCNN算法中最重要两部分之一，这**篇博客将介绍非常重要的另一部分：数据处理**。

**数据处理是通过AnchorLoader类实现的，该类所在脚本：~mx-rcnn/rcnn/core/loader.py，该类实现了数据处理的整体架构，是比较宏观的**。细节方面是通过assign_anchor函数实现的，该函数实现了关于anchor生成、正负样本界定等，代码在~/mx-rcnn/rcnn/io/rpn.py中，下一篇会介绍。

接下来就看看AnchorLoader类是怎么实现数据读取的吧（重点在于get_batch方法）。

```python
class AnchorLoader(mx.io.DataIter):
    def __init__(self, feat_sym, roidb, batch_size=1, shuffle=False, ctx=None, work_load_list=None,
                 feat_stride=16, anchor_scales=(8, 16, 32), anchor_ratios=(0.5, 1, 2), allowed_border=0, aspect_grouping=False):
        """
        This Iter will provide roi data to Fast R-CNN network
        :param feat_sym: to infer shape of assign_output
        :param roidb: must be preprocessed
        :param batch_size: must divide BATCH_SIZE(128)
        :param shuffle: bool
        :param ctx: list of contexts
        :param work_load_list: list of work load
        :param aspect_grouping: group images with similar aspects
        :return: AnchorLoader
        """
        super(AnchorLoader, self).__init__()

        # save parameters as properties
        self.feat_sym = feat_sym
        self.roidb = roidb
        self.batch_size = batch_size
        self.shuffle = shuffle
        self.ctx = ctx
        if self.ctx is None:
            self.ctx = [mx.cpu()]
        self.work_load_list = work_load_list
        self.feat_stride = feat_stride
        self.anchor_scales = anchor_scales
        self.anchor_ratios = anchor_ratios
        self.allowed_border = allowed_border
        self.aspect_grouping = aspect_grouping

        # infer properties from roidb
        self.size = len(roidb)
        self.index = np.arange(self.size)

        # decide data and label names
# 这部分定义的data_name、label_name和定义网络结构以及用module接口初始化model时定义的数据输入是一一对应的。
# 关于网络结构的输入可以参考~mx-rcnn/rcnn/symbol/symbol_resnet.py脚本的get_resnet_train函数。
        if config.TRAIN.END2END:
            self.data_name = ['data', 'im_info', 'gt_boxes']
        else:
            self.data_name = ['data']
        self.label_name = ['label', 'bbox_target', 'bbox_weight']

        # status variable for synchronization between get_data and get_label
        self.cur = 0
        self.batch = None
        self.data = None
        self.label = None

        # get first batch to fill in provide_data and provide_label
# 初始化调用reset方法进行一些变量的重置，get_batch方法用来读取第一个batch的数据，
# get_batch方法非常重要，包含了数据读取和处理相关的细节。
        self.reset()
        self.get_batch()

    @property
    def provide_data(self):
        return [(k, v.shape) for k, v in zip(self.data_name, self.data)]

    @property
    def provide_label(self):
        return [(k, v.shape) for k, v in zip(self.label_name, self.label)]

    def reset(self):
        self.cur = 0
        if self.shuffle:
            if self.aspect_grouping:
                widths = np.array([r['width'] for r in self.roidb])
                heights = np.array([r['height'] for r in self.roidb])
                horz = (widths >= heights)
                vert = np.logical_not(horz)
                horz_inds = np.where(horz)[0]
                vert_inds = np.where(vert)[0]
                inds = np.hstack((np.random.permutation(horz_inds), np.random.permutation(vert_inds)))
                extra = inds.shape[0] % self.batch_size
                inds_ = np.reshape(inds[:-extra], (-1, self.batch_size))
                row_perm = np.random.permutation(np.arange(inds_.shape[0]))
                inds[:-extra] = np.reshape(inds_[row_perm, :], (-1,))
                self.index = inds
            else:
                np.random.shuffle(self.index)

    def iter_next(self):
        return self.cur + self.batch_size <= self.size


# next方法是数据迭代器每次迭代数据时候调用的，在该方法中还是先通过get_batch()
# 得到一个batch数据，然后通过mx.io.DataBatch将数据封装成指定格式作为模型的输入。
    def next(self):
        if self.iter_next():
            self.get_batch()
            self.cur += self.batch_size
            return mx.io.DataBatch(data=self.data, label=self.label,
                                   pad=self.getpad(), index=self.getindex(),
                                   provide_data=self.provide_data, provide_label=self.provide_label)
        else:
            raise StopIteration

    def getindex(self):
        return self.cur / self.batch_size

    def getpad(self):
        if self.cur + self.batch_size > self.size:
            return self.cur + self.batch_size - self.size
        else:
            return 0

    def infer_shape(self, max_data_shape=None, max_label_shape=None):
        """ Return maximum data and label shape for single gpu """
        if max_data_shape is None:
            max_data_shape = []
        if max_label_shape is None:
            max_label_shape = []
        max_shapes = dict(max_data_shape + max_label_shape)
        input_batch_size = max_shapes['data'][0]
        im_info = [[max_shapes['data'][2], max_shapes['data'][3], 1.0]]
        _, feat_shape, _ = self.feat_sym.infer_shape(**max_shapes)
        label = assign_anchor(feat_shape[0], np.zeros((0, 5)), im_info,
                              self.feat_stride, self.anchor_scales, self.anchor_ratios, self.allowed_border)
        label = [label[k] for k in self.label_name]
        label_shape = [(k, tuple([input_batch_size] + list(v.shape[1:]))) for k, v in zip(self.label_name, label)]
        return max_data_shape, label_shape

# get_batch方法是读取数据的主要方法，该方法包含anchor的初始化、anchor标签的确定、
# 正负样本的确定等。该方法在数据初始化的时候会直接调用一次用来读取第一个batch的数据，
# 之后通过next方法每次迭代读取数据时候都会调用。
    def get_batch(self):
        # slice roidb
# 这部分是根据batch size的大小选择对应数量的输入数据。
        cur_from = self.cur
        cur_to = min(cur_from + self.batch_size, self.size)
        roidb = [self.roidb[self.index[i]] for i in range(cur_from, cur_to)]

        # decide multi device slice
        work_load_list = self.work_load_list
        ctx = self.ctx
        if work_load_list is None:
            work_load_list = [1] * len(ctx)
        assert isinstance(work_load_list, list) and len(work_load_list) == len(ctx), \
            "Invalid settings for work load. "
        slices = _split_input_slice(self.batch_size, work_load_list)

        # get testing data for multigpu
        data_list = []
        label_list = []
        for islice in slices:
            iroidb = [roidb[i] for i in range(islice.start, islice.stop)]
# get_rpn_batch()会对输入图像做短边resize到指定尺寸（默认是600），另外长边最大值
# 是1000，所以在对短边做resize后如果长边超过1000，则以长边resize到1000为准
# （短边从600按对应比例继续缩小）。需要注意的是box的标注坐标也会做对应的缩放。
# 得到的数据就放在data_list中，标注信息就放在label_list中
            data, label = get_rpn_batch(iroidb)
            data_list.append(data)
            label_list.append(label)

        # pad data first and then assign anchor (read label)
        data_tensor = tensor_vstack([batch['data'] for batch in data_list])
        for data, data_pad in zip(data_list, data_tensor):
            data['data'] = data_pad[np.newaxis, :]

        new_label_list = []
        for data, label in zip(data_list, label_list):
            # infer label shape
            data_shape = {k: v.shape for k, v in data.items()}
            del data_shape['im_info']
# self.feat_sym.infer_shape(**data_shape)是计算指定size的数据（data_shape:{'data':(1,3,600,800)}）
# 通过指定symbol（self.feat_sym）得到的输出size（feat_shape）。infer_shape方法的输入除了这种形式，
# 还可以用self.feat_sym.infer_shape(data=(1,3,600,800))，这里关键字data是在网络结构中定义的输入层名称。
            _, feat_shape, _ = self.feat_sym.infer_shape(**data_shape)
            feat_shape = [int(i) for i in feat_shape[0]]

            # add gt_boxes to data for e2e
            data['gt_boxes'] = label['gt_boxes'][np.newaxis, :, :]

            # assign anchor for label
# assign_anchor函数是给anchor分配标签的操作。输入中feat_shape是用于生成anchor的feature map维度，
# list格式，比如1*18*38*50，18是2*9的意思，9是anchor数量，2是背景和非背景2个类。
# label['gt_boxes']是x*5的numpy array，表示x个object的坐标和类别信息，是标注信息，
# 也就是ground truth，标注坐标是和图像大小对应的，ground truth主要用在anchor标签定义上。
# data['im_info']是1*3的numpy array，表示图像大小和缩放尺度信息。
# self.feat_stride是指特征缩放比例，比如16。self.anchor_scales默认是[8,16,32]。
# self.anchor_ratios默认是[0.5,1,2]。输出label是包含3个键值对的字典，分别是label["label"]、
# label["bbox_target"]、label["bbox_weights"]，这3个值都在RPN网络中用到 。
# 该函数的细节在~/mx-rcnn/rcnn/io/rpn.py中。
            label = assign_anchor(feat_shape, label['gt_boxes'], data['im_info'],
                                  self.feat_stride, self.anchor_scales,
                                  self.anchor_ratios, self.allowed_border)
            new_label_list.append(label)

        all_data = dict()
        for key in self.data_name:
            all_data[key] = tensor_vstack([batch[key] for batch in data_list])

        all_label = dict()
        for key in self.label_name:
            pad = -1 if key == 'label' else 0
            all_label[key] = tensor_vstack([batch[key] for batch in new_label_list], pad=pad)


# 最后返回的data是长度为3的列表，列表中每个值都是NDArray，分别是4维的图像内容
# data：（1,3,600,800）；2维的图像宽高和scale信息im_info：（1,3）；
# 3维的原始bounding box标注信息：gt_boxes：（1,x,5），x是object的数量。
# lable也是长度为3的列表，列表中每个值都是NDArray，分别是2维的anchor标签信息label：(1,17100)；
# 4维的anchor坐标回归target信息bbox_target:（1,36,38,50），
# 4维的anchor坐标权重信息bbox_weight：（1,36,38,50）。
        self.data = [mx.nd.array(all_data[key]) for key in self.data_name]
        self.label = [mx.nd.array(all_label[key]) for key in self.label_name]
```

这篇博客介绍了Faster RCNN算法中关于数据处理的整体结构，比较宏观，重要内容都在get_batch方法中，get_batch方法描述了Faster RCNN算法对数据读取的整体结构，这在很多后续的算法中都通用，因此当你了解了数据读取的整体结构后，就能举一反三了。

在get_batch方法中有个关于anchor的函数：assign_anchor。这应该也是本系列博客第一次提到anchor。anchor在Faster RCNN中是非常重要的概念，但很多新手对anchor的理解可能模棱两可，比如**anchor是什么？怎么生成的？anchor的标签是怎么定义的？bbox（bounding box）的回归目标是怎么定义的？bbox和anchor是什么区别？**如果你有这些疑问，那么下一篇博客我将为你解开anchor这个神秘的面纱：[Faster RCNN代码详解（四）：关于anchor的前世今生](https://blog.csdn.net/u014380165/article/details/80379812)。



