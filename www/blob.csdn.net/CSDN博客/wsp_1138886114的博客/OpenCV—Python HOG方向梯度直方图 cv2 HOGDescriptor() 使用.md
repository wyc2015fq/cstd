# OpenCV—Python HOG方向梯度直方图 cv2.HOGDescriptor() 使用 - wsp_1138886114的博客 - CSDN博客





2018年10月08日 10:33:29[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：2053








```python
```python
def createTrainingInstances(self, images):
    start = time.time()
    hog = cv2.HOGDescriptor()
    instances = []
    for img, label in images:
        img = read_color_image(img)   #函数未知
        img = cv2.resize(img, (128, 128), interpolation = cv2.INTER_AREA)
        descriptor = hog.compute(img)
        if descriptor is None:
            descriptor = []
        else:
            descriptor = descriptor.ravel()
        pairing = Instance(descriptor, label)
        instances.append(pairing)
    end = time.time() - start
    self.training_instances = instances
    print "HOG TRAIN SERIAL: %d images -> %f" % (len(images), end)
```
```

[https://github.com/achintyagopal/PaintingToArtists/blob/master/P2AParallel/HOG.py](https://github.com/achintyagopal/PaintingToArtists/blob/master/P2AParallel/HOG.py)

计算HOG特征模块

```python
```python
def hog_compute(ims):
    samples=[]
    winSize = (64,64)
    blockSize = (16,16)
    blockStride = (8,8)
    cellSize = (8,8)
    nbins = 9
    derivAperture = 1
    winSigma = 4.
    histogramNormType = 0
    L2HysThreshold = 2.0000000000000001e-01
    gammaCorrection = 0
    nlevels = 64
    hog = cv2.HOGDescriptor(winSize,blockSize,blockStride,cellSize,nbins,derivAperture,winSigma,
                            histogramNormType,L2HysThreshold,gammaCorrection,nlevels)
    #compute(img[, winStride[, padding[, locations]]]) -> descriptors
    winStride = (8,8)
    padding = (8,8)
    locations = ((10,20),(30,30),(50,50),(70,70),(90,90),(110,110),(130,130),(150,150),(170,170),(190,190))
    for im in ims:
        hist = hog.compute(im,winStride,padding,locations)
        samples.append(hist)
    return np.float32(samples)
```
```

```python
```python
def get_hog(image):
    winSize = (image.shape[1], image.shape[0])      # winSize = (64,64)
    blockSize = (8,8)                               # blockSize = (16,16)
    blockStride = (8,8)
    cellSize = (8,8)
    nbins = 9
    derivAperture = 1
    winSigma = 4.
    histogramNormType = 0
    L2HysThreshold = 2.0000000000000001e-01
    gammaCorrection = 0
    nlevels = 64
    cv2.HOGDescriptor(winSize,blockSize,blockStride,
                  cellSize,nbins,derivAperture,
                  winSigma,histogramNormType,L2HysThreshold,
                  gammaCorrection,nlevels)
    #compute(img[, winStride[, padding[, locations]]]) -> descriptors
    winStride = (8,8)
    padding = (8,8)
    locations = [] # (10, 10)# ((10,20),)
    hist = hog.compute(image,winStride,padding,locations)
    return hist
```
```

```python
```python
def compute_hog(image, locations):
    hog = cv2.HOGDescriptor()
    winStride = (8, 8)
    padding = (8, 8)
    hist = hog.compute(image, winStride, padding, locations)
    return hist
```
```

```python
```python
def get_hog_object(window_dims):
    blockSize = (8,8)
    # blockSize = (16,16)
    blockStride = (8,8)
    cellSize = (8,8)
    nbins = 9
    derivAperture = 1
    winSigma = 4.
    histogramNormType = 0 # HOGDescriptor::L2Hys
    L2HysThreshold = 2.0000000000000001e-01
    gammaCorrection = 0
    nlevels = 64
    hog = cv2.HOGDescriptor(window_dims,blockSize,blockStride,cellSize,nbins,derivAperture,winSigma,
                            histogramNormType,L2HysThreshold,gammaCorrection,nlevels)
    return hog
```
```



