
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>
#include "svm.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

// 函数名称:    k_function
// 函数功能:    利用RBF核函数计算内积
// 输入:        输入的特征向量x,与其计算内积的支持向量,模型参数
// 返回值：     内积值
// 备注：       无
double k_function(const svm_node* x, const svm_node* y, const svm_parameter* param)   //输入的特征向量x,第i个支持向量,参数
{
  //RBF核计算内积
  double sum = 0;

  while (x->index != -1 && y->index != -1) { //判断特征是否读取完了,这也是为什么先前申请内存的时候会多申请
    //l个节点的内存以及为什么每次读取完一行之后就会把接下来的一个
    if (x->index == y->index) {       //节点的index置为-1的原因
      double d = x->value - y->value;
      sum += d * d;
      ++x;
      ++y;
    }
    else {                    //如果某个特征向量的值为0,就会省略这个index,此时直接算另一个特征值的平方
      if (x->index > y->index) {
        sum += y->value * y->value;
        ++y;
      }
      else {
        sum += x->value * x->value;
        ++x;
      }
    }
  }

  while (x->index != -1) { // 如果是某个特征向量比另外一个短就执行下面两个while循环
    sum += x->value * x->value;
    ++x;
  }

  while (y->index != -1) {
    sum += y->value * y->value;
    ++y;
  }

  return exp(-param->gamma * sum); //exp函数在math.h里有

}

// 函数名称:    svm_predict_values
// 函数功能:    判断输入的x特征向量属于哪个类型
// 输入:        模型结构体,特征向量x,dec_values大于0属于一类,小于0属于另一类
// 返回值：     label值
// 备注：       无
double svm_predict_values(const svm_model* model, const struct svm_node* x, double* dec_values)
{

  int i, j, p;
  int nr_class = model->nr_class;
  int l = model->l;  //支持向量的个数
  int* start;
  int* vote;
  int vote_max_idx;
  double* kvalue = Malloc(double, l);

  for (i = 0; i < l; i++) {
    kvalue[i] = k_function(x, model->SV[i], model->param);  //利用核函数计算内积,特征向量x,第i个支持向量
  }

  //SV[i]代表第i个支持向量的指针
  start = Malloc(int, nr_class);
  start[0] = 0;

  for (i = 1; i < nr_class; i++) { //nSV[i] 每一类支持向量的个数
    start[i] = start[i - 1] + model->nSV[i - 1];  //start[i]里存放的是第i个类别第一个支持向量
  }

  vote = Malloc(int, nr_class);

  for (i = 0; i < nr_class; i++) {
    vote[i] = 0;  //每个类的投票个数先置为0
  }

  p = 0;

  for (i = 0; i < nr_class; i++) { //下面两个for循环计算出每个类的投票,共nr_class*(nr_class-1)/2次投票
    for (j = i + 1; j < nr_class; j++) {
      double sum = 0;
      int si = start[i];  //第i类别的第一个支持向量
      int sj = start[j];  //第j类别的第一个支持向量
      int ci = model->nSV[i]; //第i类别支持向量的个数
      int cj = model->nSV[j]; //第j类别支持向量的个数

      int k;
      double* coef1 = model->sv_coef[j - 1]; //sv_coef[m][l],m为总的类别数减1,l为支持向量的个数
      double* coef2 = model->sv_coef[i];

      for (k = 0; k < ci; k++) {
        sum += coef1[si + k] * kvalue[si + k];  //第i个支持向量的核内积与相应的alpha相乘
      }

      for (k = 0; k < cj; k++) {              //这里的alpha实际上是alpha与yi的乘积,所以也有负值
        sum += coef2[sj + k] * kvalue[sj + k];  //第j个支持向量的核内积与相应的alpha相乘
      }

      sum -= model->rho[p];                  //减去相应的b,b的存放顺序为类0和类1分类器、类0和类2.....
      dec_values[p] = sum;                   //减去b是因为里面存放的是b取反后的数

      if (dec_values[p] > 0) {    //若大于零则属于i类,否则属于j类
        ++vote[i];  //大于0是属于i类还是j类训练的时候就已经决定了(正样本为1,负样本为-1)
      }
      else {                     //理论上讲应该是大于1属于一类,小于-1属于另一类
        ++vote[j];
      }

      p++;
    }
  }

  vote_max_idx = 0;

  for (i = 1; i < nr_class; i++) {
    if (vote[i] > vote[vote_max_idx]) {
      vote_max_idx = i;  //得到投票数最多的类别
    }
  }


  free(kvalue);
  free(start);
  free(vote);
  return model->label[vote_max_idx]; //返回相应的label
}

