# 详解 NSURLSession - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月18日 15:14:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1054
本文翻译自 [NSURLSession Tutorial: Getting Started](https://www.raywenderlich.com/110458/nsurlsession-tutorial-getting-started)
本文来自简书，原文地址:[http://www.jianshu.com/p/158e2a8c545e](http://www.jianshu.com/p/158e2a8c545e)
![](http://upload-images.jianshu.io/upload_images/861914-8921e5e9cb2955ad.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
学习如何用 NSURLSession 实现 HTTP 数据请求和文件下载！
App 无论是从服务器拉取应用数据，还是更新社交媒体状态或是下载远程文件到硬盘里，都是 HTTP 网络请求实现的，它们就是移动应用的心脏。
为了满足开发者对于网络请求的众多要求，苹果提供了 NSURLSession，这是一套完整的网络 API 方法，用于通过 HTTP 上传和下载内容。
在本教程中，我们会学习如何使用 NSURLSession 构建 Half Tunes app，它可以让我们查询 iTunes Search API 并下载选中歌曲的 30 秒试听。最终的 app 还会支持后台传输，用户可以暂停、恢复或取消正在进行中的下载。
## 开始
下载 [启动项目](https://koenig-media.raywenderlich.com/uploads/2016/01/HalfTunes-Starter.zip) ；它已包含用于搜索歌曲和显示结果的用户界面，还有用于解析 JSON 和播放曲目的帮助方法。它们可以让你专注于实现
 app 的网络部分。
构建并运行项目；可以看到一个视图，搜索条在顶端，空的表格视图在下方：
![](http://upload-images.jianshu.io/upload_images/861914-9676a1d18a534096.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在搜索条中输入然后点击 Search。视图仍然是空白的，但不用担心；我们会调用新的 NSURLSession 以改变这种情况。
## NSURLSession 概况
开始之前，有必要了解一下 NSURLSession 以及它的组成部分，所以花一分钟看一遍下面的快速概况。
NSURLSession 技术上是既是一个类，也是一组用于处理基于 HTTP/HTTPS 请求的类：
![](http://upload-images.jianshu.io/upload_images/861914-1971781c1f2a842d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
NSURLSession 是负责发送和接收 HTTP 请求的关键对象。通过 NSURLSessionConfiguration 来创建它，有三种风格：
- defaultSessionConfiguration：创建默认配置的对象，使用硬盘持久化的全局缓存、credential 和 cookie 存储对象。
- ephemeralSessionConfiguration：和默认配置类似，除了所有会话相关的数据都被存储在内容中。把它想象成“私人”会话。
- backgroundSessionConfiguration：允许会话在后台执行上传和下载任务。即使 app 本身被暂停或终止了，传输都会继续。
NSURLSessionConfiguration 还可以让你配置会话属性，如超时值、缓存策略和附加 HTTP 头。有关配置选项的完整列表，请参见 [文档](https://developer.apple.com/library/mac/documentation/Foundation/Reference/NSURLSessionConfiguration_class/index.html#//apple_ref/occ/cl/NSURLSessionConfiguration) 。
NSURLSessionTask 是表示任务对象的抽象类。会话会创建一个任务，用来执行获取数据和下载、上传文件的实际工作。
在这个情境中，有三种类型的具体会话任务：
- NSURLSessionDataTask：将此任务用于 HTTP GET 请求以及把服务器数据取到内容中。
- NSURLSessionUploadTask：使用此任务可以将文件从磁盘上传到Web服务，通​​常通过HTTP POST或PUT方法。
- NSURLSessionDownloadTask：使用此任务将文件从远程服务下载到临时文件位置。
![](http://upload-images.jianshu.io/upload_images/861914-cce8ee59538ea9f9.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我们还可以暂停、恢复和取消任务。NSURLSessionDownloadTask 具有额外的暂停功能以备未来可以恢复。
通常，NSURLSession 以两种方式返回数据：当任务成功完成或出现错误时，通过 completion. Handler，或通过调用委托方法（在创建会话时设置的）。
现在我们已经了解了 NSURLSession 可以做的事，是时候将理论付诸实现了！
## 查询曲目
我们将首先在用户搜索曲目时添加查询 iTunes Search API 的代码。
在 SearchViewController.swift 中，将如下代码添加到类的顶部：
```
// 1
let defaultSession = NSURLSession(configuration: NSURLSessionConfiguration.defaultSessionConfiguration())
// 2
var dataTask: NSURLSessionDataTask?
```
上面的代码我们做了这些事：
- 创建了 NSURLSession 然后用默认会话配置初始化它。
- 声明了一个 NSURLSessionDataTask 变量，在用户执行搜索时用于向 iTunes Search 网络服务发出 HTTP GET 请求。data task 会在每次用户创建新查询的时候被重新初始化和复用。
现在，用如下代码替换掉 searchBarSearchButtonClicked(_:)：
```
func searchBarSearchButtonClicked(searchBar: UISearchBar) {
  dismissKeyboard()
  if !searchBar.text!.isEmpty {
    // 1
    if dataTask != nil {
      dataTask?.cancel()
    }
    // 2
    UIApplication.sharedApplication().networkActivityIndicatorVisible = true
    // 3
    let expectedCharSet = NSCharacterSet.URLQueryAllowedCharacterSet()
    let searchTerm = searchBar.text!.stringByAddingPercentEncodingWithAllowedCharacters(expectedCharSet)!
    // 4
    let url = NSURL(string: "https://itunes.apple.com/search?media=music&entity=song&term=\(searchTerm)")
    // 5
    dataTask = defaultSession.dataTaskWithURL(url!) {
      data, response, error in
      // 6
      dispatch_async(dispatch_get_main_queue()) {
        UIApplication.sharedApplication().networkActivityIndicatorVisible = false
      }
      // 7
      if let error = error {
        print(error.localizedDescription)
      } else if let httpResponse = response as? NSHTTPURLResponse {
        if httpResponse.statusCode == 200 {
          self.updateSearchResults(data)
        }
      }
    }
    // 8
    dataTask?.resume()
  }
}
```
按顺序讨论一下每个数字注释：
- 每次用户查询时，检查 data task 是否已经被初始化了。如果是，就取消之，因为我们要复用这个 Data task 对象以用于最新的查询。
- 在状态栏上启用网络活动 indicator，以向用户指明有一个正在进行的网络进程。
- 在将用户的搜索字符串作为参数传递给查询 URL 之前，在该字符串上调用 `stringByAddingPercentEncodingWithAllowedCharacters(_:)` 以确保被正确转义了。
- 下一步我们构造了一个 NSURL，将转义后的搜索字符串作为 GET 参数附加到 iTunes Search API 的 base url 上。
- 从我们创建的会话中初始化一个 NSURLSessionDataTask 以处理 HTTP GET 请求。NSURLSessionDataTask 的构造器带有一个 NSURL 参数以及一个 completion handler，以供 data task 完成时调用。
- 如果收到了任务完成的回调，在主线程中隐藏 activity indicator 并调用 UI 刷新。
- 如果 HTTP 请求成功了，调用 updateSearchResults(_:)，它将 response NSData 解析为 Tracks 然后更新 table view。
- 默认情况下，所有任务刚开始时都是暂停状态；调用 resume() 以启动 data task。
构建并运行 app；搜索任意音乐，可以看到 table view 充满了相关的曲目结果，如下所示：
![](http://upload-images.jianshu.io/upload_images/861914-3ea93557707ab662.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在我们注入了一点 NSURLSession 魔法后，Half Tunes 现在已经有点用处了！
## 下载曲目
能够看到歌曲结果是一件很爽的事，但如果点击歌曲就能下载岂不是更棒棒？这就是我们接下来要做的事情。
为了能够处理多个下载，首先创建一个自定义对象以管理活动下载的状态。
在 Data Objects 组中创建一个新文件，命名为 Download.swift。
打开 Download.swift 然后添加如下实现：
```
class Download: NSObject {
  var url: String
  var isDownloading = false
  var progress: Float = 0.0
  var downloadTask: NSURLSessionDownloadTask?
  var resumeData: NSData?
  init(url: String) {
    self.url = url
  }
}
```
梳理一下 Download 的属性：
- url：需要下载文件的 URL。也是 Download 的唯一标识符。
- isDownloading：下载是在进行中还是被暂停了。
- progress：下载的小数进度；介于 0.0 和 1.0 之间的 float 型。
- downloadTask：下载文件的 NSURLSessionDownloadTask。
- resumeData：存储在暂停下载任务时生成的 NSData。如果主机服务器支持，可以用它来恢复暂停的下载。
切换到 SearchViewController.swift 然后在类顶部添加如下代码：
`var activeDownloads = [String: Download]()`
维持 URL 和活跃下载之间的映射而已。
### 创建下载任务
准备工作都结束了，现在可以实现文件下载了。首先创建一个专用会话以处理下载任务。
在 SearchViewController.swift 中，在 viewDidLoad() 之前添加如下代码：
```
lazy var downloadsSession: NSURLSession = {
  let configuration = NSURLSessionConfiguration.defaultSessionConfiguration()
  let session = NSURLSession(configuration: configuration, delegate: self, delegateQueue: nil)
  return session
}()
```
这里我们用默认配置初始化了一个单独的会话以处理所有下载任务。还指定了一个 delegate，以通过委托调用接收 NSURLSession 事件。这样会很有用，不仅可以追踪任务完成，还有任务进度。
将 delegate queue 设置为 nil 会让会话创建一个串行操作队列（默认值）以执行对委托方法和 completion handlers 的调用。
注意 downloadsSession: 的 lazy 创建：可以把会话的创建延迟到真正需要使用它的时刻。最重要的是，可以把 self 作为 delegate 参数传给初始化程序 —— 即使 self 并未并初始化。
在 SearchViewController.swift 中，找到空的 NSURLSessionDownloadDelegate 扩展然后改成这样：
```
extension SearchViewController: NSURLSessionDownloadDelegate {
  func URLSession(session: NSURLSession, downloadTask: NSURLSessionDownloadTask, didFinishDownloadingToURL location: NSURL) {
    print("Finished downloading.")
  }
}
```
NSURLSessionDownloadDelegate 定义了使用 NSURLSession 下载任务时需要实现的代理方法。唯一不可或缺的方法是 URLSession(_:downloadTask:didFinishDownloadingToURL:)，下载完成时调用。现在我们就在下载完成时打印一条消息就可以了。
会话和 delegate 都配置好后，重要可以开始在用户请求下载歌曲的时候创建下载任务了。
在 SearchViewController.swift 中，将 `startDownload(_:)` 替换为如下实现：
```
func startDownload(track: Track) {
  if let urlString = track.previewUrl, url =  NSURL(string: urlString) {
    // 1
    let download = Download(url: urlString)
    // 2
    download.downloadTask = downloadsSession.downloadTaskWithURL(url)
    // 3
    download.downloadTask!.resume()
    // 4
    download.isDownloading = true
    // 5
    activeDownloads[download.url] = download
  }
}
```
用户点击某个曲目的 Download 按钮的时候，带上相应的 Track 调用 startDownload(_:)。解释如下：
- 首先用 track 的 preview URL 来初始化 Download。
- 使用新的会话对象创建带有 preview URL 的 NSURLSessionDownloadTask，然后将其设置为 Download 的 downloadTask 属性。
- 调用 resume() 以启动下载任务。
- 指示下载正在进行中。
- 最后，在 activeDownloads 字典中将下载 URL 映射到 Download。
构建并运行 app；搜索任意歌曲然后点击单元格上的 Download 按钮。一段时间后，应该会在控制台上看到一条消息，表示下载完成。
### 保存和播放曲目
下载任务完成后，URLSession(_:downloadTask:didFinishDownloadingToURL:) 会提供临时文件位置 URL。方法返回前，我们要将其移动到 app 沙箱容器目录中的永久位置。还有，必须从字典中移除活动下载并更新 table view。
添加一个帮助方法以简化这个步骤。在 SearchViewController.swift 中，将以下方法添加到类中：
```
func trackIndexForDownloadTask(downloadTask: NSURLSessionDownloadTask) -> Int? {
  if let url = downloadTask.originalRequest?.URL?.absoluteString {
    for (index, track) in searchResults.enumerate() {
      if url == track.previewUrl! {
        return index
      }
    }
  }
  return nil
}
```
该方法只返回给定 URL 的 Track 在 searchResults 列表中的索引。
下一步，将 URLSession(_:downloadTask:didFinishDownloadingToURL:) 替换为如下代码：
```
func URLSession(session: NSURLSession, downloadTask: NSURLSessionDownloadTask, didFinishDownloadingToURL location: NSURL) {
  // 1
  if let originalURL = downloadTask.originalRequest?.URL?.absoluteString,
    destinationURL = localFilePathForUrl(originalURL) {
    print(destinationURL)
    // 2
    let fileManager = NSFileManager.defaultManager()
    do {
      try fileManager.removeItemAtURL(destinationURL)
    } catch {
      // Non-fatal: file probably doesn't exist
    }
    do {
      try fileManager.copyItemAtURL(location, toURL: destinationURL)
    } catch let error as NSError {
      print("Could not copy file to disk: \(error.localizedDescription)")
    }
  }
  // 3
  if let url = downloadTask.originalRequest?.URL?.absoluteString {
    activeDownloads[url] = nil
    // 4
    if let trackIndex = trackIndexForDownloadTask(downloadTask) {
      dispatch_async(dispatch_get_main_queue(), {
        self.tableView.reloadRowsAtIndexPaths([NSIndexPath(forRow: trackIndex, inSection: 0)], withRowAnimation: .None)
      })
    }
  }
}
```
上面的关键步骤解释如下：
- 从任务中提取出原始请求 URL 然后将其传递给 localFilePathForUrl(:) 帮助方法。localFilePathForUrl(:) 随后就会生成一个用来存储的永久本地文件路径（通过将 URL 的 lastPathComponent（即文件名和扩展名）附加到 app 的 Documents 路径上）。
- 使用 NSFileManager，将下载的文件从临时文件位置移动到所需的目的文件路径（开始复制之前，清楚那个位置上的文件）。
- 在活跃下载中查找相应的 Download 并将其移除。
- 最后，在 table view 里找到那个 Track 然后重载相应的单元格。
构建并运行项目；选择任意曲目然后下载它。下载完成时，应该可以看到控制台中打出了文件路径位置：
![](http://upload-images.jianshu.io/upload_images/861914-5982ab41c481f425.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
下载按钮也会消失，因为曲目现在已经在设备上了。点击曲目就会在显示的 MPMoviePlayerViewController 中听到它播放，如下所示：
![](http://upload-images.jianshu.io/upload_images/861914-3c17b4ea16708a57.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 监测下载进度
目前，我们无法监控下载进度。为了改善用户体验，我们会改动 app 以监听下载进度事件，并在单元格中显示进度。
在 SearchViewController.swift 中，找到那个实现 NSURLSessionDownloadDelegate 的扩展，然后添加如下代理方法：
```
func URLSession(session: NSURLSession, downloadTask: NSURLSessionDownloadTask, didWriteData bytesWritten: Int64, totalBytesWritten: Int64, totalBytesExpectedToWrite: Int64) {
    // 1
    if let downloadUrl = downloadTask.originalRequest?.URL?.absoluteString,
      download = activeDownloads[downloadUrl] {
      // 2
      download.progress = Float(totalBytesWritten)/Float(totalBytesExpectedToWrite)
      // 3
      let totalSize = NSByteCountFormatter.stringFromByteCount(totalBytesExpectedToWrite, countStyle: NSByteCountFormatterCountStyle.Binary)
      // 4
      if let trackIndex = trackIndexForDownloadTask(downloadTask), let trackCell = tableView.cellForRowAtIndexPath(NSIndexPath(forRow: trackIndex, inSection: 0)) as? TrackCell {
        dispatch_async(dispatch_get_main_queue(), {
          trackCell.progressView.progress = download.progress
          trackCell.progressLabel.text =  String(format: "%.1f%% of %@",  download.progress * 100, totalSize)
        })
    }
  }
}
```
一步步浏览这个代理方法：
- 使用提供的 downloadTask，取出 URL 然后用它在活跃下载目录中找到 Download。
- 该方法还返回写入的总字节数以及预期写入的总字节数。计算两个值的比值就是进度，然后将结果保存到 Download 中。我们会使用这个值来更新 progress view。
- NSByteCountFormatter 带有一个字节值参数，然后生成友好的下载文件总尺寸的字符串。我们会使用此字符串来显示下载的大小以及完成百分比。
- 最后，找到负责显示这个 Track 的单元格，然后同时刷新其进度视图与进度 label（借助前面步骤得到的值）。
下一步，配置单元格以正确显示进行中的下载的 progress view 和状态。
在 tableView(_:cellForRowAtIndexPath:) 中找到如下代码行：
```bash
let downloaded = localFileExistsForTrack(track)
```
在上面那行前面添加如下代码：
```
var showDownloadControls = false
if let download = activeDownloads[track.previewUrl!] {
  showDownloadControls = true
  cell.progressView.progress = download.progress
  cell.progressLabel.text = (download.isDownloading) ? "Downloading..." : "Paused"
}
cell.progressView.hidden = !showDownloadControls
cell.progressLabel.hidden = !showDownloadControls
```
对于有活跃下载的曲目，将 showDownloadControls 设置为 true；否则，将其设置为 false。然后根据 showDownloadControls 的值来显示进度视图和标签（示例项目中已提供）。
对于被暂停的下载，状态显示为 “Paused”；否则，显示 “Downloading…”。
最后，将下面这行：
`cell.downloadButton.hidden = downloaded`
替换为：
`cell.downloadButton.hidden = downloaded || showDownloadControls`
在这里，如果曲目正在下载也要隐藏 Download 按钮。
构建并运行项目；下载任意曲目，应该看到随着下载进行，进度条状态的更新：
![](http://upload-images.jianshu.io/upload_images/861914-2a02e312f0a30437.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
棒棒的，我们有了重大进展！:]
## 暂停、恢复和取消下载
如果需要暂停或完全取消下载，该怎么做？在本节中，我们会实现暂停、恢复和取消功能，使用户能够完全控制下载过程。
先从让用户取消活跃下载开始。
替换 cancelDownload(_:) 为如下代码：
```
func cancelDownload(track: Track) {
  if let urlString = track.previewUrl,
    download = activeDownloads[urlString] {
      download.downloadTask?.cancel()
      activeDownloads[urlString] = nil
  }
}
```
为了取消下载，从活动下载字典中相应的 Download 中取出下载任务，然后对其调用 cancel() 以取消任务。然后从活动下载字典中移除它。
暂停下载在概念上和取消很相似；区别在于暂停会取消下载任务，但也会产生恢复数据，其中包含足够的信息以在未来恢复下载，需要主机服务器支持该功能。
> 
注意：只能在特定情况下恢复下载。例如，首次请求后，资源不能被修改。有关情况的完整列表，请在 [这里](https://developer.apple.com/library/ios/documentation/Foundation/Reference/NSURLSessionDownloadTask_class/index.html#//apple_ref/occ/instm/NSURLSessionDownloadTask/cancelByProducingResumeData:) 查看苹果文档。
现在，将 pauseDownload(_:) 替换为如下代码：
```
func pauseDownload(track: Track) {
  if let urlString = track.previewUrl,
    download = activeDownloads[urlString] {
      if(download.isDownloading) {
        download.downloadTask?.cancelByProducingResumeData { data in
          if data != nil {
            download.resumeData = data
          }
        }
        download.isDownloading = false
      }
  }
}
```
这里的主要区别是调用 cancelByProducingResumeData(:) 而不是 cancel()。从 cancelByProducingResumeData(:) 提供的闭包中取回恢复数据，然后将其存储到合适的 Download 中以备将来恢复。
我们还将 Download 的 isDownloading 属性设置为 false 以表示下载被暂停了。
暂停功能完成后，下面我们要恢复被暂停的下载。
将 resumeDownload(_:) 替换为如下代码：
```
func resumeDownload(track: Track) {
  if let urlString = track.previewUrl,
    download = activeDownloads[urlString] {
      if let resumeData = download.resumeData {
        download.downloadTask = downloadsSession.downloadTaskWithResumeData(resumeData)
        download.downloadTask!.resume()
        download.isDownloading = true
      } else if let url = NSURL(string: download.url) {
        download.downloadTask = downloadsSession.downloadTaskWithURL(url)
        download.downloadTask!.resume()
        download.isDownloading = true
      }
  }
}
```
当用户恢复下载时，检查相应的 Download 是否存在恢复数据。如果有，带上恢复数据调用 downloadTaskWithResumeData(_:) 以创建一个新的下载任务，然后调用 resume() 来启动任务。如果由于某些原因缺少恢复数据，就从从头开始创建一个新的下载任务，并使用下载 URL 启动它。
两种情况下，都把 Download 的 isDownloading 标志设置为 true 来表示下载被恢复了。
还需要再做一件事，这三个功能就能正常工作了：我们需要按需显示或隐藏 Pause、Cancel 和 Resume 按钮。
跳到 tableView(_:cellForRowAtIndexPath:) 然后找到下面这行：
`if let download = activeDownloads[track.previewUrl!] {`
把下面两行代码添加到上面那个 let 代码块的底部：
```
let title = (download.isDownloading) ? "Pause" : "Resume"
cell.pauseButton.setTitle(title, forState: UIControlState.Normal)
```
因为暂停和恢复功能用同一个按钮，上面的代码就按需在两个状态间切换按钮。
下面，把下面的代码添加到 tableView(_:cellForRowAtIndexPath:) 的末端，return 语句之前：
```
cell.pauseButton.hidden = !showDownloadControls
cell.cancelButton.hidden = !showDownloadControls
```
这个 app 中，只有下载活跃时才会显示按钮。
构建并运行项目；同时下载几个曲目，你可以随意暂停、恢复和取消它们：
![](http://upload-images.jianshu.io/upload_images/861914-b0ee139e37d7131a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 启用后台传输
我们的 app 目前功能已经很强大了，但还可以做一个重大的改进：后台传输。在这种模式下，即使 app 在后台或因某原因崩溃掉时，下载还是会继续。
但是如果 app 都没有运行，那怎么工作呢？有一个在 app 外部单独运行的守护进程，管理后台传输任务；当运行下载任务时，它会将适当的 delegate 消息发送到 app。如果 app 在前台传输期间终止运行了，任务将在后台继续运行，不受影响。
任务完成后，守护进程将在后台重新启动该 app。被重新启动的 app 会再次连接到那个会话，接收相关的 completion delegate 消息并执行任何必须的操作，例如将下载的文件持久化存储到硬盘。
> 
注意：如果从 app 切换器 强制退出 app，系统会取消所有会话的后台传输，并不会尝试重启 app。
还是在 SearchViewController.swift，downloadsSession 的初始化方法里，找到下面这行代码：
`let configuration = NSURLSessionConfiguration.defaultSessionConfiguration()`
……替换为下面这行：
`let configuration = NSURLSessionConfiguration.backgroundSessionConfigurationWithIdentifier("bgSessionConfiguration")`
没有用默认会话配置，我们可以用一种特殊的后台会话配置。请注意，我们还可以在此处设置会话的唯一标识符，以便在需要时引用并“重新连接”到相同的后台会话。
接下来，在 viewDidLoad() 中，添加下面这行：
`_ = self.downloadsSession`
调用 lazy 加载的 downloadsSession 可以确保 app 在 SearchViewController 初始化时创建一个后台会话。
如果后台任务在 app 未运行时完成，该应用将在后台重新启动。我们需要在 app delegate 中处理这个事件。
切至 AppDelegate.swift 然后在类顶部添加下面这行代码：
`var backgroundSessionCompletionHandler: (() -> Void)?`
下一步，把下面的方法添加到 AppDelegate.swift：
```
func application(application: UIApplication, handleEventsForBackgroundURLSession identifier: String, completionHandler: () -> Void) {
  backgroundSessionCompletionHandler = completionHandler
}
```
这儿我们把提供的 completionHandler 以变量形式保存在 app delegate 以备后面使用。
application(_:handleEventsForBackgroundURLSession:) 会唤醒 app 来处理完成的后台任务。这个事件中需要处理两件事情：
- 首先，app 需要使用 delegate 方法提供的标识符重新连接到相应的后台会话。但是，由于每次实例化 SearchViewController 时都会创建并使用一个后台会话，因此这时已经重新连接了！
- 其次，需要捕捉由 delegate 方法提供的 completion handler。调用 completion handler 会使操作系统对更新后的 UI 进行快照，以便在应用程序切换器中显示，并告诉操作系统，app 当前会话的后台活动都已完成。
但是应该何时调用 completion handler？URLSessionDidFinishEventsForBackgroundURLSession(_:) 会是一个不错的选择；这时一个 NSURLSessionDelegate 方法，与后台会话相关的所有任务都完成时会触发。
在 SearchViewController.swift 中实现下面的扩展：
```
extension SearchViewController: NSURLSessionDelegate {
  func URLSessionDidFinishEventsForBackgroundURLSession(session: NSURLSession) {
    if let appDelegate = UIApplication.sharedApplication().delegate as? AppDelegate {
      if let completionHandler = appDelegate.backgroundSessionCompletionHandler {
        appDelegate.backgroundSessionCompletionHandler = nil
        dispatch_async(dispatch_get_main_queue(), {
          completionHandler()
        })
      }
    }
  }
}
```
上面的代码知识从 app delegate 中获取存储的 completion handler，并在主线程上调用它。
构建并运行 app；启动几次并发下载，然后点 home 键让 app 在后台运行。等待下载完成，然后双击 home 键显示 app 切换器。
下载应该已经完成，它们新的状态反映在 app 截图中。打开 app 确认一下：
![](http://upload-images.jianshu.io/upload_images/861914-a8bee0f9ee1bce1f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
现在我们已经有了一个功能齐全的音乐流媒体 app！下一步是挑战 Apple Music！:]
## 下一步？
[这里](https://koenig-media.raywenderlich.com/uploads/2016/01/HalfTunes-Final.zip) 可以下载本教程的完整项目。
恭喜！你现在已具备处理 app 中大多数常见网络需求的能力。NSURLSession 还有很多细节，这篇 NSURLSession 教程中装不下了，例如上传任务和会话配置设置（如超时值和缓存策略）。
要了解有关这些功能（和其他功能）的更多信息，请查看以下资源：
- 苹果 [文档](https://developer.apple.com/library/ios/documentation/Foundation/Reference/NSURLSession_class/) ，包含所有
 API 方法的细节。
- 我们自己的 [iOS 7 By Tutorials](http://www.raywenderlich.com/store/ios-7-by-tutorials) 书，整整两章都是专门讲
 NSURLSession。还可以看看我们之前的 [NSURLSession](http://www.raywenderlich.com/51127/nsurlsession-tutorial) 教程。
- [AlamoFire](https://github.com/Alamofire/Alamofire) 是一个流行的第三方 iOS 网络库；我们在 [Beginning
 Alamofire](http://www.raywenderlich.com/85080/beginning-alamofire-tutorial) 教程中讲了基础部分。
希望你能用上这篇教程。随便在下面评论吧！
