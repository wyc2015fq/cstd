
#ifndef __ML_H__
#define __ML_H__

// disable deprecation warning which appears in VisualStudio 8.0
#if _MSC_VER >= 1400
#pragma warning(disable : 4996) 
#endif

#include <cxcore.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************\
*                               Main struct definitions                                  *
\****************************************************************************************/

/* log(2*PI) */
#define CC_LOG2PI (1.8378770664093454835606594728112)

/* columns of <trainData> matrix are training samples */
#define CC_COL_SAMPLE 0

/* rows of <trainData> matrix are training samples */
#define CC_ROW_SAMPLE 1

#define CC_IS_ROW_SAMPLE(flags) ((flags) & CC_ROW_SAMPLE)

struct CvVectors
{
    TypeId type;
    int dims, count;
    CvVectors* next;
    union
    {
        uchar** ptr;
        float** fl;
        double** db;
    } data;
};

#if 0
/* A structure, representing the lattice range of statmodel parameters.
   It is used for optimizing statmodel parameters by cross-validation method.
   The lattice is logarithmic, so <step> must be greater then 1. */
typedef struct CvParamLattice
{
    double min_val;
    double max_val;
    double step;
}
CvParamLattice;

CC_INLINE CvParamLattice cvParamLattice(double min_val, double max_val,
                                         double log_step)
{
    CvParamLattice pl;
    pl.min_val = MIN(min_val, max_val);
    pl.max_val = MAX(min_val, max_val);
    pl.step = MAX(log_step, 1.);
    return pl;
}

CC_INLINE CvParamLattice cvDefaultParamLattice(void)
{
    CvParamLattice pl = {0,0,0};
    return pl;
}
#endif

/* Variable type */
#define CC_VAR_NUMERICAL    0
#define CC_VAR_ORDERED      0
#define CC_VAR_CATEGORICAL  1

#define CC_TYPE_NAME_ML_SVM         "opencv-ml-svm"
#define CC_TYPE_NAME_ML_KNN         "opencv-ml-knn"
#define CC_TYPE_NAME_ML_NBAYES      "opencv-ml-bayesian"
#define CC_TYPE_NAME_ML_EM          "opencv-ml-em"
#define CC_TYPE_NAME_ML_BOOSTING    "opencv-ml-boost-tree"
#define CC_TYPE_NAME_ML_TREE        "opencv-ml-tree"
#define CC_TYPE_NAME_ML_ANN_MLP     "opencv-ml-ann-mlp"
#define CC_TYPE_NAME_ML_CNN         "opencv-ml-cnn"
#define CC_TYPE_NAME_ML_RTREES      "opencv-ml-random-trees"

class CC_EXPORTS CvStatModel
{
public:
    CvStatModel();
    virtual ~CvStatModel();

    virtual void clear();
    
    virtual void save(const char* filename, const char* name=0);
    virtual void load(const char* filename, const char* name=0);
    
    virtual void write(CvFileStorage* storage, const char* name);
    virtual void read(CvFileStorage* storage, CvFileNode* node);

protected:
    const char* default_model_name;
};


/****************************************************************************************\
*                                 Normal Bayes Classifier                                *
\****************************************************************************************/

class CC_EXPORTS CvNormalBayesClassifier : public CvStatModel
{
public:
    CvNormalBayesClassifier();
    virtual ~CvNormalBayesClassifier();

    CvNormalBayesClassifier(const img_t* _train_data, const img_t* _responses,
        const img_t* _var_idx=0, const img_t* _sample_idx=0);
        
    virtual bool train(const img_t* _train_data, const img_t* _responses,
        const img_t* _var_idx = 0, const img_t* _sample_idx=0, bool update=false);

    virtual float predict(const img_t* _samples, img_t* results=0) const;
    virtual void clear();

    virtual void write(CvFileStorage* storage, const char* name);
    virtual void read(CvFileStorage* storage, CvFileNode* node);

protected:
    int     var_count, var_all;
    img_t*  var_idx;
    img_t*  cls_labels;
    img_t** count;
    img_t** sum;
    img_t** productsum;
    img_t** avg;
    img_t** inv_eigen_values;
    img_t** cov_rotate_mats;
    img_t*  c;
};


/****************************************************************************************\
*                          K-Nearest Neighbour Classifier                                *
\****************************************************************************************/

// k Nearest Neighbors
class CC_EXPORTS CvKNearest : public CvStatModel
{
public:
    
    CvKNearest();
    virtual ~CvKNearest();

    CvKNearest(const img_t* _train_data, const img_t* _responses,
                const img_t* _sample_idx=0, bool _is_regression=false, int max_k=32);
        
    virtual bool train(const img_t* _train_data, const img_t* _responses,
                        const img_t* _sample_idx=0, bool is_regression=false,
                        int _max_k=32, bool _update_base=false);

    virtual float find_nearest(const img_t* _samples, int k, img_t* results=0,
        const float** neighbors=0, img_t* neighbor_responses=0, img_t* dist=0) const;

    virtual void clear();
    int get_max_k() const;
    int get_var_count() const;
    int get_sample_count() const;
    bool is_regression() const;

protected:

    virtual float write_results(int k, int k1, int start, int end,
        const float* neighbor_responses, const float* dist, img_t* _results,
        img_t* _neighbor_responses, img_t* _dist, suf32_t* sort_buf) const;

    virtual void find_neighbors_direct(const img_t* _samples, int k, int start, int end,
        float* neighbor_responses, const float** neighbors, float* dist) const;

    
    int max_k, var_count;
    int total;
    bool regression;
    CvVectors* samples;
};

/****************************************************************************************\
*                                   Support Vector Machines                              *
\****************************************************************************************/

// SVM training parameters
struct CC_EXPORTS CvSVMParams
{
    CvSVMParams();
    CvSVMParams(int _svm_type, int _kernel_type,
                 double _degree, double _gamma, double _coef0,
                 double _C, double _nu, double _p,
                 img_t* _class_weights, CTermCriteria _term_crit);
    
    int         svm_type;
    int         kernel_type;
    double      degree; // for poly
    double      gamma;  // for poly/rbf/sigmoid
    double      coef0;  // for poly/sigmoid

    double      C;  // for CC_SVM_C_SVC, CC_SVM_EPS_SVR and CC_SVM_NU_SVR
    double      nu; // for CC_SVM_NU_SVC, CC_SVM_ONE_CLASS, and CC_SVM_NU_SVR
    double      p; // for CC_SVM_EPS_SVR
    img_t*      class_weights; // for CC_SVM_C_SVC
    CTermCriteria term_crit; // termination criteria
};


