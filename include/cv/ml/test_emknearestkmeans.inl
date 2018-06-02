/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "test_precomp.hpp"

using namespace std;
using namespace cv;

static
void defaultDistribs(img_t& means, vector<img_t>& covs, int type=CC_32FC1)
{
    float mp0[] = {0.0f, 0.0f}, cp0[] = {0.67f, 0.0f, 0.0f, 0.67f};
    float mp1[] = {5.0f, 0.0f}, cp1[] = {1.0f, 0.0f, 0.0f, 1.0f};
    float mp2[] = {1.0f, 5.0f}, cp2[] = {1.0f, 0.0f, 0.0f, 1.0f};
    means cvSetMat(3, 2, type);
    img_t m0(1, 2, CC_32FC1, mp0), c0(2, 2, CC_32FC1, cp0);
    img_t m1(1, 2, CC_32FC1, mp1), c1(2, 2, CC_32FC1, cp1);
    img_t m2(1, 2, CC_32FC1, mp2), c2(2, 2, CC_32FC1, cp2);
    means.resize(3), covs.resize(3);

    img_t mr0 = means.row(0);
    m0.convertTo(mr0, type);
    c0.convertTo(covs[0], type);

    img_t mr1 = means.row(1);
    m1.convertTo(mr1, type);
    c1.convertTo(covs[1], type);

    img_t mr2 = means.row(2);
    m2.convertTo(mr2, type);
    c2.convertTo(covs[2], type);
}

// generate points sets by normal distributions
static
void generateData(img_t& data, img_t& labels, const vector<int>& sizes, const img_t& _means, const vector<img_t>& covs, int dataType, int labelType)
{
    vector<int>::const_iterator sit = sizes.begin();
    int total = 0;
    for(; sit != sizes.end(); ++sit)
        total += *sit;
    CC_Assert(_means->rows == (int)sizes.size() && covs.size() == sizes.size());
    CC_Assert(!data.empty() && data->rows == total);
    CC_Assert(data->tid == dataType);

    labels cvSetMat(data->rows, 1, labelType);

    randn(data, Scalar::all(-1.0), Scalar::all(1.0));
    vector<img_t> means(sizes.size());
    for(int i = 0; i < _means->rows; i++)
        means[i] = _means.row(i);
    vector<img_t>::const_iterator mit = means.begin(), cit = covs.begin();
    int bi, ei = 0;
    sit = sizes.begin();
    for(int p = 0, l = 0; sit != sizes.end(); ++sit, ++mit, ++cit, l++)
    {
        bi = ei;
        ei = bi + *sit;
        assert(mit->rows == 1 && mit->cols == data->cols);
        assert(cit->rows == data->cols && cit->cols == data->cols);
        for(int i = bi; i < ei; i++, p++)
        {
            img_t r = data.row(i);
            r =  r * (*cit) + *mit;
            if(labelType == CC_32FC1)
                labels.at<float>(p, 0) = (float)l;
            else if(labelType == CC_32SC1)
                labels.at<int>(p, 0) = l;
            else
            {
                CC_DbgAssert(0);
            }
        }
    }
}

static
int maxIdx(const vector<int>& count)
{
    int idx = -1;
    int maxVal = -1;
    vector<int>::const_iterator it = count.begin();
    for(int i = 0; it != count.end(); ++it, i++)
    {
        if(*it > maxVal)
        {
            maxVal = *it;
            idx = i;
        }
    }
    assert(idx >= 0);
    return idx;
}

