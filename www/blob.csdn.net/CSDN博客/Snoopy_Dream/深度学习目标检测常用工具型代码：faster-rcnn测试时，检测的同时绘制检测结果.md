# 深度学习目标检测常用工具型代码：faster-rcnn测试时，检测的同时绘制检测结果 - Snoopy_Dream - CSDN博客





2018年04月25日 20:51:22[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：100标签：[检测																[faster-rcnn																[绘制																[tools](https://so.csdn.net/so/search/s.do?q=tools&t=blog)
个人分类：[DOTA																[代码分享																[faster-rcnn](https://blog.csdn.net/e01528/article/category/7599535)
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)](https://blog.csdn.net/e01528/article/category/7620830)




faster-rcnn的tools工具代码，模仿demo.py将所有的检测结果输出来，与此同时一边检测一边绘制于待检测的图片之上。





```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# --------------------------------------------------------
# Faster R-CNN
# Copyright (c) 2015 Microsoft
# Licensed under The MIT License [see LICENSE for details]
# Written by Ross Girshick
# --------------------------------------------------------

"""
将检测结果一张一张绘制出来
"""

import _init_paths
from fast_rcnn.config import cfg
from fast_rcnn.test import im_detect
from fast_rcnn.nms_wrapper import nms
from utils.timer import Timer
import matplotlib.pyplot as plt
import numpy as np
import scipy.io as sio
import caffe, os, sys, cv2
import argparse

CLASSES = ('__background__',
           'plane', 'baseball-diamond', 'bridge', 'ground-track-field', 'small-vehicle', 'large-vehicle', 'ship', 'tennis-court',
                'basketball-court', 'storage-tank',  'soccer-ball-field', 'roundabout', 'harbor', 'swimming-pool', 'helicopter')

NETS = {'vgg16': ('VGG16',
                  'vgg16_faster_rcnn_iter_20000.caffemodel'),
                   #'VGG16_faster_rcnn_final.caffemodel'),
        'vggcp': ('VGGcp',
                  'vggcp_faster_rcnn_iter_30000.caffemodel'),
        'zf': ('ZF',
                  'ZF_faster_rcnn_final.caffemodel')}
                  


def vis_detections(im, class_name, dets, thresh=0.5):
    """Draw detected bounding boxes."""
    inds = np.where(dets[:, -1] >= thresh)[0]
    if len(inds) == 0:
        return

    im = im[:, :, (2, 1, 0)]
    fig, ax = plt.subplots(figsize=(12, 12))
    ax.imshow(im, aspect='equal')
    for i in inds:
        bbox = dets[i, :4]
        score = dets[i, -1]

        ax.add_patch(
            plt.Rectangle((bbox[0], bbox[1]),
                          bbox[2] - bbox[0],
                          bbox[3] - bbox[1], fill=False,
                          edgecolor='red', linewidth=2)
            )
        ax.text(bbox[0], bbox[1] - 2,
                '{:s} {:.3f}'.format(class_name, score),
                bbox=dict(facecolor='blue', alpha=0.5),
                fontsize=8, color='white')

    ax.set_title(('{} detections with '
                  'p({} | box) >= {:.1f}').format(class_name, class_name,
                                                  thresh),
                  fontsize=14)
    plt.axis('off')
    plt.tight_layout()
    plt.draw()

###opencv draw
def vis_detections_cv(image_name,im, class_name, dets, thresh=0.5):
    """Draw detected bounding boxes."""
    inds = np.where(dets[:, -1] >= thresh)[0]
    if len(inds) == 0:
        return
    fname, extension=os.path.splitext(image_name)
    #检测结果txt的路径
    fid = open(os.path.join('/home/respectv/soft-nms-dota/data/VOCdevkit2007/testsplit_images_bbox2/','%s.txt' %fname),'w')
    for i in inds:
        bbox = dets[i, :4]
        score = dets[i, -1]
        bbox_info = '%s %s %f %d %d %d %d\n' % (fname,class_name,score,int(bbox[0]),int(bbox[1]),int(bbox[2]),int(bbox[3]))
        fid.writelines(bbox_info)
        cv2.rectangle(im,(bbox[0],int(bbox[1])),(int(bbox[2]),int(bbox[3])),(255,255,0),2)
        cv2.putText(im, '{:s}'.format(class_name), (int(bbox[0]), int(bbox[1] - 5)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,255), 2)
    fid.close()
    #检测结果绘制于图上的路径
    cv2.imwrite(os.path.join('/home/respectv/soft-nms-dota/data/VOCdevkit2007/testsplit_image/demo','%s.txt.jpg' % image_name),im)

def demo(net, image_name):
    """Detect object classes in an image using pre-computed object proposals."""
    im_file = os.path.join('/home/respectv/soft-nms-dota/data/VOCdevkit2007/testsplit_image/images', image_name)
    im = cv2.imread(im_file)
    # Detect all object classes and regress object bounds
    timer = Timer()
    timer.tic()
    scores, boxes = im_detect(net, im)
    timer.toc()
    print ('Detection took {:.3f}s for '
           '{:d} object proposals').format(timer.total_time, boxes.shape[0])
    # Visualize detections for each class
    CONF_THRESH = 0.6
    #CONF_THRESH = 0.8
    NMS_THRESH = 0.25
    #NMS_THRESH = 0.3
    for cls_ind, cls in enumerate(CLASSES[1:]):
        cls_ind += 1 # because we skipped background
        cls_boxes = boxes[:, 4*cls_ind:4*(cls_ind + 1)]
        cls_scores = scores[:, cls_ind]
        dets = np.hstack((cls_boxes,
                          cls_scores[:, np.newaxis])).astype(np.float32)
        keep = nms(dets, NMS_THRESH)
        dets = dets[keep, :]
        #vis_detections(im, cls, dets, thresh=CONF_THRESH)
        vis_detections_cv(image_name,im, cls, dets, thresh=CONF_THRESH)

def parse_args():
    """Parse input arguments."""
    parser = argparse.ArgumentParser(description='Faster R-CNN demo')
    parser.add_argument('--gpu', dest='gpu_id', help='GPU device id to use [0]',
                        default=0, type=int)
    parser.add_argument('--cpu', dest='cpu_mode',
                        help='Use CPU mode (overrides --gpu)',
                        action='store_true')
    parser.add_argument('--net', dest='demo_net', help='Network to use [vgg16]',
                        choices=NETS.keys(), default='vgg16')
    parser.add_argument('--nms', dest='soft_nms', help='wheather to use soft_nms',
                        default=1, type=int)

    args = parser.parse_args()
    return args

if __name__ == '__main__':
    cfg.TEST.HAS_RPN = True  # Use RPN for proposals

    args = parse_args()

    prototxt = os.path.join(cfg.MODELS_DIR, NETS[args.demo_net][0],
                            'faster_rcnn_end2end', 'test.prototxt')
                            #'faster_rcnn_alt_opt', 'faster_rcnn_test.pt')
    caffemodel = os.path.join(cfg.DATA_DIR, 'faster_rcnn_models',
                              NETS[args.demo_net][1])

    if not os.path.isfile(caffemodel):
        raise IOError(('{:s} not found.\nDid you run ./data/script/'
                       'fetch_faster_rcnn_models.sh?').format(caffemodel))

    if args.cpu_mode:
        caffe.set_mode_cpu()
    else:
        caffe.set_mode_gpu()
        caffe.set_device(args.gpu_id)
        cfg.GPU_ID = args.gpu_id
    net = caffe.Net(prototxt, caffemodel, caffe.TEST)

    print '\n\nLoaded network {:s}'.format(caffemodel)

    # Warmup on a dummy image
    im = 128 * np.ones((1024, 1024, 3), dtype=np.uint8)
    #im = 128 * np.ones((300, 500, 3), dtype=np.uint8)
    for i in xrange(2):
        _, _= im_detect(net, im)
    #待检测图片的路径
    im_dir = '/home/respectv/soft-nms-dota/data/VOCdevkit2007/testsplit_image/images'
    im_names = os.listdir(im_dir)
    image_num = 0
    for im_name in im_names:
        if 'txt' in im_name:
            continue
        image_num += 1
        print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
        print 'Demo for %s/%s' % (im_dir,im_name)
        demo(net, im_name)
    #plt.show()
    #cv2.destroyAllWindows()
```](https://blog.csdn.net/e01528/article/category/7620828)](https://so.csdn.net/so/search/s.do?q=绘制&t=blog)](https://so.csdn.net/so/search/s.do?q=faster-rcnn&t=blog)](https://so.csdn.net/so/search/s.do?q=检测&t=blog)