struct CC_EXPORTS CvSVMKernel
{
    typedef void (CvSVMKernel::*Calc)(int vec_count, int vec_size, const float** vecs,
                                       const float* another, float* results);
    CvSVMKernel();
    CvSVMKernel(const CvSVMParams* _params, Calc _calc_func);
    virtual bool create(const CvSVMParams* _params, Calc _calc_func);
    virtual ~CvSVMKernel();
    
    virtual void clear();
    virtual void calc(int vcount, int n, const float** vecs, const float* another, float* results);

    const CvSVMParams* params;
    Calc calc_func;

    virtual void calc_non_rbf_base(int vec_count, int vec_size, const float** vecs,
                                    const float* another, float* results,
                                    double alpha, double beta);

    virtual void calc_linear(int vec_count, int vec_size, const float** vecs,
                              const float* another, float* results);
    virtual void calc_rbf(int vec_count, int vec_size, const float** vecs,
                           const float* another, float* results);
    virtual void calc_poly(int vec_count, int vec_size, const float** vecs,
                            const float* another, float* results);
    virtual void calc_sigmoid(int vec_count, int vec_size, const float** vecs,
                               const float* another, float* results);
};


struct CvSVMKernelRow
{
    CvSVMKernelRow* prev;
    CvSVMKernelRow* next;
    float* data;
};


struct CvSVMSolutionInfo
{
    double obj;
    double rho;
    double upper_bound_p;
    double upper_bound_n;
    double r;   // for Solver_NU
};

class CC_EXPORTS CvSVMSolver
{
public:
    typedef bool (CvSVMSolver::*SelectWorkingSet)(int& i, int& j);
    typedef float* (CvSVMSolver::*GetRow)(int i, float* row, float* dst, bool existed);
    typedef void (CvSVMSolver::*CalcRho)(double& rho, double& r);
    
    CvSVMSolver();

    CvSVMSolver(int count, int var_count, const float** samples, char* y,
                 int alpha_count, double* alpha, double Cp, double Cn,
                 CvMemStorage* storage, CvSVMKernel* kernel, GetRow get_row,
                 SelectWorkingSet select_working_set, CalcRho calc_rho);
    virtual bool create(int count, int var_count, const float** samples, char* y,
                 int alpha_count, double* alpha, double Cp, double Cn,
                 CvMemStorage* storage, CvSVMKernel* kernel, GetRow get_row,
                 SelectWorkingSet select_working_set, CalcRho calc_rho);
    virtual ~CvSVMSolver();

    virtual void clear();
    virtual bool solve_generic(CvSVMSolutionInfo& si);
    
    virtual bool solve_c_svc(int count, int var_count, const float** samples, char* y,
                              double Cp, double Cn, CvMemStorage* storage,
                              CvSVMKernel* kernel, double* alpha, CvSVMSolutionInfo& si);
    virtual bool solve_nu_svc(int count, int var_count, const float** samples, char* y,
                               CvMemStorage* storage, CvSVMKernel* kernel,
                               double* alpha, CvSVMSolutionInfo& si);
    virtual bool solve_one_class(int count, int var_count, const float** samples,
                                  CvMemStorage* storage, CvSVMKernel* kernel,
                                  double* alpha, CvSVMSolutionInfo& si);

    virtual bool solve_eps_svr(int count, int var_count, const float** samples, const float* y,
                                CvMemStorage* storage, CvSVMKernel* kernel,
                                double* alpha, CvSVMSolutionInfo& si);

    virtual bool solve_nu_svr(int count, int var_count, const float** samples, const float* y,
                               CvMemStorage* storage, CvSVMKernel* kernel,
                               double* alpha, CvSVMSolutionInfo& si);

    virtual float* get_row_base(int i, bool* _existed);
    virtual float* get_row(int i, float* dst);

    int sample_count;
    int var_count;
    int cache_size;
    int cache_line_size;
    const float** samples;
    const CvSVMParams* params;
    CvMemStorage* storage;
    CvSVMKernelRow lru_list;
    CvSVMKernelRow* rows;

    int alpha_count;

    double* G;
    double* alpha;

    // -1 - lower bound, 0 - free, 1 - upper bound
    char* alpha_status;

    char* y;
    double* b;
    float* buf[2];
    double eps;
    int max_iter;
    double C[2];  // C[0] == Cn, C[1] == Cp
    CvSVMKernel* kernel;
    
    SelectWorkingSet select_working_set_func;
    CalcRho calc_rho_func;
    GetRow get_row_func;

    virtual bool select_working_set(int& i, int& j);
    virtual bool select_working_set_nu_svm(int& i, int& j);
    virtual void calc_rho(double& rho, double& r);
    virtual void calc_rho_nu_svm(double& rho, double& r);

    virtual float* get_row_svc(int i, float* row, float* dst, bool existed);
    virtual float* get_row_one_class(int i, float* row, float* dst, bool existed);
    virtual float* get_row_svr(int i, float* row, float* dst, bool existed);
};


struct CvSVMDecisionFunc
{
    double rho;
    int sv_count;
    double* alpha;
    int* sv_index;
};


// SVM model
class CC_EXPORTS CvSVM : public CvStatModel
{
public:
    // SVM type
    enum { C_SVC=100, NU_SVC=101, ONE_CLASS=102, EPS_SVR=103, NU_SVR=104 };

    // SVM kernel type
    enum { LINEAR=0, POLY=1, RBF=2, SIGMOID=3 };

    CvSVM();
    virtual ~CvSVM();

    CvSVM(const img_t* _train_data, const img_t* _responses,
           const img_t* _var_idx=0, const img_t* _sample_idx=0,
           CvSVMParams _params=CvSVMParams());
        
    virtual bool train(const img_t* _train_data, const img_t* _responses,
                        const img_t* _var_idx=0, const img_t* _sample_idx=0,
                        CvSVMParams _params=CvSVMParams());

    virtual float predict(const img_t* _sample) const;
    virtual int get_support_vector_count() const;
    virtual const float* get_support_vector(int i) const;
    virtual void clear();

    virtual void write(CvFileStorage* storage, const char* name);
    virtual void read(CvFileStorage* storage, CvFileNode* node);
    int get_var_count() const { return var_idx ? var_idx->cols : var_all; }

protected:

