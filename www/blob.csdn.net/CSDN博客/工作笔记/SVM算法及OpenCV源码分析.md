# SVM算法及OpenCV源码分析 - 工作笔记 - CSDN博客





2016年08月10日 09:18:25[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8880








关于SVM原理，请参看：

[系统学习机器学习之SVM(一)](http://blog.csdn.net/app_12062011/article/details/50385522)


[系统学习机器学习之SVM(二)](http://blog.csdn.net/app_12062011/article/details/50536369)

[系统学习机器学习之SVM(三)--Liblinear,LibSVM使用整理，总结](http://blog.csdn.net/app_12062011/article/details/50614989)

[系统学习机器学习之SVM(四)--SVM算法总结](http://blog.csdn.net/app_12062011/article/details/51605535)




原文：

http://blog.csdn.net/zhaocj/article/details/51297907


源码分析 

OpenCV 2.4.9的SVM程序是基于LibSVMv2.6。LibSVM是由台湾大学林智仁等开发的用于SVM分类和回归的开源机器学习工具包。

在进行源码分析之前，我们先给出用于SVM算法的训练参数结构变量CvSVMParams：







```cpp
struct CvSVMParams
{
    CvSVMParams();
    CvSVMParams( int _svm_type, int _kernel_type,
                 double _degree, double _gamma, double _coef0,
                 double _C, double _nu, double _p,
                 CvMat* _class_weights, CvTermCriteria _term_crit );

    int         svm_type;
    int         kernel_type;
    double      degree; // for poly
    double      gamma;  // for poly/rbf/sigmoid
    double      coef0;  // for poly/sigmoid

    double      C;  // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
    double      nu; // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
    double      p; // for CV_SVM_EPS_SVR
    CvMat*      class_weights; // for CV_SVM_C_SVC
    CvTermCriteria term_crit; // termination criteria
};
```



svm_type表示OpenCV能够实现的SVM的类型：C-SVC、ν-SVC、单类SVM、ε-SVR和ν-SVR，对应的变量分别为：CvSVM::C_SVC、CvSVM::NU_SVC、CvSVM::ONE_CLASS、CvSVM::EPS_SVR和CvSVM::NU_SVR

kernel_type表示OpenCV能够实现的核函数的类型：线性核函数、多项式核函数、高斯核函数和Sigmoid核函数，对应的变量分别为：CvSVM::LINEAR、CvSVM::POLY、CvSVM::RBF和CvSVM::SIGMOID

degree表示多项式核函数（式54）中的参数*q*

gamma表示多项式核函数（式54）、高斯核函数（式56）和Sigmoid核函数（式57）中的参数γ

coef0表示多项式核函数（式54）和Sigmoid核函数（式57）中的参数*p*

C表示惩罚参数*C*

nu表示ν-SVC和ν-SVR的参数ν

p表示ε-SVR的参数*ε*

class_weights表示不同分类的权值，该值与参数C相乘后，实现了不同分类的不同惩罚力度，该值越大，该类别的误分类数据的惩罚就越大

term_crit:SVM表示广义SMO算法的迭代过程的终止条件，该变量是结构数据类型：










```cpp
typedef struct CvTermCriteria
{
    //CV_TERMCRIT_ITER（表示使用迭代次数作为终止条件）和CV_TERMCRIT_EPS（表示使用精度作为终止条件）二值之一，或者二者的组合
    int  type; 
    int  max_iter;    //最大迭代次数
    double  epsilon;    //结果的精确性
}
```
下面是类CvSVM的缺省构造函数：










```cpp
CvSVM::CvSVM()
{
    decision_func = 0;    //表示决策函数，数据类型为CvSVMDecisionFunc
    class_labels = 0;    //表示分类问题的类标签
    class_weights = 0;    //表示分类问题的类别权重
    storage = 0;    //表示存储空间
    var_idx = 0;    //表示用到的特征属性的索引
    kernel = 0;    //表示核函数，数据类型为CvSVMKernel
    solver = 0;    //表示广义SMO算法的求解，数据类型为类CvSVMSolver
    default_model_name = "my_svm";

    clear();    //清空一些全局变量
}
```
下面是SVM的训练函数：










```cpp
bool CvSVM::train( const CvMat* _train_data, const CvMat* _responses,
    const CvMat* _var_idx, const CvMat* _sample_idx, CvSVMParams _params )
//_train_data表示训练样本数据集
//_responses表示训练样本的响应值
//_var_idx表示真正用到的特征属性的索引
//_sample_idx表示真正用到的训练样本的索引
//_params表示SVM算法所需要的一些参数，如SVM的类型，核函数的类型等
{
    bool ok = false;    //用于该函数的正确返回标识
    CvMat* responses = 0;    //表示样本的响应值
    CvMemStorage* temp_storage = 0;    //暂存
    const float** samples = 0;    //表示完整的训练样本数据

    CV_FUNCNAME( "CvSVM::train" );

    __BEGIN__;
    //svm_type表示SVM类型，sample_count表示训练样本的数量，var_count表示特征属性的数量，sample_size表示训练样本的存储空间的尺寸大小
    int svm_type, sample_count, var_count, sample_size;
    int block_size = 1 << 16;    //先定义一个很大的存储空间的尺寸大小
    //表示拉格朗日乘子α，但我们在实际计算中，已经把5种SVM转换为统一的f(β)形式，因此这里的alpha表示的是式150、式151、式152、式154和式156中β，在后面的程序中，涉及到拉格朗日乘子α的，本质上指的都是β
    double* alpha; 

    clear();    //清空一些全局变量
    //调用set_params函数，为全局变量params赋值（它的数据类型为CvSVMParams，用于表示SVM算法所需的一些参数），并判断params中元素的正确性
    CV_CALL( set_params( _params ));

    svm_type = _params.svm_type;    //表示SVM的类型

    /* Prepare training data and related parameters */
    //调用cvPrepareTrainData函数，为SVM算法准备样本数据，即初始化样本。首先检查样本_train_data，该变量矩阵一定要是CV_ROW_SAMPLE，即矩阵的行表示样本，列表示特征属性；然后赋值样本响应值，如果SVM类型为CvSVM::ONE_CLASS，响应值为0，否则为_responses，并且如果SVM为CvSVM::C_SVC或CvSVM::NU_SVC，响应值_responses的类型为CV_VAR_CATEGORICAL，即为分类，否则为CV_VAR_ORDERED，即为回归；再根据_sample_idx和_var_idx确定那些真正要用到的样本数据以及那些真正要用到的特征属性；最终得到完整的样本数据samples。
    CV_CALL( cvPrepareTrainData( "CvSVM::train", _train_data, CV_ROW_SAMPLE,
                                 svm_type != CvSVM::ONE_CLASS ? _responses : 0,
                                 svm_type == CvSVM::C_SVC ||
                                 svm_type == CvSVM::NU_SVC ? CV_VAR_CATEGORICAL :
                                 CV_VAR_ORDERED, _var_idx, _sample_idx,
                                 false, &samples, &sample_count, &var_count, &var_all,
                                 &responses, &class_labels, &var_idx ));

    //得到训练样本的存储空间大小
    sample_size = var_count*sizeof(samples[0][0]);

    // make the storage block size large enough to fit all
    // the temporary vectors and output support vectors.
    //定义一个足够大的存储空间，用于满足所有的暂存向量和输出支持向量
    block_size = MAX( block_size, sample_count*(int)sizeof(CvSVMKernelRow));
    block_size = MAX( block_size, sample_count*2*(int)sizeof(double) + 1024 );
    block_size = MAX( block_size, sample_size*2 + 1024 );
    //下面三条语句虽然略有不同，但基本上实现的功能都是分配内存空间并初始化
    CV_CALL( storage = cvCreateMemStorage(block_size + sizeof(CvMemBlock) + sizeof(CvSeqBlock)));
    CV_CALL( temp_storage = cvCreateChildMemStorage(storage));
    CV_CALL( alpha = (double*)cvMemStorageAlloc(temp_storage, sample_count*sizeof(double)));
    // create_kernel函数的作用是通过参数params得到参数kernel，即得到核函数
    create_kernel();
    // create_solver函数的作用是实例化CvSVMSolver类，得到参数solver
    create_solver();
    //调用do_train函数，真正完成SVM训练任务，后面给出了该函数的详细讲解
    if( !do_train( svm_type, sample_count, var_count, samples, responses, temp_storage, alpha ))
        EXIT;
    //表示成功得到了SVM模型
    ok = true; // model has been trained succesfully

    __END__;
    //释放一些不再使用的变量和内存空间
    delete solver;
    solver = 0;
    cvReleaseMemStorage( &temp_storage );
    cvReleaseMat( &responses );
    cvFree( &samples );

    if( cvGetErrStatus() < 0 || !ok )
        clear();

    return ok;    //返回
}
```
do_train函数的详细讲解：










```cpp
bool CvSVM::do_train( int svm_type, int sample_count, int var_count, const float** samples,
                    const CvMat* responses, CvMemStorage* temp_storage, double* alpha )
// svm_type表示SVM算法的类型
// sample_count表示训练样本的数量
// var_count表示特征属性的数量
// samples表示完整的训练样本数据
// responses表示训练样本的响应值
// temp_storage表示训练所需的内存空间
// alpha表示拉格朗日乘子α，实际为式148中的变量β
{
    bool ok = false;    //用于该函数的正确返回标识

    CV_FUNCNAME( "CvSVM::do_train" );

    __BEGIN__;

    CvSVMDecisionFunc* df = 0;    //表示SVM的决策函数
    const int sample_size = var_count*sizeof(samples[0][0]);    //定义样本空间大小
    int i, j, k;

    cvClearMemStorage( storage );    //清空storage内存空间

    if( svm_type == ONE_CLASS || svm_type == EPS_SVR || svm_type == NU_SVR )
    //如果SVM的类型为单类SVM、ε-SVR或ν-SVR
    {
        int sv_count = 0;    //表示支持向量的数量
        //为变量df（决策函数）分配内存空间，并把它的首地址指针指向变量decision_func
        CV_CALL( decision_func = df =
            (CvSVMDecisionFunc*)cvAlloc( sizeof(df[0]) ));

        df->rho = 0;    //初始化决策函数中的变量ρ，ρ实质为偏移量b
        //调用train1函数，该函数作用主要是根据SVM类型的不同，参数solver实现不同的SMO算法函数：类型为C_SVC，实现的是solver->solve_c_svc函数；类型为NU_SVC，实现的是solver->solve_nu_svc函数；类型为ONE_CLASS，实现的是solver->solve_one_class函数；类型为EPS_SVR，实现的是solver->solve_eps_svr函数；类型为NU_SVR，实现的是solver->solve_nu_svr函数。这5个函数在后面都会给出详细的讲解
        //在这里SVM的类型只能为ONE_CLASS、EPS_SVR或NU_SVR
        if( !train1( sample_count, var_count, samples, svm_type == ONE_CLASS ? 0 :
            responses->data.i, 0, 0, temp_storage, alpha, df->rho ))
            EXIT;
        //遍历所有训练样本，统计支持向量的数量，只有αi大于0的向量才是支持向量
        for( i = 0; i < sample_count; i++ )
            sv_count += fabs(alpha[i]) > 0;

        CV_Assert(sv_count != 0);    //确保不能没有支持向量

        sv_total = df->sv_count = sv_count;    //赋值支持向量的数量
        //为支持向量数据sv和拉格朗日乘子df->alpha开辟一块存储空间
        CV_CALL( df->alpha = (double*)cvMemStorageAlloc( storage, sv_count*sizeof(df->alpha[0])) );
        CV_CALL( sv = (float**)cvMemStorageAlloc( storage, sv_count*sizeof(sv[0])));
        //遍历训练样本
        for( i = k = 0; i < sample_count; i++ )
        {
            if( fabs(alpha[i]) > 0 )    //拉格朗日乘子αi大于0
            {
                //得到支持向量的样本数据sv，以及它所对应的αi值df->alpha
                CV_CALL( sv[k] = (float*)cvMemStorageAlloc( storage, sample_size ));
                memcpy( sv[k], samples[i], sample_size );
                df->alpha[k++] = alpha[i];
            }
        }
    }
    else    //SVM类型为C-SVC和ν-SVC，即分类问题
    {
        int class_count = class_labels->cols;    //得到样本的类别数量
        int* sv_tab = 0;    //用于标识某一样本是否为支持向量
        const float** temp_samples = 0;    //表示暂时需要使用的训练样本
        // class_ranges表示分类范围，例如有9个样本，共三类，并且已经按照响应值的大小进行了排序，结果为：1,1,1,2,2,3,3,3,3，则class_ranges[0]=0, class_ranges[1]=3, class_ranges[2]=5, class_ranges[3]=9，因此通过该变量很容易得到每类的样本数，以及它们的分布范围
        int* class_ranges = 0; 
        schar* temp_y = 0;        //表示暂时需要使用的训练样本的分类标签
        //再次确认是分类问题
        assert( svm_type == CvSVM::C_SVC || svm_type == CvSVM::NU_SVC );
        //如果是C_SVC类型，并且初始化了变量params.class_weights
        if( svm_type == CvSVM::C_SVC && params.class_weights )
        {
            const CvMat* cw = params.class_weights;    //为类别权重赋值
            //判断类别权重cw的数据格式是否正确，即cw必须是一维的矩阵形式，即向量，向量的元素数量必须等于分类数量，并且数据类型必须为CV_32FC1或CV_64FC1
            if( !CV_IS_MAT(cw) || (cw->cols != 1 && cw->rows != 1) ||
                cw->rows + cw->cols - 1 != class_count ||
                (CV_MAT_TYPE(cw->type) != CV_32FC1 && CV_MAT_TYPE(cw->type) != CV_64FC1) )
                CV_ERROR( CV_StsBadArg, "params.class_weights must be 1d floating-point vector "
                    "containing as many elements as the number of classes" );
            //把cw赋值给全局变量class_weights
            CV_CALL( class_weights = cvCreateMat( cw->rows, cw->cols, CV_64F ));
            CV_CALL( cvConvert( cw, class_weights ));
            //实现了该等式：class_weights = class_weights × params.C
            CV_CALL( cvScale( class_weights, class_weights, params.C ));
        }
        //为变量df（决策函数）分配内存空间，并把它的首地址指针指向变量decision_func
        CV_CALL( decision_func = df = (CvSVMDecisionFunc*)cvAlloc(
            (class_count*(class_count-1)/2)*sizeof(df[0])));
        //分配内存空间给sv_tab，并清零
        CV_CALL( sv_tab = (int*)cvMemStorageAlloc( temp_storage, sample_count*sizeof(sv_tab[0]) ));
        memset( sv_tab, 0, sample_count*sizeof(sv_tab[0]) );
        //分配内存空间给class_ranges，temp_samples和temp_y
        CV_CALL( class_ranges = (int*)cvMemStorageAlloc( temp_storage,
                            (class_count + 1)*sizeof(class_ranges[0])));
        CV_CALL( temp_samples = (const float**)cvMemStorageAlloc( temp_storage,
                            sample_count*sizeof(temp_samples[0])));
        CV_CALL( temp_y = (schar*)cvMemStorageAlloc( temp_storage, sample_count));

        class_ranges[class_count] = 0;    //清零
        //调用cvSortSamplesByClasses函数，实现了按照响应值对样本进行升序排序，并得到了变量class_ranges
        cvSortSamplesByClasses( samples, responses, class_ranges, 0 );
        //check that while cross-validation there were the samples from all the classes
        //确保class_ranges[class_count]必须大于0，交叉验证时要用到
        if( class_ranges[class_count] <= 0 )
            CV_ERROR( CV_StsBadArg, "While cross-validation one or more of the classes have "
            "been fell out of the sample. Try to enlarge <CvSVMParams::k_fold>" );

        if( svm_type == NU_SVC )    //如果为NU_SVC类型
        {
            // check if nu is feasible
            //得到样本中任意两个类别的样本数，检查ν-SVC中的参数ν是否满足式66的条件
            for(i = 0; i < class_count; i++ )    //遍历样本的所有类别
            {
                int ci = class_ranges[i+1] - class_ranges[i];    //得到第i个类别的样本数量
                for( j = i+1; j< class_count; j++ )    //遍历当前类别以后的所有类别
                {
                    //得到第j个类别的样本数量，第j个类别在第i个类别的排序后面
                    int cj = class_ranges[j+1] - class_ranges[j]; 
                    //如果不满足式66的条件，则退出程序
                    if( params.nu*(ci + cj)*0.5 > MIN( ci, cj ) )
                    {
                        // !!!TODO!!! add some diagnostic
                        EXIT; // exit immediately; will release the model and return NULL pointer
                    }
                }
            }
        }

        // train n*(n-1)/2 classifiers
        //下面的for循环实现了一对一的多类SVC方法
        //得到n*(n-1)/2个SVM分类器，这里的n表示分类数，即class_count，每一个SVM分类器的信息都单独存储在自己的df变量中
        for( i = 0; i < class_count; i++ )    //遍历所有类别
        {
            for( j = i+1; j < class_count; j++, df++ )    //遍历当前分类以后的所有类别
            {
                //si和sj分别表示第i个类别和第j个类别在排序后样本的起始索引值
                //ci和cj分别表示第i个类别和第j个类别的样本数量
                int si = class_ranges[i], ci = class_ranges[i+1] - si;
                int sj = class_ranges[j], cj = class_ranges[j+1] - sj;
                //Cn和Cp分别表示负例和正例的惩罚参数
                double Cp = params.C, Cn = Cp;
                //k1用于计数索引，sv_count表示支持向量的数量
                int k1 = 0, sv_count = 0;

                for( k = 0; k < ci; k++ )    //遍历第i个类别的所有样本
                {
                    temp_samples[k] = samples[si + k];    //第i个类别的所有样本
                    temp_y[k] = 1;    //第i个类别的样本分类标签，设为1
                }

                for( k = 0; k < cj; k++ )    //遍历第j个类别的所有样本
                {
                    temp_samples[ci + k] = samples[sj + k];    //第j个类别的所有样本
                    temp_y[ci + k] = -1;    //第j个类别的样本分类标签，设为-1
                }

                if( class_weights )    //如果应用分类权重class_weights这个参数
                {
                    //Cp和Cn分别为加权后的第i个和第j个类别的惩罚参数
                    Cp = class_weights->data.db[i];
                    Cn = class_weights->data.db[j];
                }
                //调用train1函数，该函数作用主要是根据SVM类型的不同，参数solver实现不同的SMO算法函数：类型为C_SVC，实现的是solver->solve_c_svc函数；类型为NU_SVC，实现的是solver->solve_nu_svc函数；类型为ONE_CLASS，实现的是solver->solve_one_class函数；类型为EPS_SVR，实现的是solver->solve_eps_svr函数；类型为NU_SVR，实现的是solver->solve_nu_svr函数。这5个函数在后面都会给出详细的讲解
                //在这里SVM的类型只能为C_SVC或NU_SVC
                if( !train1( ci + cj, var_count, temp_samples, temp_y,
                             Cp, Cn, temp_storage, alpha, df->rho ))
                    EXIT;
                //遍历第i个和第j个类别的所有样本，得到当前分类器的支持向量数量
                for( k = 0; k < ci + cj; k++ )
                    sv_count += fabs(alpha[k]) > 0;

                df->sv_count = sv_count;    //赋值当前分类器的支持向量数量

                //为拉格朗日乘子df->alpha和支持向量索引df->sv_index开辟一块存储空间
                CV_CALL( df->alpha = (double*)cvMemStorageAlloc( temp_storage,
                                                sv_count*sizeof(df->alpha[0])));
                CV_CALL( df->sv_index = (int*)cvMemStorageAlloc( temp_storage,
                                                sv_count*sizeof(df->sv_index[0])));

                for( k = 0; k < ci; k++ )    //遍历第i个类别的样本
                {
                    if( fabs(alpha[k]) > 0 )    //拉格朗日乘子αi大于0
                    {
                        sv_tab[si + k] = 1;    //标注该样本为支持向量
                        //当前分类器的支持向量在样本序列的索引值
                        df->sv_index[k1] = si + k; 
                        df->alpha[k1++] = alpha[k];    //赋值当前分类器的支持向量αi
                    }
                }

                for( k = 0; k < cj; k++ )    //遍历第j个类别的样本
                {
                    if( fabs(alpha[ci + k]) > 0 )    //拉格朗日乘子αi大于0
                    {
                        sv_tab[sj + k] = 1;    //标注该样本为支持向量
                        //当前分类器的支持向量在样本序列的索引值
                        df->sv_index[k1] = sj + k; 
                        df->alpha[k1++] = alpha[ci + k];    //赋值当前分类器的支持向量αi
                    }
                }
            }
        }    //训练n*(n-1)/2个SVM分类器结束

        // allocate support vectors and initialize sv_tab
        for( i = 0, k = 0; i < sample_count; i++ )    //遍历所有训练样本
        {
            //在前面计算n*(n-1)/2个SVM分类器的过程中，无论对于哪一个分类器，只要某一样本为支持向量，它的sv_tab都为1，在这里sv_tab又被赋值为全部样本下支持向量的计数值，即当前支持向量是全部支持向量的第几个支持向量
            if( sv_tab[i] )
                sv_tab[i] = ++k;
        }

        sv_total = k;    //得到了支持向量的数量
        //为支持向量数据sv分配存储空间
        CV_CALL( sv = (float**)cvMemStorageAlloc( storage, sv_total*sizeof(sv[0])));

        for( i = 0, k = 0; i < sample_count; i++ )    //遍历所有样本
        {
            if( sv_tab[i] )    //如果当前样本是支持向量
            {
                //得到支持向量的样本数据sv
                CV_CALL( sv[k] = (float*)cvMemStorageAlloc( storage, sample_size ));
                memcpy( sv[k], samples[i], sample_size );
                k++;    //计数值累加
            }
        }

        df = (CvSVMDecisionFunc*)decision_func;    //决策函数的指针重新定位

        // set sv pointers
        //重新设置df->sv_index，以前变量df->sv_index[i]表示的是当前分类器的第i个支持向量在当前训练样本序列的索引值，而现在经过下面多重for循环，df->sv_index[i]表示的是当前分类器的第i个支持向量在全部训练样本中所有支持向量的第几个支持向量减1
        //再次遍历n*(n-1)/2个SVM分类器
        for( i = 0; i < class_count; i++ )    //遍历所有分类
        {
            for( j = i+1; j < class_count; j++, df++ )    //遍历当前分类以后的所有分类
            {
                //遍历当前SVM分类器的所有支持向量
                for( k = 0; k < df->sv_count; k++ ) 
                {
                    //为df->sv_index重新赋值
                    df->sv_index[k] = sv_tab[df->sv_index[k]]-1;
                    //确保df->sv_index的值必须小于所有支持向量的总数
                    assert( (unsigned)df->sv_index[k] < (unsigned)sv_total );
                }
            }
        }
    }
    //对于使用线性核函数的SVM来说，即式53，它们的支持向量是呈现线性的，因此只需用一个支持向量就可以代表所有的支持向量，这么可以简化SVM模型，optimize_linear_svm函数就实现了这个功能，该函数的详见讲解见后面
    optimize_linear_svm();
    ok = true;    //返回变量赋值

    __END__;

    return ok;    //函数返回
}
```
用于求解C-SVC类型的广义SMO算法：








```cpp
bool CvSVMSolver::solve_c_svc( int _sample_count, int _var_count, const float** _samples, schar* _y,
                               double _Cp, double _Cn, CvMemStorage* _storage,
                               CvSVMKernel* _kernel, double* _alpha, CvSVMSolutionInfo& _si )
{
    int i;
    //调用create函数，用于设置广义SMO算法的参数，在后面给出该函数的详细讲解
    //对于C-SVC，select_working_set_func函数指针指向select_working_set，calc_rho_func函数指针指向calc_rho，get_row_func函数指针指向get_row_svc，_Cp和_Cn分别表示正例和负例的惩罚参数C
    if( !create( _sample_count, _var_count, _samples, _y, _sample_count,
                 _alpha, _Cp, _Cn, _storage, _kernel, &CvSVMSolver::get_row_svc,
                 &CvSVMSolver::select_working_set, &CvSVMSolver::calc_rho ))
        return false;
    //遍历所有样本，清空alpha数组，赋值b数组
    for( i = 0; i < sample_count; i++ )
    {
        alpha[i] = 0;    //α即为式148中的β，对于C-SVC，β初始化为0
        b[i] = -1;    //b即为式148中的p，对于C-SVC（式150），p为-1
    }
    //调用solve_generic，执行广义SMO算法的迭代过程，该函数在后面给出详细的讲解
    if( !solve_generic( _si ))
        return false;
    //遍历所有样本，使负例的β为负值，当再次用到alpha[i]时，会取绝对值，因此正负无所谓，这么做的好处是可以通过alpha[i]值，就能看出是正例还是负例
    for( i = 0; i < sample_count; i++ )
        alpha[i] *= y[i];

    return true;
}
```
用于求解ν-SVC类型的广义SMO算法：










```cpp
bool CvSVMSolver::solve_nu_svc( int _sample_count, int _var_count, const float** _samples, schar* _y,
                                CvMemStorage* _storage, CvSVMKernel* _kernel,
                                double* _alpha, CvSVMSolutionInfo& _si )
{
    int i;
    double sum_pos, sum_neg, inv_r;
    //调用create函数，用于设置广义SMO算法的参数，在后面给出该函数的详细讲解
    //对于ν-SVC，select_working_set_func函数指针指向select_working_set_nu_svm，calc_rho_func函数指针指向calc_rho_nu_svm，get_row_func函数指针指向get_row_svc
    if( !create( _sample_count, _var_count, _samples, _y, _sample_count,
                 _alpha, 1., 1., _storage, _kernel, &CvSVMSolver::get_row_svc,
                 &CvSVMSolver::select_working_set_nu_svm, &CvSVMSolver::calc_rho_nu_svm ))
        return false;
    // sum_pos = sum_neg =νN/2，用于初始化β，详见原理部分的解释
    sum_pos = kernel->params->nu * sample_count * 0.5;
    sum_neg = kernel->params->nu * sample_count * 0.5;
    //遍历所有样本
    for( i = 0; i < sample_count; i++ )
    {
        if( y[i] > 0 )    //正例下，β的初始化
        {
            alpha[i] = MIN(1.0, sum_pos);
            sum_pos -= alpha[i];
        }
        else    //负例下，β的初始化
        {
            alpha[i] = MIN(1.0, sum_neg);
            sum_neg -= alpha[i];
        }
        b[i] = 0;    //b即为式148中的p，对于ν-SVC（式151），p为0
    }
    //调用solve_generic，执行广义SMO算法的迭代过程，该函数在后面给出详细的讲解
    if( !solve_generic( _si ))
        return false;
    //_si.r为原理部分式142下面段落中的ρ*，则inv_r就为1/ρ*
    inv_r = 1./_si.r;
    //遍历所有拉格朗日乘子，得到α＝α*/ρ*
    for( i = 0; i < sample_count; i++ )
        alpha[i] *= y[i]*inv_r;

    _si.rho *= inv_r;    //得到b＝b*/ρ*
    _si.obj *= (inv_r*inv_r);    //目标函数也要除以ρ*的平方
    _si.upper_bound_p = inv_r;    //赋值正界
    _si.upper_bound_n = inv_r;    //赋值负界

    return true;
}
```
用于求解单类SVM类型的广义SMO算法：










```cpp
bool CvSVMSolver::solve_one_class( int _sample_count, int _var_count, const float** _samples,
                                   CvMemStorage* _storage, CvSVMKernel* _kernel,
                                   double* _alpha, CvSVMSolutionInfo& _si )
{
    int i, n;
    double nu = _kernel->params->nu;    //得到参数ν
    //调用create函数，用于设置广义SMO算法的参数，在后面给出该函数的详细讲解
    //对于单类SVM，select_working_set_func函数指针指向select_working_set，calc_rho_func函数指针指向calc_rho，get_row_func函数指针指向get_row_one_class
    if( !create( _sample_count, _var_count, _samples, 0, _sample_count,
                 _alpha, 1., 1., _storage, _kernel, &CvSVMSolver::get_row_one_class,
                 &CvSVMSolver::select_working_set, &CvSVMSolver::calc_rho ))
        return false;
    //为变量y分配空间
    y = (schar*)cvMemStorageAlloc( storage, sample_count*sizeof(y[0]) );
    n = cvRound( nu*sample_count );    //定义n=[vN]
    //遍历所有样本
    for( i = 0; i < sample_count; i++ )
    {
        y[i] = 1;    //单类SVM的所有样本的响应值都设为1
        b[i] = 0;    //b即为式148中的p，对于ν-SVC（式152），p为0
        alpha[i] = i < n ? 1 : 0;    //初始化β，详见原理部分的解释
    }
    //初始化因四舍五入而产生的小数部分对应的β
    if( n < sample_count )
        alpha[n] = nu * sample_count - n;
    else
        alpha[n-1] = nu * sample_count - (n-1);
    //调用solve_generic，执行广义SMO算法的迭代过程，该函数在后面给出详细的讲解
    return solve_generic(_si);
}
```
用于求解ε-SVR类型的广义SMO算法：










```cpp
bool CvSVMSolver::solve_eps_svr( int _sample_count, int _var_count, const float** _samples,
                                 const float* _y, CvMemStorage* _storage,
                                 CvSVMKernel* _kernel, double* _alpha, CvSVMSolutionInfo& _si )
{
    int i;
    //p表示ε-SVR类型的参数ε，kernel_param_c表示惩罚参数C
    double p = _kernel->params->p, kernel_param_c = _kernel->params->C;
    //调用create函数，用于设置广义SMO算法的参数，在后面给出该函数的详细讲解
    //对于ε-SVR，select_working_set_func函数指针指向select_working_set，calc_rho_func函数指针指向calc_rho，get_row_func函数指针指向get_row_svr
    if( !create( _sample_count, _var_count, _samples, 0,
                 _sample_count*2, 0, kernel_param_c, kernel_param_c, _storage, _kernel, &CvSVMSolver::get_row_svr,
                 &CvSVMSolver::select_working_set, &CvSVMSolver::calc_rho ))
        return false;
    //为y和alpha开辟内存空间，它们的长度都为样本数量N的2倍
    y = (schar*)cvMemStorageAlloc( storage, sample_count*2*sizeof(y[0]) );
    alpha = (double*)cvMemStorageAlloc( storage, alpha_count*sizeof(alpha[0]) );
    //遍历所有样本，初始化参数
    for( i = 0; i < sample_count; i++ )
    {
        //前N个参数
        alpha[i] = 0;    //β初始化为0，前N个β即为α―
        //b即为式148中的p，对于ε-SVR（式154），前N个p为εeT－yT
        b[i] = p - _y[i];
        //y对应于式154中的z，对于ε-SVR（式154），前N个z为1
        y[i] = 1;
        //后N个参数
        alpha[i+sample_count] = 0;    //β初始化为0，后N个β即为α＋
        //b即为式148中的p，对于ε-SVR（式154），后N个p为εeT+yT
        b[i+sample_count] = p + _y[i];
        //y对应于式154中的z，对于ε-SVR（式154），后N个z为-1
        y[i+sample_count] = -1;
    }
    //调用solve_generic，执行广义SMO算法的迭代过程，该函数在后面给出详细的讲解
    if( !solve_generic( _si ))
        return false;
    //遍历所有样本，计算ε-SVR的决策函数（式105）中核函数前的系数α――α＋
    for( i = 0; i < sample_count; i++ )
        _alpha[i] = alpha[i] - alpha[i+sample_count];

    return true;
}
```
用于求解ν-SVR类型的广义SMO算法：










```cpp
bool CvSVMSolver::solve_nu_svr( int _sample_count, int _var_count, const float** _samples,
                                const float* _y, CvMemStorage* _storage,
                                CvSVMKernel* _kernel, double* _alpha, CvSVMSolutionInfo& _si )
{
    int i;
    // kernel_param_c表示惩罚参数C
    double kernel_param_c = _kernel->params->C, sum;
    //调用create函数，用于设置广义SMO算法的参数，在后面给出该函数的详细讲解
    //对于ν-SVR，select_working_set_func函数指针指向select_working_set_nu_svm，calc_rho_func函数指针指向calc_rho_nu_svm，get_row_func函数指针指向get_row_svr
    if( !create( _sample_count, _var_count, _samples, 0,
                 _sample_count*2, 0, 1., 1., _storage, _kernel, &CvSVMSolver::get_row_svr,
                 &CvSVMSolver::select_working_set_nu_svm, &CvSVMSolver::calc_rho_nu_svm ))
        return false;
    //为y和alpha开辟内存空间，它们的长度都为样本数量N的2倍
    y = (schar*)cvMemStorageAlloc( storage, sample_count*2*sizeof(y[0]) );
    alpha = (double*)cvMemStorageAlloc( storage, alpha_count*sizeof(alpha[0]) );
    //sum = CνN/2
    sum = kernel_param_c * _kernel->params->nu * sample_count * 0.5;
    //遍历所有样本，初始化参数
    for( i = 0; i < sample_count; i++ )
    {
        //初始化β，详见原理部分的解释
        alpha[i] = alpha[i + sample_count] = MIN(sum, kernel_param_c);
        sum -= alpha[i];
        //b即为式148中的p，对于ν-SVR（式156），前N个p为－yT
        b[i] = -_y[i];
        //y对应于式156中的z，对于ν-SVR（式156），前N个z为1
        y[i] = 1;
        //b即为式148中的p，对于ν-SVR（式156），后N个p为yT
        b[i + sample_count] = _y[i];
        //y对应于式156中的z，对于ν-SVR（式156），后N个z为-1
        y[i + sample_count] = -1;
    }
    //调用solve_generic，执行广义SMO算法的迭代过程，该函数在后面给出详细的讲解
    if( !solve_generic( _si ))
        return false;
    //遍历所有样本，计算ν-SVR的决策函数（式116）中核函数前的系数α――α＋
    for( i = 0; i < sample_count; i++ )
        _alpha[i] = alpha[i] - alpha[i+sample_count];

    return true;
}
```
create函数主要用于初始化和设置广义SMO算法的一些参数：










```cpp
bool CvSVMSolver::create( int _sample_count, int _var_count, const float** _samples, schar* _y,
                int _alpha_count, double* _alpha, double _Cp, double _Cn,
                CvMemStorage* _storage, CvSVMKernel* _kernel, GetRow _get_row,
                SelectWorkingSet _select_working_set, CalcRho _calc_rho )
//_sample_count表示训练样本的数量
//_var_count表示特征属性的数量
//_samples表示训练样本的数据集
//_y表示样本的响应值
//_alpha_count表示拉格朗日乘子αi的数量
//_alpha表示拉格朗日乘子αi的值
//_Cp表示对于SVC的正例的惩罚参数
//_Cn表示对于SVC的负例的惩罚参数
//_storage表示一块存储空间
//_kernel表示核函数
//_get_row表示得到矩阵Q的列的首地址
//_select_working_set表示选取工作集βi和βj
//_calc_rho表示计算ρ
{
    bool ok = false;    //该函数返回的标识变量
    int i, svm_type;    //svm_type表示SVM类型

    CV_FUNCNAME( "CvSVMSolver::create" );

    __BEGIN__;

    int rows_hdr_size;    //矩阵Q的列的首地址指针尺寸大小

    clear();    //清空一些全局变量

    sample_count = _sample_count;    //训练样本的数量
    var_count = _var_count;    //特征属性的数量
    samples = _samples;    //训练样本
    y = _y;    //样本响应值
    alpha_count = _alpha_count;    //拉格朗日乘子αi的数量
    alpha = _alpha;    //拉格朗日乘子αi的值
    kernel = _kernel;    //核函数

    C[0] = _Cn;    //负例的惩罚参数
    C[1] = _Cp;    //正例的惩罚参数
    eps = kernel->params->term_crit.epsilon;    //表示式170和式171中的ε
    max_iter = kernel->params->term_crit.max_iter;    //表示最大迭代次数
    storage = cvCreateChildMemStorage( _storage );    //开辟内存空间
    //为变量b，alpha_status，G和buf开辟内存空间
    //b表示式148中的参数p；alpha_status表示拉格朗日乘子αi（实质是βi）的状态，即βi是下界（βi=0），上界（βi=C），还是其他；G表示式149；buf在函数get_row调用时会用到
    b = (double*)cvMemStorageAlloc( storage, alpha_count*sizeof(b[0]));
    alpha_status = (schar*)cvMemStorageAlloc( storage, alpha_count*sizeof(alpha_status[0]));
    G = (double*)cvMemStorageAlloc( storage, alpha_count*sizeof(G[0]));
    for( i = 0; i < 2; i++ )
        buf[i] = (Qfloat*)cvMemStorageAlloc( storage, sample_count*2*sizeof(buf[i][0]) );
    svm_type = kernel->params->svm_type;    //SVM类型
    //函数指针赋值，该函数的作用是选择工作集βi和βj
    select_working_set_func = _select_working_set; 
    //如果函数指针select_working_set_func没有被赋值，则根据SVM的类型来确定该函数
    if( !select_working_set_func )
        //如果是ν-SVC或ν-SVR，select_working_set_func = select_working_set_nu_svm；如果是C-SVC、单类SVM或ε-SVR，select_working_set_func= select_working_set
        select_working_set_func = svm_type == CvSVM::NU_SVC || svm_type == CvSVM::NU_SVR ?
        &CvSVMSolver::select_working_set_nu_svm : &CvSVMSolver::select_working_set;
    //函数指针赋值，作用是计算ρ，即b
    calc_rho_func = _calc_rho; 
    //如果函数指针calc_rho_func没有被赋值，则根据SVM的类型来确定该函数
    if( !calc_rho_func )
        //如果是ν-SVC或ν-SVR，calc_rho_func = calc_rho_nu_svm；如果是C-SVC、单类SVM或ε-SVR，calc_rho_func = calc_rho
        calc_rho_func = svm_type == CvSVM::NU_SVC || svm_type == CvSVM::NU_SVR ?
            &CvSVMSolver::calc_rho_nu_svm : &CvSVMSolver::calc_rho;
    //函数指针赋值，作用是得到矩阵Q的列首地址
    get_row_func = _get_row; 
    //如果函数指针get_row_func没有被赋值，则根据SVM的类型来确定该函数
    if( !get_row_func )
        //如果是ε-SVR或ν-SVR，get_row_func = get_row_svr；如果是C-SVC或ν-SVC，get_row_func = get_row_svc；如果是单类SVM，get_row_func = get_row_one_class
        get_row_func = params->svm_type == CvSVM::EPS_SVR ||
                       params->svm_type == CvSVM::NU_SVR ? &CvSVMSolver::get_row_svr :
                       params->svm_type == CvSVM::C_SVC ||
                       params->svm_type == CvSVM::NU_SVC ? &CvSVMSolver::get_row_svc :
                       &CvSVMSolver::get_row_one_class;
    //定义cache_line_size和cache_size的大小，主要用于get_row_func函数
    cache_line_size = sample_count*sizeof(Qfloat);
    // cache size = max(num_of_samples^2*sizeof(Qfloat)*0.25, 64Kb)
    // (assuming that for large training sets ~25% of Q matrix is used)
    cache_size = MAX( cache_line_size*sample_count/4, CV_SVM_MIN_CACHE_SIZE );

    // the size of Q matrix row headers
    //定义rows_hdr_size大小
    rows_hdr_size = sample_count*sizeof(rows[0]);
    if( rows_hdr_size > storage->block_size )
        CV_ERROR( CV_StsOutOfRange, "Too small storage block size" );
    //定义lru_list变量
    lru_list.prev = lru_list.next = &lru_list;
    rows = (CvSVMKernelRow*)cvMemStorageAlloc( storage, rows_hdr_size );
    memset( rows, 0, rows_hdr_size );

    ok = true;    //设置正确返回变量

    __END__;

    return ok;    //返回
}
```
广义SMO算法的迭代过程：











```cpp
bool CvSVMSolver::solve_generic( CvSVMSolutionInfo& si )
{
    int iter = 0;    //用于记录迭代的次数
    int i, j, k;

    // 1. initialize gradient and alpha status
    //遍历所有拉格朗日乘子αi，得到αi的状态，程序中的α实质是式148的β
    for( i = 0; i < alpha_count; i++ ) 
    {
/****************
#define update_alpha_status(i) \
    alpha_status[i] = (schar)(alpha[i] >= get_C(i) ? 1 : alpha[i] <= 0 ? -1 : 0)
****************/
        //宏定义update_alpha_status表示得到拉格朗日乘子αi的状态：当αi等于惩罚参数C时，alpha_status[i]等于1；当αi=0时，alpha_status[i]等于-1；当0<αi<C时，alpha_status[i]等于0。其中宏定义get_C表示得到正例和负例的惩罚参数C
        update_alpha_status(i);
        G[i] = b[i];    //式149，先初始化梯度为p，数组b已经被赋值为向量p
        if( fabs(G[i]) > 1e200 )    //确保变量G[i]不能过大
            return false;
    }

    for( i = 0; i < alpha_count; i++ )    //遍历所有拉格朗日乘子αi
    {
/*******************
#define is_lower_bound(i) (alpha_status[i] < 0)
#define is_upper_bound(i) (alpha_status[i] > 0)
*******************/
        //宏定义is_lower_bound用于表示αi是否为下界，即是否等于0，如果等于0，则is_lower_bound为1，否则为0；宏定义is_upper_bound用于表示αi是否为上界，即是否等于C，如果等于C，则is_upper_bound为1，否则为0
        if( !is_lower_bound(i) )    //如果αi≠0
        {
            //函数get_row表示得到矩阵Q的第i列首地址，Q即为式148中的Q，虽然Qij都可以表示为zizjK(xi, xj)，但不同类型的SVM的zi含义不同，因此在get_row函数内，先通过get_row_base函数得到核函数K(xi, xj)的第i列，然后再调用get_row_func函数，前面已经分析过了，不同的SVM，get_row_func函数的函数指针会指向不同的函数
            const Qfloat *Q_i = get_row( i, buf[0] );
            double alpha_i = alpha[i];    //得到αi值，实质为βi
            //计算式149，得到f(β)的梯度向量▽f(β)，G[j]表示▽f(β)向量的第j个元素
            for( j = 0; j < alpha_count; j++ )
                G[j] += alpha_i*Q_i[j];
        }
    }

    // 2. optimization loop
    for(;;)    //优化β的迭代死循环
    {
        // Q_i和Q_j分别表示式148中的Q的第i列和第j列的首地址
        const Qfloat *Q_i, *Q_j;
        //C_i和C_j分别表示第i个和第j个样本所对应的惩罚参数C，如图10所示
        double C_i, C_j;
        //old_alpha_i、old_alpha_j、alpha_i和alpha_j分别表示式161中βik、βjk、βinew和βjnew
        double old_alpha_i, old_alpha_j, alpha_i, alpha_j;
        // delta_alpha_i和delta_alpha_j分别表示每次迭代前后的βi的差值和βj的差值
        double delta_alpha_i, delta_alpha_j;

#ifdef _DEBUG
        for( i = 0; i < alpha_count; i++ )
        {
            if( fabs(G[i]) > 1e+300 )
                return false;

            if( fabs(alpha[i]) > 1e16 )
                return false;
        }
#endif
        //当满足终止条件或超过最大迭代次数，则退出迭代死循环
        // select_working_set_func函数用于工作集βi和βj的选取，如果该函数的返回值为1，则表示终止迭代循环，max_iter表示最大迭代次数。前面已经分析过，不同的SVM，select_working_set_func函数的函数指针会指向不同的函数，第一类SVM指向的是select_working_set函数，第二类SVM指向的是select_working_set_nu_svm，这两个函数在后面都会给出详细的讲解
        if( (this->*select_working_set_func)( i, j ) != 0 || iter++ >= max_iter )
            break;
        //分别得到矩阵Q的第i列和第j列的首地址
        Q_i = get_row( i, buf[0] );
        Q_j = get_row( j, buf[1] );
        //分别得到第i个和第j个样本的C值
        C_i = get_C(i);
        C_j = get_C(j);
        //分别得到第i个和第j个样本的β值
        alpha_i = old_alpha_i = alpha[i];
        alpha_j = old_alpha_j = alpha[j];

        if( y[i] != y[j] )    //zi≠zj时，式160中的第1行
        {
            double denom = Q_i[i]+Q_j[j]+2*Q_i[j];    //得到式160中第1行的aij
            //得到式162中分式的分子部分，即－▽if(β)－▽jf(β)
            double delta = (-G[i]-G[j])/MAX(fabs(denom),FLT_EPSILON);
            double diff = alpha_i - alpha_j;    //得到式164中的T
            //得到式162中的βinew和βjnew
            alpha_i += delta;
            alpha_j += delta;
            //如图10(a)，限制β的值在矩形区域内
            if( diff > 0 && alpha_j < 0 )    //区域Ⅲ
            {
                alpha_j = 0;
                alpha_i = diff;
            }
            else if( diff <= 0 && alpha_i < 0 )    //区域Ⅳ
            {
                alpha_i = 0;
                alpha_j = -diff;
            }

            if( diff > C_i - C_j && alpha_i > C_i )    //区域Ⅰ
            {
                alpha_i = C_i;
                alpha_j = C_i - diff;
            }
            else if( diff <= C_i - C_j && alpha_j > C_j )    //区域Ⅱ
            {
                alpha_j = C_j;
                alpha_i = C_j + diff;
            }
        }
        else    //zi＝zj时，式160中的第2行
        {
            double denom = Q_i[i]+Q_j[j]-2*Q_i[j];    //得到式160中第2行的ats
            //得到式163中分式的分子部分，即▽if(β)－▽jf(β)
            double delta = (G[i]-G[j])/MAX(fabs(denom),FLT_EPSILON);
            double sum = alpha_i + alpha_j;    //得到式165中的S
            //得到式163中的βinew和βjnew
            alpha_i -= delta;
            alpha_j += delta;
            //如图10(b)，限制β的值在矩形区域内
            if( sum > C_i && alpha_i > C_i )    //区域Ⅰ
            {
                alpha_i = C_i;
                alpha_j = sum - C_i;
            }
            else if( sum <= C_i && alpha_j < 0)    //区域Ⅱ
            {
                alpha_j = 0;
                alpha_i = sum;
            }

            if( sum > C_j && alpha_j > C_j )    //区域Ⅲ
            {
                alpha_j = C_j;
                alpha_i = sum - C_j;
            }
            else if( sum <= C_j && alpha_i < 0 )    //区域Ⅳ
            {
                alpha_i = 0;
                alpha_j = sum;
            }
        }

        // update alpha
        //更新βi和βj，以及它们的状态
        alpha[i] = alpha_i;
        alpha[j] = alpha_j;
        update_alpha_status(i);
        update_alpha_status(j);

        // update G
        //得到迭代前后βi的差值和βj的差值
        delta_alpha_i = alpha_i - old_alpha_i;
        delta_alpha_j = alpha_j - old_alpha_j;
        //更新▽f(β)
        for( k = 0; k < alpha_count; k++ )
            G[k] += Q_i[k]*delta_alpha_i + Q_j[k]*delta_alpha_j;
    }    //迭代优化结束

    // calculate rho
    // calc_rho_func函数计算ρ，即决策函数中的参数b。前面已经分析过，不同类型的SVM，calc_rho_func函数的函数指针会指向不同的函数，第一类SVM指向的是calc_rho函数，第二类SVM指向的是calc_rho_nu_svm，这两个函数在后面都会给出详细的讲解
    (this->*calc_rho_func)( si.rho, si.r );

    // calculate objective value
    //计算式148，得到目标函数f(β)的值
    //f(β)＝0.5×[βT×(▽f(β)+p)]＝0.5×[βT×(Qβ＋p)+p)]＝0.5×βTQβ＋pTβ
    for( i = 0, si.obj = 0; i < alpha_count; i++ )
        si.obj += alpha[i] * (G[i] + b[i]);

    si.obj *= 0.5;

    si.upper_bound_p = C[1];    //正例的惩罚参数C
    si.upper_bound_n = C[0];    //负例的惩罚参数C

    return true;
}
```
第一类SVM（C-SVC、单类SVM和ε-SVR）的选取工作集*βi*和*βj*的函数：











```cpp
bool
CvSVMSolver::select_working_set( int& out_i, int& out_j )
//out_i和out_j分别表示βi和βj
{
    // return i,j which maximize -grad(f)^T d , under constraint
    // if alpha_i == C, d != +1
    // if alpha_i == 0, d != -1
    //定义一个最大值
    double Gmax1 = -DBL_MAX;        // max { -grad(f)_i * d | y_i*d = +1 }
    int Gmax1_idx = -1;    //初始化i=-1
    //定义一个最大值
    double Gmax2 = -DBL_MAX;        // max { -grad(f)_i * d | y_i*d = -1 }
    int Gmax2_idx = -1;    //初始化j＝－1

    int i;
    //遍历所有β
    for( i = 0; i < alpha_count; i++ )
    {
        double t;

        if( y[i] > 0 )    // y = +1，即式166中的zt＝1
        {
            if( !is_upper_bound(i) && (t = -G[i]) > Gmax1 )  // d = +1
            //表示满足式166中的第一行max中的第一项
            {
                Gmax1 = t;    //更新最大值
                Gmax1_idx = i;    //得到工作集中第一个βi的索引值i
            }
            if( !is_lower_bound(i) && (t = G[i]) > Gmax2 )  // d = -1
            //表示满足式166中的第二行max中的第二项
            {
                Gmax2 = t;    //更新最大值
                Gmax2_idx = i;    //得到工作集中第二个βj的索引值j
            }
        }
        else        // y = -1，即式166中的zt＝－1
        {
            if( !is_upper_bound(i) && (t = -G[i]) > Gmax2 )  // d = +1
            //表示满足式166中的第二行max中的第一项
            {
                Gmax2 = t;    //更新最大值
                Gmax2_idx = i;    //得到工作集中第二个βj的索引值j
            }
            if( !is_lower_bound(i) && (t = G[i]) > Gmax1 )  // d = -1
            //表示满足式166中的第一行max中的第二项
            {
                Gmax1 = t;    //更新最大值
                Gmax1_idx = i;    //得到工作集中第一个βi的索引值i
            }
        }
    }
    //为βi和βj赋值
    out_i = Gmax1_idx;
    out_j = Gmax2_idx;
    //式170，判断迭代是否终止
    return Gmax1 + Gmax2 < eps;
}
```
第二类SVM（ν-SVC和ν-SVR）的选取工作集*βi*和*βj*的函数：











```cpp
bool
CvSVMSolver::select_working_set_nu_svm( int& out_i, int& out_j )
{
    // return i,j which maximize -grad(f)^T d , under constraint
    // if alpha_i == C, d != +1
    // if alpha_i == 0, d != -1
    //定义一个最大值
    double Gmax1 = -DBL_MAX;    // max { -grad(f)_i * d | y_i = +1, d = +1 }
    int Gmax1_idx = -1;    //初始化ip

    double Gmax2 = -DBL_MAX;    // max { -grad(f)_i * d | y_i = +1, d = -1 }
    int Gmax2_idx = -1;    //初始化jp

    double Gmax3 = -DBL_MAX;    // max { -grad(f)_i * d | y_i = -1, d = +1 }
    int Gmax3_idx = -1;    //初始化in

    double Gmax4 = -DBL_MAX;    // max { -grad(f)_i * d | y_i = -1, d = -1 }
    int Gmax4_idx = -1;    //初始化jn

    int i;
    //遍历所有β
    for( i = 0; i < alpha_count; i++ )
    {
        double t;

        if( y[i] > 0 )    // y == +1
        //第一次由zi＝1这个条件得到ip和jp
        {
            if( !is_upper_bound(i) && (t = -G[i]) > Gmax1 )  // d = +1
            //式167的第一行公式
            {
                Gmax1 = t;    //更新最大值
                Gmax1_idx = i;    //得到ip
            }
            if( !is_lower_bound(i) && (t = G[i]) > Gmax2 )  // d = -1
            //式167的第二行公式
            {
                Gmax2 = t;    //更新最大值
                Gmax2_idx = i;    //得到jp
            }
        }
        else        // y == -1
        //第二次由zi＝－1这个条件得到in和jn
        {
            if( !is_upper_bound(i) && (t = -G[i]) > Gmax3 )  // d = +1
            //式168的第一行公式
            {
                Gmax3 = t;    //更新最大值
                Gmax3_idx = i;    //得到in
            }
            if( !is_lower_bound(i) && (t = G[i]) > Gmax4 )  // d = -1
            //式168的第二行公式
            {
                Gmax4 = t;    //更新最大值
                Gmax4_idx = i;    //得到jn
            }
        }
    }
    //式171，判断迭代是否终止
    if( MAX(Gmax1 + Gmax2, Gmax3 + Gmax4) < eps )
        return 1;
    //得到最终的工作集βi和βj
    if( Gmax1 + Gmax2 > Gmax3 + Gmax4 )    //式169的第一行公式
    {
        out_i = Gmax1_idx;
        out_j = Gmax2_idx;
    }
    else    //式169的第二行公式
    {
        out_i = Gmax3_idx;
        out_j = Gmax4_idx;
    }
    return 0;
}
```
第一类SVM（C-SVC、单类SVM和ε-SVR）的计算决策函数中参数*b*的函数：










```cpp
void
CvSVMSolver::calc_rho( double& rho, double& r )
{
    int i, nr_free = 0;
    double ub = DBL_MAX, lb = -DBL_MAX, sum_free = 0;
    //遍历所有β
    for( i = 0; i < alpha_count; i++ )
    {
        double yG = y[i]*G[i];    //式172的分子中的一项

        if( is_lower_bound(i) )    //βi＝0
        {
            if( y[i] > 0 )
                ub = MIN(ub,yG);    //式175中min的前一项内容
            else
                lb = MAX(lb,yG);    //式174中max的前一项内容
        }
        else if( is_upper_bound(i) )    //βi＝C
        {
            if( y[i] < 0)
                ub = MIN(ub,yG);    //式175中min的后一项内容
            else
                lb = MAX(lb,yG);    //式174中max的后一项内容
        }
        else    //0＜βi＜C
        {
            ++nr_free;    //式172的分母，即计数
            sum_free += yG;    //式172的分子求和
        }
    }
    //如果nr_free大于0，说明有样本的βi的值在0和C之间，则利用式172计算ρ；否则利用式173计算ρ
    rho = nr_free > 0 ? sum_free/nr_free : (ub + lb)*0.5;
    r = 0;
}
```
第二类SVM（ν-SVC和ν-SVR）的计算决策函数中参数*b*的函数：




```cpp
void
CvSVMSolver::calc_rho_nu_svm( double& rho, double& r )
{
    int nr_free1 = 0, nr_free2 = 0;
    double ub1 = DBL_MAX, ub2 = DBL_MAX;
    double lb1 = -DBL_MAX, lb2 = -DBL_MAX;
    double sum_free1 = 0, sum_free2 = 0;
    double r1, r2;

    int i;
    //遍历所有β
    for( i = 0; i < alpha_count; i++ )
    {
        double G_i = G[i];    //得到▽if(β)
        if( y[i] > 0 )    
        {
            if( is_lower_bound(i) )    //式178分子中的第二项
                ub1 = MIN( ub1, G_i );
            else if( is_upper_bound(i) )    //式178分子中的第一项
                lb1 = MAX( lb1, G_i );
            else    //式176
            {
                ++nr_free1;    //式176的分母，即计数
                sum_free1 += G_i;    //式176的分子，求和
            }
        }
        else
        {
            if( is_lower_bound(i) )    //式179分子中的第二项
                ub2 = MIN( ub2, G_i );
            else if( is_upper_bound(i) )    //式179分子中的第一项
                lb2 = MAX( lb2, G_i );
            else    //式177
            {
                ++nr_free2;    //式177的分母，即计数
                sum_free2 += G_i;    //式177的分子，求和
            }
        }
    }
    //如果nr_free1大于0，说明在zi＝1下有样本的βi的值在0和C之间，则利用式176计算r1；否则利用式178计算r1
    r1 = nr_free1 > 0 ? sum_free1/nr_free1 : (ub1 + lb1)*0.5;
    //如果nr_free2大于0，说明在zi＝－1下有样本的βi的值在0和C之间，则利用式177计算r2；否则利用式179计算r2
    r2 = nr_free2 > 0 ? sum_free2/nr_free2 : (ub2 + lb2)*0.5;

    rho = (r1 - r2)*0.5;    //式180，得到－b
    r = (r1 + r2)*0.5;    //式180，得到ρ，该变量用于ν-SVC
}
```
优化那些使用线性核函数（式53）的SVM，目的是用一个支持向量来代表所有的支持向量：








```cpp
void CvSVM::optimize_linear_svm()
{
    // we optimize only linear SVM: compress all the support vectors into one.
    if( params.kernel_type != LINEAR )    //判断是否为线性核函数
        return;
    //如果是SVC，则得到分类的数量；如果是单类SVM，则class_count为1；如果是SVR，则class_count为0
    int class_count = class_labels ? class_labels->cols :
            params.svm_type == CvSVM::ONE_CLASS ? 1 : 0;
    // df_count为决策函数的数量，在多个分类的SVC中，该值为class_count*(class_count-1)/2，其他情况该值为1
    int i, df_count = class_count > 1 ? class_count*(class_count-1)/2 : 1;
    CvSVMDecisionFunc* df = decision_func;    //赋值决策函数指针
    //遍历所有的决策函数，如果当前决策函数的支持向量的数量不为1，则退出该循环
    for( i = 0; i < df_count; i++ )
    {
        int sv_count = df[i].sv_count;
        if( sv_count != 1 )
            break;
    }

    // if every decision functions uses a single support vector;
    // it's already compressed. skip it then.
    //i为前面for循环中的循环次数索引，如果i等于df_count，说明所有的决策函数中的支持向量的数量都为1个，所以无需再优化处理，则退出该函数
    if( i == df_count )
        return;

    int var_count = get_var_count();    //得到特征属性的数量
    cv::AutoBuffer<double> vbuf(var_count);    //为vbuf分配内存
    double* v = vbuf;    //赋值
    //为new_sv变量分配内存，new_sv表示每个决策函数优化处理后的新的支持向量
    float** new_sv = (float**)cvMemStorageAlloc(storage, df_count*sizeof(new_sv[0]));
    //遍历所有的决策函数，优化处理每个决策函数
    for( i = 0; i < df_count; i++ )
    {
        //为第i个决策函数的新的支持向量new_sv[i]分配内存
        new_sv[i] = (float*)cvMemStorageAlloc(storage, var_count*sizeof(new_sv[i][0]));
        float* dst = new_sv[i];    //赋值
        memset(v, 0, var_count*sizeof(v[0]));    //清零
        int j, k, sv_count = df[i].sv_count;
        //遍历当前决策函数的所有支持向量
        for( j = 0; j < sv_count; j++ )
        {
            //得到当前支持向量所对应的样本
            const float* src = class_count > 1 && df[i].sv_index ? sv[df[i].sv_index[j]] : sv[j];
            double a = df[i].alpha[j];    //得到当前支持向量的αi
            //遍历该样本的所有特征属性，得到一个新的支持向量v＝∑ixiαi
            for( k = 0; k < var_count; k++ )
                v[k] += src[k]*a;
        }
        for( k = 0; k < var_count; k++ )
            dst[k] = (float)v[k];    //赋值
        df[i].sv_count = 1;    //此时，只有一个支持向量v
        df[i].alpha[0] = 1.;    //根据约束条件，只有一个支持向量时，α=1
        if( class_count > 1 && df[i].sv_index )
            df[i].sv_index[0] = i;    //索引赋值
    }

    sv = new_sv;    //新的支持向量
    //支持向量的数量，它等于决策函数的数量，因为此时每个决策函数只有一个支持向量
    sv_total = df_count; 
}
```



在前面应用get_row函数计算矩阵Q（即式148中的*Q*=*zizjK*(*xi*,*xj*)）的第i列首地址时，涉及到计算核函数*K*(*xi*,*xj*)，即调用calc函数。在用于SVM对样本进行预测时，需要计算决策函数，此时也需要计算核函数*K*(*xi*,*xj*)。但两者还是有区别的，在计算*Q*时，要用到所有的样本，而计算决策函数时，只需要用到支持向量即可，也就是说在计算*Q*时，用于表示支持向量的变量用全体样本来替代。

下面我们就来介绍calc函数：








```cpp
void CvSVMKernel::calc( int vcount, int var_count, const float** vecs,
                        const float* another, Qfloat* results )
// vcount表示支持向量的数量
// var_count表示样本的特征属性的数量
// vecs表示具体的支持向量
// another表示核函数所需要的另一个样本数据，如果是预测，则为预测样本
//results表示最终的核函数结果
{
    const Qfloat max_val = (Qfloat)(FLT_MAX*1e-3);
    int j;
    //调用calc_func函数
    //在实例化CvSVMKernel时，会通过create函数为calc_func的函数指针赋值：当为线性核函数（式53）时，calc_func函数为calc_linear；当为多项式核函数（式54）时，calc_func函数为calc_poly；当为高斯核函数（式56）时，calc_func函数为calc_rbf；当为Sigmoid核函数（式57）时，calc_func函数为calc_sigmoid。这4个函数在后面都有介绍
    (this->*calc_func)( vcount, var_count, vecs, another, results );
    //遍历支持向量，确保核函数的向量中不能有太大的元素
    for( j = 0; j < vcount; j++ )
    {
        if( results[j] > max_val )
            results[j] = max_val;
    }
}
```
计算线性核函数：











```cpp
void CvSVMKernel::calc_linear( int vcount, int var_count, const float** vecs,
                               const float* another, Qfloat* results )
{
    //调用calc_non_rbf_base函数，计算α xi·xj＋β，对于线性核函数来说，α＝1，β＝0，该函数在后面会给出讲解
    calc_non_rbf_base( vcount, var_count, vecs, another, results, 1, 0 );
}
```
计算多项式核函数：











```cpp
void CvSVMKernel::calc_poly( int vcount, int var_count, const float** vecs,
                             const float* another, Qfloat* results )
{
    CvMat R = cvMat( 1, vcount, QFLOAT_TYPE, results );
    //调用calc_non_rbf_base函数，计算α xi·xj＋β，对于多项式核函数（式54）来说，α＝γ，β＝p，γ为params->gamma，p为params->coef0，该函数在后面会给出讲解
    calc_non_rbf_base( vcount, var_count, vecs, another, results, params->gamma, params->coef0 );
    //最终得到式54，式中的q为params->degree
    if( vcount > 0 )
        cvPow( &R, &R, params->degree );
}
```
计算Sigmoid核函数：











```cpp
void CvSVMKernel::calc_sigmoid( int vcount, int var_count, const float** vecs,
                                const float* another, Qfloat* results )
{
    int j;
    //调用calc_non_rbf_base函数，计算－2(γ xi·xj＋p)，对于Sigmoid核函数（式57）来说，α＝－2γ，β＝－2p，γ为params->gamma，p为params->coef0，该函数在后面会给出讲解
    calc_non_rbf_base( vcount, var_count, vecs, another, results,
                       -2*params->gamma, -2*params->coef0 );
    // TODO: speedup this
    for( j = 0; j < vcount; j++ )    //计算式57
    {
        Qfloat t = results[j];    //得到e的指数部分，即－2(γ xi·xj＋p)
        double e = exp(-fabs(t));    //得到e指数
        //得到双曲正切
        if( t > 0 )
            results[j] = (Qfloat)((1. - e)/(1. + e));
        else
            results[j] = (Qfloat)((e - 1.)/(e + 1.));
    }
}
```
计算高斯（径向基函数）核函数：











```cpp
void CvSVMKernel::calc_rbf( int vcount, int var_count, const float** vecs,
                            const float* another, Qfloat* results )
{
    //定义一个矩阵，作为高斯核函数的结果输出
    CvMat R = cvMat( 1, vcount, QFLOAT_TYPE, results ); 
    double gamma = -params->gamma;    //得到式55中的参数－γ
    int j, k;
    //遍历所有支持向量
    for( j = 0; j < vcount; j++ )
    {
        const float* sample = vecs[j];    //得到当前支持向量，即样本数据
        double s = 0;
        //以4个为一个单位，遍历当前样本的特征属性，计算||xi－xj||
        for( k = 0; k <= var_count - 4; k += 4 )
        {
            double t0 = sample[k] - another[k];
            double t1 = sample[k+1] - another[k+1];

            s += t0*t0 + t1*t1;

            t0 = sample[k+2] - another[k+2];
            t1 = sample[k+3] - another[k+3];

            s += t0*t0 + t1*t1;
        }
        //计算不足4个的，也就是剩余的特征属性的||xi－xj||
        for( ; k < var_count; k++ )
        {
            double t0 = sample[k] - another[k];
            s += t0*t0;
        }
        results[j] = (Qfloat)(s*gamma);    //得到－γ||xi－xj||
    }

    if( vcount > 0 )
        cvExp( &R, &R );    //得到最终的结果，即式56
}
```
除了calc_rbf函数以外，calc_linear函数，calc_poly函数和calc_sigmoid函数都会调用calc_non_rbf_base函数，因为这3个核函数都需要计算**x***i***·x***j*：











```cpp
void CvSVMKernel::calc_non_rbf_base( int vcount, int var_count, const float** vecs,
                                     const float* another, Qfloat* results,
                                     double alpha, double beta )
{
    int j, k;
    //遍历所有支持向量
    for( j = 0; j < vcount; j++ )
    {
        const float* sample = vecs[j];    //得到当前的支持向量，即样本数据
        double s = 0;
        //以4个为一个单位，遍历当前样本的特征属性，计算xi·xj
        for( k = 0; k <= var_count - 4; k += 4 )
            s += sample[k]*another[k] + sample[k+1]*another[k+1] +
                 sample[k+2]*another[k+2] + sample[k+3]*another[k+3];
        //计算不足4个的，也就是剩余的特征属性的xi·xj
        for( ; k < var_count; k++ )
            s += sample[k]*another[k];
        //最终得到α xi·xj＋β
        results[j] = (Qfloat)(s*alpha + beta);
    }
}
```

下面介绍SVM预测样本函数predict。该函数有许多形式，如：

float CvSVM::predict(const Mat& sample,bool returnDFVal=false )

float CvSVM::predict(const CvMat* sample,bool returnDFVal=false )

float CvSVM::predict(const CvMat* samples,CvMat* results)

其中，sample表示需要预测的样本数据，returnDFVal定义该函数的返回类型，为true，并且是两类分类问题时，该函数返回决策函数中sgn函数内的具体值，否则该函数返回分类标签（SVC），或返回估计函数（SVR），results表示相对应样本的预测输出，如果只预测一个样本，则该函数返回预测结果，如果预测多个样本，则必须使用results参数来返回预测结果。

不管哪种形式的predict函数，最终都会调用下面这个函数：











```cpp
float CvSVM::predict( const float* row_sample, int row_len, bool returnDFVal ) const
// row_sample表示待预测的一个样本
// row_len表示该样本的特征属性的数量
//函数返回为预测结果
{
    assert( kernel );    //确保SVM模型中核函数数据形式的正确
    assert( row_sample );    //确保预测样本数据形式的正确

    int var_count = get_var_count();    //得到SVM模型中样本的特征属性的数量
    //确保SVM模型的样本和预测样本的特征属性的数量相同
    assert( row_len == var_count );
    (void)row_len;
    //如果是SVC，class_count为分类的数量；如果是单类SVM，class_count为1；如果是SVR，class_count为0
    int class_count = class_labels ? class_labels->cols :
                  params.svm_type == ONE_CLASS ? 1 : 0;

    float result = 0;    //表示预测结果
    cv::AutoBuffer<float> _buffer(sv_total + (class_count+1)*2);    //分配一块内存空间
    float* buffer = _buffer;

    if( params.svm_type == EPS_SVR ||    //ε-SVR类型
        params.svm_type == NU_SVR ||    //ν-SVR类型
        params.svm_type == ONE_CLASS )    //单类SVM类型
    {
        CvSVMDecisionFunc* df = (CvSVMDecisionFunc*)decision_func;    //得到决策函数
        int i, sv_count = df->sv_count;    //sv_count表示支持向量的数量
        double sum = -df->rho;    //得到决策函数的参数b
        //计算核函数
        kernel->calc( sv_count, var_count, (const float**)sv, row_sample, buffer );
        //遍历所有的支持向量，由决策函数得到最终的预测结果
        for( i = 0; i < sv_count; i++ )
            sum += buffer[i]*df->alpha[i];
        //如果是单类SVM，则对sum取sgn，即式90
        result = params.svm_type == ONE_CLASS ? (float)(sum > 0) : (float)sum;
    }
    else if( params.svm_type == C_SVC ||    //C-SVC类型
             params.svm_type == NU_SVC )    //ν-SVC类型
    {
        CvSVMDecisionFunc* df = (CvSVMDecisionFunc*)decision_func;    //得到决策函数
        int* vote = (int*)(buffer + sv_total);    //表示记录投票结果
        int i, j, k;
        //为vote开辟内存并清零
        memset( vote, 0, class_count*sizeof(vote[0]));
        //计算核函数
        kernel->calc( sv_total, var_count, (const float**)sv, row_sample, buffer );
        double sum = 0.;

        for( i = 0; i < class_count; i++ )    //遍历所有分类类别
        {
            //对不重复的两个分类进行比较
            for( j = i+1; j < class_count; j++, df++ )
            {
                sum = -df->rho;    //得到决策函数的参数b
                int sv_count = df->sv_count;
                //遍历所有的支持向量，由决策函数得到最终的预测结果
                for( k = 0; k < sv_count; k++ )
                    sum += df->alpha[k]*buffer[df->sv_index[k]];

                vote[sum > 0 ? i : j]++;    //统计投票结果
            }
        }
        //遍历所有分类类别，得到票数最多的那个分类类别
        for( i = 1, k = 0; i < class_count; i++ )
        {
            if( vote[i] > vote[k] )
                k = i;    //k表示票数最多的那个分类类别
        }
        //如果returnDFVal为true，并且是2类的SVM，则result为决策函数中sgn函数内的值，否则result为预测的分类标签
        result = returnDFVal && class_count == 2 ? (float)sum : (float)(class_labels->data.i[k]);
    }
    else    //不是以上5种SVM类型，则提示错误信息
        CV_Error( CV_StsBadArg, "INTERNAL ERROR: Unknown SVM type, "
                                "the SVM structure is probably corrupted" );

    return result;    //返回预测结果
}
```


下面是train_auto的函数原型 



C++: bool CvSVM::**train_auto**(const Mat**& trainData**,

　　　　　　　　const Mat**& responses**, 



const Mat**& varIdx**, 



const Mat**& sampleIdx**, 



CvSVMParams **params**,



int **k_fold=10**, 



CvParamGrid **Cgrid=CvSVM::get_default_grid(CvSVM::C)**, 



CvParamGrid **gammaGrid=CvSVM::get_default_grid(CvSVM::GAMMA)**, 



CvParamGrid **pGrid=CvSVM::get_default_grid(CvSVM::P)**, 



CvParamGrid **nuGrid=CvSVM::get_default_grid(CvSVM::NU)**, 



CvParamGrid **coeffGrid=CvSVM::get_default_grid(CvSVM::COEF)**, 



CvParamGrid **degreeGrid=CvSVM::get_default_grid(CvSVM::DEGREE)**, 



bool **balanced=false**



)


      自动训练函数的参数注释(13个)


前5个参数参考构造函数的参数注释。**k_fold: **交叉验证参数。训练集被分成k_fold的自子集。其中一个子集是用来测试模型，其他子集则成为训练集。所以，SVM算法复杂度是执行k_fold的次数。***Grid: **(6个)对应的SVM迭代网格参数。**balanced: **如果是true则这是一个2类分类问题。这将会创建更多的平衡交叉验证子集。

**    自动训练函数的使用说明**


这个方法根据CvSVMParams中的最佳参数C, gamma, p, nu, coef0, degree自动训练SVM模型。参数被认为是最佳的交叉验证，其测试集预估错误最小。如果没有需要优化的参数，相应的网格步骤应该被设置为小于或等于1的值。例如，为了避免gamma的优化，设置gamma_grid.step = 0，gamma_grid.min_val， gamma_grid.max_val 为任意数值。所以params.gamma 由gamma得出。最后，如果参数优化是必需的，但是相应的网格却不确定，你可能需要调用函数CvSVM::get_default_grid()，创建一个网格。例如，对于gamma，调用CvSVM::get_default_grid(CvSVM::GAMMA)。该函数为分类运行 (params.svm_type=CvSVM::C_SVC 或者 params.svm_type=CvSVM::NU_SVC) 和为回归运行 (params.svm_type=CvSVM::EPS_SVR 或者 params.svm_type=CvSVM::NU_SVR)效果一样好。如果params.svm_type=CvSVM::ONE_CLASS，没有优化，并指定执行一般的SVM。

     这里需要注意的是，对于需要的优化的参数虽然train_auto可以自动选择最优值，但在代码中也要先赋初始值，要不然编译能通过，但运行时会报错。下面是示例代码


	CvSVMParams param;  
	param.svm_type = CvSVM::EPS_SVR;  
	param.kernel_type = CvSVM::RBF;  
	param.C = 1;  //给参数赋初始值
	param.p = 5e-3;  //给参数赋初始值
	param.gamma = 0.01;  //给参数赋初始值
	param.term_crit = cvTermCriteria(CV_TERMCRIT_EPS, 100, 5e-3); 
	//对不用的参数step设为0
	CvParamGrid nuGrid = CvParamGrid(1,1,0.0);
	CvParamGrid coeffGrid = CvParamGrid(1,1,0.0);
	CvParamGrid degreeGrid = CvParamGrid(1,1,0.0);

	CvSVM regressor;
	regressor.train_auto(PCA_training,tr_label,NULL,NULL,param,
		10,
		regressor.get_default_grid(CvSVM::C),
		regressor.get_default_grid(CvSVM::GAMMA),
		regressor.get_default_grid(CvSVM::P),
		nuGrid,
		coeffGrid,
		degreeGrid);


      用上面的代码的就可以自动训练优化出参数了，最后想查看优化后的参数值可以使用CvSVMParams params_re = regressor.get_params()函数来获得各优化后的参数值。


CvSVMParams params_re = regressor.get_params();
	regressor.save("training_srv.xml");
	float C = params_re.C;
	float P = params_re.p;
	float gamma = params_re.gamma;
	printf("\nParms: C = %f, P = %f,gamma = %f \n",C,P,gamma);




应用实例



下面我们给出一个具体的SVM应用实例。本次的实例来源于http://archive.ics.uci.edu/ml/中的risi数据，目的是用于判断样本是属于哪类植物：setosa，versicolor，virginica。每类植物各选择20个样本进行训练，而每个样本具有以下4个特征属性：花萼长，花萼宽，花瓣长，花瓣宽。











```cpp
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"

#include <iostream>
using namespace cv;
using namespace std;

int main( int argc, char** argv )
{	
    //60个训练样本
    double trainingData[60][4]={{5.1,3.5,1.4,0.2}, {4.9,3.0,1.4,0.2}, {4.7,3.2,1.3,0.2},
                            {4.6,3.1,1.5,0.2}, {5.0,3.6,1.4,0.2}, {5.4,3.9,1.7,0.4},
                            {4.6,3.4,1.4,0.3}, {5.0,3.4,1.5,0.2}, {4.4,2.9,1.4,0.2},
                            {4.9,3.1,1.5,0.1}, {5.4,3.7,1.5,0.2}, {4.8,3.4,1.6,0.2},
                            {4.8,3.0,1.4,0.1}, {4.3,3.0,1.1,0.1}, {5.8,4.0,1.2,0.2},
                            {5.7,4.4,1.5,0.4}, {5.4,3.9,1.3,0.4}, {5.1,3.5,1.4,0.3},
                            {5.7,3.8,1.7,0.3}, {5.1,3.8,1.5,0.3},
                            {7.0,3.2,4.7,1.4}, {6.4,3.2,4.5,1.5}, {6.9,3.1,4.9,1.5},
                            {5.5,2.3,4.0,1.3}, {6.5,2.8,4.6,1.5}, {5.7,2.8,4.5,1.3},
                            {6.3,3.3,4.7,1.6}, {4.9,2.4,3.3,1.0}, {6.6,2.9,4.6,1.3},
                            {5.2,2.7,3.9,1.4}, {5.0,2.0,3.5,1.0}, {5.9,3.0,4.2,1.5},
                            {6.0,2.2,4.0,1.0}, {6.1,2.9,4.7,1.4}, {5.6,2.9,3.6,1.3},
                            {6.7,3.1,4.4,1.4}, {5.6,3.0,4.5,1.5}, {5.8,2.7,4.1,1.0},
                            {6.2,2.2,4.5,1.5}, {5.6,2.5,3.9,1.1},
                            {6.3,3.3,6.0,2.5}, {5.8,2.7,5.1,1.9}, {7.1,3.0,5.9,2.1},
                            {6.3,2.9,5.6,1.8}, {6.5,3.0,5.8,2.2}, {7.6,3.0,6.6,2.1},
                            {4.9,2.5,4.5,1.7}, {7.3,2.9,6.3,1.8}, {6.7,2.5,5.8,1.8},
                            {7.2,3.6,6.1,2.5}, {6.5,3.2,5.1,2.0}, {6.4,2.7,5.3,1.9},
                            {6.8,3.0,5.5,2.1}, {5.7,2.5,5.0,2.0}, {5.8,2.8,5.1,2.4},
                            {6.4,3.2,5.3,2.3}, {6.5,3.0,5.5,1.8}, {7.7,3.8,6.7,2.2},
                            {7.7,2.6,6.9,2.3}, {6.0,2.2,5.0,1.5}	};
    Mat trainingDataMat(60, 4, CV_32FC1, trainingData); 
    //训练样本所对应的类别，S表示setosa，V表示versicolor，R表示virginica
    float responses[60] = {'S','S','S','S','S','S','S','S','S','S','S','S','S','S','S','S','S','S','S','S',
                      'V','V','V','V','V','V','V','V','V','V','V','V','V','V','V','V','V','V','V','V',
                      'R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R'};
    Mat responsesMat(60, 1, CV_32FC1, responses);

    //设置SVM参数
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;    //SVM类型为C-SVC
    params.C = 10.0;    //C参数设置为10
    params.kernel_type = CvSVM::RBF;    //核函数为高斯核函数
    params.gamma = 8.0;    //高斯核函数中的γ参数设置为8
    //迭代的终止条件
    params.term_crit = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);

    //建立SVM模型
    CvSVM svm;
    svm.train(trainingDataMat, responsesMat, Mat(), Mat(), params);
    //预测样本数据
    double sampleData[4]={ 6.7, 3.1, 4.7, 1.5};
    Mat sampleMat(4, 1, CV_32FC1, sampleData);
    float r = svm.predict(sampleMat);    //预测结果
    cout<<endl<<"result:  "<<(char)r<<endl;    //输出结果

    return 0;
}
```

输出结果为：

result:  V






