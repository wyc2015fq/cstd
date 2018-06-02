
// This one is still a question:
//    [1 3' + 5]
// Need:
//       Error reporting
//
// Done:
//       Reference counting for the TREE?
//       strings (embedded quotes)
//       Comments
//       ...
//       Number recognition
//       Function handles
//       Keywords
//       Scripts
//
// [1 -2] --> [1 -2]
// [1 - 2] --> [-1]
// [1 - - 2] --> [3]
// [1 - 2 - 3 -4] --> [-4 -4]
// [1 - 2 - 3 - 4] --> -8
// [1 -2 - 3 -4] --> [1 -5 -4]
// [- 3] --> [-3]
// [2 -(3)] --> [2 -3]
// [(2 -(3))] --> [-1]
// [1 --2] --> [1 2]
//
// An additional set of cases to consider:
// [A (3)]  [A {4}]
// both of which are incorrectly parsed as
// [A(3)] and [A{4}]
//
// Question:
//
//   if a +b print
//
// Suggestion.. if have unary operator followed by a nonwhitespace
// tag it as a possible unary operator.
//
// Conclusion - if we have tight binding between the unary operators
//
// Conclusion - do not allow spaces after unary operators (unless you have to)
// [a' 1 b'] --> [a',1,b']
// [a ' 1 b'] --> [a,' 1 b']
// [a ' 1 b] --> error
// [a .' 1 b] --> error
// Conclusion - do not allow spaces before transpose operators
// [1 -3, 4] --> [1,-3,4]
// Conclusion - spaces and commas are equivalent!
// [a (3)] --> [a,3], not [a(3)]
// [(a (3))] --> [a(3)]
// Outside
// fprintf('%d\n',a (3)) --> works as a(3)
//
// Special calls are causing more trouble...
//
// Consider:
//  foo /bar
// Is this treated as an expression? or as a special function
// call?
// Also
//  foo bar.dat
// causes trouble.
// Now in general, if we have an identifier (outside a bracket) followed
// by a character, it must be a special call.  That takes care of the
// above syntax.
//
// The only one missing case is the one described above.
//
static BOOL HasNestedFunctions(const tree_t* root)
{
  int i;

  if (tree_is(root, TOK_NEST_FUNC)) {
    return TRUE;
  }

  for (i = 0; i < tree_numChildren(root); i++) {
    if (HasNestedFunctions(root->m_children + i)) {
      return TRUE;
    }
  }

  return FALSE;
}

static unsigned AdjustContextOne(unsigned m)
{
  return (((m & 0xffff) - 1) | (m & 0xffff0000));
}

BOOL parser_match(scanner_t* s, TokenValueType a)
{
  return scanner_next(s)->m_tok == a;
}

int parser_statementSeperator(scanner_t* s, tree_t* root)
{
  tree_t _root[1] = {0};
  root = root ? root : _root;

  if (parser_match(s, TOK_SEMICOLON)) {
    tree_set(root, TOK_QSTATEMENT, AdjustContextOne(scanner_contextNum(s)));
    scanner_consume(s);

    if (parser_match(s, TOK_LF)) {
      scanner_consume(s);
    }
  }
  else if (parser_match(s, TOK_LF)) {
    tree_set(root, TOK_STATEMENT, AdjustContextOne(scanner_contextNum(s)));
    scanner_consume(s);
  }
  else if (parser_match(s, TOK_COMMA)) {
    tree_set(root, TOK_STATEMENT, AdjustContextOne(scanner_contextNum(s)));
    scanner_consume(s);
  }

  tree_free(_root);
  return 0;
}

int parser_singletonStatement(scanner_t* s, tree_t* root)
{
  tree_settok(root, scanner_next(s));
  scanner_consume(s);
  return 0;
}

int parser_serror(scanner_t* s, const char* errmsg)
{
  if (scanner_contextNum(s) > s->lastpos) {
    strcpy(s->lasterr, errmsg);
    s->lastpos = scanner_contextNum(s);
  }

  throw_ParseException(scanner_contextNum(s), errmsg);
}

const token_t* parser_expect(scanner_t* s, TokenValueType a, const char* because)
{
  const token_t* ret = scanner_next(s);
  because = because ? because : "";

  if (scanner_next(s)->m_tok != (a)) {
    if (a != TOK_EOF) {
      char errstr[256];
      char buf[256];
      token_t tmp[1] = {TOK_INVALID, 0};
      token_set(tmp, a, 0, 0, 0);
      TokenToString(tmp, buf, 256);
      _snprintf(errstr, 256, "Expecting %s for %s", buf, because);
      parser_serror(s, errstr);
    }
    else {
      parser_serror(s, "Unexpected input");
    }
  }
  else {
    scanner_consume(s);
  }

  return ret;
}

