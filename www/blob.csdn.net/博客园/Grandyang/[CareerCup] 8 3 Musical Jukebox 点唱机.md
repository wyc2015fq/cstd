# [CareerCup] 8.3 Musical Jukebox 点唱机 - Grandyang - 博客园







# [[CareerCup] 8.3 Musical Jukebox 点唱机](https://www.cnblogs.com/grandyang/p/4787851.html)







8.3 Design a musical jukebox using object-oriented principles.



CareerCup这书实在是太不负责任了，就写了个半调子的程序，说是完整版也可以下载到，但是我怎么找不到，谁知道在哪里下载请告诉博主，多谢啦~



```
class Song;

class CD {
public:    
    // ...
private:
    long _id;
    string _artist;
    set<Song> _songs;
};

class Song {
public:
    // ...
private:
    long _id;
    CD _cd;
    string _title;
    long _length;
};

class Playlist {
public:
    Playlist() {};
    Playlist(Song song, queue<Song> queue): _song(song), _queue(queue) {};
    Song getNextSToPlay() {
        Song next = _queue.front(); _queue.pop();
        return next;
    }
    void queueUpSong(Song s) {
        _queue.push(s);
    }
    
private:
    Song _song;
    queue<Song> _queue;
};

class CDPlayer {
public:
    CDPlayer(CD c, Playlist p): _c(c), _p(p) {};
    CDPlayer(Playlist p): _p(p) {};
    CDPlayer(CD c): _c(c) {};
    void playSong(Song s) {}; // ...
    Playlist getPlaylist() { return _p; };
    void setPlaylist(Playlist p) { _p = p; };
    CD getCD() { return _c; };
    void setCD(CD c) { _c = c; };
    
private:
    Playlist _p;
    CD _c;
};

class User {
public:
    User(string name, long id): _name(name), _id(id) {};
    string getNmae() { return _name; };
    void setName(string name) { _name = name; };
    long getID() { return _id; };
    void setID(long id) { _id = id; };
    User getUser() { return *this; };
    static User addUser(string name, long id) {}; // ... 

private:
    string _name;
    long _id;
};

class SongSelector {
public:
    Song getCurrentSong() {}; // ...
};

class Jukebox {
public:
    Jukebox(CDPlayer cdPlayer, User user, set<CD> cdCollection, SongSelector ts): _cdPlayer(cdPlayer), _user(user), _cdCollection(cdCollection), _ts(ts) {};
    Song getCurrentSong() {
        return _ts.getCurrentSong();
    }
    void setUser(User u) {
        _user = u;
    }
    
private:
    CDPlayer _cdPlayer;
    User _user;
    set<CD> _cdCollection;
    SongSelector _ts;
};
```














