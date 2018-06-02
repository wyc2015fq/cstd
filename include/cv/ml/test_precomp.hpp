#ifdef __GNUC__
#  pragma GCC diagnostic ignored "-Wmissing-declarations"
#  if defined __clang__ || defined __APPLE__
#    pragma GCC diagnostic ignored "-Wmissing-prototypes"
#    pragma GCC diagnostic ignored "-Wextra"
#  endif
#endif

#ifndef __OPENCC_TEST_PRECOMP_HPP__
#define __OPENCC_TEST_PRECOMP_HPP__

#include "opencv2/ts/ts.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/core/core_c.h"
#include <iostream>
#include <map>

#define CC_NBAYES   "nbayes"
#define CC_KNEAREST "knearest"
#define CC_SVM      "svm"
#define CC_EM       "em"
#define CC_ANN      "ann"
#define CC_DTREE    "dtree"
#define CC_BOOST    "boost"
#define CC_RTREES   "rtrees"
#define CC_ERTREES  "ertrees"

class CC_MLBaseTest : public cvtest::BaseTest
{
public:
    CC_MLBaseTest(const char* _modelName);
    virtual ~CC_MLBaseTest();
protected:
    virtual int read_params(CvFileStorage* fs);
    virtual void run(int startFrom);
    virtual int prepare_test_case(int testCaseIdx);
    virtual std::string& get_validation_filename();
    virtual int run_test_case(int testCaseIdx) = 0;
    virtual int validate_test_results(int testCaseIdx) = 0;

    int train(int testCaseIdx);
    float get_error(int testCaseIdx, int type, std::vector<float> *resp = 0);
    void save(const char* filename);
    void load(const char* filename);

    CvMLData data;
    std::string modelName, validationFN;
    std::vector<std::string> dataSetNames;
    FileStorage validationFS;

    // MLL models
    CvNormalBayesClassifier* nbayes;
    CvKNearest* knearest;
    CvSVM* svm;
    CvANN_MLP* ann;
    CvDTree* dtree;
    CvBoost* boost;
    CvRTrees* rtrees;
    CvERTrees* ertrees;

    std::map<int, int> cls_map;

    int64 initSeed;
};

class CC_AMLTest : public CC_MLBaseTest
{
public:
    CC_AMLTest(const char* _modelName);
protected:
    virtual int run_test_case(int testCaseIdx);
    virtual int validate_test_results(int testCaseIdx);
};

class CC_SLMLTest : public CC_MLBaseTest
{
public:
    CC_SLMLTest(const char* _modelName);
protected:
    virtual int run_test_case(int testCaseIdx);
    virtual int validate_test_results(int testCaseIdx);

    std::vector<float> test_resps1, test_resps2; // predicted responses for test data
    std::string fname1, fname2;
};

#endif
