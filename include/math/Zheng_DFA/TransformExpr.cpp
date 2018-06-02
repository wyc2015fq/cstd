#include "stdafx.h"
#include "TransformExpr.h"
TransformExpr::TransformExpr()
{
  expr = "";
  state = 1;
}
void TransformExpr::SetExpr(CString m_expr)
{
  expr = m_expr;
  state = 1;
}
CString TransformExpr::GetExpr()
{
  return expr;
}
void TransformExpr::ClearAll()
{
  Regular_Input.clear();           // 正规式的输入符
  startNFA.clear();                // 记录NFA的   起始  状态集
  endNFA.clear();                  // 记录NFA的   终    状态集
  NFA_Edge.clear();                // 保存NFA所有边
  //////////////////////////////////////////////////////////////////////////////
  startDFA.clear();                // 记录DFA的起始状态集   含有 NFA 起点的集合
  endDFA.clear();                  // 记录DFA的终态集       含有 NFA 终点的集合（或称接收状态组）
  nonEndDFA.clear();               // 非中态集（或称非接收状态组）
  DFA_Edge.clear();                // 由NFA所构造成的DFA的边的集合
  //////////////////////////////////////////////////////////////////////////////
  MinimizeStartDFA.clear();        // 记录最小化DFA的起始
  MinimizeEndDFA.clear();          // 最小化后DFA的终态集
  MinimizeNonEndDFA.clear();       // 最小化后DFA的非终态集
  MinimizeDFA_Edge.clear();        // 最小化DFA 边集
  MinimizeDFAStateGather.clear();  // 最小化DFA状态集合
}
// 输入的表达式到  后缀表达式
/*
* 检验合法性
*   用户直接输入的
*/
bool TransformExpr::IsLegal(CString m_expr)   //合法性检验
{
  /*bool legal = true;*/
  int len = m_expr.GetLength();

  for (int i = 0; i < len; i++) {
    if (m_expr[i] == '(' || m_expr[i] == '*' || m_expr[i] == '|' || m_expr[i] == ')' || (type(m_expr[i]) == 0)) {
      int temp = i + 1;

      switch (m_expr[i]) {
      case '(':
        if (m_expr[temp] == '*' || m_expr[temp] == '|' || m_expr[temp] == ')') {
          return false;
        }

        break;

      case '*':
        if (m_expr[temp] == '*') {
          return false;
        }

        break;

      case '|':
        if (m_expr[temp] == '*' || m_expr[temp] == '|' || m_expr[temp] == ')') {
          return false;
        }
        else if (type(m_expr[temp]) == 0)
        {}
        else {
          return false;
        }

        break;
      }
    }
    else {
      return false;
    }
  }

  return true;
}


/*
* 转化成可计算的正则表达式
*  “与”   用 ^ 表示
*  “或”   用 | 表示
*  “闭包” 用 * 表示
*   把链接字符^加上
*/
CString TransformExpr::ToRegular_expr(CString m_expr)
{
  bool legal = true;
  int len = m_expr.GetLength();
  CString Regular_expr = m_expr;            // 正则表达式

  if (len > 1) {
    for (int i = 1; i < len; i++) {
      if (type(Regular_expr[i]) == 0) { // 操作符
        if (Regular_expr[i - 1] != '(' && Regular_expr[i - 1] != '|') {
          Regular_expr.Insert(i, "^");
          len++;
          i++;
        }
      }

      if (Regular_expr[i] == '(') {
        if (Regular_expr[i - 1] == ')' ||  type(Regular_expr[i - 1]) == 0) {
          Regular_expr.Insert(i, "^");
          len++;
          i++;
        }
      }
    }
  }

  return Regular_expr;
}

