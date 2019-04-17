# [CareerCup] 8.5 Online Book Reader System 在线读书系统 - Grandyang - 博客园







# [[CareerCup] 8.5 Online Book Reader System 在线读书系统](https://www.cnblogs.com/grandyang/p/4790816.html)







8.5 Design the data structures for an online book reader system.



这道题OOB的题让我们设计一个在线读书系统，还是没有任何提示，所以发挥空间很大。根据书上的解答，我们设计一个具有基本功能的系统：

1. 用户会员的建立和延长

2. 搜索书库中的书

3. 在线读书

4. 每次只能有一个活跃用户

5. 每个读者只能读一本书

那么最基本的先实现书类Book，和用户类User，然后是书库类Library，用户管理类UserManager，显示类Display，最后是整合类OnlineReaderSystem，具体实现代码如下：



```
class Book {
public:
    Book(int id, string det): _bookId(id), _details(det) {}
    int getID() { return _bookId; }
    void setId(int id) { _bookId = id; }
    string getDetails() { return _details; }
    void setDetails(string d) { _details = d; }

private:
    int _bookId;
    string _details;
};

class User {
public:
    User(int id, string details, int accountType): _userId(id), _details(details), _accountType(accountType) {}
    void renewMembership() {}
    int getID() { return _userId; }
    void setID(int id) { _userId = id; }
    string getDetails() { return _details; }
    void setDetails(string details) { _details = details; }
    int getAccountType() { return _accountType; }
    void setAccountType(int t) { _accountType = t; }

private:
    int _userId;
    string _details;
    int _accountType;
};

class Library {
public:
    Library() {}
    Book* addBook(int id, string details) {
        if (_books.find(id) == _books.end()) return nullptr;
        Book *book = new Book(id, details);
        _books[id] = book;
        return book;
    }
    bool remove(Book *b) { return remove(b->getID()); }
    bool remove(int id) {
        if (_books.find(id) == _books.end()) return false;
        _books.erase(id);
        return true;
    }
    Book* find(int id) {
        if (_books.find(id) == _books.end()) return nullptr;
        return _books[id];
    }

private:
    unordered_map<int, Book*> _books;
};

class UserManager {
public:
    UserManager() {}
    User* addUser(int id, string details, int accountType) {
        if (_users.find(id) == _users.end()) return nullptr;
        User *user = new User(id, details, accountType);
        _users[id] = user;
        return user;
    }
    bool remove(User *u) { return remove(u->getID()); }
    bool remove(int id) {
        if (_users.find(id) == _users.end()) return false;
        _users.erase(id);
        return true;
    }
    User* find(int id) {
        if (_users.find(id) == _users.end()) return nullptr;
        return _users[id];
    }

private:
    unordered_map<int, User*> _users;
};

class Display {
public:
    Display() {}
    void displayUser(User *user) {
        _activeUser = user;
        refreshUsername();
    }
    void displayBook(Book *book) {
        _pageNumber = 0;
        _activeBook = book;
        refreshTitle();
        refreshDetails();
        refreshPage();
    }
    void turnPageForward() {
        ++_pageNumber;
        refreshPage();
    }
    void turnPageBackward() {
        --_pageNumber;
        refreshPage();
    }
    void refreshUsername() {} // ...
    void refreshTitle() {} // ...
    void refreshDetails() {} // ...
    void refreshPage() {} // ...

private:
    Book *_activeBook;
    User *_activeUser;
    int _pageNumber = 0;
};

class OnlineReaderSystem {
public:
    OnlineReader() {
        _userManager = new UserManager();
        _library = new Library();
        _display = new Display();
    }
    Library* getLibrary() { return _library; }
    UserManager* getUserManager() { return _userManager; }
    Display* getDisplay() { return _display; }
    Book* getActiveBook() { return _activeBook; }
    void setActiveBook(Book *book) {
        _activeBook = book;
        _display->displayBook(book);
    }
    User* getActiveUser() { return _activeUser; }
    void setActiveUser(User *user) {
        _activeUser = user;
        _display->displayUser(user);
    }

private:
    Library *_library;
    UserManager *_userManager;
    Display *_display;
    Book *_activeBook;
    User *_activeUser;
};
```














