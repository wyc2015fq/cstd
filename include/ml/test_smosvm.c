
#include "chalg.h"

typedef struct sparse_binary_vector {
  int* id;
  int idsize;
} sparse_binary_vector;
typedef struct sparse_vector {
  int* id;
  double* val;
  int idsize;
} sparse_vector;

typedef double* dense_vector;

typedef struct smosvm_train {
  int N;// = 0;
  int d;// = -1;
  double C;// = 0.05;
  double tolerance;// = 0.001;
  double eps;// = 0.001;
  double two_sigma_squared;// = 2;
  double delta_b;// = 0;
  double b;
  
  double* alph;                //lagrange 乘因子
  double* w;                   //权系数向量w,仅用于线性的核函数
  
  double* error_cache;
  
  //存储数据，只使用其中一个变量
  sparse_binary_vector* sparse_binary_points;
  sparse_vector* sparse_points;
  dense_vector* dense_points;
  
  int* target;   //训练数据的分类标签
  BOOL is_sparse_data;// = FALSE;
  BOOL is_binary;// = FALSE;
  BOOL is_test_only;// = FALSE;
  BOOL is_linear_kernel;// = FALSE;
  
  int first_test_i;// = 0;
  int end_support_i;// = -1;
  double* precomputed_self_dot_product;
  
  double ( *dot_product_func ) (struct smosvm_train* p, int, int );// = NULL; //计算两个样本之间的点积
  double ( *learned_func ) (struct smosvm_train* p, int );// = NULL; //学习函数
  double ( *kernel_func ) (struct smosvm_train* p, int, int );// = NULL; //核函数

} smosvm_train;

typedef double ( *fun1_t ) (struct smosvm_train* p, int );
typedef double ( *fun2_t ) (struct smosvm_train* p, int, int );

double learned_func_linear_sparse_binary(smosvm_train* p, int k ) {
  double s = 0;
  int i;
  for ( i = 0;i < ( int ) p->sparse_binary_points[ k ].idsize;i++ )
    s += p->w[ p->sparse_binary_points[ k ].id[ i ] ];
  s -= p->b;
  return s;
}
double learned_func_linear_sparse_nobinary(smosvm_train* p, int k ) {
  double s = 0;
  int i;
  for ( i = 0;i < ( int ) p->sparse_points[ k ].idsize;i++ ) {
    int j = p->sparse_points[ k ].id[ i ];
    double v = p->sparse_points[ k ].val[ i ];
    s += p->w[ j ] * v;
  }
  s -= p->b;
  return s;
}
double learned_func_linear_dense(smosvm_train* p, int k ) {
  double s = 0;
  int i;
  for ( i = 0;i < p->d;i++ )
    s += p->w[ i ] * p->dense_points[ k ][ i ];
  s -= p->b;
  return s;
}
double learned_func_nonlinear(smosvm_train* p, int k ) {
  double s = 0;
  int i;
  for ( i = 0;i < p->end_support_i; i++ )
    if ( p->alph[ i ] > 0 )
      s += p->alph[ i ] * p->target[ i ] * p->kernel_func(p, i, k );
  s -= p->b;
  return s;
}
double dot_product_sparse_binary(smosvm_train* p, int i1, int i2 ) {
  int p1 = 0, p2 = 0, dot = 0;
  int num1 = ( int ) p->sparse_binary_points[ i1 ].idsize;
  int num2 = ( int ) p->sparse_binary_points[ i2 ].idsize;
  while ( p1 < num1 && p2 < num2 ) {
    int a1 = ( int ) p->sparse_binary_points[ i1 ].id[ p1 ];
    int a2 = ( int ) p->sparse_binary_points[ i2 ].id[ p2 ];
    if ( a1 == a2 ) {
      dot++;
      p1++;
      p2++;
    } else if ( a1 > a2 )
      p2++;
    else
      p1++;
  }
  return ( double ) dot;
}
double dot_product_sparse_nonbinary(smosvm_train* p, int i1, int i2 ) {
  int p1 = 0, p2 = 0;
  double dot = 0;
  int num1 = ( int ) p->sparse_points[ i1 ].idsize;
  int num2 = ( int ) p->sparse_points[ i2 ].idsize;
  while ( p1 < num1 && p2 < num2 ) {
    int a1 = p->sparse_points[ i1 ].id[ p1 ];
    int a2 = p->sparse_points[ i2 ].id[ p2 ];
    if ( a1 == a2 ) {
      dot += p->sparse_points[ i1 ].val[ p1 ] * p->sparse_points[ i2 ].val[ p2 ];
      p1++;
      p2++;
    } else if ( a1 > a2 )
      p2++;
    else
      p1++;
  }
  return ( double ) dot;
}
double dot_product_dense(smosvm_train* p, int i1, int i2 ) {
  double dot = 0;
  int i;
  for ( i = 0;i < p->d;i++ )
    dot += p->dense_points[ i1 ][ i ] * p->dense_points[ i2 ][ i ];
  return dot;
}
double rbf_kernel(smosvm_train* p, int i1, int i2 ) {
  double s = p->dot_product_func(p, i1, i2 );
  s *= -2;
  s += p->precomputed_self_dot_product[ i1 ] + p->precomputed_self_dot_product[ i2 ];
  return exp( -s / p->two_sigma_squared );
}

