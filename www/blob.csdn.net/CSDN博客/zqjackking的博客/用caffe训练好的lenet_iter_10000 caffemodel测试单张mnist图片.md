# 用caffe训练好的lenet_iter_10000.caffemodel测试单张mnist图片 - zqjackking的博客 - CSDN博客





2017年07月26日 19:08:11[zqjackking](https://me.csdn.net/zqjackking)阅读数：417标签：[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
个人分类：[深度学习](https://blog.csdn.net/zqjackking/article/category/6555105)









原文链接：[http://blog.csdn.net/lanxuecc/article/details/52485077](http://blog.csdn.net/lanxuecc/article/details/52485077)

1 第一种方法，使用caffe自带的caffe/python/classify.py 

因为classify.py中的测试接口caffe.Classifier需要训练图片的均值文件作为输入参数，而实际lenet-5训练时并未计算均值文件，所以这里创建一个全0的均值文件输入。编写一个zeronp.py文件如下 
```matlab
import numpy as np
zeros=np.zeros((28,28,1),dtype=no.float32)
np.save('meanfile.npy',zeros)
```

python zeronp.py来生成均值文件meanfile.npy，这里注意宽高要与输入测试的图片宽高一致 

caffe自带的classify.py，将其修改保存为classifymnist.py文件

```python
#!/usr/bin/env python
"""
classify.py is an out-of-the-box image classifer callable from the command line.

By default it configures and runs the Caffe reference ImageNet model.
"""
import numpy as np
import os
import sys
import argparse
import glob
import time
import pandas as pd 
sys.path.append('/home/zq/caffe/python')
import caffe

def main(argv):
    pycaffe_dir = os.path.dirname(__file__)

    parser = argparse.ArgumentParser()
    # Required arguments: input and output files.
    parser.add_argument(
        "input_file",
        help="Input image, directory, or npy."
    )
    parser.add_argument(
        "output_file",
        help="Output npy filename."
    )
    # Optional arguments.
    parser.add_argument(
        "--model_def",
        default=os.path.join(pycaffe_dir,
                "lenet.prototxt"), 
        help="Model definition file."
    )
    parser.add_argument(
        "--pretrained_model",
        default=os.path.join(pycaffe_dir,
                "chepai1/lenet_iter_200000.caffemodel"), 
        help="Trained model weights file."
    )
    parser.add_argument(
        "--labels_file",          #no_use
        default=os.path.join(pycaffe_dir,
                "../examples/mnist/synset_words.txt"), 
        help="mnist result words file"
    )
    parser.add_argument(
        "--force_grayscale",
        action='store_true',   
        help="Converts RGB images down to single-channel grayscale versions," +
                   "useful for single-channel networks like MNIST."
    )
    parser.add_argument(
        "--print_results",
        action='store_true', 
        help="Write output text to stdout rather than serializing to a file."
    )
    parser.add_argument(
        "--gpu",
        action='store_true',
        help="Switch for gpu computation."
    )
    parser.add_argument(
        "--center_only",
        action='store_true',
        help="Switch for prediction from center crop alone instead of " +
             "averaging predictions across crops (default)."
    )
    parser.add_argument(
        "--images_dim",
        default='28,28',     
        help="Canonical 'height,width' dimensions of input images."
    )
    parser.add_argument(
        "--mean_file",
        default=os.path.join(pycaffe_dir,
                             '../examples/mnist/meanfile.npy'), 
        help="Data set image mean of [Channels x Height x Width] dimensions " +
             "(numpy array). Set to '' for no mean subtraction."
    )
    parser.add_argument(
        "--input_scale",
        type=float,
        help="Multiply input features by this scale to finish preprocessing."
    )
    parser.add_argument(
        "--raw_scale",
        type=float,
        default=255.0,
        help="Multiply raw input by this scale before preprocessing."
    )
    parser.add_argument(
        "--channel_swap",
        default='2,1,0',
        help="Order to permute input channels. The default converts " +
             "RGB -> BGR since BGR is the Caffe default by way of OpenCV."
    )
    parser.add_argument(
        "--ext",
        default='jpg',
        help="Image file extension to take as input when a directory " +
             "is given as the input file."
    )
    args = parser.parse_args()

    image_dims = [int(s) for s in args.images_dim.split(',')]

    mean, channel_swap = None, None
    if not args.force_grayscale:
        if args.mean_file:
            mean = np.load(args.mean_file).mean(1).mean(1)
        if args.channel_swap:
            channel_swap = [int(s) for s in args.channel_swap.split(',')]

    if args.gpu:
        caffe.set_mode_gpu()
        print("GPU mode")
    else:
        caffe.set_mode_cpu()
        print("CPU mode")

    # Make classifier.
    classifier = caffe.Classifier(args.model_def, args.pretrained_model,
            image_dims=image_dims, mean=mean,
            input_scale=args.input_scale, raw_scale=args.raw_scale,
            channel_swap=channel_swap)

    # Load numpy array (.npy), directory glob (*.jpg), or image file.
    args.input_file = os.path.expanduser(args.input_file)
    if args.input_file.endswith('npy'):
        print("Loading file: %s" % args.input_file)
        inputs = np.load(args.input_file)
    elif os.path.isdir(args.input_file):
        print("Loading folder: %s" % args.input_file)
        inputs =[caffe.io.load_image(im_f)
                 for im_f in glob.glob(args.input_file + '/*.' + args.ext)]
    else:
        print("Loading image file: %s" % args.input_file)
        inputs = [caffe.io.load_image(args.input_file, not args.force_grayscale)] 

    print("Classifying %d inputs." % len(inputs))

    # Classify.
    start = time.time()
    scores = classifier.predict(inputs, not args.center_only).flatten()
    print("Done in %.2f s." % (time.time() - start))
    # print
    if args.print_results:
        labels=np.array(['0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z'])
        indices =(-scores).argsort()[:5]
        predictions = labels[indices]
        print labels
        print predictions
        print scores

        meta = [(p, '%.5f' % scores[i]) for i,p in zip(indices, predictions)]
        print meta


    # Save
    print("Saving results into %s" % args.output_file)
    np.save(args.output_file, predictions)


if __name__ == '__main__':
    main(sys.argv)
```

最后在终端执行如下命令 

python examples/mnist/classifymnist.py  –print_results –force_grayscale –center_only data/chepai/chepai_data_test/23/026893.jpg  examples/mnist/judge 

其中，–print_result是在终端打印结果，–force_grayscale是强制转为灰色，最后两个参数是输入文件和输出文件。以下就是终端的结果。 
![这里写图片描述](https://img-blog.csdn.net/20170726185556100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2 第2种方法  使用caffe自带的examples/cpp_classification/classification.cpp 

./build/examples/cpp_classification/classification.bin deploy.prototxt network.caffemodel mean.binaryproto labels.txt img.jpg 

其中，mean.binaryproto是test数据集的均值文件。用caffe自带的compute_image_mean生成。 

测试结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20170726190659669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面的数字表示softmax分类概率，只不过这里的概率都很小，前几位都是0 

这个工具有个缺点  就是不显示运算时间。





