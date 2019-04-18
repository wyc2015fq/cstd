# 基于深度学习的CT图像肺结节自动检测技术六—模型预测 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 22:39:39[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1440
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)









```python
```python
#模型预测的相关功能

from chapter4 import get_unet
from chapter5 import get_3dnnnet, stack_2dcube_to_3darray, prepare_image_for_net3D, MEAN_PIXEL_VALUE
import glob
import cv2
import numpy as np
from scipy import ndimage
from scipy.ndimage.measurements import center_of_mass
from skimage import morphology

CHANNEL_COUNT = 1
_3DCNN_WEIGHTS = './model/3dcnn.hd5'
UNET_WEIGHTS = './model/unet.hd5'
THRESHOLD = 2
BATCH_SIZE = 1


# 获取unet预测结果的中心点坐标(x,y)
def unet_candidate_dicom(unet_result_path):
    centers = []
    image_t = cv2.imread(unet_result_path, cv2.IMREAD_GRAYSCALE)
    # Thresholding(阈值化)
    image_t[image_t < THRESHOLD] = 0
    image_t[image_t > 0] = 1
    # dilation（扩张）
    selem = morphology.disk(1)
    image_eroded = morphology.binary_dilation(image_t, selem=selem)
    label_im, nb_labels = ndimage.label(image_eroded)

    for i in range(1, nb_labels + 1):
        blob_i = np.where(label_im == i, 1, 0)
        mass = center_of_mass(blob_i)
        y_px = int(round(mass[0]))
        x_px = int(round(mass[1]))
        centers.append([y_px, x_px])
    return centers


# 数据输入网络之前先进行预处理
def prepare_image_for_net(img):
    img = img.astype(np.float)
    img /= 255.
    if len(img.shape) == 3:
        img = img.reshape(img.shape[-3], img.shape[-2], img.shape[-1])
    else:
        img = img.reshape(1, img.shape[-2], img.shape[-1], 1)
    return img


# unet模型的预测代码
def unet_predict(imagepath):
    model = get_unet()
    model.load_weights(UNET_WEIGHTS)
    # read png and ready for predict
    images = []
    img = cv2.imread(imagepath, cv2.IMREAD_GRAYSCALE)
    images.append(img)
    for index, img in enumerate(images):
        img = prepare_image_for_net(img)
        images[index] = img
    images3d = np.vstack(images)
    y_pred = model.predict(images3d, batch_size=BATCH_SIZE)
    print(len(y_pred))
    count = 0
    for y in y_pred:
        y *= 255.
        y = y.reshape((y.shape[0], y.shape[1])).astype(np.uint8)
        cv2.imwrite('./temp_dir/chapter6/unet_result.png', y)
        count += 1


# 3dcnn模型的预测代码
def _3dcnn_predict(imagepath):
    cube_image = stack_2dcube_to_3darray(imagepath, 4, 8, 32)
    img3d = prepare_image_for_net3D(cube_image, MEAN_PIXEL_VALUE)
    model = get_3dnnnet(load_weight_path='./model/3dcnn.hd5')
    result = model.predict(img3d, batch_size=BATCH_SIZE, verbose=1)
    print('3dcnn result: ', result)


if __name__ == "__main__":
    unet_predict('./data/chapter6/unet_input_img.png')
    centers = unet_candidate_dicom('./temp_dir/chapter6/unet_result.png')
    print('y, x', centers)
    _3dcnn_predict('./data/chapter6/true_positive_nodules.png')
    _3dcnn_predict('./data/chapter6/false_positive_nodules.png')
```
```