double kernel(int d, const double* vx1, const double* vx2 ) {
  int j;
  double dot = 0;
  for ( j = 0;j < d;j++ )
    dot += ( vx1[ j ] - vx2[ j ] ) * ( vx1[ j ] - vx2[ j ] );
  dot /= 2;
  return -dot;
}

double predict_func(smosvm_train* p, const double* vx ) {
  int i;
  double s = 0;
  for ( i = 0;i < p->end_support_i;i++ )
    if ( p->alph[ i ] > 0 )
      s += p->alph[ i ] * p->target[ i ] * kernel(p->d, p->dense_points[ i ], vx );
  s -= p->b;
  return s;
}

#include "findfile.h"
int smosvm_train_load(smosvm_train* p, const char* fname) {
  FILE * pf = fopen(fname, "rb");
  strlist_t slist = {0};
  int i, j;
  if (NULL == pf) return 0;
  
  //获得参数29d
  p->N = 0; //训练样本的总数
  p->d = 2; //样本空间的维数
  p->C = 0.01; //惩罚因子
  p->tolerance = 0.001; //满足KKT条件的容忍度
  p->eps = 0.001;
  p->two_sigma_squared = 2; //径向基核函数的参数
  //p->data_file_name = "svm.data"; //数据文件
  //svm_file_name = "svm.model"; //模型文件
  //output_file_name = "svm.output"; //输出文件
  p->is_linear_kernel = FALSE; //是否是线性的核函数
  p->is_sparse_data = FALSE; //是否是稀疏数据
  p->is_binary = FALSE; //是否是二进制数据
  p->is_test_only = FALSE;
  get_file_list_txt(fname, &slist);
  p->N = slist.used;
  p->dense_points = MALLOC(double*, p->N);
  p->target = MALLOC(int, p->N);
  p->alph = MALLOC(double, p->N);
  p->error_cache = MALLOC(double, p->N);
  p->w = MALLOC(double, p->d);
  for (j=0; j<p->d; ++j) {
    p->w[j] = 0.;
  }
  for ( i=0; i<p->N; ++i ) {
    char* buf = slist.strs[i];
    p->dense_points[i] = MALLOC(double, p->d);
    for (j=0; j<p->d; ++j) {
      p->dense_points[i][j] = strtod(buf, &buf);
    }
    p->target[i] = strtol(buf, &buf, 10);
    p->alph[i] = 0.;
    p->error_cache[i] = 0.;
  }
  free_strlist(&slist);
  fclose(pf);
  
  //初始化学习函数,点积和核函数 26a
  if ( p->is_linear_kernel && p->is_sparse_data && p->is_binary )
    p->learned_func = learned_func_linear_sparse_binary;
  if ( p->is_linear_kernel && p->is_sparse_data && !p->is_binary )
    p->learned_func = learned_func_linear_sparse_nobinary;
  if ( p->is_linear_kernel && !p->is_sparse_data )
    p->learned_func = learned_func_linear_dense;
  if ( !p->is_linear_kernel )
    p->learned_func = learned_func_nonlinear;
  if ( p->is_sparse_data && p->is_binary )
    p->dot_product_func = dot_product_sparse_binary;
  if ( p->is_sparse_data && !p->is_binary )
    p->dot_product_func = dot_product_sparse_nonbinary;
  if ( !p->is_sparse_data )
    p->dot_product_func = dot_product_dense;
  if ( p->is_linear_kernel )
    p->kernel_func = p->dot_product_func;
  if ( !p->is_linear_kernel )
    p->kernel_func = rbf_kernel;
  if ( !p->is_linear_kernel ) {
    p->precomputed_self_dot_product = MALLOC(double, p->N);
    for ( i = 0;i < p->N; i++ )
      p->precomputed_self_dot_product[ i ] = p->dot_product_func(p, i, i );
  }
  p->end_support_i = p->N;
  return 1;
}

