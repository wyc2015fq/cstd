# [CareerCup] 8.7 Chat Server 聊天服务器 - Grandyang - 博客园







# [[CareerCup] 8.7 Chat Server 聊天服务器](https://www.cnblogs.com/grandyang/p/4802504.html)







8.7 Explain how you would design a chat server. In particular, provide details about the various backend components, classes, and methods. What would be the hardest problems to solve?



这个简易的聊天服务器功能十分的有限，毕竟只是针对面试题的，谁也无法在面试的有限时间内设计出像QQ那样强大的聊天工具，所以只是实现一些基本的功能即可，根据书上所述，包括如下功能：

1. 登陆和登出

2. 添加请求（发送，接受，和拒绝）

3. 更显一个状态消息

4. 建立私聊和群聊

5. 添加信息到私聊和群聊



```
enum UserStatusType { Offline, Away, Idle, Available, Busy };
enum RequestStatus { Unread, Read, Accepted, Rejected };

class Date {
public:
    Date() {}
    Date(int m, int d, int y): _month(m), _day(d), _year(y) {}

private:
    int _month, _day, _year;
};

class Message {
public:
    Message(string content, Date *date): _content(content), _date(date) {} 
    string getContent() { return _content; }
    Date* getDate() { return _date; }

private:
    string _content;
    Date *_date;
};

class Conversation {
public:
    vector<Message*> getMessages() { return _messages; }
    bool addMessage(Message *m) { 
        _messages.push_back(m);
        return true;
    } 
    int getId() { return _id; }

protected:
    vector<User*> _participants;
    int _id;
    vector<Message*> _messages;
};

class GroupChat: public Conversation {
public:
    void removeParticipant(User *user) {
        for (vector<User*>::iterator it = _participants.begin(); it != _participants.end(); ++it) {
            if (*it == user) _participants.erase(it);
        }
    } 
    void addParticipant(User *user) {
        _participants.push_back(user);
    } 
};

class PrivateChat: public Conversation {
public:
    PrivateChat(User *user1, User *user2) {
        _participants.push_back(user1);
        _participants.push_back(user2);
    } 
    User* getOtherParticipant(User *primary) {
        if (_participants[0] == primary) {
            return _participants[1];
        } else if (_participants[1] == primary) {
            return _participants[0];
        }
        return nullptr;
    } 
};

class AddRequest {
public:
    AddRequest(User *from, User *to, Date *date): _fromUser(from), _toUser(to), _date(date) {
        _status = RequestStatus::Unread;
    } 
    void setStatus(RequestStatus status) { _status = status; }
    RequestStatus getStatus() { return _status; }
    User* getFromUser() { return _fromUser; }
    User* getToUser() { return _toUser; }
    Date* getDate() { return _date; }

private:
    User *_fromUser;
    User *_toUser;
    Date *_date;
    RequestStatus _status;
};

class UserStatus{
public:
    UserStatus(UserStatusType type, string message): _type(type), _message(message) {} 
    UserStatusType getStatusType() { return _type; }
    string getMessage() { return _message; }

private:
    string _message;
    UserStatusType _type;
};

class UserManager;

class User {
public:
    User(int id, string accountName, string fullName): _id(id), _accountName(accountName), _fullName(fullName) {}
    bool sendMessageToUser(User *to, string content) {
        PrivateChat *chat = _privateChats[to->getId()];
        if (chat == nullptr) {
            chat = new PrivateChat(this, to);
            _privateChats[to->getId()] = chat;
        }
        Message *message = new Message(content, new Date());
        return chat->addMessage(message);
    } 
    bool sendMessageToGroupChat(int id, string content) {
        GroupChat *chat = _groupChats[id];
        if (chat != nullptr) {
            Message *message = new Message(content, new Date());
            return chat->addMessage(message);
        }
        return false;
    } 
    void setStatus(UserStatus *status) { _status = status; }
    UserStatus* getStatus() { return _status; };
    bool addContact(User *user) {
        if (_contacts.find(user->getId()) != _contacts.end()) {
            return false;
        } else {
            _contacts[user->getId()] = user;
            return true;
        }
    } 
    void receivedAddRequest(AddRequest *req) {
        int senderId = req->getFromUser()->getId();
        if (_receivedAddRequests.find(senderId) == _receivedAddRequests.end()) {
            _receivedAddRequests[senderId] = req;
        }
    } 
    void sentAddRequest(AddRequest *req) {
        int receiverId = req->getFromUser()->getId();
        if (_sentAddRequests.find(receiverId) == _sentAddRequests.end()) {
            _sentAddRequests[receiverId] = req;
        }
    } 
    void removeAddRequest(AddRequest *req) {
        if (req->getToUser() == this) {
            for (unordered_map<int, AddRequest*>::iterator it = _receivedAddRequests.begin(); it != _receivedAddRequests.end(); ++it) {
                if (it->second == req) _receivedAddRequests.erase(it);
            }
        } else if (req->getFromUser() == this) {
            for (unordered_map<int, AddRequest*>::iterator it = _sentAddRequests.begin(); it != _sentAddRequests.end(); ++it) {
                if (it->second == req) _sentAddRequests.erase(it);
            }
        }
    } 
    void requestAddUser(string accountName) {
        UserManager::getInstance()->addUser(this, accountName);
    } 
    void addConversation(PrivateChat *conversation) {
        User *otherUser = conversation->getOtherParticipant(this);
        _privateChats[otherUser->getId()] = conversation;
    }
    void addConversation(GroupChat *conversation) {
        _groupChats.push_back(conversation);
    }
    int getId() { return _id; }
    string getAccountName() { return _accountName; };
    string getFullName() { return _fullName; }

private:
    int _id;
    UserStatus *_status = nullptr;
    unordered_map<int, PrivateChat*> _privateChats;
    vector<GroupChat*> _groupChats;
    unordered_map<int, AddRequest*> _receivedAddRequests;
    unordered_map<int, AddRequest*> _sentAddRequests;
    unordered_map<int, User*> _contacts;
    string _accountName;
    string _fullName;
};

class UserManager {
public:
    static UserManager* getInstance() {
        if (_instance == nullptr) _instance = new UserManager();
        return _instance;
    }
    void addUser(User *fromUser, string toAcountName) {
        User *toUser = _usersByAccountName[toAcountName];
        AddRequest *req = new AddRequest(fromUser, toUser, new Date());
        toUser->receivedAddRequest(req);
        fromUser->sentAddRequest(req);
    } 
    void approveAddRequest(AddRequest *req) {
        req->setStatus(RequestStatus::Accepted);
        User *from = req->getFromUser();
        User *to = req->getToUser();
        from->addContact(to);
        to->addContact(from);
    } 
    void rejectAddRequest(AddRequest *req) {
        req->setStatus(RequestStatus::Rejected);
        User *from = req->getFromUser();
        User *to = req->getToUser();
        from->removeAddRequest(req);
        to->removeAddRequest(req);
    }
    void userSignedOn(string accountName) {
        User *user = _usersByAccountName[accountName];
        if (user != nullptr) {
            user->setStatus(new UserStatus(UserStatusType::Available, ""));
            _onlineUsers[user->getId()] = user;
        }
    } 
    void userSignedOff(string accountName) {
        User *user = _usersByAccountName[accountName];
        if (user != nullptr) {
            user->setStatus(new UserStatus(UserStatusType::Offline, ""));
            for (unordered_map<int, User*>::iterator it = _onlineUsers.begin(); it != _onlineUsers.end(); ++it) {
                if (it->first == user->getId()) _onlineUsers.erase(it);
            }
        }
    } 

private:
    static UserManager *_instance;
    unordered_map<int, User*> _usersById;
    unordered_map<string, User*> _usersByAccountName;
    unordered_map<int, User*> _onlineUsers;
};
```














