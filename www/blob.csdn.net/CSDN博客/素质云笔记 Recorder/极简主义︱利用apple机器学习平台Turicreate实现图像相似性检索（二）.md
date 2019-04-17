# 极简主义︱利用apple机器学习平台Turicreate实现图像相似性检索（二） - 素质云笔记/Recorder... - CSDN博客





2017年12月19日 19:25:04[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4298








apple开源机器学习框架turicreate内容非常广阔，本篇介绍冰山一角的图像相似，极简主义的代表！！！！ 

github:[https://apple.github.io/turicreate/docs/api/generated/turicreate.SFrame.html](https://apple.github.io/turicreate/docs/api/generated/turicreate.SFrame.html)

首篇博客：[python︱apple开源机器学习框架turicreate中的SFrame——新形态pd.DataFrame](http://blog.csdn.net/sinat_26917383/article/details/78805714)

原理就是利用resnet-50，提取图像特征并保存至model之中，然后可以任意查询。
- 1、数据导入环节
- 2、利用Resnet50数据特征提取
- 3、相似搜索——query查询
- 4、相似关联图搜索——similarity_graph

.

# 一、数据导入环节

专门针对图像的load_images函数，让内容读成dataframe，同时，保存了图像的长、宽信息

```python
import turicreate as tc
# Load images from the downloaded data
reference_data  = tc.image_analysis.load_images('./101_ObjectCategories')
```

数据是这样的： 
![这里写图片描述](https://img-blog.csdn.net/20171219191022654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这个数据带有图片链接地址 + 图片的长宽信息。
主函数load_images：

```
load_images(url, format='auto', with_path=True, recursive=True, ignore_failure=True, random_order=False)
```

其中，
- url : str 

       The string of the path where all the images are stored.
- format : {‘PNG’ | ‘JPG’ | ‘auto’}, optional 

       The format of the images in the directory. The default ‘auto’ parameter 

       value tries to infer the image type from the file extension. If a 

       format is specified, all images must be of that format.- with_path : bool, optional 

       Indicates whether a path column is added to the SFrame. If ‘with_path’ 

       is set to True,  the returned SFrame contains a ‘path’ column, which 

       holds a path string for each Image object.- 
recursive : bool, optional 

       Indicates whether ‘load_images’ should do recursive directory traversal, 

       or a flat directory traversal.- 
ignore_failure : bool, optional 

       If true, prints warning for failed images and keep loading the rest of 

       the images.- random_order : bool, optional 

       Load images in random order.

几个案例：

```
>>> url ='https://static.turi.com/datasets/images/nested'
    >>> image_sarray = turicreate.image_analysis.load_images(url, "auto", with_path=False,
    ...                                                    recursive=True)
```

还有快速resize图片：

```
Resize a single image
    >>> img = turicreate.Image('https://static.turi.com/datasets/images/sample.jpg')
    >>> resized_img = turicreate.image_analysis.resize(img,100,100,1)
    Resize an SArray of images
    >>> url ='https://static.turi.com/datasets/images/nested'
    >>> image_sframe = turicreate.image_analysis.load_images(url, "auto", with_path=False,
    ...                                                    recursive=True)
    >>> image_sarray = image_sframe["image"]
    >>> resized_images = turicreate.image_analysis.resize(image_sarray, 100, 100, 1)
```

.

# 二、利用Resnet50数据特征提取

主函数tc.image_similarity.create
`create(dataset, label = None, feature = None, model = 'resnet-50', verbose = True)`
model默认使用resnet50,label是标识行，如reference_data中的path

常规的执行为：
`model = tc.image_similarity.create(reference_data)`
那么model就是整个模型文件了。 

模型的输出与保存：

```bash
model.save('my_model_file')
loaded_model = tc.load_model('my_model_file')
```

模型的一些基本属性：

```
model.name()
model.summary()
```

.

# 三、相似搜索——query查询

```
query(dataset, label=None, k=5, radius=None, verbose=True)
    - dataset:SFrame,需查询的数据
    - label:索引名，选择一列，输入列名
    - k:最大返回样本数
    - radius:半径范围，如果超过radius个间隔，就不显示
    - verbose:打印进度更新和模型细节。
```

例子：

```
#Querying the model
similar_images = model.query(reference_data[0:10] ,label = 'path',k=10)
similar_images.head()
```

查询reference_data前十张图，前K=10个相似图。 

similar_images为一个SFrame，一共100行。 

label表示similar_images这个dataframe的索引是之前reference_data表中的’path’这一列。 
![这里写图片描述](https://img-blog.csdn.net/20171219191456828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
.

# 四、相似关联图搜索——similarity_graph

这个是Turicreate的核心功能，很逆天。简单介绍一下，后续有专题博客。 

主函数：

```
similarity_graph(k=5, radius=None, include_self_edges=False, output_type='SGraph', verbose=True)
    k:最大返回样本数
    radius:float,半径范围，如果超过radius个间隔，就不显示
    include_self_edges:是否包含自己,True的话，返回的结果包含自己（自己的相似性为1）
    output_type:‘SGraph’, ‘SFrame’两种
    verbose:打印进度更新和模型细节。
```

一般例子：
`graph = model.similarity_graph(k=1)  # an SGraph`
返回一个SGraph格式。会有一个构建图的过程，这里YY一张图： 
![这里写图片描述](https://github.com/apple/turicreate/raw/master/userguide/nearest_neighbors/images/boston_sim_graph.png)

来看看一个简单的结果示意：`graph.edges`
![这里写图片描述](https://img-blog.csdn.net/20171219192536359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)













