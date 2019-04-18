# 【YOLO】yolo.cfg文件  源码必备！ - YZXnuaa的博客 - CSDN博客
2018年03月01日 15:16:37[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2195
# YOLO配置文件理解
```
[net]
batch=64                           每batch个样本更新一次参数。
subdivisions=8                     如果内存不够大，将batch分割为subdivisions个子batch，每个子batch的大
```
```
小为batch/subdivisions。
                                   在darknet代码中，会将batch/subdivisions命名为batch。
height=416                         input图像的高
width=416                          Input图像的宽
channels=3                         Input图像的通道数
momentum=0.9                       动量
decay=0.0005                       权重衰减正则项，防止过拟合
angle=0                            通过旋转角度来生成更多训练样本
saturation = 1.5                   通过调整饱和度来生成更多训练样本
exposure = 1.5                     通过调整曝光量来生成更多训练样本
hue=.1                             通过调整色调来生成更多训练样本
learning_rate=0.0001               初始学习率
max_batches = 45000                训练达到max_batches后停止学习
policy=steps                       调整学习率的policy，有如下policy：CONSTANT, STEP, EXP, POLY, STEPS,
```
```
SIG, RANDOM
steps=100,25000,35000              根据batch_num调整学习率
scales=10,.1,.1                    学习率变化的比例，累计相乘
[convolutional]
batch_normalize=1                  是否做BN
filters=32                         输出多少个特征图
size=3                             卷积核的尺寸
stride=1                           做卷积运算的步长
pad=1                              如果pad为0,padding由 padding参数指定。如果pad为1，padding大小为size/2
activation=leaky                   激活函数：logistic，loggy，relu，elu，relie，plse，hardtan，lhtan，linear，ramp，
```
```
leaky，tanh，stair
[maxpool]
size=2                             池化层尺寸
stride=2                           池化步进
......
......
#######
[route]                            the route layer is to bring finer grained features in from earlier in the
```
```
network
layers=-9
[reorg]                            passthrougth layer: make these features match the feature map size at the later
```
```
layer. 
                                   The end feature map is 13x13, the feature map from earlier is 26x26x512. 
                                   The reorg layer maps the 26x26x512 feature map onto a 13x13x2048 feature map 
                                   so that it can be concatenated with the feature maps at 13x13 resolution.
stride=2
[route]
layers=-1,-3                        route层起连接作用，reorg层来match特征图尺寸。
...
[convolutional]
size=1
stride=1
pad=1
filters=125                        region前最后一个卷积层的filters数是特定的，计算公式为filter=num*(classes+5) 
                                   5的意义是5个坐标，论文中的tx,ty,tw,th,to
activation=linear
[region]
anchors = 1.08,1.19,  3.42,4.41,  6.63,11.38,  9.42,5.11,  16.62,10.52          预选框，可以手工挑选，
                                                                                也可以通过k means 从训练样本中学出
bias_match=1
classes=20                         网络需要识别的物体种类数
coords=4                           每个box的4个坐标tx,ty,tw,th
num=5                              每个grid cell预测几个box,和anchors的数量一致。当想要使用更多anchors时需要调大
```
```
num，且如果调大num后训练时Obj趋近0的话可以尝试调大object_scale
softmax=1                          使用softmax做激活函数
jitter=.2                          通过抖动增加噪声来抑制过拟合
rescore=1                          暂理解为一个开关，非0时通过重打分来调整l.delta（预测值与真实值的差）
object_scale=5                     栅格中有物体时，bbox的confidence loss对总loss计算贡献的权重
noobject_scale=1                   栅格中没有物体时，bbox的confidence loss对总loss计算贡献的权重
class_scale=1                      类别loss对总loss计算贡献的权重                      
coord_scale=1                      bbox坐标预测loss对总loss计算贡献的权重
absolute=1
thresh = .6
random=0                           random为1时会启用Multi-Scale Training，随机使用不同尺寸的图片进行训练。
```
# darknet对应代码
找到cfg文件解析的代码，选择detector demo 作为入口
darknet.c文件 main 函数开始
```
} else if (0 == strcmp(argv[1], "detector")){
    run_detector(argc, argv);
```
Detector.c文件 run_detector函数
```
char *prefix = find_char_arg(argc, argv, "-prefix", 0);
float thresh = find_float_arg(argc, argv, "-thresh", .24);
float hier_thresh = find_float_arg(argc, argv, "-hier", .5);
int cam_index = find_int_arg(argc, argv, "-c", 0);
int frame_skip = find_int_arg(argc, argv, "-s", 0);
if(argc < 4){
    fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
    return;
}
char *gpu_list = find_char_arg(argc, argv, "-gpus", 0);
char *outfile = find_char_arg(argc, argv, "-out", 0);
......
......
else if(0==strcmp(argv[2], "demo")) {
    list *options = read_data_cfg(datacfg);
    int classes = option_find_int(options, "classes", 20);
    char *name_list = option_find_str(options, "names", "data/names.list");
    char **names = get_labels(name_list);
    demo(cfg, weights, thresh, cam_index, filename, names, classes, frame_skip, prefix, hier_thresh);
}
```
read_data_cfg函数解析配置文件，保存到options指针。
## class
```
int classes = option_find_int(options, "classes", 20);
```
classes为YOLO可识别的种类数
## batch、learning_rate、momentum、decay和 subdivisions
demo.c文件demo函数
```
net = parse_network_cfg(cfgfile);
```
Parser.c文件 parse_network_cfg函数
```
list *sections = read_cfg(filename);
node *n = sections->front;
if(!n) error("Config file has no sections");
network net = make_network(sections->size - 1);
net.gpu_index = gpu_index;
size_params params;
section *s = (section *)n->val;
list *options = s->options;
if(!is_network(s)) error("First section must be [net] or [network]");
parse_net_options(options, &net);
```
parse_net_options函数
```
net->batch = option_find_int(options, "batch",1);
net->learning_rate = option_find_float(options, "learning_rate", .001);
net->momentum = option_find_float(options, "momentum", .9);
net->decay = option_find_float(options, "decay", .0001);
int subdivs = option_find_int(options, "subdivisions",1);
net->time_steps = option_find_int_quiet(options, "time_steps",1);
net->batch /= subdivs;
net->batch *= net->time_steps;
net->subdivisions = subdivs;
```
learning_rate为初始学习率，训练时的真正学习率和学习率的策略及初始学习率有关。
momentum为动量，在训练时加入动量可以帮助走出local minima 以及saddle point。
decay是权重衰减正则项，用来防止过拟合。
batch的值等于cfg文件中的batch/subdivisions 再乘以time_steps。 
time_steps在yolo默认的cfg中是没有配置的，所以是默认值1。 
因此batch可以认为就是cfg文件中的batch/subdivisions。
前面有提到batch的意义是每batch个样本更新一次参数。
而subdivisions的意义在于降低对GPU memory的要求。 
darknet将batch分割为subdivisions个子batch，每个子batch的大小为batch/subdivisions，并将子batch命名为batch。
我们看下训练时和batch有关的代码
Detector.c文件的train_detector函数
```
#ifdef GPU
    if(ngpus == 1){
        loss = train_network(net, train);
    } else {
        loss = train_networks(nets, ngpus, train, 4);
    }
#else
    loss = train_network(net, train);
#endif
```
Network.c文件的train_network函数
```
int batch = net.batch;
int n = d.X.rows / batch;
float *X = calloc(batch*d.X.cols, sizeof(float));
float *y = calloc(batch*d.y.cols, sizeof(float));
int i;
float sum = 0;
for(i = 0; i < n; ++i){
    get_next_batch(d, batch, i*batch, X, y);
    float err = train_network_datum(net, X, y);
    sum += err;
}
```
train_network_datum函数
```
*net.seen += net.batch;
......
......
forward_network(net, state);
backward_network(net, state);
float error = get_network_cost(net);
if(((*net.seen)/net.batch)%net.subdivisions == 0) update_network(net);
```
我们看到，只有((*net.seen)/net.batch)%net.subdivisions == 0时才会更新网络参数。 
*net.seen是已经训练过的子batch数，((*net.seen)/net.batch)%net.subdivisions的意义正是已经训练过了多少个真正的batch。
## policy、steps和scales
Parser.c文件 parse_network_cfg函数
```
char *policy_s = option_find_str(options, "policy", "constant");
net->policy = get_policy(policy_s);
net->burn_in = option_find_int_quiet(options, "burn_in", 0);
if(net->policy == STEP){
    net->step = option_find_int(options, "step", 1);
    net->scale = option_find_float(options, "scale", 1);
} else if (net->policy == STEPS){
    char *l = option_find(options, "steps");   
    char *p = option_find(options, "scales");   
    if(!l || !p) error("STEPS policy must have steps and scales in cfg file");
    int len = strlen(l);
    int n = 1;
    int i;
    for(i = 0; i < len; ++i){
        if (l[i] == ',') ++n;
    }
    int *steps = calloc(n, sizeof(int));
    float *scales = calloc(n, sizeof(float));
    for(i = 0; i < n; ++i){
        int step    = atoi(l);
        float scale = atof(p);
        l = strchr(l, ',')+1;
        p = strchr(p, ',')+1;
        steps[i] = step;
        scales[i] = scale;
    }
    net->scales = scales;
    net->steps = steps;
    net->num_steps = n;
} else if (net->policy == EXP){
    net->gamma = option_find_float(options, "gamma", 1);
} else if (net->policy == SIG){
    net->gamma = option_find_float(options, "gamma", 1);
    net->step = option_find_int(options, "step", 1);
} else if (net->policy == POLY || net->policy == RANDOM){
    net->power = option_find_float(options, "power", 1);
}
```
get_policy函数
```
if (strcmp(s, "random")==0) return RANDOM;
if (strcmp(s, "poly")==0) return POLY;
if (strcmp(s, "constant")==0) return CONSTANT;
if (strcmp(s, "step")==0) return STEP;
if (strcmp(s, "exp")==0) return EXP;
if (strcmp(s, "sigmoid")==0) return SIG;
if (strcmp(s, "steps")==0) return STEPS;
fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
return CONSTANT;
```
学习率动态调整的策略有多种，YOLO默认使用的是steps。
yolo-voc.cfg文件：
steps=100,25000,35000
scales=10,.1,.1
Network.c文件get_current_rate函数
```
int batch_num = get_current_batch(net);
int i;
float rate;
switch (net.policy) {
    case CONSTANT:
        return net.learning_rate;
    case STEP:
        return net.learning_rate * pow(net.scale, batch_num/net.step);
    case STEPS:
        rate = net.learning_rate;
        for(i = 0; i < net.num_steps; ++i){
            if(net.steps[i] > batch_num) return rate;
            rate *= net.scales[i];
            //if(net.steps[i] > batch_num - 1 && net.scales[i] > 1) reset_momentum(net);
        }
        return rate;
```
get_current_batch获取的是(*net.seen)/(net.batch*net.subdivisions)，即真正的batch。
steps的每个阶段是根据batch_num划分的，根据配置文件，学习率会在batch_num达到100、25000、35000时发生改变。
当前的学习率是初始学习率与当前阶段及之前所有阶段对应的scale的总乘积。
## convolutional超参数加载
Parser.c文件parse_network_cfg函数
```
LAYER_TYPE lt = string_to_layer_type(s->type);
        if(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
```
parse_convolutional函数
```
int n = option_find_int(options, "filters",1);
int size = option_find_int(options, "size",1);
int stride = option_find_int(options, "stride",1);
int pad = option_find_int_quiet(options, "pad",0);
int padding = option_find_int_quiet(options, "padding",0);
if(pad) padding = size/2;
char *activation_s = option_find_str(options, "activation", "logistic");
ACTIVATION activation = get_activation(activation_s);
int batch,h,w,c;
h = params.h;
w = params.w;
c = params.c;
batch=params.batch;
if(!(h && w && c)) error("Layer before convolutional layer must output image.");
int batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
```
需要注意的是如果enable了pad，cfg文件中的padding不会生效，实际的padding值为size/2。
## random
YOLOv2新增了一些训练技巧，Multi-Scale Training就是其中之一，如果random置为1，会启用Multi-Scale Training。 
启用Multi-Scale Training时每10个Batch，网络会随机地选择一个新的图片尺寸，由于使用的down samples是32，所以不同的尺寸大小也选择为32的倍数{320，352…..608}，最小320*320，最大608*608，网络会自动改变尺寸，并继续训练的过程。 
这一策略让网络在不同的输入尺寸上都能达到一个很好的预测效果，同一网络能在不同分辨率上进行检测。当输入图片尺寸比较小的时候跑的比较快，输入图片尺寸比较大的时候精度高。
## route 和 reorg
YOLOv2新增了Fine-Grained Features技巧，参考特征金字塔和ResNet，把高分辨率特征与低分辨率特征联系在一起，从而增加对小物体的识别精度。 
YOLOv2加上了一个Passthrough Layer来取得之前的某个26*26分辨率的层的特征。这个Passthrough layer把26 * 26的特征图与13 * 13的特征图联系在一起，把相邻的特征堆积在不同的Channel之中，类似与Resnet的Identity Mapping，从而把26*26*512变成13*13*2048。 
route层起连接作用，reorg层来match特征图尺寸。
汇总：
1.Batch_Size(批尺寸)
该参数主要用于批梯度下降算法(Batch Gradient Descent)中，批梯度下降算法是每次迭代都遍历批中的所有样本，由批中的样本共同决定最优的方向，Batch_Size 正是批中的样本数量。 
若数据集比较小，可以采用全数据集(Full Batch Learning)的形式，由全数据确定的方向能更好地代表样本总体，从而更准确地朝向极值所在的方向；但该种方式并不适用于大数据集； 
另一个极端是每次只训练一个样本，即Batch_Size=1，每次修正方向以各自样本的梯度方向修正，横冲直撞各自为政，难以达到收敛。 
在合理范围内增大Batch_Size,可以 
（1）提高内存利用率，进而提高大矩阵乘法的并行效率； 
（2）跑完一次epoch（全数据集）所需的迭代次数减少，对于相同数据量的处理速度进一步加快； 
（3）在一定范围内，一般来说Batch_Size越大，其确定的下降方向越准，引起的训练震荡越小。 
盲目增大Batch_Size的坏处： 
（1）超出内存容量； 
（2）跑完一次epoch(全数据集)所需的迭代次数减小，要想达到相同的精度，所需要的epoch数量越多，对参数的修正更加缓慢； 
（3）Batch_Size 增大到一定程度，其确定的下降方向已经基本不再变化； 
Batch_Size参数调试： 
大的Batch_Size在显存能允许的情况下收敛速度是比较快的但有时会陷入局部最小的情况；小Batch_Size引入的随机性会更大些，有可能会有更好的效果，但是收敛速度会慢一些；当Batch_Size太小，而类别数又比较多的时候,会导致loss函数震荡而不收敛。具体调试过程中，一般可根据GPU显存，设置为最大，而且一般要求是8的倍数，选择一部分数据，跑几个Batch看看loss是否在变小，再选择合适的Batch_Size。 
每Batch_Size个样本更新一次参数。
2.subdivisions
如果内存不够大，将batch分割为subdivisions个子batch,每个子batch的大小为batch/subdivisions； 
darknet代码中，是将batch/subdivisions命名为batch；
3.冲量-momentum
梯度下降法中一种常用的加速技术，对于一般的SGD，其表达式为
沿着负梯度方向下降，而带momentum项的SGD则写成
其中是momentum系数，通俗的理解上面的式子就是，如果上一次的momentum（即）与这一次的负梯度方向是相同的，那么这次下降的幅度就会加大，因此可以起到加速收敛的作用，冲量的建议配置为0.9。
4.权值衰减-weight decay
使用的目的是防止过拟合，当网络逐渐过拟合时网络权值往往会变大，因此，为了避免过拟合，在每次迭代过程中以某个小因子降低每个权值，也等效于给误差函数添加一个惩罚项，常用的惩罚项是所有权重的平方乘以一个衰减常量之和。权值衰减惩罚项使得权值收敛到较小的绝对值。
5.angle、saturation、exposure、hue
angle：图片角度变化，单位为度，假如angle=5，就是生成新图片的时候随机旋转-5~5度  
saturation & exposure: 饱和度与曝光变化大小，tiny-yolo-voc.cfg中1到1.5倍，以及1/1.5~1倍  
hue：色调变化范围,tiny-yolo-voc.cfg中-0.1~0.1  
在每次迭代中，会基于角度、饱和度、曝光、色调产生新的训练图片。
6.学习率-learning rate
学习率决定了参数移动到最优值的速度快慢，如果学习率过大，很可能会越过最优值导致函数无法收敛，甚至发散；反之，如果学习率过小，优化的效率可能过低，算法长时间无法收敛，也易使算法陷入局部最优（非凸函数不能保证达到全局最优）。合适的学习率应该是在保证收敛的前提下，能尽快收敛。 
设置较好的learning rate，需要不断尝试。在一开始的时候，可以将其设大一点，这样可以使weights快一点发生改变，在迭代一定的epochs之后人工减小学习率。 
在yolo训练中，网络训练160epoches，初始学习率为0.001，在60和90epochs时将学习率除以10。
7.burn_in
与学习率的动态变化有关？？？ 
if (batch_num < net.burn_in) return net.learning_rate * pow((float)batch_num / net.burn_in, net.power); 
Yolo network.c中出现的代码
8.最大迭代次数-max_batches
权重更新次数
9.调整学习率的策略-policy
调整学习率的policy，有如下policy：CONSTANT, STEP, EXP, POLY，STEPS, SIG, RANDOM
10.学习率变化时的迭代次数-steps
根据batch_num调整学习率，若steps=100,25000,35000，则在迭代100次，25000次，35000次时学习率发生变化，该参数与policy中的steps对应；
11.学习率变化的比率-scales
相对于当前学习率的变化比率，累计相乘，与steps中的参数个数保持一致；
12.是否做BN-batch_normalize
13.激活函数-activation
包括logistic，loggy，relu，elu，relie，plse，hardtan，lhtan，linear，ramp，leaky，tanh，stair
14.[route] layer
the route layer is to bring finer grained features in from earlier in the network 
15.[reorg] layer 
the reorg layer is to make these features match the feature map size at the later layer;The end feature map is 13x13, the feature map from earlier is 26x26x512. The reorg layer maps the 26x26x512 feature map onto a 13x13x2048 feature map so that it can be concate_nated with the feature maps at 13x13 resolution.
16.anchors
anchors：预测框的初始宽高，第一个是w，第二个是h，总数量是num*2，YOLOv2作者说anchors是使用K-MEANS获得，其实就是计算出哪种类型的框比较多，可以增加收敛速度，如果不设置anchors，默认是0.5;
17.jitter
通过抖动增加噪声来抑制过拟合
18.rescore
可理解为一个开关，非0时通过重打分来调整l.delta（预测值与真实值的差）
19.random(yolo模型训练)
random为1时会启用Multi-Scale Training，随机使用不同尺寸的图片进行训练,如果为0，每次训练大小与输入大小一致; 
是否随机确定最后的预测框
几个尺寸说明
（1）batch_size：批大小。在深度学习中，一般采用SGD训练，即每次训练在训练集中取batch_size个样本训练； 
（2）iteration：1个iteration等于使用batchsize个样本训练一次； 
（3）epoch：1个epoch等于使用训练集中的全部样本训练一次；
训练log中各参数的意义 
Region Avg IOU：平均的IOU，代表预测的bounding box和ground truth的交集与并集之比，期望该值趋近于1。 
Class:是标注物体的概率，期望该值趋近于1. 
Obj：期望该值趋近于1. 
No Obj:期望该值越来越小但不为零. 
Avg Recall：期望该值趋近1 
avg：平均损失，期望该值趋近于0
