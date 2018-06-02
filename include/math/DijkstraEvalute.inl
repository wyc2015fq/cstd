
// (1+((2+3)*(4*5)))测试
static double DijkstraEvalute(const char* s, int l) {
  int i, nop=0, nval=0;
  enum {MAX_EXPR=10};
  ExprType ops[MAX_EXPR];
  double vals[MAX_EXPR];
  l = l<=0 ? strlen(s) : l;
  for (i = 0; i <l; ) {
    char c = s[i++];
    if (c == '(');
    else if (c == '+') ops[nop++]=CC_EXPR_ADD;
    else if (c == '-') ops[nop++]=CC_EXPR_SUB;
    else if (c == '*') ops[nop++]=CC_EXPR_MUL;
    else if (c == '/') ops[nop++]=CC_EXPR_DIV;
    else if (c == 's' && (i+3<l) && 0==strncmp(s+i, "sqrt", 4)) {ops[nop++]=CC_EXPR_SQRT;i+=3;}
    else if (c == ')' ) {
      //如果为右括号，弹出运算符和两个操作数，计算结果再次压入值栈
      if (nop>0 && nval>0) {
        ExprType op=ops[--nop];
        double v=vals[--nval];
        if(op == CC_EXPR_ADD) v=vals[--nval]+v;
        else if(op == CC_EXPR_SUB) v=vals[--nval]-v;
        else if(op == CC_EXPR_MUL) v=vals[--nval]*v;
        else if(op == CC_EXPR_DIV) v=vals[--nval]/v;
        else if(op == CC_EXPR_SQRT) v=sqrt(v);
        vals[nval++]=(v);
      }
    }
    else if ('0'<=c && c<='9') { vals[nval++]=str2f(s, i, l, NULL, &i); }
  }
  return vals[0];
}