static
bool getLabelsMap(const img_t& labels, const vector<int>& sizes, vector<int>& labelsMap, bool checkClusterUniq=true)
{
    size_t total = 0, nclusters = sizes.size();
    for(size_t i = 0; i < sizes.size(); i++)
        total += sizes[i];

    assert(!labels.empty());
    assert(labels CC_MAT_TOTAL() == total && (labels->cols == 1 || labels->rows == 1));
    assert(labels->tid == CC_32SC1 || labels->tid == CC_32FC1);

    bool isFlt = labels->tid == CC_32FC1;

    labelsMap.resize(nclusters);

    vector<bool> buzy(nclusters, false);
    int startIndex = 0;
    for(size_t clusterIndex = 0; clusterIndex < sizes.size(); clusterIndex++)
    {
        vector<int> count(nclusters, 0);
        for(int i = startIndex; i < startIndex + sizes[clusterIndex]; i++)
        {
            int lbl = isFlt ? (int)labels.at<float>(i) : labels.at<int>(i);
            CC_Assert(lbl < (int)nclusters);
            count[lbl]++;
            CC_Assert(count[lbl] < (int)total);
        }
        startIndex += sizes[clusterIndex];

        int cls = maxIdx(count);
        CC_Assert(!checkClusterUniq || !buzy[cls]);

        labelsMap[clusterIndex] = cls;

        buzy[cls] = true;
    }

    if(checkClusterUniq)
    {
        for(size_t i = 0; i < buzy.size(); i++)
            if(!buzy[i])
                return false;
    }

    return true;
}

static
bool calcErr(const img_t& labels, const img_t& origLabels, const vector<int>& sizes, float& err, bool labelsEquivalent = true, bool checkClusterUniq=true)
{
    err = 0;
    CC_Assert(!labels.empty() && !origLabels.empty());
    CC_Assert(labels->rows == 1 || labels->cols == 1);
    CC_Assert(origLabels->rows == 1 || origLabels->cols == 1);
    CC_Assert(labels CC_MAT_TOTAL() == origLabels CC_MAT_TOTAL());
    CC_Assert(labels->tid == CC_32SC1 || labels->tid == CC_32FC1);
    CC_Assert(origLabels->tid == labels->tid);

    vector<int> labelsMap;
    bool isFlt = labels->tid == CC_32FC1;
    if(!labelsEquivalent)
    {
        if(!getLabelsMap(labels, sizes, labelsMap, checkClusterUniq))
            return false;

        for(int i = 0; i < labels->rows; i++)
            if(isFlt)
                err += labels.at<float>(i) != labelsMap[(int)origLabels.at<float>(i)] ? 1.f : 0.f;
            else
                err += labels.at<int>(i) != labelsMap[origLabels.at<int>(i)] ? 1.f : 0.f;
    }
    else
    {
        for(int i = 0; i < labels->rows; i++)
            if(isFlt)
                err += labels.at<float>(i) != origLabels.at<float>(i) ? 1.f : 0.f;
            else
                err += labels.at<int>(i) != origLabels.at<int>(i) ? 1.f : 0.f;
    }
    err /= (float)labels->rows;
    return true;
}

//--------------------------------------------------------------------------------------------
class CC_KMeansTest : public cvtest::BaseTest {
public:
    CC_KMeansTest() {}
protected:
    virtual void run(int start_from);
};

