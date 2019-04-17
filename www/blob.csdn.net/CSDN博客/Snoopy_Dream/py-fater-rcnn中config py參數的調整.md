# py-fater-rcnn中config.py參數的調整 - Snoopy_Dream - CSDN博客





2018年05月04日 10:03:44[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：266标签：[config																[faster-rcnn](https://so.csdn.net/so/search/s.do?q=faster-rcnn&t=blog)](https://so.csdn.net/so/search/s.do?q=config&t=blog)
个人分类：[DOTA																[faster-rcnn](https://blog.csdn.net/e01528/article/category/7599535)](https://blog.csdn.net/e01528/article/category/7620828)








```python
faster中的兩次的負樣本都參與位置回歸，不參與分類

# 最短边Scale成600
__C.TRAIN.SCALES = (600,)
 
# 最长边最大为1000
__C.TRAIN.MAX_SIZE = 1000
 
# 一个minibatch包含两张图片,但是调用是yaml里改成1，而且只能为1
__C.TRAIN.IMS_PER_BATCH = 2
 -------------------------------------------------------------------------
# 这一部分是PropsalTargetCreator.py中的参数


# 1. 从rpn经过NMS抑制後得到的2000个roi中選出128個sampel_rois，进行标准化处理，最後的訓練
# 2. RoIs 和 gt_bboxes 的 IoU 大于 0.5 的，选择一些（128*0.25=32）
# **注意這裏如果不夠32個，比如滿足IOU大於0.5的只有20個則剩餘108都是負樣本是108個**
# **但是還需要主要的是這裏的位置回歸只对roi與gt的IOU>0.5[可設置，這裏感覺和上面的選擇條件有些重復，不明所以]正样本计算损失. 而且是只对正样本中的这个类别 4 个参数计算损失，負樣本不進行loss計算***
# 3. 选择 RoIs 和 gt_bboxes 的 IoU 小于等于 0（或者 0.1）的选择一些（比如 128-正樣本 个）作为负样本


#  Minibatch大小，即ROI的数量，如果你的目標比較多的話，可以適當設置的大一些
__C.TRAIN.BATCH_SIZE = 128
 
# minibatch中前景样本所占的比例，負樣本只在分類時參與
__C.TRAIN.FG_FRACTION = 0.25
 
# Overlap threshold for a ROI to be considered foreground (if >= FG_THRESH)
# 与前景的overlap大于等于0.5认为该ROI为前景样本，如果你的anchor經過修改，變得很多，
# 比如小汽車長都在0-128個像素，然後你的anchor的長寬從4 14 24 34 ...128 這裏的IOU可以比較大，這樣有利於快速收斂和精度的提高

__C.TRAIN.FG_THRESH = 0.5 
# 与前景的overlap在0.1-0.5认为该ROI为背景样本
__C.TRAIN.BG_THRESH_HI = 0.5
__C.TRAIN.BG_THRESH_LO = 0.1

# Use horizontally-flipped images during training?
# 水平翻转图像，增加数据量
__C.TRAIN.USE_FLIPPED = True
 
# Train bounding-box regressors
# 训练bb回归器
__C.TRAIN.BBOX_REG = True
 
# Overlap required between a ROI and ground-truth box in order for that ROI to
# be used as a bounding-box regression training example
# BBOX阈值，只有ROI与gt的重叠度大于阈值，这样的ROI才能用作bb回归的训练样本
# 如果你上面的.FG_THRESH做過調整，這裏最好做相應的調整 
__C.TRAIN.BBOX_THRESH = 0.5

# Iterations between snapshots
# 每迭代1000次产生一次snapshot
__C.TRAIN.SNAPSHOT_ITERS = 10000
 
# 为产生的snapshot文件名称添加一个可选的infix. solver.prototxt指定了snapshot名称的前缀
__C.TRAIN.SNAPSHOT_INFIX = ''
 
# 在roi_data_layer.layer使用预取线程，作者认为不太有效，因此设为False
__C.TRAIN.USE_PREFETCH = False
 
# Normalize the targets (subtract empirical mean, divide by empirical stddev)
# 归一化目标BBOX_NORMALIZE_TARGETS，减去经验均值，除以标准差
__C.TRAIN.BBOX_NORMALIZE_TARGETS = True
# Deprecated (inside weights)
# 弃用
__C.TRAIN.BBOX_INSIDE_WEIGHTS = (1.0, 1.0, 1.0, 1.0)
# Normalize the targets using "precomputed" (or made up) means and stdevs
# (BBOX_NORMALIZE_TARGETS must also be True)
# 在BBOX_NORMALIZE_TARGETS为True时，归一化targets,使用经验均值和方差
__C.TRAIN.BBOX_NORMALIZE_TARGETS_PRECOMPUTED = False
__C.TRAIN.BBOX_NORMALIZE_MEANS = (0.0, 0.0, 0.0, 0.0)
__C.TRAIN.BBOX_NORMALIZE_STDS = (0.1, 0.1, 0.2, 0.2)
 
 --------------------------------------------------------------------------
# Train using these proposals
# 使用'selective_search'的proposal训练！注意该文件来自fast rcnn，下文提到RPN
__C.TRAIN.PROPOSAL_METHOD = 'selective_search'
 
# Make minibatches from images that have similar aspect ratios (i.e. both
# tall and thin or both short and wide) in order to avoid wasting computation
# on zero-padding.
# minibatch的两个图片应该有相似的宽高比，以避免冗余的zero-padding计算
__C.TRAIN.ASPECT_GROUPING = True
# Use RPN to detect objects
# 使用RPN检测目标
__C.TRAIN.HAS_RPN = False
# IOU >= thresh: positive example
# RPN的正样本阈值
__C.TRAIN.RPN_POSITIVE_OVERLAP = 0.7
# IOU < thresh: negative example
# RPN的负样本阈值
__C.TRAIN.RPN_NEGATIVE_OVERLAP = 0.3
# If an anchor statisfied by positive and negative conditions set to negative
# 如果一个anchor同时满足正负样本条件，设为负样本（应该用不到）
__C.TRAIN.RPN_CLOBBER_POSITIVES = False
# Max number of foreground examples
# 前景样本的比例
__C.TRAIN.RPN_FG_FRACTION = 0.5
# Total number of examples
# batch size大小
__C.TRAIN.RPN_BATCHSIZE = 256
# NMS threshold used on RPN proposals
# 非极大值抑制的阈值
__C.TRAIN.RPN_NMS_THRESH = 0.7
# Number of top scoring boxes to keep before apply NMS to RPN proposals
# 在对RPN proposal使用NMS前，要保留的top scores的box数量
__C.TRAIN.RPN_PRE_NMS_TOP_N = 12000
# Number of top scoring boxes to keep after applying NMS to RPN proposals
# 在对RPN proposal使用NMS后，要保留的top scores的box数量
__C.TRAIN.RPN_POST_NMS_TOP_N = 2000
# Proposal height and width both need to be greater than RPN_MIN_SIZE (at orig image scale)
# proposal的高和宽都应该大于RPN_MIN_SIZE，否则，映射到conv5上不足一个像素点
__C.TRAIN.RPN_MIN_SIZE = 16
# Deprecated (outside weights)
# 弃用
__C.TRAIN.RPN_BBOX_INSIDE_WEIGHTS = (1.0, 1.0, 1.0, 1.0)
# Give the positive RPN examples weight of p * 1 / {num positives}
# 给定正RPN样本的权重
# and give negatives a weight of (1 - p)
# 给定负RPN样本的权重
# Set to -1.0 to use uniform example weighting
# 这里正负样本使用相同权重
__C.TRAIN.RPN_POSITIVE_WEIGHT = -1.0
 
#
# Testing options
# 测试选项
#
 
__C.TEST = edict()
 
# Scales to use during testing (can list multiple scales)
# Each scale is the pixel size of an image's shortest side
__C.TEST.SCALES = (600,)
 
# Max pixel size of the longest side of a scaled input image
__C.TEST.MAX_SIZE = 1000
 
# Overlap threshold used for non-maximum suppression (suppress boxes with
# IoU >= this threshold)
# 测试时非极大值抑制的阈值
__C.TEST.NMS = 0.3
 
# Experimental: treat the (K+1) units in the cls_score layer as linear
# predictors (trained, eg, with one-vs-rest SVMs).
# 分类不再用SVM，设置为False
__C.TEST.SVM = False
 
# Test using bounding-box regressors
# 使用bb回归
__C.TEST.BBOX_REG = True
 
# Propose boxes
# 不使用RPN生成proposal
__C.TEST.HAS_RPN = False
 
# Test using these proposals
# 使用selective_search生成proposal
__C.TEST.PROPOSAL_METHOD = 'selective_search'
 
## NMS threshold used on RPN proposals
#  RPN proposal的NMS阈值
__C.TEST.RPN_NMS_THRESH = 0.7
## Number of top scoring boxes to keep before apply NMS to RPN proposals
__C.TEST.RPN_PRE_NMS_TOP_N = 6000
## Number of top scoring boxes to keep after applying NMS to RPN proposals
__C.TEST.RPN_POST_NMS_TOP_N = 300
# Proposal height and width both need to be greater than RPN_MIN_SIZE (at orig image scale)
__C.TEST.RPN_MIN_SIZE = 16
 
#
# MISC
#
 
# The mapping from image coordinates to feature map coordinates might cause
# 从原图到feature map的坐标映射，可能会造成在原图上不同的box到了feature map坐标系上变得相同了
# some boxes that are distinct in image space to become identical in feature
# coordinates. If DEDUP_BOXES > 0, then DEDUP_BOXES is used as the scale factor
# for identifying duplicate boxes.
# 1/16 is correct for {Alex,Caffe}Net, VGG_CNN_M_1024, and VGG16
# 缩放因子
__C.DEDUP_BOXES = 1./16.
 
# Pixel mean values (BGR order) as a (1, 1, 3) array
# We use the same pixel mean for all networks even though it's not exactly what
# they were trained with
# 所有network所用的像素均值设为相同
__C.PIXEL_MEANS = np.array([[[102.9801, 115.9465, 122.7717]]])
 
# For reproducibility
__C.RNG_SEED = 3
 
# A small number that's used many times
# 极小的数
__C.EPS = 1e-14
 
# Root directory of project
# 项目根路径
__C.ROOT_DIR = osp.abspath(osp.join(osp.dirname(__file__), '..', '..'))
 
# Data directory
# 数据路径
__C.DATA_DIR = osp.abspath(osp.join(__C.ROOT_DIR, 'data'))
 
# Model directory
# 模型路径
__C.MODELS_DIR = osp.abspath(osp.join(__C.ROOT_DIR, 'models', 'pascal_voc'))
 
# Name (or path to) the matlab executable
# matlab executable
__C.MATLAB = 'matlab'
 
# Place outputs under an experiments directory
# 输出在experiments路径下
__C.EXP_DIR = 'default'
 
# Use GPU implementation of non-maximum suppression
# GPU实施非极大值抑制
__C.USE_GPU_NMS = True
 
# Default GPU device id
# 默认GPU id
__C.GPU_ID = 0
 
def get_output_dir(imdb, net=None):
    #返回输出路径，在experiments路径下
    """Return the directory where experimental artifacts are placed.
    If the directory does not exist, it is created.
 
    A canonical标准 path is built using the name from an imdb and a network
    (if not None).
    """
    outdir = osp.abspath(osp.join(__C.ROOT_DIR, 'output', __C.EXP_DIR, imdb.name))
    if net is not None:
        outdir = osp.join(outdir, net.name)
    if not os.path.exists(outdir):
        os.makedirs(outdir)
    return outdir
 
def _merge_a_into_b(a, b):
    #两个配置文件融合
    """Merge config dictionary a into config dictionary b, clobbering the
    options in b whenever they are also specified in a.
    """
    if type(a) is not edict:
        return
 
    for k, v in a.iteritems():
        # a must specify keys that are in b
        if not b.has_key(k):
            raise KeyError('{} is not a valid config key'.format(k))
 
        # the types must match, too
        old_type = type(b[k])
        if old_type is not type(v):
            if isinstance(b[k], np.ndarray):
                v = np.array(v, dtype=b[k].dtype)
            else:
                raise ValueError(('Type mismatch ({} vs. {}) '
                                'for config key: {}').format(type(b[k]),
                                                            type(v), k))
 
        # recursively merge dicts
        if type(v) is edict:
            try:
                _merge_a_into_b(a[k], b[k])
            except:
                print('Error under config key: {}'.format(k))
                raise
        #用配置a更新配置b的对应项
        else:
            b[k] = v
 
def cfg_from_file(filename):
    """Load a config file and merge it into the default options."""
    # 导入配置文件并与默认选项融合
    import yaml
    with open(filename, 'r') as f:
        yaml_cfg = edict(yaml.load(f))
 
    _merge_a_into_b(yaml_cfg, __C)
 
def cfg_from_list(cfg_list):
    # 命令行设置config
    """Set config keys via list (e.g., from command line)."""
    from ast import literal_eval
    assert len(cfg_list) % 2 == 0
    for k, v in zip(cfg_list[0::2], cfg_list[1::2]):
        key_list = k.split('.')
        d = __C
        for subkey in key_list[:-1]:
            assert d.has_key(subkey)
            d = d[subkey]
        subkey = key_list[-1]
        assert d.has_key(subkey)
        try:
            value = literal_eval(v)
        except:
            # handle the case when v is a string literal
            value = v
        assert type(value) == type(d[subkey]), \
            'type {} does not match original type {}'.format(
            type(value), type(d[subkey]))
        d[subkey] = value
```
- 
`AnchorTargetCreator裏的參數`

- 
将 20000 多个候选的 anchor 选出 256 个 anchor 进行**分类和回归位置**。选择过程如下：

- 
对于每一个 ground truth bounding box**选择和它重叠度（IoU）最高的一个 anchor **作为正样本

- 
对于剩下的 anchor，从中**选择和任意一个****重叠度超过 0.7 的 anchor**，作为正样本，正样本的数目不超过 128 个。

- 
随机选择和**重叠度小于 0.3 的 anchor 作为负样**本。负样本和正样本的总数为 256。

- 对于每个 anchor, gt_label 要么为 1（前景），要么为 0（背景），而 gt_loc 则是由 4 个位置参数 (tx,ty,tw,th) 组成，这样比直接回归座标更好。
- 计算分类损失用的是交叉熵损失，而计算回归损失用的是 Smooth_l1_loss. 在计算回归损失的时候，只**计算正样本（128个前景）的损失**，不计算负样本的位置损失。






