# 实践：使用FLANN.LSH进行检索 - wishchinYang的专栏 - CSDN博客
2014年04月01日 20:05:54[wishchin](https://me.csdn.net/wishchin)阅读数：3381
**1.Survey：**
FLANN 库详情见：[http://en.wikipedia.org/wiki/Flann](http://en.wikipedia.org/wiki/Flann)
[http://medievalscotland.org/kmo/AnnalsIndex/Feminine/Flann.shtml](http://medievalscotland.org/kmo/AnnalsIndex/Feminine/Flann.shtml)
FLANN主页：[http://www.cs.ubc.ca/research/flann/](http://www.cs.ubc.ca/research/flann/)：FLANN is written in C++ and contains bindings for the following languages: C, MATLAB and Python.
OpenCV的FLANN库相对于原始FLANN库功能较少；比如不能直接使用flann::Matrix<unsigned char>   data ();
~~OpenCV和PCL都使用了FLANN 库，自从用Python实现CP之后，发现重写LSH的工作量还是相当大，于是使用PCL的FLANN库，省去转化的麻烦。~~
使用CP的检索方式，看来只能用matlab实现了，因为没有办法实现128位的hash表。
**2.使用过程中遇到的麻烦（Vs.KD-Tree）：**
VS2010不能完全支持CX0标准，不支持vector的下标越界检验，很受伤.....
2.1.使用函数载入特征数据集，存入vector：
//Load the data source
loadVotexFModels(pathName, extension, models);
原始特征数据可以直接存入矩阵，貌似只能使用UChar型：
```cpp
// Convert data into FLANN format
    flann::Matrix<unsigned char> data (
        new unsigned char[models.size () * models[0].second.size ()],
        models.size (),
        models[0].second.size ());
    for (size_t i = 0; i < data.rows; ++i)
        for (size_t j = 0; j < data.cols; ++j)
            data[i][j] = models[i].second[j];
    flann::save_to_file (data, training_data_h5_file_name, "training_data");// Save data to disk (list of models)
    delete[] data.ptr ();
```
2.1.使用存储时，使用了C语言的类型FILE* (据说比使用C++的stream快256倍)
```cpp
unsigned int table_number =6;
    unsigned int key_size     =8;    //unsigned int key_size     =32;//32 is so big a value;在库的内部没有排错语句，很失败！
    unsigned int multi_probe_level=2; 
    //Create hash index
    flann::LshIndex<flann::ChiSquareDistance<unsigned char> > index (data, flann::LshIndexParams (table_number, key_size,multi_probe_level));
    index.buildIndex ();
    FILE* StreamIdx =fopen(kdtree_idx_file_name.c_str(),"wb");//Use the  FILE* Type.
    index.saveIndex(StreamIdx);
    fclose(StreamIdx);
```
2.3 修改文件：
.flann/util/result_set.h   line263:
size_t j ==0 时，会造成 --j 成为一个很大的数，造成下表越界，故添加语句：if (j>=dist_index_.size()) break; //wishchin 跳出循环。
2.4.对位操作符的修改：
使用unsigned int key_size     =32;时
向右以为size_t(1)<<  key_size , 产生的值为1造成向量下标超出；或许可以改成power()函数....
long long(1)<<  key_size ；约为4GBits.
*|=* 或等于的使用，把函数符号拆开 x = x| y；
**3.使用LSH检索特征：**
FILE* StreamIdx =fopen(kdtree_idx_file_name.c_str(),"rb");
index.loadIndex(StreamIdx);//唯一调用函数...
测试函数：
```cpp
testCreateLshindex(argc,argv);
void testCreateLshindex(int argc, _TCHAR* argv[])
{
    CLSH  FeatureIndex;
    std::string pathName(argv[2]);
    std::string H5_file_Name(argv[3]);
    std::string idx_file_Name(argv[4]);
    std::string data_list_file_name(argv[5]);
    FeatureIndex.genLshVotexFFromFile(pathName,
        H5_file_Name,idx_file_Name,data_list_file_name);
    return;
}
testLshSearch(argc,argv);// 测试检索结果！准确率挺高的...
void testLshSearch(int argc, _TCHAR* argv[])
{
    CLSH  FeatureIndex;
    std::string pathName(argv[2]);
    std::string H5_file_Name(argv[3]);
    std::string idx_file_Name(argv[4]);
    const std::string data_list_file_name(argv[5]);
    std::string test_file_name(argv[1]);
    Votex_model Feature;
    FeatureIndex.loadVotexFHist(test_file_name,Feature);
    unsigned int table_number     =6;
    unsigned int key_size         =8;
    unsigned int multi_probe_level=2;
    int k =6;
    flann::Matrix<unsigned char>   data;
    std::vector<std::string>   Filelist;
    
    FeatureIndex.loadLshSQL(H5_file_Name,
        idx_file_Name,
        data_list_file_name,
        data,
        Filelist);
    flann::LshIndex<flann::ChiSquareDistance<unsigned char> >  index(data, flann::LshIndexParams (table_number, key_size,multi_probe_level));
    FeatureIndex.loadLshIndex(idx_file_Name,data,index,table_number ,key_size ,multi_probe_level);
    flann::Matrix<int>        k_indices;
    flann::Matrix<float>    k_distances;
    k_indices = flann::Matrix<int>(new int[k], 1, k);
    k_distances = flann::Matrix<float>(new float[k], 1, k);
    FeatureIndex.searchLshSQL(Feature,index,k,k_indices,k_distances);
    for (int idx =0;idx< k;++idx){
        cout<< Filelist[(k_indices[0][idx])]<<endl;
    }
```
    return;
}