void CC_KMeansTest::run(int /*start_from*/)
{
    const int iters = 100;
    int sizesArr[] = { 5000, 7000, 8000 };
    int pointsCount = sizesArr[0]+ sizesArr[1] + sizesArr[2];

    img_t data(pointsCount, 2, CC_32FC1), labels;
    vector<int> sizes(sizesArr, sizesArr + sizeof(sizesArr) / sizeof(sizesArr[0]));
    img_t means;
    vector<img_t> covs;
    defaultDistribs(means, covs);
    generateData(data, labels, sizes, means, covs, CC_32FC1, CC_32SC1);

    int code = cvtest::TS::OK;
    float err;
    img_t bestLabels;
    // 1. flag==KMEANS_PP_CENTERS
    kmeans(data, 3, bestLabels, TermCriteria(TermCriteria::COUNT, iters, 0.0), 0, KMEANS_PP_CENTERS, NULL);
    if(!calcErr(bestLabels, labels, sizes, err , false))
    {
        ts->printf(cvtest::TS::LOG, "Bad output labels if flag==KMEANS_PP_CENTERS.\n");
        code = cvtest::TS::FAIL_INVALID_OUTPUT;
    }
    else if(err > 0.01f)
    {
        ts->printf(cvtest::TS::LOG, "Bad accuracy (%f) if flag==KMEANS_PP_CENTERS.\n", err);
        code = cvtest::TS::FAIL_BAD_ACCURACY;
    }

    // 2. flag==KMEANS_RANDOM_CENTERS
    kmeans(data, 3, bestLabels, TermCriteria(TermCriteria::COUNT, iters, 0.0), 0, KMEANS_RANDOM_CENTERS, NULL);
    if(!calcErr(bestLabels, labels, sizes, err, false))
    {
        ts->printf(cvtest::TS::LOG, "Bad output labels if flag==KMEANS_RANDOM_CENTERS.\n");
        code = cvtest::TS::FAIL_INVALID_OUTPUT;
    }
    else if(err > 0.01f)
    {
        ts->printf(cvtest::TS::LOG, "Bad accuracy (%f) if flag==KMEANS_RANDOM_CENTERS.\n", err);
        code = cvtest::TS::FAIL_BAD_ACCURACY;
    }

    // 3. flag==KMEANS_USE_INITIAL_LABELS
    labels.copyTo(bestLabels);
    RNG rng;
    for(int i = 0; i < 0.5f * pointsCount; i++)
        bestLabels.at<int>(rng.next() % pointsCount, 0) = rng.next() % 3;
    kmeans(data, 3, bestLabels, TermCriteria(TermCriteria::COUNT, iters, 0.0), 0, KMEANS_USE_INITIAL_LABELS, NULL);
    if(!calcErr(bestLabels, labels, sizes, err, false))
    {
        ts->printf(cvtest::TS::LOG, "Bad output labels if flag==KMEANS_USE_INITIAL_LABELS.\n");
        code = cvtest::TS::FAIL_INVALID_OUTPUT;
    }
    else if(err > 0.01f)
    {
        ts->printf(cvtest::TS::LOG, "Bad accuracy (%f) if flag==KMEANS_USE_INITIAL_LABELS.\n", err);
        code = cvtest::TS::FAIL_BAD_ACCURACY;
    }

    ts->set_failed_test_info(code);
}

//--------------------------------------------------------------------------------------------
class CC_KNearestTest : public cvtest::BaseTest {
public:
    CC_KNearestTest() {}
protected:
    virtual void run(int start_from);
};

void CC_KNearestTest::run(int /*start_from*/)
{
    int sizesArr[] = { 500, 700, 800 };
    int pointsCount = sizesArr[0]+ sizesArr[1] + sizesArr[2];

    // train data
    img_t trainData(pointsCount, 2, CC_32FC1), trainLabels;
    vector<int> sizes(sizesArr, sizesArr + sizeof(sizesArr) / sizeof(sizesArr[0]));
    img_t means;
    vector<img_t> covs;
    defaultDistribs(means, covs);
    generateData(trainData, trainLabels, sizes, means, covs, CC_32FC1, CC_32FC1);

    // test data
    img_t testData(pointsCount, 2, CC_32FC1), testLabels, bestLabels;
    generateData(testData, testLabels, sizes, means, covs, CC_32FC1, CC_32FC1);

    int code = cvtest::TS::OK;
    KNearest knearest;
    knearest.train(trainData, trainLabels);
    knearest.find_nearest(testData, 4, &bestLabels);
    float err;
    if(!calcErr(bestLabels, testLabels, sizes, err, true))
    {
        ts->printf(cvtest::TS::LOG, "Bad output labels.\n");
        code = cvtest::TS::FAIL_INVALID_OUTPUT;
    }
    else if(err > 0.01f)
    {
        ts->printf(cvtest::TS::LOG, "Bad accuracy (%f) on test data.\n", err);
        code = cvtest::TS::FAIL_BAD_ACCURACY;
    }
    ts->set_failed_test_info(code);
}

