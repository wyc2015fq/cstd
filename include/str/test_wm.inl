// 多模式匹配时，怎样有效地将大文本读入字符串并进行处理？
// 想要用WM算法进行多模式匹配：（这是一个纯拼速度的作业）
// 1）要进行匹配的txt有100-1000个，总大小在10个G以内，内存4G，要怎么比较快地读文件？
// 题目中给了一个目标字符串的target.txt，一个含有所有文件名（这些文件都是都是要被搜索的）的filename.txt，和若干个被搜索的txt。
// 2）要求可以做预编译，但是预编译时不能读模式串，那么这个时候还有必要进行预编译么？
// WM.h是我在网上搜到的：（说实话我没办法完全看懂所以只能凑合着用）
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#ifndef _WMDLL_H_
#define _WMDLL_H_
using namespace std;
struct HASH_NODE {
  string* str;
  HASH_NODE* next;
  ~HASH_NODE() {}
};
struct HASH_NODE_INT {
  int offset;
  HASH_NODE_INT* next;
  ~HASH_NODE_INT() {}
};
class WM
{
public:
  WM(vector<string>& _pattern, int BLOCK_SIZE) {
    pattern = _pattern;
    B = BLOCK_SIZE;
    patt_count = pattern.size();
    //pattern_length = new int(patt_count);
    m = pattern[0].length();
    for (int i = 0; i < patt_count; ++i) {
      if (m > pattern[i].length()) {
        m = pattern[i].length();
      }
    }
    MATCH_ENTRY = -1;
  }
  ~WM() {
    map<string, HASH_NODE*>::iterator it = HASH.begin();
    while (it != HASH.end()) {
      HASH_NODE* hn = it->second;
      HASH_NODE* tmp = NULL;
      while (hn) {
        tmp = hn;
        hn = hn->next;
        delete tmp;
      }
      delete hn;
      ++it;
    }
    map<string, HASH_NODE_INT*>::iterator iter = match_offset.begin();
    while (iter != match_offset.end()) {
      HASH_NODE_INT* hni = iter->second;
      HASH_NODE_INT* tmp = NULL;
      while (hni) {
        tmp = hni;
        hni = hni->next;
        delete tmp;
      }
      delete hni;
      ++iter;
    }
  }
  void make_table() {
    vector<string>::iterator iter = pattern.begin();
    int jump = 0;
    while (iter != pattern.end()) {
      for (int i = 0; i < m - B + 1; ++i) {
        string sub_pt(*iter, i, B);
        //make shift table
        jump = (m - B - i == 0) ? MATCH_ENTRY : m - B - i;
        if (!SHIFT[sub_pt]) {
          SHIFT[sub_pt] = jump;
        }
        else {
          SHIFT[sub_pt] = SHIFT[sub_pt] < jump ? SHIFT[sub_pt] : jump;
        }
        // make prefix table
        if (i == 0) {
          string prefix(*iter, 0, B);
          PREFIX[*iter] = prefix;
        }
        // make hash table
        if (i == m - B) { // the last block of match window
          // calc hash list
          HASH_NODE* hn = new HASH_NODE();
          hn->str = &(*iter);
          hn->next = NULL;
          if (!HASH[sub_pt]) {
            HASH[sub_pt] = hn;
          }
          else {
            hn->next = HASH[sub_pt];
            HASH[sub_pt] = hn;
          }
        }
      }
      iter++;
    }
  }
  void show_shift() {
    cout << "============== SHIFT TABLE ==============" << endl;
    map<string, int>::iterator iter = SHIFT.begin();
    int jump;
    while (iter != SHIFT.end()) {
      jump = (iter->second == MATCH_ENTRY) ? iter->second - MATCH_ENTRY : iter->second;
      cout << iter->first << " " << jump << endl;
      iter++;
    }
  }
  void show_hash() {
    cout << "============== HASH TABLE ==============" << endl;
    map<string, HASH_NODE*>::iterator iter = HASH.begin();
    while (iter != HASH.end()) {
      cout << iter->first << endl;
      HASH_NODE* hn = iter->second;
      while (hn) {
        cout << " " << *hn->str << endl;
        hn = hn->next;
      }
      iter++;
    }
  }
  void show_prefix() {
    cout << "============== PREFIX TABLE ==============" << endl;
    map<string, string>::iterator iter = PREFIX.begin();
    while (iter != PREFIX.end()) {
      cout << iter->first << endl << " " << iter->second << endl;
      ++iter;
    }
  }
  void show_match() {
    //cout<<"================ match summary ================"<<endl;
    cout << "KEY\tCOUNT\tOFFSET" << endl;
    map<string, HASH_NODE_INT*>::iterator iter = match_offset.begin();
    while (iter != match_offset.end()) {
      cout << "[" << iter->first << "]\t" << match_count[iter->first] << "\t";
      HASH_NODE_INT* hni = iter->second;
      while (hni) {
        cout << hni->offset << "\t";
        hni = hni->next;
      }
      ++iter;
      cout << endl;
    }
  }
  void match(string& _text) {
    string text = _text;
    int ptr = m - 1; // text ptr, pointing at end of pattern window
    while (ptr < text.length()) {
      string sub_pt(text, ptr - 1, B);
      // match entry of a pattern
      //cout<<"DEBUG checking: "<<sub_pt<<" "<<SHIFT[sub_pt]<<endl;
      if (SHIFT[sub_pt] == MATCH_ENTRY) {
        // prefix in this match window
        string prefix(text, ptr - (m - B) - 1, B);
        HASH_NODE* hn = HASH[sub_pt];
        while (hn) {
          if ((PREFIX[*hn->str].compare(prefix)) == 0) {
            // try full match of the pattern
            string _window(text, ptr - m + 1, hn->str->length());
            if (((*hn->str).compare(_window)) == 0) {
              // log the match results
              HASH_NODE_INT* hni = new HASH_NODE_INT();
              hni->offset = ptr - m + 1;
              hni->next = NULL;
              if (!match_offset[_window]) {
                match_offset[_window] = hni;
                match_count[_window] = 1;
              }
              else {
                hni->next = match_offset[_window];
                match_offset[_window] = hni;
                match_count[_window] += 1;
              }
            }
            else {
            }
          }
          hn = hn->next;
        }
        // in classic wm algo, after match or miss match
        // can only move one step forward
        // there are various enhancement on this topic
        ptr++;
      }
      // no a match entry, safe to move forward
      else {
        if (!SHIFT[sub_pt]) {
          ptr += m - 1; // no such block in pattern
        }
        else {
          ptr += SHIFT[sub_pt]; // existen block in pattern
        }
      }
    }
  }
private:
  vector<string> pattern; // the pattern list
  map<string, int> SHIFT; // shift table
  map<string, HASH_NODE*> HASH; // hash table
  map<string, string> PREFIX; // prefix table
  map<string, HASH_NODE_INT*> match_offset; // store the match offset
  map<string, int> match_count; // store the match count
  int B; // block size
  int m; // min pattern size
  int patt_count;
  int* pattern_length;
  int MATCH_ENTRY; // undefiend map value equal to 0 so need define another value to mark the match entry which originally marked as 0
};
#endif /* _WMDLL_H_ */