int parser_identifier(scanner_t* s, tree_t* root)
{
  if (!parser_match(s, TOK_IDENT)) {
    parser_serror(s, "expecting identifier");
  }

  tree_settok(root, scanner_next(s));
  scanner_consume(s);
  return 0;
}

int parser_expression(scanner_t* s, tree_t* root);

int parser_keyword(scanner_t* s, tree_t* root)
{
  tree_t* p;
  tree_settok(root, parser_expect(s, TOK_RDIV, 0));
  tree_rename(root, TOK_KEYWORD);
  p = tree_addChild_n(root, 1);
  parser_identifier(s, p);

  if (parser_match(s, TOK_EQ)) {
    scanner_consume(s);
    p = tree_addChild_n(root, 1);
    parser_expression(s, p);
  }

  return 0;
}

int parser_indexingExpressions(scanner_t* s, tree_t* root, BOOL blankRefOK)
{
  BOOL deref = TRUE;

  while (deref) {
    if (parser_match(s, TOK_LBRACKET0)) {
      tree_t* sub;
      scanner_consume(s);

      if (s->octCompat) {
        scanner_pushWSFlag(s, TRUE);
      }

      sub = tree_addChild_n(root, 1);
      tree_set(sub, TOK_PARENS, scanner_contextNum(s));

      while (!parser_match(s, TOK_RBRACKET0)) {
        tree_t* p = tree_addChild_n(sub, 1);

        if (parser_match(s, TOK_COLON)) {
          tree_settok(p, parser_expect(s, TOK_COLON, 0));
        }
        else if (parser_match(s, TOK_RDIV)) {
          parser_keyword(s, p);
        }
        else {
          parser_expression(s, p);
        }

        if (!parser_match(s, TOK_RBRACKET0)) {
          parser_expect(s, TOK_COMMA, 0);
        }
      }

      if ((tree_numChildren(sub) == 0) && (!blankRefOK)) {
        if (s->octCompat) {
          scanner_popWSFlag(s);
        }

        parser_serror(s, "The expression A() is not allowed.");
      }

      parser_expect(s, TOK_RBRACKET0, 0);

      if (s->octCompat) {
        scanner_popWSFlag(s);
      }
    }
    else if (parser_match(s, TOK_LBRACKET2)) {
      tree_t* sub = tree_addChild_n(root, 1);
      scanner_consume(s);
      tree_set(sub, TOK_BRACES, scanner_contextNum(s));

      while (!parser_match(s, TOK_RBRACKET2)) {
        tree_t* p = tree_addChild_n(sub, 1);

        if (parser_match(s, TOK_COLON)) {
          tree_settok(p, parser_expect(s, TOK_COLON, 0));
        }
        else {
          parser_expression(s, p);
        }

        if (!parser_match(s, TOK_RBRACKET2)) {
          parser_expect(s, TOK_COMMA, 0);
        }
      }

      if (tree_numChildren(sub) == 0)  {
        parser_serror(s, "The expression A{} is not allowed.");
      }

      parser_expect(s, TOK_RBRACKET2, 0);
    }
    else if (parser_match(s, TOK_DOT)) {
      tree_t* p;
      tree_t* dynroot = tree_addChild_n(root, 1);
      tree_settok(dynroot, scanner_next(s));
      scanner_consume(s);
      p = tree_addChild_n(dynroot, 1);

      if (parser_match(s, TOK_LBRACKET0)) {
        scanner_consume(s);
        tree_rename(dynroot, TOK_DYN);
        parser_expression(s, p);
        parser_expect(s, TOK_RBRACKET0, 0);
      }
      else {
        parser_identifier(s, p);
      }
    }
    else {
      deref = FALSE;
    }
  }

  return 0;
}

// Parse A(foo).goo{1:3}... etc
int parser_variableDereference(scanner_t* s, tree_t* root, BOOL blankRefOK)
{
  tree_t* ident = tree_addChild_n(root, 1);
  parser_identifier(s, ident);
  tree_set(root, TOK_VARIABLE, scanner_contextNum(s));
  parser_indexingExpressions(s, root, blankRefOK);
  return 0;
}

int parser_multiFunctionCall(scanner_t* s, tree_t* root)
{
  tree_t* lhs, *rhs, *p;
  parser_expect(s, TOK_LBRACKET1, 0);
  tree_rename(root, TOK_MULTI);
  lhs = tree_addChild_n(root, 2);
  rhs = lhs+1;
  tree_set(lhs, TOK_BRACKETS, scanner_contextNum(s));

  while (!parser_match(s, TOK_RBRACKET1)) {
    p = tree_addChild_n(lhs, 1);
    parser_variableDereference(s, p, TRUE);

    if (parser_match(s, TOK_COMMA)) {
      scanner_consume(s);
    }
  }

  parser_expect(s, TOK_RBRACKET1, 0);
  parser_expect(s, TOK_EQ, 0);
  parser_expression(s, rhs);
  return 0;
}

