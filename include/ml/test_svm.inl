
#include "cv/ml/svm.inl"
int test_svm() {
    // step 1:  
    float labels[4] = {1.0, -1.0, -1.0, -1.0};  
    Mat labelsMat(3, 1, CV_32FC1, labels);  
      
    float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };  
    Mat trainingDataMat(3, 2, CV_32FC1, trainingData);  
      
    // step 2:  
    CvSVMParams params;  
    params.svm_type = CvSVM::C_SVC;  
    params.kernel_type = CvSVM::LINEAR;  
    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);  
      
    // step 3:  
    CvSVM SVM;  
    SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);  
      
    // step 4:  
    Vec3b green(0, 255, 0), blue(255, 0, 0);  
    for (int i=0; i<image.rows; i++)  
    {  
        for (int j=0; j<image.cols; j++)  
        {  
            Mat sampleMat = (Mat_<float>(1,2) << i,j);  
            float response = SVM.predict(sampleMat);  
      
            if (fabs(response-1.0) < 0.0001)  
            {  
                image.at<Vec3b>(j, i) = green;  
            }  
            else if (fabs(response+1.0) < 0.001)  
            {  
                image.at<Vec3b>(j, i) = blue;  
            }  
        }  
    }  
      
    // step 5:  
    int c = SVM.get_support_vector_count();  
      
    for (int i=0; i<c; i++)  
    {  
        const float* v = SVM.get_support_vector(i);  
    }  

}

int main() {
  return 0;
}