// 将正则表达式转化为 后缀表达式
CString TransformExpr::ToPostfix()
{
  LinkedStack<CString> s_Figure;    // 操作数栈
  LinkedStack<CString> Operator;    // 运算符栈
  CString input_expr = expr;
  CString output_expr = "";

  input_expr += "#";   // 将结束符#添加到末尾
  Operator.Add("#");   // 首先将#压栈

  int len = input_expr.GetLength();

  for (int i = 0; i < len; i ++) {
    if (type(input_expr[i]) == 0 && input_expr[i] != '#') { // 操作数进栈
      output_expr += input_expr.Mid(i, 1);  // 输出后缀表达式
    }
    else { // 运算符
      CString top_expr = Operator.Top();       // 栈顶运算符
      CString temp_expr = input_expr.Mid(i, 1); // 当前字符
      CString delete_expr = "";
      char top_char = top_expr[0];   //CString 转 char
      char temp_char = temp_expr[0]; //CString 转 char

      switch (OperatePriority(top_char, temp_char)) {
      case '<':// 栈内运算符优先级小于  读入的运算符，进栈
        Operator.Add(temp_expr);
        break;

      case '>':
        Operator.Delete(delete_expr);
        output_expr += delete_expr;
        i--;
        break;

      case 'e':   // 结束 正常的
        break;
        break;

      case 'x':   // 结束 不正常的
        output_expr = "ERROR";
        break;
        break;

      case '=':
        Operator.Delete(delete_expr);
        break;
      }
    }
  }

  return output_expr;
}
//--------------------- 输入的表达式到  后缀表达式---------------------------------------------------------------------

//--------------------- 后缀表达式到 NFA ------------------------------------------------------------------------------
// 将后缀式转化成NFA
void TransformExpr::Regular_exprToNFA(const char* m_Regular_expr)
{
  int len = strlen(m_Regular_expr);
  LinkedStack<NFA>  s_NFA;              // 保存产生的NFA

  NFA left_nfa;  // 左操作数
  NFA right_nfa; // 右操作数
  NFA result;    // 保存运算结果

  for (int i = 0; i < len; i++) {
    if (type(m_Regular_expr[i]) == 0) { // 操作数 生成NFA 进栈
      ////////////  STD::find用法
      //find（v.begin（）,v.end（）,7）!= v.end（）;   //找到
      //find（v.begin（）,v.end（）,4）== v.end（））; //未找到
      //   STD::find用法
      if (find(Regular_Input.begin(), Regular_Input.end(), m_Regular_expr[i]) == Regular_Input.end()) {
        Regular_Input.push_back(m_Regular_expr[i]);  // 未找到 将输入的字母表保存
      }

      NFA temp;                       // 临时变量生成NFA
      temp.start = 0;
      temp.end = 0;
      MakeNFA_Self(m_Regular_expr[i], &temp, NFA_Edge);
      s_NFA.Add(temp);
    }
    else {                              // 运算符 运算
      switch (m_Regular_expr[i]) {
      case '|':  // 取出2个运算数运算
        s_NFA.Delete(right_nfa); // 右边运算数
        s_NFA.Delete(left_nfa);  // 左边运算数
        OperateRule_Binary(left_nfa, m_Regular_expr[i], right_nfa, &result, NFA_Edge);
        s_NFA.Add(result);
        break;

      case '^':  // 取出2个运算数运算
        s_NFA.Delete(right_nfa); // 右边运算数
        s_NFA.Delete(left_nfa);  // 左边运算数
        OperateRule_Binary(left_nfa, m_Regular_expr[i], right_nfa, &result, NFA_Edge);
        s_NFA.Add(result);
        break;

      case '*':  // 取出1个运算数运算
        s_NFA.Delete(left_nfa);  // 左边运算数
        OperateRule_Unary(left_nfa, m_Regular_expr[i], &result, NFA_Edge);
        s_NFA.Add(result);
        break;
      }
    }
  }

  if (!s_NFA.IsEmpty()) {                   // 记录NFA的 起点和终点
    startNFA.push_back(s_NFA.Top().start);// 记录NFA的 起点
    endNFA.push_back(s_NFA.Top().end);    // 记录NFA的 终点
  }
}
//--------------------- 后缀表达式到 NFA ------------------------------------------------------------------------------