void parser_flushSeperators(scanner_t* s)
{
  while (1) {
    tree_t term[1] = {0};
    parser_statementSeperator(s, term);

    if (!tree_valid(term)) {
      tree_free(term);
      return;
    }

    tree_free(term);
  }

  return ;
}

int parser_statement(scanner_t* s, tree_t* root);

int parser_statementList(scanner_t* s, tree_t* root)
{
  tree_set(root, TOK_BLOCK, scanner_contextNum(s));
  parser_flushSeperators(s);

  while (1) {
    tree_t sta[1] = {0};
    tree_t sep[1] = {0};
    tree_t tmp = {0};
    parser_statement(s, sta);

    if (!tree_valid(sta)) {
      tree_free(sta);
      return 0;
    }

    parser_statementSeperator(s, sep);

    if (!tree_valid(sep)) {
      tree_free(sta);
      tree_free(sep);
      return 0;
    }

    tree_addChild1(sep, sta);
    tree_addChild1(root, sep);
    parser_flushSeperators(s);
    *sta = *sep = tmp;
  }

  return 0;
}

int parser_functionDefinition(scanner_t* s, tree_t* root)
{
  tree_t* lhs, *rhs, *p, *save;
  tree_settok(root, parser_expect(s, TOK_FUNCTION, 0));

  if (parser_match(s, TOK_LBRACKET1)) {
    scanner_consume(s);
    lhs = tree_addChild_n(root, 2);
    rhs = lhs+1;
    tree_set(lhs, TOK_BRACKETS, scanner_contextNum(s));

    while (!parser_match(s, TOK_RBRACKET1)) {
      p = tree_addChild_n(lhs, 1);
      parser_identifier(s, p);

      if (parser_match(s, TOK_COMMA)) {
        scanner_consume(s);
      }
    }

    parser_expect(s, TOK_RBRACKET1, 0);
    parser_expect(s, TOK_EQ, 0);
    parser_identifier(s, rhs);
  }
  else {
    lhs = tree_addChild_n(root, 2);

    // Two possible parses here
    if (parser_match(s, TOK_EQ)) {
      save = tree_addChild_n(lhs, 1);
      parser_identifier(s, save);
      tree_set(lhs, TOK_BRACKETS, scanner_contextNum(s));
      parser_expect(s, TOK_EQ, 0);
      rhs = lhs+1;
      parser_identifier(s, rhs);
    }
    else {
      save = lhs+1;
      parser_identifier(s, save);
      tree_set(lhs, TOK_BRACKETS, scanner_contextNum(s));
    }
  }

  // Process (optional) args
  if (parser_match(s, TOK_LBRACKET0)) {
    tree_t* args = tree_addChild_n(root, 1);
    scanner_consume(s);
    tree_set(args, TOK_PARENS, scanner_contextNum(s));

    while (!parser_match(s, TOK_RBRACKET0)) {
      tree_t* ident, *p;
      ident = tree_addChild_n(args, 1);

      if (parser_match(s, TOK_AND)) {
        tree_settok(ident, parser_expect(s, TOK_AND, 0));
        p = tree_addChild_n(ident, 1);
        parser_identifier(s, p);
      }
      else {
        parser_identifier(s, ident);
      }

      if (!parser_match(s, TOK_RBRACKET0)) {
        parser_expect(s, TOK_COMMA, 0);
      }
    }

    parser_expect(s, TOK_RBRACKET0, 0);
  }
  else {
    tree_t* args = tree_addChild_n(root, 1);
    tree_set(args, TOK_PARENS, scanner_contextNum(s));
  }

  parser_statementSeperator(s, 0);
  p = tree_addChild_n(root, 1);
  parser_statementList(s, p);
  return 0;
}

BOOL parser_matchNumber(scanner_t* s)
{
  return (parser_match(s, TOK_REAL) || parser_match(s, TOK_IMAG) ||
      parser_match(s, TOK_REALF) || parser_match(s, TOK_IMAGF));
}