class EM_Params
{
public:
    EM_Params(int _nclusters=10, int _covMatType=EM::COV_MAT_DIAGONAL, int _startStep=EM::START_AUTO_STEP,
           const TermCriteria& _termCrit=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, FLT_EPSILON),
           const img_t* _probs=0, const img_t* _weights=0,
           const img_t* _means=0, const std::vector<img_t>* _covs=0)
        : nclusters(_nclusters), covMatType(_covMatType), startStep(_startStep),
        probs(_probs), weights(_weights), means(_means), covs(_covs), termCrit(_termCrit)
    {}

    int nclusters;
    int covMatType;
    int startStep;

    // all 4 following matrices should have type CC_32FC1
    const img_t* probs;
    const img_t* weights;
    const img_t* means;
    const std::vector<img_t>* covs;

    TermCriteria termCrit;
};

//--------------------------------------------------------------------------------------------
class CC_EMTest : public cvtest::BaseTest
{
public:
    CC_EMTest() {}
protected:
    virtual void run(int start_from);
    int runCase(int caseIndex, const EM_Params& params,
                  const img_t& trainData, const img_t& trainLabels,
                  const img_t& testData, const img_t& testLabels,
                  const vector<int>& sizes);
};

int CC_EMTest::runCase(int caseIndex, const EM_Params& params,
                        const img_t& trainData, const img_t& trainLabels,
                        const img_t& testData, const img_t& testLabels,
                        const vector<int>& sizes)
{
    int code = cvtest::TS::OK;

    img_t labels;
    float err;

    EM em(params.nclusters, params.covMatType, params.termCrit);
    if(params.startStep == EM::START_AUTO_STEP)
        em.train(trainData, NULL, labels);
    else if(params.startStep == EM::START_E_STEP)
        em.trainE(trainData, *params.means, *params.covs, *params.weights, NULL, labels);
    else if(params.startStep == EM::START_M_STEP)
        em.trainM(trainData, *params.probs, NULL, labels);

    // check train error
    if(!calcErr(labels, trainLabels, sizes, err , false, false))
    {
        ts->printf(cvtest::TS::LOG, "Case index %i : Bad output labels.\n", caseIndex);
        code = cvtest::TS::FAIL_INVALID_OUTPUT;
    }
    else if(err > 0.008f)
    {
        ts->printf(cvtest::TS::LOG, "Case index %i : Bad accuracy (%f) on train data.\n", caseIndex, err);
        code = cvtest::TS::FAIL_BAD_ACCURACY;
    }

    // check test error
    labels cvSetMat(testData->rows, 1, CC_32SC1);
    for(int i = 0; i < testData->rows; i++)
    {
        img_t sample = testData.row(i);
        img_t probs;
        labels.at<int>(i) = static_cast<int>(em.predict(sample, probs)[1]);
    }
    if(!calcErr(labels, testLabels, sizes, err, false, false))
    {
        ts->printf(cvtest::TS::LOG, "Case index %i : Bad output labels.\n", caseIndex);
        code = cvtest::TS::FAIL_INVALID_OUTPUT;
    }
    else if(err > 0.008f)
    {
        ts->printf(cvtest::TS::LOG, "Case index %i : Bad accuracy (%f) on test data.\n", caseIndex, err);
        code = cvtest::TS::FAIL_BAD_ACCURACY;
    }

    return code;
}

