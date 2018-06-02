#include "_cv.h"


CC_IMPL CvKalman*
cvCreateKalman(int DP, int MP, int CP)
{
    CvKalman *kalman = 0;

    CC_FUNCNAME("cvCreateKalman");
    
    __BEGIN__;

    if(DP <= 0 || MP <= 0)
        CC_ERROR(CC_StsOutOfRange,
        "state and measurement vectors must have positive number of dimensions");

    if(CP < 0)
        CP = DP;
    
    /* allocating memory for the structure */
    CC_CALL(kalman = (CvKalman *)cAlloc(sizeof(CvKalman)));
    memset(kalman, 0, sizeof(*kalman));
    
    kalman->DP = DP;
    kalman->MP = MP;
    kalman->CP = CP;

    CC_CALL(kalman->state_pre = cvCreateMat(DP, 1, CC_32FC1));
    cvZero(kalman->state_pre);
    
    CC_CALL(kalman->state_post = cvCreateMat(DP, 1, CC_32FC1));
    cvZero(kalman->state_post);
    
    CC_CALL(kalman->transition_matrix = cvCreateMat(DP, DP, CC_32FC1));
    cvSetIdentity(kalman->transition_matrix);

    CC_CALL(kalman->process_noise_cov = cvCreateMat(DP, DP, CC_32FC1));
    cvSetIdentity(kalman->process_noise_cov);
    
    CC_CALL(kalman->measurement_matrix = cvCreateMat(MP, DP, CC_32FC1));
    cvZero(kalman->measurement_matrix);

    CC_CALL(kalman->measurement_noise_cov = cvCreateMat(MP, MP, CC_32FC1));
    cvSetIdentity(kalman->measurement_noise_cov);

    CC_CALL(kalman->error_cov_pre = cvCreateMat(DP, DP, CC_32FC1));
    
    CC_CALL(kalman->error_cov_post = cvCreateMat(DP, DP, CC_32FC1));
    cvZero(kalman->error_cov_post);

    CC_CALL(kalman->gain = cvCreateMat(DP, MP, CC_32FC1));

    if(CP > 0)
    {
        CC_CALL(kalman->control_matrix = cvCreateMat(DP, CP, CC_32FC1));
        cvZero(kalman->control_matrix);
    }

    CC_CALL(kalman->temp1 = cvCreateMat(DP, DP, CC_32FC1));
    CC_CALL(kalman->temp2 = cvCreateMat(MP, DP, CC_32FC1));
    CC_CALL(kalman->temp3 = cvCreateMat(MP, MP, CC_32FC1));
    CC_CALL(kalman->temp4 = cvCreateMat(MP, DP, CC_32FC1));
    CC_CALL(kalman->temp5 = cvCreateMat(MP, 1, CC_32FC1));

#if 1
    kalman->PosterState = kalman->state_pre->tt.fl;
    kalman->PriorState = kalman->state_post->tt.fl;
    kalman->DynamMatr = kalman->transition_matrix->tt.fl;
    kalman->MeasurementMatr = kalman->measurement_matrix->tt.fl;
    kalman->MNCovariance = kalman->measurement_noise_cov->tt.fl;
    kalman->PNCovariance = kalman->process_noise_cov->tt.fl;
    kalman->KalmGainMatr = kalman->gain->tt.fl;
    kalman->PriorErrorCovariance = kalman->error_cov_pre->tt.fl;
    kalman->PosterErrorCovariance = kalman->error_cov_post->tt.fl;
#endif    

    __END__;

    if(cvGetErrStatus() < 0)
        cvReleaseKalman(&kalman);

    return kalman;
}