int parser_specialFunctionCall(scanner_t* s, tree_t* root)
{
  tree_t* p;
  scanner_pushWSFlag(s, FALSE);
  tree_set(root, TOK_SPECIAL, scanner_contextNum(s));
  p = tree_addChild_n(root, 1);
  parser_identifier(s, p);

  // Next must be a whitespace
  if (!parser_match(s, TOK_SPACE)) {
    parser_serror(s, "Not special call");
  }

  scanner_consume(s);
  {
    scanner_t t_lex[1] = {0};
    scanner_copy(t_lex, s);

    if (token_is(scanner_next(t_lex), TOK_SEMICOLON) ||
        token_is(scanner_next(t_lex), TOK_LF) ||
        token_is(scanner_next(t_lex), TOK_LBRACKET0) ||
        token_is(scanner_next(t_lex), TOK_COMMA)) {
      parser_serror(s, "Not special call");
    }

    if (token_isBinaryOperator(scanner_next(t_lex)) || token_isUnaryOperator(scanner_next(t_lex))) {
      scanner_consume(t_lex);

      if (token_is(scanner_next(t_lex), TOK_SPACE)) {
        parser_serror(s, "Not special call");
      }
    }
  }
  // If the next thing is a character or a number, we grab "blobs"
  s->m_blobFlag = TRUE;

  while (!parser_match(s, TOK_SEMICOLON) && !parser_match(s, TOK_LF) && !(parser_match(s, TOK_COMMA))) {
    tree_t* p = tree_addChild_n(root, 1);
    tree_settok(p, scanner_next(s));
    scanner_consume(s);

    if (parser_match(s, TOK_SPACE)) {
      scanner_consume(s);
    }
  }

  s->m_blobFlag = FALSE;
  scanner_popWSFlag(s);
  return 0;
}

int parser_forIndexExpression(scanner_t* s, tree_t* root)
{
  tree_t ident[1] = {0};

  if (parser_match(s, TOK_LBRACKET0)) {
    scanner_consume(s);
    parser_forIndexExpression(s, root);
    parser_expect(s, TOK_RBRACKET0, 0);
    return 0;
  }

  parser_identifier(s, ident);

  if (parser_match(s, TOK_EQ)) {
    tree_t expr[1] = {0};
    tree_settok(root, scanner_next(s));
    scanner_consume(s);
    parser_expression(s, expr);
    tree_addChild2(root, ident, expr);
  }
  else {
    tree_free(root);
    *root = *ident;
  }

  return 0;
}

int parser_forStatement(scanner_t* s, tree_t* root)
{
  tree_t* index;
  tree_t* block;
  tree_settok(root, parser_expect(s, TOK_FOR, 0));
  index = tree_addChild_n(root, 2);
  block = index+1;
  parser_forIndexExpression(s, index);
  parser_statementSeperator(s, 0);
  parser_statementList(s, block);
  parser_expect(s, TOK_END, "FOR statement");
  return 0;
}

int parser_whileStatement(scanner_t* s, tree_t* root)
{
  tree_t* warg, *block;
  tree_settok(root, parser_expect(s, TOK_WHILE, 0));
  warg = tree_addChild_n(root, 2);
  block = warg+1;
  parser_expression(s, warg);
  parser_statementSeperator(s, 0);
  parser_statementList(s, block);
  parser_expect(s, TOK_END, "WHILE statement");
  return 0;
}

int parser_ifStatement(scanner_t* s, tree_t* root)
{
  tree_t* test, *trueblock, *elseif, *block, *elseblk;
  tree_settok(root, parser_expect(s, TOK_IF, 0));
  test = tree_addChild_n(root, 2);
  trueblock = test + 1;
  parser_expression(s, test);
  parser_statementSeperator(s, 0);
  parser_statementList(s, trueblock);

  while (parser_match(s, TOK_ELSEIF)) {
    elseif = tree_addChild_n(root, 1);
    tree_settok(elseif, scanner_next(s));
    scanner_consume(s);
    test = tree_addChild_n(elseif, 2);
    block = test+1;
    parser_expression(s, test);
    parser_statementList(s, block);
  }

  if (parser_match(s, TOK_ELSE)) {
    elseblk = tree_addChild_n(root, 1);
    tree_settok(elseblk, scanner_next(s));
    scanner_consume(s);
    block = tree_addChild_n(elseblk, 1);
    parser_statementList(s, block);
  }

  parser_expect(s, TOK_END, " IF block");
  return 0;
}

int parser_declarationStatement(scanner_t* s, tree_t* root)
{
  tree_settok(root, scanner_next(s));
  scanner_consume(s);

  while (parser_match(s, TOK_IDENT)) {
    tree_t* p = tree_addChild_n(root, 1);
    parser_identifier(s, p);
  }

  return 0;
}

