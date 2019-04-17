# 决策树（九）--极端随机森林及OpenCV源码分析 - 工作笔记 - CSDN博客





2016年08月08日 11:44:28[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7353








原文：

http://blog.csdn.net/zhaocj/article/details/51648966

一、原理

ET或Extra-Trees（**Ext**remely **ra**ndomized trees，极端随机树）是由PierreGeurts等人于2006年提出。该算法与随机森林算法十分相似，都是由许多决策树构成。但该算法与随机森林有两点主要的区别：

1、随机森林应用的是Bagging模型，而ET是使用所有的训练样本得到每棵决策树，也就是每棵决策树应用的是相同的全部训练样本；

2、随机森林是在一个随机子集内得到最佳分叉属性，而ET是完全随机的得到分叉值，从而实现对决策树进行分叉的。

对于第2点的不同，我们再做详细的介绍。我们仅以二叉树为例，当特征属性是类别的形式时，随机选择具有某些类别的样本为左分支，而把具有其他类别的样本作为右分支；当特征属性是数值的形式时，随机选择一个处于该特征属性的最大值和最小值之间的任意数，当样本的该特征属性值大于该值时，作为左分支，当小于该值时，作为右分支。这样就实现了在该特征属性下把样本随机分配到两个分支上的目的。然后计算此时的分叉值（如果特征属性是类别的形式，可以应用基尼指数；如果特征属性是数值的形式，可以应用均方误差）。遍历节点内的所有特征属性，按上述方法得到所有特征属性的分叉值，我们选择分叉值最大的那种形式实现对该节点的分叉。从上面的介绍可以看出，这种方法比随机森林的随机性更强。

对于某棵决策树，由于它的最佳分叉属性是随机选择的，因此用它的预测结果往往是不准确的，但多棵决策树组合在一起，就可以达到很好的预测效果。

当ET构建好了以后，我们也可以应用全部的训练样本来得到该ET的预测误差。这是因为尽管构建决策树和预测应用的是同一个训练样本集，但由于最佳分叉属性是随机选择的，所以我们仍然会得到完全不同的预测结果，用该预测结果就可以与样本的真实响应值比较，从而得到预测误差。如果与随机森林相类比的话，在ET中，全部训练样本都是OOB样本，所以计算ET的预测误差，也就是计算这个OOB误差。

在这里，我们仅仅介绍了ET算法与随机森林的不同之处，ET算法的其他内容（如预测、OOB误差的计算）与随机森林是完全相同的，具体内容请看关于随机森林的介绍。



二、源码分析



下面是ET算法的类CvERTrees，它继承于CvRTrees类：

```cpp
class CV_EXPORTS_W CvERTrees : public CvRTrees
{
public:
    CV_WRAP CvERTrees();
    virtual ~CvERTrees();
    virtual bool train( const CvMat* trainData, int tflag,
                        const CvMat* responses, const CvMat* varIdx=0,
                        const CvMat* sampleIdx=0, const CvMat* varType=0,
                        const CvMat* missingDataMask=0,
                        CvRTParams params=CvRTParams());
    CV_WRAP virtual bool train( const cv::Mat& trainData, int tflag,
                       const cv::Mat& responses, const cv::Mat& varIdx=cv::Mat(),
                       const cv::Mat& sampleIdx=cv::Mat(), const cv::Mat& varType=cv::Mat(),
                       const cv::Mat& missingDataMask=cv::Mat(),
                       CvRTParams params=CvRTParams());
    virtual bool train( CvMLData* data, CvRTParams params=CvRTParams() );
protected:
    virtual std::string getName() const;
    virtual bool grow_forest( const CvTermCriteria term_crit );
};
```





我们从CvERTrees类可以看出，它没有预测函数predict，因此，如果要进行ET的预测，调用的是它的父类CvRTrees内的predict函数。在训练样本时，CvERTrees类与CvRTrees类的训练过程是完全一致的，即在train函数内再调用grow_forest函数，而且两个类的train函数的输入参数的形式也是完全一样的。但在grow_forest函数内会有一点不同，那就是CvERTrees类中的grow_forest函数把全体训练样本都当成OOB样本，因此它不需要随机样本掩码矩阵变量sample_idx_mask_for_tree，而表示样本索引值变量的sample_idx_for_tree保存的就是正常顺序的训练样本的索引值。

ET算法与随机森林算法最大的不同就在于节点的分叉上，而这一点是体现在CvForestERTree类上的：










```cpp
class CV_EXPORTS CvForestERTree : public CvForestTree
{
protected:
    virtual double calc_node_dir( CvDTreeNode* node );
    virtual CvDTreeSplit* find_split_ord_class( CvDTreeNode* n, int vi,
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_cat_class( CvDTreeNode* n, int vi,
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_ord_reg( CvDTreeNode* n, int vi,
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_cat_reg( CvDTreeNode* n, int vi,
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual void split_node_data( CvDTreeNode* n );
};
```

CvForestERTree类定义了一些专用于ET算法的计算分叉、得到最佳分叉属性的函数，下面我们就逐一介绍这些函数。



按最佳分叉属性标注该节点的所有样本是被分配到左分支还是右分支：








```cpp
double CvForestERTree::calc_node_dir( CvDTreeNode* node )
{
    //表示特征属性的种类是属于左分支还是右分支，-1为左分支，1为右分支，如果该特征属性缺失，则为0
    char* dir = (char*)data->direction->data.ptr;
    //n表示该节点的样本数量，vi表示分类的最佳特征属性
    int i, n = node->sample_count, vi = node->split->var_idx;
    double L, R;

    assert( !node->split->inversed );    //确保分叉不反转

    if( data->get_var_type(vi) >= 0 ) // split on categorical var
    //表示该特征属性是种类的形式
    {
        //开辟一块内存空间
        cv::AutoBuffer<uchar> inn_buf(n*sizeof(int)*(!data->have_priors ? 1 : 2));
        int* labels_buf = (int*)(uchar*)inn_buf;
        //labels指向该特征属性中各个样本所对应的种类，get_cat_var_data函数在ER算法中被重新定义
        const int* labels = data->get_cat_var_data( node, vi, labels_buf );
        // subset数组的每一位表示特征属性的种类，左分支的种类位是1，右分支的是0
        const int* subset = node->split->subset;
        if( !data->have_priors )    //无先验概率
        {
            int sum = 0, sum_abs = 0;

            for( i = 0; i < n; i++ )    //遍历该节点的所有样本
            {
                int idx = labels[i];    //表示该样本的特征属性的种类
                //d为-1表示idx（特征属性的种类）属于左分支，为1表示属于右分支，如果没有该特征属性，则d为0
                int d = ( ((idx >= 0)&&(!data->is_buf_16u)) || ((idx != 65535)&&(data->is_buf_16u)) ) ?
                    CV_DTREE_CAT_DIR(idx,subset) : 0;
                //sum表示d累加求和，因为d也可能为负值，所以sum的含义为右分支比左分支多出的特征属性种类；sum_abs表示d的绝对值之和，表示的含义为被分叉的特征属性种类
                sum += d; sum_abs += d & 1;
                dir[i] = (char)d;    //赋值
            }
            //L和R分别表示左右分支的特征属性的种类数量
            R = (sum_abs + sum) >> 1;
            L = (sum_abs - sum) >> 1;
        }
        else    //有先验概率
        {
            const double* priors = data->priors_mult->data.db;    //得到先验概率
            double sum = 0, sum_abs = 0;
            int *responses_buf = labels_buf + n;
            //responses指向该节点样本的分类，即响应值
            const int* responses = data->get_class_labels(node, responses_buf);

            for( i = 0; i < n; i++ )    //遍历该节点的所有样本
            {
                int idx = labels[i];    //表示该样本的特征属性的种类
                double w = priors[responses[i]];    //得到该响应值的先验概率
                //d为-1表示idx（特征属性的种类）属于左分支，为1表示属于右分支，如果没有该特征属性，则d为0
                int d = idx >= 0 ? CV_DTREE_CAT_DIR(idx,subset) : 0;
                sum += d*w; sum_abs += (d & 1)*w;    //增加了先验概率
                dir[i] = (char)d;
            }
            //L和R分别表示左右分支的特征属性的种类数量
            R = (sum_abs + sum) * 0.5;
            L = (sum_abs - sum) * 0.5;
        }
    }
    else // split on ordered var
    //表示该特征属性是数值的形式
    {
        //得到分叉属性的值split_val，如果样本的分叉属性的值小于该值，则被分叉为左节点，否则为右节点
        float split_val = node->split->ord.c;
        //为该特征属性开辟一块内存空间
        cv::AutoBuffer<uchar> inn_buf(n*(sizeof(int)*(!data->have_priors ? 1 : 2) + sizeof(float)));
        float* val_buf = (float*)(uchar*)inn_buf;    //用于存储各个样本当前特征属性的值
        int* missing_buf = (int*)(val_buf + n);    //表示各个样本是否缺失当前特征属性
        const float* val = 0;    //指向数组val_buf
        const int* missing = 0;    //指向数组missing_buf
        // get_ord_var_data函数在ER算法中被重新定义，各个样本的vi特征属性的值存储在val_buf数组内，各个样本是否缺失该特征属性用missing_buf数组表示
        data->get_ord_var_data( node, vi, val_buf, missing_buf, &val, &missing, 0 );

        if( !data->have_priors )    //无先验概率
        {
            L = R = 0;
            for( i = 0; i < n; i++ )    //遍历所有样本
            {
                if ( missing[i] )    //该样本缺失vi这个特征属性
                    dir[i] = (char)0;    //方向信息赋值为0
                else
                {
                    if ( val[i] < split_val)    //左分支
                    {
                        dir[i] = (char)-1;    //方向信息赋值为-1
                        L++;    //左分支计数
                    }
                    else    //右分支
                    {
                        dir[i] = (char)1;    //方向信息赋值为1
                        R++;    //右分支计数
                    }
                }
            }
        }
        else    //有先验概率
        {
            const double* priors = data->priors_mult->data.db;    //得到先验概率
            int* responses_buf = missing_buf + n;
            //responses指向该节点样本的分类，即响应值
            const int* responses = data->get_class_labels(node, responses_buf);
            L = R = 0;
            for( i = 0; i < n; i++ )    //遍历所有样本
            {
                if ( missing[i] )    //该样本缺失vi这个特征属性
                    dir[i] = (char)0;
                else
                {
                    double w = priors[responses[i]];    //得到先验概率
                    if ( val[i] < split_val)    //左分支
                    {
                        dir[i] = (char)-1;
                         L += w;
                    }
                    else    //右分支
                    {
                        dir[i] = (char)1;
                        R += w;
                    }
                }
            }
        }
    }

    node->maxlr = MAX( L, R );    //表示左右分支最大值
    return node->split->quality/(L + R);    //得到规范化系数
}
```

特征为数值的分类树的分叉方法：










```cpp
CvDTreeSplit* CvForestERTree::find_split_ord_class( CvDTreeNode* node, int vi, float init_quality, CvDTreeSplit* _split,
                                                    uchar* _ext_buf )
{
    const float epsilon = FLT_EPSILON*2;    //定义一个最小常数
    const float split_delta = (1 + FLT_EPSILON) * FLT_EPSILON;    //定义另一个常数

    int n = node->sample_count;    //该节点的样本数量
    int m = data->get_num_classes();    //样本数据的分类数
    //为该特征属性vi开辟一块内存空间
    cv::AutoBuffer<uchar> inn_buf;
    if( !_ext_buf )
        inn_buf.allocate(n*(2*sizeof(int) + sizeof(float)));
    uchar* ext_buf = _ext_buf ? _ext_buf : (uchar*)inn_buf;
    float* values_buf = (float*)ext_buf;    //用于存储各个样本在特征属性vi的值
    int* missing_buf = (int*)(values_buf + n);    //表示各个样本是否缺失当前特征属性
    const float* values = 0;    //指向数组values_buf
    const int* missing = 0;    //指向数组missing_buf
    //得到数组values_buf和missing_buf
    data->get_ord_var_data( node, vi, values_buf, missing_buf, &values, &missing, 0 );
    int* responses_buf = missing_buf + n;
    //responses指向该节点样本的分类，即响应值
    const int* responses = data->get_class_labels( node, responses_buf );

    double lbest_val = 0, rbest_val = 0, best_val = init_quality, split_val = 0;
    //得到不同分类的先验概率
    const double* priors = data->have_priors ? data->priors_mult->data.db : 0;
    bool is_find_split = false;    //表示是否找到了分叉属性
    float pmin, pmax;    //分别表示样本的特征属性vi的最小值和最大值
    int smpi = 0;    //表示缺失特征属性的样本索引值
    //得到第一个不缺失vi特征属性的样本
    while ( missing[smpi] && (smpi < n) )
        smpi++;
    assert(smpi < n);    //确保smpi的正确
    //初始化pmin和pmax
    pmin = values[smpi];
    pmax = pmin;
    for (; smpi < n; smpi++)    //遍历样本，得到pmin和pmax
    {
        float ptemp = values[smpi];    //得到当前样本的vi的值
        int ms = missing[smpi];    //当前样本是否缺失该vi值
        if (ms) continue;    //缺失则遍历下一个样本
        if ( ptemp < pmin)    //更新pmin值
            pmin = ptemp;
        if ( ptemp > pmax)    //更新pmax值
            pmax = ptemp;
    }
    float fdiff = pmax-pmin;    //pmax与pmin的差值
    //差值大于一个常数，表示前面计算的结果是有意义的，也就是得到了分叉属性
    if (fdiff > epsilon) 
    {
        is_find_split = true;    //表示找到了分叉属性
        cv::RNG* rng = data->rng;    //表示随机数
        //随机数为0和1之间的任意数，split_val为pmax与pmin之间任意一个数
        split_val = pmin + rng->uniform(0.f, 1.f) * fdiff ;
        //如果split_val太接近pmax或pmin，则split_val为一个定值
        if (split_val - pmin <= FLT_EPSILON)
            split_val = pmin + split_delta;
        if (pmax - split_val <= FLT_EPSILON)
            split_val = pmax - split_delta;

        // calculate Gini index
        //计算基尼指数
        if ( !priors )    //样本没有先验概率
        {
            cv::AutoBuffer<int> lrc(m*2);
            //lc和rc分别表示分类的左、右分支
            int *lc = lrc, *rc = lc + m;
            int L = 0, R = 0;    //表示左、右分支的样本数

            // init arrays of class instance counters on both sides of the split
            for(int i = 0; i < m; i++ )    //数组lc和rc清零
            {
                lc[i] = 0;
                rc[i] = 0;
            }
            for( int si = 0; si < n; si++ )    //遍历所有样本
            {
                int r = responses[si];    //该样本的响应值
                float val = values[si];    //该样本的特征属性vi的值
                int ms = missing[si];    //当前样本是否缺失该vi值
                if (ms) continue;    //缺失则遍历下一个样本
                if ( val < split_val )    //左分支
                {
                    lc[r]++;
                    L++;
                }
                else    //右分支
                {
                    rc[r]++;
                    R++;
                }
            }
            //得到分类后的基尼指数best_val
            for (int i = 0; i < m; i++)
            {
                lbest_val += lc[i]*lc[i];
                rbest_val += rc[i]*rc[i];
            }
            best_val = (lbest_val*R + rbest_val*L) / ((double)(L*R));
        }
        else    //样本有先验概率
        {
            cv::AutoBuffer<double> lrc(m*2);
            double *lc = lrc, *rc = lc + m;
            double L = 0, R = 0;

            // init arrays of class instance counters on both sides of the split
            for(int i = 0; i < m; i++ )
            {
                lc[i] = 0;
                rc[i] = 0;
            }
            for( int si = 0; si < n; si++ )
            {
                int r = responses[si];
                float val = values[si];
                int ms = missing[si];
                double p = priors[r];    //得到先验概率
                if (ms) continue;
                if ( val < split_val )    //左分支
                {
                    lc[r] += p;
                    L += p;
                }
                else    //右分支
                {
                    rc[r] += p;
                    R += p;
                }
            }
            for (int i = 0; i < m; i++)
            {
                lbest_val += lc[i]*lc[i];
                rbest_val += rc[i]*rc[i];
            }
            best_val = (lbest_val*R + rbest_val*L) / (L*R);
        }

    }

    CvDTreeSplit* split = 0;
    if( is_find_split )    //表明得到了分叉属性
    {
        split = _split ? _split : data->new_split_ord( 0, 0.0f, 0, 0, 0.0f );    //实例化split变量
        split->var_idx = vi;    //特征属性
        split->ord.c = (float)split_val;    //特征属性的值
        split->ord.split_point = -1;
        split->inversed = 0;
        split->quality = (float)best_val;    //基尼指数
    }
    return split;    //返回
}
```

特征为类的分类树的分叉方法：










```cpp
CvDTreeSplit* CvForestERTree::find_split_cat_class( CvDTreeNode* node, int vi, float init_quality, CvDTreeSplit* _split,
                                                    uchar* _ext_buf )
{
    int ci = data->get_var_type(vi);    //得到该节点的特征属性的形式
    int n = node->sample_count;    //得到该节点的样本数量
    int cm = data->get_num_classes();    //得到样本数据的分类数
    int vm = data->cat_count->data.i[ci];    //得到该特征属性vi的类别数量
    double best_val = init_quality;
    CvDTreeSplit *split = 0;

    if ( vm > 1 )    //如果该特征属性vi的类别大于1个
    {
        //开辟内存空间
        cv::AutoBuffer<int> inn_buf;
        if( !_ext_buf )
            inn_buf.allocate(2*n);
        int* ext_buf = _ext_buf ? (int*)_ext_buf : (int*)inn_buf;
        //labels指向该特征属性中各个样本所对应的种类，get_cat_var_data函数在ER算法中被重新定义
        const int* labels = data->get_cat_var_data( node, vi, ext_buf );
        //responses指向该节点样本的分类，即响应值
        const int* responses = data->get_class_labels( node, ext_buf + n );
        //得到分类的先验概率
        const double* priors = data->have_priors ? data->priors_mult->data.db : 0;

        // create random class mask
        cv::AutoBuffer<int> valid_cidx(vm);    //表示该特征属性vi的有效类别的索引
        for (int i = 0; i < vm; i++)
        {
            valid_cidx[i] = -1;    //初始化数组valid_cidx
        }
        for (int si = 0; si < n; si++)    //遍历所有样本
        {
            int c = labels[si];    //当前样本在vi中的分类类别
            //分类不符合条件，则遍历下一个样本
            if ( ((c == 65535) && data->is_buf_16u) || ((c<0) && (!data->is_buf_16u)) )
                continue;
            valid_cidx[c]++;    //分类计数
        }

        int valid_ccount = 0;    //表示有效分类类别的数量
        for (int i = 0; i < vm; i++)    //遍历所有分类
            if (valid_cidx[i] >= 0)    //如果当前分类有样本
            {
                // valid_cidx表示有效分类的索引
                valid_cidx[i] = valid_ccount;
                valid_ccount++;
            }
        if (valid_ccount > 1)    //表示该节点可以被分叉
        {
            CvRNG* rng = forest->get_rng();    //表示随机数
            //得到一个小于valid_ccount的任意整数
            int l_cval_count = 1 + cvRandInt(rng) % (valid_ccount-1);
            //表示有效分类的掩码矢量，每个元素对应一个分类，其中前l_cval_count个元素被置1，后面的元素为0。置换后为1的样本将被分配到左分支，为0的样本将被分配到右分支
            CvMat* var_class_mask = cvCreateMat( 1, valid_ccount, CV_8UC1 );
            CvMat submask;    //表示前l_cval_count个元素的掩码子集
            //矩阵var_class_mask清零
            memset(var_class_mask->data.ptr, 0, valid_ccount*CV_ELEM_SIZE(var_class_mask->type));
            //submask指向var_class_mask的列
            cvGetCols( var_class_mask, &submask, 0, l_cval_count );
            cvSet( &submask, cvScalar(1) );    //置1
            //遍历所有有效分类，每次置换任意两个元素，从而实现了打乱分类的目的
            for (int i = 0; i < valid_ccount; i++) 
            {
                uchar temp;
                int i1 =  cvRandInt( rng ) % valid_ccount;
                int i2 = cvRandInt( rng ) % valid_ccount;
                CV_SWAP( var_class_mask->data.ptr[i1], var_class_mask->data.ptr[i2], temp );
            }

            split = _split ? _split : data->new_split_cat( 0, -1.0f );
            split->var_idx = vi;    //得到该特征属性 
            memset( split->subset, 0, (data->max_c_count + 31)/32 * sizeof(int));    //清零

            // calculate Gini index
            //计算基尼指数
            double lbest_val = 0, rbest_val = 0;
            if( !priors )    //没有先验概率
            {
                cv::AutoBuffer<int> lrc(cm*2);
                int *lc = lrc, *rc = lc + cm;    //分别表示左、右分支
                int L = 0, R = 0;    //分别表示左、右分支样本数
                // init arrays of class instance counters on both sides of the split
                for(int i = 0; i < cm; i++ )    //数组lc和rc清零
                {
                    lc[i] = 0;
                    rc[i] = 0;
                }
                for( int si = 0; si < n; si++ )    //遍历所有样本
                {
                    int r = responses[si];    //该样本的响应值
                    int var_class_idx = labels[si];    //该样本的特征属性vi的类别索引
                    //如果var_class_idx不符合条件，则遍历下一个样本
                    if ( ((var_class_idx == 65535) && data->is_buf_16u) || ((var_class_idx<0) && (!data->is_buf_16u)) )
                        continue;
                    //表示类别对应于有效分类类别的索引
                    int mask_class_idx = valid_cidx[var_class_idx];
                    if (var_class_mask->data.ptr[mask_class_idx])
                    // submask子集内的元素，是为1
                    {
                        lc[r]++;    //左分支
                        L++;
                        //左分支相应的位被置1
                        split->subset[var_class_idx >> 5] |= 1 << (var_class_idx & 31);
                    }
                    else    // submask子集以外的元素，是为0
                    {
                        rc[r]++;    //右分支
                        R++;
                    }
                }
                //计算基尼指数
                for (int i = 0; i < cm; i++)
                {
                    lbest_val += lc[i]*lc[i];
                    rbest_val += rc[i]*rc[i];
                }
                best_val = (lbest_val*R + rbest_val*L) / ((double)(L*R));    //基尼指数
            }
            else    //有先验概率
            {
                cv::AutoBuffer<int> lrc(cm*2);
                int *lc = lrc, *rc = lc + cm;
                double L = 0, R = 0;
                // init arrays of class instance counters on both sides of the split
                for(int i = 0; i < cm; i++ )
                {
                    lc[i] = 0;
                    rc[i] = 0;
                }
                for( int si = 0; si < n; si++ )
                {
                    int r = responses[si];
                    int var_class_idx = labels[si];
                    if ( ((var_class_idx == 65535) && data->is_buf_16u) || ((var_class_idx<0) && (!data->is_buf_16u)) )
                        continue;
                    double p = priors[si];    //得到先验概率
                    int mask_class_idx = valid_cidx[var_class_idx];

                    if (var_class_mask->data.ptr[mask_class_idx])    //左分支
                    {
                        lc[r]+=(int)p;
                        L+=p;
                        split->subset[var_class_idx >> 5] |= 1 << (var_class_idx & 31);
                    }
                    else    //右分支
                    {
                        rc[r]+=(int)p;
                        R+=p;
                    }
                }
                for (int i = 0; i < cm; i++)
                {
                    lbest_val += lc[i]*lc[i];
                    rbest_val += rc[i]*rc[i];
                }
                best_val = (lbest_val*R + rbest_val*L) / (L*R);
            }
            split->quality = (float)best_val;    //基尼指数

            cvReleaseMat(&var_class_mask);    //释放变量
        }
    }

    return split;    //返回值
}
```

特征为数值的回归树的分叉方法：








```cpp
CvDTreeSplit* CvForestERTree::find_split_ord_reg( CvDTreeNode* node, int vi, float init_quality, CvDTreeSplit* _split,
                                                  uchar* _ext_buf )
{
    const float epsilon = FLT_EPSILON*2;    //定义一个很小的常数
    const float split_delta = (1 + FLT_EPSILON) * FLT_EPSILON;    //定义另一个常数
    int n = node->sample_count;    //该节点的样本数量
    cv::AutoBuffer<uchar> inn_buf;    //开辟一块内存空间
    if( !_ext_buf )
        inn_buf.allocate(n*(2*sizeof(int) + 2*sizeof(float)));
    uchar* ext_buf = _ext_buf ? _ext_buf : (uchar*)inn_buf;
    float* values_buf = (float*)ext_buf;    //用于存储各个样本在特征属性vi的值
    int* missing_buf = (int*)(values_buf + n);    //表示各个样本是否缺失当前特征属性
    const float* values = 0;    //指向数组values_buf
    const int* missing = 0;    //指向数组missing_buf
    //得到数组values_buf和missing_buf
    data->get_ord_var_data( node, vi, values_buf, missing_buf, &values, &missing, 0 );
    float* responses_buf =  (float*)(missing_buf + n);
    int* sample_indices_buf =  (int*)(responses_buf + n);
    //responses指向该节点样本的响应值
    const float* responses = data->get_ord_responses( node, responses_buf, sample_indices_buf );

    double best_val = init_quality, split_val = 0, lsum = 0, rsum = 0;
    int L = 0, R = 0;

    bool is_find_split = false;    //表示是否找到了分叉属性
    float pmin, pmax;    //分别表示样本的特征属性vi的最小值和最大值
    int smpi = 0;    //表示缺失特征属性的样本索引值
    while ( missing[smpi] && (smpi < n) )    //得到第一个不缺失vi特征属性的样本
        smpi++;

    assert(smpi < n);    //确保smpi的正确
    //初始化pmin和pmax
    pmin = values[smpi];
    pmax = pmin;
    for (; smpi < n; smpi++)    //遍历样本，得到pmin和pmax
    {
        float ptemp = values[smpi];    //得到当前样本的vi的值
        int m = missing[smpi];    //当前样本是否缺失该vi值
        if (m) continue;    //缺失则遍历下一个样本
        if ( ptemp < pmin)    //更新pmin值
            pmin = ptemp;
        if ( ptemp > pmax)    //更新pmax值
            pmax = ptemp;
    }
    float fdiff = pmax-pmin;    //pmax与pmin的差值
    //差值大于一个常数，表示前面计算的结果是有意义的，也就是得到了分叉属性
    if (fdiff > epsilon)
    {
        is_find_split = true;    //表示找到了分叉属性
        cv::RNG* rng = data->rng;    //表示随机数
        //随机数为0和1之间的任意数，split_val为pmax与pmin之间任意一个数
        split_val = pmin + rng->uniform(0.f, 1.f) * fdiff ;
        //如果split_val太接近pmax或pmin，则split_val为一个定值
        if (split_val - pmin <= FLT_EPSILON)
            split_val = pmin + split_delta;
        if (pmax - split_val <= FLT_EPSILON)
            split_val = pmax - split_delta;

        for (int si = 0; si < n; si++)    //遍历所有样本，计算均方误差
        {
            float r = responses[si];    //当前样本的响应值
            float val = values[si];    //当前样本的特征属性vi的值
            int m = missing[si];    //当前样本是否缺失该vi值
            if (m) continue;    //缺失则遍历下一个样本
            if (val < split_val)    //左分支
            {
                lsum += r;
                L++;
            }
            else    //右分支
            {
                rsum += r;
                R++;
            }
        }
        best_val = (lsum*lsum*R + rsum*rsum*L)/((double)L*R);    //得到均方误差
    }

    CvDTreeSplit* split = 0;
    if( is_find_split )    //表明得到了分叉属性
    {
        split = _split ? _split : data->new_split_ord( 0, 0.0f, 0, 0, 0.0f );    //实例化split变量
        split->var_idx = vi;    //特征属性
        split->ord.c = (float)split_val;    //特征属性的值
        split->ord.split_point = -1;
        split->inversed = 0;
        split->quality = (float)best_val;    //适合回归树的均方误差值
    }
    return split;   //返回
}
```

特征为类的回归树的分叉方法：






```cpp
CvDTreeSplit* CvForestERTree::find_split_cat_reg( CvDTreeNode* node, int vi, float init_quality, CvDTreeSplit* _split,
                                                  uchar* _ext_buf )
{
    int ci = data->get_var_type(vi);    //得到该节点的特征属性的形式
    int n = node->sample_count;    //得到该节点的样本数量
    int vm = data->cat_count->data.i[ci];    //得到该特征属性vi的类别数量
    double best_val = init_quality;
    CvDTreeSplit *split = 0;
    float lsum = 0, rsum = 0;

    if ( vm > 1 )    //如果该特征属性vi的类别大于1个
    {
        int base_size =  vm*sizeof(int);    //开辟内存空间
        cv::AutoBuffer<uchar> inn_buf(base_size);
        if( !_ext_buf )
            inn_buf.allocate(base_size + n*(2*sizeof(int) + sizeof(float)));
        uchar* base_buf = (uchar*)inn_buf;
        uchar* ext_buf = _ext_buf ? _ext_buf : base_buf + base_size;
        int* labels_buf = (int*)ext_buf;
        //labels指向该特征属性中各个样本所对应的种类，get_cat_var_data函数在ER算法中被重新定义
        const int* labels = data->get_cat_var_data( node, vi, labels_buf );
        float* responses_buf =  (float*)(labels_buf + n);
        int* sample_indices_buf = (int*)(responses_buf + n);
        //responses指向该节点样本的分类，即响应值
        const float* responses = data->get_ord_responses( node, responses_buf, sample_indices_buf );

        // create random class mask
        int *valid_cidx = (int*)base_buf;    //表示该特征属性vi的有效类别的索引
        for (int i = 0; i < vm; i++)
        {
            valid_cidx[i] = -1;    //初始化数组valid_cidx
        }
        for (int si = 0; si < n; si++)    //遍历所有样本
        {
            int c = labels[si];    //当前样本在vi中的分类类别
            //分类不符合条件，则遍历下一个样本
            if ( ((c == 65535) && data->is_buf_16u) || ((c<0) && (!data->is_buf_16u)) )
                        continue;
            valid_cidx[c]++;    //分类计数
        }

        int valid_ccount = 0;    //表示有效分类类别的数量
        for (int i = 0; i < vm; i++)    //遍历所有分类
            if (valid_cidx[i] >= 0)    //如果当前分类有样本
            {
                // valid_cidx表示有效分类的索引
                valid_cidx[i] = valid_ccount;
                valid_ccount++;
            }
        if (valid_ccount > 1)    //表示该节点可以被分叉
        {
            CvRNG* rng = forest->get_rng();    //表示随机数
            //得到一个小于valid_ccount的任意整数
            int l_cval_count = 1 + cvRandInt(rng) % (valid_ccount-1);
            //表示有效分类的掩码矢量，每个元素对应一个分类，其中前l_cval_count个元素被置1，后面的元素为0。置换后为1的样本将被分配到左分支，为0的样本将被分配到右分支
            CvMat* var_class_mask = cvCreateMat( 1, valid_ccount, CV_8UC1 );
            CvMat submask;    //表示前l_cval_count个元素的掩码子集
            //矩阵var_class_mask清零
            memset(var_class_mask->data.ptr, 0, valid_ccount*CV_ELEM_SIZE(var_class_mask->type));
            //submask指向var_class_mask的列
            cvGetCols( var_class_mask, &submask, 0, l_cval_count );
            cvSet( &submask, cvScalar(1) );    //置1
            //遍历所有有效分类，每次置换任意两个元素，从而实现了打乱分类的目的
            for (int i = 0; i < valid_ccount; i++)
            {
                uchar temp;
                int i1 = cvRandInt( rng ) % valid_ccount;
                int i2 = cvRandInt( rng ) % valid_ccount;
                CV_SWAP( var_class_mask->data.ptr[i1], var_class_mask->data.ptr[i2], temp );
            }

            split = _split ? _split : data->new_split_cat( 0, -1.0f);
            split->var_idx = vi;    //得到该特征属性
            memset( split->subset, 0, (data->max_c_count + 31)/32 * sizeof(int));    //清零

            int L = 0, R = 0;
            for( int si = 0; si < n; si++ )    //遍历所有样本
            {
                float r = responses[si];    //当前样本的响应值
                int var_class_idx = labels[si];    //当前样本在特征属性vi中的类别
                //如果var_class_idx不符合条件，则遍历下一个样本
                if ( ((var_class_idx == 65535) && data->is_buf_16u) || ((var_class_idx<0) && (!data->is_buf_16u)) )
                        continue;
                //表示类别对应于有效分类类别的索引
                int mask_class_idx = valid_cidx[var_class_idx];
                if (var_class_mask->data.ptr[mask_class_idx])
                // submask子集内的元素，是为1
                {
                    lsum += r;
                    L++;
                    //左分支相应的位被置1
                    split->subset[var_class_idx >> 5] |= 1 << (var_class_idx & 31);
                }
                else    // submask子集以外的元素，是为0
                {
                    rsum += r;
                    R++;
                }
            }
            best_val = (lsum*lsum*R + rsum*rsum*L)/((double)L*R);    //计算均方误差

            split->quality = (float)best_val;    //适合回归树的均方误差值

            cvReleaseMat(&var_class_mask);    //释放内存
        }
    }

    return split;    //返回
}
```

把节点分叉，并完成相关的运算：










```cpp
void CvForestERTree::split_node_data( CvDTreeNode* node )
{
    //n为该节点的样本数，scount为训练的所有样本数，nl和nr分别表示左分支和右分支的样本数
    int vi, i, n = node->sample_count, nl, nr, scount = data->sample_count;
    //样本的方向信息，即属于哪个分支，-1为左分支，1为右分支，0为缺失
    char* dir = (char*)data->direction->data.ptr;
    CvDTreeNode *left = 0, *right = 0;
    int new_buf_idx = data->get_child_buf_idx( node );
    CvMat* buf = data->buf;
    size_t length_buf_row = data->get_length_subbuf();
    cv::AutoBuffer<int> temp_buf(n);
    //用替代分叉属性完成对该节点的分叉，即把该节点分叉为左分支或右分支
    complete_node_dir(node);
    //遍历该节点的所有样本
    for( i = nl = nr = 0; i < n; i++ )
    {
        //得到该样本的方向信息，这里的方向信息是左分支为0，右分支为1
        int d = dir[i];
        nr += d;    //累加d，含义是计算右分支的样本数
        nl += d^1;    //d与1异或，并累加，含义是计算左分支的样本数
    }

    bool split_input_data;    //分叉以后的标识
    //定义左右分支的节点变量
    node->left = left = data->new_node( node, nl, new_buf_idx, node->offset );
    node->right = right = data->new_node( node, nr, new_buf_idx, node->offset + nl );
    //判断目前决策树的深度和左右分支的样本数，如果满足要求，则该标识为1，否则为0
    split_input_data = node->depth + 1 < data->params.max_depth &&
        (node->left->sample_count > data->params.min_sample_count ||
        node->right->sample_count > data->params.min_sample_count);

    cv::AutoBuffer<uchar> inn_buf(n*(sizeof(int)+sizeof(float)));
    // split ordered vars
    //遍历样本的所有形式是数值的特征属性，对特征属性是数值形式的进行分叉
    for( vi = 0; vi < data->var_count; vi++ )
    {
        int ci = data->get_var_type(vi);
        //如果是类形式的特征属性，则继续下一个特征属性的循环
        if (ci >= 0) continue;
        //n1表示该节点在特征属性vi下的样本数量
        int n1 = node->get_num_valid(vi), nr1 = 0;
        float* values_buf = (float*)(uchar*)inn_buf;    //用于存储各个样本当前特征属性的值
        int* missing_buf = (int*)(values_buf + n);    //表示各个样本是否缺失当前特征属性
        const float* values = 0;    //指向数组values_buf
        const int* missing = 0;    //指向数组missing_buf
        // get_ord_var_data函数在ER算法中被重新定义，各个样本的vi特征属性的值存储在values_buf数组内，各个样本是否缺失该特征属性用missing_buf数组表示
        data->get_ord_var_data( node, vi, values_buf, missing_buf, &values, &missing, 0 );
        //遍历该节点的所有样本，得到右分支中特征属性是数值形式的样本数量
        for( i = 0; i < n; i++ )
            nr1 += ((!missing[i]) & dir[i]);
        //设置该特征属性vi下的左右分支的样本数
        left->set_num_valid(vi, n1 - nr1);
        right->set_num_valid(vi, nr1);
    }
    // split categorical vars, responses and cv_labels using new_idx relocation table
    //遍历样本的所有形式是类的特征属性，对特征属性是类形式的进行分叉
    for( vi = 0; vi < data->get_work_var_count() + data->ord_var_count; vi++ )
    {
        int ci = data->get_var_type(vi);
        //如果是数值形式的特征属性，则继续下一个特征属性的循环
        if (ci < 0) continue;

        int n1 = node->get_num_valid(vi), nr1 = 0;
        // src_lbls指向该特征属性中各个样本所对应的种类，get_cat_var_data函数在ER算法中被重新定义
        const int* src_lbls = data->get_cat_var_data(node, vi, (int*)(uchar*)inn_buf);

        for(i = 0; i < n; i++)
            temp_buf[i] = src_lbls[i];    //赋值

        if (data->is_buf_16u)
        {
            unsigned short *ldst = (unsigned short *)(buf->data.s + left->buf_idx*length_buf_row +
                ci*scount + left->offset);
            unsigned short *rdst = (unsigned short *)(buf->data.s + right->buf_idx*length_buf_row +
                ci*scount + right->offset);

            for( i = 0; i < n; i++ )    //遍历所有样本
            {
                int d = dir[i];    //得到方向信息
                int idx = temp_buf[i];    //得到该样本的特征属性的种类
                if (d)    //右分支
                {
                    *rdst = (unsigned short)idx;
                    rdst++;    //指向下一个地址
                    nr1 += (idx != 65535);    //右分支计数
                }
                else    //左分支
                {
                    *ldst = (unsigned short)idx;
                    ldst++;    //指向下一个地址
                }
            }

            if( vi < data->var_count )    // data->var_count为样本的特征属性数量
            {
                //设置该特征属性vi下的左右分支的样本数
                left->set_num_valid(vi, n1 - nr1);
                right->set_num_valid(vi, nr1);
            }
        }
        else
        //与if(data->is_buf_16u)的情况相似，唯一的不同就是分配的内存空间大小不同
        {
            int *ldst = buf->data.i + left->buf_idx*length_buf_row +
                ci*scount + left->offset;
            int *rdst = buf->data.i + right->buf_idx*length_buf_row +
                ci*scount + right->offset;

            for( i = 0; i < n; i++ )
            {
                int d = dir[i];
                int idx = temp_buf[i];
                if (d)
                {
                    *rdst = idx;
                    rdst++;
                    nr1 += (idx >= 0);
                }
                else
                {
                    *ldst = idx;
                    ldst++;
                }

            }

            if( vi < data->var_count )
            {
                left->set_num_valid(vi, n1 - nr1);
                right->set_num_valid(vi, nr1);
            }
        }
    }

    // split sample indices
    int *sample_idx_src_buf = (int*)(uchar*)inn_buf;
    const int* sample_idx_src = 0;
    if (split_input_data)
    {
        //得到样本序列索引
        sample_idx_src = data->get_sample_indices(node, sample_idx_src_buf);
        //初始化temp_buf数组为sample_idx_src数组
        for(i = 0; i < n; i++)
            temp_buf[i] = sample_idx_src[i];

        int pos = data->get_work_var_count();

        if (data->is_buf_16u)
        {
            unsigned short* ldst = (unsigned short*)(buf->data.s + left->buf_idx*length_buf_row +
                pos*scount + left->offset);
            unsigned short* rdst = (unsigned short*)(buf->data.s + right->buf_idx*length_buf_row +
                pos*scount + right->offset);

            for (i = 0; i < n; i++)   //遍历所有样本
            {
                int d = dir[i];    //得到该样本的方向信息
                unsigned short idx = (unsigned short)temp_buf[i];
                if (d)    //右分支
                {
                    *rdst = idx;    //样本索引
                    rdst++;    //指向下一个地址
                }
                else    //左分支
                {
                    *ldst = idx;
                    ldst++;
                }
            }
        }
        else
        //与if(data->is_buf_16u)的情况相似，唯一的不同就是分配的内存空间大小不同
        {
            int* ldst = buf->data.i + left->buf_idx*length_buf_row +
                pos*scount + left->offset;
            int* rdst = buf->data.i + right->buf_idx*length_buf_row +
                pos*scount + right->offset;
            for (i = 0; i < n; i++)
            {
                int d = dir[i];
                int idx = temp_buf[i];
                if (d)
                {
                    *rdst = idx;
                    rdst++;
                }
                else
                {
                    *ldst = idx;
                    ldst++;
                }
            }
        }
    }

    // deallocate the parent node data that is not needed anymore
    data->free_node_data(node);    //释放该节点
}
```

在这里，我们仅仅给出了专属于ET算法的一些函数，要想更好的理解ET程序，还需要结合决策树和随机森林的相关程序，具体的内容请看那两篇文章的介绍。



三、应用实例



我们仍然应用随机森林那篇文章中给出的房屋售价的实例，对房屋面积为201.5，房间数量为3的房屋进行预测售价：











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
	double trainingData[28][2]={{210.4, 3}, {240.0, 3}, {300.0, 4}, {153.4, 3}, {138.0, 3},
								{194.0,4}, {189.0, 3}, {126.8, 3}, {132.0, 2}, {260.9, 4},
								{176.7,3}, {160.4, 3}, {389.0, 3}, {145.8, 3}, {160.0, 3},
								{141.6,2}, {198.5, 4}, {142.7, 3}, {149.4, 3}, {200.0, 3},
								{447.8,5}, {230.0, 4}, {123.6, 3}, {303.1, 4}, {188.8, 2},
								{196.2,4}, {110.0, 3}, {252.6, 3} };
	CvMat trainingDataCvMat = cvMat( 28, 2, CV_32FC1, trainingData );

	float responses[28] = {	399900, 369000, 539900, 314900, 212000, 239999, 329999,
                        259900, 299900, 499998, 252900, 242900, 573900, 464500,
                        329900, 232000, 299900, 198999, 242500, 347000, 699900, 
                        449900, 199900, 599000, 255000, 259900, 249900, 469000};
	CvMat responsesCvMat = cvMat( 28, 1, CV_32FC1, responses );
    
	CvRTParams params= CvRTParams(10, 2, 0, false,16, 0, true, 0, 100, 0, CV_TERMCRIT_ITER );
    
    CvERTrees etrees;
	etrees.train(&trainingDataCvMat, CV_ROW_SAMPLE, &responsesCvMat, 
                                  NULL, NULL, NULL, NULL,params);

	double sampleData[2]={201.5, 3};
	Mat sampleMat(2, 1, CV_32FC1, sampleData);
	float r = etrees.predict(sampleMat);
	cout<<endl<<"result:  "<<r<<endl;

	return 0;
}
```
上面的结果与随机森林的结果是一样的，这是因为我们的样本很少，特征属性也不多，无法体现随机性的特点。总之，在这个系列的文章中，我们的应用实例仅仅是一个例子，不具有任何实际应用。




