# yahoo鉴黄模型测试 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月08日 13:38:13[QuJack](https://me.csdn.net/A784586)阅读数：647标签：[CNN鉴黄																[yahoo鉴黄																[鉴黄模型																[深度学习鉴黄](https://so.csdn.net/so/search/s.do?q=深度学习鉴黄&t=blog)
个人分类：[机器学习深度学习](https://blog.csdn.net/A784586/article/category/6844322)





**yahoo鉴黄模型测试**

参看网址：https://github.com/yahoo/open_nsfw

--------------------------------------------------------------------------

环境和平台 ： python3+win10+caffe

-------------------------------------------------------------------------



```python
#coding=utf-8
"""
#coding=utf-8
Copyright 2016 Yahoo Inc.
Licensed under the terms of the 2 clause BSD license. 
Please see LICENSE file in the project root for terms.
"""

import numpy as np
import os
import sys
import argparse
import glob
import time
import locale
from PIL import Image
#import Image
#from StringIO import StringIO
from io import StringIO
from io import BytesIO
import caffe
import sys
print(sys.getdefaultencoding())
import chardet

fileWrite=open('log.json','w')

def resize_image(data, sz=(256, 256)):
    """
    Resize image. Please use this resize logic for best results instead of the 
    caffe, since it was used to generate training dataset 
    :param str data:
        The image data
    :param sz tuple:
        The resized image dimensions
    :returns bytearray:
        A byte array with the resized image
    """
    print(type(data))
    #im = Image.open(BytesIO(data))
    im = Image.open(BytesIO(data))############
    #
    if im.mode != "RGB":
        im = im.convert('RGB')
    imr = im.resize(sz, resample=Image.BILINEAR)
    fh_im = BytesIO()########fh_im = BytesIO()
    imr.save(fh_im, format='JPEG')
    fh_im.seek(0)
    return bytearray(fh_im.read())

def caffe_preprocess_and_compute(pimg, caffe_transformer=None, caffe_net=None,
    output_layers=None):
    """
    Run a Caffe network on an input image after preprocessing it to prepare
    it for Caffe.
    :param PIL.Image pimg:
        PIL image to be input into Caffe.
    :param caffe.Net caffe_net:
        A Caffe network with which to process pimg afrer preprocessing.
    :param list output_layers:
        A list of the names of the layers from caffe_net whose outputs are to
        to be returned.  If this is None, the default outputs for the network
        are returned.
    :return:
        Returns the requested outputs from the Caffe net.
    """
    if caffe_net is not None:

        # Grab the default output names if none were requested specifically.
        if output_layers is None:
            output_layers = caffe_net.outputs

        img_data_rs = resize_image(pimg, sz=(256, 256))
        image = caffe.io.load_image(BytesIO(img_data_rs))

        H, W, _ = image.shape
        _, _, h, w = caffe_net.blobs['data'].data.shape
        #print(image.shape)
        h_off = int(max((H - h) / 2, 0))
        w_off = int(max((W - w) / 2, 0))
        crop = image[h_off:h_off + h, w_off:w_off + w, :]
        transformed_image = caffe_transformer.preprocess('data', crop)
        transformed_image.shape = (1,) + transformed_image.shape

        input_name = caffe_net.inputs[0]
        all_outputs = caffe_net.forward_all(blobs=output_layers,
                    **{input_name: transformed_image})

        outputs = all_outputs[output_layers[0]][0].astype(float)
        return outputs
    else:
        return []


def main(argv):
    pycaffe_dir = os.path.dirname(__file__)

    parser = argparse.ArgumentParser()
    # Required arguments: input file.
    parser.add_argument(
        "--input_file",
        help="Input image, directory, or npy."
    )

   #Optional arguments.
   #模型定义
    parser.add_argument(
        "--model_def",
        default=os.path.join(pycaffe_dir,
                "../nsfw_model/deploy.prototxt"),
        help="Model definition file."
    )
	#模型
    parser.add_argument(
        "--pretrained_model",
        default=os.path.join(pycaffe_dir, "../nsfw_model/resnet_50_1by2_nsfw.caffemodel"),
        help="Trained model weights file."
    )
	#后缀
    parser.add_argument(
        "--ext",
        default='jpg',
        help="Image file extension to take as input when a directory " +
             "is given as the input file."
    )

    args = parser.parse_args()
    args.input_file = os.path.expanduser(args.input_file)
    if os.path.isdir(args.input_file):
        print("Loading folder: %s" % args.input_file)
        inputs =[im_f
                for im_f in glob.glob(args.input_file + '/*.' + args.ext)]
    else:
        print("Loading file: %s" % args.input_file)
        inputs = [args.input_file]
    print("inputs len:",len(inputs))
    my_dict={}
    for i in range(0,len(inputs)):
        path_img=inputs[i]
		#print("leixing1:",type(path_img))
		#print ("当前系统：",locale.getdefaultlocale())
		#locale.setlocale(locale.LC_ALL, locale='zh_CN.GB2312')
		#print ("当前系统：",locale.getlocale())
        image_data = open(path_img,'rb').read()
		#utf8 gbk  utf-16  GB2312  unicode_escape  zh_CN.cp936  ,'r',encoding='utf8'
		#print(image_data)
		#print("image_data bianma:",chardet.detect(image_data))
		#print("leixing2:",type(image_data))
		# Pre-load caffe model.
        nsfw_net = caffe.Net(args.model_def, args.pretrained_model, caffe.TEST) # pylint: disable=invalid-name

		# Load transformer
		# Note that the parameters are hard-coded for best results
        caffe_transformer = caffe.io.Transformer({'data': nsfw_net.blobs['data'].data.shape})
        caffe_transformer.set_transpose('data', (2, 0, 1))  # move image channels to outermost
        caffe_transformer.set_mean('data', np.array([104, 117, 123]))  # subtract the dataset-mean value in each channel
        caffe_transformer.set_raw_scale('data', 255)  # rescale from [0, 1] to [0, 255]
        caffe_transformer.set_channel_swap('data', (2, 1, 0))  # swap channels from RGB to BGR

		# Classify.
        scores = caffe_preprocess_and_compute(image_data, caffe_transformer=caffe_transformer, caffe_net=nsfw_net, output_layers=['prob'])

		# Scores is the array containing SFW / NSFW image probabilities
		# scores[1] indicates the NSFW probability
        print ("NSFW score:  " , scores[1])
        if(scores[1]<0.000000003):
            my_dict.update({inputs[i]:scores[1]})
        if(scores[1]<0.0000003):
            my_dict.update({inputs[i]:scores[1]})
        else:
            my_dict.update({inputs[i]:scores[1]})
    for key in my_dict:
        fileWrite.write(key + ":"+str(my_dict[key])+"\n")
    fileWrite.close()
    print ("finished")

if __name__ == '__main__':
    main(sys.argv)
```](https://so.csdn.net/so/search/s.do?q=鉴黄模型&t=blog)](https://so.csdn.net/so/search/s.do?q=yahoo鉴黄&t=blog)](https://so.csdn.net/so/search/s.do?q=CNN鉴黄&t=blog)




