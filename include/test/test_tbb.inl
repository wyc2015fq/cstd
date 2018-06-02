#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

// 链接tbb_debug.lib
#pragma comment(lib, "tbb_debug.lib")

using namespace tbb;

// 对每个Item执行该操作
void Foo(float value)
{
    printf("%.2f ", value);
}

class ApplyFoo
{
    float * const my_a;
public:
    void operator () (const blocked_range<size_t> & r) const
    {
        float * a = my_a;
        for (size_t i = r.begin(); i != r.end(); ++ i)
            Foo(a[i]);
    }
    
    ApplyFoo(float a[]) : my_a(a) {}
};

int test_tbb()
{
    // 创建task scheduler
    // task_scheduler_init支持一个参数，以指定使用的线程数
    task_scheduler_init init;
    float a[100];
    for (int i = 0; i < 100; i ++)
        a[i] = (float)i;
    // TBB会把数组分成若干的block
    // 对block调用ApplyFoo这个functor
    parallel_for(blocked_range<size_t>(0, 100), ApplyFoo(a));
    return 0;
}