void CC_EMTest::run(int /*start_from*/)
{
    int sizesArr[] = { 500, 700, 800 };
    int pointsCount = sizesArr[0]+ sizesArr[1] + sizesArr[2];

    // Points distribution
    img_t means;
    vector<img_t> covs;
    defaultDistribs(means, covs, CC_64FC1);

    // train data
    img_t trainData(pointsCount, 2, CC_64FC1), trainLabels;
    vector<int> sizes(sizesArr, sizesArr + sizeof(sizesArr) / sizeof(sizesArr[0]));
    generateData(trainData, trainLabels, sizes, means, covs, CC_64FC1, CC_32SC1);

    // test data
    img_t testData(pointsCount, 2, CC_64FC1), testLabels;
    generateData(testData, testLabels, sizes, means, covs, CC_64FC1, CC_32SC1);

    EM_Params params;
    params.nclusters = 3;
    img_t probs(trainData->rows, params.nclusters, CC_64FC1, Scalar(1));
    params.probs = &probs;
    img_t weights(1, params.nclusters, CC_64FC1, Scalar(1));
    params.weights = &weights;
    params.means = &means;
    params.covs = &covs;

    int code = cvtest::TS::OK;
    int caseIndex = 0;
    {
        params.startStep = EM::START_AUTO_STEP;
        params.covMatType = EM::COV_MAT_GENERIC;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }
    {
        params.startStep = EM::START_AUTO_STEP;
        params.covMatType = EM::COV_MAT_DIAGONAL;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }
    {
        params.startStep = EM::START_AUTO_STEP;
        params.covMatType = EM::COV_MAT_SPHERICAL;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }
    {
        params.startStep = EM::START_M_STEP;
        params.covMatType = EM::COV_MAT_GENERIC;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }
    {
        params.startStep = EM::START_M_STEP;
        params.covMatType = EM::COV_MAT_DIAGONAL;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }
    {
        params.startStep = EM::START_M_STEP;
        params.covMatType = EM::COV_MAT_SPHERICAL;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }
    {
        params.startStep = EM::START_E_STEP;
        params.covMatType = EM::COV_MAT_GENERIC;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }
    {
        params.startStep = EM::START_E_STEP;
        params.covMatType = EM::COV_MAT_DIAGONAL;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }
    {
        params.startStep = EM::START_E_STEP;
        params.covMatType = EM::COV_MAT_SPHERICAL;
        int currCode = runCase(caseIndex++, params, trainData, trainLabels, testData, testLabels, sizes);
        code = currCode == cvtest::TS::OK ? code : currCode;
    }

    ts->set_failed_test_info(code);
}

class CC_EMTest_SaveLoad : public cvtest::BaseTest {
public:
    CC_EMTest_SaveLoad() {}
protected:
    virtual void run(int /*start_from*/)
    {
        int code = cvtest::TS::OK;
        const int nclusters = 2;
        EM em(nclusters);

        img_t samples = img_t(3,1,CC_64FC1);
        samples.at<double>(0,0) = 1;
        samples.at<double>(1,0) = 2;
        samples.at<double>(2,0) = 3;

        img_t labels;

        em.train(samples, labels);

        img_t firstResult(samples->rows, 1, CC_32SC1);
        for(int i = 0; i < samples->rows; i++)
            firstResult.at<int>(i) = static_cast<int>(em.predict(samples.row(i))[1]);

        // Write out
        string filename = tempfile(".xml");
        {
            FileStorage fs = FileStorage(filename, FileStorage::WRITE);
            try
            {
                fs << "em" << "{";
                em.write(fs);
                fs << "}";
            }
            catch(...)
            {
                ts->printf(cvtest::TS::LOG, "Crash in write method.\n");
                ts->set_failed_test_info(cvtest::TS::FAIL_EXCEPTION);
            }
        }

        em.clear();

        // Read in
        {
            FileStorage fs = FileStorage(filename, FileStorage::READ);
            CC_Assert(fs.isOpened());
            FileNode fn = fs["em"];
            try
            {
                em.read(fn);
            }
            catch(...)
            {
                ts->printf(cvtest::TS::LOG, "Crash in read method.\n");
                ts->set_failed_test_info(cvtest::TS::FAIL_EXCEPTION);
            }
        }

        remove(filename.c_str());

        int errCaseCount = 0;
        for(int i = 0; i < samples->rows; i++)
            errCaseCount = std::abs(em.predict(samples.row(i))[1] - firstResult.at<int>(i)) < FLT_EPSILON ? 0 : 1;

        if(errCaseCount > 0)
        {
            ts->printf(cvtest::TS::LOG, "Different prediction results before writeing and after reading (errCaseCount=%d).\n", errCaseCount);
            code = cvtest::TS::FAIL_BAD_ACCURACY;
        }

        ts->set_failed_test_info(code);
    }
};

