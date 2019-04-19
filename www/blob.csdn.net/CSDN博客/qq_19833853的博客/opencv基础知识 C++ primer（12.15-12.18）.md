# opencv基础知识  C++ primer（12.15-12.18） - qq_19833853的博客 - CSDN博客
2016年12月15日 14:21:35[jessie Fei](https://me.csdn.net/qq_19833853)阅读数：139
opencv Mat 基础知识
1.仅复制矩阵头的方法(若改变A,C的值会影响B的值)
Mat A,C;
A=imread(argv[1],CV_LOAD_IMAGE_COLOR)；
Mat B(A);//为创建矩阵头，IplImage *img=cvLoadImage(" ",1）; Mat mtx(img);
C=A;
2.取ROI
Mat D=(A,Rect(10,10,100,100));
Mat E=A(Range:all(),Range(1,3));
3.矩阵的复制(改变A不会影响F、G)
Mat F=A.clone();
Mat G;
A.copyTo(G);
4.三种具体创建Mat的方法
Mat M(2,2,CV_8UC3，Scalar(0,0,255));
cout<<"M="<<endl<<" "<<M<<endl<<endl;

