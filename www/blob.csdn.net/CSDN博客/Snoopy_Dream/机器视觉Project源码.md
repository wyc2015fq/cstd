# 机器视觉Project源码 - Snoopy_Dream - CSDN博客





2018年03月14日 15:54:32[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：170








申明：本文非笔者原创，原文转载自：http://www.ihalcon.com/read-3733.html




一、特征提取Feature Extraction：
- SIFT [1] [[Demo program](http://www.cs.ubc.ca/~lowe/keypoints/siftDemoV4.zip)][[SIFT Library](http://blogs.oregonstate.edu/hess/code/sift/)] [[VLFeat](http://www.vlfeat.org/)]
- PCA-SIFT [2] [[Project](http://www.cs.cmu.edu/~yke/pcasift/)]
- Affine-SIFT [3] [[Project](http://www.ipol.im/pub/algo/my_affine_sift/)]
- SURF [4] [[OpenSURF](http://www.chrisevansdev.com/computer-vision-opensurf.html)] [[Matlab Wrapper](http://www.maths.lth.se/matematiklth/personal/petter/surfmex.php)]
- Affine Covariant Features [5] [[Oxford project](http://www.robots.ox.ac.uk/~vgg/research/affine/)]
- MSER [6] [[Oxford project](http://www.robots.ox.ac.uk/~vgg/research/affine/)] [[VLFeat](http://www.vlfeat.org/)]
- Geometric Blur [7] [[Code](http://www.robots.ox.ac.uk/~vgg/software/MKL/)]
- Local Self-Similarity Descriptor [8] [[Oxford implementation](http://www.robots.ox.ac.uk/~vgg/software/SelfSimilarity/)]
- Global and Efficient Self-Similarity [9] [[Code](http://www.vision.ee.ethz.ch/~calvin/gss/selfsim_release1.0.tgz)]
- Histogram of Oriented Graidents [10] [[INRIA Object Localization Toolkit](http://www.navneetdalal.com/software)] [[OLT toolkit for Windows](http://www.computing.edu.au/~12482661/hog.html)]
- GIST [11] [[Project](http://people.csail.mit.edu/torralba/code/spatialenvelope/)]
- Shape Context [12] [[Project](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/shape/sc_digits.html)]
- Color Descriptor [13] [[Project](http://koen.me/research/colordescriptors/)]
- Pyramids of Histograms of Oriented Gradients [[Code](http://www.robots.ox.ac.uk/~vgg/research/caltech/phog/phog.zip)]
- Space-Time Interest Points (STIP) [14][[Project](http://www.nada.kth.se/cvap/abstracts/cvap284.html)] [[Code](http://www.irisa.fr/vista/Equipe/People/Laptev/download/stip-1.1-winlinux.zip)]
- Boundary Preserving Dense Local Regions [15][[Project](http://vision.cs.utexas.edu/projects/bplr/bplr.html)]
- Weighted Histogram[[Code](http://www.wisdom.weizmann.ac.il/~bagon/matlab_code/whistc.tar.gz)]
- Histogram-based Interest Points Detectors[[Paper](http://www.cs.nthu.edu.tw/~htchen/hipd_cvpr09.pdf)][[Code](http://740-2.cs.nthu.edu.tw/~htchen/hipd/hist_corner.zip)]
- An OpenCV - C++ implementation of Local Self Similarity Descriptors [[Project](http://intuitionlogic.com/post/2011/04/11/A-OpenCV-C++-implementation-of-Local-Self-Similarity-Descriptors.aspx)]
- Fast Sparse Representation with Prototypes[[Project](http://faculty.ucmerced.edu/mhyang/cvpr10_fsr.html)]
- Corner Detection [[Project](http://kiwi.cs.dal.ca/~dparks/CornerDetection/index.htm)]
- AGAST Corner Detector: faster than FAST and even FAST-ER[[Project](http://www6.in.tum.de/Main/ResearchAgast)]
- Real-time Facial Feature Detection using Conditional Regression Forests[[Project](http://files.is.tue.mpg.de/jgall/projects/facialfeatures/facialfeatures.html)]
- Global and Efficient Self-Similarity for Object Classification and Detection[[code](http://groups.inf.ed.ac.uk/calvin/gss/selfsim_release1.0.tgz)]
- WαSH: Weighted α-Shapes for Local Feature Detection[[Project](http://image.ntua.gr/iva/research/wash/)]
- HOG[[Project](http://soc.fudan.edu.cn/vip/projects/gradproj/wiki/HOG%E4%BB%A3%E7%A0%81)]
- Online Selection of Discriminative Tracking Features[[Project](http://www.cs.ucla.edu/~roozbehm/cs7495/report.html)]

二、图像分割Image Segmentation：- Normalized Cut [1] [[Matlab code](http://www.cis.upenn.edu/~jshi/software/)]
- Gerg Mori’ Superpixel code [2] [[Matlab code](http://www.cs.sfu.ca/~mori/research/superpixels/)]
- Efficient Graph-based Image Segmentation [3] [[C++ code](http://people.cs.uchicago.edu/~pff/segment/)] [[Matlab wrapper](http://www.mathworks.com/matlabcentral/fileexchange/25866-efficient-graph-based-image-segmentation)]
- Mean-Shift Image Segmentation [4] [[EDISON C++ code](http://coewww.rutgers.edu/riul/research/code/EDISON/index.html)] [[Matlab wrapper](http://www.wisdom.weizmann.ac.il/~bagon/matlab_code/edison_matlab_interface.tar.gz)]
- OWT-UCM Hierarchical Segmentation [5] [[Resources](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/grouping/resources.html)]
- Turbepixels [6] [[Matlab code 32bit](http://www.cs.toronto.edu/~babalex/turbopixels_code.tar.gz)] [[Matlab code 64bit](http://www.cs.toronto.edu/~babalex/TurboPixels64.rar)] [[Updated code](http://www.cs.toronto.edu/~babalex/superpixels_update.tgz)]
- Quick-Shift [7] [[VLFeat](http://www.vlfeat.org/overview/quickshift.html)]
- SLIC Superpixels [8] [[Project](http://ivrgwww.epfl.ch/supplementary_material/RK_SLICSuperpixels/index.html)]
- Segmentation by Minimum Code Length [9] [[Project](http://perception.csl.uiuc.edu/coding/image_segmentation/)]
- Biased Normalized Cut [10] [[Project](http://www.cs.berkeley.edu/~smaji/projects/biasedNcuts/)]
- Segmentation Tree [11-12] [[Project](http://vision.ai.uiuc.edu/segmentation)]
- Entropy Rate Superpixel Segmentation [13] [[Code](http://www.umiacs.umd.edu/~mingyliu/src/ers_matlab_wrapper_v0.1.zip)]
- Fast Approximate Energy Minimization via Graph Cuts[[Paper](http://www.csd.uwo.ca/faculty/olga/Papers/pami01_final.pdf)][[Code](http://vision.csd.uwo.ca/code/gco-v3.0.zip)]
- Efﬁcient Planar Graph Cuts with Applications in Computer Vision[[Paper](http://www.csd.uwo.ca/~schmidtf/pdf/schmidt_et_al_cvpr09.pdf)][[Code](http://vision.csd.uwo.ca/code/PlanarCut-v1.0.zip)]
- Isoperimetric Graph Partitioning for Image Segmentation[[Paper](http://www.cns.bu.edu/~lgrady/grady2006isoperimetric.pdf)][[Code](http://www.cns.bu.edu/~lgrady/grady2006isoperimetric_code.zip)]
- Random Walks for Image Segmentation[[Paper](http://www.cns.bu.edu/~lgrady/grady2006random.pdf)][[Code](http://www.cns.bu.edu/~lgrady/random_walker_matlab_code.zip)]
- Blossom V: A new implementation of a minimum cost perfect matching algorithm[[Code](http://pub.ist.ac.at/~vnk/software/blossom5-v2.03.src.tar.gz%20%20http:/pub.ist.ac.at/~vnk/software/blossom5-v2.03.src.tar.gz)]
- An Experimental Comparison of Min-Cut/Max-Flow Algorithms for Energy Minimization in Computer Vision[[Paper](http://www.csd.uwo.ca/~yuri/Papers/pami04.pdf)][[Code](http://pub.ist.ac.at/~vnk/software/maxflow-v3.01.src.tar.gz)]
- Geodesic Star Convexity for Interactive Image Segmentation[[Project](http://www.robots.ox.ac.uk/~vgg/software/iseg/)]
- Contour Detection and Image Segmentation Resources[[Project](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/grouping/resources.html)][[Code](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/grouping/BSR/BSR_source.tgz)]
- Biased Normalized Cuts[[Project](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/grouping/biasedNcuts/)]
- Max-flow/min-cut[[Project](http://vision.csd.uwo.ca/code/)]
- Chan-Vese Segmentation using Level Set[[Project](http://www.ipol.im/pub/art/2012/g-cv/)]
- A Toolbox of Level Set Methods[[Project](http://www.cs.ubc.ca/~mitchell/ToolboxLS/index.html)]
- Re-initialization Free Level Set Evolution via Reaction Diffusion[[Project](http://www4.comp.polyu.edu.hk/~cslzhang/RD/RD.htm)]
- Improved C-V active contour model[[Paper](http://www4.comp.polyu.edu.hk/~cskhzhang/J_papers/ICV.pdf)][[Code](http://www4.comp.polyu.edu.hk/~cskhzhang/J_papers/ICV.rar)]
- A Variational Multiphase Level Set Approach to Simultaneous Segmentation and Bias Correction[[Paper](http://www4.comp.polyu.edu.hk/~cskhzhang/J_papers/ICIP10_SVMLS.pdf)][[Code](http://www4.comp.polyu.edu.hk/~cskhzhang/J_papers/SVMLS_v0.rar)]
- Level Set Method Research by Chunming Li[[Project](http://www.engr.uconn.edu/~cmli/)]
- ClassCut for Unsupervised Class Segmentation[[cod](http://groups.inf.ed.ac.uk/calvin/classcut/ClassCut-release_v1.0.zip)e]
- SEEDS: Superpixels Extracted via Energy-Driven Sampling [[Project](http://www.vision.ee.ethz.ch/~vamichae/seeds/)][[other](http://www.mvdblive.org/seeds/)]

**三、目标检测Object Detection：**- A simple object detector with boosting [[Project](http://people.csail.mit.edu/torralba/shortCourseRLOC/boosting/boosting.html)]
- INRIA Object Detection and Localization Toolkit [1] [[Project](http://pascal.inrialpes.fr/soft/olt/)]
- Discriminatively Trained Deformable Part Models [2] [[Project](http://people.cs.uchicago.edu/~pff/latent/)]
- Cascade Object Detection with Deformable Part Models [3] [[Project](http://people.cs.uchicago.edu/~rbg/star-cascade/)]
- Poselet [4] [[Project](http://www.eecs.berkeley.edu/~lbourdev/poselets/)]
- Implicit Shape Model [5] [[Project](http://www.vision.ee.ethz.ch/~bleibe/code/ism.html)]
- Viola and Jones’s Face Detection [6] [[Project](http://pr.willowgarage.com/wiki/Face_detection)]
- Bayesian Modelling of Dyanmic Scenes for Object Detection[[Paper](http://vision.eecs.ucf.edu/papers/01512057.pdf)][[Code](http://vision.eecs.ucf.edu/Code/Background.zip)]
- Hand detection using multiple proposals[[Project](http://www.robots.ox.ac.uk/~vgg/software/hands/)]
- Color Constancy, Intrinsic Images, and Shape Estimation[[Paper](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/reconstruction/BarronMalikECCV2012.pdf)][[Code](http://www.cs.berkeley.edu/~barron/BarronMalikECCV2012_code.zip)]
- Discriminatively trained deformable part models[[Project](http://people.cs.uchicago.edu/~rbg/latent/)]
- Gradient Response Maps for Real-Time Detection of Texture-Less Objects: LineMOD [[Project](http://campar.cs.tum.edu/Main/StefanHinterstoisser)]
- Image Processing On Line[[Project](http://www.ipol.im/)]
- Robust Optical Flow Estimation[[Project](http://www.ipol.im/pub/pre/21/)]
- Where's Waldo: Matching People in Images of Crowds[[Project](http://homes.cs.washington.edu/~rahul/data/WheresWaldo.html)]
- Scalable Multi-class Object Detection[[Project](http://files.is.tue.mpg.de/jgall/projects/houghMC/houghMC.html)]
- Class-Specific Hough Forests for Object Detection[[Project](http://files.is.tue.mpg.de/jgall/projects/houghforest/houghforest.html)]
- Deformed Lattice Detection In Real-World Images[[Project](http://vision.cse.psu.edu/data/data.shtml)]
- Discriminatively trained deformable part models[[Project](http://people.cs.uchicago.edu/~rbg/latent/)]

四、显著性检测Saliency Detection：- Itti, Koch, and Niebur’ saliency detection [1] [[Matlab code](http://www.saliencytoolbox.net/)]
- Frequency-tuned salient region detection [2] [[Project](http://ivrgwww.epfl.ch/supplementary_material/RK_CVPR09/index.html)]
- Saliency detection using maximum symmetric surround [3] [[Project](http://ivrg.epfl.ch/supplementary_material/RK_ICIP2010/index.html)]
- Attention via Information Maximization [4] [[Matlab code](http://www.cse.yorku.ca/~neil/AIM.zip)]
- Context-aware saliency detection [5] [[Matlab code](http://webee.technion.ac.il/labs/cgm/Computer-Graphics-Multimedia/Software/Saliency/Saliency.html)]
- Graph-based visual saliency [6] [[Matlab code](http://www.klab.caltech.edu/~harel/share/gbvs.php)]
- Saliency detection: A spectral residual approach. [7] [[Matlab code](http://www.klab.caltech.edu/~xhou/projects/spectralResidual/spectralresidual.html)]
- Segmenting salient objects from images and videos. [8] [[Matlab code](http://www.cse.oulu.fi/MVG/Downloads/saliency)]
- Saliency Using Natural statistics. [9] [[Matlab code](http://cseweb.ucsd.edu/~l6zhang/)]
- Discriminant Saliency for Visual Recognition from Cluttered Scenes. [10] [[Code](http://www.svcl.ucsd.edu/projects/saliency/)]
- Learning to Predict Where Humans Look [11] [[Project](http://people.csail.mit.edu/tjudd/WherePeopleLook/index.html)]
- Global Contrast based Salient Region Detection [12] [[Project](http://cg.cs.tsinghua.edu.cn/people/~cmm/saliency/)]
- Bayesian Saliency via Low and Mid Level Cues[[Project](http://ice.dlut.edu.cn/lu/Project/TIP_scm/TIP_scm.htm)]
- Top-Down Visual Saliency via Joint CRF and Dictionary Learning[[Paper](http://faculty.ucmerced.edu/mhyang/papers/cvpr12a.pdf)][[Code](http://faculty.ucmerced.edu/mhyang/code/top-down-saliency.zip)]
- Saliency Detection: A Spectral Residual Approach[[Code](http://www.klab.caltech.edu/~xhou/projects/dva/dva.html)]

五、图像分类、聚类Image Classification, Clustering- Pyramid Match [1] [[Project](http://people.csail.mit.edu/jjl/libpmk/)]
- Spatial Pyramid Matching [2] [[Code](http://www.cs.unc.edu/~lazebnik/research/SpatialPyramid.zip)]
- Locality-constrained Linear Coding [3] [[Project](http://www.ifp.illinois.edu/~jyang29/LLC.htm)] [[Matlab code](http://www.ifp.illinois.edu/~jyang29/codes/CVPR10-LLC.rar)]
- Sparse Coding [4] [[Project](http://www.ifp.illinois.edu/~jyang29/ScSPM.htm)] [[Matlab code](http://www.ifp.illinois.edu/~jyang29/codes/CVPR09-ScSPM.rar)]
- Texture Classification [5] [[Project](http://www.robots.ox.ac.uk/~vgg/research/texclass/index.html)]
- Multiple Kernels for Image Classification [6] [[Project](http://www.robots.ox.ac.uk/~vgg/software/MKL/)]
- Feature Combination [7] [[Project](http://www.vision.ee.ethz.ch/~pgehler/projects/iccv09/index.html)]
- SuperParsing [[Code](http://www.cs.unc.edu/~jtighe/Papers/ECCV10/eccv10-jtighe-code.zip)]
- Large Scale Correlation Clustering Optimization[[Matlab code](http://www.wisdom.weizmann.ac.il/~bagon/matlab_code/LargeScaleCC1.0.tar.gz)]
- Detecting and Sketching the Common[[Project](http://www.wisdom.weizmann.ac.il/~vision/SketchTheCommon)]
- Self-Tuning Spectral Clustering[[Project](http://www.vision.caltech.edu/lihi/Demos/SelfTuningClustering.html)][[Code](http://www.vision.caltech.edu/lihi/Demos/SelfTuning/ZPclustering.zip)]
- User Assisted Separation of Reflections from a Single Image Using a Sparsity Prior[[Paper](http://www.wisdom.weizmann.ac.il/~levina/papers/assisted-eccv04.pdf)][[Code](http://www.wisdom.weizmann.ac.il/~levina/papers/reflections.zip)]
- Filters for Texture Classification[[Project](http://www.robots.ox.ac.uk/~vgg/research/texclass/filters.html#download)]
- Multiple Kernel Learning for Image Classification[[Project](http://www.robots.ox.ac.uk/~vgg/software/MKL/)]
- SLIC Superpixels[[Project](http://ivrg.epfl.ch/supplementary_material/RK_SLICSuperpixels/)]

六、抠图Image Matting- A Closed Form Solution to Natural Image Matting [[Code](http://people.csail.mit.edu/alevin/matting.tar.gz)]
- Spectral Matting [[Project](http://www.vision.huji.ac.il/SpectralMatting/)]
- Learning-based Matting [[Code](http://www.mathworks.com/matlabcentral/fileexchange/31412)]

七、目标跟踪Object Tracking：- A Forest of Sensors - Tracking Adaptive Background Mixture Models [[Project](http://www.ai.mit.edu/projects/vsam/Tracking/index.html)]
- Object Tracking via Partial Least Squares Analysis[[Paper](http://faculty.ucmerced.edu/mhyang/papers/tip12_pls_tracking.pdf)][[Code](http://faculty.ucmerced.edu/mhyang/code/PLS_tracker_tip.zip)]
- Robust Object Tracking with Online Multiple Instance Learning[[Paper](http://faculty.ucmerced.edu/mhyang/papers/pami11b.pdf)][[Code](http://vision.ucsd.edu/~bbabenko/project_miltrack.shtml)]
- Online Visual Tracking with Histograms and Articulating Blocks[[Project](http://www.cise.ufl.edu/~smshahed/tracking.htm)]
- Incremental Learning for Robust Visual Tracking[[Project](http://www.cs.toronto.edu/~dross/ivt/)]
- Real-time Compressive Tracking[[Project](http://www4.comp.polyu.edu.hk/~cslzhang/CT/CT.htm)]
- Robust Object Tracking via Sparsity-based Collaborative Model[[Project](http://faculty.ucmerced.edu/mhyang/project/cvpr12_scm.htm)]
- Visual Tracking via Adaptive Structural Local Sparse Appearance Model[[Project](http://faculty.ucmerced.edu/mhyang/project/cvpr12_jia_project.htm)]
- Online Discriminative Object Tracking with Local Sparse Representation[[Paper](http://faculty.ucmerced.edu/mhyang/papers/wacv12a.pdf)][[Code](http://faculty.ucmerced.edu/mhyang/code/wacv12a_code.zip)]
- Superpixel Tracking[[Project](http://faculty.ucmerced.edu/mhyang/papers/iccv11a.html)]
- Learning Hierarchical Image Representation with Sparsity, Saliency and Locality[[Paper](http://faculty.ucmerced.edu/mhyang/papers/bmvc11a.pdf)][[Code](http://faculty.ucmerced.edu/mhyang/code/BMVC11-HSSL-package.zip)]
- Online Multiple Support Instance Tracking [[Paper](http://faculty.ucmerced.edu/mhyang/papers/fg11a.pdf)][[Code](http://faculty.ucmerced.edu/mhyang/code/fg11_omsit.zip)]
- Visual Tracking with Online Multiple Instance Learning[[Project](http://vision.ucsd.edu/~bbabenko/project_miltrack.shtml)]
- Object detection and recognition[[Project](http://c2inet.sce.ntu.edu.sg/Jianxin/)]
- Compressive Sensing Resources[[Project](http://dsp.rice.edu/cs)]
- Robust Real-Time Visual Tracking using Pixel-Wise Posteriors[[Project](http://www.robots.ox.ac.uk/~cbibby/index.shtml)]
- Tracking-Learning-Detection[[Project](http://info.ee.surrey.ac.uk/Personal/Z.Kalal/)][[OpenTLD/C++ Code](https://github.com/arthurv/OpenTLD)]
- the HandVu：vision-based hand gesture interface[[Project](http://ilab.cs.ucsb.edu/index.php/component/content/article/12/29)]
- Learning Probabilistic Non-Linear Latent Variable Models for Tracking Complex Activities[[Project](http://files.is.tue.mpg.de/jgall/projects/stochGPLVM/stochGPLVM.html)]

八、Kinect：- Kinect toolbox[[Project](http://kinecttoolbox.codeplex.com/)]
- OpenNI[[Project](http://www.openni.org/)]
- zouxy09 CSDN Blog[[Resource](http://blog.csdn.net/zouxy09/article/details/8145688)]
- FingerTracker 手指跟踪[[code](http://makematics.com/code/FingerTracker/)]

九、3D相关：- 3D Reconstruction of a Moving Object[[Paper](http://www.wisdom.weizmann.ac.il/~ronen/papers/Simakov%20Frolova%20Basri%20-%20Dense%20Shape%20Reconstruction%20Under%20Arbitrary%20Unknown%20Lighting.pdf)] [[Code](http://www.wisdom.weizmann.ac.il/~bagon/matlab_code/SFB_matlab1.0.tar.gz)]
- Shape From Shading Using Linear Approximation[[Code](http://vision.eecs.ucf.edu/shadsrc.html)]
- Combining Shape from Shading and Stereo Depth Maps[[Project](http://vision.eecs.ucf.edu/combsrc.html)][[Code](http://vision.eecs.ucf.edu/projects/ShapeFromShading/combine.tar.Z)]
- Shape from Shading: A Survey[[Paper](http://vision.eecs.ucf.edu/papers/shah/99/ZTCS99.pdf)][[Code](http://vision.eecs.ucf.edu/projects/ShapeFromShading/SFS_Survey_1_00.tar.gz)]
- A Spatio-Temporal Descriptor based on 3D Gradients (HOG3D)[[Project](http://lear.inrialpes.fr/people/klaeser/research_hog3d)][[Code](http://lear.inrialpes.fr/people/klaeser/software_3d_video_descriptor)]
- Multi-camera Scene Reconstruction via Graph Cuts[[Paper](http://www.cs.cornell.edu/~rdz/papers/kz-eccv02-recon.pdf)][[Code](http://pub.ist.ac.at/~vnk/software/match-v3.4.src.tar.gz)]
- A Fast Marching Formulation of Perspective Shape from Shading under Frontal Illumination[[Paper](http://www.cs.ucf.edu/~vision)][[Code](http://www.ee.cityu.edu.hk/~syyuen/Public/SfS/PRL_Perspective_FMM.zip)]
- Reconstruction:3D Shape, Illumination, Shading, Reflectance, Texture[[Project](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/reconstruction/)]
- Monocular Tracking of 3D Human Motion with a Coordinated Mixture of Factor Analyzers[[Code](http://faculty.ucmerced.edu/mhyang/code/PackagedTrackingCode.tar.gz)]
- Learning 3-D Scene Structure from a Single Still Image[[Project](http://ai.stanford.edu/~asaxena/reconstruction3d/)]

十、机器学习算法：- Matlab class for computing Approximate Nearest Nieghbor (ANN) [[Matlab class](http://www.wisdom.weizmann.ac.il/~bagon/matlab_code/ann_wrapper_Mar2012.tar.gz) providing interface to[ANN library](http://www.cs.umd.edu/~mount/ANN/)]
- Random Sampling[[code](http://www.wisdom.weizmann.ac.il/~bagon/matlab_code/weight_sample.tar.gz)]
- Probabilistic Latent Semantic Analysis (pLSA)[[Code](http://www.robots.ox.ac.uk/~vgg/software/pLSA/pLSA_demo.tgz)]
- FASTANN and FASTCLUSTER for approximate k-means (AKM)[[Project](http://www.robots.ox.ac.uk/~vgg/software/fastann/)]
- Fast Intersection / Additive Kernel SVMs[[Project](http://www.cs.berkeley.edu/~smaji/projects/fiksvm/)]
- SVM[[Code](http://osmot.cs.cornell.edu/svm_light/)]
- Ensemble learning[[Project](http://c2inet.sce.ntu.edu.sg/Jianxin/)]
- Deep Learning[[Net](http://deeplearning.net/)]
- Deep Learning Methods for Vision[[Project](http://cs.nyu.edu/~fergus/tutorials/deep_learning_cvpr12/)]
- Neural Network for Recognition of Handwritten Digits[[Project](http://www.codeproject.com/KB/library/NeuralNetRecognition.aspx)]
- Training a deep autoencoder or a classifier on MNIST digits[[Project](http://www.cs.toronto.edu/~hinton/MatlabForSciencePaper.html)]
- THE MNIST DATABASE of handwritten digits[[Project](http://yann.lecun.com/exdb/mnist/)]
- Ersatz：deep neural networks in the cloud[[Project](http://www.ersatz1.com/)]
- Deep Learning [[Project](http://www.cs.nyu.edu/~yann/research/deep/)]
- sparseLM : Sparse Levenberg-Marquardt nonlinear least squares in C/C++[[Project](http://www.ics.forth.gr/~lourakis/sparseLM/)]
- Weka 3: Data Mining Software in Java[[Project](http://www.cs.waikato.ac.nz/ml/weka/)]
- Invited talk "A Tutorial on Deep Learning" by Dr. Kai Yu (余凯)[[Video](http://vipl.ict.ac.cn/News/academic-report-tutorial-deep-learning-dr-kai-yu)]
- CNN - Convolutional neural network class[[Matlab Tool](http://www.mathworks.cn/matlabcentral/fileexchange/24291)]
- Yann LeCun's Publications[[Wedsite](http://yann.lecun.com/exdb/publis/index.html#lecun-98)]
- LeNet-5, convolutional neural networks[[Project](http://yann.lecun.com/exdb/lenet/index.html)]
- Training a deep autoencoder or a classifier on MNIST digits[[Project](http://www.cs.toronto.edu/~hinton/MatlabForSciencePaper.html)]
- Deep Learning 大牛Geoffrey E. Hinton's HomePage[[Website](http://www.cs.toronto.edu/~hinton/)]
- Multiple Instance Logistic Discriminant-based Metric Learning (MildML) and Logistic Discriminant-based Metric Learning (LDML)[[Code](http://lear.inrialpes.fr/people/guillaumin/code.php#mildml)]
- Sparse coding simulation software[[Project](http://redwood.berkeley.edu/bruno/sparsenet/)]
- Visual Recognition and Machine Learning Summer School[[Software](http://lear.inrialpes.fr/software)]

十一、目标、行为识别Object, Action Recognition：- Action Recognition by Dense Trajectories[[Project](http://lear.inrialpes.fr/people/wang/dense_trajectories)][[Code](http://lear.inrialpes.fr/people/wang/download/dense_trajectory_release.tar.gz)]
- Action Recognition Using a Distributed Representation of Pose and Appearance[[Project](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/shape/action/)]
- Recognition Using Regions[[Paper](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/shape/glam-cvpr09.pdf)][[Code](http://www.eecs.berkeley.edu/Research/Projects/CS/vision/shape/glam_cvpr09_v2.zip)]
- 2D Articulated Human Pose Estimation[[Project](http://www.vision.ee.ethz.ch/~calvin/articulated_human_pose_estimation_code/)]
- Fast Human Pose Estimation Using Appearance and Motion via Multi-Dimensional Boosting Regression[[Paper](http://faculty.ucmerced.edu/mhyang/papers/cvpr07a.pdf)][[Code](http://www.cise.ufl.edu/~smshahed/cvpr07_fast_human_pose.zip)]
- Estimating Human Pose from Occluded Images[[Paper](http://faculty.ucmerced.edu/mhyang/papers/accv09a.pdf)][[Code](http://faculty.ucmerced.edu/mhyang/code/accv09_pose.zip)]
- Quasi-dense wide baseline matching[[Project](http://www.ee.oulu.fi/~jkannala/quasidense/quasidense.html)]
- ChaLearn Gesture Challenge: Principal motion: PCA-based reconstruction of motion histograms[[Project](http://gesture.chalearn.org/data/sample-code)]
- Real Time Head Pose Estimation with Random Regression Forests[[Project](http://files.is.tue.mpg.de/jgall/projects/RFhead/RFhead.html)]
- 2D Action Recognition Serves 3D Human Pose Estimation[[Project]](http://files.is.tue.mpg.de/jgall/projects/ARforPose/ARforPose.html)
- A Hough Transform-Based Voting Framework for Action Recognition[[Project]](http://files.is.tue.mpg.de/jgall/projects/houghAR/houghAR.html)
- Motion Interchange Patterns for Action Recognition in Unconstrained Videos[[Project]](http://www.openu.ac.il/home/hassner/projects/MIP/)
- 2D articulated human pose estimation software[[Project](http://groups.inf.ed.ac.uk/calvin/articulated_human_pose_estimation_code/)]
- Learning and detecting shape models [[code](http://groups.inf.ed.ac.uk/calvin/release-learn-shapes-v1.3.tgz)]
- Progressive Search Space Reduction for Human Pose Estimation[[Project](http://www.robots.ox.ac.uk/~vgg/software/UpperBody/index.html)]
- Learning Non-Rigid 3D Shape from 2D Motion[[Project](http://movement.stanford.edu/learning-nr-shape/)]

十二、图像处理：- Distance Transforms of Sampled Functions[[Project](http://cs.brown.edu/~pff/dt/)]
- The Computer Vision Homepage[[Project](http://www.cs.cmu.edu/~cil/vision.html)]
- Efficient appearance distances between windows[[code](http://groups.inf.ed.ac.uk/calvin/efficientAppDistances/releaseEfficientAppDistances.zip)]
- Image Exploration algorithm[[code](http://groups.inf.ed.ac.uk/calvin/ReleasedCode/image_exploration_v1.1.tgz)]
- Motion Magnification 运动放大 [[Project](http://people.csail.mit.edu/celiu/motionmag/motionmag.html)]
- Bilateral Filtering for Gray and Color Images 双边滤波器 [[Project](http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/MANDUCHI1/Bilateral_Filtering.html)]
- A Fast Approximation of the Bilateral Filter using a Signal Processing Approach [[Project]](http://people.csail.mit.edu/sparis/bf/)

十三、一些实用工具：- EGT: a Toolbox for Multiple View Geometry and Visual Servoing[[Project](http://egt.dii.unisi.it/)] [[Code](http://egt.dii.unisi.it/download/EGT_v1p3.zip)]
- a development kit of matlab mex functions for OpenCV library[[Project](http://www.cs.stonybrook.edu/~kyamagu/mexopencv/)]
- Fast Artificial Neural Network Library[[Project](http://leenissen.dk/fann/wp/)]

十四、人手及指尖检测与识别：- finger-detection-and-gesture-recognition [[Code](http://code.google.com/p/finger-detection-and-gesture-recognition/downloads/list)]
- Hand and Finger Detection using JavaCV[[Project](http://www.javacodegeeks.com/2012/12/hand-and-finger-detection-using-javacv.html?utm_source=feedburner&utm_medium=feed&utm_campaign=Feed%3A+JavaCodeGeeks+%28Java+Code+Geeks%29)]
- Hand and fingers detection[[Code](http://forum.openframeworks.cc/index.php?topic=1916.0)]

十五、场景解释：- Nonparametric Scene Parsing via Label Transfer [[Project](http://people.csail.mit.edu/celiu/LabelTransfer/code.html)]

十六、光流Optical flow：- High accuracy optical flow using a theory for warping [[Project](http://perception.inrialpes.fr/~chari/myweb/Software/)]
- Dense Trajectories Video Description [[Project](http://lear.inrialpes.fr/people/wang/dense_trajectories)]
- SIFT Flow: Dense Correspondence across Scenes and its Applications[[Project](http://people.csail.mit.edu/celiu/SIFTflow/)]
- KLT: An Implementation of the Kanade-Lucas-Tomasi Feature Tracker [[Project](http://www.ces.clemson.edu/~stb/klt/)]
- Tracking Cars Using Optical Flow[[Project](http://www.mathworks.cn/cn/help/vision/examples/tracking-cars-using-optical-flow.html)]
- Secrets of optical flow estimation and their principles[[Project](http://ps.is.tue.mpg.de/person/black#tabs-code)]
- implmentation of the Black and Anandan dense optical flow method[[Project](http://ps.is.tue.mpg.de/person/black#tabs-code)]
- Optical Flow Computation[[Project](https://www.ceremade.dauphine.fr/~peyre/numerical-tour/tours/multidim_5_opticalflow/#37)]
- Beyond Pixels: Exploring New Representations and Applications for Motion Analysis[[Project](http://people.csail.mit.edu/celiu/OpticalFlow/)]
- A Database and Evaluation Methodology for Optical Flow[[Project](http://vision.middlebury.edu/flow/)]
- optical flow relative[[Project](http://lmb.informatik.uni-freiburg.de/resources/software.php)]
- Robust Optical Flow Estimation [[Project](http://www.ipol.im/pub/pre/21/)]
- optical flow[[Project](http://www.jonathanmugan.com/GraphicsProject/OpticalFlow/)]

十七、图像检索Image Retrieval：- Semi-Supervised Distance Metric Learning for Collaborative Image Retrieval [[Paper](http://www.ee.columbia.edu/~wliu/CVPR08_ssml.pdf)][[code](http://www.ee.columbia.edu/~wliu/SSMetric.zip)]

十八、马尔科夫随机场Markov Random Fields：- Markov Random Fields for Super-Resolution [[](http://www.ee.columbia.edu/~wliu/CVPR08_ssml.pdf)[Project](http://people.csail.mit.edu/billf/project%20pages/sresCode/Markov%20Random%20Fields%20for%20Super-Resolution.html)]
- A Comparative Study of Energy Minimization Methods for Markov Random Fields with Smoothness-Based Priors [[Project](http://vision.middlebury.edu/MRF/)]

十九、运动检测Motion detection：- Moving Object Extraction, Using Models or Analysis of Regions [[](http://www.ee.columbia.edu/~wliu/CVPR08_ssml.pdf)[Project](http://www.visionbib.com/bibliography/motion-i763.html)]
- Background Subtraction: Experiments and Improvements for ViBe [[Project](http://www2.ulg.ac.be/telecom/publi/publications/mvd/VanDroogenbroeck2012Background/index.html)]
- A Self-Organizing Approach to Background Subtraction for Visual Surveillance Applications [[Project](http://www.na.icar.cnr.it/~maddalena.l/MODLab/SoftwareSOBS.html)]
- changedetection.net: A new change detection benchmark dataset[[Project](http://www.changedetection.net/)]
- ViBe - a powerful technique for background detection and subtraction in video sequences[[Project](http://www2.ulg.ac.be/telecom/research/vibe/)]
- Background Subtraction Program[[Project](http://www.umiacs.umd.edu/~knkim/UMD-BGS/index.html)]
- Motion Detection Algorithms[[Project](http://www.codeproject.com/Articles/10248/Motion-Detection-Algorithms)]
- Stuttgart Artificial Background Subtraction Dataset[[Project](http://www.vis.uni-stuttgart.de/index.php?id=sabs)]
- Object Detection, Motion Estimation, and Tracking[[Project](http://www.mathworks.cn/cn/help/vision/motion-analysis-and-tracking.html)]

Feature Detection and DescriptionGeneral Libraries:- [VLFeat](http://www.vlfeat.org/) – Implementation of various feature descriptors (including SIFT, HOG, and LBP) and covariant feature detectors (including DoG, Hessian, Harris Laplace, Hessian Laplace, Multiscale Hessian, Multiscale Harris). Easy-to-use Matlab interface. See [Modern features: Software](https://docs.google.com/viewer?a=v&pid=sites&srcid=ZGVmYXVsdGRvbWFpbnxlY2N2MTJmZWF0dXJlc3xneDo3ZDllMzVhMDA4YzEzNmU2) – Slides providing a demonstration of VLFeat and also links to other software. Check also [VLFeat hands-on session training](https://sites.google.com/site/eccv12features/)
- [OpenCV](http://opencv.org/) – Various implementations of modern feature detectors and descriptors (SIFT, SURF, FAST, BRIEF, ORB, FREAK, etc.)

Fast Keypoint Detectors for Real-time Applications:- [FAST](http://www.edwardrosten.com/work/fast.html) – High-speed corner detector implementation for a wide variety of platforms
- [AGAST](http://www6.in.tum.de/Main/ResearchAgast) – Even faster than the FAST corner detector. A multi-scale version of this method is used for the BRISK descriptor (ECCV 2010).

Binary Descriptors for Real-Time Applications:- [BRIEF](http://cvlab.epfl.ch/software/brief/) – C++ code for a fast and accurate interest point descriptor (not invariant to rotations and scale) (ECCV 2010)
- [ORB](http://docs.opencv.org/modules/features2d/doc/feature_detection_and_description.html) – OpenCV implementation of the Oriented-Brief (ORB) descriptor (invariant to rotations, but not scale)
- [BRISK](http://www.asl.ethz.ch/people/lestefan/personal/BRISK) – Efficient Binary descriptor invariant to rotations and scale. It includes a Matlab mex interface. (ICCV 2011)
- [FREAK](http://www.ivpe.com/freak.htm) – Faster than BRISK (invariant to rotations and scale) (CVPR 2012)

SIFT and SURF Implementations:- SIFT: [VLFeat](http://www.vlfeat.org/), [OpenCV](http://docs.opencv.org/modules/nonfree/doc/feature_detection.html), [Original code](http://www.cs.ubc.ca/~lowe/keypoints/) by David Lowe, [GPU implementation](http://cs.unc.edu/~ccwu/siftgpu/), [OpenSIFT](http://robwhess.github.com/opensift/)
- SURF: [Herbert Bay’s code](http://www.vision.ee.ethz.ch/~surf/), [OpenCV](http://docs.opencv.org/modules/nonfree/doc/feature_detection.html), [GPU-SURF](http://www.visual-experiments.com/demos/gpusurf/)

Other Local Feature Detectors and Descriptors:- [VGG Affine Covariant features](http://www.robots.ox.ac.uk/~vgg/research/affine/) – Oxford code for various affine covariant feature detectors and descriptors.
- [LIOP descriptor](http://vision.ia.ac.cn/Students/wzh/publication/liop/index.html) – Source code for the Local Intensity order Pattern (LIOP) descriptor (ICCV 2011).
- [Local Symmetry Features](http://www.cs.cornell.edu/projects/symfeat/) – Source code for matching of local symmetry features under large variations in lighting, age, and rendering style (CVPR 2012).

Global Image Descriptors:- [GIST](http://people.csail.mit.edu/torralba/code/spatialenvelope/) – Matlab code for the GIST descriptor
- [CENTRIST](https://sites.google.com/site/wujx2001/home) – Global visual descriptor for scene categorization and object detection (PAMI 2011)

Feature Coding and Pooling- [VGG Feature Encoding Toolkit](http://www.robots.ox.ac.uk/~vgg/software/enceval_toolkit/) – Source code for various state-of-the-art feature encoding methods – including Standard hard encoding, Kernel codebook encoding, Locality-constrained linear encoding, and Fisher kernel encoding.
- [Spatial Pyramid Matching](http://www.cs.illinois.edu/homes/slazebni/) – Source code for feature pooling based on spatial pyramid matching (widely used for image classification)

Convolutional Nets and Deep Learning- [EBLearn](http://eblearn.sourceforge.net/) – C++ Library for Energy-Based Learning. It includes several demos and step-by-step instructions to train classifiers based on convolutional neural networks.
- [Torch7](http://www.torch.ch/) – Provides a matlab-like environment for state-of-the-art machine learning algorithms, including a fast implementation of convolutional neural networks.
- [Deep Learning](http://deeplearning.net/software_links/) - Various links for deep learning software.

Part-Based Models- [Deformable Part-based Detector](http://people.cs.uchicago.edu/~rbg/latent/) – Library provided by the authors of the original paper (state-of-the-art in PASCAL VOC detection task)
- [Efficient Deformable Part-Based Detector](http://vision.mas.ecp.fr/Personnel/iasonas/dpms.html) – Branch-and-Bound implementation for a deformable part-based detector.
- [Accelerated Deformable Part Model](http://www.idiap.ch/~cdubout/coding.html) – Efficient implementation of a method that achieves the exact same performance of deformable part-based detectors but with significant acceleration (ECCV 2012).
- [Coarse-to-Fine Deformable Part Model](http://iselab.cvc.uab.es/CoarseToFine) – Fast approach for deformable object detection (CVPR 2011).
- [Poselets](http://www.eecs.berkeley.edu/~lbourdev/poselets/) – C++ and Matlab versions for object detection based on poselets.
- [Part-based Face Detector and Pose Estimation](http://www.ics.uci.edu/~xzhu/face/) – Implementation of a unified approach for face detection, pose estimation, and landmark localization (CVPR 2012).

Attributes and Semantic Features- [Relative Attributes](http://ttic.uchicago.edu/~dparikh/relative.html#code) – Modified implementation of RankSVM to train Relative Attributes (ICCV 2011).
- [Object Bank](http://vision.stanford.edu/projects/objectbank/) – Implementation of object bank semantic features (NIPS 2010). See also [ActionBank](http://www.cse.buffalo.edu/~jcorso/r/actionbank/)
- [Classemes, Picodes, and Meta-class features](http://vlg.cs.dartmouth.edu/projects/vlg_extractor/vlg_extractor/Home.html) – Software for extracting high-level image descriptors (ECCV 2010, NIPS 2011, CVPR 2012).

Large-Scale Learning- [Additive Kernels](http://ttic.uchicago.edu/~smaji/projects/fiksvm/) – Source code for fast additive kernel SVM classifiers (PAMI 2013).
- [LIBLINEAR](http://www.csie.ntu.edu.tw/~cjlin/liblinear/) – Library for large-scale linear SVM classification.
- [VLFeat](http://www.vlfeat.org/) – Implementation for Pegasos SVM and Homogeneous Kernel map.

Fast Indexing and Image Retrieval- [FLANN](http://www.cs.ubc.ca/~mariusm/index.php/FLANN/FLANN) – Library for performing fast approximate nearest neighbor.
- [Kernelized LSH](http://www.cse.ohio-state.edu/~kulis/klsh/klsh.htm) – Source code for Kernelized Locality-Sensitive Hashing (ICCV 2009).
- [ITQ Binary codes](http://www.unc.edu/~yunchao/itq.htm) – Code for generation of small binary codes using Iterative Quantization and other baselines such as Locality-Sensitive-Hashing (CVPR 2011).
- [INRIA Image Retrieval](http://lear.inrialpes.fr/src/inria_fisher/) – Efficient code for state-of-the-art large-scale image retrieval (CVPR 2011).

Object Detection- See [Part-based Models](http://rogerioferis.com/VisualRecognitionAndSearch/Resources.html#parts) and [Convolutional Nets](http://rogerioferis.com/VisualRecognitionAndSearch/Resources.html#convnets) above.
- [Pedestrian Detection at 100fps](https://bitbucket.org/rodrigob/doppia) – Very fast and accurate pedestrian detector (CVPR 2012).
- [Caltech Pedestrian Detection Benchmark](http://www.vision.caltech.edu/Image_Datasets/CaltechPedestrians/) – Excellent resource for pedestrian detection, with various links for state-of-the-art implementations.
- [OpenCV](http://docs.opencv.org/trunk/modules/objdetect/doc/cascade_classification.html?highlight=face%20detection) – Enhanced implementation of Viola&Jones real-time object detector, with trained models for face detection.
- [Efficient Subwindow Search](https://sites.google.com/site/christophlampert/software) – Source code for branch-and-bound optimization for efficient object localization (CVPR 2008).

3D Recognition- [Point-Cloud Library](http://www.pointclouds.org/) – Library for 3D image and point cloud processing.

Action Recognition- [ActionBank](http://www.cse.buffalo.edu/~jcorso/r/actionbank/) – Source code for action recognition based on the ActionBank representation (CVPR 2012).
- [STIP Features](http://www.di.ens.fr/~laptev/download.html) – software for computing space-time interest point descriptors
- [Independent Subspace Analysis](http://ai.stanford.edu/~quocle/) – Look for Stacked ISA for Videos (CVPR 2011)
- [Velocity Histories of Tracked Keypoints](http://www.cs.rochester.edu/~rmessing/uradl/) - C++ code for activity recognition using the velocity histories of tracked keypoints (ICCV 2009)

DatasetsAttributes- [Animals with Attributes](http://attributes.kyb.tuebingen.mpg.de/) – 30,475 images of 50 animals classes with 6 pre-extracted feature representations for each image.
- [aYahoo and aPascal](http://vision.cs.uiuc.edu/attributes/) – Attribute annotations for images collected from Yahoo and Pascal VOC 2008.
- [FaceTracer](http://www.cs.columbia.edu/CAVE/databases/facetracer/) – 15,000 faces annotated with 10 attributes and fiducial points.
- [PubFig](http://www.cs.columbia.edu/CAVE/databases/pubfig/) – 58,797 face images of 200 people with 73 attribute classifier outputs.
- [LFW](http://vis-www.cs.umass.edu/lfw/) – 13,233 face images of 5,749 people with 73 attribute classifier outputs.
- [Human Attributes](http://www.eecs.berkeley.edu/~lbourdev/poselets/) – 8,000 people with annotated attributes. Check also this [link](https://sharma.users.greyc.fr/hatdb/) for another dataset of human attributes.
- [SUN Attribute Database](http://cs.brown.edu/~gen/sunattributes.html) – Large-scale scene attribute database with a taxonomy of 102 attributes.
- [ImageNet Attributes](http://www.image-net.org/download-attributes) – Variety of attribute labels for the ImageNet dataset.
- [Relative attributes](http://ttic.uchicago.edu/~dparikh/relative.html#data) – Data for OSR and a subset of PubFig datasets. Check also this [link](http://vision.cs.utexas.edu/whittlesearch/) for the WhittleSearch data.
- [Attribute Discovery Dataset](http://tamaraberg.com/attributesDataset/index.html) – Images of shopping categories associated with textual descriptions.

Fine-grained Visual Categorization- [Caltech-UCSD Birds Dataset](http://www.vision.caltech.edu/visipedia/CUB-200-2011.html) – Hundreds of bird categories with annotated parts and attributes.
- [Stanford Dogs Dataset](http://vision.stanford.edu/aditya86/ImageNetDogs/) – 20,000 images of 120 breeds of dogs from around the world.
- [Oxford-IIIT Pet Dataset](http://www.robots.ox.ac.uk/~vgg/data/pets/) – 37 category pet dataset with roughly 200 images for each class. Pixel level trimap segmentation is included.
- [Leeds Butterfly Dataset](http://www.comp.leeds.ac.uk/scs6jwks/dataset/leedsbutterfly/) – 832 images of 10 species of butterflies.
- [Oxford Flower Dataset](http://www.robots.ox.ac.uk/~vgg/data/flowers/) – Hundreds of flower categories.

Face Detection- [FDDB](http://vis-www.cs.umass.edu/fddb/) – UMass face detection dataset and benchmark (5,000+ faces)
- [CMU/MIT](http://vasc.ri.cmu.edu/idb/html/face/frontal_images/index.html) – Classical face detection dataset.

Face Recognition- [Face Recognition Homepage](http://www.face-rec.org/databases/) – Large collection of face recognition datasets.
- [LFW](http://vis-www.cs.umass.edu/lfw/) – UMass unconstrained face recognition dataset (13,000+ face images).
- [NIST Face Homepage](http://www.nist.gov/itl/iad/ig/face.cfm) – includes face recognition grand challenge (FRGC), vendor tests (FRVT) and others.
- [CMU Multi-PIE](http://www.multipie.org/) – contains more than 750,000 images of 337 people, with 15 different views and 19 lighting conditions.
- [FERET](http://www.nist.gov/itl/iad/ig/colorferet.cfm) – Classical face recognition dataset.
- [Deng Cai’s face dataset in Matlab Format](http://www.cad.zju.edu.cn/home/dengcai/Data/FaceData.html) – Easy to use if you want play with simple face datasets including Yale, ORL, PIE, and Extended Yale B.
- [SCFace](http://www.scface.org/) – Low-resolution face dataset captured from surveillance cameras.

Handwritten Digits- [MNIST](http://yann.lecun.com/exdb/mnist/) – large dataset containing a training set of 60,000 examples, and a test set of 10,000 examples.

Pedestrian Detection- [Caltech Pedestrian Detection Benchmark](http://www.vision.caltech.edu/Image_Datasets/CaltechPedestrians/) – 10 hours of video taken from a vehicle,350K bounding boxes for about 2.3K unique pedestrians.
- [INRIA Person Dataset](http://pascal.inrialpes.fr/data/human/) – Currently one of the most popular pedestrian detection datasets.
- [ETH Pedestrian Dataset](http://www.vision.ee.ethz.ch/~aess/dataset/) – Urban dataset captured from a stereo rig mounted on a stroller.
- [TUD-Brussels Pedestrian Dataset](http://www.d2.mpi-inf.mpg.de/tud-brussels) – Dataset with image pairs recorded in an crowded urban setting with an onboard camera.
- [PASCAL Human Detection](http://pascallin.ecs.soton.ac.uk/challenges/VOC/) – One of 20 categories in PASCAL VOC detection challenges.
- [USC Pedestrian Dataset](http://iris.usc.edu/Vision-Users/OldUsers/bowu/DatasetWebpage/dataset.html) – Small dataset captured from surveillance cameras.

Generic Object Recognition- [ImageNet](http://www.image-net.org/) – Currently the largest visual recognition dataset in terms of number of categories and images.
- [Tiny Images](http://groups.csail.mit.edu/vision/TinyImages/) – 80 million 32x32 low resolution images.
- [Pascal VOC](http://pascallin.ecs.soton.ac.uk/challenges/VOC/) – One of the most influential visual recognition datasets.
- [Caltech 101](http://www.vision.caltech.edu/Image_Datasets/Caltech101/) / [Caltech 256](http://www.vision.caltech.edu/Image_Datasets/Caltech256/) – Popular image datasets containing 101 and 256 object categories, respectively.
- [MIT LabelMe](http://new-labelme.csail.mit.edu/Release3.0/index.php) – Online annotation tool for building computer vision databases.

Scene Recognition- [MIT SUN Dataset](http://groups.csail.mit.edu/vision/SUN/) – MIT scene understanding dataset.
- [UIUC Fifteen Scene Categories](http://www-cvr.ai.uiuc.edu/ponce_grp/data/) – Dataset of 15 natural scene categories.

Feature Detection and Description- [VGG Affine Dataset](http://www.robots.ox.ac.uk/~vgg/data/data-aff.html) – Widely used dataset for measuring performance of feature detection and description. Check[VLBenchmarks](http://www.vlfeat.org/benchmarks/index.html)for an evaluation framework.

Action Recognition- [Benchmarking Activity Recognition](http://rogerioferis.com/VisualRecognitionAndSearch/material/LiuFerisSunTutorial.pdf) – CVPR 2012 tutorial covering various datasets for action recognition.

RGBD Recognition- [RGB-D Object Dataset](http://www.cs.washington.edu/rgbd-dataset/index.html) – Dataset containing 300 common household objects



