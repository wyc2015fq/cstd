# Caffe用训练好的模型测试图片 - AI之路 - CSDN博客





2017年08月23日 08:03:37[AI之路](https://me.csdn.net/u014380165)阅读数：5737








这是一个python脚本，用训练好的caffemodel来测试图片，接下来直接上代码，里面有详细解释，大部分你要修改的只是路径，另外在这个脚本的基础上你可以根据自己的需要进行改动。

需要的东西：**训练好的caffemodel，deploy.prototxt（可以从你的train.prototxt修改得到），可以用的caffe，待测试的图像（比如jpg）**

```python
import sys
caffe_root='/your/caffe/root/' #修改成你的Caffe项目路径
sys.path.append(caffe_root+'python')
import caffe
caffe.set_mode_gpu() #设置为GPU运行
from pylab import *

# 修改成你的deploy.prototxt文件路径
model_def = 'your/deploy.prototxt/path' 
model_weights = '/your/caffemodel/path' # 修改成你的caffemodel文件的路径

net = caffe.Net(model_def,      # defines the structure of the model
                model_weights,  # contains the trained weights
                caffe.TEST)     # use test mode (e.g., don't perform dropout)

#这是一个由mean.binaryproto文件生成mean.npy文件的函数
def convert_mean(binMean,npyMean):
    blob = caffe.proto.caffe_pb2.BlobProto()
    bin_mean = open(binMean, 'rb' ).read()
    blob.ParseFromString(bin_mean)
    arr = np.array( caffe.io.blobproto_to_array(blob) )
    npy_mean = arr[0]
    np.save(npyMean, npy_mean )
binMean='your/mean.binaryproto/path' #修改成你的mean.binaryproto文件的路径
npyMean='which/path/you/want/to/save/mean.npy' #你想把生成的mean.npy文件放在哪个路径下
convert_mean(binMean,npyMean)

transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
transformer.set_transpose('data', (2,0,1))  # 通道变换，例如从(530,800,3) 变成 (3,530,800)
transformer.set_mean('data', np.load(npyMean).mean(1).mean(1)) #如果你在训练模型的时候没有对输入做mean操作，那么这边也不需要
transformer.set_raw_scale('data', 255)  # rescale from [0, 1] to [0, 255]
transformer.set_channel_swap('data', (2, 1, 0))  # swap channels from RGB to BGR

with open('your/txt file/path') as image_list: # 修改成你要测试的txt文件的路径，这个txt文件的内容一般是：每行表示图像的路径，然后空格，然后是标签，也就是说每行都是两列
    with open('predict/result/you/want/to/save','w') as result: # 如果你想把预测的结果写到一个txt文件中，那么把这个路径修改成你想保存这个txt文件的路径
        count_right=0
        count_all=0
        while 1:
            list_name=image_list.readline()
            if list_name == '\n' or list_name == '': #如果txt文件都读完了则跳出循环
                break
            image_type=list_name[0:-3].split('.')[-1]
            if image_type == 'gif': #这里我对gif个数的图像直接跳过
                continue
            image = caffe.io.load_image('/your/image/path/'+list_name) 
            # 这里要添加你的图像所在的路径，根据你的list_name灵活调整，总之就是图像路径
            #imshow(image)
            transformed_image = transformer.preprocess('data', image)

            # 用转换后的图像代替net.blob中的data
            net.blobs['data'].data[...] = transformed_image
            net.blobs['data'].reshape(1, 3, 224, 224)
            ### perform classification
            output = net.forward()

        # 读取预测结果和真实label
            output_prob = net.blobs['prob'].data[0]
            true_label = int(list_name[-2:-1])
    # 如果预测结果和真实label一样，则count_right+1
            if(output_prob.argmax()==true_label):
                count_right=count_right+1
            count_all=count_all+1

    # 保存预测结果，这个可选
            result.writelines(list_name[0:-1]+' '+str(output_prob.argmax())+'\n')
    #可以每预测完100个样本就打印一些，这样好知道预测的进度，尤其是要预测几万或更多样本的时候，否则你还以为代码卡死了
            if(count_all%100==0):
                print count_all

       # 打印总的预测结果
        print 'Accuracy: '+ str(float(count_right)/float(count_all))
        print 'count_all: ' + str(count_all)
        print 'count_right: ' + str(count_right)
        print 'count_wrong: ' + str(count_all-count_right)
```



