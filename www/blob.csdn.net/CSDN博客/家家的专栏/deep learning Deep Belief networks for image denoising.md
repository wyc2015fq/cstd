# deep learning Deep Belief networks for image denoising - 家家的专栏 - CSDN博客





2014年06月17日 16:26:27[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3610








http://arxiv.org/pdf/1312.6158v2.pdf


Deep belief network (DBN), 

          hierarchical generative models  for feature representation

          Features (extracted by DBNs) are presented as the values of the last layer nodes. 




Image denoise:

In the last later of DBNs, Distinguish between nodes presents noise and nodes presenting image content. 

        The nodes in the last layer are divided in two distinct groups of nodes. 

Detect nodes, then make the noise nodes inactive and reconstruct a noise-less image. 

        Use a criterion called** relative activity** to **detect**noise nodes.




**Relative activity of each nod**e is defined as the difference between two values of a particular node resulted from feeding the network using a noiseless image and its corresponding noisy image. 

![](https://img-blog.csdn.net/20140617162548781)





传统的一些降噪方法：

         Fourier analysis, Spatial filtering, Wavelet transform, Sparse coding and dictionary learning, 




与DL相关的，另外两篇关于降噪的paper：

Image denoising and inpainting with deep neural networks

Convolutional networks and applications in vision





