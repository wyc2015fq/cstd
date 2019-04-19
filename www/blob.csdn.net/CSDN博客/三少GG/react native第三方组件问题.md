# react native第三方组件问题 - 三少GG - CSDN博客
2016年07月27日 11:36:17[三少GG](https://me.csdn.net/scut1135)阅读数：1288
组件 [react-native-router-flux](https://github.com/aksonov/react-native-router-flux) 中的tabbar
 在安卓机器上反应速度非常慢，
已有issue提问:  [https://github.com/aksonov/react-native-router-flux/issues/199](https://github.com/aksonov/react-native-router-flux/issues/199)
# Extremely slow on android #199
看似原因：
 For future reference make sure to not have console.log anywhere in the code, whether it is your redux middleware or in the debug for this library
实则：
## turn off "JS Dev Mode （已解决问题,只需一步）
You will want to make sure that you turn off "JS Dev Mode," or else it
will run painfully slow on device.
After running "react-native run-android" you should "shake" your device
to bring up the menu. Select "Dev Settings" then uncheck "JS Dev Mode."
After that run "react-native run-android" again and it should be more 
performant, at least I hope for you :)
