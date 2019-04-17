# webrtc编译笔记 - tifentan的专栏 - CSDN博客





2016年12月05日 16:33:42[露蛇](https://me.csdn.net/tifentan)阅读数：147








有时间再整理。

ios下xcode：



export GYP_GENERATOR_FLAGS="xcode_project_version=7.2 xcode_ninja_target_pattern=All_iOS xcode_ninja_executable_target_pattern=AppRTCDemo output_dir=out_ios"

export GYP_GENERATORS="ninja,xcode-ninja"




GYP_DEFINES="OS=ios target_arch=arm64"

webrtc/build/gyp_webrtc




如遇到



```
AssertionError: Multiple codesigning fingerprints for identity: iPhone Developer
```




这个错误是由于mac os 上同时存在多个签名证书造成的。 

执行：

```
security find-identity
```


查看本机上具有多少个有效签名。有效签名为Valid identities only下面列出来的。

```
find . -name common.gypi | xargs grep CODE_SIGN_IDENTITY
```


应该看到这么几行

```
./chromium/src/build/common.gypi:                  'CODE_SIGN_IDENTITY[sdk=iphoneos*]': 'iPhone Developer',
./chromium/src/build/common.gypi:                  'CODE_SIGN_IDENTITY[sdk=iphoneos*]': '',
```


将iPhone Developer 修改为我们选择的合适的签名，然后重新执行：
`webrtc/build/gyp_webrtc`






参考

http://www.jianshu.com/p/1b4c79b45055


http://blog.csdn.net/cffishappy/article/details/51083900






