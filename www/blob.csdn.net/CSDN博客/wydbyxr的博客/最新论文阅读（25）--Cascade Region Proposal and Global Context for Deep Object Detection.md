# 最新论文阅读（25）--Cascade Region Proposal and Global Context for Deep Object Detection - wydbyxr的博客 - CSDN博客
2018年06月09日 19:06:14[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：213
# Cascade Region Proposal and Global Context for Deep Object Detection
```
- 2017年10月  
- 改进了faster-RCNN    
- 杭州海康威视研究院
```
　　网络结构就是Faster R-CNN+级联RPN（即两个RPN和两个ROIpooling）。 
　　另外，还有些训练技巧,细节见论文。
![这里写图片描述](https://img-blog.csdn.net/20180609190428550?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 两个RPN
![这里写图片描述](https://img-blog.csdn.net/20180609190508245?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 两个ROIpooling
![这里写图片描述](https://img-blog.csdn.net/20180609190522641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