int smosvm_train_save(smosvm_train* p, const char* fname ) {
  FILE* pf = fopen(fname, "wb");
  int i, j;
  if (NULL==pf) return 0;
  fprintf(pf, "%d\r\n%d\r\n%d\r\n%d\r\n%f\r\n", p->d, p->is_sparse_data, p->is_binary, p->is_linear_kernel, p->b);
  if ( p->is_linear_kernel ) {
    for ( i = 0;i < p->d;i++ ) {
      fprintf(pf, "%f\r\n", p->w[ i ]);
    }
  } else {
    int n_support_vectors = 0;
    fprintf(pf, "%d\r\n", p->two_sigma_squared);
    for ( i = 0;i < p->end_support_i;i++ )
      if ( p->alph[ i ] > 0 )
        n_support_vectors++;
    fprintf(pf, "%d\r\n", n_support_vectors);
    for ( i = 0;i < p->end_support_i;i++ ) {
      if ( p->alph[ i ] > 0 )
        fprintf(pf, "%g\r\n", p->alph[ i ] );
    }
    for ( i = 0;i < p->end_support_i;i++ ) {
      if ( p->alph[ i ] > 0 ) {
        if ( p->is_sparse_data && p->is_binary ) {
          for ( j = 0;j < ( int ) p->sparse_binary_points[ i ].idsize;j++ )
            fprintf(pf, "%d ", ( p->sparse_binary_points[ i ].id[ j ] + 1 ) );
        } else if ( p->is_sparse_data && !p->is_binary ) {
          for ( j = 0;j < ( int ) p->sparse_points[ i ].idsize;j++ )
            fprintf(pf, "%d %d ", ( p->sparse_points[ i ].id[ j ] + 1 ), p->sparse_points[ i ].val[ j ] );
        } else {
          for ( j = 0;j < p->d;j++ )
            fprintf(pf, "%g ", p->dense_points[ i ][ j ] );
        }
        fprintf(pf, "%d\r\n", p->target[ i ] );
      }
    }
  }
  return 1;
}

int load_svm(smosvm_train* p, const char* svm_file_name ) {
  int n;
  FILE* pf = fopen(svm_file_name, "rb");
  if (NULL==pf) { return 0;  }
  //初始化学习函数和核函数
  p->N = 0;
  p->d = 2; //样本空间的维数
  p->C = 0.01; //惩罚因子
  p->tolerance = 0.001; //满足KKT条件的容忍度
  p->eps = 0.001;
  end_support_i = first_test_i = n = read_svm( svm_file );
  if ( n <= 0 )
    return 2;
  N += n;
  return 0;
}

int smosvm_train_free(smosvm_train* p) {
  int i;
  for ( i=0; i<p->N; ++i ) {
    SAFEFREE(p->dense_points[i]);
  }
  SAFEFREE(p->dense_points);
  SAFEFREE(p->precomputed_self_dot_product);
  SAFEFREE(p->error_cache);
  SAFEFREE(p->target);
  SAFEFREE(p->alph);
  SAFEFREE(p->w);
  return 0;
}

double error_rate(smosvm_train* p) {
  int i;
  int n_total = 0;
  int n_error = 0;
  for ( i = p->first_test_i;i < p->N;i++ ) {
    if ( p->learned_func(p, i ) > 0 != p->target[ i ] > 0 )
      n_error++;
    n_total++;
  }
  return (double)( n_error ) / (double)( n_total );
}