int parser_tryStatement(scanner_t* s, tree_t* root)
{
  tree_t* block, *catchblock;
  tree_settok(root, parser_expect(s, TOK_TRY, 0));
  parser_statementSeperator(s, 0);
  block = tree_addChild_n(root, 1);
  parser_statementList(s, block);

  if (parser_match(s, TOK_CATCH)) {
    catchblock = tree_addChild_n(root, 1);
    block = tree_addChild_n(catchblock, 1);
    tree_settok(catchblock, scanner_next(s));
    scanner_consume(s);
    parser_statementSeperator(s, 0);
    parser_statementList(s, block);
  }

  parser_expect(s, TOK_END, "TRY block");
  return 0;
}

int parser_assignmentStatement(scanner_t* s, tree_t* root)
{
  tree_t ident[1] = {0};
  tree_t expr[1] = {0};
  parser_variableDereference(s, ident, FALSE);

  if (!s->octCompat || parser_match(s, TOK_EQ)) {
    tree_settok(root, parser_expect(s, TOK_EQ, 0));
    parser_expression(s, expr);
    tree_addChild2(root, ident, expr);
  }
  else if (parser_match(s, TOK_PLUS_EQ)) {
    tree_settok(root, parser_expect(s, TOK_PLUS_EQ, 0));
    parser_expression(s, expr);
    tree_addChild2(root, ident, expr);
  }
  else if (parser_match(s, TOK_MINUS_EQ)) {
    tree_settok(root, parser_expect(s, TOK_MINUS_EQ, 0));
    parser_expression(s, expr);
    tree_addChild2(root, ident, expr);
  }
  else {
    parser_serror(s, "Not an assignment statement");
  }

  tree_free(ident);
  tree_free(expr);
  return 0;
}

int parser_switchStatement(scanner_t* s, tree_t* root)
{
  tree_t* swexpr, *caseblock, *csexpr, *block, *otherwise;
  tree_settok(root, parser_expect(s, TOK_SWITCH, 0));
  swexpr = tree_addChild_n(root, 1);
  parser_expression(s, swexpr);
  parser_flushSeperators(s);

  while (parser_match(s, TOK_CASE)) {
    caseblock = tree_addChild_n(root, 0);
    csexpr = tree_addChild_n(caseblock, 2);
    block = csexpr+1;
    tree_settok(caseblock, scanner_next(s));
    scanner_consume(s);
    parser_expression(s, csexpr);
    parser_statementSeperator(s, 0);
    parser_statementList(s, block);
  }

  if (parser_match(s, TOK_OTHERWISE)) {
    otherwise = tree_addChild_n(root, 1);
    block = tree_addChild_n(otherwise, 1);
    tree_settok(otherwise, scanner_next(s));
    scanner_consume(s);
    parser_statementSeperator(s, 0);
    parser_statementList(s, block);
  }

  parser_expect(s, TOK_END, " SWITCH block");
  return 0;
}

int parser_dBStepOrTraceStatement(scanner_t* s, tree_t* root)
{
  tree_t* p;
  tree_settok(root, scanner_next(s));
  scanner_consume(s);

  if (parser_match(s, TOK_COMMA) || parser_match(s, TOK_SEMICOLON) || parser_match(s, TOK_LF)) {
    return 0;
  }

  p = tree_addChild_n(root, 1);
  parser_expression(s, p);
  return 0;
}

int parser_statement(scanner_t* s, tree_t* root)
{
  if (parser_match(s, TOK_EOF)) {
    return 0;
  }

  if (parser_match(s, TOK_END)) {
    return 0;
  }

  if (parser_match(s, TOK_FOR)) {
    return parser_forStatement(s, root);
  }

  if (parser_match(s, TOK_BREAK)) {
    return parser_singletonStatement(s, root);
  }

  if (parser_match(s, TOK_CONTINUE)) {
    return parser_singletonStatement(s, root);
  }

  if (parser_match(s, TOK_DBUP) || parser_match(s, TOK_DBDOWN)) {
    return parser_singletonStatement(s, root);
  }

  if (parser_match(s, TOK_WHILE)) {
    return parser_whileStatement(s, root);
  }

  if (parser_match(s, TOK_DBSTEP) || parser_match(s, TOK_DBTRACE)) {
    return parser_dBStepOrTraceStatement(s, root);
  }

  if (parser_match(s, TOK_IF)) {
    return parser_ifStatement(s, root);
  }

  if (parser_match(s, TOK_SWITCH)) {
    return parser_switchStatement(s, root);
  }

  if (parser_match(s, TOK_TRY)) {
    return parser_tryStatement(s, root);
  }

  if (parser_match(s, TOK_KEYBOARD) || parser_match(s, TOK_RETURN) ||
      parser_match(s, TOK_RETALL) || parser_match(s, TOK_QUIT)) {
    return parser_singletonStatement(s, root);
  }

  if (parser_match(s, TOK_GLOBAL) || parser_match(s, TOK_PERSISTENT)) {
    return parser_declarationStatement(s, root);
  }

  // Now come the tentative parses
  {
    scanner_t save[1] = {0};
    tree_t* p;
    scanner_copy(save, s);

    if (parser_match(s, TOK_IDENT)) {
      parser_assignmentStatement(s, root);
      s->lastpos = 0;
      return 0;
    }

    if (parser_match(s, TOK_LBRACKET1)) {
      parser_multiFunctionCall(s, root);
      s->lastpos = 0;
      return 0;
    }

    if (parser_match(s, TOK_IDENT)) {
      parser_specialFunctionCall(s, root);
      s->lastpos = 0;
      return 0;
    }

    if (parser_match(s, TOK_FUNCTION)) {
      parser_functionDefinition(s, root);
      tree_rename(root, TOK_NEST_FUNC);
      parser_expect(s, TOK_END, "FUNCTION definition");
      s->lastpos = 0;
      return 0;
    }

    tree_set(root, TOK_EXPR, scanner_contextNum(s));
    p = tree_addChild_n(root, 1);
    parser_expression(s, p);
    s->lastpos = 0;
  }
  return 0;
}

