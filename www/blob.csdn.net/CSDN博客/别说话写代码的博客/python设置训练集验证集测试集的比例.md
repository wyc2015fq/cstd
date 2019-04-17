# python设置训练集验证集测试集的比例 - 别说话写代码的博客 - CSDN博客





2018年12月18日 16:20:58[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：313








```python
# -*- coding: utf-8 -*-
import os
import argparse as ap
import random
import math

Path = "../BCCD/JPEGImages/"
Out_Path = "../BCCD/ImageSets/Main/"

if __name__ == "__main__":
    # Argument Parser
    parser = ap.ArgumentParser()
    parser.add_argument("--images", help="Path to images",
                        default=Path)
    parser.add_argument("--output", help="Path to output directory",
                        default=Out_Path)
    args = vars(parser.parse_args())

    images_path = args["images"]
    output_dir = args["output"]
    trainval_rate = 0.9
    train_rate = 0.8

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    images_names = os.listdir(images_path)
    images_list = []
    for img in images_names:
        images_list.append(img.split('.')[0])
    random.shuffle(images_list)
    annotation_num = len(images_list)
    trainval_num = int(math.ceil(trainval_rate * annotation_num))
    train_num = int(math.ceil(trainval_num * train_rate))
    trainval = images_list[0:trainval_num]
    test = images_list[trainval_num:]
    train = trainval[0:train_num]
    val = trainval[train_num:trainval_num]
    trainval = sorted(trainval)
    train = sorted(train)
    val = sorted(val)
    test = sorted(test)
    fout = open(os.path.join(output_dir, "trainval.txt"), 'w')
    for line in trainval:
        fout.write(line + "\n")
    fout.close()
    fout = open(os.path.join(output_dir, "train.txt"), 'w')
    for line in train:
        fout.write(line + "\n")
    fout.close()
    fout = open(os.path.join(output_dir, "val.txt"), 'w')
    for line in val:
        fout.write(line + "\n")
    fout.close()
    fout = open(os.path.join(output_dir, "test.txt"), 'w')
    for line in test:
        fout.write(line + "\n")
    fout.close()
    print(annotation_num, len(trainval), len(test), len(train), len(val))
```