    virtual bool set_params(const CvSVMParams& _params);
    virtual bool train1(int sample_count, int var_count, const float** samples,
                    const void* _responses, double Cp, double Cn,
                    CvMemStorage* _storage, double* alpha, double& rho);
    virtual void create_kernel();
    virtual void create_solver();

    virtual void write_params(CvFileStorage* fs);
    virtual void read_params(CvFileStorage* fs, CvFileNode* node);

    CvSVMParams params;
    img_t* class_labels;
    int var_all;
    float** sv;
    int sv_total;
    img_t* var_idx;
    img_t* class_weights;
    CvSVMDecisionFunc* decision_func;
    CvMemStorage* storage;

    CvSVMSolver* solver;
    CvSVMKernel* kernel;
};


/* The function trains SVM model with optimal parameters, obtained by using cross-validation.
The parameters to be estimated should be indicated by setting theirs values to FLT_MAX.
The optimal parameters are saved in <model_params> */
/*CVAPI(CvStatModel*)
cvTrainSVM_CrossValidation(const img_t* train_data, int tflag,
            const img_t* responses,
            CvStatModelParams* model_params,
            const CvStatModelParams* cross_valid_params,
            const img_t* comp_idx   CC_DEFAULT(0),
            const img_t* sample_idx CC_DEFAULT(0),
            const CvParamLattice* degree_lattice CC_DEFAULT(0),
            const CvParamLattice* gamma_lattice  CC_DEFAULT(0),
            const CvParamLattice* coef0_lattice  CC_DEFAULT(0),
            const CvParamLattice* C_lattice      CC_DEFAULT(0),
            const CvParamLattice* nu_lattice     CC_DEFAULT(0),
            const CvParamLattice* p_lattice      CC_DEFAULT(0));*/

/****************************************************************************************\
*                              Expectation - Maximization                                *
\****************************************************************************************/

struct CC_EXPORTS CvEMParams
{
    CvEMParams() : nclusters(10), cov_mat_type(1/*CvEM::COV_MAT_DIAGONAL*/),
        start_step(0/*CvEM::START_AUTO_STEP*/), probs(0), weights(0), means(0), covs(0)
    {
        term_crit=cTermCriteria(CC_TERMCRIT_ITER+CC_TERMCRIT_EPS, 100, FLT_EPSILON);
    }

    CvEMParams(int _nclusters, int _cov_mat_type=1/*CvEM::COV_MAT_DIAGONAL*/,
                int _start_step=0/*CvEM::START_AUTO_STEP*/,
                CTermCriteria _term_crit=cTermCriteria(CC_TERMCRIT_ITER+CC_TERMCRIT_EPS, 100, FLT_EPSILON),
                const img_t* _probs=0, const img_t* _weights=0, const img_t* _means=0, const img_t** _covs=0) :
                nclusters(_nclusters), cov_mat_type(_cov_mat_type), start_step(_start_step),
                probs(_probs), weights(_weights), means(_means), covs(_covs), term_crit(_term_crit)
    {}

    int nclusters;
    int cov_mat_type;
    int start_step;
    const img_t* probs;
    const img_t* weights;
    const img_t* means;
    const img_t** covs;
    CTermCriteria term_crit;
};


class CC_EXPORTS CvEM : public CvStatModel
{
public:
    // Type of covariation matrices
    enum { COV_MAT_SPHERICAL=0, COV_MAT_DIAGONAL=1, COV_MAT_GENERIC=2 };

    // The initial step
    enum { START_E_STEP=1, START_M_STEP=2, START_AUTO_STEP=0 };

    CvEM();
    CvEM(const img_t* samples, const img_t* sample_idx=0,
          CvEMParams params=CvEMParams(), img_t* labels=0);
    virtual ~CvEM();

    virtual bool train(const img_t* samples, const img_t* sample_idx=0,
                        CvEMParams params=CvEMParams(), img_t* labels=0);

    virtual float predict(const img_t* sample, img_t* probs) const;
    virtual void clear();

    int get_nclusters() const;
    const img_t* get_means() const;
    const img_t** get_covs() const;
    const img_t* get_weights() const;
    const img_t* get_probs() const;

protected:

    virtual void set_params(const CvEMParams& params,
                             const CvVectors& train_data);
    virtual void init_em(const CvVectors& train_data);
    virtual double run_em(const CvVectors& train_data);
    virtual void init_auto(const CvVectors& samples);
    virtual void kmeans(const CvVectors& train_data, int nclusters,
                         img_t* labels, CTermCriteria criteria,
                         const img_t* means);
    CvEMParams params;
    double log_likelihood;

    img_t* means;
    img_t** covs;
    img_t* weights;
    img_t* probs;

    img_t* log_weight_div_det;
    img_t* inv_eigen_values;
    img_t** cov_rotate_mats;
};

/****************************************************************************************\
*                                      Decision Tree                                     *
\****************************************************************************************/

struct CvPair32s32f
{
    int i;
    float val;
};


#define CC_DTREE_CAT_DIR(idx,subset) \
    (2*((subset[(idx)>>5]&(1 << ((idx) & 31)))==0)-1)

struct CvDTreeSplit
{
    int var_idx;
    int inversed;
    float quality;
    CvDTreeSplit* next;
    union
    {
        int subset[2];
        struct
        {
            float c;
            int split_point;
        }
        ord;
    };
};


struct CvDTreeNode
{
    int class_idx;
    int Tn;
    double value;

    CvDTreeNode* parent;
    CvDTreeNode* left;
    CvDTreeNode* right;

    CvDTreeSplit* split;

    int sample_count;
    TypeId type;
    int* num_valid;
    int offset;
    int buf_idx;
    double maxlr;

    // global pruning data
    int complexity;
    double alpha;
    double node_risk, tree_risk, tree_error;

    // cross-validation pruning data
    int* cv_Tn;
    double* cv_node_risk;
    double* cv_node_error;

    int get_num_valid(int vi) { return num_valid ? num_valid[vi] : sample_count; }
    void set_num_valid(int vi, int n) { if(num_valid) num_valid[vi] = n; }
};


struct CC_EXPORTS CvDTreeParams
{
    int   max_categories;
    int   max_type;
    int   min_sample_count;
    int   cv_folds;
    bool  use_surrogates;
    bool  use_1se_rule;
    bool  truncate_pruned_tree;
    float regression_accuracy;
    const float* priors;

    CvDTreeParams() : max_categories(10), max_type(INT_MAX), min_sample_count(10),
        cv_folds(10), use_surrogates(true), use_1se_rule(true),
        truncate_pruned_tree(true), regression_accuracy(0.01f), priors(0)
    {}