static unsigned precedence(const token_t* t)
{
  switch (t->m_tok) {
  case TOK_SOR:
    return 1;

  case TOK_SAND:
    return 2;

  case TOK_OR:
    return 3;

  case TOK_AND:
    return 4;

  case TOK_LSS:
    return 5;

  case TOK_GTR:
    return 5;

  case TOK_LEQ :
    return 5;

  case TOK_GEQ:
    return 5;

  case TOK_EQU:
    return 5;

  case TOK_NEQ:
    return 5;

  case TOK_COLON:
    return 6;

  case TOK_PLUS:
    return 7;

  case TOK_MINUS:
    return 7;

  case TOK_MUL:
    return 8;

  case TOK_RDIV:
    return 8;

  case TOK_LDIV:
    return 8;

  case TOK_DOTTIMES:
    return 8;

  case TOK_DOTRDIV:
    return 8;

  case TOK_DOTLDIV:
    return 8;

  case TOK_UNARY_PLUS:
    return 9;

  case TOK_UNARY_MINUS:
    return 9;

  case TOK_TILDE:
    return 9;

  case TOK_INCR_PREFIX:
    return 9;

  case TOK_DECR_PREFIX:
    return 9;

  case TOK_CARET:
    return 10;

  case TOK_DOTPOWER:
    return 10;
  }

  return 1;
}

int parser_matDef(scanner_t* s, tree_t* root, TokenValueType basetok, TokenValueType closebracket)
{
  // Octave compatibility mode requires commas between matrix entries, so white
  // space is ignored inside matrix definitions.
  if (!s->octCompat) {
    scanner_pushWSFlag(s, FALSE);
  }

  tree_set(root, basetok, 0);

  if (parser_match(s, TOK_SPACE)) {
    scanner_consume(s);
  }

  while (!parser_match(s, closebracket)) {
    tree_t* rowdef = tree_addChild_n(root, 1);
    tree_set(rowdef, TOK_ROWDEF, scanner_contextNum(s));

    while (!parser_match(s, TOK_SEMICOLON) && !parser_match(s, TOK_LF) && !parser_match(s, closebracket)) {
      tree_t* p = tree_addChild_n(rowdef, 1);
      parser_expression(s, p);

      if (parser_match(s, TOK_COMMA)) {
        scanner_consume(s);

        while (parser_match(s, TOK_SPACE)) {
          scanner_consume(s);
        }
      }
      else if (parser_match(s, TOK_SPACE)) {
        scanner_consume(s);
      }
    }

    if (parser_match(s, TOK_SEMICOLON) || parser_match(s, TOK_LF)) {
      scanner_consume(s);
    }

    if (parser_match(s, TOK_SPACE)) {
      scanner_consume(s);
    }
  }

  if (!s->octCompat) {
    scanner_popWSFlag(s);
  }

  return 0;
}