// 函数名称:    svm_predict_values
// 函数功能:    判断输入的x特征向量属于哪个类型
// 输入:        模型结构体,特征向量x
// 返回值：     label值
// 备注：       无
double svm_predict(const svm_model* model, const struct svm_node* x)  //返回预测的label值
{
  int nr_class = model->nr_class;
  double* dec_values;
  double pred_result;

  dec_values = Malloc(double, nr_class * (nr_class - 1) / 2);
  pred_result = svm_predict_values(model, x, dec_values);
  free(dec_values);
  return pred_result;
}

// 函数名称:    sigmoid_predict
// 函数功能:    计算公式里的rij
// 输入:        预测值,A和B
// 返回值：     rij
// 备注：       概率预测预测的时候调用函数
static double sigmoid_predict(double decision_value, double A, double B)
{
  double fApB = decision_value * A + B;

  if (fApB >= 0) {
    return exp(-fApB) / (1.0 + exp(-fApB));
  }
  else {
    return 1.0 / (1 + exp(fApB)) ;
  }
}

// 函数名称:    multiclass_probability
// 函数功能:    计算概率
// 输入:        预测值,A和B
// 返回值：     rij
// 备注：       概率预测预测的时候调用函数
static void multiclass_probability(int k, double** r, double* p)
{
  int t, j;
  int iter = 0, max_iter = max(100, k);
  double** Q = Malloc(double*, k);
  double* Qp = Malloc(double, k);
  double pQp, eps = 0.005 / k;

  for (t = 0; t < k; t++) { //计算出Q矩阵
    p[t] = 1.0 / k; // Valid if k = 1
    Q[t] = Malloc(double, k);
    Q[t][t] = 0;

    for (j = 0; j < t; j++) {
      Q[t][t] += r[j][t] * r[j][t];
      Q[t][j] = Q[j][t];
    }

    for (j = t + 1; j < k; j++) {
      Q[t][t] += r[j][t] * r[j][t];
      Q[t][j] = -r[j][t] * r[t][j];
    }
  }

  for (iter = 0; iter < max_iter; iter++) {
    double max_error;
    // stopping condition, recalculate QP,pQP for numerical accuracy
    pQp = 0;

    for (t = 0; t < k; t++) {
      Qp[t] = 0;

      for (j = 0; j < k; j++) {
        Qp[t] += Q[t][j] * p[j];
      }

      pQp += p[t] * Qp[t];
    }

    max_error = 0;

    for (t = 0; t < k; t++) {
      double error = fabs(Qp[t] - pQp);

      if (error > max_error) {
        max_error = error;
      }
    }

    if (max_error < eps) {
      break;  //判断是否满足终止条件
    }

    for (t = 0; t < k; t++) {
      double diff = (-Qp[t] + pQp) / Q[t][t];
      p[t] += diff;
      pQp = (pQp + diff * (diff * Q[t][t] + 2 * Qp[t])) / (1 + diff) / (1 + diff);

      for (j = 0; j < k; j++) {
        Qp[j] = (Qp[j] + diff * Q[t][j]) / (1 + diff);
        p[j] /= (1 + diff);
      }
    }
  }

  for (t = 0; t < k; t++) {
    free(Q[t]);
  }

  free(Q);
  free(Qp);
}