//--------------------- NFA 到 DFA ------------------------------------------------------------------------------------
//NFA 转化 DFA
void TransformExpr::NFA_To_DFA()
{
  vector<vector<int> > DFAStateGather;          // DFA状态集合
  queue<vector<int> > NFA_state;                // 所有状态
  LinkedStack<vector<int> > DFA_state;          // 所有状态
  vector<int> result;                          // 临时状态集合

  Find_NULL_Closure(startNFA, result, NFA_Edge); // 计算从起点经过$可到达的集合

  DFA_state.Add(result);                       // 状态集合压栈
  NFA_state.push(result);
  DFAStateGather.push_back(result);            // 状态添加
  startDFA.push_back(GetState_Gather(DFAStateGather, result));     // 起点
  ////////////////////记录终态 和 非终态////////////////////////////////////////////
  int m;

  for (m = 0; m < endNFA.size(); m++) {
    if (find(result.begin(), result.end(), endNFA[m]) != result.end()) { // 找到了
      break;
    }
  }

  if (m < endNFA.size()) {
    endDFA.push_back(GetState_Gather(DFAStateGather, result));  // 添加入  终态
  }
  else {
    nonEndDFA.push_back(GetState_Gather(DFAStateGather, result));  // 添加入  非终态
  }

  ////////////////////记录终态 和 非终态////////////////////////////////////////////
  while (!NFA_state.empty()) {                 // 遍历队列 找出所有可到达的状态集合
    vector<int> temp;          // 临时状态集合
    temp = NFA_state.front();  // 取出队列中的头元素
    NFA_state.pop();           // 删除列头元素

    for (int i = 0; i < Regular_Input.size(); i++) {     // 遍历输入字典
      vector<int> result;                              // 临时状态集合
      result.clear();                                  // 清空
      Move(temp, Regular_Input[i], result , NFA_Edge); // 查找当前集合 通过 可到达的集合

      if (result.size() > 0 && GetState_Gather(DFAStateGather, result) == 0) { // 如果 DFAStateGather中没有此状态  添加入DFAStateGather并入队列
        DFA_state.Add(result);             // 新NFA状态 进栈
        NFA_state.push(result);            // 新状态 进队列
        DFAStateGather.push_back(result);  // 状态添加

        ////////////////////记录终态 和 非终态////////////////////////////////////////////
        for (m = 0; m < endNFA.size(); m++) {
          if (find(result.begin(), result.end(), endNFA[m]) != result.end()) { // 找到了
            break;
          }
        }

        if (m < endNFA.size()) {
          endDFA.push_back(GetState_Gather(DFAStateGather, result));  // 如果含有NFA终态   就把它添加入  DFA终态
        }
        else {
          nonEndDFA.push_back(GetState_Gather(DFAStateGather, result));  // 否则添加入  非终态
        }

        ////////////////////记录终态 和 非终态////////////////////////////////////////////
      }

      if (result.size() > 0) {               // 将DFA边添加到边集
        EDGE edge;                         // DFA边
        edge.start = GetState_Gather(DFAStateGather, temp);
        edge.input_expr = Regular_Input[i];
        edge.end = GetState_Gather(DFAStateGather, result);
        DFA_Edge.push_back(edge);
      }
    }
  }
}
// 最小化DFA
void TransformExpr::MinimizeDFA()
{
  queue<vector<int> >  que_MinimizeDFAGather;     // 队列  保存产生的中间状态集合
  MinimizeDFAStateGather.push_back(endDFA);      // 添加终态    --[添加集合原则：最先添加状态少的集合]
  que_MinimizeDFAGather.push(endDFA);            // 添加终态
  MinimizeDFAStateGather.push_back(nonEndDFA);   // 添加非终态  {{终态}，{非终态}}
  que_MinimizeDFAGather.push(nonEndDFA);         // 添加非终态

  //--------- 利用可区分概念进行划分  找出一样的状态 --------------------
  while (!que_MinimizeDFAGather.empty()) {
    vector<int> temp_state;
    temp_state = que_MinimizeDFAGather.front();                  // 取出队列头部的数据 临时保存
    que_MinimizeDFAGather.pop();                                 // 删除
    bool IsSplitable = false;                                     // 判断 读入的字符能否区分

    if (temp_state.size() > 1) {                                 // 数量大于1  才有必要分
      int current_pos = 0;                                     // 保存当前集合  在总集合中的位置

      for (int index = 0; index < MinimizeDFAStateGather.size(); index++) {
        if (temp_state == MinimizeDFAStateGather[index]) {
          current_pos = index;
          break;
        }
      }

      for (int k = 0; k < Regular_Input.size() && !IsSplitable; k++) { // 输入的字符进行分离（如果A能分离 就不用B）
        vector<vector<int> > GoadGather;                      // 输入字符后 所有状态到达的集合
        GoadGather.clear();

        for (int j = 0; j < temp_state.size(); j++) {        // 遍历要拆分的集合 元素
          GoadGather.resize(MinimizeDFAStateGather.size() + temp_state.size() + 1);      // 重新分配空间  最多是每个状态都不同
          int GoadState = 0;
          GoadState = GetDFAState_Gather(MinimizeDFAStateGather, temp_state[j], Regular_Input[k]);

          if (GoadState >= 0) {
            GoadGather[GoadState].push_back(temp_state[j]);
          }
          else {                                           // 遇到没此输入  所以目标状态为空  开辟新空间
            GoadGather[MinimizeDFAStateGather.size()].push_back(temp_state[j]);
          }
        }

        for (int stateIndex = 0; stateIndex < GoadGather.size() && GoadGather.size() > 0; stateIndex++) {
          if (GoadGather[stateIndex].size() == temp_state.size()) {
            break;  // 输入元素后的状态都一样   继续输入字符验证
          }
          else if (GoadGather[stateIndex].size() > 0) {    // 新状态   需要划分
            if (MinimizeDFAStateGather[current_pos] == temp_state) {
              MinimizeDFAStateGather.erase(MinimizeDFAStateGather.begin() + current_pos);  // 删除某位置的数据
            }

            IsSplitable = true;                                                            // 磕划分
            MinimizeDFAStateGather.push_back(GoadGather[stateIndex]);                      // 添加进  状态集合
            que_MinimizeDFAGather.push(GoadGather[stateIndex]);                            // 添加进队列
          }
          else {
            continue;  // 空集  不用管
          }
        }
      }
    }
  }

  //--------- 利用可区分概念进行划分  找出一样的状态 ------------------------
}
void TransformExpr::CombinSameState()
{
  //--------- 获得最小化边集  将状态一样的集合用一个元素代替 ----------------
  //原理：用集合中的一个代替其他的：消去其他状态X，把从消去状态X射出和射入的弧都引到代表状态Y上
  int i, j;
  vector<EDGE> tempDFAEdge;
  tempDFAEdge = DFA_Edge;                                    // 临时边集  已经把相同的边用一个代替

  for (i = 0; i < MinimizeDFAStateGather.size(); i++) {  // 查找可合并的集合并代替
    if (MinimizeDFAStateGather[i].size() > 1) {            // 可合并的集合
      vector<int> tempState;
      tempState.clear();
      tempState = MinimizeDFAStateGather[i];
      int RepState = MinimizeDFAStateGather[i][0];       // 取第一个元素为代表

      for (j = 1; j < tempState.size(); j++) {       // 替换元素
        for (int k = 0; k < tempDFAEdge.size(); k++) {
          //EDGE e;
          if (tempDFAEdge[k].start == tempState[j]) { // 起点一样  替换
            tempDFAEdge[k].start = RepState;
          }

          if (tempDFAEdge[k].end == tempState[j]) {  // 终点一样  替换
            tempDFAEdge[k].end = RepState;
          }
        }
      }
    }
  }

  for (i = 0; i < tempDFAEdge.size(); i++) {             // 删除一样的状态
    EDGE e;
    e.start = tempDFAEdge[i].start;
    e.input_expr = tempDFAEdge[i].input_expr;
    e.end = tempDFAEdge[i].end;

    if (i == 0) {                                          // 添加第一项
      MinimizeDFA_Edge.push_back(e);
    }
    else {
      if (!HaveSameEdge(MinimizeDFA_Edge, e)) {          // 如果在集合中不存在此状态  添加
        MinimizeDFA_Edge.push_back(e);
      }
    }
  }

  //--------- 获得最小化边集  将状态一样的集合用一个元素代替 ----------------

  //--------- 查找终态 和 非终态 --------------------------------------------
  for (i = 0; i < MinimizeDFAStateGather.size(); i++) {
    int state = 0;

    if (find(MinimizeDFAStateGather[i].begin(), MinimizeDFAStateGather[i].end(), startDFA.front()) != MinimizeDFAStateGather[i].end()) {
      MinimizeStartDFA.push_back(MinimizeDFAStateGather[i][0]);   // 选择代表【第一个】
    }

    for (j = 0; j < MinimizeDFAStateGather[i].size(); j++) {
      if (find(endDFA.begin(), endDFA.end(), MinimizeDFAStateGather[i][j]) != endDFA.end()) { // 终态
        if (find(MinimizeEndDFA.begin(), MinimizeEndDFA.end(), MinimizeDFAStateGather[i][0]) == MinimizeEndDFA.end()) { // 集合中没有此状态
          MinimizeEndDFA.push_back(MinimizeDFAStateGather[i][0]);
        }
      }
      else {
        if (find(MinimizeNonEndDFA.begin(), MinimizeNonEndDFA.end(), MinimizeDFAStateGather[i][0]) == MinimizeNonEndDFA.end()) {
          MinimizeNonEndDFA.push_back(MinimizeDFAStateGather[i][0]);
        }
      }
    }
  }

  sort(MinimizeEndDFA.begin(), MinimizeEndDFA.end());
  sort(MinimizeNonEndDFA.begin(), MinimizeNonEndDFA.end());
  //--------- 查找终态 和 非终态 --------------------------------------------
}
//--------------------- 最小化 DFA ------------------------------------------------------------------------------------

