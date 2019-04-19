# nginx for Windows - =朝晖= - 博客园
# [nginx for Windows](https://www.cnblogs.com/dhcn/p/7105456.html)
zt from nginx official site.
[Known issues](http://nginx.org/en/docs/windows.html#known_issues)
[Possible future enhancements](http://nginx.org/en/docs/windows.html#possible_future_enhancements)
Version of nginx for Windows uses the native Win32 API (not the Cygwin emulation layer). Only the `select()` connection processing method is currently used, so high performance and scalability should not be expected. Due to this and some other known issues version of nginx for Windows is considered to be a *beta* version. At this time, it provides almost the same functionality as a UNIX version of nginx except for XSLT filter, image filter, GeoIP module, and embedded Perl language.
To install nginx/Windows, [download](http://nginx.org/en/download.html) the latest development version distribution (1.3.4), since the development branch of nginx contains all known fixes. Then unpack the distribution, [Go](http://lib.csdn.net/base/go) to the nginx-1.3.4 directory, and run `nginx`. Here is an example for the drive C: root directory:
> cd c:\
unzip nginx-1.3.4.zip
cd nginx-1.3.4
start nginx
Run the `tasklist` command-line utility to see nginx processes:
> C:\nginx-1.3.4>tasklist /fi "imagename eq nginx.exe"
Image Name           PID Session Name     Session#    Mem Usage
=============== ======== ============== ========== ============
nginx.exe            652 Console                 0      2 780 K
nginx.exe           1332 Console                 0      3 112 K
One of the processes is the master process and another is the worker process. If nginx does not start, look for the reason in the error log file`logs\error.log`. If the log file has not been created, the reason for this should be reported in the Windows Event Log. If an error page is displayed instead of the expected page, also look for the reason in the `logs\error.log` file.
nginx/Windows uses the directory where it has been run as the prefix for relative paths in the configuration. In the example above, the prefix is `C:\nginx-1.3.4\`. Paths in a configuration file must be specified in UNIX-style using forward slashes:
> access_log   logs/site.log;
root         C:/web/html;
nginx/Windows runs as a standard console application (not a service), and it can be managed using the following commands:
> |nginx -s stop|fast shutdown|
|----|----|
|nginx -s quit|graceful shutdown|
|nginx -s reload|changing configuration, starting new worker processes with a new configuration, graceful shutdown of old worker processes|
|nginx -s reopen|re-opening log files|
#### Known issues
- Although several workers can be started, only one of them actually does any work.
- A worker can handle no more than 1024 simultaneous connections.
- The cache and other modules which require shared memory support do not work on Windows Vista and later versions due to address space layout randomization being enabled in these Windows versions.
#### Possible future enhancements
- Running as a service.
- Using the I/O completion ports as a connection processing method.
- Using multiple worker threads inside a single worker process.

