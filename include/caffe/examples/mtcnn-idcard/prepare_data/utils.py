import numpy as np
import os
import math
import numpy.random as npr

def load_data(fn):
    f = open(fn, "r")
    data = []
    for line in f.readlines():                          #依次读取每行  
        annotation = line.strip().split(' ')
        #image path
        fn = annotation[0]
        s = annotation[1:]
        pts = []
        for i in range(len(s)//2):
            pts.append((float(s[i*2]), float(s[i*2+1])) )
        data.append((fn, pts))
    f.close()
    return data

def mkdir(path):
    if not os.path.exists(path):
        os.mkdir(path)

def save_list(fn, data):
    f = open(fn, 'w')
    for ss in data:
        f.write('%s\n'%(ss))
    f.close()

def bound_box(pts):
    if len(pts)==0:
        return (-1,-1,-1,-1)
    min_x,min_y = pts[0]
    max_x,max_y = pts[0]
    for x,y in pts:
        min_x = min(min_x, x)
        max_x = max(max_x, x)
        min_y = min(min_y, y)
        max_y = max(max_y, y)
    box = (min_x, min_y, max_x, max_y)
    return box

def box_size(box):
    return box[2]-box[0], box[3]-box[1]

def list_add(a,b):
    len = max(len(a), len(b))
    c = []
    for i in range(len):
        if i<len(a) and i<len(b):
            c.append(a[i]+a[b])
        elif i<len(a):
            c.append(a[i])
        else:
            c.append(b[i])
    return c

def box_ex(box,x,y):
    box = (box[0]-x, box[1]-y,box[2]+x, box[3]+y)
    return box

def pts_ex(pts):
    pts1 =[]
    pts1 += pts
    pts1 += [[pts[2][0]+pts[1][0]-pts[0][0],pts[2][1]+pts[1][1]-pts[0][1]]]
    #print(pts1)
    return pts1

def bound_box_ex(pts, r):
    bbox = bound_box(pts)
    box_w, box_h = box_size(bbox)
    bbox = box_ex(bbox, box_w*r, box_h*r)
    return bbox

def randbox(img_w,img_h,minsz):
    #size = npr.randint(40, min(img_w, img_h) / 2)
    minsz = min(minsz, img_w/2, img_h/2)
    size_w = npr.randint(minsz, img_w)
    size_h = npr.randint(minsz, img_h)
    nx = npr.randint(0, img_w - size_w)
    ny = npr.randint(0, img_h - size_h)
    return nx, ny, nx + size_w, ny + size_h

def box_offset_r(box, nbox):
    x1, y1, x2, y2 = box
    nx1, ny1, nx2, ny2 = nbox
    size_w = nx2-nx1
    size_h = ny2-ny1
    offset_x1 = (x1 - nx1) / float(size_w)
    offset_y1 = (y1 - ny1) / float(size_h)
    offset_x2 = (x2 - nx2) / float(size_w)
    offset_y2 = (y2 - ny2) / float(size_h)
    return offset_x1, offset_y1, offset_x2, offset_y2

def pts_offset_r(pts, nbox):
    out = []
    nx1, ny1, nx2, ny2 = nbox
    size_w = nx2-nx1
    size_h = ny2-ny1
    for x,y in pts:
        offset_x1 = (x - nx1) / float(size_w)
        offset_y1 = (y - ny1) / float(size_h)
        out.append(offset_x1)
        out.append(offset_y1)
    return out

def randbox2(img_w,img_h,box,k):
    x1, y1, x2, y2 = box
    if x1<0:
        return randbox(img_w, img_h, 20),(-1,-1,-1,-1)
    w = x2 - x1 + 1
    h = y2 - y1 + 1
    #size_w = npr.randint(int(min(w, h) * (1-k)), np.ceil((1+k) * max(w, h)))
    size_w = npr.randint(int(w * (1-k)), np.ceil(w * (1+k)))
    size_h = npr.randint(int(h * (1-k)), np.ceil(h * (1+k)))
    #print(size_w,size_h)
    # delta here is the offset of box center
    delta_x = npr.randint(-w * k, w * k)
    delta_y = npr.randint(-h * k, h * k)

    nx1 = int(max(x1 + w / 2 + delta_x - size_w / 2, 0))
    ny1 = int(max(y1 + h / 2 + delta_y - size_h / 2, 0))
    nx2 = int(min(nx1 + size_w, img_w))
    ny2 = int(min(ny1 + size_h, img_h))

    nbox = (int(nx1), int(ny1), int(nx2), int(ny2))
    return nbox


def IoU(box, boxes):
    """Compute IoU between detect box and gt boxes

    Parameters:
    ----------
    box: numpy array , shape (5, ): x1, y1, x2, y2, score
        input box
    boxes: numpy array, shape (n, 4): x1, y1, x2, y2
        input ground truth boxes

    Returns:
    -------
    ovr: numpy.array, shape (n, )
        IoU
    """
    box_area = (box[2] - box[0] + 1) * (box[3] - box[1] + 1)
    area = (boxes[:, 2] - boxes[:, 0] + 1) * (boxes[:, 3] - boxes[:, 1] + 1)
    xx1 = np.maximum(box[0], boxes[:, 0])
    yy1 = np.maximum(box[1], boxes[:, 1])
    xx2 = np.minimum(box[2], boxes[:, 2])
    yy2 = np.minimum(box[3], boxes[:, 3])

    # compute the width and height of the bounding box
    w = np.maximum(0, xx2 - xx1 + 1)
    h = np.maximum(0, yy2 - yy1 + 1)

    inter = w * h
    ovr = inter / (box_area + area - inter)
    return ovr

def convert_to_square(bbox):
    """Convert bbox to square

    Parameters:
    ----------
    bbox: numpy array , shape n x 5
        input bbox

    Returns:
    -------
    square bbox
    """
    square_bbox = bbox.copy()

    h = bbox[:, 3] - bbox[:, 1] + 1
    w = bbox[:, 2] - bbox[:, 0] + 1
    max_side = np.maximum(h,w)
    square_bbox[:, 0] = bbox[:, 0] + w*0.5 - max_side*0.5
    square_bbox[:, 1] = bbox[:, 1] + h*0.5 - max_side*0.5
    square_bbox[:, 2] = square_bbox[:, 0] + max_side - 1
    square_bbox[:, 3] = square_bbox[:, 1] + max_side - 1
    return square_bbox

import cv2


def imshow(name, img, rect=None,pts=None):
    """
    rect : x1,y1,x2,y2
    """
    img2 = img.copy()
    if rect is not None:
        rect = [int(x) for x in rect]
        x1,y1,x2,y2=rect
        cv2.rectangle(img2, (x1,y1),(x2,y2), (255, 0, 0), 1)


    font = cv2.FONT_HERSHEY_SIMPLEX
    color = (255,0,0)
    #print(pts)
    scale = 1
    for i in range(len(pts)):
        x,y = pts[i]
        x,y = (int(x*scale),int(y*scale))
        cv2.circle(img2,(x,y),5, color,-1)
        cv2.putText(img2, str(i), (x,y), font, 0.5, (255, 0, 255), 2)

    cv2.imshow(name, img2)
    c = cv2.waitKey(-1)
    cv2.destroyWindow(name)

# 定义旋转rotate函数
def rotate(image1, angle, scale, pts):
    (h, w) = image1.shape[:2]
    k = max(h,w)
    k = int(abs(math.sin(angle*3.14/180.))*k*0.5)
    #print(k,h,w)
    image = cv2.copyMakeBorder(image1,k,k,k,k,cv2.BORDER_REFLECT)
    # 获取图像尺寸
    (h, w) = image.shape[:2]
    # 若未指定旋转中心，则将图像中心设为旋转中心
    center = (w / 2, h / 2)

    # 执行旋转
    M = cv2.getRotationMatrix2D(center, angle, scale)
    #print(M)
    npt = len(pts)
    #print(pts,npt)
    t = np.array(pts, dtype=np.float32).reshape(-1, 2)+k
    t = np.row_stack( (np.transpose(t), np.ones((1,npt))) )
    #print(t)
    t = np.matmul(M,t)
    #k = 0
    t = np.transpose(t-k)
    t = t.tolist()
    #print(t)
    rotated = cv2.warpAffine(image, M, (w, h))

    # 返回旋转后的图像
    out = rotated[k:(h-k), k:(w-k)]
    return out,t

def add_shader(img):
    img3 = img.copy()
    img3 = rotate(img3, npr.randint(-10, 10))
    h,w = img3.shape
    k = 20
    #img3 = img3[randint(0,k):h-k,randint(0,k):w-k]
    h,w = img3.shape
    #print(h,w)
    g = 10
    k = 255
    r = 0.5+0.5*random.random()
    img3 = circle(img3,0.1,randi([0,0],[w,h]), randi([50],[255])[0], randi([g,g,g],[k,k,k]))
    img3 = circle(img3, 0.1,randi([w*2/3,0],[w,h]), randi([10],[100])[0], randi([g,g,g],[k,k,k]))

    r = 3+2*int(random.random()*2)
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(r, r))
    if random.random()<0.5:  
        img3 = cv2.dilate(img3,kernel)
    else: 
        img3 = cv2.erode(img3,kernel)

    if random.random()>0.5:
        k = 1/(1+random.random())
    else:
        k = 1+random.random()
    img3 = np.power(img3/float(np.max(img3)), k)
    return img3


