# 【用Python学习Caffe】3. 图像训练测试数据集LMDB的生成 - tostq的专栏 - CSDN博客





2017年06月22日 22:12:38[tostq](https://me.csdn.net/tostq)阅读数：4139
所属专栏：[用Python学习Caffe](https://blog.csdn.net/column/details/16089.html)









## 3. 图像训练测试数据集LMDB的生成

Caffe深度学习网络在训练和测试中所使用的的数据集，并不是一张张的图像，而将图像集打包成LMDB或者LevelDB的数据库形式，这样做好处是能提高读取图像的时间。而本节的目的在于介绍如何将图像数据生成可用于测试和训练LMDB格式的数据集。

### 3.1 原始图像的采集

可以通过爬虫等方法从互联网中获得许多张图像（图像格式不限，JPG或者PNG都是可以的） 

然后将所有图像用顺序的数字重命名。
    def rename_img(Img_dir):
        # 重新命名Img,这里假设图像名称表示为000011.jpg、003456.jpg、000000.jpg格式，最高6位，前补0
        # 列出图像，并将图像改为序号名称
        listfile=os.listdir(Img_dir) # 提取图像名称列表
        total_num = 0
        for line in listfile:  #把目录下的文件都赋值给line这个参数
            if line[-4:] == '.jpg':
                newname = '{:0>6}'.format(total_num) +'.jpg'
                os.rename(os.path.join(Img_dir, line), os.path.join(Img_dir, newname))
                total_num+=1         #统计所有图像

### 3.2 确定类别标签

将所有类别用一组从0开始的数字标记，比如背景可以用0来表示，而狗可以用1来表示，其余类别依次用以下数字标记。前面我们知道，对于分类网络来说，其最终输出为一组向量，而这里的各个类别的数字标记代表了该组向量的某个值，如果该位置的值最大，表明该位置所代表的分类类别可能性最大。

前面的分类实验中的`synset_words.txt`实际就是一个列表类别文件，其包含1000行文本，每一行文本代表一个类别（每个类别有多个同名名称），而行号则代表对应于分类网络最终输出向量的位置。
    # synset_words.txt的内容
    n01440764 tench, Tinca tinca
    n01443537 goldfish, Carassius auratus
    n01484850 great white shark, white shark, man-eater, man-eating shark, Carcharodon carcharias
    n01491361 tiger shark, Galeocerdo cuvieri
    n01494475 hammerhead, hammerhead shark
    n01496331 electric ray, crampfish, numbfish, torpedo
    n01498041 stingray
    n01514668 cock
    n01514859 hen
    ... ...

而目标检测实验有的类别列表文件`labelmap_voc.prototxt`却不一样：
    item {
      name: "none_of_the_above"
      label: 0
      display_name: "background"
    }
    item {
      name: "aeroplane"
      label: 1
      display_name: "aeroplane"
    }
    item {
      name: "bicycle"
      label: 2
      display_name: "bicycle"
    }
    ... ...

生成该文件的Python实现例子：
    # 生成分类标签文件
    def labelmap(labelmap_file, label_info):
        labelmap = caffe_pb2.LabelMap()
        for i in range(len(label_info)):
            labelmapitem = caffe_pb2.LabelMapItem()
            labelmapitem.name = label_info[i]['name']
            labelmapitem.label = label_info[i]['label']
            labelmapitem.display_name = label_info[i]['display_name']
            labelmap.item.add().MergeFrom(labelmapitem)
        with open(labelmap_file, 'w') as f:
            f.write(str(labelmap))```
    ```label_info = [
        dict(name='none', label=0, display_name='background'),  # 背景
        dict(name="cat",label=1, display_name='cat'),  # 背景
        dict(name="dog",label=2, display_name='dog'),  # 背景
    ]
    labelmap(data_root+'labelmap_voc.prototxt', label_info)

### 3.3 图像的标记

对于监督的深度学习网络来说，其训练数据一般是带标签的，因此所采集的原始图像进行标记。 

对于分类任务来说，每张图像所代表的分类可以用一个数字来表示，而该数字代表了某个类别。

而对于目标检测任务来说，每张图像中可以会有多个目标，每个目标都有一组位置信息和分类信息，因此通常是通过xml文件来表示。这里推荐使用labelImg来进行图像标记，该软件可以直接通过`pip install labelImg`来进行安装，在Anaconda中安装完成后，在使用时可能会提示PyQt4错误，可以将所有import PyQt4的部分及使用PyQt4的部分删除。

![labelImg标记图像](http://i.imgur.com/tzDGOB9.png)

通过labelImg进行图像标记里，会对每张图像生成对应的同名的xml文件。

![生成xml](http://i.imgur.com/1uLYEBA.png)

### 3.4 生成训练及测试数据列表文件

所谓的数据列表文件主要是标明数据中各个图像的位置及其对应的标签。

对于深度学习网络的训练任务中一般会需要三组数据库： 

训练集，验证集及测试集。这里训练集中主要是用于训练网络，而验证集中主要是在训练网络中验证测试部分网络超参数，而测试集是用于考察网络的精度。因此，虽然三组数据集都是带标签的数据集，但训练集中的标签是网络可以已知的，也就是说网络会利用标签信息进行梯度更新，而验证集中的标签对于网络本身是未知的，但对于实验者是已知的，也就是说实验者可以通过验证集来验证一些网络在训练中不可改变的超参数。而测试集的标签对于实验者和网络都是未知的，其是用于考察实验者所设计的网络本身的有效性。

有时，我们也可以将训练集和验证集合二为一`trainval.txt`，但必须要有测试集`test.txt`，如果没有测试集，将无法考察网络的泛化能力（也即真实的检测精度）。因此在得到的大量的图像中，我们一般会随机抽取的一组图像作为测试集数据，而其他数据作为训练验证数据。

对于分类任务来说，数据列表文件中每一行包含图像地址及该图像对应的分类标号，比如
    # ILSVRC2012 测试数据列表文件test.txt中的内容
    ILSVRC2012_test_00000001.JPEG 0
    ILSVRC2012_test_00000002.JPEG 0
    ILSVRC2012_test_00000003.JPEG 0
    ILSVRC2012_test_00000004.JPEG 0
    ILSVRC2012_test_00000005.JPEG 0
    ILSVRC2012_test_00000006.JPEG 0
    ILSVRC2012_test_00000007.JPEG 0
    ILSVRC2012_test_00000008.JPEG 0
    ... ...

而对于目标检测数据集来说，其中每一行包含图像地址及该图像标记文件xml的地址，比如
    # VOC2007 测试数据列表文件train_val.txt中的内容
    VOC2007/JPEGImages/003746.jpg VOC2007/Annotations/003746.xml
    VOC2007/JPEGImages/004854.jpg VOC2007/Annotations/004854.xml
    VOC2007/JPEGImages/001589.jpg VOC2007/Annotations/001589.xml
    VOC2007/JPEGImages/000606.jpg VOC2007/Annotations/000606.xml
    VOC2007/JPEGImages/008234.jpg VOC2007/Annotations/008234.xml
    VOC2007/JPEGImages/001389.jpg VOC2007/Annotations/001389.xml
    VOC2007/JPEGImages/008650.jpg VOC2007/Annotations/008650.xml
    VOC2007/JPEGImages/007239.jpg VOC2007/Annotations/007239.xml
    ... ...

用Python生成列表文件的例子
    def detection_list(Img_dir, Ano_dir, Data_dir, test_num):
        # 造成目标检测图像数据库
        # Img_dir表示图像文件夹
        # Ano_dir表示图像标记文件夹，用labelImg生成
        # Data_dir生成的数据库文件地址
        # test_num测试图像的数目
        # 列出图像
        listfile=os.listdir(Img_dir) # 提取图像名称列表
        # 列出图像，并将图像改为序号名称
        total_num = 0
        for line in listfile:  #把目录下的文件都赋值给line这个参数
            if line[-4:] == '.jpg'
                total_num+=1         #统计所有图像`
        trainval_num = total_num-test_num # 训练图像数目
        # 生成训练图像及测试图像列表
        test_list_file=open(Data_dir+'/test.txt','w')
        train_list_file=open(Data_dir+'/trainval.txt','w')
        test_list = np.random.randint(0,total_num-1, size=test_num)
        train_list = range(total_num)
        for n in range(test_num):
            train_list.remove(test_list[n])
        random.shuffle(train_list)
        # 测试图像排序，而训练图像不用排序
        test_list = np.sort(test_list)
        # train_list = np.sort(train_list)
        for n in range(trainval_num):
            train_list_file.write(Img_dir + '{:0>6}'.format(train_list[n]) +'.jpg '+ Ano_dir + '{:0>6}'.format(train_list[n]) +'.xml\n')
        for n in range(test_num):
            test_list_file.write(Img_dir + '{:0>6}'.format(test_list[n]) +'.jpg '+ Ano_dir + '{:0>6}'.format(test_list[n]) +'.xml\n')

### 3.5 生成lmdb文件

生成lmdb或者leveldb文件主要要用到caffe的exe工具create_annoset，该软件可以由caffe的VS项目所生成，其是由C++所编写的。 

因此可以直接通过Python调用create_annoset接口得到。
    def create_annoset(anno_args):
        if anno_args.anno_type == "detection":
            cmd = "E:\Code\windows-ssd/Build/x64/Release/convert_annoset.exe" \
                  " --anno_type={}" \
                  " --label_type={}" \
                  " --label_map_file={}" \
                  " --check_label={}" \
                  " --min_dim={}" \
                  " --max_dim={}" \
                  " --resize_height={}" \
                  " --resize_width={}" \
                  " --backend={}" \
                  " --shuffle={}" \
                  " --check_size={}" \
                  " --encode_type={}" \
                  " --encoded={}" \
                  " --gray={}" \
                  " {} {} {}" \
                .format(anno_args.anno_type, anno_args.label_type, anno_args.label_map_file, anno_args.check_label,
                        anno_args.min_dim, anno_args.max_dim, anno_args.resize_height, anno_args.resize_width, anno_args.backend, anno_args.shuffle,
                        anno_args.check_size, anno_args.encode_type, anno_args.encoded, anno_args.gray, anno_args.root_dir, anno_args.list_file, anno_args.out_dir)
        elif anno_args.anno_type == "classification":
            cmd = "E:\Code\windows-ssd/Build/x64/Release/convert_annoset.exe" \
                  " --anno_type={}" \
                  " --min_dim={}" \
                  " --max_dim={}" \
                  " --resize_height={}" \
                  " --resize_width={}" \
                  " --backend={}" \
                  " --shuffle={}" \
                  " --check_size={}" \
                  " --encode_type={}" \
                  " --encoded={}" \
                  " --gray={}" \
                  " {} {} {}" \
                .format(anno_args.anno_type, anno_args.min_dim, anno_args.max_dim, anno_args.resize_height,
                        anno_args.resize_width, anno_args.backend, anno_args.shuffle, anno_args.check_size, anno_args.encode_type, anno_args.encoded,
                        anno_args.gray, anno_args.root_dir, anno_args.list_file, anno_args.out_dir)
        print cmd
        os.system(cmd)

### 3.6 具体代码下载

GitHub仓库[Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)中的[generate_lmdb.py](https://github.com/tostq/Caffe-Python-Tutorial/blob/master/generate_lmdb.py)

项目地址：[https://github.com/tostq/Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)