int takeStep(smosvm_train* p, int i1, int i2 ) {
  int y1, y2, s, i;
  double alph1, alph2; //旧的权值
  double a1, a2;  //新的权值
  double E1, E2, L, H, k11, k22, k12, eta, lobj, hobj;
  double* alph = p->alph,* error_cache = p->error_cache;
  double C = p->C, eps = p->eps, b = p->b;
  int* target = p->target;
  int is_linear_kernel = p->is_linear_kernel;
  int is_sparse_data = p->is_sparse_data;
  int is_binary = p->is_binary;
  fun1_t learned_func = p->learned_func;
  fun2_t kernel_func = p->kernel_func;

  if ( i1 == i2 )
    return 0;
  //21a
  alph1 = alph[ i1 ];
  y1 = target[ i1 ];
  if ( alph1 > 0 && alph1 < C )
    E1 = error_cache[ i1 ];
  else
    E1 = learned_func(p, i1 ) - y1;
  alph2 = alph[ i2 ];
  y2 = target[ i2 ];
  if ( alph2 > 0 && alph2 < C )
    E2 = error_cache[ i2 ];
  else
    E2 = learned_func(p, i2 ) - y2;
    
  s = y1 * y2;
  //22a
  if ( y1 == y2 ) {
    double gamma = alph1 + alph2;
    if ( gamma > C ) {
      L = gamma - C;
      H = C;
    } else {
      L = 0;
      H = gamma;
    }
  } else {
    double gamma = alph1 - alph2;
    if ( gamma > 0 ) {
      L = 0;
      H = C - gamma;
    } else {
      L = -gamma;
      H = C;
    }
  }
  
  if ( L == H )
    return 0;
  //22b
  k11 = kernel_func(p, i1, i1 );
  k12 = kernel_func(p, i1, i2 );
  k22 = kernel_func(p, i2, i2 );
  eta = 2 * k12 - k11 - k12;
  
  if ( eta < 0 ) {
    a2 = alph2 + y2 * ( E2 - E1 ) / eta;
    if ( a2 < L )
      a2 = L;
    else if ( a2 > H )
      a2 = H;
  } else {
    //22d
    double c1 = eta / 2;
    double c2 = y2 * ( E1 - E2 ) - eta * alph2;
    lobj = c1 * L * L + c2 * L;
    hobj = c1 * H * H + c2 * H;
    
    if ( lobj > hobj + eps )
      a2 = L;
    else if ( lobj < hobj - eps )
      a2 = H;
    else
      a2 = alph2;
  }
  if ( fabs( a2 - alph2 ) < eps * ( a2 + alph2 + eps ) )
    return 0;
  a1 = alph1 - s * ( a2 - alph2 );
  if ( a1 < 0 ) {
    a2 += s * a1;
    a1 = 0;
  } else if ( a1 > C ) {
    double t = a1 - C;
    a2 += s * t;
    a1 = C;
  }
  //更新b 23a
  {
    double b1, b2, bnew;
    if ( a1 > 0 && a1 < C )
      bnew = b + E1 + y1 * ( a1 - alph1 ) * k11 + y2 * ( a2 - alph2 ) * k12;
    else {
      if ( a2 > 0 && a2 < C )
        bnew = b + E2 + y1 * ( a1 - alph1 ) * k12 + y2 * ( a2 - alph2 ) * k22;
      else {
        b1 = b + E1 + y1 * ( a1 - alph1 ) * k11 + y2 * ( a2 - alph2 ) * k12;
        b2 = b + E2 + y1 * ( a1 - alph1 ) * k12 + y2 * ( a2 - alph2 ) * k22;
        bnew = ( b1 + b2 ) / 2;
      }
    }
    p->delta_b = bnew - b;
    b = bnew;
  }
  //如果使用线性的核函数，需要更新权向量 23c
  if ( is_linear_kernel ) {
    double t1 = y1 * ( a1 - alph1 );
    double t2 = y2 * ( a2 - alph2 );
    if ( is_sparse_data && is_binary ) {
      int p1, num1, p2, num2;
      num1 = ( int ) p->sparse_binary_points[ i1 ].idsize;
      for ( p1 = 0;p1 < num1;p1++ )
        p->w[ p->sparse_binary_points[ i1 ].id[ p1 ] ] += t1;
      num2 = ( int ) p->sparse_binary_points[ i2 ].idsize;
      for ( p2 = 0;p2 < num2;p2++ )
        p->w[ p->sparse_binary_points[ i2 ].id[ p2 ] ] += t2;
    } else if ( is_sparse_data && !is_binary ) {
      int p1, num1, p2, num2;
      num1 = ( int ) p->sparse_points[ i1 ].idsize;
      for ( p1 = 0;p1 < num1;p1++ )
        p->w[ p->sparse_points[ i1 ].id[ p1 ] ] += t1 * p->sparse_points[ i1 ].val[ p1 ];
      num2 = ( int ) p->sparse_points[ i2 ].idsize;
      for ( p2 = 0;p2 < num2;p2++ )
        p->w[ p->sparse_points[ i2 ].id[ p2 ] ] += t2 * p->sparse_points[ i2 ].val[ p2 ];
    } else
      for ( i = 0;i < p->d;i++ )
        p->w[ i ] += p->dense_points[ i1 ][ i ] * t1 + p->dense_points[ i2 ][ i ] * t2;
  }
  //更新错误率 24a
  {
    double t1 = y1 * ( a1 - alph1 );
    double t2 = y2 * ( a2 - alph2 );
    for ( i = 0;i < p->end_support_i;i++ )
      if ( 0 < alph[ i ] && alph[ i ] < C )
        error_cache[ i ] += t1 * kernel_func(p, i1, i ) + t2 * kernel_func(p, i2, i ) - p->delta_b;
    error_cache[ i1 ] = 0;
    error_cache[ i2 ] = 0;
  }
  alph[ i1 ] = a1;
  alph[ i2 ] = a2;
  return 1;
}

