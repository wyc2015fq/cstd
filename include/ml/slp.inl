// 感知器学习算法

//这里我们对乘机和的处理：如果大于0，则输出值为1；其他情况，输出值为-1  
double sigmoid (double val)  
{
  return (val > 0.0) ? 1 : -1;
}
//单层感知器
//singlelayer perceptrons(SLP)  
double slp_calculate_output(const double* inputs, const double* weights, int nInputs)  
{
  int i;
  double sum =0.0;  
  for (i = 0 ; i < nInputs ; ++i) {  
    sum += (weights[i] * inputs[i]);  
  }  
  //这里我们对乘机和的处理：如果大于0，则输出值为1；其他情况，输出值为-1  
  return sigmoid(sum);
}  
enum { nInputs  = 4,
nOutputs = 3,
 nHiddens = 4,
};

// 多层感知器（Multi-Layer Perceptrons） MLP，包含多层计算
typedef struct mlp
{
  double inputs[nInputs+1];//多一个，存放的bias，一般存放入1  
  double outputs[nOutputs];  
  double hiddens[nHiddens+1]; //多一个，存放的bias，一般存放入1  
  double weight_hiddens_2_inputs[nHiddens+1][nInputs+1];  
  double weight_outputs_2_hiddens[nOutputs][nHiddens+1];  
} mlp;
//计算输出端
int mlp_calculate_outputs(mlp * pMlp)  
{
  int i, h, o;
  double sum =0.0;  
  //首先计算隐藏层中的每一个结点的值  
  for (h = 0 ; h < nHiddens ; ++h)  
  {  
    double sum = 0.0;
    for (i = 0 ; i < nInputs + 1 ; ++i)
    {
      sum += pMlp->weight_hiddens_2_inputs[h][i]*pMlp->inputs[i];  
    }
    pMlp->hiddens[h] = sigmoid(sum);
  }
  //利用隐藏层作为“输入层”，计算输出层  
  for (o = 0 ; o < nOutputs ; ++o)  
  {
    double sum = 0.0;
    for (h = 0 ; h < nHiddens + 1 ; ++h)  
    {
      sum += pMlp->weight_outputs_2_hiddens[o][h]*pMlp->hiddens[h];  
    }
    pMlp->outputs[o] = sigmoid(sum);  
  }  
  return 0;  
}

// 感知器学习算法
//1.感知器学习
//其实感知器学习算法，就是利用第一节介绍的单层感知器。首先利用给的正确数据，
//计算得到输出值，将输出值和正确的值相比，由此来调整每一个输出端上的权值。
//公式2便是用来调整权值，首先 是一个“学习参数”，一般我将它设置成小于1的正数。T便是训练数据中的正确结果， 便是第i个输入端的输入值，便是第i个输入端上面的权值。


enum {
  nTests   = 4, //训练数据的数量  
    nTestInputs  = 2, //输入端的数量
  };
//计算输出值
int compute(double* inputs, double* weights)  
{
  int i;
  double sum = 0.0;
  for (i = 0 ; i < nTestInputs; ++i)  
  {  
    sum += weights[i]*inputs[i];  
  }  
  //bias  
  sum += 1.0 * weights[nInputs];  
  if(sum >0.0)  
    return 1;  
  else  
    return-1;  
}  
//  
int test_slp1()  
{  
  COLOR color_tab[] = {
   CV_RGB(255, 0, 0),
   CV_RGB(0, 255, 0),
   CV_RGB(100, 100, 255),
   CV_RGB(255, 0, 255),
   CV_RGB(255, 255, 0)
  };
  //PerceptronLearning Algorithm(PLA)  
  const double  alpha =0.2;       //学习参数  
  typedef struct slp  
  {  
    double inputs[nTestInputs];  
    double output;  
  } slp; //单层感知器

  //正确的训练数据
  slp slps[nTests] = {
    {-1.0,-1.0,-1.0},  
    {-1.0, 1.0, 1.0},  
    { 1.0,-1.0, 1.0},  
    { 1.0, 1.0, 1.0}  
  };
  double weights[nTestInputs + 1] = {0.0};
  BOOL bLearningOK = FALSE;
  int i, j, w;
  //感知器学习算法
  while(!bLearningOK)  
  {
    bLearningOK = TRUE;  
    for (i = 0 ; i < nTests ; ++i)  
    {
      int output = compute(slps[i].inputs, weights);  
      if(output!= (int)slps[i].output)
      {
        for(w = 0 ; w < nTestInputs; ++w)  
        {
          weights[w] += alpha * slps[i].output * slps[i].inputs[w];  
        }
        weights[nTestInputs] += alpha *slps[i].output ;  
        bLearningOK = FALSE;
      }
    }
  }
  {
    img_t im[1] = {0};
    double tx = im->w/3;
    double ty = im->h/3;
    imsetsize(im, 512, 512, 3, 1);
    imzero(im);
    for (i=0; i<im->h; ++i) {
      for (j=0; j<im->w; ++j) {
        double x = (j-(im->w/2))/tx;
        double y = (i-(im->h/2))/ty;
        double inputs[nTestInputs] = {x, y};
        double output = compute(inputs, weights);  
        int cluster_idx = output<0;
        COLOR clr = color_tab[cluster_idx];
        imdraw_SetPixel(im, 0, j, i, _RGB_A(clr, 10));
      }
    }
    for (i=0; i<nTests; ++i) {
      int cluster_idx = slps[i].output<0;
      COLOR clr = color_tab[cluster_idx];
      double x = slps[i].inputs[0];
      double y = slps[i].inputs[1];
      x = x*(im->w/2)+(im->w/2);
      y = y*(im->h/2)+(im->h/2);
      imdrawaa_ellipse(im, 0, x, y, 2, 2, 0, clr, 1);
    }
    imshow(im);
    cvWaitKey(-1);
  }
  for(w = 0 ; w < nTestInputs + 1 ; ++w)
  {
    printf("weight%f:%f\n", w, weights[w]);
  }
  printf("\n");  
  for (i = 0 ;i < nTests ; ++i)  
  {
    double tt;
    printf("rightresult：%d\t", slps[i].output);  
    tt = compute(slps[i].inputs,weights);
    printf("caculateresult:%f\n", tt);  
  }  
  
  return 0;  
}  


