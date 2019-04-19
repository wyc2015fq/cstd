# Laravel错误与日志处理 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月02日 16:22:24[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：543
App\Exceptions\Handler class
 is where all exceptions triggered by your application are logged and then rendered back to the user. We'll dive deeper
 into this class throughout this documentation.
For logging, Laravel utilizes the [Monolog](https://github.com/Seldaek/monolog) library,
 which provides support for a variety of powerful log handlers. Laravel configures several of these handlers for you, allowing you to
choose between a single log file, rotating log files, or writing error information to the system log.
Error Detail
The debug option
 in your config/app.php configuration
 file determines how much information about an error is actually displayed to the user. By default, this option is set to respect the value of the APP_DEBUG environment
 variable, which is stored in your .env file.
For local development, you should set the APP_DEBUG environment
 variable to true. In your production environment,
this value should always be false.
 If the value is set to true in production, you risk exposing sensitive configuration
 values to your application's end users.

Laravel supports writing log information to single files, daily files,
 the syslog, and the errorlog.
 To configure which storage mechanism Laravel uses, you should modify the logoption in
 your config/app.php configuration
 file. For example, if you wish to use daily log files instead of a single file, you should set the log value
 in your app configuration file to daily:
'log'=>'daily'
**Maximum Daily Log Files**
When using the daily log
 mode, Laravel will only retain five days of log files by default. If you want to adjust the number
 of retained files, you may add a log_max_files configuration value to yourapp configuration
 file:
'log_max_files'=>30
Severity Levels
When using Monolog, log messages may have different levels of severity. By default, Laravel
 writes all log levels to storage. However, in your production environment, you may wish to configure the minimum severity that should be logged by adding the log_level option
 to your app.phpconfiguration
 file.
Once this option has been configured, Laravel will log all levels
greater than or equal to
the specified severity. For example, a default log_level of error will
 log **error**, **critical**, **alert**,
 and **emergency**messages:
'log_level'=>env('APP_LOG_LEVEL','error'),
Monolog recognizes the following severity levels -
from least severe to most severe:debug, info, notice, warning, error, critical, alert, emergency.

[Exception Handler](https://laravel.com/docs/5.3/errors#the-exception-handler)
All exceptions are handled by the App\Exceptions\Handlerclass.
 This class contains two methods:report and render.
 We'll examine each of these methods in detail. The report method is used to log exceptions
 or send them to an external service like [Bugsnag](https://bugsnag.com/) or [Sentry](https://github.com/getsentry/sentry-laravel).
 By default, the reportmethod simply passes the exception to the base class where the
 exception is logged. However, you are free to log exceptions however you wish.
**Ignoring Exceptions By Type想要忽略的**
The $dontReportproperty
of the exception handler contains an array of exception types that will not be logged. 
[HTTP Exceptions](https://laravel.com/docs/5.3/errors#http-exceptions)
Some exceptions describe HTTP error codes from the server. For example, this may be a "page not found" error (404), an "unauthorized
 error" (401) or even a developer generated 500 error. In order to generate such a response from anywhere in your application, you may use the abort helper:
abort(404);
abort(403,'Unauthorized action.');
Custom HTTP Error Pages
For example, if you wish to customize the error page for 404 HTTP status codes, create aresources/views/errors/404.blade.php
The views within this directory should be named to
 match the HTTP status code they correspond to. The HttpException instance
 raised by the abort function will be passed to the view as an $exception variable.
[Logging](https://laravel.com/docs/5.3/errors#logging)
**emergency**, **alert**, **critical**, **error**,**warning**, **notice**, **info** and **debug**.
By default, Laravel is configured to create a log file for your application in the storage/logs directory.
 You may write information to the logs using the Log[facade](https://laravel.com/docs/5.3/facades):
Log::emergency($message);Log::alert($message);Log::critical($message);Log::error($message);Log::warning($message);Log::notice($message);Log::info($message);Log::debug($message);
**Contextual Information上下文信息**
An array of contextual data
[https://laravel.com/docs/5.3/errors](https://laravel.com/docs/5.3/errors)
