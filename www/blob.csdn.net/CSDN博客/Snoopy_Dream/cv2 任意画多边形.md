# cv2 任意画多边形 - Snoopy_Dream - CSDN博客





2018年08月06日 12:05:13[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：283








```
for result in results:
    f = open(result,'r')
    lines = f.readlines()
    boxes = []
    for line in lines:
        score = float(line.strip().split(' ')[8])
        box,name=line.strip().split(' ')[0:8],line.strip().split(' ')[9]
        box = np.array(box,dtype = np.float)#！！np.float
#!!经常提示 box = np.array(box,dtype = np.int32)

#ValueError: invalid literal for long() with base 10: '5185.0'


        if float(score)>thr:
            boxes.append(box)
    boxes = np.array(boxes)
    f.close()   
    filepath=pic_path + result.split('/')[-1].split('.')[0]+'.png'
    im=cv2.imread(filepath)
    #print line3
    for i in range(boxes.shape[0]):
        box =np.array( [[boxes[i][0],boxes[i][1]],[boxes[i][2],boxes[i][3]], \
                      [boxes[i][4],boxes[i][5]],[boxes[i][6],boxes[i][7]]],np.int32)
        box = box.reshape((-1,1,2))
        cv2.polylines(im,[box],True,(0,255,255),1)##[box]！！！
    #cv2.rectangle(im,(int(float(x)),int(float(y))),(int(float(x))+int(float(w)),int(float(y))+int(float(h))),(0,0,255),1)
    cv2.imwrite(newpic_path+result.split('/')[-1].split('.')[0]+'.png',im)
```





