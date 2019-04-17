# 全面梳理 faster-rcnn 之训练数据是如何读入的 - Snoopy_Dream - CSDN博客





2018年04月25日 21:14:04[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：110
个人分类：[faster-rcnn](https://blog.csdn.net/e01528/article/category/7599535)









下面我和大家一起从训练最开始学习作者如何将原始数据读入并通过RoIDataLayer转化成网络训练所需的数据的总体过程。

## 训练从./tools/train_net.py开始，进入主函数，我们只关注跟数据有关的模块。

首先是imdb, roidb = combined_roidb(args.imdb_name)语句**，传入的参数imdb_name默认是“voc_2007_trainval”，这只是一个数据集的名字而已，进入该文件下的**combined_roidb**函数，该函数首先有个内部函数**get_roidb**，下面有调用，进入这函数第一行**imdb = get_imdb（imdb_name）**，这个get_imdb为**factory**中的函数，细看可以知道**get_imdb这个函数通过给数据集的名字返回了该数据集对应的类的对象。

到现在我们可以通过一些变量名知道imdb即影像数据集，roidb即感兴趣区域的数据集，factory即为工厂，它的函数get_imdb根据名字返回数据集对应的类的对象。**假如**imdb_name为“voc_2007_trainval”，我们可以知道get_imdb返回的是**pascal_voc(‘trainval’, ‘2007’)**，而进入**pascal_voc.py**文件，我们可以看到他是一个继承于imdb类的子类（class pascal_voc(imdb):），在init函数中，我们可以知道该类记录了文件路径目标类别还有影像扩展名等与数据集相关的内容，而父类imdb中则是名字等一般性的内容。

了解了这些内容以后，我们回到**combined_roidb函数**的**get_roidb函数**继续往下读，通过get_imdb记录了数据集的路径之类的信息后。get_roidb函数又调用了语句**imdb.set_proposal_method(cfg.TRAIN.PROPOSAL_METHOD)**，在config.py中我们看到cfg.TRAIN.PROPOSAL_METHOD值是selective_search，但在/experiments/cfgs/faster_rcnn_end2end.yml中它的值是**gt**，而imdb的**set_proposal_method**函数里**为method = eval(‘self.’ + method + ‘_roidb’)    self.roidb_handler = method**其中eval函数把字符串当成表达式求值，从而我们知道下一行调用了**gt_roidb函数**，进入pascal_voc下的gt_roidb函数，其中关于cache相当于缓存加速未来调用的，可以不管。

我们看到该函数核心语句**gt_roidb = [self._load_pascal_annotation(index)  for index in self.image_index]**在数据集类pascal_voc中我们知道**image_index记录的是所有影像名的列表**，可以知道**对每张影像名调用_load_pascal_annotation**，进入该函数一眼可以看到在路径后面加了“Annotations”即可以知道该函数读取的是影像对应存储目标的xml文件。最后返回了**“boxes”，“gt_classes”等构成的字典**。

随后get_roidb函数调用了**get_training_roidb（imdb）**，进入**fast_rcnn下的train.py**文件，进入get_training_roidb函数，调用的append_flipped_images函数通过名字可以看出是增加翻转的，可以不管，然后该函数又调用了**prepare_roidb**，从上面我们已经知道image_index记录的是所有影像名的列表，在roidb.py中prepare_roidb通过循环**for i in xrange(len(imdb.image_index)**)对每张影像在上面**gt_roidb**函数读取到的内容**“gt_overlaps”**进行了求最大处理，并记录影像路径还有影像高宽等信息在roidb中并返回，返回的**roidb是一个长度与影像数一致的列表**。由此知该函数即如函数名所示做准备prepare。

现在回到combined_roidb函数，语句roidbs = [get_roidb(s) for s in imdb_names.split(‘+’)]我们可以知道对于每个数据集返回了带有该数据集信息的imdb和包含每张影像感兴趣区域信息的roidb（包括每张影像点目标和影像宽高等信息），事实上roidb属于imdb。

继续往下看，**train_net.py**调用了**train_net(args.solver, roidb, output_dir,pretrained_model=args.pretrained_model,max_iters=args.max_iters)**函数，此函数为**fast_rcnn/train.py**中的**train_net**函数该函数首先调用了filter_roidb，如函数名即对roi进行过滤，然后调用了SolverWrapper，即是solver的封装类的对象，在init函数中，开始都是导入网络之类的准备性工作，在最后一句话中，调用了

### self.solver.net.layers[0].set_roidb(roidb)##这句话很关键，

### 即调用了网络第一层也即是prototxt文件的第一层即

### RoIDataLayer层的set_roidb函数，把之前读取的roidb对象传进了网络。

到现在我们再来看**roidb对象**里到底记录了哪些内容呢？

### 包括数据集所有影像的影像路径，所有影像对应的目标，以及宽高等信息。

有了这些信息，就可以在RoIDataLayer层的前向传播函数调用get_next_minibatch，该函数首先调用了_get_next_minibatch_inds此批次运行的影像下标，然后获取他们对应的roidb对象后组成列表，传进get_minibatch函数。在RoIDataLayer层中输出为data，im_info和gt_boxes，在get_minibatch函数中，他把所有的输出信息弄成blob以便进行前向传播。data通过roidb的“image”记录的路径信息然后调用opencv的读影像函数进行读取并进行尺度缩放后输出，对应代码为im_blob, im_scales = _get_image_blob(roidb, random_scale_inds)   blobs = {‘data’: im_blob}，而其余的gt_boxes信息之前已经保存，可以直接将之前的信息保存到对应的blob结构中，最后输出。

### blobs[‘gt_boxes’] = gt_boxes

### blobs[‘im_info’] = np.array(  [[im_blob.shape[2], im_blob.shape[3], im_scales[0]]], dtype=np.float32)，

### 最后返回blob，在前向传播中输出top layer。



