# AsyncTask来源分析(一) - weixin_33985507的博客 - CSDN博客
2015年09月30日 12:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
于android开发过程AsyncTask我会经常处理它。在网上，也有很多的其描述，因此，这里是不是说的用法，直接写自己的学习经验，以及它的一些浅显的认识，忘记错批评。
AsyncTask它是一个抽象类。它提供了一个抽象方法doInBackground为了提供子类实现，用者自己来实现一些比較耗时的工作。
在AsyncTask的生命周期中任务分为三个状态，由变量mStatus来控制,mStatus为枚举类型,枚举类的名称为Status：
PENDING状态:标志这个任务还没有被运行过，该状态在定义状态变量mStatus的时候就被初始化了。private volatile Status mStatus = Status.PENDING;
RUNNING状态:标志这个任务已经被运行 ,当你调用execute运行任务的execute的时候会设置mStatus = RUNNING;
FINISHED状态：在运行finish(Result result)方法的时候会设置mStatus = FINISHED。至于finish()方法的运行时机是任务运行完以后有handler发送一条相关信息，然后在handleMessage里面调用。其实onPostExecute就是在finish里面被调用的。
这三个状态的作用是防止一个AsyncTask的对象多次调用execute方法（其实AsyncTask的一个对象仅仅能被运行一次就是通过Status枚举来控制的），比方当前的AsynTask对象的mStatus为RUNNING或者FINISHED的时候你再调用execute方法的时候就会抛出异常了。看看execute的代码就能够知道：
```java
/**
     * 运行任务的入口方法
     * @param params
     * @return
     */
    public final AsyncTask<Params, Progress, Result> execute(Params... params) {
        return executeOnExecutor(sDefaultExecutor, params);
    }
    public final AsyncTask<Params, Progress, Result> executeOnExecutor(
            Executor exec, Params... params) {
        //一个任务运行被运行一次的限制就是在这里被体现的
        if (mStatus != Status.PENDING) {
            switch (mStatus) {
            case RUNNING:
                throw new IllegalStateException("Cannot execute task:"
                        + " the task is already running.");
            case FINISHED:
                throw new IllegalStateException("Cannot execute task:"
                        + " the task has already been executed "
                        + "(a task can be executed only once)");
            }
        }
        //标志任务正在运行。假设再次调用该对象的execute方法的话。会走517行代码逻辑
        mStatus = Status.RUNNING;
        //任务开启前运行的方法，此时仍然在UI线程中
        onPreExecute();
        //保存參数
        mWorker.mParams = params;
        //运行任务
        exec.execute(mFuture);
        return this;
    }
```
看executeOnExecutor方法，能够知道当第一次调用execute方法的时候executeOnExecutor里面的if条件是不成立的。当再次调用execute方法的时候if条件成立，然后就会抛出异常。
通过观察这种方法，能够发现非常多的信息，比方onPreExecute()方法就是先于其它方法运行，此时运行onPreExecute方法的时候仍在在UI线程中，所以我们能够重写这种方法以实如今运行任务之前进行其它处理的问题：比方显示载入滚动栏等。接着会运行exec.exeute(mFuture)方法来在UI线程上开启一个新的线程来运行耗时的任务：也就是运行你重写的doInBackground()方法。我们知道AsyncTask的doInBackgound方法是有返回值的，这个返回值能够传给onPreExecute(Reusult
 result)方法，然后做详细的处理。也就是我们会从线程中获取一个返回值。