CC_IMPL void
cvReleaseKalman(CvKalman** _kalman)
{
    CvKalman *kalman;

    CC_FUNCNAME("cvReleaseKalman");
    __BEGIN__;
    
    if(!_kalman)
        CC_ERROR(CC_StsNullPtr, "");
    
    kalman = *_kalman;
    
    /* freeing the memory */
    cvReleaseMat(&kalman->state_pre);
    cvReleaseMat(&kalman->state_post);
    cvReleaseMat(&kalman->transition_matrix);
    cvReleaseMat(&kalman->control_matrix);
    cvReleaseMat(&kalman->measurement_matrix);
    cvReleaseMat(&kalman->process_noise_cov);
    cvReleaseMat(&kalman->measurement_noise_cov);
    cvReleaseMat(&kalman->error_cov_pre);
    cvReleaseMat(&kalman->gain);
    cvReleaseMat(&kalman->error_cov_post);
    cvReleaseMat(&kalman->temp1);
    cvReleaseMat(&kalman->temp2);
    cvReleaseMat(&kalman->temp3);
    cvReleaseMat(&kalman->temp4);
    cvReleaseMat(&kalman->temp5);

    memset(kalman, 0, sizeof(*kalman));

    /* deallocating the structure */
    cFree(_kalman);

    __END__;
}


CC_IMPL const img_t*
cvKalmanPredict(CvKalman* kalman, const img_t* control)
{
    img_t* result = 0;
    
    CC_FUNCNAME("cvKalmanPredict");

    __BEGIN__;
    
    if(!kalman)
        CC_ERROR(CC_StsNullPtr, "");

    /* update the state */
    /* x'(k) = A*x(k) */
    CC_CALL(cvMatMulAdd(kalman->transition_matrix, kalman->state_post, 0, kalman->state_pre));

    if(control && kalman->CP > 0)
        /* x'(k) = x'(k) + B*u(k) */
        CC_CALL(cvMatMulAdd(kalman->control_matrix, control, kalman->state_pre, kalman->state_pre));
    
    /* update error covariance matrices */
    /* temp1 = A*P(k) */
    CC_CALL(cvMatMulAdd(kalman->transition_matrix, kalman->error_cov_post, 0, kalman->temp1));
    
    /* P'(k) = temp1*At + Q */
    CC_CALL(cvGEMM(kalman->temp1, kalman->transition_matrix, 1, kalman->process_noise_cov, 1,
                     kalman->error_cov_pre, CC_GEMM_B_T));

    result = kalman->state_pre;

    __END__;

    return result;
}


CC_IMPL const img_t*
cvKalmanCorrect(CvKalman* kalman, const img_t* measurement)
{
    img_t* result = 0;

    CC_FUNCNAME("cvKalmanCorrect");

    __BEGIN__;
    
    if(!kalman || !measurement)
        CC_ERROR(CC_StsNullPtr, "");

    /* temp2 = H*P'(k) */
    CC_CALL(cvMatMulAdd(kalman->measurement_matrix,
                          kalman->error_cov_pre, 0, kalman->temp2));
    /* temp3 = temp2*Ht + R */
    CC_CALL(cvGEMM(kalman->temp2, kalman->measurement_matrix, 1,
                     kalman->measurement_noise_cov, 1, kalman->temp3, CC_GEMM_B_T));

    /* temp4 = inv(temp3)*temp2 = Kt(k) */
    CC_CALL(cvSolve(kalman->temp3, kalman->temp2, kalman->temp4, CC_SVD));

    /* K(k) */
    CC_CALL(cvTranspose(kalman->temp4, kalman->gain));
    
    /* temp5 = z(k) - H*x'(k) */
    CC_CALL(cvGEMM(kalman->measurement_matrix, kalman->state_pre, -1, measurement, 1, kalman->temp5));

    /* x(k) = x'(k) + K(k)*temp5 */
    CC_CALL(cvMatMulAdd(kalman->gain, kalman->temp5, kalman->state_pre, kalman->state_post));

    /* P(k) = P'(k) - K(k)*temp2 */
    CC_CALL(cvGEMM(kalman->gain, kalman->temp2, -1, kalman->error_cov_pre, 1,
                     kalman->error_cov_post, 0));

    result = kalman->state_post;

    __END__;

    return result;
}
