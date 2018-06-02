#ifndef _LIBSVM_H
#define _LIBSVM_H


typedef struct svm_node { //index:data  用来存储一个单个特征
  int index;
  double value;
} svm_node;

typedef struct svm_problem { //struct svm_problem存储本次参加运算的所有样本
  int l;
  double* y;
  struct svm_node** x;
} svm_problem;
typedef struct svm_parameter { //svm模型的参数
  int svm_type;
  int kernel_type;
  double gamma;  /* for poly/rbf/sigmoid */
} svm_parameter;
//
// svm_model
//
enum { C_SVC };  /* svm_type */

typedef struct svm_model { //模型结构体
  svm_parameter param[1];  /* parameter */
  int nr_class;    /* number of classes, = 2 in regression/one class svm */
  int l;      /* total #SV */
  struct svm_node** SV;    /* SVs (SV[l]) */
  double** sv_coef;  /* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
  double* rho;    /* constants in decision functions (rho[k*(k-1)/2]) */
  /* for classification only */
  double* probA;    /* pariwise probability information */
  double* probB;
  int* label;    /* label of each class (label[k]) */
  int* nSV;    /* number of SVs for each class (nSV[k]) */
  /* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
  /* XXX */
  int free_sv;    /* 1 if svm_model is created by svm_load_model*/
  /* 0 if svm_model is created by svm_train */
} svm_model;

struct svm_model* svm_load_model(const char* model_file_name);  //加载模型
void svm_get_labels(const struct svm_model* model, int* label); //得到样本类型的表示
double svm_predict_values(const struct svm_model* model, const struct svm_node* x, double* dec_values);//预测label值
double svm_predict(const struct svm_model* model, const struct svm_node* x);//得到预测的label值
void svm_destroy_model(struct svm_model* model);  //释放model
double svm_predict_probability(const svm_model* model, const svm_node* x, double* prob_estimates);//概率预测


#endif /* _LIBSVM_H */
