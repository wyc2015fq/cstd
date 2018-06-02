

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvCreateConDensation
//    Purpose: Creating CvConDensation structure and allocating memory for it
//    Context:
//    Parameters:
//      Kalman     - double pointer to CvConDensation structure
//      DP         - dimension of the dynamical vector
//      MP         - dimension of the measurement vector
//      SamplesNum - number of samples in sample set used in algorithm 
//    Returns:
//    Notes:
//      
//F*/

CC_IMPL CvConDensation* cvCreateConDensation(int DP, int MP, int SamplesNum)
{
    int i;
    CvConDensation *CD = 0;

    CC_FUNCNAME("cvCreateConDensation");
    __BEGIN__;
    
    if(DP < 0 || MP < 0 || SamplesNum < 0)
        CC_ERROR(CC_StsOutOfRange, "");
    
    /* allocating memory for the structure */
    CC_CALL(CD = (CvConDensation *) cAlloc(sizeof(CvConDensation)));
    /* setting structure params */
    CD->SamplesNum = SamplesNum;
    CD->DP = DP;
    CD->MP = MP;
    /* allocating memory for structure fields */
    CC_CALL(CD->flSamples = (float **) cAlloc(sizeof(float *) * SamplesNum));
    CC_CALL(CD->flNewSamples = (float **) cAlloc(sizeof(float *) * SamplesNum));
    CC_CALL(CD->flSamples[0] = (float *) cAlloc(sizeof(float) * SamplesNum * DP));
    CC_CALL(CD->flNewSamples[0] = (float *) cAlloc(sizeof(float) * SamplesNum * DP));

    /* setting pointers in pointer's arrays */
    for(i = 1; i < SamplesNum; i++)
    {
        CD->flSamples[i] = CD->flSamples[i - 1] + DP;
        CD->flNewSamples[i] = CD->flNewSamples[i - 1] + DP;
    }

    CC_CALL(CD->State = (float *) cAlloc(sizeof(float) * DP));
    CC_CALL(CD->DynamMatr = (float *) cAlloc(sizeof(float) * DP * DP));
    CC_CALL(CD->flConfidence = (float *) cAlloc(sizeof(float) * SamplesNum));
    CC_CALL(CD->flCumulative = (float *) cAlloc(sizeof(float) * SamplesNum));

    CC_CALL(CD->RandS = (CvRandState *) cAlloc(sizeof(CvRandState) * DP));
    CC_CALL(CD->Temp = (float *) cAlloc(sizeof(float) * DP));
    CC_CALL(CD->RandomSample = (float *) cAlloc(sizeof(float) * DP));

    /* Returning created structure */
    __END__;

    return CD;
}

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvReleaseConDensation
//    Purpose: Releases CvConDensation structure and frees memory allocated for it
//    Context:
//    Parameters:
//      Kalman     - double pointer to CvConDensation structure
//      DP         - dimension of the dynamical vector
//      MP         - dimension of the measurement vector
//      SamplesNum - number of samples in sample set used in algorithm 
//    Returns:
//    Notes:
//      
//F*/
CC_IMPL void
cvReleaseConDensation(CvConDensation ** ConDensation)
{
    CC_FUNCNAME("cvReleaseConDensation");
    __BEGIN__;
    
    CvConDensation *CD = *ConDensation;
    
    if(!ConDensation)
        CC_ERROR(CC_StsNullPtr, "");

    if(!CD)
        EXIT;

    /* freeing the memory */
	cFree(&CD->State);
    cFree(&CD->DynamMatr);
    cFree(&CD->flConfidence);
    cFree(&CD->flCumulative);
    cFree(&CD->flSamples[0]);
    cFree(&CD->flNewSamples[0]);
    cFree(&CD->flSamples);
    cFree(&CD->flNewSamples);
    cFree(&CD->Temp);
    cFree(&CD->RandS);
    cFree(&CD->RandomSample);
    /* release structure */
    cFree(ConDensation);
    
    __END__;

}

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvConDensUpdateByTime
//    Purpose: Performing Time Update routine for ConDensation algorithm
//    Context:
//    Parameters:
//      Kalman     - pointer to CvConDensation structure
//    Returns:
//    Notes:
//      
//F*/
CC_IMPL void
cvConDensUpdateByTime(CvConDensation * ConDens)
{
    int i, j;
    float Sum = 0;

    CC_FUNCNAME("cvConDensUpdateByTime");
    __BEGIN__;
    
    if(!ConDens)
        CC_ERROR(CC_StsNullPtr, "");

    /* Sets Temp to Zero */
    icvSetZero_32f(ConDens->Temp, ConDens->DP, 1);

    /* Calculating the Mean */
    for(i = 0; i < ConDens->SamplesNum; i++)
    {
        icvScaleVector_32f(ConDens->flSamples[i], ConDens->State, ConDens->DP,
                             ConDens->flConfidence[i]);
        icvAddVector_32f(ConDens->Temp, ConDens->State, ConDens->Temp, ConDens->DP);
        Sum += ConDens->flConfidence[i];
        ConDens->flCumulative[i] = Sum;
    }

    /* Taking the new vector from transformation of mean by dynamics matrix */

    icvScaleVector_32f(ConDens->Temp, ConDens->Temp, ConDens->DP, 1.f / Sum);
    icvTransformVector_32f(ConDens->DynamMatr, ConDens->Temp, ConDens->State, ConDens->DP,
                             ConDens->DP);
    Sum = Sum / ConDens->SamplesNum;

    /* Updating the set of random samples */
    for(i = 0; i < ConDens->SamplesNum; i++)
    {
        j = 0;
        while((ConDens->flCumulative[j] <= (float) i * Sum)&&(j<ConDens->SamplesNum-1))
        {
            j++;
        }
        icvCopyVector_32f(ConDens->flSamples[j], ConDens->DP, ConDens->flNewSamples[i]);
    }

    /* Adding the random-generated vector to every vector in sample set */
    for(i = 0; i < ConDens->SamplesNum; i++)
    {
        for(j = 0; j < ConDens->DP; j++)
        {
            cvbRand(ConDens->RandS + j, ConDens->RandomSample + j, 1);
        }

        icvTransformVector_32f(ConDens->DynamMatr, ConDens->flNewSamples[i],
                                 ConDens->flSamples[i], ConDens->DP, ConDens->DP);
        icvAddVector_32f(ConDens->flSamples[i], ConDens->RandomSample, ConDens->flSamples[i],
                           ConDens->DP);
    }

    __END__;
}

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvConDensInitSamplSet
//    Purpose: Performing Time Update routine for ConDensation algorithm
//    Context:
//    Parameters:
//    conDens     - pointer to CvConDensation structure
//    lowerBound  - vector of lower bounds used to random update of sample set
//    lowerBound  - vector of upper bounds used to random update of sample set
//    Returns:
//    Notes:
//      
//F*/

