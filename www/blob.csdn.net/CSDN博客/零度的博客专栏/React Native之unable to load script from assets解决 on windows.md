# React Native之unable to load script from assets解决 on windows - 零度的博客专栏 - CSDN博客
2017年12月25日 17:01:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：193

1.进入到项目根目录，在main下建立一个assets文件夹，mkdir android/app/src/main/assets
2.运行命令：
react-native bundle --platform android --dev false --entry-file index.android.js --bundle-output android/app/src/main/assets/index.android.bundle
 --assets-dest android/app/src/main/res
3.最后在执行命令：react-native run-android
注意：Using npm version 4.3.0 react-native-cli version 2.01 react-native version 0.49.5. In project directory.
            The file name has changed from index.android.js to index.js.词句意思是说，react-native 0.49以后index.android.js已经被替换为index.js
因此,执行应变成如下命令：
- `mkdir android/app/src/main/assets`
- `react-native bundle --platform android --dev false --entry-file index.js --bundle-output android/app/src/main/assets/index.android.bundle --assets-dest android/app/src/main/res`
- `react-native run-android`

