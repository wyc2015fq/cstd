enum {
  REOP_OR = 256, // |
  REOP_CAT, //
  REOP_STAR, // *
  REOP_ANY, // .
};
#define re_isopt(ch) (((ch) == '*' || (ch) == '|' || (ch) == '^'))
#define re_ischr(ch) (!re_isopt(ch) && (ch) != '(' && (ch) != ')' && (ch) != '\0')
//合法性检验
int re_check(const uchar* expr, int len)
{
  int i, prech = 0, ch;
  for (i = 0; i < len - 1; i++) {
    ch = expr[i];
    if (prech == '(' || prech == '*' || prech == '|' || prech == ')') {
      int t1 = i + 1;
      switch (prech) {
      case '(':
        if (ch == '*' || ch == '|' || ch == ')') {
          return FALSE;
        }
        break;
      case '*':
        if (ch == '*') {
          return FALSE;
        }
        break;
      case '|':
        if (ch == '*' || ch == '|' || ch == ')') {
          return FALSE;
        }
        else if (re_ischr(ch)) {
        }
        else {
          return FALSE;
        }
        break;
      }
    }
    else {
      return FALSE;
    }
    prech = ch;
  }
  return TRUE;
}
int re_to_infix(const uchar* expr, int len, uchar* outexpr, int maxlen)
{
  int i, j = 0, prech = 0;
  for (i = 0; i < len;) {
    uchar ch = expr[i++];
    if (re_ischr(ch)) {
      if (re_ischr(prech) || ')' == prech) {
        outexpr[j++] = '^';
      }
      if ('\\' == ch) {
        if (i < len) {
          ch = expr[i++];
        }
        else {
          return 0;
        }
      }
      outexpr[j++] = ch;
    }
    else if ('(' == ch) {
      if (re_ischr(prech)) {
        outexpr[j++] = '^';
      }
      outexpr[j++] = ch;
    }
    else {
      outexpr[j++] = ch;
    }
    prech = ch;
  }
  outexpr[j++] = '#';
  outexpr[j] = 0;
  return j;
}
// 运算符优先级
// *最高（左结合）
// ^次之
// |最低
uchar re_opt_priority(int a, int b)
{
  int i, j;
  const uchar* c[] = {
    "0#)*|^(",
    "#ex<<<<",
    "(x=<<<<",
    "^>><>><",
    "*>>>>><",
    "|>><><<",
    ")>>>>>x",
  };
  for (i = 1; i < 6; i++)if (c[i][0] == a) {
      break;
    }
  for (j = 1; j < 6; j++)if (c[0][j] == b) {
      break;
    }
  return c[i][j];
}
// 转化成可计算的中缀表达式
// 与  用 ^ 表示
// 或  用 | 表示
// 闭包 用 * 表示
//把链接字符^加上
int re_infix_to_postfix(const uchar* infix, int len, uchar* postfix, int maxlen)
{
  int i, j = 0, ret = 1;
  uchar optstack[256];
  uchar prech = 0, ch = 0;
  int top = 0;
  optstack[top++] = '#';
  for (i = 0; i < len;) {
    ch = infix[i++];
    if (re_ischr(ch) && ch != '#') {
      // 操作数进栈
      postfix[j++] = ch;
    }
    else {  // 运算符
      switch (re_opt_priority(optstack[top - 1], ch)) {
      case '<':// 栈内运算符优先级小于 读入的运算符，进栈
        optstack[top++] = ch;
        break;
      case '>':
        postfix[j++] = optstack[--top];
        i--;
        break;
        // 结束 正常的
      case 'e':
        postfix[j] = 0;
        return j;
        // 结束 不正常的
      case 'x':
        postfix[j] = 0;
        return 0;
      case '=':
        --top;
        break;
      }
    }
  }
  postfix[j] = 0;
  return j;
}
typedef struct EDGE {           // 自动机的边
  int start, end;             // 起点 终点
  uchar input_expr;            // 输入字符
} EDGE;
typedef struct nfa_t {
  EDGE edge[256];
  int start;
  int end;
  int edgelen;
  int stlen;
} nfa_t;
// NFA 显示
int re_print_NFA(const nfa_t* nfa)
{
  int i;
  const EDGE* edge = nfa->edge;
  printf("%d [%d -> %d]\n", nfa->stlen, nfa->start, nfa->end);
  for (i = 0; i < nfa->edgelen; i++) {
    printf("%2d [%2d -> %2d] %c\n", i, edge[i].start, edge[i].end, edge[i].input_expr);
  }
  return 0;
}
#define setedge(x, st, en, input) ((x).start=st,(x).end=en,(x).input_expr=input)
// 将后缀式转化成NFA
int re2nfa(const uchar* expr, int len, nfa_t* nfa)
{
  EDGE s_NFA[256];       // 保存产生的NFA
  int i, k, j = 0, top = 0;
  EDGE l_nfa; // 左操作数
  EDGE r_nfa; // 右操作数
  uchar Regular_Input[256] = {0};
  EDGE* edge = nfa->edge;
  int stage = 0;
  EDGE ed;
  for (i = 0; i < len;) {
    uchar ch = expr[i++];
    if (re_ischr(ch)) { // 操作数 生成NFA 进栈
      // 将输入的字母表保存
      Regular_Input[ch] = 1;
      setedge(ed, stage, stage + 1, ch);
      s_NFA[top++] = ed;
      edge[j++] = ed;
      stage += 2;
    }
    else {               // 运算符 运算
      switch (ch) {
      case '^': // 取出2个运算数运算
        r_nfa = s_NFA[--top]; // 右边运算数
        l_nfa = s_NFA[--top]; // 左边运算数
        for (k = 0; k < j; k++) {
          if (edge[k].start == r_nfa.start) {
            edge[k].start = l_nfa.end;  // 将两条边连接
          }
        }
        setedge(ed, l_nfa.start, r_nfa.end, '$'), s_NFA[top++] = ed;
        break;
      case '|': // 取出2个运算数运算
        r_nfa = s_NFA[--top]; // 右边运算数
        l_nfa = s_NFA[--top]; // 左边运算数
        setedge(ed, stage, l_nfa.start, '$'), edge[j++] = ed;
        setedge(ed, stage, r_nfa.start, '$'), edge[j++] = ed;
        setedge(ed, l_nfa.end, stage + 1, '$'), edge[j++] = ed;
        setedge(ed, r_nfa.end, stage + 1, '$'), edge[j++] = ed;
        setedge(ed, stage, stage + 1, '$'), s_NFA[top++] = ed;
        stage += 2;
        break;
      case '*': // 取出1个运算数运算
        l_nfa = s_NFA[--top]; // 左边运算数
        setedge(ed, stage, l_nfa.start, '$'), edge[j++] = ed;
        setedge(ed, l_nfa.end, stage + 1, '$'), edge[j++] = ed;
        setedge(ed, l_nfa.end, l_nfa.start, '$'), edge[j++] = ed;
        setedge(ed, stage, stage + 1, '$'), edge[j++] = ed;
        s_NFA[top++] = ed;
        stage += 2;
        break;
      }
    }
  }
  nfa->edgelen = j;
  nfa->stlen = 0;
  {
    uchar ss[256] = {0};
    uchar id[256] = {0};
    for (i = 0; i < nfa->edgelen; ++i) {
      ss[nfa->edge[i].start] = 1;
      ss[nfa->edge[i].end] = 1;
    }
    for (j = 0, i = 0; i < 256; ++i) {
      if (ss[i]) {
        ss[i] = j++;
      }
    }
    nfa->stlen = j;
    for (i = 0; i < nfa->edgelen; ++i) {
      nfa->edge[i].start = ss[nfa->edge[i].start];
      nfa->edge[i].end = ss[nfa->edge[i].end];
    }
    // 记录NFA的 起点和终点
    //startNFA.push_back(s_NFA.Top().start);// 记录NFA的 起点
    //endNFA.push_back(s_NFA.Top().end);  // 记录NFA的 终点
    for (i = 0; i < top; ++i) {
      nfa->start = ss[s_NFA[i].start];
      nfa->end = ss[s_NFA[i].end];
    }
  }
  return j;
}
#undef setedge
typedef struct dfa_table_t {
  uchar tlb[50][128];
  uchar end[50];
} dfa_table_t;
int dfa_table_match(dfa_table_t* dfa, const uchar* str, int slen)
{
  int i, ret;
  int currentState, nextState = 0; // 起始状态
  for (i = 0; i < slen; i++) { // 不断读入字符
    currentState = nextState;       // 目前的起点
    nextState = dfa->tlb[currentState][str[i]];
    ret = dfa->end[nextState];
    if (ret) { // 遍历完表了【如果全部匹配，则i等于TestInput的长度】
      return ret;
    }
  }
  return 0;
}
typedef struct dfa_t {
  EDGE edge[256];
  uchar start[50];
  uchar end[50];
  int edgelen;
  int startlen;
  int endlen;
} dfa_t;
int dfa_match(dfa_t* dfa, const uchar* str, int slen)
{
  int i, j, ret;
  int currentState, nextState = 0; // 起始状态
  for (i = 0; i < slen; i++) {
    currentState = nextState;
    for (j = 0; j < dfa->edgelen; ++j) {
      if (dfa->edge[j].start == currentState && dfa->edge[j].input_expr == str[i]) {
        nextState = dfa->edge[j].end;
        ret = dfa->end[nextState];
        if (ret) {
          return ret;
        }
        break;
      }
    }
  }
  return 0;
}
int nfa_move(const nfa_t* nfa, const uchar* input, int in_expr, uchar* output)
{
  int i, j, temp_start = 0, cnt = 0;
  const EDGE* edge = nfa->edge;
  for (i = 0; i < nfa->stlen; i++) {
    if (input[i]) {
      for (j = 0; j < nfa->edgelen; j++) {
        if (edge[j].start == i && edge[j].input_expr == in_expr && !output[edge[j].end]) {
          output[edge[j].end] = 1; // 添加进栈
          ++cnt;
        }
      }
    }
  }
  return cnt;
}
// 查找通过in_expr到达的状态
//   input 起点集合
//   output 输出可到达集合
//   EDGE  边集
int nfa_move_null(const nfa_t* nfa, uchar* input)
{
  int cnt = 0, cntn = 0;
  for (; cnt = nfa_move(nfa, input, '$', input);) {
    cntn += cnt;
  }
  return cnt;
}
#define nfa_find_null_closure(nfa, stset) nfa_move(nfa, stset, '$')
int nfa2dfa(const nfa_t* nfa, dfa_t* dfa)
{
  uchar* sts;
  uchar* sts1;
  int dst[256]; // 所有状态
  int ndst = 0;
  int end[256] = {0}; // 所有终态
  int nend = 0;
  int stlen = nfa->stlen;
  int stsize = 2 * nfa->edgelen * stlen;
  uchar chset[256] = {0};
  int i, ist = 0, tt;
  sts = MALLOC(uchar, stsize);
  MEMSET(sts, 0, stsize);
  sts[nfa->start] = 1;
  nfa_move_null(nfa, sts);
  if (sts[nfa->end]) {
    end[nend++] = ist; // 添加入终态
  }
  dst[ndst++] = ist++;
  //str_printf_arr("%I8u,", stlen, sts, 70);
  //printf("\n");
  for (; ndst > 0;) {
    int cnt, st = dst[--ndst];
    uchar* sts0 = sts + st * stlen;
    int chsetlen = 0;
    for (i = 0; i < nfa->edgelen; ++i) {
      if ('$' != nfa->edge[i].input_expr && sts0[nfa->edge[i].start]) {
        chset[chsetlen++] = nfa->edge[i].input_expr;
      }
    }
    for (i = 0; i < chsetlen; ++i) { // 遍历输入字典
      sts1 = sts + ist * stlen;
      //memcpy(sts1, sts0, stlen);
      cnt = nfa_move(nfa, sts0, chset[i], sts1);
      nfa_move_null(nfa, sts1);
      tt = memfind_block(sts, sts1, ist, stlen, stlen);
      // 将DFA边添加到边集
      //str_printf_arr("%I8u,", stlen, sts1, 70);
      printf("[%d -> %d] %c\n", st, tt < 0 ? ist : tt, chset[i]);
      if (tt < 0) {
        if (sts1[nfa->end]) {
          end[nend++] = ist; // 添加入终态
        }
        dst[ndst++] = ist++;
      }
    }
  }
  str_printf_arr("%u,", nend, end, 70);
  printf(" <- end stage\n");
  FREE(sts);
  return 0;
}
#if 0
//NFA 转化 DFA
int nfa2dfa(const nfa_t* nfa, dfa_t* dfa)
{
  int* DFAStateGather;      // DFA状态集合
  int* NFA_state;        // 所有状态
  int* DFA_state;        // 所有状态
  int* result;             // 临时状态集合
  Find_NULL_Closure(startNFA, result, NFA_Edge); // 计算从起点经过$可到达的集合
  DFA_state.Add(result);            // 状态集合压栈
  NFA_state.push(result);
  DFAStateGather.push_back(result);      // 状态添加
  startDFA.push_back(GetState_Gather(DFAStateGather, result));   // 起点
  ////////////////////记录终态 和 非终态////////////////////////////////////////////
  int m;
  for (m = 0; m < endNFA.size(); m++) {
    if (find(result.begin(), result.end(), endNFA[m]) != result.end()) { // 找到了
      break;
    }
  }
  if (m < endNFA.size()) {
    endDFA.push_back(GetState_Gather(DFAStateGather, result)); // 添加入终态
  }
  else {
    nonEndDFA.push_back(GetState_Gather(DFAStateGather, result)); // 添加入 非终态
  }
  ////////////////////记录终态 和 非终态////////////////////////////////////////////
  while (!NFA_state.empty()) {         // 遍历队列找出所有可到达的状态集合
    vector<int> temp;     // 临时状态集合
    temp = NFA_state.front(); // 取出队列中的头元素
    NFA_state.pop();      // 删除列头元素
    for (int i = 0; i < Regular_Input.size(); i++) {  // 遍历输入字典
      vector<int> result;               // 临时状态集合
      result.clear();                 // 清空
      Move(temp, Regular_Input[i], result , NFA_Edge); // 查找当前集合 通过 可到达的集合
      if (result.size() > 0 && GetState_Gather(DFAStateGather, result) == 0) { // 如果 DFAStateGather中没有此状态 添加入DFAStateGather并入队列
        DFA_state.Add(result);       // 新NFA状态 进栈
        NFA_state.push(result);      // 新状态 进队列
        DFAStateGather.push_back(result); // 状态添加
        ////////////////////记录终态 和 非终态////////////////////////////////////////////
        for (m = 0; m < endNFA.size(); m++) {
          if (find(result.begin(), result.end(), endNFA[m]) != result.end()) { // 找到了
            break;
          }
        }
        if (m < endNFA.size()) {
          endDFA.push_back(GetState_Gather(DFAStateGather, result)); // 如果含有NFA终态  就把它添加入 DFA终态
        }
        else {
          nonEndDFA.push_back(GetState_Gather(DFAStateGather, result)); // 否则添加入 非终态
        }
        ////////////////////记录终态 和 非终态////////////////////////////////////////////
      }
      if (result.size() > 0) {        // 将DFA边添加到边集
        EDGE edge;             // DFA边
        edge.start = GetState_Gather(DFAStateGather, temp);
        edge.input_expr = Regular_Input[i];
        edge.end = GetState_Gather(DFAStateGather, result);
        DFA_Edge.push_back(edge);
      }
    }
  }
  return 0;
}
#endif