bool TransformExpr::TestInput_MinimizeNFA(CString TestInput)
{
  int currentState, nextState;
  nextState = MinimizeStartDFA.front();      // 起始状态
  int i = 0;

  for (; i < TestInput.GetLength(); i++) {   // 不断读入字符
    currentState = nextState;              // 目前的起点
    int j = 0;                             // 计数遍历表

    for (; j < MinimizeDFA_Edge.size(); j++) { // 遍历表
      if (MinimizeDFA_Edge[j].start == currentState && MinimizeDFA_Edge[j].input_expr == TestInput[i]) { // 找到通道
        nextState = MinimizeDFA_Edge[j].end; // 记录下个起始状态
        break;
      }
    }

    if (j == MinimizeDFA_Edge.size()) {    // 遍历完表了【如果全部匹配，则i等于TestInput的长度】
      break;  //           【如果有不匹配的，则i 小于TestInput的长度】
    }
  }

  if (i == TestInput.GetLength() && find(MinimizeEndDFA.begin(), MinimizeEndDFA.end(), nextState) != MinimizeEndDFA.end()) { // 最后状态是在 可接受状态中
    return true;
  }
  else {
    return false;
  }
}
//--------------------- 运算规则  -------------------------------------------------------------------------------------

/* 查找通过$到达的状态
*    input  起点集合
*    output 输出可到达集合
*    EDGE   边集
*/
void TransformExpr::Find_NULL_Closure(vector<int> input, vector<int>& output, vector<EDGE> edge)
{
  LinkedStack<int> state;                // 状态集合
  int temp_start = 0;
  output.clear();                        // 清除原有数据

  for (int i = 0; i < input.size(); i++) { //
    state.Add(input[i]);
  }

  while (!state.IsEmpty()) {
    output.push_back(state.Top());     // 首先加入
    state.Delete(temp_start);

    for (int i = 0; i < edge.size(); i++) {
      if (edge[i].start == temp_start && edge[i].input_expr == '$') {
        state.Add(edge[i].end);  // 添加进栈
      }
    }
  }

  sort(output.begin(), output.end());    // 进行排序，方便后来判断
}
/*
  input通过输入in_expr到达的集合
  input   输入集合
  in_expr 输入字符
  result  起点集合通过 当前输入 可到达的集合
*/
void TransformExpr::Move(vector<int> input, char in_expr, vector<int>& result, vector<EDGE> edge)
{
  vector<int> temp_input;
  result.clear();

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < edge.size(); j++)    // 遍历边集
      if (edge[j].start == input[i] && edge[j].input_expr == in_expr) {
        temp_input.push_back(edge[j].end);
      }
  }

  Find_NULL_Closure(temp_input, result, edge); // 查找$可到达的集合
}

