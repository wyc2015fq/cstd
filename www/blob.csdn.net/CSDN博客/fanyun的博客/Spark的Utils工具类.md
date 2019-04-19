# Spark的Utils工具类 - fanyun的博客 - CSDN博客
2018年03月18日 21:34:27[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：397
所属专栏：[大数据](https://blog.csdn.net/column/details/bigdata01.html)
         Apache Spark 是专为大规模数据处理而设计的快速通用的计算引擎。Spark能更好地适用于数据挖掘与机器学习等需要迭代的MapReduce的算法。
        Spark 是一种与 Hadoop 相似的开源集群计算环境，但是两者之间还存在一些不同之处，这些有用的不同之处使 Spark 在某些工作负载方面表现得更加优越，换句话说，Spark 启用了内存分布数据集，除了能够提供交互式查询外，它还可以优化迭代工作负载。
       Spark 是在 Scala 语言中实现的，它将 Scala 用作其应用程序框架。与 Hadoop 不同，Spark 和 Scala 能够紧密集成，其中的 Scala 可以像操作本地集合对象一样轻松地操作分布式数据集。
尽管创建 Spark 是为了支持分布式数据集上的迭代作业，但是实际上它是对 Hadoop 的补充，可以在 Hadoop 文件系统中并行运行。通过名为 Mesos 的第三方集群框架可以支持此行为。可用来构建大型的、低延迟的数据分析应用程序。    
      Utils是Spark中最常用的工具类之一，如果不关心其实现，也不会对理解Spark有太多影响。但是对于Scala或者Spark的初学者来说，通过了解Utils工具类的实现，也是个不错的入门途径。下面将逐个介绍Utils工具类提供的常用方法。
### 1.localHostName
功能描述：获取本地机器名。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def localHostName(): String = {  
-     customHostname.getOrElse(localIpAddressHostname)  
-   }  
### 2.getDefaultPropertiesFile
功能描述：获取默认的Spark属性文件。
> 
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def getDefaultPropertiesFile(env: Map[String, String] = sys.env): String = {  
-   env.get("SPARK_CONF_DIR")  
-     .orElse(env.get("SPARK_HOME").map{ t => s"$t${File.separator}conf"})  
-     .map { t => new File(s"$t${File.separator}spark-defaults.conf")}  
-     .filter(_.isFile)  
-     .map(_.getAbsolutePath)  
-     .orNull  
- }  
### 3.loadDefaultSparkProperties
功能描述：加载指定文件中的Spark属性，如果没有指定文件，则加载默认Spark属性文件的属性。
> 
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def loadDefaultSparkProperties(conf:SparkConf, filePath: String = null):String = {  
-   val path =Option(filePath).getOrElse(getDefaultPropertiesFile())  
-   Option(path).foreach { confFile =>  
-     getPropertiesFromFile(confFile).filter{ case (k,v) =>  
-       k.startsWith("spark.")  
-     }.foreach { case (k, v) =>  
-      conf.setIfMissing(k, v)  
-      sys.props.getOrElseUpdate(k, v)  
-     }  
-   }  
-   path  
- }  
### 4.getCallSite
功能描述：获取当前SparkContext的当前调用堆栈，将栈里最靠近栈底的属于spark或者Scala核心的类压入callStack的栈顶，并将此类的方法存入lastSparkMethod；将栈里最靠近栈顶的用户类放入callStack，将此类的行号存入firstUserLine，类名存入firstUserFile，最终返回的样例类CallSite存储了最短栈和长度默认为20的最长栈的样例类。在JavaWordCount例子中，获得的数据如下：
- 最短栈：JavaSparkContext at JavaWordCount.java:44；
- 最长栈：org.apache.spark.api.java.JavaSparkContext.<init>(JavaSparkContext.scala:61)org.apache.spark.examples.JavaWordCount.main(JavaWordCount.java:44)。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def getCallSite(skipClass: String => Boolean =coreExclusionFunction): CallSite = {  
-     val trace =Thread.currentThread.getStackTrace().filterNot { ste: StackTraceElement=>  
-       ste == null || ste.getMethodName == null || ste.getMethodName.contains("getStackTrace")  
-     }  
-     var lastSparkMethod= "<unknown>"
-     var firstUserFile= "<unknown>"
-     var firstUserLine= 0
-     var insideSpark= true
-     var callStack= new ArrayBuffer[String]() :+ "<unknown>"
- 
- for (el<- trace) {  
- if (insideSpark){  
- if (skipClass(el.getClassName)){  
-           lastSparkMethod = if(el.getMethodName == "<init>") {  
-             el.getClassName.substring(el.getClassName.lastIndexOf('.') + 1)  
-           } else {  
-             el.getMethodName  
-           }  
-           callStack(0) = el.toString // Putlast Spark method on top of the stack trace.
-         } else {  
-           firstUserLine = el.getLineNumber  
-           firstUserFile = el.getFileName  
-           callStack += el.toString  
-           insideSpark = false
-         }  
-       } else {  
-         callStack += el.toString  
-       }  
-     }  
-     val callStackDepth= System.getProperty("spark.callstack.depth","20").toInt  
-     CallSite(  
-       shortForm = s"$lastSparkMethod at $firstUserFile:$firstUserLine",  
-       longForm = callStack.take(callStackDepth).mkString("\n"))  
-   }  
### 5.startServiceOnPort
功能描述：Scala跟其它脚本语言一样，函数也可以传递，此方法正是通过回调startService这个函数来启动服务，并最终返回startService返回的service地址及端口。如果启动过程有异常，还会多次重试，直到达到maxRetries表示的最大次数。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def startServiceOnPort[T](  
-      startPort:Int,  
-      startService:Int => (T, Int),  
-      conf:SparkConf,  
-      serviceName:String = ""): (T, Int) = {  
-   require(startPort == 0 || (1024 <= startPort && startPort < 65536),  
- "startPort should be between 1024 and 65535(inclusive), or 0 for a random free port.")  
-    val serviceString= if (serviceName.isEmpty) "" elses" '$serviceName'"
-    val maxRetries= portMaxRetries(conf)  
- for (offset<- 0 to maxRetries){  
-      val tryPort= if (startPort == 0) {  
-        startPort  
-      } else {  
-        ((startPort+ offset - 1024)% (65536 - 1024))+ 1024
-      }  
- try {  
-        val (service,port) = startService(tryPort)  
-        logInfo(s"Successfullystarted service$serviceString on port $port.")  
- return (service,port)  
-      } catch {  
- case e:Exception if isBindCollision(e) =>  
- if (offset>= maxRetries) {  
-            val exceptionMessage=  
-              s"${e.getMessage}:Service$serviceString failed after $maxRetries retries!"
-            val exception= new BindException(exceptionMessage)  
-            exception.setStackTrace(e.getStackTrace)  
- throw exception  
-          }  
-         logWarning(s"Service$serviceString couldnot bind on port $tryPort. " +  
-            s"Attempting port ${tryPort+ 1}.")  
-      }  
-    }  
- throw newSparkException(s"Failed to start service$serviceString on port$startPort")  
-  }  
### 6.createDirectory
功能描述：用spark+UUID的方式创建临时文件目录，如果创建失败会多次重试，最多重试10次。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def createDirectory(root: String, namePrefix: String = "spark"): File = {  
-   var attempts = 0
-   val maxAttempts = MAX_DIR_CREATION_ATTEMPTS  
-   var dir: File = null
- while (dir == null){  
-     attempts += 1
- if (attempts > maxAttempts) {  
- throw newIOException("Failed to create a temp directory(under " + root + ") after "+  
-        maxAttempts + " attempts!")  
-     }  
- try {  
-       dir = new File(root, "spark-"+ UUID.randomUUID.toString)  
- if (dir.exists() || !dir.mkdirs()) {  
-         dir = null
-       }  
-     } catch { casee: SecurityException => dir = null;}  
-   }  
- 
-   dir  
- }  
### 7.getOrCreateLocalRootDirs
功能描述：根据spark.local.dir的配置，作为本地文件的根目录，在创建一、二级目录之前要确保根目录是存在的。然后调用createDirectory创建一级目录。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- private[spark] defgetOrCreateLocalRootDirs(conf: SparkConf): Array[String] = {  
- if (isRunningInYarnContainer(conf)) {  
-    getYarnLocalDirs(conf).split(",")  
-   } else {  
-    Option(conf.getenv("SPARK_LOCAL_DIRS"))  
-      .getOrElse(conf.get("spark.local.dir",System.getProperty("java.io.tmpdir")))  
-       .split(",")  
-       .flatMap {root =>  
- try {  
-           val rootDir = newFile(root)  
- if (rootDir.exists || rootDir.mkdirs()) {  
-             val dir = createDirectory(root)  
-            chmod700(dir)  
-            Some(dir.getAbsolutePath)  
-           } else {  
-            logError(s"Failed to create dir in $root. Ignoring this directory.")  
-             None  
-           }  
-         } catch {  
- case e: IOException =>  
-          logError(s"Failed to create local rootdir in $root. Ignoring this directory.")  
-           None  
-         }  
-       }  
-       .toArray  
-   }  
- }  
### 8.getLocalDir
功能描述：查询Spark本地文件的一级目录。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def getLocalDir(conf: SparkConf): String = {  
-  getOrCreateLocalRootDirs(conf)(0)  
- }  
### 9.createTempDir
功能描述：在Spark一级目录下创建临时目录，并将目录注册到shutdownDeletePaths：scala.collection.mutable.HashSet[String]中。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def createTempDir(  
-     root: String= System.getProperty("java.io.tmpdir"),  
-     namePrefix:String = "spark"): File = {  
-   val dir =createDirectory(root, namePrefix)  
-  registerShutdownDeleteDir(dir)  
-   dir  
- }  
### 10.RegisterShutdownDeleteDir
功能描述：将目录注册到shutdownDeletePaths：scala.collection.mutable.HashSet[String]中，以便在进程退出时删除。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def registerShutdownDeleteDir(file: File) {  
-   val absolutePath =file.getAbsolutePath()  
-   shutdownDeletePaths.synchronized{  
-     shutdownDeletePaths += absolutePath  
-   }  
- }  
### 11.hasRootAsShutdownDeleteDir
功能描述：判断文件是否匹配关闭时要删除的文件及目录，shutdownDeletePaths：scala.collection.mutable.HashSet[String]存储在进程关闭时要删除的文件及目录。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def hasRootAsShutdownDeleteDir(file: File): Boolean = {  
-   val absolutePath= file.getAbsolutePath()  
-   val retval= shutdownDeletePaths.synchronized {  
-     shutdownDeletePaths.exists { path =>  
-       !absolutePath.equals(path) && absolutePath.startsWith(path)  
-     }  
-   }  
- if (retval){  
-     logInfo("path = " + file + ", already present as root for deletion.")  
-   }  
-   retval  
- }  
### 12.deleteRecursively
功能描述：用于删除文件或者删除目录及其子目录、子文件，并且从shutdownDeletePaths：scala.collection.mutable.HashSet[String]中移除此文件或目录。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def deleteRecursively(file: File) {  
- if (file != null){  
- try {  
- if (file.isDirectory &&!isSymlink(file)) {  
-         var savedIOException:IOException = null
- for (child<- listFilesSafely(file)) {  
- try {  
-            deleteRecursively(child)  
-           } catch {  
- case ioe:IOException => savedIOException = ioe  
-           }  
-         }  
- if (savedIOException!= null) {  
- throw savedIOException  
-         }  
-         shutdownDeletePaths.synchronized {  
-           shutdownDeletePaths.remove(file.getAbsolutePath)  
-         }  
-       }  
-     } finally {  
- if (!file.delete()) {  
- if (file.exists()) {  
- throw newIOException("Failed to delete: " +file.getAbsolutePath)  
-         }  
-       }  
-     }  
-   }  
- }  
### 13.getSparkClassLoader
功能描述：获取加载当前class的ClassLoader。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def getSparkClassLoader = getClass.getClassLoader  
### 14.getContextOrSparkClassLoader
功能描述：用于获取线程上下文的ClassLoader，没有设置时获取加载Spark的ClassLoader。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def getContextOrSparkClassLoader =  
-   Option(Thread.currentThread().getContextClassLoader).getOrElse(getSparkClassLoader)  
### 15.newDaemonCachedThreadPool
功能描述：使用Executors.newCachedThreadPool创建的缓存线程池。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def newDaemonCachedThreadPool(prefix: String): ThreadPoolExecutor = {  
-   val threadFactory =namedThreadFactory(prefix)  
-  Executors.newCachedThreadPool(threadFactory).asInstanceOf[ThreadPoolExecutor]  
- }  
### 16.doFetchFile
功能描述：使用URLConnection通过http协议下载文件。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- private defdoFetchFile(url: String, targetDir: File, filename: String, conf: SparkConf,  
-     securityMgr:SecurityManager, hadoopConf: Configuration) {  
-   val tempFile= File.createTempFile("fetchFileTemp",null, newFile(targetDir.getAbsolutePath))  
-   val targetFile= new File(targetDir, filename)  
-   val uri = new URI(url)  
-   val fileOverwrite= conf.getBoolean("spark.files.overwrite",defaultValue = false)  
-   Option(uri.getScheme).getOrElse("file")match {  
- case"http"| "https" | "ftp" =>  
-       logInfo("Fetching " + url + " to " + tempFile)  
-       var uc:URLConnection = null
- if (securityMgr.isAuthenticationEnabled()) {  
-         logDebug("fetchFile with security enabled")  
-         val newuri= constructURIForAuthentication(uri,securityMgr)  
-         uc = newuri.toURL().openConnection()  
-         uc.setAllowUserInteraction(false)  
-       } else {  
-         logDebug("fetchFile not using security")  
-         uc = newURL(url).openConnection()  
-       }  
-       valtimeout = conf.getInt("spark.files.fetchTimeout", 60) * 1000
-       uc.setConnectTimeout(timeout)  
-       uc.setReadTimeout(timeout)  
-       uc.connect()  
-       val in = uc.getInputStream()  
-      downloadFile(url, in, tempFile, targetFile,fileOverwrite)  
- case"file"=>  
-       val sourceFile= if (uri.isAbsolute)new File(uri)else newFile(url)  
-      copyFile(url, sourceFile, targetFile, fileOverwrite)  
- case _ =>  
-       val fs =getHadoopFileSystem(uri, hadoopConf)  
-       val in = fs.open(newPath(uri))  
-      downloadFile(url, in, tempFile, targetFile,fileOverwrite)  
-   }  
- }  
### 17.fetchFile
功能描述：如果文件在本地有缓存，则从本地获取，否则通过HTTP远程下载。最后对.tar、.tar.gz等格式的文件解压缩后，调用shell命令行的chmod命令给文件增加a+x的权限。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def fetchFile(  
-     url: String,  
-     targetDir:File,  
-     conf:SparkConf,  
-     securityMgr:SecurityManager,  
-     hadoopConf:Configuration,  
-     timestamp:Long,  
-     useCache:Boolean) {  
-   val fileName= url.split("/").last  
-   val targetFile= new File(targetDir, fileName)  
-   val fetchCacheEnabled= conf.getBoolean("spark.files.useFetchCache",defaultValue = true)  
- if (useCache && fetchCacheEnabled) {  
-     val cachedFileName= s"${url.hashCode}${timestamp}_cache"
-     val lockFileName= s"${url.hashCode}${timestamp}_lock"
-     val localDir= new File(getLocalDir(conf))  
-     val lockFile= new File(localDir,lockFileName)  
-     val raf = new RandomAccessFile(lockFile,"rw")  
-     val lock = raf.getChannel().lock()  
-     val cachedFile= new File(localDir,cachedFileName)  
- try {  
- if (!cachedFile.exists()){  
-        doFetchFile(url, localDir, cachedFileName, conf, securityMgr, hadoopConf)  
-       }  
-     } finally {  
-       lock.release()  
-     }  
-     copyFile(  
-       url,  
-       cachedFile,  
-       targetFile,  
-      conf.getBoolean("spark.files.overwrite",false)  
-     )  
-   } else {  
-    doFetchFile(url, targetDir, fileName,conf, securityMgr, hadoopConf)  
-   }  
- if (fileName.endsWith(".tar.gz") || fileName.endsWith(".tgz")) {  
-     logInfo("Untarring " + fileName)  
-    Utils.execute(Seq("tar", "-xzf", fileName),targetDir)  
-   } elseif(fileName.endsWith(".tar")){  
-     logInfo("Untarring " + fileName)  
-    Utils.execute(Seq("tar", "-xf", fileName),targetDir)  
-   }  
-   FileUtil.chmod(targetFile.getAbsolutePath, "a+x")  
- }  
### 18.executeAndGetOutput
功能描述：执行一条command命令，并且获取它的输出。调用stdoutThread的join方法，让当前线程等待stdoutThread执行完成。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def executeAndGetOutput(  
-     command:Seq[String],  
-     workingDir:File = new File("."),  
-    extraEnvironment: Map[String, String] = Map.empty): String = {  
-   val builder= new ProcessBuilder(command: _*)  
-      .directory(workingDir)  
-   val environment= builder.environment()  
- for ((key, value) <- extraEnvironment) {  
-     environment.put(key,value)  
-   }  
-   val process= builder.start()  
- new Thread("readstderr for " + command(0)) {  
-     override defrun() {  
- for (line<- Source.fromInputStream(process.getErrorStream).getLines()){  
-         System.err.println(line)  
-       }  
-     }  
-   }.start()  
-   val output= new StringBuffer  
-   val stdoutThread= new Thread("readstdout for " + command(0)) {  
-     override defrun() {  
- for (line<- Source.fromInputStream(process.getInputStream).getLines()){  
-         output.append(line)  
-       }  
-     }  
-   }  
-   stdoutThread.start()  
-   val exitCode= process.waitFor()  
-   stdoutThread.join()   // Wait for itto finish reading output
- if (exitCode!= 0) {  
-     logError(s"Process $commandexited with code $exitCode: $output")  
- throw newSparkException(s"Process $command exited with code $exitCode")  
-   }  
-   output.toString  
- }  
### 19.memoryStringToMb
功能描述：将内存大小字符串转换为以MB为单位的整型值。
**[java]**[view plain](http://blog.csdn.net/beliefer/article/details/50904662#)[copy](http://blog.csdn.net/beliefer/article/details/50904662#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612072/fork)
- def memoryStringToMb(str: String): Int = {  
-   val lower =str.toLowerCase  
- if (lower.endsWith("k")) {  
-     (lower.substring(0,lower.length-1).toLong/ 1024).toInt  
-   } elseif(lower.endsWith("m")){  
-     lower.substring(0,lower.length-1).toInt  
-   } elseif(lower.endsWith("g")){  
-     lower.substring(0,lower.length-1).toInt* 1024
-   } elseif(lower.endsWith("t")){  
-     lower.substring(0,lower.length-1).toInt* 1024 * 1024
-   } else {// nosuffix, so it's just a number in bytes
-     (lower.toLong / 1024/ 1024).toInt  
-   }  
- }  
- 
