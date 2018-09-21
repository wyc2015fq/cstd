import numpy as np
import cv2
from fileio import *
import math

maxnpts = 3


def near_pt(x,y,pts):
    id = -1
    dis = 10000000
    for i in range(len(pts)):
        x1, y1 = pts[i]
        d = math.sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1))
        if d<dis:
            id = i
            dis = d
    return id,dis

def draw_circle(event,x,y,flags,param):
    pts = param['pts']
    scale = param['scale']
    x,y = (x/scale,y/scale)
    id,dis = near_pt(x,y,pts)

    param['id'] = id
    param['dis'] = dis
    if event == cv2.EVENT_MOUSEMOVE and param['active_id']>=0:
        pts[param['active_id']] = (x,y)
        param['id'] = param['active_id']
        param['dis'] = 0
        pass
    if event == cv2.EVENT_MOUSEHWHEEL:
        print('EVENT_MOUSEHWHEEL', flags)
        pass
    if event == cv2.EVENT_LBUTTONUP:
        param['active_id'] = -1
    if event == cv2.EVENT_LBUTTONDOWN:
        print('EVENT_LBUTTONDOWN')
        #print(param[])
        if id>=0 and dis<(10/scale):
            pts[id] = (x,y)
            param['active_id'] = id
        else:
            if len(pts)<maxnpts:
                pts.append((x,y))


imgroot = 'E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/'

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

def save_data(fn, data):
    f = open(fn, 'w')
    for fn,pts in data:
        ss = ' '.join([str(x)+' '+ str(y) for x,y in pts])
        f.write('%s %s\n'%(fn,ss))
    f.close()

fn = 'E:/data/ew_id/list1.txt'
fn1 = 'E:/data/ew_id/list1.txt'

def clickpoint(name, data, i):
    fn = imgroot + data[i][0]
    pts = data[i][1]
    print(fn)
    img = cv2.imread(fn, cv2.IMREAD_COLOR)
    c = 0
    if img is None:
        img = np.zeros((512,512,3),np.uint8)
    cv2.namedWindow(name)
    rows,cols,_ = img.shape
    scale = 500./rows
    param = {'pts': pts, 'scale':scale,'id':-1,'dis':0,'active_id':-1}
    cv2.setMouseCallback(name,draw_circle,param)
    font = cv2.FONT_HERSHEY_SIMPLEX
    while(1):
        res=cv2.resize(img,(int(cols*scale),int(rows*scale)),interpolation=cv2.INTER_LINEAR)
        for i in range(len(pts)):
            x,y = pts[i]
            x,y = (int(x*scale),int(y*scale))
            color = (255,0,0) if param['id']==i and param['dis']<10 else (0,0,255)
            cv2.circle(res,(x,y),5, color,-1)
            cv2.putText(res, str(i), (x,y), font, 0.5, (255, 0, 255), 2)
        cv2.imshow('image', res)
        c = cv2.waitKey(30) & 0xFF
        if c==ord('s'):
            save_data(fn1, data)
            print('save_data')

        if c==ord('d') or c==ord('a') or c==27:
            break
    cv2.destroyWindow(name)
    return c

#print(li[1:10])
i = 0
data = load_data(fn)
n = len(data)
while(n>0):
    i = (i+n) % n
    c = clickpoint('image', data, i)
    if c==ord('d'):
        i = i+1
    if c==ord('a'):
        i = i-1
        print('pre')
    print(c)
    if c == 27:
        break

save_data(fn1,data)
 
# 关闭文件
cv2.destroyAllWindows()