// 输入input后的集合在总集合中的位置(从0开始）
int TransformExpr::GetDFAState_Gather(std::vector<vector<int> > state, int istate, char input)
{
  int i, GoadState = 0;                            // 输入input后的目标状态

  for (i = 0; i < DFA_Edge.size(); i++) {
    if (DFA_Edge[i].start == istate && DFA_Edge[i].input_expr == input) {
      GoadState = DFA_Edge[i].end;  // 终点
    }
  }

  for (i = 0; i < state.size(); i++) {
    if (find(state[i].begin(), state[i].end(), GoadState) != state[i].end()) { // 找到了
      return i;  // 是从0开始的
    }
  }

  return -1;                                    // 没找到：说明是输入input后到达空集
}
//  判断某条边在边集中是否存在
//  存在：   TRUE
//  不存在： FALSE
bool TransformExpr::HaveSameEdge(vector<EDGE> edge, EDGE e)
{
  for (int i = 0; i < edge.size(); i++) {
    if (edge[i].start == e.start && edge[i].input_expr == e.input_expr && edge[i].end == e.end) {
      return true;
    }
  }

  return false;
}

// 获得某个集合在  总集合中的顺序编号（从1开始）
int TransformExpr::GetState_Gather(std::vector<vector<int> > state, std::vector<int> JudgeState)
{
  int num = 0;

  for (int i = 0; i < state.size(); i++) {
    if (state[i] == JudgeState) {
      return ++i;
    }
  }

  return 0;        // 没发现
}
// 运算符优先级
// *最高（左结合）
// ^次之
// |最低
char TransformExpr::OperatePriority(char a, char b)
{
  int i, j;
  char c[7][7] = {
    {'0', '#', ')', '*', '|', '^', '('},
    {'#', 'e', 'x', '<', '<', '<', '<'},
    {'(', 'x', '=', '<', '<', '<', '<'},
    {'^', '>', '>', '<', '>', '>', '<'},
    {'*', '>', '>', '>', '>', '>', '<'},
    {'|', '>', '>', '<', '>', '<', '<'},
    {')', '>', '>', '>', '>', '>', 'x'},
  };

  for (i = 1; i < 6; i++)if (c[i][0] == a) {
      break;
    }

  for (j = 1; j < 6; j++)if (c[0][j] == b) {
      break;
    }

  return c[i][j];
}
// 每个输入构成自身NFA
// @-->@
void TransformExpr::MakeNFA_Self(char input, NFA* nfa, std::vector<EDGE>& edge)
{
  EDGE e; // 新边

  if (nfa->start == 0 && nfa->end == 0) {
    e.start = nfa->start = state; // 起点
    e.input_expr = input;         // 输入
    e.end = nfa->end = ++state;   // 重点
    state++;
  }
  else {
    e.start = nfa->start; // 起点
    e.input_expr = input; // 输入
    e.end = nfa->end ;    // 重点
  }

  edge.push_back(e);
}
// 双目运算符规则
void TransformExpr::OperateRule_Binary(NFA Operand_left, char Operator, NFA Operand_right, NFA* result, vector<EDGE>& edge)
{
  int i;
  NFA temp;

  switch (Operator) {
  case'^':// “与” 运算
    result->start = Operand_left.start;
    result->end = Operand_right.end;

    for (i = 0; i < edge.size(); i++) {
      if (edge[i].start == Operand_right.start) {
        edge[i].start = Operand_left.end;    // 将两条边连接
      }
    }

    break;

  case'|':// “或” 运算
    result->start = temp.start = state;
    temp.end = Operand_left.start;
    MakeNFA_Self('$', &temp, edge);

    temp.start = state;
    temp.end = Operand_right.start;
    MakeNFA_Self('$', &temp, edge);

    temp.start = Operand_left.end;
    temp.end = ++state;
    MakeNFA_Self('$', &temp, edge);

    temp.start = Operand_right.end;
    temp.end = result->end = state;
    MakeNFA_Self('$', &temp, edge);

    state++;
    break;
  };
}
// “闭包” 运算
void TransformExpr::OperateRule_Unary(NFA Operand, char Operator, NFA* result, vector<EDGE>& edge)
{
  NFA temp;
  result->start = temp.start = state;  // 新状态
  temp.end = Operand.start;
  MakeNFA_Self('$', &temp, edge);

  temp.start = Operand.end;
  result->end = temp.end = ++state;    // 新状态
  MakeNFA_Self('$', &temp, edge);

  temp.start = Operand.end;
  temp.end = Operand.start;
  MakeNFA_Self('$', &temp, edge);

  temp.start = result->start;
  temp.end = result->end;
  MakeNFA_Self('$', &temp, edge);

  state++;       // 状态数++

}
// 判断是运算符还是 操作数
int TransformExpr::type(char m_expr)
{
  if (m_expr == '*' || m_expr == '|' || m_expr == '^') {
    return 1;
  }
  else if (m_expr != '(' && m_expr != ')') {
    return 0;
  }
  else {
    return -1;
  }
}
//--------------------- 运算规则  --------------------------------------------------------------------------------------

