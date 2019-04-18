# 线程句柄作WaitForSingleObject的参数是什么意思？ - L_Andy的专栏 - CSDN博客

2015年01月28日 11:22:39[卡哥](https://me.csdn.net/L_Andy)阅读数：1279



**[cpp]**[view
 plain](http://blog.csdn.net/stpeace/article/details/17933771#)[copy](http://blog.csdn.net/stpeace/article/details/17933771#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/145160/fork)

- #include <windows.h>
- #include <iostream.h>
- 
- DWORD WINAPI Fun1Proc(LPVOID lpParameter);  
- 
- int main()  
- {  
- HANDLE hThread1;  
-     hThread1 = CreateThread(NULL, 0, Fun1Proc, NULL, 0, NULL);  
- 
-     WaitForSingleObject(hThread1, INFINITE);  
- 
- int i;  
- for(i = 0; i < 100; i++)  
-     {  
-         cout << "main thread " << i + 1 << endl;  
-     }  
- 
-     CloseHandle(hThread1);  
- return 0;  
- }  
- 
- DWORD WINAPI Fun1Proc(LPVOID lpParameter)  
- {  
- int i;  
- for(i = 0; i < 100; i++)  
-     {  
-         cout << "thread one " << i + 1 << endl;  
-     }  
- 
- return 0;  
- }  

       结果：

thread one 1

thread one 2

thread one 3

thread one 4

thread one 5

thread one 6

thread one 7

thread one 8

thread one 9

thread one 10

thread one 11

thread one 12

thread one 13

thread one 14

thread one 15

thread one 16

thread one 17

thread one 18

thread one 19

thread one 20

thread one 21

thread one 22

thread one 23

thread one 24

thread one 25

thread one 26

thread one 27

thread one 28

thread one 29

thread one 30

thread one 31

thread one 32

thread one 33

thread one 34

thread one 35

thread one 36

thread one 37

thread one 38

thread one 39

thread one 40

thread one 41

thread one 42

thread one 43

thread one 44

thread one 45

thread one 46

thread one 47

thread one 48

thread one 49

thread one 50

thread one 51

thread one 52

thread one 53

thread one 54

thread one 55

thread one 56

thread one 57

thread one 58

thread one 59

thread one 60

thread one 61

thread one 62

thread one 63

thread one 64

thread one 65

thread one 66

thread one 67

thread one 68

thread one 69

thread one 70

thread one 71

thread one 72

thread one 73

thread one 74

thread one 75

thread one 76

thread one 77

thread one 78

thread one 79

thread one 80

thread one 81

thread one 82

thread one 83

thread one 84

thread one 85

thread one 86

thread one 87

thread one 88

thread one 89

thread one 90

thread one 91

thread one 92

thread one 93

thread one 94

thread one 95

thread one 96

thread one 97

thread one 98

thread one 99

thread one 100

main thread 1

main thread 2

main thread 3

main thread 4

main thread 5

main thread 6

main thread 7

main thread 8

main thread 9

main thread 10

main thread 11

main thread 12

main thread 13

main thread 14

main thread 15

main thread 16

main thread 17

main thread 18

main thread 19

main thread 20

main thread 21

main thread 22

main thread 23

main thread 24

main thread 25

main thread 26

main thread 27

main thread 28

main thread 29

main thread 30

main thread 31

main thread 32

main thread 33

main thread 34

main thread 35

main thread 36

main thread 37

main thread 38

main thread 39

main thread 40

main thread 41

main thread 42

main thread 43

main thread 44

main thread 45

main thread 46

main thread 47

main thread 48

main thread 49

main thread 50

main thread 51

main thread 52

main thread 53

main thread 54

main thread 55

main thread 56

main thread 57

main thread 58

main thread 59

main thread 60

main thread 61

main thread 62

main thread 63

main thread 64

main thread 65

main thread 66

main thread 67

main thread 68

main thread 69

main thread 70

main thread 71

main thread 72

main thread 73

main thread 74

main thread 75

main thread 76

main thread 77

main thread 78

main thread 79

main thread 80

main thread 81

main thread 82

main thread 83

main thread 84

main thread 85

main thread 86

main thread 87

main thread 88

main thread 89

main thread 90

main thread 91

main thread 92

main thread 93

main thread 94

main thread 95

main thread 96

main thread 97

main thread 98

main thread 99

main thread 100

      一切不言而喻。

小结：线程结束时，waitforsingleObject才有信号。

>>hThread1代表的线程如果在运行，就要一直等下去，直到线程退出来吗？
是否一直等下去，取决于第二个参数传入的内容。如下所示:
INFINITE:像你所使用的那样传入此参数,此函数会一直等待下去。
0       :函数检测对象状态并立即返回
>0      :如果超出等待时间，线程仍然处于非信号状态，将返回WAIT_TIMEOUT

