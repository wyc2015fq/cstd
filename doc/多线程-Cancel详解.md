# 多线程-Cancel详解



多线程-Cancel详解

在取消一个对等线程的请求被同意时，会有一个取消过程同pthread_cancel( )的返回异步发生。

目标线程的取消类型和取消状态决定了取消何时真正发生。可取消性状态描述了线程的取消状况为可取消或不可取消。

线程的可取消性类型决定了线程在收到取消请求后继续执行的能力。可取消性状态和类型是由线程自己动态设置的。

调用线程的可取消性状态和类型是由pthread_setcancelstate( )和pthread_setcanceltype( )设置的。

pthread_setcancelstate( )将调用线程设置为state所指定的可取消性状态，并将之前的状态在oldstate中返回。

pthread_setcanceltype( ) 将调用线程设置为type所指定的可取消性类型，并将之前的类型在oldtype中返回。

调用形式

\#include <pthread.h> int pthread_setcancelstate(int state, int *oldstate);

int pthread_setcanceltype(int type, int *oldtype);

用于设置线程取消状态的state和oldstate的值是：

PTHREAD_CANCEL_DISABLE

PTHREAD_CANCEL_ENABLE

PTHREAD_CANCEL_DISABLE使得线程忽略取消请求。

PTHREAD_CANCEL_ENABLE使得线程允许取消请求。

PTHREAD_CANCEL_ENABLE是任何新近创建的线程的默认状态。

如果成功，函数返回0。如果没有成功，函数返回一个错误号。

如果没有传递有效的state值，则pthread_setcancelstate( )会失败。

函数pthread_setcanceltype( )将调用线程的可取消性状态设置为type指定的类型，并将之前的状态通过oldtype返回。

type和oldtype的值可以为：

PTHREAD_CANCEL_DEFFERED

PTHREAD_CANCEL_ASYNCHRONOUS

PTHREAD_CANCEL_DEFFERED使得线程推迟终止，

直到它到达它的取消点。这是任何新近创建的线程的默认可取消性类型。

PTHREAD_CANCEL_ASYNCHRONOUS使得线程立即终止。

如果成功，函数返回0。如果不成功，函数返回一个错误号。

如果没有传递有效的type，则pthread_setcanceltype( )会失败。

pthread_setcancelstate ( )和pthread_setcanceltype( )共同使用来建立线程的可取消性。

 