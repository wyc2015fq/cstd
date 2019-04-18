# MaskRCNN路标：TensorFlow版本用于抠图 - wishchinYang的专栏 - CSDN博客
2017年12月12日 17:16:02[wishchin](https://me.csdn.net/wishchin)阅读数：4187
            MaskRCNN用于检测路标，作为更详细的目标检测，用以得到更精准的额路标位置，路标的几何中心点，用于构建更为精准的拓扑地图，减少构图误差。
            抠图工具已经完成，把框抠出来，用0值表示背景。
python代码：
```python
def mainex():
    #initDir();
    # Root directory of the project
    ROOT_DIR = os.getcwd()
    # Directory to save logs and trained model
    MODEL_DIR = os.path.join(ROOT_DIR, "logs")
    # Path to trained weights file
    # Download this file and place in the root of your
    # project (See README file for details)
    #COCO_MODEL_PATH = os.path.join(ROOT_DIR, "mask_rcnn_coco.h5")
    COCO_MODEL_PATH= "D:/Works/PyProj/MaskRCNN-tensor/data/model/mask_rcnn_coco.h5";
    # Directory of images to run detection on
    #IMAGE_DIR = os.path.join(ROOT_DIR, "images");
    IMAGE_DIR = "data/MedSeaTest/";
    config = InferenceConfig()
    config.display();
    # 3.
    # Create model object in inference mode.
    model = modellib.MaskRCNN(mode="inference", model_dir=MODEL_DIR, config=config)
    # Load weights trained on MS-COCO
    model.load_weights(COCO_MODEL_PATH, by_name=True);
    # 4
    class_names= init_classname();
    IMAGE_DIR = "D:/DataSet/PicStyleTest/Medsea3/deskfilter/";
    proDir(model, class_names, IMAGE_DIR);
```
处理目录：
```python
def proDir( model,class_names,IMAGE_DIR ):
    # Load a random image from the images folder
    print(IMAGE_DIR);
    
    extention =".jpg";
    filelist =traverseFolder( IMAGE_DIR , extention);#traverse( IMAGE_DIR , extention);#
    for file in filelist:
        print("Is processing: ");print(file);
        image = skimage.io.imread( file );
        # Run detection
        results = model.detect([image], verbose=1);
        # Visualize results
        #r = results[0];
        fileName = file;
        
        getAllLabelMask(fileName, image, results[0], class_names)
```
```python
def getAllLabelMask(fileName,image, maskResult,class_names ):
    """
    boxes: [num_instance, (y1, x1, y2, x2, class_id)] in image coordinates.
    masks: [num_instances, height, width]
    class_ids: [num_instances]
    class_names: list of class names of the dataset
    scores: (optional) confidence scores for each box
    figsize: (optional) the size of the image.
    """
    boxes  = maskResult['rois'];
    masks  = maskResult['masks']; 
    scores = maskResult['scores'];
    class_ids = maskResult['class_ids'];
    
    # Number of instances
    N = boxes.shape[0];
    if not( N<1 or boxes.shape[0] == masks.shape[-1] == class_ids.shape[0]):
        return
    row = image.shape[1];
    col = image.shape[0];
    for i in range(N):
        # Bounding box
        if not np.any(boxes[i]): 
            continue;
        y1, x1, y2, x2 = boxes[i];
        # Label
        class_id = class_ids[i];
        score = scores[i] if scores is not None else None
        label = class_names[class_id];
        
        # Mask
        mask = masks[:, :, i];
        masked_image = np.zeros((col, row, 3), dtype=np.uint8);
        masked_image = apply_maskX(masked_image, mask);
        
        #frontImage = np.zeros( (col, row), dtype=np.uint8 );
        frontImage = image.copy();
        for m in range(row):
            for n in range(col):
                if(masked_image[n, m, 0]<254):
                    #frontImage[n, m] = 255;
                    frontImage[n,m,0] =0;
                    frontImage[n, m, 1] = 0;
                    frontImage[n, m, 2] = 0;
        #roiMask = masked_image[y1:y2, x1:x2];
        roiImg = frontImage[y1:y2, x1:x2];
        roiImg = cv2.cvtColor(roiImg, cv2.COLOR_BGR2RGB);
        fileMask = fileName[0: len(fileName)-4];
        fileMask = fileMask +"."+ str(i)+"."+label+"."+"Mask.png";
        cv2.imwrite(fileMask, roiImg);
```
结果：
![](https://img-blog.csdn.net/20171212171341517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