class CC_EMTest_Classification : public cvtest::BaseTest
{
public:
    CC_EMTest_Classification() {}
protected:
    virtual void run(int)
    {
        // This test classifies spam by the following way:
        // 1. estimates distributions of "spam" / "not spam"
        // 2. predict classID using Bayes classifier for estimated distributions.

        CvMLData data;
        string dataFilename = string(ts->get_data_path()) + "spambase.data";

        if(data.read_csv(dataFilename.c_str()) != 0)
        {
            ts->printf(cvtest::TS::LOG, "File with spambase dataset cann't be read.\n");
            ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_TEST_DATA);
        }

        img_t values = data.get_values();
        CC_Assert(values->cols == 58);
        int responseIndex = 57;

        img_t samples = values.colRange(0, responseIndex);
        img_t responses = values.col(responseIndex);

        vector<int> trainSamplesMask(samples->rows, 0);
        int trainSamplesCount = (int)(0.5f * samples->rows);
        for(int i = 0; i < trainSamplesCount; i++)
            trainSamplesMask[i] = 1;
        RNG rng(0);
        for(size_t i = 0; i < trainSamplesMask.size(); i++)
        {
            int i1 = rng(static_cast<unsigned>(trainSamplesMask.size()));
            int i2 = rng(static_cast<unsigned>(trainSamplesMask.size()));
            T_SWAP(_Tp, trainSamplesMask[i1], trainSamplesMask[i2]);
        }

        EM model0(3), model1(3);
        img_t samples0, samples1;
        for(int i = 0; i < samples->rows; i++)
        {
            if(trainSamplesMask[i])
            {
                img_t sample = samples.row(i);
                int resp = (int)responses.at<float>(i);
                if(resp == 0)
                    samples0.push_back(sample);
                else
                    samples1.push_back(sample);
            }
        }
        model0.train(samples0);
        model1.train(samples1);

        img_t trainConfusionMat(2, 2, CC_32SC1, Scalar(0)),
            testConfusionMat(2, 2, CC_32SC1, Scalar(0));
        const double lambda = 1.;
        for(int i = 0; i < samples->rows; i++)
        {
            img_t sample = samples.row(i);
            double sampleLogLikelihoods0 = model0.predict(sample)[0];
            double sampleLogLikelihoods1 = model1.predict(sample)[0];

            int classID = sampleLogLikelihoods0 >= lambda * sampleLogLikelihoods1 ? 0 : 1;

            if(trainSamplesMask[i])
                trainConfusionMat.at<int>((int)responses.at<float>(i), classID)++;
            else
                testConfusionMat.at<int>((int)responses.at<float>(i), classID)++;
        }
//        std::cout << trainConfusionMat << std::endl;
//        std::cout << testConfusionMat << std::endl;

        double trainError = (double)(trainConfusionMat.at<int>(1,0) + trainConfusionMat.at<int>(0,1)) / trainSamplesCount;
        double testError = (double)(testConfusionMat.at<int>(1,0) + testConfusionMat.at<int>(0,1)) / (samples->rows - trainSamplesCount);
        const double maxTrainError = 0.23;
        const double maxTestError = 0.26;

        int code = cvtest::TS::OK;
        if(trainError > maxTrainError)
        {
            ts->printf(cvtest::TS::LOG, "Too large train classification error (calc = %f, valid=%f).\n", trainError, maxTrainError);
            code = cvtest::TS::FAIL_INVALID_TEST_DATA;
        }
        if(testError > maxTestError)
        {
            ts->printf(cvtest::TS::LOG, "Too large test classification error (calc = %f, valid=%f).\n", testError, maxTestError);
            code = cvtest::TS::FAIL_INVALID_TEST_DATA;
        }

        ts->set_failed_test_info(code);
    }
};

TEST(ML_KMeans, accuracy) { CC_KMeansTest test; test.safe_run(); }
TEST(ML_KNearest, accuracy) { CC_KNearestTest test; test.safe_run(); }
TEST(ML_EM, accuracy) { CC_EMTest test; test.safe_run(); }
TEST(ML_EM, save_load) { CC_EMTest_SaveLoad test; test.safe_run(); }
TEST(ML_EM, classification) { CC_EMTest_Classification test; test.safe_run(); }
