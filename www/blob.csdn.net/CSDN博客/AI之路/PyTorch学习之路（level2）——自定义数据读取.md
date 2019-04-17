# PyTorch学习之路（level2）——自定义数据读取 - AI之路 - CSDN博客





2017年11月25日 22:33:29[AI之路](https://me.csdn.net/u014380165)阅读数：9885标签：[深度学习																[框架](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)
个人分类：[深度学习																[PyTorch](https://blog.csdn.net/u014380165/article/category/7286599)](https://blog.csdn.net/u014380165/article/category/6829229)

所属专栏：[PyTorch使用及源码解读](https://blog.csdn.net/column/details/19413.html)







在上一篇博客[PyTorch学习之路（level1）——训练一个图像分类模型](http://blog.csdn.net/u014380165/article/details/78525273)中介绍了如何用PyTorch训练一个图像分类模型，**建议先看懂那篇博客后再看这篇博客。**在那份代码中，采用torchvision.datasets.ImageFolder这个接口来读取图像数据，该接口默认你的训练数据是按照一个类别存放在一个文件夹下。**但是有些情况下你的图像数据不是这样维护的，比如一个文件夹下面各个类别的图像数据都有，同时用一个对应的标签文件，比如txt文件来维护图像和标签的对应关系，在这种情况下就不能用torchvision.datasets.ImageFolder来读取数据了，需要自定义一个数据读取接口。**另外这篇博客最后还顺带介绍如何保存模型和多GPU训练。

**怎么做呢？**

**先来看看torchvision.datasets.ImageFolder这个类是怎么写的**，主要代码如下，想详细了解的可以看：[官方github代码](https://github.com/pytorch/vision/blob/master/torchvision/datasets/folder.p)。

**看起来很复杂，其实非常简单**。继承的类是torch.utils.data.Dataset，主要包含三个方法：初始化`__init__`，获取图像`__getitem__`，数据集数量 `__len__`。`__init__`方法中先通过find_classes函数得到分类的类别名（classes）和类别名与数字类别的映射关系字典（class_to_idx）。然后通过make_dataset函数得到imags，这个imags是一个列表，其中每个值是一个tuple，每个tuple包含两个元素：图像路径和标签。剩下的就是一些赋值操作了。在`__getitem__`方法中最重要的就是 img = self.loader(path)这行，表示数据读取，可以从`__init__`方法中看出self.loader采用的是default_loader，这个default_loader的核心就是用python的PIL库的Image模块来读取图像数据。

```python
class ImageFolder(data.Dataset):
    """A generic data loader where the images are arranged in this way: ::

        root/dog/xxx.png
        root/dog/xxy.png
        root/dog/xxz.png

        root/cat/123.png
        root/cat/nsdf3.png
        root/cat/asd932_.png

    Args:
        root (string): Root directory path.
        transform (callable, optional): A function/transform that  takes in an PIL image
            and returns a transformed version. E.g, ``transforms.RandomCrop``
        target_transform (callable, optional): A function/transform that takes in the
            target and transforms it.
        loader (callable, optional): A function to load an image given its path.

     Attributes:
        classes (list): List of the class names.
        class_to_idx (dict): Dict with items (class_name, class_index).
        imgs (list): List of (image path, class_index) tuples
    """


    def __init__(self, root, transform=None, target_transform=None,
                 loader=default_loader):
        classes, class_to_idx = find_classes(root)
        imgs = make_dataset(root, class_to_idx)
        if len(imgs) == 0:
            raise(RuntimeError("Found 0 images in subfolders of: " + root + "\n"
                               "Supported image extensions are: " + ",".join(IMG_EXTENSIONS)))

        self.root = root
        self.imgs = imgs
        self.classes = classes
        self.class_to_idx = class_to_idx
        self.transform = transform
        self.target_transform = target_transform
        self.loader = loader

    def __getitem__(self, index):
        """
        Args:
            index (int): Index

        Returns:
            tuple: (image, target) where target is class_index of the target class.
        """
        path, target = self.imgs[index]
        img = self.loader(path)
        if self.transform is not None:
            img = self.transform(img)
        if self.target_transform is not None:
            target = self.target_transform(target)

        return img, target

    def __len__(self):
        return len(self.imgs)
```

稍微看下default_loader函数，该函数主要分两种情况调用两个函数，一般采用pil_loader函数。

```python
def pil_loader(path):
    with open(path, 'rb') as f:
        with Image.open(f) as img:
            return img.convert('RGB')

def accimage_loader(path):
    import accimage
    try:
        return accimage.Image(path)
    except IOError:
        # Potentially a decoding problem, fall back to PIL.Image
        return pil_loader(path)

def default_loader(path):
    from torchvision import get_image_backend
    if get_image_backend() == 'accimage':
        return accimage_loader(path)
    else:
        return pil_loader(path)
```

**看懂了ImageFolder这个类，就可以自定义一个你自己的数据读取接口了。**

**首先在PyTorch中和数据读取相关的类基本都要继承一个基类：torch.utils.data.Dataset。然后再改写其中的`__init__`、`__len__`、`__getitem__`等方法即可**。

下面假设img_path是你的图像文件夹，该文件夹下面放了所有图像数据（包括训练和测试），然后txt_path下面放了train.txt和val.txt两个文件，txt文件中每行都是图像路径，tab键，标签。所以下面代码的`__init__`方法中self.img_name和self.img_label的读取方式就跟你数据的存放方式有关，你可以根据你实际数据的维护方式做调整。`__getitem__`方法没有做太大改动，依然采用default_loader方法来读取图像。最后在Transform中将每张图像都封装成Tensor。

```python
class customData(Dataset):
    def __init__(self, img_path, txt_path, dataset = '', data_transforms=None, loader = default_loader):
        with open(txt_path) as input_file:
            lines = input_file.readlines()
            self.img_name = [os.path.join(img_path, line.strip().split('\t')[0]) for line in lines]
            self.img_label = [int(line.strip().split('\t')[-1]) for line in lines]
        self.data_transforms = data_transforms
        self.dataset = dataset
        self.loader = loader

    def __len__(self):
        return len(self.img_name)

    def __getitem__(self, item):
        img_name = self.img_name[item]
        label = self.img_label[item]
        img = self.loader(img_name)

        if self.data_transforms is not None:
            try:
                img = self.data_transforms[self.dataset](img)
            except:
                print("Cannot transform image: {}".format(img_name))
        return img, label
```

**定义好了数据读取接口后，怎么用呢？**

在代码中可以这样调用。

```bash
image_datasets = {x: customData(img_path='/ImagePath',
                                    txt_path=('/TxtFile/' + x + '.txt'),
                                    data_transforms=data_transforms,
                                    dataset=x) for x in ['train', 'val']}
```

这样返回的image_datasets就和用torchvision.datasets.ImageFolder类返回的数据类型一样，有点狸猫换太子的感觉，这就是在第一篇博客中说的写代码类似搭积木的感觉。

有了image_datasets，然后依然用torch.utils.data.DataLoader类来做进一步封装，将这个batch的图像数据和标签都分别封装成Tensor。

```python
dataloders = {x: torch.utils.data.DataLoader(image_datasets[x],
                                                 batch_size=batch_size,
                                                 shuffle=True) for x in ['train', 'val']}
```

另外，每次迭代生成的模型要怎么保存呢？非常简单，那就是用torch.save。输入就是你的模型和要保存的路径及模型名称，如果这个output文件夹没有，可以手动新建一个或者在代码里面新建。
`torch.save(model, 'output/resnet_epoch{}.pkl'.format(epoch))`
最后，关于多GPU的使用，PyTorch支持多GPU训练模型，假设你的网络是model，那么只需要下面一行代码（调用 torch.nn.DataParallel接口）就可以让后续的模型训练在0和1两块GPU上训练，加快训练速度。
` model = torch.nn.DataParallel(model, device_ids=[0,1])`
**完整代码请移步**：[Github](https://github.com/miraclewkf/ImageClassification-PyTorch/blob/master/level2/train_customData.py)





