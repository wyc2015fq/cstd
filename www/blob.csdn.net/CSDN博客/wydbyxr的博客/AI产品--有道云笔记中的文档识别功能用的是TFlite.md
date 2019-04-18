# AI产品--有道云笔记中的文档识别功能用的是TFlite - wydbyxr的博客 - CSDN博客
2018年11月22日 09:47:13[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：19
# 有道云笔记中的文档识别功能用的是TFlite
  在 TFLite 发布之前，有道云笔记中的文档识别功能是基于移动端 TensorFlow 库 (TensorFlow Mobile) 的。当 TFLite 发布后，我们希望迁移到 TFLite 上。促使我们迁移的主要动力是链接库的体积。
  经过压缩后，Android 上的 TensorFlow 动态库的体积大约是 4.5M 左右。如果希望满足 Android 平台下的多种处理器架构，可能需要打包 4 个左右的动态库，加起来体积达到 18M 左右；而 tflite 库的体积在 600K 左右，即便是打包 4 个平台下的链接库，也只需要占用 2.5M 左右的体积。这在寸土寸金的移动 App 上，价值是很大的。
