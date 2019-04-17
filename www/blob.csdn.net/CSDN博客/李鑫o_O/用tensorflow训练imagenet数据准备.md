# 用tensorflow训练imagenet数据准备 - 李鑫o_O - CSDN博客





2017年08月02日 17:07:59[hustlx](https://me.csdn.net/HUSTLX)阅读数：8770








                
在tensorflow的models/models/inception/inception/data中有数据处理的一些脚本，但是实在不好用，然而将图像转化为TFRecords的一些python脚本还是能用，首先工作目录为：models/inception/inception/data   将imagenet的原始数据ILSVRC2012_img_train.tar和ILSVRC2012_img_val.tar放在这个目录下，然后执行如下脚本：
```
mkdir -p ILSVRC2012
mkdir -p ILSVRC2012/raw-data
mkdir -p ILSVRC2012/raw-data/imagenet-data
mkdir -p ILSVRC2012/raw-data/imagenet-data/bounding_boxes
mv ILSVRC2012_bbox_train_v2.tar.gz ILSVRC2012/raw-data/imagenet-data/bounding_boxes/
tar xvf ILSVRC2012/raw-data/imagenet-data/bounding_boxes/ILSVRC2012_bbox_train_v2.tar.gz -C ILSVRC2012/raw-data/imagenet-data/bounding_boxes/
NUM_XML=$(ls -1 ILSVRC2012/raw-data/imagenet-data/bounding_boxes/* | wc -l)
echo "Identified ${NUM_XML} bounding box annotations."
mkdir -p ILSVRC2012/raw-data/imagenet-data/validation/
tar xf ILSVRC2012_img_val.tar -C ILSVRC2012/raw-data/imagenet-data/validation/
mkdir -p ILSVRC2012/raw-data/imagenet-data/train/
mv ILSVRC2012_img_train.tar ILSVRC2012/raw-data/imagenet-data/train/ && cd ILSVRC2012/raw-data/imagenet-data/train/
tar -xvf ILSVRC2012_img_train.tar && rm -f ILSVRC2012_img_train.tar 
find . -name "*.tar" | while read NAE ; do mkdir -p "${NAE%.tar}"; tar -xvf "${NAE}" -C "${NAE%.tar}"; rm -f "${NAE}"; done 
cd .. && cd .. && cd .. && cd ..

python preprocess_imagenet_validation_data.py ILSVRC2012/raw-data/imagenet-data/validation/ imagenet_2012_validation_synset_labels.txt
#这一步应将里面的JPEG改为jpeg
python process_bounding_boxes.py ILSVRC2012/raw-data/imagenet-data/bounding_boxes/ imagenet_lsvrc_2015_synsets.txt | sort > ILSVRC2012/raw-data/imagenet_2012_bounding_boxes.csv

python build_imagenet_data.py --train_directory=ILSVRC2012/raw-data/imagenet-data/train/ --validation_directory=ILSVRC2012/raw-data/imagenet-data/validation/ --output_directory=ILSVRC2012/ --imagenet_metadata_file=imagenet_metadata.txt --labels_file=imagenet_lsvrc_2015_synsets.txt --bounding_box_file=ILSVRC2012/raw-data/imagenet_2012_bounding_boxes.csv
```
`#最后一步需要将build_imagenet_data.py里面的JPEG换成jpeg`



