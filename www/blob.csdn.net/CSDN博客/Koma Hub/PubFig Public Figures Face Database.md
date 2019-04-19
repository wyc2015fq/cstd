# PubFig: Public Figures Face Database - Koma Hub - CSDN博客
2019年01月22日 22:00:55[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：54
个人分类：[数据集](https://blog.csdn.net/Rong_Toa/article/category/8634038)
![](https://img-blog.csdnimg.cn/20190122220008597.png)
## Introduction
The PubFig database is a large, real-world face dataset consisting of **58,797** images of **200** people collected from the internet. Unlike most other existing face datasets, these images are taken in completely uncontrolled situations with non-cooperative subjects. Thus, there is large variation in pose, lighting, expression, scene, camera, imaging conditions and parameters, etc. The PubFig dataset is similar in spirit to the [Labeled Faces in the Wild (LFW) dataset](http://vis-www.cs.umass.edu/lfw/) created at UMass-Amherst, although there are some significant differences in the two:
- LFW contains **13,233 images** of **5,749 people**, and is thus much broader than PubFig. However, it's also smaller and much shallower (many fewer images per person on average).
- LFW is derived from the [Names and Faces in the News](http://www.cs.columbia.edu/~aberg/faces.html) work of T. Berg, et al. These images were originally collected using news sources online. For many people, there are often several images taken at the same event, with the person wearing similar clothing and in the same environment. [Our paper at ICCV 2009](http://www.cs.columbia.edu/CAVE/projects/faceverification/) showed that this can often be exploited by algorithms to give unrealistics boosts in performance.
- Of course, the PubFig dataset no doubt has biases of its own, and we welcome any attempts to categorize these.
We have created a face verification benchmark on this dataset that test the abilities of algorithms to classify a pair of images as being of the same person or not. Importantly, these two people should have **never** been seen by the algorithm during training. In the future, we hope to create recognition benchmarks as well.
## Citation
**The database is made available only for non-commercial use. If you use this dataset, please cite the following paper:**
> 
**"Attribute and Simile Classifiers for Face Verification,"**
Neeraj Kumar, Alexander C. Berg, Peter N. Belhumeur, and Shree K. Nayar,
*International Conference on Computer Vision (ICCV), 2009.*
[bibtex] [[pdf](http://www.cs.columbia.edu/CAVE/publications/pdfs/Kumar_ICCV09.pdf)] [[webpage](http://www.cs.columbia.edu/CAVE/projects/faceverification/)]
## News
- December 23, 2010: Updated PubFig to v1.2. The changes are as follows:
	- We added md5 checksums for all images in the datafiles on the [download](http://www.cs.columbia.edu/CAVE/databases/pubfig/download/) page.
- September 10, 2010: Updated PubFig to v1.1. The major changes are as follows:
	- We recomputed attribute values using updated classifiers, expanding to 73 attributes.
- Attribute values now exist for the development set as well as the evaluation set (previously only the evaluation set had attribute values).
- We updated the face rectangles for faces to be much tighter around the face, as opposed to the rather loose boundaries given before.
- We removed 679 bad images, including non-jpegs, images with non-standard colorspaces, corrupted images, and images with very poor alignment.
- We generated a new cross-validation set, taking into account these deleted images. We ran our algorithm with our new attribute classifiers on this set, obtaining a new curve.
- We removed the verification subsets by pose, lighting, and expression, as they were not being used. Instead, we created a [single datafile](http://www.cs.columbia.edu/CAVE/databases/pubfig/download/#pubfig_labels) which contains the manual labels for these parameters.
- Some of the datafile formats have changed slightly, to be more consistent with the others.
- We added the python script used to generate the output ROC curves
- We updated this website to be cleaner and easier to read
- December 21, 2009: Added face locations to dataset
- December 2, 2009: Created website and publicly released v1.0 of dataset
## Related Projects
- [Attribute and Simile Classifiers for Face Verification](http://www.cs.columbia.edu/CAVE/projects/faceverification/) (Columbia)
- [FaceTracer: A Search Engine for Large Collections of Images with Faces](http://www.cs.columbia.edu/CAVE/projects/face_search/) (Columbia)
- [Labeled Faces in the Wild](http://vis-www.cs.umass.edu/lfw/) (UMass-Amherst)
- [Names and Faces](http://www.cs.columbia.edu/~aberg/faces.html) (SUNY-Stonybrook)