    CvDTreeParams(int _max_type, int _min_sample_count,
                   float _regression_accuracy, bool _use_surrogates,
                   int _max_categories, int _cv_folds,
                   bool _use_1se_rule, bool _truncate_pruned_tree,
                   const float* _priors) :
        max_categories(_max_categories), max_type(_max_type),
        min_sample_count(_min_sample_count), cv_folds (_cv_folds), 
        use_surrogates(_use_surrogates), use_1se_rule(_use_1se_rule), 
        truncate_pruned_tree(_truncate_pruned_tree),
        regression_accuracy(_regression_accuracy),
        priors(_priors)
    {}
};


struct CC_EXPORTS CvDTreeTrainData
{
    CvDTreeTrainData();
    CvDTreeTrainData(const img_t* _train_data, int _tflag,
                      const img_t* _responses, const img_t* _var_idx=0,
                      const img_t* _sample_idx=0, const img_t* _var_type=0,
                      const img_t* _missing_mask=0,
                      const CvDTreeParams& _params=CvDTreeParams(),
                      bool _shared=false, bool _add_labels=false);
    virtual ~CvDTreeTrainData();

    virtual void set_data(const img_t* _train_data, int _tflag,
                          const img_t* _responses, const img_t* _var_idx=0,
                          const img_t* _sample_idx=0, const img_t* _var_type=0,
                          const img_t* _missing_mask=0,
                          const CvDTreeParams& _params=CvDTreeParams(),
                          bool _shared=false, bool _add_labels=false,
                          bool _update_data=false);

    virtual void get_vectors(const img_t* _subsample_idx,
         float* values, uchar* missing, float* responses, bool get_class_idx=false);

    virtual CvDTreeNode* subsample_data(const img_t* _subsample_idx);

    virtual void write_params(CvFileStorage* fs);
    virtual void read_params(CvFileStorage* fs, CvFileNode* node);

    // release all the data
    virtual void clear();

    int get_num_classes() const;
    int get_var_type(int vi) const;
    int get_work_var_count() const;

    virtual int* get_class_labels(CvDTreeNode* n);
    virtual float* get_ord_responses(CvDTreeNode* n);
    virtual int* get_labels(CvDTreeNode* n);
    virtual int* get_cat_var_data(CvDTreeNode* n, int vi);
    virtual CvPair32s32f* get_ord_var_data(CvDTreeNode* n, int vi);
    virtual int get_child_buf_idx(CvDTreeNode* n);

    ////////////////////////////////////

    virtual bool set_params(const CvDTreeParams& params);
    virtual CvDTreeNode* new_node(CvDTreeNode* parent, int count,
                                   int storage_idx, int offset);

    virtual CvDTreeSplit* new_split_ord(int vi, float cmp_val,
                int split_point, int inversed, float quality);
    virtual CvDTreeSplit* new_split_cat(int vi, float quality);
    virtual void free_node_data(CvDTreeNode* node);
    virtual void free_train_data();
    virtual void free_node(CvDTreeNode* node);

    int sample_count, var_all, var_count, max_c_count;
    int ord_var_count, cat_var_count;
    bool have_labels, have_priors;
    bool is_classifier;

    int buf_count, buf_size;
    bool shared;

    img_t* cat_count;
    img_t* cat_ofs;
    img_t* cat_map;

    img_t* counts;
    img_t* buf;
    img_t* direction;
    img_t* split_buf;

    img_t* var_idx;
    img_t* var_type; // i-th element =
                     //   k<0  - ordered
                     //   k>=0 - categorical, see k-th element of cat_* arrays
    img_t* priors;
    img_t* priors_mult;

    CvDTreeParams params;

    CvMemStorage* tree_storage;
    CvMemStorage* temp_storage;

    CvDTreeNode* data_root;

    CvSet* node_heap;
    CvSet* split_heap;
    CvSet* cv_heap;
    CvSet* nv_heap;

    CRNG rng;
};


class CC_EXPORTS CvDTree : public CvStatModel
{
public:
    CvDTree();
    virtual ~CvDTree();

    virtual bool train(const img_t* _train_data, int _tflag,
                        const img_t* _responses, const img_t* _var_idx=0,
                        const img_t* _sample_idx=0, const img_t* _var_type=0,
                        const img_t* _missing_mask=0,
                        CvDTreeParams params=CvDTreeParams());

    virtual bool train(CvDTreeTrainData* _train_data, const img_t* _subsample_idx);

    virtual CvDTreeNode* predict(const img_t* _sample, const img_t* _missing_data_mask=0,
                                  bool preprocessed_input=false) const;
    virtual const img_t* get_var_importance();
    virtual void clear();

    virtual void read(CvFileStorage* fs, CvFileNode* node);
    virtual void write(CvFileStorage* fs, const char* name);
    
    // special read & write methods for trees in the tree ensembles
    virtual void read(CvFileStorage* fs, CvFileNode* node,
                       CvDTreeTrainData* data);
    virtual void write(CvFileStorage* fs);
    
    const CvDTreeNode* get_root() const;
    int get_pruned_tree_idx() const;
    CvDTreeTrainData* get_data();

protected:

    virtual bool do_train(const img_t* _subsample_idx);