int examineExample(smosvm_train* p, int i1 ) {
  double y1, alph1, E1, r1;
  y1 = p->target[ i1 ];
  alph1 = p->alph[ i1 ];
  if ( alph1 > 0 && alph1 < p->C )
    E1 = p->error_cache[ i1 ];
  else
    E1 = p->learned_func(p, i1 ) - y1;
    
  r1 = y1 * E1;
  if ( ( r1 - p->tolerance && alph1 < p->C ) || ( r1 > p->tolerance && alph1 > 0 ) ) //不满足KKT条件
  {
    //寻找第二个权值更新，并返回
    //寻找|E1-E2|最大的，18b
    {
      int k, i2;
      double tmax;
      for ( i2 = -1, tmax = 0, k = 0;k < p->end_support_i;k++ )
        if ( p->alph[ k ] > 0 && p->alph[ k ] < p->C ) {
          double E2, temp;
          E2 = p->error_cache[ k ];
          temp = fabs( E1 - E2 );
          if ( temp > tmax ) {
            tmax = temp;
            i2 = k;
          }
        }
      if ( i2 >= 0 ) {
        if ( takeStep(p, i1, i2 ) )
          return 1;
      }
    }
    //寻找非边界样本19b
    {
      int k, k0;
      int i2;
      for ( k0 = ( int ) ( ( rand() / RAND_MAX ) * p->end_support_i ), k = k0;k < p->end_support_i + k0;k++ ) {
        i2 = k % p->end_support_i;
        if ( p->alph[ i2 ] > 0 && p->alph[ i2 ] < p->C ) {
          if ( takeStep(p, i1, i2 ) )
            return 1;
        }
      }
    }
    //寻找整个样本19c
    {
      int k0, k, i2;
      for ( k0 = ( int ) ( ( rand() / RAND_MAX ) * p->end_support_i ), k = k0;k < p->end_support_i + k0;k++ ) {
        i2 = k % p->end_support_i;
        if ( takeStep(p, i1, i2 ) )
          return 1;
      }
    }
    
  }
  return 0;
}

int smosvm_train_process(smosvm_train* p) {
  int numChanged=0, examineAll=0;
  int k;
  if ( !p->is_test_only ) {
    numChanged = 0;
    examineAll = 1;
    while ( numChanged > 0 || examineAll ) {
      numChanged = 0;
      if ( examineAll ) {
        for ( k = 0;k < p->N;k++ )
          numChanged += examineExample(p, k );
      } else {
        for ( k = 0;k < p->N;k++ )
          if ( p->alph[ k ] != 0 && p->alph[ k ] != p->C )
            numChanged += examineExample(p, k );
      }
      if ( examineAll == 1 )
        examineAll = 0;
      else if ( numChanged == 0 )
        examineAll = 1;
      //诊断信息36d
    }
    printf("threshold=%f", p->b);
  }
  printf("训练完毕，错误率为: %f", error_rate(p));
  return 0;
}

int test_smosvm() {
  smosvm_train st = {0};
  smosvm_train_load(&st, "svm.data");
  smosvm_train_process(&st);
  smosvm_train_save(&st, "svm2.model");
  smosvm_train_free(&st);
  return 0;
}

int main() {
  test_smosvm();
  return 0;
}
