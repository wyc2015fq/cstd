# ReactiveX 学习笔记（16）RxPY - zwvista - 博客园

## [ReactiveX 学习笔记（16）RxPY](https://www.cnblogs.com/zwvista/p/9509020.html)

### RxPY

RxPY 是 ReactiveX 的 Python语言实现。

```
# 安装 RxPY
$ pip3 install rx
Successfully installed rx-1.6.1
```

### Basics

```
from rx import Observable
source = Observable.of("Alpha", "Beta", "Gamma", "Delta", "Epsilon")
source.subscribe(on_next=lambda value: print("Received {0}".format(value)),
                 on_completed=lambda: print("Done!"),
                 on_error=lambda error: print("Error Occurred: {0}".format(error))
                 )
'''
Received Alpha
Received Beta
Received Gamma
Received Delta
Received Epsilon
Done!
'''
```

### Operators

```
from rx import Observable
Observable.of("Alpha", "Beta", "Gamma", "Delta", "Epsilon") \
    .map(lambda s: len(s)) \
    .filter(lambda i: i >= 5) \
    .subscribe(lambda value: print("Received {0}".format(value)))
'''
Received 5
Received 5
Received 5
Received 7
'''
```

### Events

```
from rx import Observable
Observable.interval(1000) \
    .map(lambda i: "{0} Mississippi".format(i)) \
    .subscribe(lambda s: print(s))
input("Press any key to quit\n")
'''
Press any key to quit
0 Mississippi
1 Mississippi
2 Mississippi
3 Mississippi
4 Mississippi
...
'''
```

### Multicasting

```
from rx import Observable
from random import randint
three_emissions = Observable.range(1, 3)
three_random_ints = three_emissions.map(lambda i: randint(1, 100000))
three_random_ints.subscribe(lambda i: print("Subscriber 1 Received: {0}".format(i)))
three_random_ints.subscribe(lambda i: print("Subscriber 2 Received: {0}".format(i)))
'''
Subscriber 1 Received: 76795
Subscriber 1 Received: 19486
Subscriber 1 Received: 96809
Subscriber 2 Received: 79148
Subscriber 2 Received: 33423
Subscriber 2 Received: 3319
'''
```

```
from rx import Observable
from random import randint
three_emissions = Observable.range(1, 3)
three_random_ints = three_emissions.map(lambda i: randint(1, 100000)).publish()
three_random_ints.subscribe(lambda i: print("Subscriber 1 Received: {0}".format(i)))
three_random_ints.subscribe(lambda i: print("Subscriber 2 Received: {0}".format(i)))
three_random_ints.connect()
'''
Subscriber 1 Received: 83159
Subscriber 2 Received: 83159
Subscriber 1 Received: 82025
Subscriber 2 Received: 82025
Subscriber 1 Received: 72397
Subscriber 2 Received: 72397
'''
```

```
from rx import Observable
from random import randint
three_emissions = Observable.range(1, 3)
three_random_ints = three_emissions.map(lambda i: randint(1, 100000)).publish().auto_connect(2)
three_random_ints.subscribe(lambda i: print("Subscriber 1 Received: {0}".format(i)))
three_random_ints.subscribe(lambda i: print("Subscriber 2 Received: {0}".format(i))) # second subscriber triggers firing
'''
Subscriber 1 Received: 72527
Subscriber 2 Received: 72527
Subscriber 1 Received: 53066
Subscriber 2 Received: 53066
Subscriber 1 Received: 32249
Subscriber 2 Received: 32249
'''
```

### Combining

```
from rx import Observable
letters = Observable.of("Alpha", "Beta", "Gamma", "Delta", "Epsilon")
intervals = Observable.interval(1000)
Observable.zip(letters, intervals, lambda s, i: (s, i)) \
    .subscribe(lambda t: print(t))
input("Press any key to quit\n")
'''
Press any key to quit
('Alpha', 0)
('Beta', 1)
('Gamma', 2)
('Delta', 3)
('Epsilon', 4)
'''
```

### Concurrency

