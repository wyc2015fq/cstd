# 继承nn.Module后的 init与forward函数【trian_val、vgg16、faster_rcnn、rpn】.py 学习 文件结构 大工程安排 - Snoopy_Dream - CSDN博客





2018年11月13日 12:01:55[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：237








本篇文章主要是用来学习大工程的构造，具体包括如何进行init和forward，如何层层递进，高层设置输入，传入底层的input中。

从train_val.py中的初始化vgg开始，这里调用了vgg的初始化，嵌套faster_rcnn初始化，再内嵌套调用rpn初始化，调anchor_target_layer/prosal_layer的初始化。

其中faster_rcnn的forward函数中，利用self调用了顶层vgg的函数，如base_feat = self.RCNN_base(im_data)，这里返回了基础rcnn之后的共享卷进层。而RCNN_base的定义则是在vgg的_init_modules(self)当中。注意到在train_val.py中应当先是执行了vgg的_init_modules(self)，再进行fasterRCNN的传入数据，因为fasterRCNN的forward需要vgg当中的init_modules.

### **init调用层次**

**从trian_val.py开始：**

*fasterRCNN = vgg16(imdb.classes, pretrained=True, class_agnostic=args.class_agnostic)#调用的vgg的init*

**再看*vgg的init，vgg里的self是第一层的self***

*class vgg16(_fasterRCNN):#因为_fasterRCNN不是一个基类，vgg16涉及三代，不能super，需要单独* init* vgg16*的父类
*  def __init__(self, classes, pretrained=False, class_agnostic=False):    #没有super    _fasterRCNN.__init__(self, classes, class_agnostic)#*单独init它的父类 *调用了_fasterRCNN的init*

***再看**_fasterRCNN的init，_fasterRCNN里的self是第二层的self***

*class _fasterRCNN(nn.Module):#nn.Module是基类，直接super初始化_fasterRCNN父类Module    def __init__(self, classes, class_agnostic):        super(_fasterRCNN, self).__init__()#*先执行它的父类的init模块的意思 *初始化了nn.Module**        self.RCNN_rpn = _RPN(self.dout_base_model) #初始化_RPN dout_base_model 512 vgg.py        self.RCNN_proposal_target = _ProposalTargetLayer(self.n_classes) #初始化_ProposalTargetLayer*

然后**一直初始化下去。**

还有一个细节值得注意，就是*在****_fasterRCNN.py中**** 有self.RCNN_cls_score但是RCNN_cls_score在该python中完全没有定义（在顶层中定义即vgg中），而且_init_modules也是一样都在顶层中定义，总结：**顶层的function(self)*****，可以直接在底层中self.function() **：比方说是在faster_rcnn.py中

```python
#权重初始化
    def _init_weights(self):

         ......

        normal_init(self.RCNN_rpn.RPN_Conv, 0, 0.01, cfg.TRAIN.TRUNCATED)
        normal_init(self.RCNN_cls_score, 0, 0.01, cfg.TRAIN.TRUNCATED)#因为该函数是在trian_val中直接调用，RCNN_cls_score是在vgg中定义，是第一层self，所以直接self.RCNN_cls_score。

    def create_architecture(self):
        self._init_modules()#顶层的function(self)，可以直接在底层中self.function()
        self._init_weights()#权重初始化
```

*self._init_modules() #模型初始化 _init_modules()的函数不在本py中，而在vgg当中 。这是因为对于不同的模型，比如换成resnet它的模型的初始化就不一样了，所以模型初始化放到了，vgg，resnet这些python当中。而从上一个例子上可以看出来，***trian_val***的init就是直接用的vgg的，所以vgg里的self就是第一层self，faster_rcnn是第二层，rpn是第三层的。而***create_architecture**的*调用，是在***trian_val.py *****fasterRCNN** = vgg16（...）*之后，紧接着**fasterRCNN.create_architecture()，**

同理： cls_score = self.RCNN_cls_score(pooled_feat)也是一样的，RCNN_cls_score在vgg的init当中。

像relu这种不用在init里写，放到forward。forward主要用于计算，forward调用forward的函数，init调用另一模块的init模块。



再看train_val.py文件

### **数据读取：**

*  sampler_batch = sampler(train_size, args.batch_size)*

*  dataset = roibatchLoader(roidb, ratio_list, ratio_index, args.batch_size, \                           imdb.num_classes, training=True)*

*  dataloader = torch.utils.data.DataLoader(dataset, batch_size=args.batch_size,                            sampler=sampler_batch, num_workers=args.num_workers)*

### 初始化数据：

*  # initilize the tensor holder here.  im_data = torch.FloatTensor(1)  im_info = torch.FloatTensor(1)  num_boxes = torch.LongTensor(1)  gt_boxes = torch.FloatTensor(1)*


*  # ship to cuda  if args.cuda:    im_data = im_data.cuda()    im_info = im_info.cuda()    num_boxes = num_boxes.cuda()    gt_boxes = gt_boxes.cuda()*

*  # make variable  im_data = Variable(im_data)  im_info = Variable(im_info)  num_boxes = Variable(num_boxes)  gt_boxes = Variable(gt_boxes)*

*  if args.cuda:    cfg.CUDA = True*

### 初始化模型：

*fasterRCNN =** vgg16**(imdb.classes, pretrained=True, class_agnostic=args.class_agnostic)*

fasterRCNN.create_architecture()#初始化权重和模型

### 加载模型参数：

*params = []  for key, value in dict(fasterRCNN.named_parameters()).items():    if value.requires_grad:      if 'bias' in key:        params += [{'params':[value],'lr':lr*(cfg.TRAIN.DOUBLE_BIAS + 1), \                'weight_decay': cfg.TRAIN.BIAS_DECAY and cfg.TRAIN.WEIGHT_DECAY or 0}]      else:        params += [{'params':[value],'lr':lr, 'weight_decay': cfg.TRAIN.WEIGHT_DECAY}]*

### *定义优化器：*

***optimizer = torch.optim.SGD(params, momentum=cfg.TRAIN.MOMENTUM)***

### *数据迭代：*

*  for epoch in range(args.start_epoch, args.max_epochs + 1):    # setting to train mode    fasterRCNN.train()    loss_temp = 0    start = time.time()*

***   data_iter = iter(dataloader)**    for step in range(iters_per_epoch):** data = next(data_iter)**      im_data.data.resize_(data[0].size()).copy_(data[0])      im_info.data.resize_(data[1].size()).copy_(data[1])      gt_boxes.data.resize_(data[2].size()).copy_(data[2])      num_boxes.data.resize_(data[3].size()).copy_(data[3])*

*      fasterRCNN.zero_grad()      rois, cls_prob, bbox_pred, \      rpn_loss_cls, rpn_loss_box, \      RCNN_loss_cls, RCNN_loss_bbox, \      rois_label = **fasterRCNN**(im_data, im_info, gt_boxes, num_boxes)*

*      loss = rpn_loss_cls.mean() + rpn_loss_box.mean() \           + RCNN_loss_cls.mean() + RCNN_loss_bbox.mean()      loss_temp += loss.item()*

***     # backward**      optimizer.zero_grad()      loss.backward()      optimizer.step()*

*      if step % args.disp_interval == 0:        end = time.time()        if step > 0:          loss_temp /= args.disp_interval        loss_rpn_cls = rpn_loss_cls.item()        loss_rpn_box = rpn_loss_box.item()        loss_rcnn_cls = RCNN_loss_cls.item()        loss_rcnn_box = RCNN_loss_bbox.item()        fg_cnt = torch.sum(rois_label.data.ne(0))        bg_cnt = rois_label.data.numel() - fg_cnt*