def save_rand_image(save_dir,img,box,boxes,type,min_iou,max_iou,idx,max_num,outsize,f1,pts2,label,cls_list):
    neg_num = 0
    avg = (min_iou+max_iou)*0.5
    height, width, channel = img.shape
    while neg_num < max_num:
        nbox = randbox2(width,height,box,1-avg)
        offset = box_offset_r(box, nbox)
        nx1,ny1,nx2,ny2 = nbox
        crop_box = np.array((nx1,ny1,nx2,ny2))

        Iou = IoU(crop_box, boxes)
        Iou = np.max(Iou)
        #print(crop_box,boxes,Iou)
        if Iou >= min_iou and Iou < max_iou and ny1<ny2 and nx1<nx2:
            cropped_im = img[ny1 : ny2, nx1 : nx2, :]
            resized_im = cv2.resize(cropped_im, outsize, interpolation=cv2.INTER_LINEAR)
            save_file = os.path.join(save_dir, "%s.jpg"%idx)
            offset_x1, offset_y1, offset_x2, offset_y2 = offset#box_offset_r(box, nbox)
            im       = (resized_im - 127.5)/127.5
            roi      = [offset_x1, offset_y1, offset_x2, offset_y2]
            pts      = [-1,-1,-1,-1,-1,-1]
            if pts2 is None:
                pts = [-1,-1,-1,-1,-1,-1]
            else:
                pts = pts_offset_r(pts2, nbox)
            cls_list.append([im,label,roi]) 
            f1.append('%s %d %.2f %.2f %.2f %.2f'%(save_file,type,offset_x1, offset_y1, offset_x2, offset_y2))
            cv2.imwrite(save_file, resized_im)
            print(save_file)
            #if idx<2: imshow('asdf', resized_im, (int(box[0]+offset_x1*outsize[0]), int(offset_y1*outsize[1]), int(offset_x2*outsize[0]), int(offset_y2*outsize[1])))
            idx += 1
            neg_num += 1
    return idx

def get_all_list(all_list):
    cls_list = []
    roi_list = []
    pts_list = []
    for im,label,pts in all_list:
        if label!=-1:
            cls_list.append([im,label,pts])
        if label!=0:
            roi_list.append([im,label,pts])
            pts_list.append([im,label,pts])
    return cls_list,roi_list,pts_list


import pickle

def pickle_save(fn,data):
    f = open(fn,'wb')
    pickle.dump(data, f)
    f.close()

def pickle_load(fn):
    f = open(fn,'rb')
    data = pickle.load(f)
    f.close()
    return data
