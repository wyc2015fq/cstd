# plda源码（二） - Arthur的随笔 - CSDN博客
2018年12月17日 15:38:43[largetalk](https://me.csdn.net/largetalk)阅读数：103
[vocabulary.cc](http://vocabulary.cc)
存储单词到id的映射
```
class Vocabulary {
 public:
    int GetOrCreateID(string word,bool &created);
    bool GetID(string word, int &id) const;
    bool GetWordByID(int id, string &word) const;
    bool Load(string filename);
    map<string, int> & GetMap();
 private:
    map<string, int> word_ids;
    vector<string> id_words;
    int cur_id;
};
int Vocabulary::GetOrCreateID(string word, bool &created) {
    int id = cur_id;
    auto ret = word_ids.insert(std::pair<string, int>(word, id));
    if (ret.second == false) {
        created = false;
        return ret.first->second;
    } else {
        created = true;
        cur_id++;
        id_words.push_back(word);
        return id;
    }
}
bool Vocabulary::GetID(string word, int &id) const {
    map<string, int>::const_iterator itr = word_ids.find(word);
    if (itr != word_ids.end()) {
        id = itr->second;
        return true;
    }
    return false;
}
bool Vocabulary::GetWordByID(int id, string &word) const {
    if (id >= cur_id) {
        return false;
    }
    word = id_words[id];
    return true;
}
bool Vocabulary::Load(string filename) {
    std::ifstream infile(filename);
    if (infile.fail()) {
        std::cerr << "Vocabulary::Load open file error, file:" << filename
                  << "\n";
        return false;
    }
    string line;
    while (getline(infile, line)) {
        bool created;
        GetOrCreateID(line, created);
        if (!created) {
            std::cerr << "Vocabulary::Load ,duplicated word:" << line << "\n";
            return false;
        }
    }
    infile.close();
    return true;
}
map<string, int> & Vocabulary::GetMap(){
    return word_ids;
}
```
