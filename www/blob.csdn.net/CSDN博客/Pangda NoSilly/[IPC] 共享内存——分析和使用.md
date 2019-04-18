# [IPC] 共享内存——分析和使用 - Pangda NoSilly - CSDN博客

2018年10月31日 22:56:59[hepangda](https://me.csdn.net/hepangda)阅读数：149


# 共享内存

## 共享内存简介

共享内存是所有进程间通信（IPC）手段中速度最快的，不同进程间使用事实上的同一内存区域，这样使得进程间使用信息时免去“复制”这一流程，减少开销。

以下面“进程A从文件`f`中读取数据，进行加工之后，将数据传递给进程B”这种场景为例，若使用其他的IPC形式，我们至少需要以下步骤：

```
1. 从文件f中复制数据到进程A的内存中；
2. 加工数据；
3. 将加工好的数据通过系统调用拷贝到内核空间中；
4. 进程B得知有数据发来，从内核空间将加工好的数据拷贝到进程B的内存中；
5. 进程B使用数据
```

而我们若使用共享内存，则至少需要以下三个步骤：

```
1. 从文件f中复制数据到共享内存区域中；
2. 加工数据；
3. 进程B使用数据
```

显然的，在使用共享内存时，我们减少了一些无意义的数据复制。

不过，在这之中还有一些隐藏在其中影响效率的其他细节——系统调用的数量。在我们使用其他的IPC形式时，读取、写入时将不可避免的使用系统调用，当进行大量的数据吞吐时，频繁的系统调用就会严重地拖累性能。而当我们使用共享内存区域时，可能仅仅需要一个读取内存的开销，或是附加一个同步原语的开销。

除此之外，使用共享内存区域也有一些意外的好处，当进程因为意外退出再重启时，若对共享内存区域使用得当，那么在共享内存区域存放的数据，可以以很小的代价继续使用。

当然，作为代价，共享内存并不具备一些便利性的API，使用共享内存作为IPC形式需要手动的规定共享内存区的“使用协议”，对于有竞争的区域，应当使用信号量等同步原语予以保护。

## 如何使用

我们以两进程间不存在亲缘关系这种最为“一般”的情况为例，在此时我们可以使用以下两种方式共享内存：
- 内存映射文件
- 共享内存区对象

若两进程之间存在亲缘关系，那么在使用“内存映射文件”这种形式共享内存时，则还可以使用“匿名映射”这一方法，该方法将在下文中陈述。

## 内存映射文件

### mmap函数

内存映射文件用到的函数是`mmap`，是的，它是个“身兼多任”的函数，函数原型为：

```
#include <sys/mman.h>

void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
```

`mmap`函数将`fildes`参数所指定的文件描述符中从偏移为`off`开始持续`len`长度的区域以`prot`方式保护，映射到当前进程的`addr`地址处。若执行成功，那么将返回被映射区的起始地址，反之则返回`MAP_FAILED`。

若我们将`addr`参数指定为`NULL`，则表示我们希望由内核选择一个合适的映射地址，于是通常地我们都将之设定为`NULL`。除此之外，我们将`mmap`函数作为内存映射文件用于共享内存时，`off`参数也通常设为0，这也是很容易理解的，毕竟`off`设为其他值除了增大文件之外，在这种情景下毫无意义。

`prot`参数可以指定为`PROT_READ`（数据可写）、`PROT_WRITE`（数据可读）、`PROT_EXEC`（数据可执行）、`PROT_NONE`（数据不可访问）。一般而言，我们会将其指定为`PROT_READ | PROT_WRITE`，表示其可读可写。

`flags`参数可以指定为`MAP_SHARED`（对于区域的变动是共享的）、`MAP_PRIVATE`（对于区域的变动是独自的）和`MAP_FIXED`（要求准确的解释addr参数）。对于我们作为共享内存这一用法来说，显然我们应当指定`MAP_SHARED`作为参数。

`MAP_FIXED`参数的意义可能会使人感到迷惑，何谓准确的解释`addr`参数？当我们未指定`addr`参数时，内核可能因为内存对齐等其他原因，更改最终映射的地址，也就是说此时`addr`参数仅仅是一个参考值，若要保证最终映射的地址一定为`addr`参数所指定的地址，则需要设定此参数。不过对于一个良好的、可移植的代码，`addr`通常将设定为NULL，也就没有使用`MAP_FIXED`的必要。

对于共享内存的使用目的，`len`参数就是共享内存区的大小。

当使用完毕之后，我们可以使用`munmap`函数解除相应的内存映射，该函数的原型为：

```
int munmap(void *addr, size_t len);
```

`addr`参数为对应的`mmap`函数的返回值，`len`与`mmap`中`len`的意义相同。

### 建立内存映射文件

在熟悉了`mmap`函数的使用之后，简单的谈一谈如何建立内存映射文件，首先，需要使用共享内存的进程使用`open`函数打开指定的文件，然后进程使用`mmap`将之映射到自己的地址空间中，在`mmap`映射建立完成之后，我们就可以使用`close`关闭掉文件了，这对于已经建立映射没有影响。

下面是一段代码，使用了内存映射文件，由A进程产生一个随机数并共享给B进程，由B进程打印出来。

```
// 文件：a.c
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void err(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) {
  // (1) 使用open打开指定的内存映射文件
  int fd = open("/tmp/example.shm", O_RDWR | O_CREAT, 0666);
  if (fd == -1) {
    err("open");
  }

  // (2) 文件可能之前不存在，没有页面，此时直接使用将引发一个bus error
  //     先将其截断为指定大小，然后才可以进行相关读取
  if (ftruncate(fd, sizeof(int)) == -1) {
    err("ftruncate");
  }

  // (3) 调用mmap将内存映射文件映射到自身的地址空间中
  int *p = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if (p == MAP_FAILED) {
    err("mmap");
  }

  // (4) 完成映射之后可以关闭fd，不影响已经建立的映射
  close(fd);

  // (5) 产生随机数并存入共享内存中，使用sleep为了便于观察
  for (srand(7777);; sleep(1)) {
    int v = rand();
    *p = v;
    printf("produce: %d\n", v);
  }

  // (6) 解除内存映射
  if (munmap(p, sizeof(int)) == -1) {
    err("munmap");
  }

  return EXIT_SUCCESS;
}

// 文件：b.c
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void err(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) {
  int fd = open("/tmp/example.shm", O_RDWR, 0666);
  if (fd == -1) {
    err("open");
  }

  int *p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p == MAP_FAILED) {
    err("mmap");
  }

  close(fd);

  for (;; sleep(1)) {
    printf("%d\n", *p);
  }

  if (munmap(p, sizeof(int)) == -1) {
    err("munmap");
  }

  return EXIT_SUCCESS;
}
```

通过这种手法进行的共享内存，内存的内容将保存在文件中，这并不需要我们自身进行任何操作，全部交由内核代劳。

### 匿名映射

上面的方法中，我们将不得不在文件系统中建立一个文件，若我们想要在具有亲缘关系的进程中共享内存，我们可以采用“匿名映射”的方法取代建立文件，具体方法有两种：
- 打开`/dev/zero`文件以取代之前的文件
- 使用MAP_ANON参数并将`fd`参数指定为`-1`

以下是使用匿名映射的例子，在该例中，父进程负责在共享内存中产生随机数，而子进程则负责打印父进程产生的随机数。

```
// 注释部分代码为使用/dev/zero的情况
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void err(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) {
  // int fd = open("/dev/zero", O_RDWR, 0666);
  // if (fd == -1) {
    // err("open");
  // }

  // int *p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  int *p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
  // 或者MAP_SHARED | MAP_ANONYMOUS

  if (p == MAP_FAILED) {
    err("mmap");
  }

  // close(fd);

  pid_t id = fork();
  if (id == 0) {
    for (;; sleep(1)) {
      printf("%d\n", *p);
    }
  } else {
    for (srand(7777);; sleep(1)) {
      int v = rand();
      *p = v;
      printf("produce: %d\n", v);
    }
  }

  if (munmap(p, sizeof(int)) == -1) {
    err("munmap");
  }

  return EXIT_SUCCESS;
}
```

## 共享内存区对象

共享内存区对象具有两种实现，由于其与内存映射的用法高度统一且可以使用一些与文件相关的系统调用，与习惯比较一致，所以这里仅介绍POSIX共享内存区。

### POSIX共享内存区对象函数

POSIX共享内存区对象提供了以下两个函数：

```
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int shm_open(const char *name, int oflag, mode_t mode);
int shm_unlink(const char *name);
```

这两个函数的直观感觉与文件操作的`open`函数与`unlink`函数一致，实际上他们的使用方法也近乎一致，区别仅仅在于`shm_open`函数和`shm_unlink`函数操作的对象是共享内存区对象，而非普通的文件。`shm_open`函数在调用成功时将返回共享内存区对象的文件描述符。

并且，和普通的`unlink`函数一样，使用`shm_unlink`函数删除之后，并不影响已经打开该共享内存区对象的进程接着使用它。

新创建的共享内存区对象同新创建的内存映射文件一样，需要先使用`ftruncate`函数将之改为我们需要的大小。

使用`shm_open`创建共享内存区对象之后，既可以使用`read`和`write`系统调用使用，也可以像内存映射文件一样使用`mmap`将之映射到内存地址空间。不过，既然使用共享内存是为了更高的效率，那么我们就应当使用`mmap`。用法与之前的内存映射文件没有太大的区别，以下是一个使用POSIX共享内存区对象改写的，产生随机数的简单的例子：

```
// 文件：a.c
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void err(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) {
  // (1) 使用shm_open创建新的共享内存
  int fd = shm_open("example", O_RDWR | O_CREAT, 0666);
  if (fd == -1) {
    err("shm_open");
  }

  // (2) 设定共享内存的大小
  if (ftruncate(fd, sizeof(int)) == -1) {
    err("ftruncate");
  }

  // (3) 调用mmap将内存映射文件映射到自身的地址空间中
  int *p = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if (p == MAP_FAILED) {
    err("mmap");
  }

  // (4) 完成映射之后可以关闭fd，不影响已经建立的映射
  close(fd);

  // (5) 产生随机数并存入共享内存中，使用sleep为了便于观察
  for (srand(7777);; sleep(1)) {
    int v = rand();
    *p = v;
    printf("produce: %d\n", v);
  }

  // (6) 在文件系统中删除共享内存区对象
  if (shm_unlink("example") == -1) {
    err("shm_unlink");
  }

  // (7) 解除内存映射
  if (munmap(p, sizeof(int)) == -1) {
    err("munmap");
  }

  return EXIT_SUCCESS;
}

// 文件： b.c
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void err(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) {
  int fd = shm_open("example", O_RDWR, 0666);
  if (fd == -1) {
    err("open");
  }

  int *p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p == MAP_FAILED) {
    err("mmap");
  }

  close(fd);

  for (;; sleep(1)) {
    printf("%d\n", *p);
  }

  if (munmap(p, sizeof(int)) == -1) {
    err("munmap");
  }

  return EXIT_SUCCESS;
}
```

可以看出，他们之间并没有什么显著的不同。

### 文件系统抽象

使用`shm_open`函数创建的共享内存区对象将被存放在`/dev/shm`目录下，我们可以通过`ls /dev/shm`查看当前的共享内存区对象，并且，我们也可以使用`mount`命令或者`df`命令看到其在文件系统中的挂载点，以下是一个例子：

```
[pangda@hepangda] df
Filesystem     1K-blocks     Used Available Use% Mounted on
dev              4035096        0   4035096   0% /dev
run              4041804     1208   4040596   1% /run
/dev/sdb2       71721912 19109432  48926084  29% /
tmpfs            4041804    53052   3988752   2% /dev/shm
tmpfs            4041804        0   4041804   0% /sys/fs/cgroup
tmpfs            4041804    17204   4024600   1% /tmp
/dev/sda2          98304    83544     14760  85% /boot
/dev/sdb3       63223176  6687152  53294672  12% /home
tmpfs             808360       20    808340   1% /run/user/1000
```

## 解决共享内存区的竞争

到此为止，我所给出的程序全部是不存在竞争关系的程序，一般来说很难有如此“和谐”的使用场景。那么，我们将之前的事件复杂化一些：现在有多个进程在产生随机数，但只有一个进程来打印随机数，而我们不想让任何一个产生的随机数被“浪费”。

为了解决这样的问题，我们应当规划我们的共享内存区域，并且使用一个信号量保证不出现“浪费”：

```
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void err(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

// 共享内存区域
struct memory_area {
  sem_t sem;  // 信号量，用于同步和解决竞争
  int num;    // 存储产生的随机数
};

// 生产随机数函数
void produce(struct memory_area *p) {
  for (srand(p->num);; sleep(1)) {
    sem_wait(&(p->sem));
    int v = rand();
    p->num = v;
    printf("[%d] produce: %d\n", getpid(), v);
  }
}

// 打印随机数函数
void consume(struct memory_area *p) {
  for (;; sleep(1)) {
    printf("consume: %d\n", p->num);
    sem_post(&(p->sem));
  }
}

int main(int argc, const char *argv[]) {
  // 建立共享内存区域
  struct memory_area *p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANON, -1, 0);
  if (p == MAP_FAILED) err("mmap");

  // 初始化信号量
  sem_init(&(p->sem), 1, 1);

  // 创建进程并分别执行打印和产生随机数的函数
  pid_t id1, id2;
  if ((id1 = fork()) != 0) {
    if ((id2 = fork()) != 0) {
      consume(p);
    } else {
      produce(p);
    }
  } else {
    produce(p);
  }

  // 销毁信号量
  sem_destroy(&(p->sem));
  // 解除映射
  if (munmap(p, sizeof(int)) == -1) err("munmap");

  return EXIT_SUCCESS;
}
```

