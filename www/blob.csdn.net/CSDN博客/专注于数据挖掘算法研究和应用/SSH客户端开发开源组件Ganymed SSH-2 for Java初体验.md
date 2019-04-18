# SSH客户端开发开源组件Ganymed SSH-2 for Java初体验 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月26日 14:25:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2617








1、Ganymed SSH-2 for Java

Ganymed SSH-2 for Java是一个纯Java实现的SHH2库。


看官方介绍：

Ganymed SSH-2 for Java is a library which implements the SSH-2 protocol in pure Java (tested on J2SE 1.4.2 and 5.0). It allows
 one to connect to SSH servers from within Java programs. It supports SSH sessions (remote command execution and shell access), local and remote port forwarding, local stream forwarding, X11 forwarding, SCP and SFTP. There are no dependencies on any JCE provider,
 as all crypto functionality is included.




2、开源组件下载地址：http://www.ganymed.ethz.ch/ssh2/

      解压后如下：

![](https://img-blog.csdn.net/20170526142205865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

ganymed-ssh2-build210.jar引入工程包，javadoc是学习文档，examples是开发案例。





3、开发案例Basic代码参考：



```java

```
package com.ssh;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import ch.ethz.ssh2.Connection;
import ch.ethz.ssh2.Session;
import ch.ethz.ssh2.StreamGobbler;

public class Basic
{
	public static void main(String[] args)
	{
		String hostname = "172.0.0.1";
		String username = "hdfs";
		String password = "hdfs";

		try
		{
			/* Create a connection instance */

			Connection conn = new Connection(hostname);

			/* Now connect */

			conn.connect();

			/* Authenticate.
			 * If you get an IOException saying something like
			 * "Authentication method password not supported by the server at this stage."
			 * then please check the FAQ.
			 */

			boolean isAuthenticated = conn.authenticateWithPassword(username, password);

			if (isAuthenticated == false)
				throw new IOException("Authentication failed.");

			/* Create a session */

			Session sess = conn.openSession();

			sess.execCommand("uname -a && date && uptime && who");

			System.out.println("Here is some information about the remote host:");

			/* 
			 * This basic example does not handle stderr, which is sometimes dangerous
			 * (please read the FAQ).
			 */

			InputStream stdout = new StreamGobbler(sess.getStdout());

			BufferedReader br = new BufferedReader(new InputStreamReader(stdout));

			while (true)
			{
				String line = br.readLine();
				if (line == null)
					break;
				System.out.println(line);
			}

			/* Show exit status, if available (otherwise "null") */

			System.out.println("ExitCode: " + sess.getExitStatus());

			/* Close this session */

			sess.close();

			/* Close the connection */

			conn.close();

		}
		catch (IOException e)
		{
			e.printStackTrace(System.err);
			System.exit(2);
		}
	}
}