int parser_transposeFixup(scanner_t* s, tree_t* root)
{
  while ((scanner_next(s)->m_tok == TOK_SINGLE_QUOTE) || (scanner_next(s)->m_tok == TOK_DOTTRANSPOSE)) {
    tree_t tt[1] = {0}, t;
    tree_settok(tt, scanner_next(s));
    tree_addChild1(tt, root);
    CV_SWAP(*tt, *root, t);
    tree_free(tt);
    scanner_consume(s);
  }

  if (parser_match(s, TOK_SPACE)) {
    if (!((scanner_peek(s, 0, '-') || scanner_peek(s, 0, '+')) && !scanner_peek(s, 1, ' '))) {
      scanner_consume(s);
    }
  }

  if (s->octCompat) {
    if (parser_match(s, TOK_INCR)) {
      tree_t tt[1] = {0}, t;
      tree_set(tt, TOK_INCR_POSTFIX, 0);
      tree_addChild1(tt, root);
      CV_SWAP(*tt, *root, t);
      scanner_consume(s);
    }
    else if (parser_match(s, TOK_DECR)) {
      tree_t tt[1] = {0}, t;
      tree_set(tt, TOK_DECR_POSTFIX, 0);
      tree_addChild1(tt, root);
      CV_SWAP(*tt, *root, t);
      scanner_consume(s);
    }
  }

  return 0;
}

int parser_anonymousFunction(scanner_t* s, tree_t* root)
{
  unsigned pos1, pos2;
  tree_t* args, *p;
  pos1 = scanner_contextNum(s);
  tree_set(root, TOK_ANONYMOUS_FUNC, scanner_contextNum(s));
  parser_expect(s, TOK_LBRACKET0, 0);
  args = tree_addChild_n(root, 1);
  tree_set(args, TOK_PARENS, scanner_contextNum(s));

  while (!parser_match(s, TOK_RBRACKET0)) {
    p = tree_addChild_n(args, 1);
    parser_identifier(s, p);

    if (!parser_match(s, TOK_RBRACKET0)) {
      parser_expect(s, TOK_COMMA, 0);
    }
  }

  parser_expect(s, TOK_RBRACKET0, 0);
  pos1 = scanner_contextNum(s);
  p = tree_addChild_n(root, 1);
  parser_expression(s, p);
  pos2 = scanner_contextNum(s);
  scanner_snippet(s, pos1, pos2, root->m_node);
  return 0;
}

int parser_primaryExpression(scanner_t* s, tree_t* root);

int parser_exp(scanner_t* s, tree_t* root, unsigned p)
{
  tree_t t[1] = {0}, tt;
  parser_primaryExpression(s, t);

  if (s->octCompat && (parser_match(s, TOK_LBRACKET0) || parser_match(s, TOK_LBRACKET2) || parser_match(s, TOK_DOT))) {
    tree_t sub[1] = {0};
    tree_set(sub, TOK_REINDEX, scanner_contextNum(s));
    tree_addChild1(t, sub);
    parser_indexingExpressions(s, sub, TRUE);
    CV_SWAP(*sub, *t, tt);
    tree_free(sub);
  }

  while (token_isBinaryOperator(scanner_next(s)) && (precedence(scanner_next(s)) >= p)) {
    token_t opr_save[1] = {0};
    tree_t t1[1] = {0};
    tree_t t2[1] = {0};
    unsigned q;
    *opr_save = *scanner_next(s);
    scanner_consume(s);

    if (parser_match(s, TOK_SPACE)) {
      scanner_consume(s);
    }

    if (token_isRightAssociative(opr_save)) {
      q = precedence(opr_save);
    }
    else {
      q = 1 + precedence(opr_save);
    }

    tree_settok(t2, opr_save);
    parser_exp(s, t1, q);
    tree_addChild2(t2, t, t1);
    CV_SWAP(*t, *t2, tt);
    tree_free(t1);
    tree_free(t2);
  }

  CV_SWAP(*root, *t, tt);
  tree_free(t);
  return 0;
}

