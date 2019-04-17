# [CareerCup] 8.9 An In-memory File System 内存文件系统 - Grandyang - 博客园







# [[CareerCup] 8.9 An In-memory File System 内存文件系统](https://www.cnblogs.com/grandyang/p/4806835.html)







8.9 Explain the data structures and algorithms that you would use to design an in-memory file system. Illustrate with an example in code where possible.



这道题让我们设计一个内存文件系统，咋一听感觉挺吓人啊，像是很底层的东西，但其实只是一道很普通的OOB的题目而已。根据书上所述，在一个简化的文件系统，由文件和目录构成的，而文件类File和目录类Directory都是由一个入口类Entry派生而来的，可参见如下代码：



```
class Directory;

class Entry {
public:
    Entry(string n, Directory *p) {
        _name = n;
        _parent = p;
        _created = getCurrentTimeMillis();
    }
    static long getCurrentTimeMillis() {
        time_t res = time(NULL);
        localtime(&res);
        return (long)res;
    }
    bool deleteEntry() {
        if (_parent == nullptr) return false;
        return _parent->deleteEntry(this);
    }
    virtual int size() = 0;
    string getFullPath() {
        if (_parent == nullptr) return _name;
        else return _parent->getFullPath() + "/" + _name;
    }
    long getCreationTime() { return _created; }
    long getLastUpdatedTime() { return _lastUpdated; }
    long getLastAccessedTime() { return _lastAccessed; }
    void changeName(string n) { _name = n; }
    string getName() { return _name; }

protected:
    Directory *_parent;
    long _created;
    long _lastUpdated;
    long _lastAccessed;
    string _name;
};

class File: public Entry {
public:
    File(string n, Directory *p, int sz): Entry(n, p) {
        _size = sz;
    }
    int size() { return _size; }
    string getContents() { return _content; }
    void setContents(string c) { _content = c; }

private:
    string _content;
    int _size;
};

class Directory: public Entry {
public:
    Directory(string n, Directory *p): Entry(n, p) {}
    int size() {
        int size = 0;
        for (auto a : _contents) {
            size += a->size();
        }
        return size;
    }
    int numberOfFiles() {
        int cnt = 0;
        for (auto a : _contents) {
            if (Directory *d = dynamic_cast<Directory*>(a)) {
                ++cnt;
                cnt += d->numberOfFiles();
            } else if (File *f = dynamic_cast<File*>(a)) {
                ++cnt;
            }
        }
        return cnt;
    }
    bool deleteEntry(Entry *entry) {
        for (vector<Entry*>::iterator it = _contents.begin(); it != _contents.end(); ++it) {
            if (*it == entry) {
                _contents.erase(it);
            }
        }
    }
    void addEntry(Entry *entry) {
        _contents.push_back(entry);
    }

protected:
    vector<Entry*> _contents;
    vector<Entry*> getContents() { return _contents; }
};
```














