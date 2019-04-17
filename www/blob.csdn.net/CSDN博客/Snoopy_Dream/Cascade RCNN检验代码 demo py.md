# Cascade RCNN检验代码 demo.py - Snoopy_Dream - CSDN博客





2018年08月16日 10:10:26[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：377
所属专栏：[cascade rcnn目标检测](https://blog.csdn.net/column/details/25672.html)









来源自：[https://gist.github.com/makefile/6731ca0e311b6401681c15635bb97330](https://gist.github.com/makefile/6731ca0e311b6401681c15635bb97330)



```python
import os
import sys
import argparse
import numpy as np
from PIL import Image, ImageDraw
import cv2
import time

# Make sure that caffe is on the python path:
caffe_root = '../../..'
#os.chdir(caffe_root)
sys.path.insert(0, os.path.join(caffe_root, 'python'))
import caffe

# from google.protobuf import text_format
# from caffe.proto import caffe_pb2

class CaffeDetection:
    def __init__(self, gpu_id, model_def, model_weights,
                 cascade=0, FPN=0):
        if gpu_id < 0:
            caffe.set_mode_cpu()
        else:
            caffe.set_device(gpu_id)
            caffe.set_mode_gpu()

        # Load the net in the test phase for inference, and configure input preprocessing.
        self.net = caffe.Net(model_def,      # defines the structure of the model
                             model_weights,  # contains the trained weights
                             caffe.TEST)     # use test mode (e.g., don't perform dropout)
        # input preprocessing: 'data' is the name of the input blob == net.inputs[0]
        #self.transformer = caffe.io.Transformer({'data': self.net.blobs['data'].data.shape})
        #self.transformer.set_transpose('data', (2, 0, 1))
        #self.transformer.set_mean('data', np.array([104, 117, 123])) # mean pixel
        ## the reference model operates on images in [0,255] range instead of [0,1]
        #self.transformer.set_raw_scale('data', 255)
        ## the reference model has channels in BGR order instead of RGB
        #self.transformer.set_channel_swap('data', (2, 1, 0))

        self.cascade = cascade > 0
        self.FPN = FPN > 0
        print cascade,FPN
        if not self.cascade:
            # baseline model
            if self.FPN:
                self.proposal_blob_names = ['proposals_to_all']
            else:
                self.proposal_blob_names = ['proposals']

            self.bbox_blob_names = ['output_bbox_1st']
            self.cls_prob_blob_names = ['cls_prob_1st']
            self.output_names = ['1st']
        else:
            # cascade-rcnn model
            if self.FPN:
                self.proposal_blob_names = ['proposals_to_all', 'proposals_to_all_2nd',
                                       'proposals_to_all_3rd', 'proposals_to_all_2nd', 'proposals_to_all_3rd']
            else:
                self.proposal_blob_names = ['proposals', 'proposals_2nd', 'proposals_3rd',
                                       'proposals_2nd', 'proposals_3rd']

            self.bbox_blob_names = ['output_bbox_1st', 'output_bbox_2nd', 'output_bbox_3rd',
                           'output_bbox_2nd', 'output_bbox_3rd']
            self.cls_prob_blob_names = ['cls_prob_1st', 'cls_prob_2nd', 'cls_prob_3rd',
                               'cls_prob_2nd_avg', 'cls_prob_3rd_avg']
            self.output_names = ['1st', '2nd', '3rd', '2nd_avg', '3rd_avg']

        self.num_outputs = len(self.proposal_blob_names)
        assert(self.num_outputs==len(self.bbox_blob_names))
        assert(self.num_outputs==len(self.cls_prob_blob_names))
        assert(self.num_outputs==len(self.output_names))
        # detection configuration
        # detect_final_boxes = np.zeros(nImg, num_outputs)
        #self.det_thr = 0.001 # threshold for testing
        self.det_thr = 0.3 # threshold for demo
        self.max_per_img = 100 # max number of detections
        self.nms_thresh = 0.5 # NMS
        if FPN:
            self.shortSize = 800
            self.longSize = 1312
        else:
            self.shortSize = 600
            self.longSize = 1000

        self.PIXEL_MEANS = np.array([104, 117, 123],dtype=np.uint8)
        self.num_cls = 80

    def detect(self, image_file):
        '''
        rcnn detection
        '''
        #image = caffe.io.load_image(image_file)
        image = cv2.imread(image_file) # BGR, default is cv2.IMREAD_COLOR 3-channel
        orgH, orgW, channel = image.shape
        print("image shape:",image.shape)
        rzRatio = self.shortSize / min(orgH, orgW)
        imgH = min(rzRatio * orgH, self.longSize)
        imgW = min(rzRatio * orgW, self.longSize)
        imgH = round(imgH / 32) * 32
        imgW = round(imgW / 32) * 32 # must be the multiple of 32
        hwRatios = [imgH/orgH, imgW/orgW]
        #transformed_image = self.transformer.preprocess('data', image)
        #image = cv2.resize(im_orig, None, None, fx=im_scale, fy=im_scale,
        resized_w = int(imgW)
        resized_h = int(imgH)
        print 'resized -> ',(resized_w, resized_h)
        image = cv2.resize(image, (resized_w, resized_h), interpolation=cv2.INTER_LINEAR)
        image -= self.PIXEL_MEANS
        #cv2.imwrite("transformed_image.jpg", image)
        transformed_image = np.transpose(image, (2,0,1)) # C H W

        # set net to batch size of 1
        self.net.blobs['data'].reshape(1, 3, resized_h, resized_w)

        #Run the net and examine the top_k results
        self.net.blobs['data'].data[...] = transformed_image.astype(np.float32, copy=False)

        start = time.time()
        # Forward pass.
        blobs_out = self.net.forward()
        print('output_bbox_1st---',blobs_out['output_bbox_1st'].shape)
        #print blobs_out
        end = time.time()
        cost_millis = int((end - start) * 1000)
        print "detection cost ms: ", cost_millis

        detect_final_boxes = []
        for nn in range(self.num_outputs):
            # detect_boxes = cell(num_cls, 1);
            tmp = self.net.blobs[self.bbox_blob_names[nn]].data.copy() # if no need modify,then no need copy
            print(self.bbox_blob_names[nn], tmp.shape)
            #tmp = tmp.reshape((-1,5))
            tmp = tmp[:,:,0,0]
            tmp[:,1] /= hwRatios[1]
            tmp[:,3] /= hwRatios[1]
            tmp[:,2] /= hwRatios[0]
            tmp[:,4] /= hwRatios[0]

            # clipping bbs to image boarders
            tmp[:, 1] = np.maximum(0,tmp[:,1])
            tmp[:, 2] = np.maximum(0,tmp[:,2])
            tmp[:, 3] = np.minimum(orgW,tmp[:,3])
            tmp[:, 4] = np.minimum(orgH,tmp[:,4])
            tmp[:, 3] = tmp[:, 3] - tmp[:, 1] + 1 # w
            tmp[:, 4] = tmp[:, 4] - tmp[:, 2] + 1 # h

            output_bboxs = tmp[:,1:]

            tmp = self.net.blobs[self.cls_prob_blob_names[nn]].data
            print(self.cls_prob_blob_names[nn], tmp.shape)
            cls_prob = tmp.reshape((-1,self.num_cls+1))

            tmp = self.net.blobs[self.proposal_blob_names[nn]].data.copy()
            print(self.proposal_blob_names[nn], tmp.shape)
            tmp = tmp[:,1:]
            tmp[:, 2] = tmp[:, 2] - tmp[:, 0] + 1  # w
            tmp[:, 3] = tmp[:, 3] - tmp[:, 1] + 1  # h
            proposals = tmp
            keep_id = np.where((proposals[:, 2] > 0) & (proposals[:, 3] > 0))[0]
            proposals = proposals[keep_id,:]
            output_bboxs = output_bboxs[keep_id,:]
            cls_prob = cls_prob[keep_id,:]

            detect_boxes = []
            for i in range(self.num_cls):
                cls_id = i + 1
                prob = cls_prob[:, cls_id][:, np.newaxis] # 0 is background
                #print (output_bboxs.shape, prob.shape)
                bbset = np.hstack([output_bboxs, prob])
                if self.det_thr > 0:
                    keep_id = np.where(prob >= self.det_thr)[0]
                    bbset = bbset[keep_id,:]

                keep = self.cpu_nms_single_cls(bbset, self.nms_thresh)
                if len(keep) == 0: continue
                bbset = bbset[keep,:]
                cls_ids = np.array([cls_id] * len(bbset))[:, np.newaxis]
                #print "cls_ids.shape", cls_ids.shape, bbset.shape
                detect_boxes.extend(np.hstack([cls_ids, bbset]).tolist())
            print "detected box num: ", len(detect_boxes)
            detect_boxes = np.asarray(detect_boxes)
            if self.max_per_img > 0 and len(detect_boxes) > self.max_per_img:
                rank_scores = detect_boxes[:, 5].copy()[::-1]
                rank_scores.sort() # 'descend'
                print len(rank_scores),self.max_per_img
                print np.where(detect_boxes[:, 5] >= rank_scores[self.max_per_img])
                keep_id = np.where(detect_boxes[:, 5] >= rank_scores[self.max_per_img])[0]
                detect_boxes = detect_boxes[keep_id,:]
            #detect_final_boxes.extend(detect_boxes.tolist())
            detect_final_boxes.append(detect_boxes.tolist())

        return detect_final_boxes

    def cpu_nms_single_cls(self, dets, thresh):
        """Pure Python NMS baseline."""
        x1 = dets[:, 0]
        y1 = dets[:, 1]
        w = dets[:, 2]
        h = dets[:, 3]
        scores = dets[:, 4]

        x2 = x1 + w - 1
        y2 = y1 + h - 1
        # areas = (x2 - x1 + 1) * (y2 - y1 + 1)
        areas = w * h
        order = scores.argsort()[::-1]

        keep = []
        while order.size > 0:
            i = order[0]
            keep.append(i)
            xx1 = np.maximum(x1[i], x1[order[1:]])
            yy1 = np.maximum(y1[i], y1[order[1:]])
            xx2 = np.minimum(x2[i], x2[order[1:]])
            yy2 = np.minimum(y2[i], y2[order[1:]])

            w = np.maximum(0.0, xx2 - xx1 + 1)
            h = np.maximum(0.0, yy2 - yy1 + 1)
            inter = w * h
            ovr = inter / (areas[i] + areas[order[1:]] - inter)

            inds = np.where(ovr <= thresh)[0]
            order = order[inds + 1]

        return keep

def main(args):
    '''main '''
    wordname_15 = ['__background__', 'plane', 'baseball-diamond', 'bridge', 'ground-track-field', 'small-vehicle', 'large-vehicle', 'ship', 'tennis-court',
                   'basketball-court', 'storage-tank',  'soccer-ball-field', 'roundabout', 'harbor', 'swimming-pool', 'helicopter']
    wordname_5 = ['__background__', '1:plane', '2:ship', '3:storage', '4:harbor', '5:bridge']
    # {cls_name: cls_id} # start from 1
    #cls_ids = {k: idx+1 for idx, k in enumerate(wordname_15)}

    detection = CaffeDetection(args.gpu_id,
                               args.model_def, args.model_weights,
                               cascade=args.cascade, FPN=args.FPN)
    results = detection.detect(args.image_file)
    #print(results)

    img = Image.open(args.image_file)
    draw = ImageDraw.Draw(img)
    width, height = img.size
    for item in results[len(results)-1]:# the 3rd_avg result
        xmin = int(round(item[1]))
        ymin = int(round(item[2]))
        xmax = int(round(item[1] + item[3] - 1))
        ymax = int(round(item[2] + item[4] - 1))
        cls_id = int(item[0])
        draw.rectangle([xmin, ymin, xmax, ymax], outline=(255, 0, 0))
        draw.text([xmin, ymin], str(cls_id), (0, 0, 255))
        print [cls_id, xmin, ymin, xmax, ymax, round(item[-1]*1000)/1000]

    img.save('detect_result.jpg')


def parse_args():
    '''parse args'''
    parser = argparse.ArgumentParser()
    parser.add_argument('--gpu_id', type=int, default=0, help='gpu id')
    parser.add_argument('--model_def',
                        default='models/deploy.prototxt')
    parser.add_argument('--cascade', default=0, type=int)
    parser.add_argument('--FPN', default=0, type=int)
    parser.add_argument('--model_weights',
                        default='models/models_iter_120000.caffemodel')
    parser.add_argument('--image_file', default='examples/images/fish-bike.jpg')
    return parser.parse_args()

if __name__ == '__main__':
    main(parse_args())
```