int parser_primaryExpression(scanner_t* s, tree_t* root)
{
  unsigned q;
  tree_t* child = NULL;
  tree_t* p = NULL;

  if (token_isUnaryOperator(scanner_next(s))) {
    token_t* opr = scanner_next(s);
    scanner_consume(s);

    if (parser_match(s, TOK_SPACE)) {
      scanner_consume(s);
    }

    if (token_is(opr, TOK_PLUS)) {
      (opr->m_tok = TOK_UNARY_PLUS);
    }

    if (token_is(opr, TOK_MINUS)) {
      (opr->m_tok = TOK_UNARY_MINUS);
    }

    if (token_is(opr, TOK_INCR)) {
      (opr->m_tok = TOK_INCR_PREFIX);
    }

    if (token_is(opr, TOK_DECR)) {
      (opr->m_tok = TOK_DECR_PREFIX);
    }

    q = precedence(opr);
    tree_settok(root, opr);
    child = tree_addChild_n(root, 1);
    parser_exp(s, child, q);
    return 0;
  }
  else if (parser_match(s, TOK_LBRACKET0)) {
    scanner_consume(s);
    scanner_pushWSFlag(s, TRUE);
    parser_exp(s, root, 0);
    scanner_popWSFlag(s);
    parser_expect(s, TOK_RBRACKET0, 0);
    parser_transposeFixup(s, root);
    return 0;
  }
  else if (parser_match(s, TOK_AT)) {
    tree_settok(root, scanner_next(s));
    scanner_consume(s);
    p = tree_addChild1(root, 0);

    if (parser_match(s, TOK_LBRACKET0)) {
      parser_anonymousFunction(s, p);
    }
    else {
      parser_identifier(s, p);
    }

    parser_transposeFixup(s, root);
    return 0;
  }
  else if (parser_matchNumber(s) || parser_match(s, TOK_STRING)) {
    tree_settok(root, scanner_next(s));
    scanner_consume(s);
    parser_transposeFixup(s, root);
    return 0;
  }
  else if (parser_match(s, TOK_END)) {
    const token_t* tok = parser_expect(s, TOK_END, "transpose");
    tree_settok(root, tok);
    parser_transposeFixup(s, root);
    return 0;
  }
  else if (parser_match(s, TOK_IDENT)) {
    parser_variableDereference(s, root, 0);
    parser_transposeFixup(s, root);
    return 0;
  }
  else if (parser_match(s, TOK_LBRACKET1)) {
    scanner_consume(s);
    parser_matDef(s, root, TOK_MATDEF, TOK_RBRACKET1);
    parser_expect(s, TOK_RBRACKET1, 0);
    parser_transposeFixup(s, root);
    return 0;
  }
  else if (parser_match(s, TOK_LBRACKET2)) {
    scanner_consume(s);
    parser_matDef(s, root, TOK_CELLDEF, TOK_RBRACKET2);
    parser_expect(s, TOK_RBRACKET2, 0);
    parser_transposeFixup(s, root);
    return 0;
  }
  else {
    if (parser_match(s, TOK_RBRACKET0) || parser_match(s, TOK_RBRACKET1) || parser_match(s, TOK_RBRACKET2)) {
      parser_serror(s, "mismatched parenthesis");
    }
    else {
      parser_serror(s, "unrecognized token");
    }
  }

  return 0;
}

int parser_expression(scanner_t* s, tree_t* root)
{
  if (parser_match(s, TOK_SPACE)) {
    scanner_consume(s);
  }

  return parser_exp(s, root, 0);
}


// NOTES -
//   There are still some issues here...
//    We need to introduce another tentative parse for functions
//    Consider the case:
//     function foo
//       statements
//       function hoo
//           function sub
//           end
//       end
//     end
//  The current code will parse foo into a function,
int parser_process(scanner_t* s, tree_t* root)
{
  tree_t* p;
  s->lastpos = 0;

  while (parser_match(s, TOK_LF)) {
    scanner_consume(s);
  }

  if (parser_match(s, TOK_FUNCTION)) {
    tree_set(root, TOK_FUNCTION_DEFS, scanner_contextNum(s));

    while (parser_match(s, TOK_FUNCTION)) {
      tree_t* child = tree_addChild1(root, 0);
      parser_functionDefinition(s, child);

      while (parser_match(s, TOK_LF)) {
        scanner_consume(s);
      }
    }

    if (HasNestedFunctions(root) || parser_match(s, TOK_END)) {
      parser_expect(s, TOK_END, 0);
    }

    while (parser_match(s, TOK_LF)) {
      scanner_consume(s);
    }

    while (parser_match(s, TOK_FUNCTION)) {
      p = tree_addChild1(root, 0);
      parser_functionDefinition(s, p);

      if (HasNestedFunctions(root) || parser_match(s, TOK_END)) {
        parser_expect(s, TOK_END, 0);
      }

      while (parser_match(s, TOK_LF)) {
        scanner_consume(s);
      }
    }
  }
  else {
    tree_set(root, TOK_SCRIPT, scanner_contextNum(s));
    p = tree_addChild1(root, 0);
    parser_statementList(s, p);
  }

  parser_expect(s, TOK_EOF, 0);
  return 0;
}

int parser_processStatementList(scanner_t* s, tree_t* root)
{
  return parser_statementList(s, root);
}

int ParseString(const char* arg, int len, tree_t* root)
{
  scanner_t s[1] = {0};
  int ret;
  scanner_set(s, arg, len, "", FALSE);
  ret = parser_processStatementList(s, root);
  scanner_free(s);
  return ret;
}

int ParseExpressionString(const char* arg, int len, tree_t* root)
{
  scanner_t s[1] = {0};
  int ret;
  scanner_set(s, arg, len, "", FALSE);
  ret = parser_expression(s, root);
  scanner_free(s);
  return ret;
}
