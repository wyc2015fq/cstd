#include "cstd.h"
#include "str.h"
#include "cfile.h"
#if 0
Java实现双数组Trie树(DoubleArrayTrie, DAT)
传统的Trie实现简单，但是占用的空间实在是难以接受，特别是当字符集不仅限于英文26个字符的时候，爆炸起来的空间根本无法接受。双数组Trie就是优化了空间的Trie树，原理本文就不讲了，请参考An Efficient Implementation of Trie Structures，本程序的编写也是参考这篇论文的。
传统的Trie实现简单，但是占用的空间实在是难以接受，特别是当字符集不仅限于英文26个字符的时候，爆炸起来的空间根本无法接受。
双数组Trie就是优化了空间的Trie树，原理本文就不讲了，请参考An Efficient Implementation of Trie Structures，本程序的编写也是参考这篇论文的。
关于几点论文没有提及的细节和与论文不一一致的实现：
1.对于插入字符串，如果有一个字符串是另一个字符串的子串的话，我是将结束符也作为一条边，产生一个新的结点，这个结点新节点的Base我置为0
所以一个字符串结束也有2中情况：一个是Base值为负，存储剩余字符(可能只有一个结束符)到Tail数组；另一个是Base为0。
所以在查询的时候要考虑一下这两种情况
2.对于第一种冲突（论文中的Case 3），可能要将Tail中的字符串取出一部分，作为边放到索引中。论文是使用将尾串左移的方式，我的方式直接修改Base值，而不是移动尾串。
下面是java实现的代码，可以处理相同字符串插入，子串的插入等情况
下面是测试结果，构造6W英文单词的DAT，大概需要20秒
我增长数组的时候是每次长度增加到2倍，初始1024
Base和Check数组的长度为131072
Tail的长度为262144
#endif
#define END_CHAR '\0'
#define DEFAULT_LEN 1024
typedef struct datrie_t {
  int* Base;
  int* Check;
  uchar* Tail;
  int Base_length;
  int Check_length;
  int Tail_length;
  int Pos;// = 1
  uchar CharMap[256];
  int CharMap_size;
  //int* CharList;
} datrie_t;
void datrie_free(datrie_t* tr)
{
  FREE(tr->Base);
  FREE(tr->Check);
  FREE(tr->Tail);
}
void datrie_init(datrie_t* tr)
{
  int i;
  tr->Pos = 1;
  tr->Base_length = tr->Check_length = tr->Tail_length = DEFAULT_LEN;
  MYREALLOC(tr->Base, tr->Base_length);
  MYREALLOC(tr->Check, tr->Check_length);
  MYREALLOC(tr->Tail, tr->Tail_length);
  //MYREALLOC(tr->CharList, tr->Check_length);
  tr->Base[1] = 1;
  MEMSET(tr->CharMap, 0, 256);
  tr->CharMap[END_CHAR] = ++tr->CharMap_size;
  //CharList.add(END_CHAR);
  //CharList.add(END_CHAR);
  for (i = 0; i < 26; ++i) {
    tr->CharMap['a' + i] = ++tr->CharMap_size;
    //CharList.add((char)('a' + i));
  }
}
void datrie_extend_array(datrie_t* tr)
{
  tr->Base_length *= 2;
  tr->Check_length *= 2;
  MYREALLOC(tr->Base, tr->Base_length);
  MYREALLOC(tr->Check, tr->Check_length);
}
void datrie_extend_tail(datrie_t* tr)
{
  tr->Tail_length *= 2;
  MYREALLOC(tr->Tail, tr->Tail_length);
}
int datrie_getcharcode(datrie_t* tr, uchar c)
{
  if (!tr->CharMap[(uchar)c]) {
    tr->CharMap[(uchar)c] = ++tr->CharMap_size;
  }
  return tr->CharMap[(uchar)c];
}
int datrie_CopyToTailArray(datrie_t* tr, const char* s, int s_length, int p)
{
  int i, _Pos = tr->Pos;
  while (s_length - p + 1 > tr->Tail_length - tr->Pos) {
    datrie_extend_tail(tr);
  }
  for (i = p; i < s_length; ++i) {
    tr->Tail[_Pos] = s[i];
    _Pos++;
  }
  return _Pos;
}
int datrie_x_check(datrie_t* tr, const int* set, int set_length)
{
  int i, j;
  for (i = 1; ; ++i) {
    bool flag = true;
    for (j = 0; j < set_length; ++j) {
      int cur_p = i + set[j];
      if (cur_p >= tr->Base_length) {
        datrie_extend_array(tr);
      }
      if (tr->Base[cur_p] != 0 || tr->Check[cur_p] != 0) {
        flag = false;
        break;
      }
    }
    if (flag) {
      return i;
    }
  }
}
int datrie_GetChildList(datrie_t* tr, int p, int* ret)
{
  int i, j = 0;
  for (i = 1; i <= tr->CharMap_size; ++i) {
    if (tr->Base[p] + i >= tr->Check_length) {
      break;
    }
    if (tr->Check[tr->Base[p] + i] == p) {
      ret[j++] = (i);
    }
  }
  return j;
}
bool datrie_TailContainString(datrie_t* tr, int start, const uchar* s2, int s2_length)
{
  int i;
  for (i = 0; i < s2_length; ++i) {
    if (s2[i] != tr->Tail[i + start]) {
      return false;
    }
  }
  return true;
}
bool datrie_TailMatchString(datrie_t* tr, int start, const char* s2, int s2_length)
{
  int i;
  for (i = 0; i < s2_length; ++i) {
    if (s2[i] != tr->Tail[i + start]) {
      return false;
    }
  }
  if (END_CHAR != tr->Tail[i + start]) {
    return false;
  }
  return true;
}
void datrie_insert(datrie_t* tr, const char* s0, int s_length)
{
  int i, pre_p = 1, cur_p;
  uchar s[256];
  s_length = s_length < 0 ? strlen(s0) : s_length;
  ASSERT(s_length < 256);
  memcpy(s, s0, s_length);
  s[s_length++] = END_CHAR;
  for (i = 0; i < s_length; ++i) {
    //获取状态位置
    cur_p = tr->Base[pre_p] + datrie_getcharcode(tr, s[i]);
    //如果长度超过现有，拓展数组
    if (cur_p >= tr->Base_length) {
      datrie_extend_array(tr);
    }
    //空闲状态
    if (tr->Base[cur_p] == 0 && tr->Check[cur_p] == 0) {
      tr->Base[cur_p] = -tr->Pos;
      tr->Check[cur_p] = pre_p;
      tr->Pos = datrie_CopyToTailArray(tr, s, s_length, i + 1);
      break;
    }
    else {
      //已存在状态
      if (tr->Base[cur_p] > 0 && tr->Check[cur_p] == pre_p) {
        pre_p = cur_p;
        continue;
      }
      else {
        //冲突 1：遇到 Base[cur_p]小于0的，即遇到一个被压缩存到Tail中的字符串
        if (tr->Base[cur_p] < 0 && tr->Check[cur_p] == pre_p) {
          int head = -tr->Base[cur_p];
          if (s[i + 1] == END_CHAR && tr->Tail[head] == END_CHAR) { //插入重复字符串
            break;
          }
          //公共字母的情况，因为上一个判断已经排除了结束符，所以一定是2个都不是结束符
          if (tr->Tail[head] == s[i + 1]) {
            int cc = datrie_getcharcode(tr, s[i + 1]);
            int avail_base = datrie_x_check(tr, &cc, 1);
            tr->Base[cur_p] = avail_base;
            tr->Check[avail_base + datrie_getcharcode(tr, s[i + 1])] = cur_p;
            tr->Base[avail_base + datrie_getcharcode(tr, s[i + 1])] = -(head + 1);
            pre_p = cur_p;
            continue;
          }
          else {
            //2个字母不相同的情况，可能有一个为结束符
            int avail_base;
            int cc[2] = {datrie_getcharcode(tr, s[i + 1]), datrie_getcharcode(tr, tr->Tail[head]) };
            avail_base = datrie_x_check(tr, cc, 2);
            tr->Base[cur_p] = avail_base;
            tr->Check[avail_base + datrie_getcharcode(tr, tr->Tail[head])] = cur_p;
            tr->Check[avail_base + datrie_getcharcode(tr, s[i + 1])] = cur_p;
            //Tail 为END_FLAG 的情况
            if (tr->Tail[head] == END_CHAR) {
              tr->Base[avail_base + datrie_getcharcode(tr, tr->Tail[head])] = 0;
            }
            else {
              tr->Base[avail_base + datrie_getcharcode(tr, tr->Tail[head])] = -(head + 1);
            }
            if (s[i + 1] == END_CHAR) {
              tr->Base[avail_base + datrie_getcharcode(tr, s[i + 1])] = 0;
            }
            else {
              tr->Base[avail_base + datrie_getcharcode(tr, s[i + 1])] = -tr->Pos;
            }
            tr->Pos = datrie_CopyToTailArray(tr, s, s_length, i + 2);
            break;
          }
        }
        else {
          //冲突2：当前结点已经被占用，需要调整pre的base
          if (tr->Check[cur_p] != pre_p) {
            int toBeAdjust;
            int list1[256];
            int list[256];
            int list_length;
            int list1_length = datrie_GetChildList(tr, pre_p, list1);
            int origin_base, avail_base, j, k;
            if (true) {
              toBeAdjust = pre_p;
              MEMCPY(list, list1, list1_length);
              list_length = list1_length;
            }
            origin_base = tr->Base[toBeAdjust];
            list[list_length++] = datrie_getcharcode(tr, s[i]);
            avail_base = datrie_x_check(tr, list, list_length);
            list_length--;
            tr->Base[toBeAdjust] = avail_base;
            for (j = 0; j < list_length; ++j) {
              //BUG
              int tmp1 = origin_base + list[j];
              int tmp2 = avail_base + list[j];
              tr->Base[tmp2] = tr->Base[tmp1];
              tr->Check[tmp2] = tr->Check[tmp1];
              //有后续
              if (tr->Base[tmp1] > 0) {
                int subsequence[256];
                int subsequence_length = datrie_GetChildList(tr, tmp1, subsequence);
                for (k = 0; k < subsequence_length; ++k) {
                  tr->Check[tr->Base[tmp1] + subsequence[k]] = tmp2;
                }
              }
              tr->Base[tmp1] = 0;
              tr->Check[tmp1] = 0;
            }
            //更新新的cur_p
            cur_p = tr->Base[pre_p] + datrie_getcharcode(tr, s[i]);
            if (s[i] == END_CHAR) {
              tr->Base[cur_p] = 0;
            }
            else {
              tr->Base[cur_p] = -tr->Pos;
            }
            tr->Check[cur_p] = pre_p;
            tr->Pos = datrie_CopyToTailArray(tr, s, s_length, i + 1);
            break;
          }
        }
      }
    }
  }
  return ;
}
bool datrie_exists(datrie_t* tr, const char* word0, int word_length)
{
  int pre_p = 1;
  int cur_p = 0;
  int i;
  const uchar* word = (const uchar*)word0;
  word_length = word_length < 0 ? strlen(word0) : word_length;
  for (i = 0; i < word_length; ++i) {
    cur_p = tr->Base[pre_p] + datrie_getcharcode(tr, word[i]);
    if (tr->Check[cur_p] != pre_p) {
      return false;
    }
    if (tr->Base[cur_p] < 0) {
      if (datrie_TailMatchString(tr, -tr->Base[cur_p], word + i + 1, word_length - i - 1)) {
        return true;
      }
      return false;
    }
    pre_p = cur_p;
  }
  if (tr->Check[tr->Base[cur_p] + datrie_getcharcode(tr, END_CHAR)] == cur_p) {
    return true;
  }
  return false;
}
//内部函数，返回匹配单词的最靠后的Base index
int datrie_find(datrie_t* tr, const char* word, int word_length, char* prefix)
{
  int pre_p = 1;
  int cur_p = 0;
  int i, j = 0, p = -1;
  for (i = 0; i < word_length; ++i) {
    // BUG
    if (prefix) {
      prefix[j++] += word[i];
      prefix[j] = 0;
    }
    cur_p = tr->Base[pre_p] + datrie_getcharcode(tr, (uchar)word[i]);
    if (tr->Check[cur_p] != pre_p) {
      p = -1;
      return p;
    }
    if (tr->Base[cur_p] < 0) {
      if (datrie_TailContainString(tr, -tr->Base[cur_p], word + i + 1, word_length - i - 1)) {
        p = cur_p;
        return p;
      }
      p = -1;
      return p;
    }
    pre_p = cur_p;
  }
  p = cur_p;
  return p;
}
#if 0
ArrayList<const char*> GetAllChildWord(int index)
{
  int i;
  ArrayList<const char*> result = new ArrayList<const char*>();
  if (Base[index] == 0) {
    result.add("");
    return result;
  }
  if (Base[index] < 0) {
    const char* r = "";
    for (int i = -Base[index]; Tail[i] != END_CHAR; ++i) {
      r += Tail[i];
    }
    result.add(r);
    return result;
  }
  for (i = 1; i <= CharMap_size(); ++i) {
    if (Check[Base[index] + i] == index) {
for (const char * s: GetAllChildWord(Base[index] + i)) {
        result.add(CharList.get(i) + s);
      }
      //result.addAll(GetAllChildWord(Base[index]+i));
    }
  }
  return result;
}
public ArrayList<const char*> FindAllWords(const char* word)
{
  ArrayList<const char*> result = new ArrayList<const char*>();
  const char* prefix = "";
  FindStruct fs = datrie_find(word);
  int p = fs.p;
  if (p == -1) {
    return result;
  }
  if (Base[p] < 0) {
    const char* r = "";
    for (int i = -Base[p]; Tail[i] != END_CHAR; ++i) {
      r += Tail[i];
    }
    result.add(fs.prefix + r);
    return result;
  }
  if (Base[p] > 0) {
    ArrayList<const char*> r = GetAllChildWord(p);
    for (int i = 0; i < r_size(); ++i) {
      r.set(i, fs.prefix + r.get(i));
    }
    return r;
  }
  return result;
}
#endif
int test_datrie()
{
  int i;
  vstr_t sv[1] = {0};
  datrie_t tr[1] = {0};
  _chdir("E:/book_txt/词典");
  vstr_load("牛津高阶英语词典第8版词表.txt", sv);
  datrie_init(tr);
  for (i = 0; i < sv->n; ++i) {
    datrie_insert(tr, sv->v[i].s, -1);
  }
  printf("Base_length = %d\n", tr->Base_length);
  printf("Tail_length = %d\n", tr->Tail_length);
  for (i = 0; i < sv->n; ++i) {
    ASSERT(datrie_exists(tr, sv->v[i].s, -1));
  }
  return 0;
}