```
import multiprocessing
import random
import time
from threading import current_thread
from rx import Observable
from rx.concurrency import ThreadPoolScheduler
def intense_calculation(value):
    # sleep for a random short duration between 0.5 to 2.0 seconds to simulate a long-running calculation
    time.sleep(random.randint(5, 20) * .1)
    return value
# calculate number of CPU's, then create a ThreadPoolScheduler with that number of threads
optimal_thread_count = multiprocessing.cpu_count()
pool_scheduler = ThreadPoolScheduler(optimal_thread_count)
# Create Process 1
Observable.of("Alpha", "Beta", "Gamma", "Delta", "Epsilon") \
    .map(lambda s: intense_calculation(s)) \
    .subscribe_on(pool_scheduler) \
    .subscribe(on_next=lambda s: print("PROCESS 1: {0} {1}".format(current_thread().name, s)),
               on_error=lambda e: print(e),
               on_completed=lambda: print("PROCESS 1 done!"))
# Create Process 2
Observable.range(1, 10) \
    .map(lambda s: intense_calculation(s)) \
    .subscribe_on(pool_scheduler) \
    .subscribe(on_next=lambda i: print("PROCESS 2: {0} {1}".format(current_thread().name, i)),
               on_error=lambda e: print(e), on_completed=lambda: print("PROCESS 2 done!"))
# Create Process 3, which is infinite
Observable.interval(1000) \
    .map(lambda i: i * 100) \
    .observe_on(pool_scheduler) \
    .map(lambda s: intense_calculation(s)) \
    .subscribe(on_next=lambda i: print("PROCESS 3: {0} {1}".format(current_thread().name, i)),
               on_error=lambda e: print(e))
input("Press any key to exit\n")
'''
Press any key to exit
PROCESS 1: ThreadPoolExecutor-1_0 Alpha
PROCESS 1: ThreadPoolExecutor-1_0 Beta
PROCESS 2: ThreadPoolExecutor-1_1 1
PROCESS 3: ThreadPoolExecutor-1_2 0
PROCESS 2: ThreadPoolExecutor-1_1 2
PROCESS 1: ThreadPoolExecutor-1_0 Gamma
PROCESS 3: ThreadPoolExecutor-1_2 100
PROCESS 2: ThreadPoolExecutor-1_1 3
PROCESS 1: ThreadPoolExecutor-1_0 Delta
PROCESS 3: ThreadPoolExecutor-1_4 200
PROCESS 1: ThreadPoolExecutor-1_0 Epsilon
PROCESS 1 done!
PROCESS 2: ThreadPoolExecutor-1_1 4
PROCESS 2: ThreadPoolExecutor-1_1 5
PROCESS 3: ThreadPoolExecutor-1_5 300
PROCESS 2: ThreadPoolExecutor-1_1 6
PROCESS 3: ThreadPoolExecutor-1_2 400
PROCESS 3: ThreadPoolExecutor-1_2 500
PROCESS 2: ThreadPoolExecutor-1_1 7
PROCESS 3: ThreadPoolExecutor-1_2 600
PROCESS 2: ThreadPoolExecutor-1_1 8
PROCESS 3: ThreadPoolExecutor-1_2 700
PROCESS 2: ThreadPoolExecutor-1_1 9
PROCESS 3: ThreadPoolExecutor-1_2 800
PROCESS 2: ThreadPoolExecutor-1_1 10
PROCESS 2 done!
PROCESS 3: ThreadPoolExecutor-1_2 900
PROCESS 3: ThreadPoolExecutor-1_2 1000
PROCESS 3: ThreadPoolExecutor-1_2 1100
PROCESS 3: ThreadPoolExecutor-1_2 1200
PROCESS 3: ThreadPoolExecutor-1_2 1300
PROCESS 3: ThreadPoolExecutor-1_2 1400
PROCESS 3: ThreadPoolExecutor-1_2 1500
PROCESS 3: ThreadPoolExecutor-1_2 1600
PROCESS 3: ThreadPoolExecutor-1_2 1700
PROCESS 3: ThreadPoolExecutor-1_2 1800
...
'''
```

### Alignment

```
from rx import Observable, Observer

xs = Observable.of(1,2,3)
ys = Observable.of(4,5,6)
zs = xs + ys  # Concatenate observables
zs.to_list().subscribe(lambda value: print(value))

xs = Observable.of(1,2,3)
ys = xs * 4
ys.to_list().subscribe(lambda value: print(value))

xs = Observable.of(1,2,3)
ys = xs[1:-1]
ys.to_list().subscribe(lambda value: print(value))

xs = Observable.of(1,2,3,4,5,6)
ys = xs.to_blocking()
zs = (x*x for x in ys if x > 3)
for x in zs:
    print(x)
'''
[1, 2, 3, 4, 5, 6]
[1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3]
[2]
16
25
36
'''
```