//--------------------- 显示      --------------------------------------------------------------------------------------
// NFA 显示
CString TransformExpr::ShowNFA()
{
  CString show_NFA = "";

  for (int i = 0; i < this->NFA_Edge.size(); i++) {
    CString num = "";
    CString start = "";
    CString input = "";
    CString end = "";
    num.Format("%d ", i);

    start.Format("%d  ", this->NFA_Edge[i].start);

    //////////////////////////////////////
    char* a;                            //
    a = &this->NFA_Edge[i].input_expr;  //取到输入符
    a[1] = '\0';                        //结束符
    input.Format(_T("%s  "), a);        //转化
    //////////////////////////////////////

    end.Format("%d  ", this->NFA_Edge[i].end);
    show_NFA += ("Index " + num + ":");
    show_NFA += (start + input + end + " \r\n");
  }

  return show_NFA;
}
// DFA 显示
CString TransformExpr::ShowDFA()
{
  CString show_DFA = "";

  for (int i = 0; i < this->DFA_Edge.size(); i++) {
    CString num = "";
    CString start = "";
    CString input = "";
    CString end = "";
    num.Format("%d ", i);

    start.Format("%d  ", this->DFA_Edge[i].start);
    //////////////////////////////////////
    char* a;                            //
    a = &this->DFA_Edge[i].input_expr;  //取到输入符
    a[1] = '\0';                        //结束符
    input.Format(_T("%s "), a);         //转化
    //////////////////////////////////////
    end.Format("%d ", this->DFA_Edge[i].end);

    show_DFA += ("Index " + num + ":");
    show_DFA += (start + input + end + "\r\n");
  }

  return show_DFA;
}
// 最小化DFA 显示
CString TransformExpr::ShowMinimizeDFA()
{
  CString show_MinimizeDFA = "";

  for (int i = 0; i < this->MinimizeDFA_Edge.size(); i++) {
    CString num = "";
    CString start = "";
    CString input = "";
    CString end = "";
    num.Format("%d ", i);

    start.Format("%d  ", this->MinimizeDFA_Edge[i].start);
    //////////////////////////////////////
    char* a;                            //
    a = &this->MinimizeDFA_Edge[i].input_expr;  //取到输入符
    a[1] = '\0';                        //结束符
    input.Format(_T("%s "), a);         //转化
    //////////////////////////////////////
    end.Format("%d ", this->MinimizeDFA_Edge[i].end);

    show_MinimizeDFA += ("Index " + num + ":");
    show_MinimizeDFA += (start + input + end + "\r\n");
  }

  return show_MinimizeDFA;
}
//--------------------- 显示      --------------------------------------------------------------------------------------

