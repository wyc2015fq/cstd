# OpenCV:OpenCV中的 parallel_for 和opencv parallel_for_ - wishchinYang的专栏 - CSDN博客
2017年03月21日 10:20:03[wishchin](https://me.csdn.net/wishchin)阅读数：2440
        OpenCV使用OMP完成并行运算，在使用AdaBoost检测的时候，在cascadedetect.cpp 里面，大量使用
```cpp
parallel_for_(Range(0, stripCount), CascadeClassifierInvoker( *this, processingRectSize, stripSize, yStep, factor,candidatesVector, rejectLevels, levelWeights, false, currentMask, &mtx) );
```
语句。
        策略对应的机制：
        CascadeClassifierInvoker继承自于 ParallelLoopBody ，实现parallel_for_( )语句。
                 class CascadeClassifierInvoker : public ParallelLoopBody  
CV2.4.3中自带的calcOpticalFlowPyrLK函数也用parallel_for重写过了，之前我一直认为parallel_for就是用来并行计算的，之前也自己写了一些用parallel_for实现的算法。
      直到今天在opencv官网中看到别人的提问，才发现parallel_for实际上是serial loop （普通循环结构），而parallel_for_才是parallel loop（[OpenCV官网answer](http://answers.opencv.org/question/3730/how-to-use-parallel_for/)），用以实现TBB或者OpenCL并行。
       参考文章：[OpenCV中parallel_for 和 parallel_for_学习笔记](http://blog.csdn.net/chouclee/article/details/8682561)
        函数定义在parallel.cpp里面，定义为：
```cpp
/* ================================   parallel_for_  ================================ */
void cv::parallel_for_(const cv::Range& range, const cv::ParallelLoopBody& body, double nstripes)
{
#ifdef CV_PARALLEL_FRAMEWORK
  if(numThreads != 0)
    {
        ProxyLoopBody pbody(body, range, nstripes);
        cv::Range stripeRange = pbody.stripeRange();
#if defined HAVE_TBB
        tbb::parallel_for(tbb::blocked_range<int>(stripeRange.start, stripeRange.end), pbody);
#elif defined HAVE_CSTRIPES
        parallel(MAX(0, numThreads))
        {
            int offset = stripeRange.start;
            int len = stripeRange.end - offset;
            Range r(offset + CPX_RANGE_START(len), offset + CPX_RANGE_END(len));
            pbody(r);
            barrier();
        }
#elif defined HAVE_OPENMP
        #pragma omp parallel for schedule(dynamic)
        for (int i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));
#elif defined HAVE_GCD
        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);
#elif defined HAVE_CONCURRENCY
        if(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            Concurrency::parallel_for(stripeRange.start, stripeRange.end, pbody);
        }
        else
        {
            pplScheduler->Attach();
            Concurrency::parallel_for(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }
#else
#error You have hacked and compiling with unsupported parallel framework
#endif
    }
    else
#endif // CV_PARALLEL_FRAMEWORK
    {
        (void)nstripes;
        body(range);
    }
}
```
其中使用的:
```cpp
tbb::parallel_for(tbb::blocked_range<int>(stripeRange.start, stripeRange.end), pbody);
```
使用TBB重写 parallel_for 完成使用TBB并行加速。
