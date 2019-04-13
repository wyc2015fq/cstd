
# 为 OpenCL 提供支模版支持 - 丁丁的博客 - CSDN博客


2015年06月23日 15:05:49[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：1246标签：[OpenCL																](https://so.csdn.net/so/search/s.do?q=OpenCL&t=blog)[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[template																](https://so.csdn.net/so/search/s.do?q=template&t=blog)[GPU																](https://so.csdn.net/so/search/s.do?q=GPU&t=blog)[GPGPU																](https://so.csdn.net/so/search/s.do?q=GPGPU&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=GPU&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=template&t=blog)个人分类：[原创																](https://blog.csdn.net/JackyTintin/article/category/1194487)[编程语言																](https://blog.csdn.net/JackyTintin/article/category/6788740)[HPC																](https://blog.csdn.net/JackyTintin/article/category/6788759)[
							](https://blog.csdn.net/JackyTintin/article/category/6788740)
[
																								](https://blog.csdn.net/JackyTintin/article/category/1194487)

[
		](https://so.csdn.net/so/search/s.do?q=template&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=OpenCL&t=blog)
OpenCL本身（至少目前）并不支持模版。在许多场景下（例如移植CUDA代码），这会带来一些麻烦。
这里，我们介绍一种解决方案。主要思路是，利用OpenCL在运行时编译的特点，通过宏定义实现类似C++模型的功能。
首先我们要解决动态获取类型名称的问题
解决方法如下：
template<typenameT>structTypeParseTraits {staticconstchar*name;
};template<typenameT>constchar* TypeParseTraits<T>::name ="Unsupported Type!";\#define REGISTER_PARSE_TYPE(T) template<> struct TypeParseTraits<T> {\staticconstchar* name; \
 };  \template<>constchar* TypeParseTraits<T>::name =\#T;REGISTER_PARSE_TYPE(int);
REGISTER_PARSE_TYPE(float);
REGISTER_PARSE_TYPE(double);对于注册过的类似，我们可以通过 TypeParseTraits::name得到相应的字符串。对于未注册的类型，默认返回“Unsupported”。（一个实例参见[boostCompute](https://github.com/boostorg/compute/blob/master/include/boost/compute/type_traits/type_name.hpp)）
另外一种类似的方法如下：
template<typenameT>structCLTypes {staticconstchar* getName() {return"Unsupported"; }
};template<>structCLTypes<int> {staticconstchar* getName() {return"int"; }
};template<>structCLTypes<float> {staticconstchar* getName() {return"float"; }
};template<>structCLTypes<double> {staticconstchar* getName() {return"double"; }
};需要注意的是，无论哪各方法，我们可能会选择使用C++提供的内置函数功能。
typeid(T).name()但 C++ 标准并不保证能返回期望的结果（例如int并不保证能返回我们期望的字符串”int”）（[参见这里](http://www.cplusplus.com/reference/typeinfo/type_info/name/)），因此，使用它这会存在潜在的移植性问题。
动态获得类型名称的基础上，我们可以通过宏替换 OpenCL kernel 定义中的”模型“类型。
staticstd::once_flagcompiled;std::call_once(compiled, []() {std::ostringstreamoptions;
        options <<"-D T="<<TypeParseTraits<T>::name;
        prg =cl::Program(addVectors_src,false);
        prg.build(options.str().c_str());std::cout<<"vector addition kernel compiled for type: "<<TypeParseTraits<T>::name<<std::endl;
        kl =cl::Kernel(prg,"addVectors_kl");
    });为了保证每个kernel源码只会编译一次，这里我们使用了C++11的call_once功能（[参见这里](http://www.cplusplus.com/reference/mutex/call_once/?kw=call_once)）
实现这个目的的另一种方法是使用利用常规的静态变量。
static bool compliled = []() {std::ostringstreamoptions;
        options <<"-D T="<<TypeParseTraits<T>::name;
        prg =cl::Program(addVectors_src,false);
        prg.build(options.str().c_str());std::cout<<"vector addition kernel compiled for type: "<<TypeParseTraits<T>::name<<std::endl;
        kl =cl::Kernel(prg,"addVectors_kl");returntrue;
    };这里，两种方法并无差别。但在其他情形下，call_once可能会更灵活一些，参见[讨论](http://stackoverflow.com/questions/17407553/what-is-the-difference-between-stdcall-once-and-function-level-static-initiali)。
完整的代码如下：
\#define STRINGFY(src) \#srctemplate<typenameT>structTypeParseTraits {staticconstchar* name ="Unsupported";
};\#define REGISTER_PARSE_TYPE(X) template<> struct TypeParseTraits<X> { \staticconstchar* name =\#X; \}; 
REGISTER_PARSE_TYPE(int);
REGISTER_PARSE_TYPE(float);
REGISTER_PARSE_TYPE(double);template<typenameT>voidaddVectors(vector<T>&out,vector<T>&in1,vector<T>&in2, size_t n) {staticstd::once_flag compiled;staticcl::Program prg;staticcl::Kernel kl;staticconstchar* addVectors_src = STRINGFY(
        kernelvoidaddVectors_kl(globalconstT *consta, globalconstT *constb, global T *restrictconstc) {unsignedidx = get_global_id(0);
        c[idx] = a[idx] + b[idx];
        });std::call_once(compiled, []() {std::ostringstreamoptions;
        options <<"-D T="<< TypeParseTraits<T>::name;
        prg = cl::Program(addVectors_src,false);
        prg.build(options.str().c_str());std::cout<<"vector addition kernel compiled for type: "<< TypeParseTraits<T>::name <<std::endl;
        kl = cl::Kernel(prg,"addVectors_kl");
    });
    Buffer a(begin(in1), end(in1),true,false);
    Buffer b(begin(in2), end(in2),true,false);
    Buffer c(CL_MEM_READ_WRITE, n *sizeof(T));autoaddVectors_kl = cl::make_kernel<Buffer, Buffer, Buffer>(kl);
    addVectors_kl(EnqueueArgs(n), a, b, c);
    cl::copy(c, begin(out), end(out));
}一个简单的测试
voidpseudotemplate_test() {constintn =10;vector<int>iv1(n,1);vector<int>iv2(n,10);vector<int>iv3(n);vector<double>dv1(n,2.0);vector<double>dv2(n,3.0);vector<double>dv3(n);
    addVectors(iv3, iv1, iv2, iv1.size());
    addVectors(dv3, dv1, dv2, dv1.size());for(inti =0; i < n; i++) {cout<< iv3[i] <<":\t"<< iv1[i] + iv2[i] << endl;
    }cout<< endl;for(inti =0; i < n; i++) {cout<< dv3[i] <<":\t"<< dv1[i] + dv2[i] << endl;
    }cout<< endl;
}输出
11:1111:1111:1111:1111:1111:1111:1111:1111:1111:115:55:55:55:55:55:55:55:55:55:5关于STRINGFY，参见
[1. stackoverflow上关于动态获取类型名称字符串的讨论](http://stackoverflow.com/a/1055563)
[2. Templating and Caching OpenCL Kernels](http://arrayfire.com/templating-and-caching-opencl-kernels/)

