# java遍历文件夹读取文件进行处理 - youfangyuan - CSDN博客
2013年06月16日 12:37:52[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：2318
```java
import java.io.File;
public interface FileTreeWalk {
    public void walk(File path);
}
```
```java
import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
public class FileTreeWalker {
    private File path;
    private static final FileFilter directoryFilter = new FileFilter() {
        public boolean accept(File pathname) {
            return pathname.isDirectory();
        }
    };
    private FileFilter filter;
    public FileTreeWalker(File path) throws IOException {
        this(path, new FileFilter() {
            public boolean accept(File pathname) {
                return pathname.isFile();
            }
        });
    }
    public FileTreeWalker(File path, FileFilter filter) throws IOException {
        if (path == null || !path.exists() || path.isFile()) {
            throw new IOException("Path " + path + " is not a valid directory.");
        }
        this.path = path;
        this.filter = filter;
    }
    public void walk(FileTreeWalk walk) {
        walkDirectory(walk, path);
    }
    private void walkDirectory(FileTreeWalk walk, File dir) {
        File[] files = dir.listFiles(filter);
        for (File file : files) {
            walk.walk(file);
        }
        File[] dirs = dir.listFiles(directoryFilter);
        for (File subDir : dirs) {
            walkDirectory(walk, subDir);
        }
    }
}
```
