# 端到端语音识别（二） ctc - xmdxcsj的专栏 - CSDN博客





2017年04月20日 19:50:11[xmucas](https://me.csdn.net/xmdxcsj)阅读数：6457








## 相关笔记

[CTC学习笔记（一） 简介](http://blog.csdn.net/xmdxcsj/article/details/51763868)
[CTC学习笔记（二） 训练和公式推导](http://blog.csdn.net/xmdxcsj/article/details/51763886)
[CTC学习笔记（三） 解码](http://blog.csdn.net/xmdxcsj/article/details/51807600)
[CTC学习笔记（四） 解码-WFST](http://blog.csdn.net/xmdxcsj/article/details/51824214)
[CTC学习笔记（五） eesen训练源码](http://blog.csdn.net/xmdxcsj/article/details/53326058)
## History

ICML-2006. Graves et al. [1] introduced the connectionist temporal classification (CTC) objective function for phone recognition. 

ICML-2014. Graves [2] demonstrated that character-level speech transcription can be performed by a recurrent neural network with minimal preprocessing. 

 Baidu. 2014 [3] DeepSpeech, 2015 [4] DeepSpeech2. 

ASRU-2015. YaJie Miao [5] presented Eesen framework. 

ASRU-2015. Google [6] extended the application of Context-Dependent (CD) LSTM trained with CTC and sMBR loss. 

ICASSP-2016. Google [7] presented a compact large vocabulary speech recognition system that can run efficiently on mobile devices, accurately and with low latency. 

NIPS-2016. Google [8] used whole words as acoustic units. 

2017, IBM [9] employed direct acoustics-to-word models.
## Reference

[1]. A. Graves, S. Fernandez, F. Gomez, and J. Schmidhuber. Connectionist temporal classfification: labelling unsegmented sequence data with recurrent neural networks. In ICML, 2006. 

[2]. Graves, Alex and Jaitly, Navdeep. Towards end-to-end speech recognition with recurrent neural networks. In Proceedings of the 31st International Conference on Machine Learning (ICML-14), pp. 1764–1772, 2014. 

[3]. Hannun, A., Case, C., Casper, J., Catanzaro, B., Diamos, G.,Elsen, E., Prenger, R., Satheesh, S., Sengupta, S., Coates,A., et al. (2014a).Deepspeech: Scaling up end-to-end speech recognition. arXiv preprint arXiv:1412.5567. 

[4]. D. Amodei, R. Anubhai, E. Battenberg, C. Case, J. Casper, B. Catanzaro, J. Chen, M. Chrzanowski, A. Coates, G. Diamos et al., “Deep speech 2: End-to-end speech recognition in english and mandarin,” CoRR arXiv:1512.02595, 2015. 

[5]. Yajie Miao, Mohammad Gowayyed, Florian Metze. EESEN: End-to-End Speech Recognition using Deep RNN Models and WFST-based Decoding. 2015 Automatic Speech Recognition and Understanding Workshop (ASRU 2015)  

[6]. A. Senior, H. Sak, F. de Chaumont Quitry, T. N. Sainath, and K. Rao, “Acoustic Modelling with CD-CTC-SMBR LSTM RNNS,” in ASRU, 2015 

[7]. I. McGraw, R. Prabhavalkar, R. Alvarez, M. Gonzalez Arenas, K. Rao, D. Rybach, O. Alsharif, H. Sak, A. Gruenstein, F. Beaufays, and C. Parada, “Personalized speech recognition on mobile devices,” in Proc. of ICASSP, 2016. 

[8]. H. Soltau, H. Liao, and H. Sak, “Neural speech recognizer: Acoustic-to-word lstm model for large vocabulary speech recognition,” arXiv preprint arXiv:1610.09975,2016. 

[9]. K. Audhkhasi, B. Ramabhadran, G. Saon, M. Picheny, D. Nahamoo, “Direct Acoustics-to-Word Models for English Conversational Speech Recognition” arXiv preprint arXiv:1703.07754,2017.





