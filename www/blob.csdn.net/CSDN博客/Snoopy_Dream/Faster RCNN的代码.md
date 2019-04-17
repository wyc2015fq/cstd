# Faster RCNN的代码 - Snoopy_Dream - CSDN博客





2018年04月17日 20:37:38[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：56









最近开始学习深度学习，看了下Faster RCNN的代码，在学习的过程中也查阅了很多其他人写的博客，得到了很大的帮助，所以也打算把自己一些粗浅的理解记录下来，一是记录下自己的菜鸟学习之路，方便自己过后查阅，二来可以回馈网络。目前编程能力有限，且是第一次写博客，中间可能会有一些错误。

# 目录


- [目录](https://blog.csdn.net/u011956147/article/details/53053381#%E7%9B%AE%E5%BD%95)- - [第一步准备](https://blog.csdn.net/u011956147/article/details/53053381#%E7%AC%AC%E4%B8%80%E6%AD%A5%E5%87%86%E5%A4%87)
- [第二步Stage 1 RPN init from ImageNet model](https://blog.csdn.net/u011956147/article/details/53053381#%E7%AC%AC%E4%BA%8C%E6%AD%A5stage-1-rpn-init-from-imagenet-model)- [在config参数的基础上改动参数以适合当前任务主要有](https://blog.csdn.net/u011956147/article/details/53053381#1%E5%9C%A8config%E5%8F%82%E6%95%B0%E7%9A%84%E5%9F%BA%E7%A1%80%E4%B8%8A%E6%94%B9%E5%8A%A8%E5%8F%82%E6%95%B0%E4%BB%A5%E9%80%82%E5%90%88%E5%BD%93%E5%89%8D%E4%BB%BB%E5%8A%A1%E4%B8%BB%E8%A6%81%E6%9C%89)
- [初始化化caffe](https://blog.csdn.net/u011956147/article/details/53053381#2-%E5%88%9D%E5%A7%8B%E5%8C%96%E5%8C%96caffe)
- [准备roidb和imdb](https://blog.csdn.net/u011956147/article/details/53053381#3-%E5%87%86%E5%A4%87roidb%E5%92%8Cimdb)
- [设置输出路径output_dir  get_output_dirimdb函数在config中用来保存中间生成的caffemodule等](https://blog.csdn.net/u011956147/article/details/53053381#4-%E8%AE%BE%E7%BD%AE%E8%BE%93%E5%87%BA%E8%B7%AF%E5%BE%84outputdir-getoutputdirimdb%E5%87%BD%E6%95%B0%E5%9C%A8config%E4%B8%AD%E7%94%A8%E6%9D%A5%E4%BF%9D%E5%AD%98%E4%B8%AD%E9%97%B4%E7%94%9F%E6%88%90%E7%9A%84caffemodule%E7%AD%89)
- [正式开始训练](https://blog.csdn.net/u011956147/article/details/53053381#5%E6%AD%A3%E5%BC%8F%E5%BC%80%E5%A7%8B%E8%AE%AD%E7%BB%83)
- [保存最后得到的权重参数](https://blog.csdn.net/u011956147/article/details/53053381#6%E4%BF%9D%E5%AD%98%E6%9C%80%E5%90%8E%E5%BE%97%E5%88%B0%E7%9A%84%E6%9D%83%E9%87%8D%E5%8F%82%E6%95%B0)

- [第三步Stage 1 RPN generate proposals](https://blog.csdn.net/u011956147/article/details/53053381#%E7%AC%AC%E4%B8%89%E6%AD%A5stage-1-rpn-generate-proposals)- [关注rpn_generate函数](https://blog.csdn.net/u011956147/article/details/53053381#1%E5%85%B3%E6%B3%A8rpngenerate%E5%87%BD%E6%95%B0)
- [保存得到的proposal文件](https://blog.csdn.net/u011956147/article/details/53053381#2%E4%BF%9D%E5%AD%98%E5%BE%97%E5%88%B0%E7%9A%84proposal%E6%96%87%E4%BB%B6)

- [第四步Stage 1 Fast R-CNN using RPN proposals init from ImageNet model](https://blog.csdn.net/u011956147/article/details/53053381#%E7%AC%AC%E5%9B%9B%E6%AD%A5stage-1-fast-r-cnn-using-rpn-proposals-init-from-imagenet-model)
- [第五步Stage 2 RPN init from stage 1 Fast R-CNN model](https://blog.csdn.net/u011956147/article/details/53053381#%E7%AC%AC%E4%BA%94%E6%AD%A5stage-2-rpn-init-from-stage-1-fast-r-cnn-model)
- [第六步Stage 2 RPN generate proposals](https://blog.csdn.net/u011956147/article/details/53053381#%E7%AC%AC%E5%85%AD%E6%AD%A5stage-2-rpn-generate-proposals)
- [第七步Stage 2 Fast R-CNN init from stage 2 RPN R-CNN model](https://blog.csdn.net/u011956147/article/details/53053381#%E7%AC%AC%E4%B8%83%E6%AD%A5stage-2-fast-r-cnn-init-from-stage-2-rpn-r-cnn-model)
- [第八步输出最后模型](https://blog.csdn.net/u011956147/article/details/53053381#%E7%AC%AC%E5%85%AB%E6%AD%A5%E8%BE%93%E5%87%BA%E6%9C%80%E5%90%8E%E6%A8%A1%E5%9E%8B)
- [AnchorTargetLayer和ProposalLayer](https://blog.csdn.net/u011956147/article/details/53053381#anchortargetlayer%E5%92%8Cproposallayer)
- [代码文件夹说明](https://blog.csdn.net/u011956147/article/details/53053381#%E4%BB%A3%E7%A0%81%E6%96%87%E4%BB%B6%E5%A4%B9%E8%AF%B4%E6%98%8E)- [tools](https://blog.csdn.net/u011956147/article/details/53053381#tools)
- [RPN](https://blog.csdn.net/u011956147/article/details/53053381#rpn)
- [nms](https://blog.csdn.net/u011956147/article/details/53053381#nms)

- [参考](https://blog.csdn.net/u011956147/article/details/53053381#%E5%8F%82%E8%80%83)
- [原文地址](https://blog.csdn.net/u011956147/article/details/53053381#%E5%8E%9F%E6%96%87%E5%9C%B0%E5%9D%80)





### 第一步，准备

从train_faster_rcnn_alt_opt.py入：
- 初始化参数：args = parse_args() 采用的是Python的argparse  
主要有–net_name，–gpu，–cfg等（在cfg中只是修改了几个参数，其他大部分参数在congig.py中，涉及到训练整个网络）。
- cfg_from_file(args.cfg_file) 这里便是代用config中的函数cfg_from_file来读取前面cfg文件中的参数，同时调用_merge_a_into_b函数把所有的参数整合，其中__C = edict() cfg = __C cfg是一个词典（edict）数据结构。
- faster rcnn采用的是多进程，mp_queue是进程间用于通讯的数据结构

```python
import multiprocessing as mp
mp_queue = mp.Queue()
```

同时solvers, max_iters, rpn_test_prototxt = get_solvers(args.net_name)得到solver参数 
接下来便进入了训练的各个阶段。

### 第二步，Stage 1 RPN, init from ImageNet model

```python
cfg.TRAIN.SNAPSHOT_INFIX = 'stage1'
mp_kwargs = dict(
        queue=mp_queue,
        imdb_name=args.imdb_name,
        init_model=args.pretrained_model,
        solver=solvers[0],
        max_iters=max_iters[0],
        cfg=cfg)
p = mp.Process(target=train_rpn, kwargs=mp_kwargs)
p.start()
rpn_stage1_out = mp_queue.get()
p.join()
```

可以看到第一个步骤是用ImageNet的模型M0来Finetuning RPN网络得到模型M1。以训练为例，这里的args参数都在脚本 experiments/scrips/faster_rcnn_alt_opt.sh中找到。主要关注train_rpn函数。 
    对于train_rpn函数，主要分一下几步：

#### 1.在config参数的基础上改动参数，以适合当前任务，主要有

```python
cfg.TRAIN.HAS_RPN = True
cfg.TRAIN.BBOX_REG = False  # applies only to Fast R-CNN bbox regression
cfg.TRAIN.PROPOSAL_METHOD = 'gt'
```

这里，关注proposal method 使用的是gt，后面会使用到gt_roidb函数，重要。

#### 2.  初始化化caffe

#### 3.  准备roidb和imdb

主要涉及到的函数get_roidb 
在get_roidb函数中调用factory中的get_imdb根据__sets[name]中的key（一个lambda表达式）转到pascol_voc类。class pascal_voc（imdb）在初始化自己的时候，先调用父类的初始化方法，例如：

```python
{
    year:’2007’
    image _set:’trainval’
    devkit _path:’data/VOCdevkit2007’
    data _path:’data /VOCdevkit2007/VOC2007’
    classes:(…)_如果想要训练自己的数据，需要修改这里_
    class _to _ind:{…} _一个将类名转换成下标的字典 _  建立索引0,1,2....
    image _ext:’.jpg’
    image _index: [‘000001’,’000003’,……]_根据trainval.txt获取到的image索引_
    roidb _handler: <Method gt_roidb >
    salt:  <Object uuid >
    comp _id:’comp4’
    config:{…}
}
```

注意，在这里，并没有读入任何数据，只是建立了图片的索引。

```python
imdb.set_proposal_method(cfg.TRAIN.PROPOSAL_METHOD)
```

设置proposal方法，接上面，设置为gt，这里只是设置了生成的方法，第一次调用发生在下一句，roidb = get_training_roidb(imdb) –> append_flipped_images()时的这行代码：“boxes = self.roidb[i][‘boxes’].copy()”，其中get_training_roidb位于train.py，主要实现图片的水平翻转，并添加回去。实际是该函数调用了imdb. append_flipped_images也就是在这个函数，调用了pascal_voc中的gt_roidb，转而调用了同一个文件中的_load_pascal_annotation，该函数根据图片的索引，到Annotations这个文件夹下去找相应的xml标注数据，然后加载所有的bounding box对象，xml的解析到此结束，接下来是roidb中的几个类成员的赋值：
- boxes  一个二维数组，每一行存储 xmin ymin xmax ymax
- gt _classes存储了每个box所对应的类索引(类数组在初始化函数中声明)
- gt _overlap是一个二维数组，共有num _classes(即类的个数)行，每一行对应的box的类索引处值为1，其余皆为0，后来被转成了稀疏矩阵
- seg _areas存储着某个box的面积
- flipped 为false 代表该图片还未被翻转(后来在train.py里会将翻转的图片加进去，用该变量用于区分

最后将这些成员变量组装成roidb返回。 
在get_training_roidb函数中还调用了roidb中的prepare_roidb函数，这个函数就是用来准备imdb 的roidb，给roidb中的字典添加一些属性，比如image(图像的索引)，width，height，通过前面的gt _overla属性，得到max_classes和max_overlaps. 
至此，

```python
return roidb，imdb
```

#### 4. 设置输出路径，output_dir = get_output_dir(imdb)，函数在config中，用来保存中间生成的caffemodule等

#### 5.正式开始训练

```python
model_paths = train_net(solver, roidb, output_dir,
                        pretrained_model=init_model,
                        max_iters=max_iters)
```

调用train中的train_net函数，其中，首先filter_roidb，判断roidb中的每个entry是否合理，合理定义为至少有一个前景box或背景box，roidb全是groudtruth时，因为box与对应的类的重合度(overlaps)显然为1，也就是说roidb起码要有一个标记类。如果roidb包含了一些proposal，overlaps在[BG_THRESH_LO, BG_THRESH_HI]之间的都将被认为是背景，大于FG_THRESH才被认为是前景，roidb 至少要有一个前景或背景，否则将被过滤掉。将没用的roidb过滤掉以后，返回的就是filtered_roidb。在train文件中，需要关注的是SolverWrapper类。详细见train.py，在这个类里面，引入了caffe SGDSlover，最后一句self.solver.net.layers[0].set_roidb(roidb)将roidb设置进layer(0)(在这里就是ROILayer)调用ayer.py中的set_roidb方法，为layer(0)设置roidb，同时打乱顺序。最后train_model。在这里，就需要去实例化每个层，在这个阶段，首先就会实现ROIlayer，详细参考layer中的setup，在训练时roilayer的forward函数，在第一个层，只需要进行数据拷贝，在不同的阶段根据prototxt文件定义的网络结构拷贝数据，blobs = self._get_next_minibatch()这个函数读取图片数据（调用get_minibatch函数，这个函数在minibatch中，主要作用是为faster rcnn做实际的数据准备，在读取数据的时候，分出了boxes，gt_boxes,im_info（宽高缩放）等）。 
第一个层，对于stage1_rpn_train.pt文件中，该layer只有3个top blob：’data’、’im_info’、’gt_boxes’。 
对于stage1_fast_rcnn_train.pt文件中，该layer有6个top blob：top: ‘data’、’rois’、’labels’、’bbox_targets’、’bbox_inside_weights’、’bbox_outside_weights’，这些数据准备都在minibatch中。至此后数据便在caffe中流动了，直到训练结束。 
[画出网络的结构](http://ethereon.github.io/netscope/#/editor) 这里只截取了一部分： 
![这里写图片描述](https://img-blog.csdn.net/20161106101807108)
值得注意的是在rpn-data层使用的是AnchorTargetLayer，该层使用python实现的，往后再介绍。

#### 6.保存最后得到的权重参数

```python
rpn_stage1_out = mp_queue.get()
```
- 1

至此，第一阶段完成，在后面的任务开始时，如果有需要，会在这个输出的地址找这一阶段得到的权重文件。

### 第三步，Stage 1 RPN, generate proposals

这一步就是调用上一步训练得到的模型M1来生成proposal P1，在这一步只产生proposal，参数：

```python
mp_kwargs = dict(
        queue=mp_queue,
        imdb_name=args.imdb_name,
        rpn_model_path=str(rpn_stage1_out['model_path']),
        cfg=cfg,
        rpn_test_prototxt=rpn_test_prototxt)
p = mp.Process(target=rpn_generate, kwargs=mp_kwargs)
p.start()
rpn_stage1_out['proposal_path'] = mp_queue.get()['proposal_path']
p.join()
```

#### 1.关注rpn_generate函数

前面和上面讲到的train_rpn基本相同，从rpn_proposals = imdb_proposals(rpn_net, imdb)开始，imdb_proposals函数在rpn.generate.py文件中,rpn_proposals是一个列表的列表，每个子列表。对于imdb_proposals，使用im = cv2.imread(imdb.image_path_at(i))读入图片数据，调用 im_proposals生成单张图片的rpn proposals，以及得分。这里，im_proposals函数会调用网络的forward，从而得到想要的boxes和scores，这里需要好好理解blobs_out = net.forward(data,im_info)中net forward和layer forward间的调用关系。 
![这里写图片描述](https://img-blog.csdn.net/20161106102144768)
在这里，也会有proposal，同样会使用python实现的ProposalLayer，这个函数也在rpn文件夹内，后面再补充。

```python
boxes = blobs_out['rois'][:, 1:].copy() / scale     
scores = blobs_out['scores'].copy()
return boxes, scores
```
- 


至此，得到imdb proposal

#### 2.保存得到的proposal文件

```python
queue.put({'proposal_path': rpn_proposals_path})
rpn_stage1_out['proposal_path'] = mp_queue.get()['proposal_path']
```
- 


至此，Stage 1 RPN, generate proposals结束

### 第四步，Stage 1 Fast R-CNN using RPN proposals, init from ImageNet model

参数：

```python
cfg.TRAIN.SNAPSHOT_INFIX = 'stage1'
mp_kwargs = dict(
        queue=mp_queue,
        imdb_name=args.imdb_name,
        init_model=args.pretrained_model,
        solver=solvers[1],
        max_iters=max_iters[1],
        cfg=cfg,
        rpn_file=rpn_stage1_out['proposal_path'])
p = mp.Process(target=train_fast_rcnn, kwargs=mp_kwargs)
p.start()
fast_rcnn_stage1_out = mp_queue.get()
p.join()
```

这一步，用上一步生成的proposal，以及imagenet模型M0来训练fast-rcnn模型M2。 
关注train_fast_rcnn 
同样地，会设置参数，这里注意cfg.TRAIN.PROPOSAL_METHOD = ‘rpn’ 不同于前面，后面调用的将是rpn_roidb。cfg.TRAIN.IMS_PER_BATCH = 2，每个mini-batch包含两张图片，以及它们proposal的roi区域。且在这一步是有rpn_file的（后面和rpn_roidb函数使用有关）。其他的和前面差不多。提一下，这里在train_net的时候，会调用add_bbox_regression_targets位于roidb中，主要是添加bbox回归目标，即添加roidb的‘bbox_targets’属性，同时根据cfg中的参数设定，求取bbox_targets的mean和std，因为需要训练class-specific regressors在这里就会涉及到bbox_overlaps函数，放在util.bbox中。 
要注意的是在这一步get_roidb时，如前所说，使用的是rpn_roidb，会调用imdb. create_roidb_from_box_list该方法功能是从box_list中读取每张图的boxes，而这个box_list就是从上一步保存的proposal文件中读取出来的，然后做一定的处理，详细见代码，重点是在最后会返回roidb，rpn_roidb中的gt_overlaps是rpn_file中的box与gt_roidb中box的gt_overlaps等计算IoU等处理后得到的，而不像gt_roidb()方法生成的gt_roidb中的gt_overlaps全部为1.0。同时使用了imdb.merge_roidb，类imdb的静态方法【这里不太懂，需要再学习下】，把rpn_roidb和gt_roidb归并为一个roidb，在这里，需要具体去了解合并的基本原理。 
![这里写图片描述](https://img-blog.csdn.net/20161106102533779)

### 第五步，Stage 2 RPN, init from stage 1 Fast R-CNN model

参数：

```python
cfg.TRAIN.SNAPSHOT_INFIX = 'stage2'
mp_kwargs = dict(
        queue=mp_queue,
        imdb_name=args.imdb_name,
        init_model=str(fast_rcnn_stage1_out['model_path']),
        solver=solvers[2],
        max_iters=max_iters[2],
        cfg=cfg)
p = mp.Process(target=train_rpn, kwargs=mp_kwargs)
rpn_stage2_out = mp_queue.get()
```

这部分就是利用模型M2练rpn网络，这一次与stage1的rpn网络不通，这一次conv层的参数都是不动的，只做前向计算，训练得到模型M3，这属于微调了rpn网络。 
![这里写图片描述](https://img-blog.csdn.net/20161106102651796)

### 第六步，Stage 2 RPN, generate proposals

参数：

```python
mp_kwargs = dict(
        queue=mp_queue,
        imdb_name=args.imdb_name,
        rpn_model_path=str(rpn_stage2_out['model_path']),
        cfg=cfg,
        rpn_test_prototxt=rpn_test_prototxt)
p = mp.Process(target=rpn_generate, kwargs=mp_kwargs)
p.start()
rpn_stage2_out['proposal_path'] = mp_queue.get()['proposal_path']
p.join()
```

这一步，基于上一步得到的M3模型，产生proposal P2，网络结构和前面产生proposal P1的一样。 
![这里写图片描述](https://img-blog.csdn.net/20161106102803496)

### 第七步，Stage 2 Fast R-CNN, init from stage 2 RPN R-CNN model

参数：

```python
cfg.TRAIN.SNAPSHOT_INFIX = 'stage2'
mp_kwargs = dict(
        queue=mp_queue,
        imdb_name=args.imdb_name,
        init_model=str(rpn_stage2_out['model_path']),
        solver=solvers[3],
        max_iters=max_iters[3],
        cfg=cfg,
        rpn_file=rpn_stage2_out['proposal_path'])
p = mp.Process(target=train_fast_rcnn, kwargs=mp_kwargs)
p.start()
fast_rcnn_stage2_out = mp_queue.get()
p.join()
```
- 


这一步基于模型M3和P2训练fast rcnn得到最终模型M4，这一步，conv层和rpn都是参数固定，只是训练了rcnn层（也就是全连接层），与stage1不同，stage1只是固定了rpn层，其他层还是有训练。模型结构与stage1相同： 
![这里写图片描述](https://img-blog.csdn.net/20161106102917781)

### 第八步，输出最后模型

```python
final_path = os.path.join(
        os.path.dirname(fast_rcnn_stage2_out['model_path']),
        args.net_name + '_faster_rcnn_final.caffemodel')
print 'cp {} -> {}'.format(
        fast_rcnn_stage2_out['model_path'], final_path)
shutil.copy(fast_rcnn_stage2_out['model_path'], final_path)
print 'Final model: {}'.format(final_path)
```

只是对上一步模型输出的一个拷贝。 
至此，整个faster-rcnn的训练过程就结束了。

### AnchorTargetLayer和ProposalLayer

前面说过还有这两个层没有说明，一个是anchortarget layer一个是proposal layer，下面逐一简要分析。

```python
class AnchorTargetLayer(caffe.Layer)
```
- 


首先是读取参数，在prototxt，实际上只读取了param_str: “‘feat_stride’: 16”，这是个很重要的参数，目前我的理解是滑块滑动的大小，对于识别物体的大小很有用，比如小物体的识别，需要把这个参数减小等。 
首先 setup部分，

```python
anchor_scales = layer_params.get('scales', (8, 16, 32))
self._anchors = generate_anchors(scales=np.array(anchor_scales))
```

调用generate_anchors方法生成最初始的9个anchor该函数位于generate_anchors.py 主要功能是生成多尺度，多宽高比的anchors，8,16,32其实就是scales:[2^3 2^4 2^5]，base_size为16，具体是怎么实现的可以查阅源代码。_ratio_enum()部分生成三种宽高比 1:2,1:1,2:1的anchor如下图所示：（以下参考 [另外一篇博客](http://blog.csdn.net/xzzppp/article/details/52317863)） 
![这里写图片描述](https://img-blog.csdn.net/20161106103631396)
_scale_enum()部分，生成三种尺寸的anchor，以_ratio_enum()部分生成的anchor[0 0 15 15]为例，扩展了三种尺度 128*128,256*256,512*512，如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20161106103707690)
另外一个函数就是forward()。 
在faster rcnn中会根据不同图的输入，得到不同的feature map，height, width = bottom[0].data.shape[-2:]首先得到conv5的高宽，以及gt box gt_boxes = bottom[1].data，图片信息im_info = bottom[2].data[0, :]，然后计算偏移量，shift_x = np.arange(0, width) * self._feat_stride，在这里，你会发现，例如你得到的fm是H=61，W=36，然后你乘以16，得到的图形大概就是1000*600，其实这个16大概就是网络的缩放比例。接下来就是生成anchor，以及对anchor做一定的筛选，详见代码。

另外一个需要理解的就是proposal layer，这个只是在测试的时候用，许多东西和AnchorTargetLayer类似，不详细介绍，可以查看代码。主要看看forward函数，函数算法介绍在注释部分写的很详细：

```python
# Algorithm:
# for each (H, W) location i
#   generate A anchor boxes centered on cell i
#   apply predicted bbox deltas at cell i to each of the A anchors
# clip predicted boxes to image
# remove predicted boxes with either height or width < threshold
# sort all (proposal, score) pairs by score from highest to lowest
# take top pre_nms_topN proposals before NMS
# apply NMS with threshold 0.7 to remaining proposals
# take after_nms_topN proposals after NMS
# return the top proposals (-> RoIs top, scores top)
```

在这个函数中会引用NMS方法。

### 代码文件夹说明

转载出处：[另外一篇博客](http://blog.csdn.net/bailufeiyan/article/details/50749694)

#### tools

在tools文件夹中，是我们直接调用的最外层的封装文件。其中主要包含的文件为：
- **_init_paths.py** :用来初始化路径的，也就是之后的路径会join（path，*）
- **compress_net.py**：用来压缩参数的，使用了SVD来进行压缩，这里可以发现，作者对于fc6层和fc7层进行了压缩，也就是两个全连接层。
- **demo.py** ：通常，我们会直接调用这个函数，如果要测试自己的模型和数据，这里需要修改。这里调用了fast_rcnn中的test、config、nums_wrapper函数。vis_detections用来做检测，parse_args用来进行参数设置，以及damo和主函数。
- **eval_recall.py**：评估函数
- **reval.py**：re-evaluate，这里调用了fast_rcnn以及dataset中的函数。其中，from_mats函数和from_dets函数分别loadmat文件和pkl文件。
- **rpn_genetate.py**：这个函数调用了rpn中的genetate函数，之后我们会对rpn层做具体的介绍。这里，主要是一个封装调用的过程，我们在这里调用配置的参数、设置rpn的test参数，以及输入输出等操作。
- **test_net.py**：测试fast rcnn网络。主要就是一些参数配置。
- **train_faster_rcnn_alt_opt.py**：训练faster rcnn网络使用交替的训练，这里就是根据faster rcnn文章中的具体实现。可以在主函数中看到，其包括的步骤为： 
- RPN 1，使用imagenet model进行初始化参数，生成proposal，这里存储在mp_kwargs
- fast rcnn 1，使用 imagenet model 进行初始化参数，使用刚刚生成的proposal进行fast rcnn的训练
- RPN 2使用 fast rcnn 中的参数进行初始化（这里要注意哦），并生成proposal
- fast rcnn 2，使用RPN 2 中的 model进行初始化参数
- 值得注意的是：在我们训练时，我们可以在get_solvers中的max_iters中设置迭代次数，在不确定网络是否可以调通时，减少迭代次数可以减少测试时间。
- 我们在训练faster rcnn网络时，就是调用这个文件训练的

- **train_net.py**：使用fast rcnn，训练自己数据集的网络模型
- **train_svms.py**：使用最原始的RCNN网络训练post-hoc SVMs

#### RPN

这里我们主要看lib/rpn文件夹下的代码。这里主要介绍了rpn的模型，其中，包含的主要文件如下：
- **generate_anchors.py**: 生成多尺度和多比例的锚点。这里由generate_anthors函数主要完成，可以看到，使用了 3 个尺度( 128, 256, and 512)以及 3 个比例(1:1,1:2,2:1)。一个锚点由w, h, x_ctr, y_ctr固定，也就是宽、高、x center和y center固定。
- **proposal_layer.py**:这个函数是用来将RPN的输出转变为object proposals的。作者新增了ProposalLayer类，这个类中，重新了set_up和forward函数，其中forward实现了：生成锚点box、对于每个锚点提供box的参数细节、将预测框切成图像、删除宽、高小于阈值的框、将所有的(proposal, score) 对排序、获取 pre_nms_topN proposals、获取NMS 、获取 after_nms_topN proposals。（注：NMS，nonmaximum suppression，非极大值抑制）
- **anchor_target_layer.py**:生成每个锚点的训练目标和标签，将其分类为1 (object), 0 (not object) ， -1 (ignore).当label>0，也就是有object时，将会进行box的回归。其中，forward函数功能：在每一个cell中，生成9个锚点，提供这9个锚点的细节信息，过滤掉超过图像的锚点，测量同GT的overlap。
- **proposal_target_layer.py**:对于每一个object proposal 生成训练的目标和标签，分类标签从0-k，对于标签>0的box进行回归。（注意，同anchor_target_layer.py不同，两者一个是生成anchor，一个是生成proposal）
- **generate.py**:使用一个rpn生成object proposals。

作者就是通过以上这些文件生成rpn的。

#### nms

lib/nms文件夹下是非极大值抑制，这部分大家应该已经非常熟悉了，其Python版本的核心函数为py_cpu_nms.py，具体实现以及注释如下：

```python
def py_cpu_nms(dets, thresh):
    """Pure Python NMS baseline."""
    #x1、y1、x2、y2、以及score赋值
    x1 = dets[:, 0]
    y1 = dets[:, 1]
    x2 = dets[:, 2]
    y2 = dets[:, 3]
    scores = dets[:, 4]

    #每一个op的面积
    areas = (x2 - x1 + 1) * (y2 - y1 + 1)
    #order是按照score排序的
    order = scores.argsort()[::-1]

    keep = []
    while order.size > 0:
        i = order[0]
        keep.append(i)
        xx1 = np.maximum(x1[i], x1[order[1:]])
        yy1 = np.maximum(y1[i], y1[order[1:]])
        xx2 = np.minimum(x2[i], x2[order[1:]])
        yy2 = np.minimum(y2[i], y2[order[1:]])

        #计算相交的面积
        w = np.maximum(0.0, xx2 - xx1 + 1)
        h = np.maximum(0.0, yy2 - yy1 + 1)
        inter = w * h
        #计算：重叠面积/（面积1+面积2-重叠面积）
        ovr = inter / (areas[i] + areas[order[1:]] - inter)

        inds = np.where(ovr <= thresh)[0]
        order = order[inds + 1]
```

### 参考

在这里，没有贴出代码的注释，只是梳理了下Faster RCNN训练的流程，因为代码的注释网络上已经有很多，需要看代码的注释可以参考下面几个博客，我看代码的时候也有参考： 
[1] [http://www.cnblogs.com/CarryPotMan/p/5390336.html](http://www.cnblogs.com/CarryPotMan/p/5390336.html)
[2] [http://blog.csdn.net/u010668907/article/category/6237110](http://blog.csdn.net/u010668907/article/category/6237110)
[3] [http://blog.csdn.net/sunyiyou9/article/category/6269359](http://blog.csdn.net/sunyiyou9/article/category/6269359)
[4] [http://blog.csdn.net/bailufeiyan/article/details/50749694](http://blog.csdn.net/bailufeiyan/article/details/50749694)

### 原文地址：

[http://blog.csdn.net/u011956147/article/details/53053381](http://blog.csdn.net/u011956147/article/details/53053381)