    virtual void try_split_node(CvDTreeNode* n);
    virtual void split_node_data(CvDTreeNode* n);
    virtual CvDTreeSplit* find_best_split(CvDTreeNode* n);
    virtual CvDTreeSplit* find_split_ord_class(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_split_cat_class(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_split_ord_reg(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_split_cat_reg(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_surrogate_split_ord(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_surrogate_split_cat(CvDTreeNode* n, int vi);
    virtual double calc_node_dir(CvDTreeNode* node);
    virtual void complete_node_dir(CvDTreeNode* node);
    virtual void cluster_categories(const int* vectors, int vector_count,
        int var_count, int* sums, int k, int* cluster_labels);

    virtual void calc_node_value(CvDTreeNode* node);

    virtual void prune_cv();
    virtual double update_tree_rnc(int T, int fold);
    virtual int cut_tree(int T, int fold, double min_alpha);
    virtual void free_prune_data(bool cut_tree);
    virtual void free_tree();

    virtual void write_node(CvFileStorage* fs, CvDTreeNode* node);
    virtual void write_split(CvFileStorage* fs, CvDTreeSplit* split);
    virtual CvDTreeNode* read_node(CvFileStorage* fs, CvFileNode* node, CvDTreeNode* parent);
    virtual CvDTreeSplit* read_split(CvFileStorage* fs, CvFileNode* node);
    virtual void write_tree_nodes(CvFileStorage* fs);
    virtual void read_tree_nodes(CvFileStorage* fs, CvFileNode* node);

    CvDTreeNode* root;
    
    int pruned_tree_idx;
    img_t* var_importance;

    CvDTreeTrainData* data;
};


/****************************************************************************************\
*                                   Random Trees Classifier                              *
\****************************************************************************************/

class CvRTrees;

class CC_EXPORTS CvForestTree: public CvDTree
{
public:
    CvForestTree();
    virtual ~CvForestTree();

    virtual bool train(CvDTreeTrainData* _train_data, const img_t* _subsample_idx, CvRTrees* forest);

    virtual int get_var_count() const {return data ? data->var_count : 0;}
    virtual void read(CvFileStorage* fs, CvFileNode* node, CvRTrees* forest, CvDTreeTrainData* _data);

    /* dummy methods to avoid warnings: BEGIN */
    virtual bool train(const img_t* _train_data, int _tflag,
                        const img_t* _responses, const img_t* _var_idx=0,
                        const img_t* _sample_idx=0, const img_t* _var_type=0,
                        const img_t* _missing_mask=0,
                        CvDTreeParams params=CvDTreeParams());

    virtual bool train(CvDTreeTrainData* _train_data, const img_t* _subsample_idx);
    virtual void read(CvFileStorage* fs, CvFileNode* node);
    virtual void read(CvFileStorage* fs, CvFileNode* node,
                       CvDTreeTrainData* data);
    /* dummy methods to avoid warnings: END */

protected:
    virtual CvDTreeSplit* find_best_split(CvDTreeNode* n);
    CvRTrees* forest;
};


struct CC_EXPORTS CvRTParams : public CvDTreeParams
{
    //Parameters for the forest
    bool calc_var_importance; // true <=> RF processes variable importance
    int nactive_vars;
    CTermCriteria term_crit;

    CvRTParams() : CvDTreeParams(5, 10, 0, false, 10, 0, false, false, 0),
        calc_var_importance(false), nactive_vars(0)
    {
        term_crit = cTermCriteria(CC_TERMCRIT_ITER+CC_TERMCRIT_EPS, 50, 0.1);
    }

    CvRTParams(int _max_type, int _min_sample_count,
                float _regression_accuracy, bool _use_surrogates,
                int _max_categories, const float* _priors, bool _calc_var_importance,
                int _nactive_vars, int max_num_of_trees_in_the_forest,
                float forest_accuracy, int termcrit_type) :
        CvDTreeParams(_max_type, _min_sample_count, _regression_accuracy,
                       _use_surrogates, _max_categories, 0,
                       false, false, _priors),
        calc_var_importance(_calc_var_importance),
        nactive_vars(_nactive_vars)
    {
        term_crit = cTermCriteria(termcrit_type,
            max_num_of_trees_in_the_forest, forest_accuracy);
    }
};


class CC_EXPORTS CvRTrees : public CvStatModel
{
public:
    CvRTrees();
    virtual ~CvRTrees();
    virtual bool train(const img_t* _train_data, int _tflag,
                        const img_t* _responses, const img_t* _var_idx=0,
                        const img_t* _sample_idx=0, const img_t* _var_type=0,
                        const img_t* _missing_mask=0,
                        CvRTParams params=CvRTParams());
    virtual float predict(const img_t* sample, const img_t* missing = 0) const;
    virtual void clear();

    virtual const img_t* get_var_importance();
    virtual float get_proximity(const img_t* sample1, const img_t* sample2,
        const img_t* missing1 = 0, const img_t* missing2 = 0) const;

    virtual void read(CvFileStorage* fs, CvFileNode* node);
    virtual void write(CvFileStorage* fs, const char* name);

    img_t* get_active_var_mask();
    CRNG* get_rng();

    int get_tree_count() const;
    CvForestTree* get_tree(int i) const;

protected:

    bool grow_forest(const CTermCriteria term_crit);

    // array of the trees of the forest
    CvForestTree** trees;
    CvDTreeTrainData* data;
    int ntrees;
    int nclasses;
    double oob_error;
    img_t* var_importance;
    int nsamples;

    CRNG rng;
    img_t* active_var_mask;
};


/****************************************************************************************\
*                                   Boosted tree classifier                              *
\****************************************************************************************/

struct CC_EXPORTS CvBoostParams : public CvDTreeParams
{
    int boost_type;
    int weak_count;
    int split_criteria;
    double weight_trim_rate;

    CvBoostParams();
    CvBoostParams(int boost_type, int weak_count, double weight_trim_rate,
                   int max_type, bool use_surrogates, const float* priors);
};


class CvBoost;

class CC_EXPORTS CvBoostTree: public CvDTree
{
public:
    CvBoostTree();
    virtual ~CvBoostTree();

    virtual bool train(CvDTreeTrainData* _train_data,
                        const img_t* subsample_idx, CvBoost* ensemble);

    virtual void scale(double s);
    virtual void read(CvFileStorage* fs, CvFileNode* node,
                       CvBoost* ensemble, CvDTreeTrainData* _data);
    virtual void clear();

    /* dummy methods to avoid warnings: BEGIN */
    virtual bool train(const img_t* _train_data, int _tflag,
                        const img_t* _responses, const img_t* _var_idx=0,
                        const img_t* _sample_idx=0, const img_t* _var_type=0,
                        const img_t* _missing_mask=0,
                        CvDTreeParams params=CvDTreeParams());

    virtual bool train(CvDTreeTrainData* _train_data, const img_t* _subsample_idx);
    virtual void read(CvFileStorage* fs, CvFileNode* node);
    virtual void read(CvFileStorage* fs, CvFileNode* node,
                       CvDTreeTrainData* data);
    /* dummy methods to avoid warnings: END */

protected:
    
    virtual void try_split_node(CvDTreeNode* n);
    virtual CvDTreeSplit* find_surrogate_split_ord(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_surrogate_split_cat(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_split_ord_class(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_split_cat_class(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_split_ord_reg(CvDTreeNode* n, int vi);
    virtual CvDTreeSplit* find_split_cat_reg(CvDTreeNode* n, int vi);
    virtual void calc_node_value(CvDTreeNode* n);
    virtual double calc_node_dir(CvDTreeNode* n);

    CvBoost* ensemble;
};


class CC_EXPORTS CvBoost : public CvStatModel
{
public:
    // Boosting type
    enum { DISCRETE=0, REAL=1, LOGIT=2, GENTLE=3 };
    
    // Splitting criteria
    enum { DEFAULT=0, GINI=1, MISCLASS=3, SQERR=4 };

    CvBoost();
    virtual ~CvBoost();

    CvBoost(const img_t* _train_data, int _tflag,
             const img_t* _responses, const img_t* _var_idx=0,
             const img_t* _sample_idx=0, const img_t* _var_type=0,
             const img_t* _missing_mask=0,
             CvBoostParams params=CvBoostParams());
        
    virtual bool train(const img_t* _train_data, int _tflag,
             const img_t* _responses, const img_t* _var_idx=0,
             const img_t* _sample_idx=0, const img_t* _var_type=0,
             const img_t* _missing_mask=0,
             CvBoostParams params=CvBoostParams(),
             bool update=false);

    virtual float predict(const img_t* _sample, const img_t* _missing=0,
                           img_t* weak_responses=0, CSlice slice=CC_WHOLE_SEQ,
                           bool raw_mode=false) const;

    virtual void prune(CSlice slice);

    virtual void clear();

    virtual void write(CvFileStorage* storage, const char* name);
    virtual void read(CvFileStorage* storage, CvFileNode* node);

    CvSeq* get_weak_predictors();

    img_t* get_weights();
    img_t* get_subtree_weights();
    img_t* get_weak_response();
    const CvBoostParams& get_params() const;

protected:

    virtual bool set_params(const CvBoostParams& _params);
    virtual void update_weights(CvBoostTree* tree);
    virtual void trim_weights();
    virtual void write_params(CvFileStorage* fs);
    virtual void read_params(CvFileStorage* fs, CvFileNode* node);

    CvDTreeTrainData* data;
    CvBoostParams params;    
    CvSeq* weak;
    
    img_t* orig_response;
    img_t* sum_response;
    img_t* weak_eval;
    img_t* subsample_mask;
    img_t* weights;
    img_t* subtree_weights;
    bool have_subsample;
};


/****************************************************************************************\
*                              Artificial Neural Networks (ANN)                          *
\****************************************************************************************/

/////////////////////////////////// Multi-Layer Perceptrons //////////////////////////////

struct CC_EXPORTS CvANN_MLP_TrainParams
{
    CvANN_MLP_TrainParams();
    CvANN_MLP_TrainParams(CTermCriteria term_crit, int train_method,
                           double param1, double param2=0);
    ~CvANN_MLP_TrainParams();

    enum { BACKPROP=0, RPROP=1 };

    CTermCriteria term_crit;
    int train_method;

    // backpropagation parameters
    double bp_dw_scale, bp_moment_scale;
    
    // rprop parameters
    double rp_dw0, rp_dw_plus, rp_dw_minus, rp_dw_min, rp_dw_max;
};


class CC_EXPORTS CvANN_MLP : public CvStatModel
{
public:
    CvANN_MLP();
    CvANN_MLP(const img_t* _layer_sizes,
               int _activ_func=SIGMOID_SYM,
               double _f_param1=0, double _f_param2=0);

    virtual ~CvANN_MLP();

    virtual void create(const img_t* _layer_sizes,
                         int _activ_func=SIGMOID_SYM,
                         double _f_param1=0, double _f_param2=0);

    virtual int train(const img_t* _inputs, const img_t* _outputs,
                       const img_t* _sample_weights, const img_t* _sample_idx=0,
                       CvANN_MLP_TrainParams _params = CvANN_MLP_TrainParams(),
                       int flags=0);
    virtual float predict(const img_t* _inputs,
                           img_t* _outputs) const;

    virtual void clear();

    // possible activation functions
    enum { IDENTITY = 0, SIGMOID_SYM = 1, GAUSSIAN = 2 };

    // available training flags
    enum { UPDATE_WEIGHTS = 1, NO_INPUT_SCALE = 2, NO_OUTPUT_SCALE = 4 };

    virtual void read(CvFileStorage* fs, CvFileNode* node);
    virtual void write(CvFileStorage* storage, const char* name);

    int get_layer_count() { return layer_sizes ? layer_sizes->cols : 0; }
    const img_t* get_layer_sizes() { return layer_sizes; }
    double* get_weights(int layer)
    {
        return layer_sizes && weights &&
            (unsigned)layer <= (unsigned)layer_sizes->cols ? weights[layer] : 0;
    }

protected:

    virtual bool prepare_to_train(const img_t* _inputs, const img_t* _outputs,
            const img_t* _sample_weights, const img_t* _sample_idx,
            CvVectors* _ivecs, CvVectors* _ovecs, double** _sw, int _flags);

    // sequential random backpropagation
    virtual int train_backprop(CvVectors _ivecs, CvVectors _ovecs, const double* _sw);
    
    // RPROP algorithm
    virtual int train_rprop(CvVectors _ivecs, CvVectors _ovecs, const double* _sw);

    virtual void calc_activ_func(img_t* xf, const double* bias) const;
    virtual void calc_activ_func_deriv(img_t* xf, img_t* deriv, const double* bias) const;
    virtual void set_activ_func(int _activ_func=SIGMOID_SYM,
                                 double _f_param1=0, double _f_param2=0);
    virtual void init_weights();
    virtual void scale_input(const img_t* _src, img_t* _dst) const;
    virtual void scale_output(const img_t* _src, img_t* _dst) const;
    virtual void calc_input_scale(const CvVectors* vecs, int flags);
    virtual void calc_output_scale(const CvVectors* vecs, int flags);

    virtual void write_params(CvFileStorage* fs);
    virtual void read_params(CvFileStorage* fs, CvFileNode* node);

    img_t* layer_sizes;
    img_t* wbuf;
    img_t* sample_weights;
    double** weights;
    double f_param1, f_param2;
    double min_val, max_val, min_val1, max_val1;
    int activ_func;
    int max_count, max_buf_sz;
    CvANN_MLP_TrainParams params;
    CRNG rng;
};

#if 0
/****************************************************************************************\
*                            Convolutional Neural Network                                *
\****************************************************************************************/
typedef struct CvCNNLayer CvCNNLayer;
typedef struct CvCNNetwork CvCNNetwork;

#define CC_CNN_LEARN_RATE_DECREASE_HYPERBOLICALLY  1
#define CC_CNN_LEARN_RATE_DECREASE_SQRT_INV        2
#define CC_CNN_LEARN_RATE_DECREASE_LOG_INV         3

#define CC_CNN_GRAD_ESTIM_RANDOM        0
#define CC_CNN_GRAD_ESTIM_BY_WORST_IMG  1

#define ICC_CNN_LAYER                0x55550000
#define ICC_CNN_CONVOLUTION_LAYER    0x00001111
#define ICC_CNN_SUBSAMPLING_LAYER    0x00002222
#define ICC_CNN_FULLCONNECT_LAYER    0x00003333

#define ICC_IS_CNN_LAYER(layer)                                          \
    (((layer) != NULL) && ((((CvCNNLayer*)(layer))->flags & CC_MAGIC_MASK)\
        == ICC_CNN_LAYER))

#define ICC_IS_CNN_CONVOLUTION_LAYER(layer)                              \
    ((ICC_IS_CNN_LAYER(layer)) && (((CvCNNLayer*) (layer))->flags       \
        & ~CC_MAGIC_MASK) == ICC_CNN_CONVOLUTION_LAYER)

#define ICC_IS_CNN_SUBSAMPLING_LAYER(layer)                              \
    ((ICC_IS_CNN_LAYER(layer)) && (((CvCNNLayer*) (layer))->flags       \
        & ~CC_MAGIC_MASK) == ICC_CNN_SUBSAMPLING_LAYER)

#define ICC_IS_CNN_FULLCONNECT_LAYER(layer)                              \
    ((ICC_IS_CNN_LAYER(layer)) && (((CvCNNLayer*) (layer))->flags       \
        & ~CC_MAGIC_MASK) == ICC_CNN_FULLCONNECT_LAYER)

typedef void (C_CDECL *CvCNNLayerForward)
    (CvCNNLayer* layer, const img_t* input, img_t* output);

typedef void (C_CDECL *CvCNNLayerBackward)
    (CvCNNLayer* layer, int t, const img_t* X, const img_t* dE_dY, img_t* dE_dX);

typedef void (C_CDECL *CvCNNLayerRelease)
    (CvCNNLayer** layer);

typedef void (C_CDECL *CvCNNetworkAddLayer)
    (CvCNNetwork* network, CvCNNLayer* layer);

typedef void (C_CDECL *CvCNNetworkRelease)
    (CvCNNetwork** network);

#define CC_CNN_LAYER_FIELDS()           \
    /* Indicator of the layer's type */ \
    int flags;                          \
                                        \
    /* Number of input images */        \
    int n_input_planes;                 \
    /* Height of each input image */    \
    int input_height;                   \
    /* Width of each input image */     \
    int input_width;                    \
                                        \
    /* Number of output images */       \
    int n_output_planes;                \
    /* Height of each output image */   \
    int output_height;                  \
    /* Width of each output image */    \
    int output_width;                   \
                                        \
    /* Learning rate at the first iteration */                      \
    float init_learn_rate;                                          \
    /* Dynamics of learning rate decreasing */                      \
    int learn_rate_decrease_type;                                   \
    /* Trainable weights of the layer (including bias) */           \
    /* i-th row is a set of weights of the i-th output plane */     \
    img_t* weights;                                                 \
                                                                    \
    CvCNNLayerForward  forward;                                     \
    CvCNNLayerBackward backward;                                    \
    CvCNNLayerRelease  release;                                     \
    /* Pointers to the previous and next layers in the network */   \
    CvCNNLayer* prev_layer;                                         \
    CvCNNLayer* next_layer

typedef struct CvCNNLayer
{
    CC_CNN_LAYER_FIELDS();
}CvCNNLayer;

typedef struct CvCNNConvolutionLayer
{
    CC_CNN_LAYER_FIELDS();
    // Kernel size (height and width) for convolution.
    int K;
    // connections matrix, (i,j)-th element is 1 iff there is a connection between
    // i-th plane of the current layer and j-th plane of the previous layer;
    // (i,j)-th element is equal to 0 otherwise
    img_t *connect_mask;
    // value of the learning rate for updating weights at the first iteration
}CvCNNConvolutionLayer;

typedef struct CvCNNSubSamplingLayer
{
    CC_CNN_LAYER_FIELDS();
    // ratio between the heights (or widths - ratios are supposed to be equal)
    // of the input and output planes
    int sub_samp_scale;
    // amplitude of sigmoid activation function
    float a;
    // scale parameter of sigmoid activation function
    float s;
    // exp2ssumWX = exp(2<s>*(bias+w*(x1+...+x4))), where x1,...x4 are some elements of X
    // - is the vector used in computing of the activation function in backward
    img_t* exp2ssumWX;
    // (x1+x2+x3+x4), where x1,...x4 are some elements of X
    // - is the vector used in computing of the activation function in backward
    img_t* sumX;
}CvCNNSubSamplingLayer;

// Structure of the last layer.
typedef struct CvCNNFullConnectLayer
{
    CC_CNN_LAYER_FIELDS();
    // amplitude of sigmoid activation function
    float a;
    // scale parameter of sigmoid activation function
    float s;
    // exp2ssumWX = exp(2*<s>*(W*X)) - is the vector used in computing of the 
    // activation function and it's derivative by the formulae
    // activ.func. = <a>(exp(2<s>WX)-1)/(exp(2<s>WX)+1) == <a> - 2<a>/(<exp2ssumWX> + 1)
    // (activ.func.)' = 4<a><s>exp(2<s>WX)/(exp(2<s>WX)+1)^2
    img_t* exp2ssumWX;
}CvCNNFullConnectLayer;

typedef struct CvCNNetwork
{
    int n_layers;
    CvCNNLayer* layers;
    CvCNNetworkAddLayer add_layer;
    CvCNNetworkRelease release;
}CvCNNetwork;

typedef struct CvCNNStatModel
{
    CC_STAT_MODEL_FIELDS();
    CvCNNetwork* network;
    // etalons are allocated as rows, the i-th etalon has label cls_labeles[i]
    img_t* etalons;
    // classes labels
    img_t* cls_labels;
}CvCNNStatModel;

typedef struct CvCNNStatModelParams
{
    CC_STAT_MODEL_PARAM_FIELDS();
    // network must be created by the functions cvCreateCNNetwork and <add_layer>
    CvCNNetwork* network;
    img_t* etalons;
    // termination criteria
    int max_iter;
    int start_iter;
    int grad_estim_type;
}CvCNNStatModelParams;

CVAPI(CvCNNLayer*) cvCreateCNNConvolutionLayer(
    int n_input_planes, int input_height, int input_width,
    int n_output_planes, int K,
    float init_learn_rate, int learn_rate_decrease_type,
    img_t* connect_mask CC_DEFAULT(0), img_t* weights CC_DEFAULT(0));

CVAPI(CvCNNLayer*) cvCreateCNNSubSamplingLayer(
    int n_input_planes, int input_height, int input_width,
    int sub_samp_scale, float a, float s,
    float init_learn_rate, int learn_rate_decrease_type, img_t* weights CC_DEFAULT(0));

CVAPI(CvCNNLayer*) cvCreateCNNFullConnectLayer(
    int n_inputs, int n_outputs, float a, float s,
    float init_learn_rate, int learning_type, img_t* weights CC_DEFAULT(0));

CVAPI(CvCNNetwork*) cvCreateCNNetwork(CvCNNLayer* first_layer);

CVAPI(CvStatModel*) cvTrainCNNClassifier(
            const img_t* train_data, int tflag,
            const img_t* responses,
            const CvStatModelParams* params,
            const img_t* CC_DEFAULT(0),
            const img_t* sample_idx CC_DEFAULT(0),
            const img_t* CC_DEFAULT(0), const img_t* CC_DEFAULT(0));

/****************************************************************************************\
*                               Estimate classifiers algorithms                          *
\****************************************************************************************/
typedef const img_t* (C_CDECL *CvStatModelEstimateGetMat)
                    (const CvStatModel* estimateModel);

typedef int (C_CDECL *CvStatModelEstimateNextStep)
                    (CvStatModel* estimateModel);

typedef void (C_CDECL *CvStatModelEstimateCheckClassifier)
                    (CvStatModel* estimateModel,
                const CvStatModel* model, 
                const img_t*       features, 
                      int          sample_t_flag,
                const img_t*       responses);

typedef void (C_CDECL *CvStatModelEstimateCheckClassifierEasy)
                    (CvStatModel* estimateModel,
                const CvStatModel* model);

typedef float (C_CDECL *CvStatModelEstimateGetCurrentResult)
                    (const CvStatModel* estimateModel,
                            float*       correlation);

typedef void (C_CDECL *CvStatModelEstimateReset)
                    (CvStatModel* estimateModel);

//-------------------------------- Cross-validation --------------------------------------
#define CC_CROSS_VALIDATION_ESTIMATE_CLASSIFIER_PARAM_FIELDS()    \
    CC_STAT_MODEL_PARAM_FIELDS();                                 \
    int     k_fold;                                               \
    int     is_regression;                                        \
    CRNG*  rng

typedef struct CvCrossValidationParams
{
    CC_CROSS_VALIDATION_ESTIMATE_CLASSIFIER_PARAM_FIELDS();
} CvCrossValidationParams;

#define CC_CROSS_VALIDATION_ESTIMATE_CLASSIFIER_FIELDS()    \
    CvStatModelEstimateGetMat               getTrainIdxMat; \
    CvStatModelEstimateGetMat               getCheckIdxMat; \
    CvStatModelEstimateNextStep             nextStep;       \
    CvStatModelEstimateCheckClassifier      check;          \
    CvStatModelEstimateGetCurrentResult     getResult;      \
    CvStatModelEstimateReset                reset;          \
    int     is_regression;                                  \
    int     folds_all;                                      \
    int     samples_all;                                    \
    int*    sampleIdxAll;                                   \
    int*    folds;                                          \
    int     max_fold_size;                                  \
    int         current_fold;                               \
    int         is_checked;                                 \
    img_t*      sampleIdxTrain;                             \
    img_t*      sampleIdxEval;                              \
    img_t*      predict_results;                            \
    int     correct_results;                                \
    int     all_results;                                    \
    double  sq_error;                                       \
    double  sum_correct;                                    \
    double  sum_predict;                                    \
    double  sum_cc;                                         \
    double  sum_pp;                                         \
    double  sum_cp

typedef struct CvCrossValidationModel
{
    CC_STAT_MODEL_FIELDS();
    CC_CROSS_VALIDATION_ESTIMATE_CLASSIFIER_FIELDS();
} CvCrossValidationModel;

CVAPI(CvStatModel*) 
cvCreateCrossValidationEstimateModel
           (int                samples_all,
       const CvStatModelParams* estimateParams CC_DEFAULT(0),
       const img_t*             sampleIdx CC_DEFAULT(0));

CVAPI(float) 
cvCrossValidation(const img_t*             trueData,
                         int                tflag,
                   const img_t*             trueClasses,
                         CvStatModel*     (*createClassifier)(const img_t*,
                                                                     int,
                                                               const img_t*,
                                                               const CvStatModelParams*,
                                                               const img_t*,
                                                               const img_t*,
                                                               const img_t*,
                                                               const img_t*),
                   const CvStatModelParams* estimateParams CC_DEFAULT(0),
                   const CvStatModelParams* trainParams CC_DEFAULT(0),
                   const img_t*             compIdx CC_DEFAULT(0),
                   const img_t*             sampleIdx CC_DEFAULT(0),
                         CvStatModel**      pCrValModel CC_DEFAULT(0),
                   const img_t*             typeMask CC_DEFAULT(0),
                   const img_t*             missedMeasurementMask CC_DEFAULT(0));
#endif

/****************************************************************************************\
*                           Auxilary functions declarations                              *
\****************************************************************************************/

/* Generates <sample> from multivariate normal distribution, where <mean> - is an
   average row vector, <cov> - symmetric covariation matrix */
CVAPI(void) cvRandMVNormal(img_t* mean, img_t* cov, img_t* sample,
                           CRNG* rng CC_DEFAULT(0));

/* Generates sample from gaussian mixture distribution */
CVAPI(void) cvRandGaussMixture(img_t* means[],
                               img_t* covs[],
                               float weights[],
                               int clsnum,
                               img_t* sample,
                               img_t* sampClasses CC_DEFAULT(0));

#define CC_TS_CONCENTRIC_SPHERES 0

/* creates test set */
CVAPI(void) cvCreateTestSet(int type, img_t** samples,
                 int num_samples,
                 int num_features,
                 img_t** responses,
                 int num_classes, ...);

/* Aij <- Aji for i > j if lower_to_upper != 0
              for i < j if lower_to_upper = 0 */
CVAPI(void) cvCompleteSymm(img_t* matrix, int lower_to_upper);

#ifdef __cplusplus
}
#endif

#endif /*__ML_H__*/

