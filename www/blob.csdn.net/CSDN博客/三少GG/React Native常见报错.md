# React Native常见报错 - 三少GG - CSDN博客
2016年07月10日 17:22:44[三少GG](https://me.csdn.net/scut1135)阅读数：8456
一。 Failed run android simulator
 : No connected devices!
https://github.com/facebook/react-native/issues/3091

When you have the platform, and the device, you can then boot the device with emulator:
```
$  **emulator -avd <name>**
```
**eg: emulator -avd React_API22**
Once booted, the command you were likely trying to run `react-native run-android` should produce the effect of a react mobile app
 running in your emulator window.
二。ReactNative经典报错
解决办法：
1.
注意Android5.0以上版本命令方式：
打开Android SDK目录 xxx\Android\sdk\platform-tools。在当前文件路径呼出命令行，输入：
**adb reverse tcp:8081 tcp:8081**
**点击Reload JS 即可成功拉取ReactNative bundle。**
2.
我们摇一摇手机，点击Dev Settings后，点击Debug server host & port for device,设置IP和端口。
![](https://img-blog.csdn.net/20160615171516211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20160615171520961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里的IP是电脑的IP，端口号固定8081。
![](https://img-blog.csdn.net/20160615171529883?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
设置完成后，回到空白页面，再次摇一摇手机，选择Reload JS，程序就运行起来，出现Welcome to React Native！
![](https://img-blog.csdn.net/20160615171646712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个信息是在项目目录下index.android.js里的，可以打开看看：
class HelloAndroid extends Component {
  render() {
    return (
      <View style={styles.container}>
        <Text style={styles.welcome}>
          Welcome to React Native!
        </Text>
        <Text style={styles.instructions}>
          To get started, edit index.android.js
        </Text>
        <Text style={styles.instructions}>
          Shake or press menu button for dev menu
        </Text>
      </View>
    );
  }
}
3.   RN 需要启动一个 Dev server 来辅助开发，Android 5.0 可以直接通过 USB 的访问，5.0 以下只能通过 WiFi 来访问。
adb reverse was introduced in Android 5.0。Please provide alternative solution.
解决办法：
- Press the device Menu button
- Select Dev Settings
- Select Debug server host for device
- Type in your machine's IP and Reload JS (it was 192.168.1.100:8081 for me)
别忘电脑、手机需在同一wifi网络下!
4.has not been registered.
http://www.jianshu.com/p/82a09063e61c
# 但是，还有一种情况！
即便你确保一致了但还是出现相同的错误提示，这又是怎么搞得呢？这个时候你可以检查一下你的命令行。有可能你同时在运行一个以上的程序，像我。如果你的react-native在运行程序A而你打开了程序B，也会出现相同的问题。解决方法很简单，关掉命令行运行程序。
我遇到了你文中所说的后者那个问题，**没有关掉上一个项目运行所开的终端。**
5. null is not an object
未bind(this)
显示`null is not an object (evaluating '_this3._pressRow')`，上图： 
![这里写图片描述](https://img-blog.csdn.net/20160713222800920)
一看就知道是找不到方法，后来查阅这个资料，知道了解决方法，原来renderRow要与外面的this绑定起来，这个跟Android里的上下文环境Context很像：
```
<ListView 
                    contentContainerStyle = {styles.list}
                    pageSize={2}
                    dataSource = {this.state.dataSource}
                    renderRow={this._renderRow.bind(this)}
                />
```
