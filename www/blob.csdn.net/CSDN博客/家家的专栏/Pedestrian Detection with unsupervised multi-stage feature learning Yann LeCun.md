# Pedestrian Detection with unsupervised multi-stage feature learning Yann LeCun - 家家的专栏 - CSDN博客





2014年07月24日 21:30:03[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1909








Pedestrian detection + a convolutional network model （with a few twists）.

1、**Unsupervised convolutional sparse auto-encoders** to pre-train features at all levels from the relatively small INNRIA dataset. 

2、**End-to-end supervised training** to **train **the classifier and
**fine-tune** the features in an intregrated fashion. 

3、**Multi-stage features** with layer-skipping connections enable output stages to combine
**global **shape detectors with **local **motif detectors.









