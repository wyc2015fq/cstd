# Async 和 Await - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Wing](http://www.jobbole.com/members/wing011203) 翻译，[唐尤华](http://www.jobbole.com/members/tangyouhua) 校稿。未经许可，禁止转载！
英文出处：[Stephen Cleary](http://blog.stephencleary.com/2012/02/async-and-await.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
大部分人可能已经知道，Visual Studio 11引入了新的“async”和“await”。这是另外一篇介绍文章。
（译者注：本文写于 2012 年）
首先，点睛之笔：async 会从根本上改变我们大部分代码的编写方式。
是的，我相信async、await会带来比LINQ更大的影响。在很短时间以后，理解 async 会变成一个基本需求。
## 关键字介绍
让我们开始吧。我会使用一些在后面才会详细说明的内容——在第一部分中请坚持一下。
异步方法看上去就像这样：

C#
```
public async Task DoSomethingAsync()
{
  // 在真实世界里，我们会真的去执行一些操作。。。
  // 但对于这个示例，我们只是简单的（异步地）等待100毫秒。
  await Task.Delay(100);
}
```
“async”这个关键字让我们能够在方法内部使用“await”关键字，并且改变了处理结果的方式，*这就是async关键字做的全部内容！*我们并没有在线程池的线程中运行这个方法，也没有使用任何其它魔法。async关键字*只是*激活了await关键字（并管理方法结果）。
异步方法在开始执行时，和其它任何方法都是一样的。也就是说，在遇到“await”关键字（或者抛出异常）之前，方法都是同步运行的。
“await”关键字可以让事情异步运行。await就像一元操作符。它接收一个单独的参数：可等待（可等待是一个异步操作）。await会检查可等待操作是否已经结束，如果可等待操作已经完成，方法就会继续运行（就像一个正常的同步运行方法）。
如果“await”发现可等待操作还没有完成，那么就会异步地执行。它会告诉可执行操作，在完成之后~~，~~继续执行方法剩余的部分，然后从异步方法返回。
过些时候，当可执行操作完成后，它会执行异步方法的剩余部分。如果你在等待一个内置的可等待操作（例如Task），那么异步方法的剩余部分会在“await”返回之前的“上下文”中执行。
我喜欢将“await”当做“异步等待”。也就是说，异步*方法*会暂停，直到可等待操作结束（因此它在等待），但实际上线程并不会被阻塞（因此它是异步的）。
## 可等待
如我所说，“await”关键字使用一个单独的参数——“可等待的”，一个异步操作。在.NET框架中，我们有两个常用的可等待类型：Task<T> 和 Task。
我们也有一些其它的可等待类型：有一些特殊方法，例如”Task.Yield”会返回一个可等待的值，但这个值不是Task；对于WinRT运行时（随Windows 8而来）会有一个非托管的可等待类型。你也可以创建自己的可等待类型（通常是为了性能的原因），或者使用扩展方法将一个不可等待的类型变为可等待类型。
这就是我想说的，如何创建你自己的可等待类型。在使用async/await的全部时间里，我不得不创建了几个可等待类型。如果想了解更多关于创建自定义可等待类型的信息，你可以查看 [Parallel团队的博客](http://blogs.msdn.com/b/pfxteam/) 或者 [Jon Skeet的博客](http://codeblog.jonskeet.uk/)。
关于可等待类型，有一点很重要：可等待指的是类型，而不是返回该类型的方法。换句话说，你可以等待一个返回 Task 的方法的结果，*这是因为这个方法返回的是Task，而不是因为方法是异步的。*这样你也可以等待一个返回 Task 非异步方法的结果。

C#
```
public async Task NewStuffAsync()
{
  //使用await关键字，希望你能玩得开心。
  await ...
}
public Task MyOldTaskParallelLibraryCode()
{
  // 请注意这不是一个异步方法，因此我们不能够在这里使用await关键字。
  ...
}
public async Task ComposeAsync()
{
  // 我们可以等待任务，而无需关心它们来自何处。
  await NewStuffAsync();
  await MyOldTaskParallelLibraryCode();
}
```
小贴士：如果你有一个非常简单的异步方法，你可能根本不会使用 await 关键字（例如，使用Task.FromResult）。如果你可以不使用 await 关键字，那么你就不应该使用它，并将 async 关键字从方法声明中移除。一个返回 Task.FromResult 的非异步方法要比一个返回正常值的异步方法更有效率。
## 返回类型
异步方法可以返回Task<T>、Task或者void。在大多数情况下，你可能会希望返回 Task<T> 或者 Task，只有万不得已时，才会返回void。
为什么返回 Task<T> 或者 Task 呢？因为它们是可等待的，而void不是。这样当你有一个返回 Task<T> 或者 Task 的异步方法时，你就可以将结果传递给 await。如果使用void方法，你不能向await传递任何值。
当有异步的事件处理方法时，你只能返回void。
你也可以在其他“高级别”行为上使用异步void方法——例如，针对控制台程序的单独的“static async void MainAsync()”。然而，异步void方法有自身的问题，可以参考异步控制台程序。异步void方法的主要还是用于事件处理。
## 返回值
如果一个异步方法返回 Task 或者 void，那么它不会有返回值，如果一个异步方法返回 Task<T>，那么它必须返回一个类型为 T 的值：

C#
```
public async Task<int> CalculateAnswer()
{
  await Task.Delay(100); // (Probably should be longer...)
  // 返回一个int类型，而不是Task<int>。
  return 42;
}
```
这里有点儿奇怪，我们需要去适应。但这样设计的背后，还是有一些“[好的理由](http://blog.stephencleary.com/2011/09/async-ctp-why-do-keywords-work-that-way.html)”的。
## 上下文
在概述中~~，~~我提到过，如果你在等待一个内置的可等待操作，那么可等待操作会捕捉当前的“上下文”，并在后面执行异步方法剩余部分时，使用该“上下文”。那么“上下文”到底是什么？
简单的回答是：
- 如果你是在UI线程上，那么它就是UI上下文。
- 如果你是在响应一个ASP.NET的请求，那么它就是ASP.NET的请求上下文。
- 否则，通常它是一个线程池上下文。
复杂的回答：
- 如果 SynchronizationContext.Current 不为空，那么它是当前的 SynchronizationContext（UI上下文和ASP.NET请求上下文都是 SynchronizationContext 上下文）。
- 否则，它是当前的 TaskScheduler（TaskScheduler.Default是线程池上下文）。
那么在实际中这意味着什么？首先，捕捉（以及存储）UI、ASP.NET上下文是透明的。

C#
```
// WinForms示例（对于WPF来说是一样的）
private async void DownloadFileButton_Click(object sender, EventArgs e)
{
  // 既然我们使用异步的方式等待，那么UI线程就不会被文件下载所阻塞。
  await DownloadFileAsync(fileNameTextBox.Text);
  // 既然我们从UI上下文中恢复，我们就可以直接访问UI元素。
  resultTextBox.Text = "File downloaded!";
}
// ASP.NET示例
protected async void MyButton_Click(object sender, EventArgs e)
{
  // 既然我们使用异步方法来等待，那么ASP.NET线程就不会被文件下载所阻塞。
  // 这样当我们等待的时候，ASP.NET线程就可以处理其他的请求。
  await DownloadFileAsync(...);
  // 既然我们从ASP.NET上下文中恢复，我们就可以访问当前的请求。
  // 我们可能实际上是在另外一个*线程*上，但我们有相同的ASP.NET请求上下文。
  Response.Write("File downloaded!");
}
```
对于事件处理来说，这是很棒的，但对于你要写的其它大部分代码来说（实际上，这是你将会写的大部分异步代码），这可能并不是你想要的。
## 避免上下文
很多时候，你可能并不需要异步回到“主”上下文。对于大部分异步方法来说，在设计时会在头脑中考虑组合：它们会等待其它操作，每个操作本身都代表一个异步操作（这意味着，它们可以和其它操作组合在一起）。在这种情况下，你可以通过调用 ConfigureAwait 方法并传入false的方式，来告诉“等待者”不要捕捉当前的上下文。例如：

C#
```
private async Task DownloadFileAsync(string fileName)
{
  // 使用HttpClient或者其他任何方式来下载文件内容
  var fileContents = await DownloadFileContentsAsync(fileName).ConfigureAwait(false);
  // 请注意由于ConfigureAwait(false)，我们并没有在原来的上下文中。
  // 相反，我们运行在线程池上。
  // 将文件内容写入到外部的磁盘文件中。
  await WriteToDiskAsync(fileName, fileContents).ConfigureAwait(false);
  // 这里第二次调用ConfigureAwait(false)并不是*必需*的，但这是一个最佳实践。
}
// WinForms实例（对于WPF来说是一样的）
private async void DownloadFileButton_Click(object sender, EventArgs e)
{
  // 因为我们使用异步方式来等待，UI线程就不会被文件下载所阻塞。
  await DownloadFileAsync(fileNameTextBox.Text);
  // 因为我们从UI上下文中恢复了， 我们就可以直接访问UI元素了。
  resultTextBox.Text = "File downloaded!";
}
```
在这个示例中， 有一点非常重要：每个“级别”上的异步方法只会调用它自己的上下文。DownloadFileButton_Click 在 UI 上下文中启动，然后调用 DownloadFileAsync 方法。DownloadFileAsync 也在 UI 上下文中启动，但它通过调用 ConfigureAwait(false) 方法将上下文丢弃。DownloadFileAsync 方法剩余部分会运行在线程池上下文中。然而，当 DownloadFileAsync 方法结束后，DownloadButton_Click 方法会恢复执行，它*确实*会恢复 UI 上下文。
一种好的做法：如果你不知道你*确实*需要上下文，那么就用 ConfigureAwait(false) 方法。
## 异步组合
到目前为止，我们只考虑了连续组合：异步方法一次只会等待一个操作。我们也可以启动多个操作，并等待其中一个（或者全部）结束。为此，我们可以启动这些操作，但到后面再等待它们：

C#
```
public async Task DoOperationsConcurrentlyAsync()
{
  Task[] tasks = new Task[3];
  tasks[0] = DoOperation0Async();
  tasks[1] = DoOperation1Async();
  tasks[2] = DoOperation2Async();
  // 在这里，所有的三个任务会同时运行。
  // 现在我们等待所有的任务。
  await Task.WhenAll(tasks);
}
public async Task<int> GetFirstToRespondAsync()
{
  // 调用两个Web服务；然后读取第一个响应。
  Task<int>[] tasks = new[] { WebService1Async(), WebService2Async() };
  // 等待第一个Web服务响应。
  Task<int> firstTask = await Task.WhenAny(tasks);
  // 返回结果。
  return await firstTask;
}
```
通过使用并发组合（Task.WhenAll 或者 Task.WhenAny），你可以执行简单的并发操作。你也可以将这些方法和 Task.Run 一起使用~~，~~来执行简单的并行计算。然而，这种方式并不适用于任务并行库（Task 并行库）——对于任何CPU密集型的高级并行操作来说，都应该使用 TPL 完成。
## 使用指南
请查看“[基于任务的异步模式（TAP）文档](http://www.microsoft.com/download/en/details.aspx?id=19957)”。这份文档写得非常好，它包含了API设计方面的指导原则，以及如何正确使用async、await（包括取消以及进度报告）。
现在有很多适用await的新技术，我们应该使用它们，而不是那些旧的技术。如果你还在新的异步代码中使用任何下面列出的旧示例，那么你就做错了：
|旧技术|新技术|描述|
|----|----|----|
|task.Wait|await task|Wait/await for a task to complete等待一个任务结束|
|task.Result|await task|Get the result of a completed task获取已结束任务的结果|
|Task.WaitAny|await Task.WhenAny|Wait/await for one of a collection of tasks to complete等待任务集中的任何一个结束|
|Task.WaitAll|await Task.WhenAll|Wait/await for every one of a collection of tasks to complete等待任务集中的全部任务都结束|
|Thread.Sleep|await Task.Delay|Wait/await for a period of time等待一段时间|
|Task constructor|Task.Run or TaskFactory.StartNew|Create a code-based task创建一个基于代码的任务|
## 下一步
我已经在MSDN中发表了一篇文章：[异步编程的最佳实践](http://msdn.microsoft.com/en-us/magazine/jj991977.aspx)，这篇文章进一步解释了“避免使用异步void”、“自始至终使用异步”以及“配置上下文”的指导原则。
[MSDN 官方文档](http://msdn.microsoft.com/en-us/library/hh191443.aspx)非常不错，它包含了“[基于任务的异步模式文档](http://msdn.microsoft.com/en-us/library/hh873175.aspx)”的在线版本，这个文档非常好，它讨论了异步方法的设计。
异步团队发表了“[async/await FAQ](http://blogs.msdn.com/b/pfxteam/archive/2012/04/12/10293335.aspx)“，我们可以很好地通过它来继续学习异步。在这里面还包含了最好的博客文章和视频的链接。另外，[Stephen Toub](http://blogs.msdn.com/b/pfxteam)写的任何文章都非常有启发性。
当然，另外一个资源就是我自己的博客。
我在“[Concurrency Cookbook](http://stephencleary.com/book/)”中包含了大量关于使用 async 和 await 的用例，同时也包含了一些你应该使用任务并行库（TPL）、Rx或者TPL数据流的情况。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/0aa7ead79d59232a0029c904275b9b2b.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/038c2db938b7abfa515a6bfef71d6592.jpg)