/* 判断两个状态是否一样
   一样    TRUE
   不一样  False
*/
//bool TransformExpr::IsEquelState(vector<int> left_state, vector<int> right_state)
//{
//  if (left_state.size()!=right_state.size())
//    return false;
//  else
//  {
//       for (int i = 0;i< left_state.size();i++)
//       if (left_state[i]!= right_state[i])
//         return false;
//  }
//  return true;
//}
///*判断该状态在栈中是否存在
//  栈中存在  FALSE
//  不存在    TRUE
//*/
//bool TransformExpr::IsStackExistState(LinkedStack<vector<int> > state, std::vector<int> JudgeState)
//{
//  LinkedStack<vector<int> > temp_state;  // 要操作的栈
//  LinkedStack<vector<int> > link_state;  // 中间连接的栈
//
//  while(!state.IsEmpty() && state.Top().size()>0)
//  {
//        vector<int> temp;   // 临时变量
//    state.Delete(temp);
//    link_state.Add(temp);
//  }
//
//  while(!link_state.IsEmpty() && link_state.Top().size()>0)
//  {
//        vector<int> temp;   // 临时变量
//    link_state.Delete(temp);
//    state.Add(temp);
//    temp_state.Add(temp);
//  }
//
//  while(!temp_state.IsEmpty() && temp_state.Top().size()>0)
//  {
//    vector<int> temp;   // 临时变量
//    temp_state.Delete(temp);
//        if (temp == JudgeState)
//      return false;
//  }
//
//  return true;
//}
///*
//  获得DFA状态 编号
//  编号按进展顺序一次递增的
//*/
//int TransformExpr::GetDFA_State(LinkedStack<vector<int> > state, vector<int> JudgeState)
//{
//    LinkedStack<vector<int> > temp_state;  // 要操作的栈
//  LinkedStack<vector<int> > link_state;  // 中间连接的栈
//    int total = 1;      // 状态总数
//  int pos = 0;        // 要查找的集合  在栈中的位置
//
//  while(!state.IsEmpty() && state.Top().size()>0)
//  {
//        vector<int> temp;   // 临时变量
//    state.Delete(temp);
//    link_state.Add(temp);
//
//  }
//
//  while(!link_state.IsEmpty() && link_state.Top().size()>0)
//  {
//        vector<int> temp;   // 临时变量
//    link_state.Delete(temp);
//    state.Add(temp);
//    temp_state.Add(temp);
//    if(temp == JudgeState)// 有一个一样
//    {
//      pos = total;
//    }
//        total++;
//  }
//  return pos;
//}
//--------------------- NFA 到 DFA ------------------------------------------------------------------------------------

//--------------------- 最小化 DFA ------------------------------------------------------------------------------------
// 最小化DFA  用分割法
//   初始划分：终态与非终态；
//   利用可区分的概念，反复分裂划分中的组Gi，直到不可再分裂；
//   由最终划分构造D'，关键是选代表和修改状态转移；
//   消除可能的死状态和不可达状态。
//   ------  可区分  ------
//   对于任何两个状态t和s，若从一状态出发接受输入字符串ω，
//   而从另一状态出发不接受ω，
//   或者从t出发和从s出发到达不同的接受状态，则称ω对状态t和s是
//   可区分的。