// 函数名称:    svm_predict_probability
// 函数功能:    概率估计,概率估计的结果存放在prob_estimates中
// 输入:        模型结构体,特征向量x,概率估计的结果数组
// 返回值：     label值
// 备注：       无
double svm_predict_probability(const svm_model* model, const svm_node* x, double* prob_estimates)
{
  if ((model->param->svm_type == C_SVC) &&
      model->probA != NULL && model->probB != NULL) {
    int i, j;
    int nr_class = model->nr_class;
    double* dec_values = Malloc(double, nr_class * (nr_class - 1) / 2);
    double min_prob;
    double** pairwise_prob;
    int k;
    int prob_max_idx;

    svm_predict_values(model, x, dec_values);

    min_prob = 1e-7;
    pairwise_prob = Malloc(double*, nr_class);

    for (i = 0; i < nr_class; i++) {
      pairwise_prob[i] = Malloc(double, nr_class);
    }

    k = 0;

    for (i = 0; i < nr_class; i++) { //得到rij
      for (j = i + 1; j < nr_class; j++) {
        pairwise_prob[i][j] = min(max(sigmoid_predict(dec_values[k], model->probA[k], model->probB[k]), min_prob), 1 - min_prob);
        pairwise_prob[j][i] = 1 - pairwise_prob[i][j];
        k++;
      }
    }

    multiclass_probability(nr_class, pairwise_prob, prob_estimates);

    prob_max_idx = 0;

    for (i = 1; i < nr_class; i++) {
      if (prob_estimates[i] > prob_estimates[prob_max_idx]) {
        prob_max_idx = i;
      }
    }

    for (i = 0; i < nr_class; i++) {
      free(pairwise_prob[i]);
    }

    free(dec_values);
    free(pairwise_prob);
    return model->label[prob_max_idx];
  }
  else {
    return svm_predict(model, x);
  }
}

static const char* svm_type_table[] = { //模型的类型
  "c_svc", NULL
};

static const char* kernel_type_table[] = { //核函数的类型
  "rbf", NULL
};


static char* line = NULL;  //存储每次读取的字符串
static int max_line_len;   //一般先设置为1024

// 函数名称:    readline
// 函数功能:    从输入的file文件中读取一行字符串
// 输入:        file文件
// 返回值：     字符串
// 备注：       无
static char* readline(FILE* input)
{
  int len;

  //fgets的返回值为读取字符串的指针,如果没有读取到字符串就为NULL
  if (fgets(line, max_line_len, input) == NULL) { //从input中读取长度最大为max_line_len的字符串到line中
    return NULL;  //碰到换行符结束(存贮中包括换行符)
  }

  while (strrchr(line, '\n') == NULL) { //strrchr函数用于查找'\n'这个字符从line字符串后面开始第一次出现的位置
    //若为空，说明1024的空间存储不下这个字符串,下面会申请两倍的空间
    max_line_len *= 2;
    line = (char*) realloc(line, max_line_len);
    len = (int) strlen(line);

    if (fgets(line + len, max_line_len - len, input) == NULL) { //判断字符串是否读取完,没有的话会继续循环增加存储空间
      break;
    }
  }

  return line;
}

