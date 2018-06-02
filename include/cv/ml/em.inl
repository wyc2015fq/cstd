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
// Copyright(C) 2000, Intel Corporation, all rights reserved.
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
//(including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort(including negligence or otherwise) arising in any way out of
// the use of this software, even ifadvised of the possibility of such damage.
//
//M*/



namespace cv
{

const double minEigenValue = DBL_EPSILON;

///////////////////////////////////////////////////////////////////////////////////////////////////////

EM::EM(int _nclusters, int _covMatType, const TermCriteria& _termCrit)
{
    nclusters = _nclusters;
    covMatType = _covMatType;
    maxIters = (_termCrit.type & TermCriteria::MAX_ITER) ? _termCrit.maxCount : DEFAULT_MAX_ITERS;
    epsilon = (_termCrit.type & TermCriteria::EPS) ? _termCrit.epsilon : 0;
}

EM::~EM()
{
    //clear();
}

void EM::clear()
{
    trainSamples.release();
    trainProbs.release();
    trainLogLikelihoods.release();
    trainLabels.release();

    weights.release();
    means.release();
    covs.clear();

    covsEigenValues.clear();
    invCovsEigenValues.clear();
    covsRotateMats.clear();

    logWeightDivDet.release();
}


bool EM::train(const img_t* samples,
               img_t* logLikelihoods,
               img_t* labels,
               img_t* probs)
{
    img_t samplesMat = samples;
    setTrainData(START_AUTO_STEP, samplesMat, 0, 0, 0, 0);
    return doTrain(START_AUTO_STEP, logLikelihoods, labels, probs);
}

bool EM::trainE(const img_t* samples,
                const img_t* _means0,
                const img_t* _covs0,
                const img_t* _weights0,
                img_t* logLikelihoods,
                img_t* labels,
                img_t* probs)
{
    img_t samplesMat = samples;
    vector<img_t> covs0;
    _covs0.getMatVector(covs0);

    img_t means0 = _means0, weights0 = _weights0;

    setTrainData(START_E_STEP, samplesMat, 0, !_means0.empty() ? &means0 : 0,
                 !_covs0.empty() ? &covs0 : 0, !_weights0.empty() ? &weights0 : 0);
    return doTrain(START_E_STEP, logLikelihoods, labels, probs);
}

bool EM::trainM(const img_t* samples,
                const img_t* _probs0,
                img_t* logLikelihoods,
                img_t* labels,
                img_t* probs)
{
    img_t samplesMat = samples;
    img_t probs0 = _probs0;

    setTrainData(START_M_STEP, samplesMat, !_probs0.empty() ? &probs0 : 0, 0, 0, 0);
    return doTrain(START_M_STEP, logLikelihoods, labels, probs);
}


Vec2d EM::predict(const img_t* _sample, img_t* _probs) const
{
    img_t sample = _sample;
    CC_Assert(isTrained());

    CC_Assert(!sample.empty());
    if(sample->tid != CC_64F)
    {
        img_t tmp;
        sample.convertTo(tmp, CC_64FC1);
        sample = tmp;
    }
    sample cvReshape(,1, 1);

    img_t probs;
    if(_probs.needed())
    {
        _probs cvSetMat(1, nclusters, CC_64FC1);
        probs = _probs;
    }

    return computeProbabilities(sample, !probs.empty() ? &probs : 0);
}

bool EM::isTrained() const
{
    return !means.empty();
}


static
void checkTrainData(int startStep, const img_t& samples,
                    int nclusters, int covMatType, const img_t* probs, const img_t* means,
                    const vector<img_t>* covs, const img_t* weights)
{
    // Check samples.
    CC_Assert(!samples.empty());
    CC_Assert(samples CC_MAT_CN() == 1);

    int nsamples = samples->rows;
    int dim = samples->cols;

    // Check training params.
    CC_Assert(nclusters > 0);
    CC_Assert(nclusters <= nsamples);
    CC_Assert(startStep == EM::START_AUTO_STEP ||
              startStep == EM::START_E_STEP ||
              startStep == EM::START_M_STEP);
    CC_Assert(covMatType == EM::COV_MAT_GENERIC ||
              covMatType == EM::COV_MAT_DIAGONAL ||
              covMatType == EM::COV_MAT_SPHERICAL);

    CC_Assert(!probs ||
        (!probs->empty() &&
         probs->rows == nsamples && probs->cols == nclusters &&
         (probs->tid() == CC_32FC1 || probs->tid() == CC_64FC1)));

    CC_Assert(!weights ||
        (!weights->empty() &&
         (weights->cols == 1 || weights->rows == 1) && static_cast<int>(weights->total()) == nclusters &&
         (weights->tid() == CC_32FC1 || weights->tid() == CC_64FC1)));

    CC_Assert(!means ||
        (!means->empty() &&
         means->rows == nclusters && means->cols == dim &&
         means->CC_MAT_CN() == 1));

    CC_Assert(!covs ||
        (!covs->empty() &&
         static_cast<int>(covs->size()) == nclusters));
    if(covs)
    {
        const CSize covSize(dim, dim);
        for(size_t i = 0; i < covs->size(); i++)
        {
            const img_t& m = (*covs)[i];
            CC_Assert(!m.empty() && m.size() == covSize && (m CC_MAT_CN() == 1));
        }
    }

    if(startStep == EM::START_E_STEP)
    {
        CC_Assert(means);
    }
    else if(startStep == EM::START_M_STEP)
    {
        CC_Assert(probs);
    }
}

static
void preprocessSampleData(const img_t& src, img_t& dst, int dstType, bool isAlwaysClone)
{
    if(src->tid == dstType && !isAlwaysClone)
        dst = src;
    else
        src.convertTo(dst, dstType);
}

static
void preprocessProbability(img_t& probs)
{
    max(probs, 0., probs);

    const double uniformProbability = (double)(1./probs->cols);
    for(int y = 0; y < probs->rows; y++)
    {
        img_t sampleProbs = probs.row(y);

        double maxVal = 0;
        minMaxLoc(sampleProbs, 0, &maxVal);
        if(maxVal < FLT_EPSILON)
            sampleProbs.setTo(uniformProbability);
        else
            normalize(sampleProbs, sampleProbs, 1, 0, NORM_L1);
    }
}

void EM::setTrainData(int startStep, const img_t& samples,
                      const img_t* probs0,
                      const img_t* means0,
                      const vector<img_t>* covs0,
                      const img_t* weights0)
{
    clear();

    checkTrainData(startStep, samples, nclusters, covMatType, probs0, means0, covs0, weights0);

    bool isKMeansInit = (startStep == EM::START_AUTO_STEP) || (startStep == EM::START_E_STEP && (covs0 == 0 || weights0 == 0));
    // Set checked data
    preprocessSampleData(samples, trainSamples, isKMeansInit ? CC_32FC1 : CC_64FC1, false);

    // set probs
    if(probs0 && startStep == EM::START_M_STEP)
    {
        preprocessSampleData(*probs0, trainProbs, CC_64FC1, true);
        preprocessProbability(trainProbs);
    }

    // set weights
    if(weights0 && (startStep == EM::START_E_STEP && covs0))
    {
        weights0->convertTo(weights, CC_64FC1);
        weights cvReshape(,1,1);
        preprocessProbability(weights);
    }

    // set means
    if(means0 && (startStep == EM::START_E_STEP/* || startStep == EM::START_AUTO_STEP*/))
        means0->convertTo(means, isKMeansInit ? CC_32FC1 : CC_64FC1);

    // set covs
    if(covs0 && (startStep == EM::START_E_STEP && weights0))
    {
        covs.resize(nclusters);
        for(size_t i = 0; i < covs0->size(); i++)
            (*covs0)[i].convertTo(covs[i], CC_64FC1);
    }
}

void EM::decomposeCovs()
{
    CC_Assert(!covs.empty());
    covsEigenValues.resize(nclusters);
    if(covMatType == EM::COV_MAT_GENERIC)
        covsRotateMats.resize(nclusters);
    invCovsEigenValues.resize(nclusters);
    for(int clusterIndex = 0; clusterIndex < nclusters; clusterIndex++)
    {
        CC_Assert(!covs[clusterIndex].empty());

        CSVD svd(covs[clusterIndex], CSVD::MODIFY_A + CSVD::FULL_UV);

        if(covMatType == EM::COV_MAT_SPHERICAL)
        {
            double maxSingularVal = svd.w.at<double>(0);
            covsEigenValues[clusterIndex] = img_t(1, 1, CC_64FC1, Scalar(maxSingularVal));
        }
        else if(covMatType == EM::COV_MAT_DIAGONAL)
        {
            covsEigenValues[clusterIndex] = svd.w;
        }
        else //EM::COV_MAT_GENERIC
        {
            covsEigenValues[clusterIndex] = svd.w;
            covsRotateMats[clusterIndex] = svd.u;
        }
        max(covsEigenValues[clusterIndex], minEigenValue, covsEigenValues[clusterIndex]);
        invCovsEigenValues[clusterIndex] = 1./covsEigenValues[clusterIndex];
    }
}

void EM::clusterTrainSamples()
{
    int nsamples = trainSamples->rows;

    // Cluster samples, compute/update means

    // Convert samples and means to 32F, because kmeans requires this type.
    img_t trainSamplesFlt, meansFlt;
    if(trainSamples->tid != CC_32F)
        trainSamples.convertTo(trainSamplesFlt, CC_32FC1);
    else
        trainSamplesFlt = trainSamples;
    if(!means.empty())
    {
        if(means->tid != CC_32F)
            means.convertTo(meansFlt, CC_32FC1);
        else
            meansFlt = means;
    }

    img_t labels;
    kmeans(trainSamplesFlt, nclusters, labels,  TermCriteria(TermCriteria::COUNT, means.empty() ? 10 : 1, 0.5), 10, KMEANS_PP_CENTERS, meansFlt);

    // Convert samples and means back to 64F.
    CC_Assert(meansFlt->tid == CC_32FC1);
    if(trainSamples->tid != CC_64F)
    {
        img_t trainSamplesBuffer;
        trainSamplesFlt.convertTo(trainSamplesBuffer, CC_64FC1);
        trainSamples = trainSamplesBuffer;
    }
    meansFlt.convertTo(means, CC_64FC1);

    // Compute weights and covs
    weights = img_t(1, nclusters, CC_64FC1, Scalar(0));
    covs.resize(nclusters);
    for(int clusterIndex = 0; clusterIndex < nclusters; clusterIndex++)
    {
        img_t clusterSamples;
        for(int sampleIndex = 0; sampleIndex < nsamples; sampleIndex++)
        {
            if(labels.at<int>(sampleIndex) == clusterIndex)
            {
                const img_t sample = trainSamples.row(sampleIndex);
                clusterSamples.push_back(sample);
            }
        }
        CC_Assert(!clusterSamples.empty());

        calcCovarMatrix(clusterSamples, covs[clusterIndex], means.row(clusterIndex),
            CC_COVAR_NORMAL + CC_COVAR_ROWS + CC_COVAR_USE_AVG + CC_COVAR_SCALE, CC_64FC1);
        weights.at<double>(clusterIndex) = static_cast<double>(clusterSamples->rows)/static_cast<double>(nsamples);
    }

    decomposeCovs();
}

void EM::computeLogWeightDivDet()
{
    CC_Assert(!covsEigenValues.empty());

    img_t logWeights;
    max(weights, DBL_MIN, weights);
    log(weights, logWeights);

    logWeightDivDet cvSetMat(1, nclusters, CC_64FC1);
    // note: logWeightDivDet = log(weight_k) - 0.5 * log(|det(cov_k)|)

    for(int clusterIndex = 0; clusterIndex < nclusters; clusterIndex++)
    {
        double logDetCov = 0.;
        const int evalCount = static_cast<int>(covsEigenValues[clusterIndex] CC_MAT_TOTAL());
        for(int di = 0; di < evalCount; di++)
            logDetCov += std::log(covsEigenValues[clusterIndex].at<double>(covMatType != EM::COV_MAT_SPHERICAL ? di : 0));

        logWeightDivDet.at<double>(clusterIndex) = logWeights.at<double>(clusterIndex) - 0.5 * logDetCov;
    }
}

bool EM::doTrain(int startStep, img_t* logLikelihoods, img_t* labels, img_t* probs)
{
    int dim = trainSamples->cols;
    // Precompute the empty initial train data in the cases of EM::START_E_STEP and START_AUTO_STEP
    if(startStep != EM::START_M_STEP)
    {
        if(covs.empty())
        {
            CC_Assert(weights.empty());
            clusterTrainSamples();
        }
    }

    if(!covs.empty() && covsEigenValues.empty())
    {
        CC_Assert(invCovsEigenValues.empty());
        decomposeCovs();
    }

    if(startStep == EM::START_M_STEP)
        mStep();

    double trainLogLikelihood, prevTrainLogLikelihood = 0.;
    for(int iter = 0; ; iter++)
    {
        eStep();
        trainLogLikelihood = sum(trainLogLikelihoods)[0];

        if(iter >= maxIters - 1)
            break;

        double trainLogLikelihoodDelta = trainLogLikelihood - prevTrainLogLikelihood;
        if(iter != 0 &&
            (trainLogLikelihoodDelta < -DBL_EPSILON ||
             trainLogLikelihoodDelta < epsilon * std::fabs(trainLogLikelihood)))
            break;

        mStep();

        prevTrainLogLikelihood = trainLogLikelihood;
    }

    if(trainLogLikelihood <= -DBL_MAX/10000.)
    {
        clear();
        return false;
    }

    // postprocess covs
    covs.resize(nclusters);
    for(int clusterIndex = 0; clusterIndex < nclusters; clusterIndex++)
    {
        if(covMatType == EM::COV_MAT_SPHERICAL)
        {
            covs[clusterIndex] cvSetMat(dim, dim, CC_64FC1);
            setIdentity(covs[clusterIndex], Scalar(covsEigenValues[clusterIndex].at<double>(0)));
        }
        else if(covMatType == EM::COV_MAT_DIAGONAL)
        {
            covs[clusterIndex] = img_t::diag(covsEigenValues[clusterIndex]);
        }
    }

    if(labels.needed())
        trainLabels.copyTo(labels);
    if(probs.needed())
        trainProbs.copyTo(probs);
    if(logLikelihoods.needed())
        trainLogLikelihoods.copyTo(logLikelihoods);

    trainSamples.release();
    trainProbs.release();
    trainLabels.release();
    trainLogLikelihoods.release();

    return true;
}

Vec2d EM::computeProbabilities(const img_t& sample, img_t* probs) const
{
    // L_ik = log(weight_k) - 0.5 * log(|det(cov_k)|) - 0.5 *(x_i - mean_k)' cov_k^(-1) (x_i - mean_k)]
    // q = arg(max_k(L_ik))
    // probs_ik = exp(L_ik - L_iq) / (1 + sum_j!=q (exp(L_ij - L_iq))
    // see Alex Smola's blog http://blog.smola.org/page/2 for
    // details on the log-sum-exp trick

    CC_Assert(!means.empty());
    CC_Assert(sample->tid == CC_64FC1);
    CC_Assert(sample->rows == 1);
    CC_Assert(sample->cols == means->cols);

    int dim = sample->cols;

    img_t L(1, nclusters, CC_64FC1);
    int label = 0;
    for(int clusterIndex = 0; clusterIndex < nclusters; clusterIndex++)
    {
        const img_t centeredSample = sample - means.row(clusterIndex);

        img_t rotatedCenteredSample = covMatType != EM::COV_MAT_GENERIC ?
                centeredSample : centeredSample * covsRotateMats[clusterIndex];

        double Lval = 0;
        for(int di = 0; di < dim; di++)
        {
            double w = invCovsEigenValues[clusterIndex].at<double>(covMatType != EM::COV_MAT_SPHERICAL ? di : 0);
            double val = rotatedCenteredSample.at<double>(di);
            Lval += w * val * val;
        }
        CC_DbgAssert(!logWeightDivDet.empty());
        L.at<double>(clusterIndex) = logWeightDivDet.at<double>(clusterIndex) - 0.5 * Lval;

        if(L.at<double>(clusterIndex) > L.at<double>(label))
            label = clusterIndex;
    }

    double maxLVal = L.at<double>(label);
    img_t expL_Lmax = L; // exp(L_ij - L_iq)
    for(int i = 0; i < L->cols; i++)
        expL_Lmax.at<double>(i) = std::exp(L.at<double>(i) - maxLVal);
    double expDiffSum = sum(expL_Lmax)[0]; // sum_j(exp(L_ij - L_iq))

    if(probs)
    {
        probs->create(1, nclusters, CC_64FC1);
        double factor = 1./expDiffSum;
        expL_Lmax *= factor;
        expL_Lmax.copyTo(*probs);
    }

    Vec2d res;
    res[0] = std::log(expDiffSum)  + maxLVal - 0.5 * dim * CC_LOG2PI;
    res[1] = label;

    return res;
}

void EM::eStep()
{
    // Compute probs_ik from means_k, covs_k and weights_k.
    trainProbs cvSetMat(trainSamples->rows, nclusters, CC_64FC1);
    trainLabels cvSetMat(trainSamples->rows, 1, CC_32SC1);
    trainLogLikelihoods cvSetMat(trainSamples->rows, 1, CC_64FC1);

    computeLogWeightDivDet();

    CC_DbgAssert(trainSamples->tid == CC_64FC1);
    CC_DbgAssert(means->tid == CC_64FC1);

    for(int sampleIndex = 0; sampleIndex < trainSamples->rows; sampleIndex++)
    {
        img_t sampleProbs = trainProbs.row(sampleIndex);
        Vec2d res = computeProbabilities(trainSamples.row(sampleIndex), &sampleProbs);
        trainLogLikelihoods.at<double>(sampleIndex) = res[0];
        trainLabels.at<int>(sampleIndex) = static_cast<int>(res[1]);
    }
}

void EM::mStep()
{
    // Update means_k, covs_k and weights_k from probs_ik
    int dim = trainSamples->cols;

    // Update weights
    // not normalized first
    reduce(trainProbs, weights, 0, CC_REDUCE_SUM);

    // Update means
    means cvSetMat(nclusters, dim, CC_64FC1);
    means = Scalar(0);

    const double minPosWeight = trainSamples->rows * DBL_EPSILON;
    double minWeight = DBL_MAX;
    int minWeightClusterIndex = -1;
    for(int clusterIndex = 0; clusterIndex < nclusters; clusterIndex++)
    {
        if(weights.at<double>(clusterIndex) <= minPosWeight)
            continue;

        if(weights.at<double>(clusterIndex) < minWeight)
        {
            minWeight = weights.at<double>(clusterIndex);
            minWeightClusterIndex = clusterIndex;
        }

        img_t clusterMean = means.row(clusterIndex);
        for(int sampleIndex = 0; sampleIndex < trainSamples->rows; sampleIndex++)
            clusterMean += trainProbs.at<double>(sampleIndex, clusterIndex) * trainSamples.row(sampleIndex);
        clusterMean /= weights.at<double>(clusterIndex);
    }

    // Update covsEigenValues and invCovsEigenValues
    covs.resize(nclusters);
    covsEigenValues.resize(nclusters);
    if(covMatType == EM::COV_MAT_GENERIC)
        covsRotateMats.resize(nclusters);
    invCovsEigenValues.resize(nclusters);
    for(int clusterIndex = 0; clusterIndex < nclusters; clusterIndex++)
    {
        if(weights.at<double>(clusterIndex) <= minPosWeight)
            continue;

        if(covMatType != EM::COV_MAT_SPHERICAL)
            covsEigenValues[clusterIndex] cvSetMat(1, dim, CC_64FC1);
        else
            covsEigenValues[clusterIndex] cvSetMat(1, 1, CC_64FC1);

        if(covMatType == EM::COV_MAT_GENERIC)
            covs[clusterIndex] cvSetMat(dim, dim, CC_64FC1);

        img_t clusterCov = covMatType != EM::COV_MAT_GENERIC ?
            covsEigenValues[clusterIndex] : covs[clusterIndex];

        clusterCov = Scalar(0);

        img_t centeredSample;
        for(int sampleIndex = 0; sampleIndex < trainSamples->rows; sampleIndex++)
        {
            centeredSample = trainSamples.row(sampleIndex) - means.row(clusterIndex);

            if(covMatType == EM::COV_MAT_GENERIC)
                clusterCov += trainProbs.at<double>(sampleIndex, clusterIndex) * centeredSample.t() * centeredSample;
            else
            {
                double p = trainProbs.at<double>(sampleIndex, clusterIndex);
                for(int di = 0; di < dim; di++)
                {
                    double val = centeredSample.at<double>(di);
                    clusterCov.at<double>(covMatType != EM::COV_MAT_SPHERICAL ? di : 0) += p*val*val;
                }
            }
        }

        if(covMatType == EM::COV_MAT_SPHERICAL)
            clusterCov /= dim;

        clusterCov /= weights.at<double>(clusterIndex);

        // Update covsRotateMats for EM::COV_MAT_GENERIC only
        if(covMatType == EM::COV_MAT_GENERIC)
        {
            CSVD svd(covs[clusterIndex], CSVD::MODIFY_A + CSVD::FULL_UV);
            covsEigenValues[clusterIndex] = svd.w;
            covsRotateMats[clusterIndex] = svd.u;
        }

        max(covsEigenValues[clusterIndex], minEigenValue, covsEigenValues[clusterIndex]);

        // update invCovsEigenValues
        invCovsEigenValues[clusterIndex] = 1./covsEigenValues[clusterIndex];
    }

    for(int clusterIndex = 0; clusterIndex < nclusters; clusterIndex++)
    {
        if(weights.at<double>(clusterIndex) <= minPosWeight)
        {
            img_t clusterMean = means.row(clusterIndex);
            means.row(minWeightClusterIndex).copyTo(clusterMean);
            covs[minWeightClusterIndex].copyTo(covs[clusterIndex]);
            covsEigenValues[minWeightClusterIndex].copyTo(covsEigenValues[clusterIndex]);
            if(covMatType == EM::COV_MAT_GENERIC)
                covsRotateMats[minWeightClusterIndex].copyTo(covsRotateMats[clusterIndex]);
            invCovsEigenValues[minWeightClusterIndex].copyTo(invCovsEigenValues[clusterIndex]);
        }
    }

    // Normalize weights
    weights /= trainSamples->rows;
}

void EM::read(const FileNode& fn)
{
    Algorithm::read(fn);

    decomposeCovs();
    computeLogWeightDivDet();
}

} // namespace cv