CC_IMPL void
cvConDensInitSampleSet(CvConDensation * conDens, img_t * lowerBound, img_t * upperBound)
{
    int i, j;
    float *LBound;
    float *UBound;
    float Prob = 1.f / conDens->SamplesNum;

    CC_FUNCNAME("cvConDensInitSampleSet");
    __BEGIN__;
    
    if(!conDens || !lowerBound || !upperBound)
        CC_ERROR(CC_StsNullPtr, "");

    if(CC_MAT_TYPE(lowerBound->tid) != CC_32F ||
        !CC_ARE_TYPES_EQ(lowerBound,upperBound))
        CC_ERROR(CC_StsBadArg, "source  has not appropriate format");

    if((lowerBound->cols != 1) || (upperBound->cols != 1))
        CC_ERROR(CC_StsBadArg, "source  has not appropriate size");

    if((lowerBound->rows != conDens->DP) || (upperBound->rows != conDens->DP))
        CC_ERROR(CC_StsBadArg, "source  has not appropriate size");

    LBound = lowerBound->tt.fl;
    UBound = upperBound->tt.fl;
    /* Initializing the structures to create initial Sample set */
    for(i = 0; i < conDens->DP; i++)
    {
        cvRandInit(&(conDens->RandS[i]),
                    LBound[i],
                    UBound[i],
                    i);
    }
    /* Generating the samples */
    for(j = 0; j < conDens->SamplesNum; j++)
    {
        for(i = 0; i < conDens->DP; i++)
        {
            cvbRand(conDens->RandS + i, conDens->flSamples[j] + i, 1);
        }
        conDens->flConfidence[j] = Prob;
    }
    /* Reinitializes the structures to update samples randomly */
    for(i = 0; i < conDens->DP; i++)
    {
        cvRandInit(&(conDens->RandS[i]),
                    (LBound[i] - UBound[i]) / 5,
                    (UBound[i] - LBound[i]) / 5,
                    i);
    }

    __END__;
}