在细致分析之前先说几个多线程编程的几个相关的类：
Callable：为接口提供了一个call()方法，实现该接口的类可作为被其他线程运行的任务，这点和Runnable一样。
与Runnable的run方法不同的是call()方法是有返回值的（Callable就相当于一个有返回结果的Runnable)，所以Callable能够用来运行那些存在延迟计算的任务。鉴于AsyncTask的目的有时候就是用来运行耗时的任务而且用该任务的运行结果来改动UI,所以能够想象的出AsyncTask里面肯定会有Callable的身影。在开发中使用AsyncTask的时候肯定会重写doInbackgound()方法。把耗时的任务放在里面运行，其实doInBackground方法就是在一个Callable的call方法里面运行的，而且利用到了Callable能够有返回值得这一特性来把doInBackground的返回值返回并由FutureTask来获取到这一个返回结果，然后经过一系列的辗转终于交给onPostExecute方法来处理。
一句废话：Callable能够返回计算结果，这个结果并不仅仅是一个数字。事实上结果能够是一个基本类型，也能够是返回一个对象当然也能够返回一个集合。
FutureTask:表示一种抽象的可生成结果的计算，它表示的计算结果是通过Callable来实现的。FutureTask将计算结果从运行计算的线程传递到获取这个运算结果的线程（摘自Java并发编程实践).能够通过FutureTask的get()方法来获取任务的计算结果.当中get()并非马上就能够得到运算结果：假设任务已经完毕运算的话。就会马上返回结果。否则get方法将会被堵塞直到任务完毕后返回运算结果（当然也可能产生异常).通过查看它的源代码能够发现FutureTask提供了一个空的方法done(),该方法在任务线程运行完的时候调用。所以程序猿自己能够写一个FutureTask的子类来重写这种方法来处理任务线程运行完后的逻辑。
当然这个在AsyncTask的源代码中得到了体现。注意FutureTask在进入完毕状态之后，该FutureTask会永远停止在这个状态上（不会被再次运行）；通过观察AsyncTask的源代码能够知道，代码中的FutureTask仅仅有在构造器中初始化了一次，源代码中并没有给该类提供了再次初始化FutureTask的方法所以这也是为什么AsyncTask仅仅会被运行一次的原因。同一时候，FutureTask另一个cancel(boolean)的方法能够用来取消正在运行的任务。
实际上AsyncTask的取消任务的方法就是简单的调用FutureTask的cancel方法来完毕的。总结：一般FutureTask用来运行比較耗时的计算。在任务运行的过程中也能够调用cancel方法来强制取消任务的运行。同一时候它能够让程序猿知道任务线程运行完的时机并能够获取任务线程返回的结果。然后用这个结果在主线程中做一些处理。比方在AsyncTask的onPostExecte方法中就能够用返回的结果来改动UI线程的界面。
通过上面的说明以及AsyncTask的源代码能够总结出一下几点：
1.无论你重写不重写onPreExecute()方法，这种方法必然先运行。
2.doInBackground方法交给一个Callable的call方法运行，而且通过FutureTask的get方法获取
3.当doInBackground方法执行完的时候，FutureTask的done方法中会通过Handler发送一条消息，该消息绑定了doInBackground执行的结果。然后在
handleMessage方法中进行处理，也就是交给了onPostExecute方法进行处理
带着上面的几点，先看看Callable和FutureTask的初始化的位置以及详细都它们是怎么处理doInBackground()的，源代码例如以下：
```java
public AsyncTask() {
        初始化mWorker
        mWorker = new WorkerRunnable<Params, Result>() {
            public Result call() throws Exception {
               //设置控制变量为true
                mTaskInvoked.set(true);
                Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
                //noinspection unchecked
                //doInBackground方法再此运行
                return postResult(doInBackground(mParams));
            }
        };
        mFuture = new FutureTask<Result>(mWorker) {
            @Override
            protected void done() {
                try {
                    postResultIfNotInvoked(get());
                } catch (InterruptedException e) {
                    android.util.Log.w(LOG_TAG, e);
                } catch (ExecutionException e) {
                    throw new RuntimeException("An error occured while executing doInBackground()",
                            e.getCause());
                } catch (CancellationException e) {
                    postResultIfNotInvoked(null);
                }
            }
        };
    }
```
以上代码简单说明：
WorkerRunnable是AsyncTask的抽象的嵌套内部类，它实现了Callable接口。
在call()方法中会运行postResult()方法。该方法的參数正是doInBackground()
执行过后的返回值(注意：这儿正是doInBackground的执行的地方).看看那postResult()方法的都干了些什么
```java
private Result postResult(Result result) {
        @SuppressWarnings("unchecked")
        Message message = sHandler.obtainMessage(MESSAGE_POST_RESULT,
                new AsyncTaskResult<Result>(this, result));
        message.sendToTarget();
        return result;
 }
```
 能够看到它发送了一条message,那么就看看该message在Handler的handleMessage方法里面是怎么处理的。
```java
private static class InternalHandler extends Handler {
        @SuppressWarnings({"unchecked", "RawUseOfParameterizedType"})
        @Override
        public void handleMessage(Message msg) {
            AsyncTaskResult result = (AsyncTaskResult) msg.obj;
            switch (msg.what) {
                case MESSAGE_POST_RESULT:
                    // There is only one result
                    //设置任务为结束
                    result.mTask.finish(result.mData[0]);
                    break;
                case MESSAGE_POST_PROGRESS:
                    result.mTask.onProgressUpdate(result.mData);
                    break;
            }
        }
    }
```
 result.mTask.finish(result.mData[0]);调用了AsyncTask的finish方法。就如前面所说finish方法体里面正好调用了onPostExecute方法，finish方法例如以下
```java
private void finish(Result result) {
        if (isCancelled()) {
            onCancelled(result);
        } else {
            //onPostExecute在此处调用
            onPostExecute(result);
        }
        //设置任务的状态为FINISHED
        mStatus = Status.FINISHED;
    }
```
至此一个AsyncTask的异步任务就会运行完成。还有个FutureTask的done方法没说明。done()方法调用了postResultIfNotInvoked(get()),其方法例如以下
```java
private void postResultIfNotInvoked(Result result) {
        final boolean wasTaskInvoked = mTaskInvoked.get();
        if (!wasTaskInvoked) {
            postResult(result);
        }
    }
```
能够看出当if条件成立的时候仍然会运行postResult方法。
查看FutureTask的源代码可知在调用cancel()方法取消任务的时候会运行这个done()方法，在发生异常的时候相它会使用这种方法运行
待续，如果错了，欢迎批评
版权声明：本文博主原创文章，博客，未经同意不得转载。
