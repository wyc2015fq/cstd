# golang内存分布 - walkabc - 博客园
# [golang内存分布](https://www.cnblogs.com/hitfire/articles/6401333.html)
　　golang和C语言之间进行调用
```go;gutter
import (
	"unsafe"
	"fmt"
)
/*
#include <stdio.h>
#include <stdlib.h>
struct SongInfo {
    char *name;
    unsigned int viewCount;
};
void trace(struct SongInfo *songInfo) {
    if(!songInfo) {
        printf("imgInfo is null\n");
        return ;
    }
    songInfo->viewCount += 10;
    fprintf(stdout, "format = %d\n", songInfo->viewCount);
}
*/
import "C"
type Song struct {
}
func (this *Song) TraceInfo() {
	songInfo := C.struct_SongInfo{name: C.CString("zhinian"), viewCount: 9890}
	defer C.free(unsafe.Pointer(songInfo.name))
	C.trace(&songInfo)
	fmt.Println(songInfo.viewCount)
}
```
　　注意import "C"的位置，需要把import "C"放在C语言代码之后。

