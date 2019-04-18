# Caffe RPN：把RPN网络layer添加到caffe基础结构中 - wishchinYang的专栏 - CSDN博客
2018年03月14日 09:56:25[wishchin](https://me.csdn.net/wishchin)阅读数：140
          在测试MIT Scene Parsing Benchmark (SceneParse150)使用FCN网络时候，遇到Caffe错误。
          遇到错误：不可识别的网络层crop 网络层
    CreatorRegistry& registry = Registry();
    CHECK_EQ(registry.count(type), 1) << "Unknown layer type: " << type
        << " (known types: " << LayerTypeListString() << ")";
          重新添加caffe源代码里面的crop_layer.cpp到工程layer文件夹，重新编译。