// 函数名称:    svm_load_model
// 函数功能:    从.model文件中加载模型
// 输入:        模型的文件名
// 返回值：     模型
// 备注：       无
svm_model* svm_load_model(const char* model_file_name)
{
  int m, l, i, j, k;
  int elements;
  long pos;
  svm_model* model;
  svm_parameter* param;
  svm_node* x_space;
  char cmd[81];
  FILE* fp;
  char* p, *endptr, *idx, *val;

  fp = fopen(model_file_name, "rb");

  if (fp == NULL) {
    return NULL;
  }

  // read parameters
  model = Malloc(svm_model, 1); //分配内存
  param = model->param;
  model->rho = NULL;  //相当于判别函数中的b
  model->probA = NULL;
  model->probB = NULL;
  model->label = NULL; //类别 label[n]
  model->nSV = NULL;  //每一个类别支持向量的个数

  ////////对于c_svc模型并使用RBF核函数的分类器而言,整个while循环把模型的类型,模型的核函数类型,////////////////
  ////////////RBF核函数的gamma参数,总的类别数nr_class,总的支持向量个数total_sv,rho也就是判别式里b的值///////////
  /////////////一共n*(n-1)/2个,分类器的类别数,每个类别支持向量的个数,都赋值给模型对应参数////
  while (1) {
    fscanf(fp, "%80s", cmd); //从fp模型中读取80个字符到cmd中,遇到空格结束读取

    if (strcmp(cmd, "svm_type") == 0) { //如果读取到的字符串为svm_type,接下来就读取是什么类型的模型
      int i;
      fscanf(fp, "%80s", cmd); //从fp模型中再读取80个字符,遇到空格结束读取

      for (i = 0; svm_type_table[i]; i++) {
        if (strcmp(svm_type_table[i], cmd) == 0) { //svm_type_table是个字符指针数组
          //指向"c_svc","nu_svc","one_class","epsilon_svr","nu_svr"
          param->svm_type = i;   //把读取到的模型类型赋值给模型的参数
          break;
        }
      }

      if (svm_type_table[i] == NULL) { //如果没有读取到任何模型,就返回
        fprintf(stderr, "unknown svm type.\n");
        free(model->rho);
        free(model->label);
        free(model->nSV);
        free(model);
        return NULL;
      }
    }
    else if (strcmp(cmd, "kernel_type") == 0) { //第二次循环将再取80个字符到cmd中,并与kernel_type比较
      int i;
      fscanf(fp, "%80s", cmd);

      for (i = 0; kernel_type_table[i]; i++) {
        if (strcmp(kernel_type_table[i], cmd) == 0) {
          param->kernel_type = i;     //读取模型文件中的kernel_type,初始化模型
          break;
        }
      }

      if (kernel_type_table[i] == NULL) {
        fprintf(stderr, "unknown kernel function.\n");
        free(model->rho);
        free(model->label);
        free(model->nSV);
        free(model);
        return NULL;
      }
    }
    else if (strcmp(cmd, "probA") == 0) {
      int i, n;
      n = model->nr_class * (model->nr_class - 1) / 2;
      model->probA = Malloc(double, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%lf", &model->probA[i]);
      }
    }
    else if (strcmp(cmd, "probB") == 0) {
      int i, n;
      n = model->nr_class * (model->nr_class - 1) / 2;
      model->probB = Malloc(double, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%lf", &model->probB[i]);
      }
    }
    else if (strcmp(cmd, "gamma") == 0) {
      fscanf(fp, "%lf", &param->gamma);
    }
    else if (strcmp(cmd, "nr_class") == 0) {
      fscanf(fp, "%d", &model->nr_class);
    }
    else if (strcmp(cmd, "total_sv") == 0) {
      fscanf(fp, "%d", &model->l);
    }
    else if (strcmp(cmd, "rho") == 0) {
      int i, n;
      n = model->nr_class * (model->nr_class - 1) / 2;
      model->rho = Malloc(double, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%lf", &model->rho[i]); //每一个二分类器的b
      }
    }
    else if (strcmp(cmd, "label") == 0) {
      int i, n = model->nr_class;
      model->label = Malloc(int, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%d", &model->label[i]);  //每个类别所用的label
      }
    }


    else if (strcmp(cmd, "nr_sv") == 0) {
      int i, n = model->nr_class;
      model->nSV = Malloc(int, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%d", &model->nSV[i]);  //每个类别支持向量的个数
      }
    }
    else if (strcmp(cmd, "SV") == 0) { //这个else if只是让读取流到SV下一行
      while (1) {
        int c = getc(fp);   //从fp中读取一个字符

        if (c == EOF || c == '\n') {
          break;
        }
      }

      break;
    }
    else {
      fprintf(stderr, "unknown text in model file: [%s]\n", cmd);
      free(model->rho);
      free(model->label);
      free(model->nSV);
      free(model);
      return NULL;
    }
  }

  // read sv_coef and SV
  //读取支持向量的拉格朗日乘子和支持向量
  elements = 0;
  pos = ftell(fp);   //读取文件fp的长度,从文件开始到当前位置
  max_line_len = 1024;
  line = Malloc(char, max_line_len);

  while (readline(fp) != NULL) { //readline会接着上面fp读取到的位置接着往下读,碰到'\n'结束,字符串存在line中
    //每一次while循环读取一行字符串
    p = strtok(line, ":"); //分解字符串,分隔符为':'。注意:执行一次只在第一个:处分割,把第一个:前的字符串提取出来

    //printf("%s",p);
    while (1) {
      p = strtok(NULL, ":"); //第二次执行,会把第一个和第二个:之间的字符串提取出来

      //printf("%s",p);
      if (p == NULL) {
        break;
      }

      ++elements;  //统计特征的总个数=特征的维数*支持向量的总个数l
    }
  }

  elements += model->l;  //elements的个数加上支持向量的个数

  fseek(fp, pos, SEEK_SET); //SEEK_SET=0,设置文件指针stream的位置,从SEEK_SET开始以pos为偏移量,执行完之后fp指向文件尾
  //SEEK_SET表示文件开头
  m = model->nr_class - 1 ;
  l = model->l;
  model->sv_coef = Malloc(double*, m);  //sv_coef相当于判别式中的alpha

  for (i = 0; i < m; i++) {
    model->sv_coef[i] = Malloc(double, l); //sv_coef[i]对应第i个alpha
  }

  model->SV = Malloc(svm_node*, l);    //SV保存支持向量的指针
  x_space = NULL;

  if (l > 0) {
    x_space = Malloc(svm_node, elements);
  }

  j = 0;

  for (i = 0; i < l; i++) {
    readline(fp);     //读取第一行字符到line中
    model->SV[i] = &x_space[j];
    p = strtok(line, " \t");   //以空格分割字符串
    //strtod（将字符串转换成浮点数） ,如果转换正确就会把endptr赋值为NULL
    //否则把字符串最后一个地址的地址赋给它
    model->sv_coef[0][i] = strtod(p, &endptr); //把P转换成float赋值给sv_coef[0][i]

    for (k = 1; k < m; k++) {
      p = strtok(NULL, " \t");
      model->sv_coef[k][i] = strtod(p, &endptr); //sv_coef[m][l],m为总的类别数减1,l为支持向量的个数
    }

    while (1) {
      idx = strtok(NULL, ":");//提取出索引
      val = strtok(NULL, " \t");//提取出数据

      if (val == NULL) {
        break;
      }

      x_space[j].index = (int) strtol(idx, &endptr, 10);
      x_space[j].value = strtod(val, &endptr);

      ++j;
    }

    x_space[j++].index = -1;//每提取完一行的索引和数据就把接下来一个节点的索引置为-1
  }

  free(line);

  if (ferror(fp) != 0 || fclose(fp) != 0) {
    return NULL;
  }

  model->free_sv = 1; // XXX
  return model;  //返回模型
}

// 函数名称:    svm_destroy_model
// 函数功能:    释放模型
// 输入:        模型名
// 返回值：     无
// 备注：       无
void svm_destroy_model(svm_model* model)  //释放模型
{
  int i;

  if (model->free_sv && model->l > 0) {
    free((void*)(model->SV[0]));
  }

  for (i = 0; i < model->nr_class - 1; i++) {
    free(model->sv_coef[i]);
  }

  free(model->SV);
  free(model->sv_coef);
  free(model->rho);
  free(model->label);
  free(model->nSV);
  free(model->probA);
  free(model->probB);
  free(model);
}
