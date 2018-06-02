//TransformExpr.h
#ifndef _TRANSFORMEXPR
#define _TRANSFORMEXPR
#include "LinkedStack.h"
#include <vector>      // 使用vector要加 using namespace std;
#include <queue>       
using namespace std;
//-----------  存储结构  ------------------------
typedef struct EDGE                       // 自动机的边
{
	int start;                            // 起点
	char input_expr;                      // 输入字符
	int end;                              // 终点
}EDGE;

typedef struct NFA                        // 构造的NFA
{
	int start;                            // 起点                
	int end;                              // 终点
}NFA;
//-----------  存储结构  ------------------------
class TransformExpr
{
public:
	TransformExpr();
	//-----------------------------转化为后缀表达式----------------------------------------
	void SetExpr(CString m_expr);                                                          // 传入表达市
	CString GetExpr();                                                                     // 获得表达式                   
	bool IsLegal(CString m_expr);                                                          // 判断输入的合法性
	CString ToRegular_expr(CString m_expr);                                                // 转化成可运算的  运算式 
	CString ToPostfix();                                                                   // 转化成后缀表达式
	//-----------------------------转化为后缀表达式----------------------------------------

	//-----------------------------后缀表达式到NFA------------------------------------------
	void Regular_exprToNFA(const char* m_Regular_expr);                                         // 这则表达式到NFA
	CString ShowNFA();                                                                      // 显示NFA的边
    //-----------------------------后缀表达式到NFA------------------------------------------
    
	//----------------------------- NFA到DFA -----------------------------------------------
	void Find_NULL_Closure(vector<int> input, vector<int>&output, vector<EDGE> edge);       // 查找通过$到达的集合
	void Move(vector<int> input, char in_expr, vector<int>&result, vector<EDGE> edge);      // 查找当前输入可到达的集合
	void NFA_To_DFA();                                                                      // NFA 转化DFA
	//bool IsEquelState(vector<int> left_state, vector<int> right_state);                     // 判断两个状态是否一样 
	//bool IsStackExistState(LinkedStack<vector<int> > state, vector<int> JudgeState);         // 判断战中是否已有此状态
	//int GetDFA_State(LinkedStack<vector<int> > state, vector<int> JudgeState);               // 获得DFA状态编号
	int GetState_Gather(vector<vector<int> > state, vector<int> JudgeState);                 // 判断状态集合在总集合中的编号
	CString ShowDFA();                                                                      // 显示DFA的边
	//----------------------------- NFA到DFA -----------------------------------------------

    //----------------------------- 最小化DFA ----------------------------------------------
    void MinimizeDFA();                                                                     // 最小化DFA
	int GetDFAState_Gather(vector<vector<int> > state, int istate, char input);              // 判断istate状态输入input后 的状态  在总集合中的编号
	bool HaveSameEdge(vector<EDGE> edge, EDGE e);                                           // 判断集合中是否有与该边相同的边　
	void CombinSameState();                                                                 // 合并相同状态
	CString ShowMinimizeDFA();                                                              // 最小化显示DFA的边
	//----------------------------- 最小化DFA ----------------------------------------------

	//-----------------------------运算规则及判断------------------------------------------
	char OperatePriority(char a,char b);                                                                       // 运算符优先级
	void MakeNFA_Self(char input,NFA* nfa,vector<EDGE>& edge);                                                 // 构造自身NFA
	void OperateRule_Binary(NFA Operand_left,char Operator,NFA Operand_right,NFA* result,vector<EDGE>& edge);  // 双目运算规则
	void OperateRule_Unary(NFA Operand,char Operator,NFA* result,vector<EDGE>& edge);                          // 单目运算规则
	int type(char  m_expr);                                                                                    // 判断字符类型1运算符  0操作数
    //-----------------------------运算规则及判断-------------------------------------------

	//-----------------------------测试输入字符串-------------------------------------------
	bool TestInput_MinimizeNFA(CString TestInput);                                          // 测试输入
	//-----------------------------测试输入字符串-------------------------------------------
	void ClearAll();                                                                        // 清空所有数据                                        
	~TransformExpr(){}
private:
	CString expr;                     // 最初输入的表达式
public:
	int state;                        // 计数状态
	vector<char> Regular_Input;       // 正规式的输入符
	vector<int>	startNFA;	          // 记录NFA的   起始  状态集
	vector<int>	endNFA;		          // 记录NFA的   终    状态集
	vector<EDGE> NFA_Edge;            // 保存NFA所有边
    //////////////////////////////////////////////////////////////////////////////
	vector<int> startDFA;             // 记录DFA的起始状态集   含有 NFA 起点的集合
	vector<int> endDFA;               // 记录DFA的终态集       含有 NFA 终点的集合（或称接收状态组）
	vector<int> nonEndDFA;            // 非中态集（或称非接收状态组）
	vector<EDGE> DFA_Edge;	          // 由NFA所构造成的DFA的边的集合
    //////////////////////////////////////////////////////////////////////////////
    vector<int> MinimizeStartDFA;     // 记录最小化DFA的起始
    vector<int> MinimizeEndDFA;		  // 最小化后DFA的终态集
	vector<int> MinimizeNonEndDFA; 	  // 最小化后DFA的非终态集
	vector<EDGE> MinimizeDFA_Edge;    // 最小化DFA 边集
	vector<vector<int> > MinimizeDFAStateGather;    // 最小化DFA状态集合
};


#endif