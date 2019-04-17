# 程序优化：算法对上SIMD+OMP(4) - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 03:24:27[vbskj](https://me.csdn.net/vbskj)阅读数：1213









使用Intrinsics技术，实现SIMD，再加上OMP


这次很简单，只需在循环上加上OpenMP的指示：


#paragm omp parallel for

for(int h = 0; h < 1024; h++)

{

    .......


    #paragm omp parallel for

    for(int w = 0; w < 1024; w++) 


    ......


}


You see，现在就剩下比较结果了。

int _tmain(int argc, _TCHAR* argv[])

{

    INIT_PERF() // 初始化性能计数器



float perf0 = 0.0, perf1 = 0.0, perf2 = 0.0, perf3 = 0.0; // 分别用来统计4种方法的结果

constint loop = 1000; // 进行1000测试



for ( int i = 0; i < loop; i++ ) perf0 += test_Normal_Filter(); // 传统方法



for ( int i = 0; i < loop; i++ ) perf1 += test_Normal_OMP_Filter(); // 传统方法+OMP



for ( int i = 0; i < loop; i++ ) perf2 += test_SIMD_Filter(); // SIMD方法



for ( int i = 0; i < loop; i++ ) perf3 += test_SIMD_OMP_Filter(); // SIMD方法+OMP



    cout<<"Counter Freq:"<<m_CounterFreq<<endl; // 计数频率

    cout<<"Perf0:"<<endl<<"Total:"<<perf0<<" "<<perf0/loop<<"/per proc"<<endl; // 传统方法的结果

    cout<<"Perf1:"<<endl<<"Total:"<<perf1<<" "<<perf1/loop<<"/per proc"<<endl; // 传统方法_OMP

    cout<<"Perf2:"<<endl<<"Total:"<<perf2<<" "<<perf2/loop<<"/per proc"<<endl; //SIMD

    cout<<"Perf3:"<<endl<<"Total:"<<perf3<<" "<<perf3/loop<<"/per proc"<<endl; // SIMD + OMP



return 0;
}



最后的结果，传统方法都要优于SIMD方法!

得出的结论就是：

如果处理比较简单，像例子中这样没有复杂计算，数据相关性小，适合于流水线发挥作用的情况，注意算法的优化

可能带来更好的性能提升。

所以，如果“感觉”使用新方法能够提高性能，请先做测试，并比较“优化”前后的结果，然后再决定是否进行

“优化”。因为，上面的例子再次证明了，“感觉”有时候（经常是“往往”啦）并不可靠。



