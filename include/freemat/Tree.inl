
static void tree_rename(tree_t* d, TokenValueType newtok)
{
  token_set(d->m_node, newtok, 0, 0, 0);
}
static unsigned tree_context(tree_t* d)
{
  return d->m_node->m_pos;
}
static BOOL tree_valid(const tree_t* d)
{
  return d->m_node->m_tok != (TOK_INVALID);
}
static BOOL tree_is(const tree_t* d, TokenValueType tok)
{
  return (d->m_node->m_tok == tok);
}
static TokenValueType tree_token(const tree_t* d)
{
  return d->m_node->m_tok;
}
static int tree_numChildren(const tree_t* d)
{
  return d->m_childrenlen;
}
static BOOL tree_hasChildren(const tree_t* d)
{
  return (d->m_childrenlen > 0);
}
//static void tree_setText(tree_t* d, const char* t, int l) {str_setstr(d->m_node->m_text, t, l);}
static int tree_array(const tree_t* d, mxArray* arr)
{
  return token_fillArray(d->m_node, arr);
}

int tree_free(tree_t* d)
{
  int i;
  token_free(d->m_node);

  for (i = 0; i < d->m_childrenlen; ++i) {
    tree_free(d->m_children + i);
  }

  free(d->m_children);
  d->m_children = NULL;
  d->m_childrenlen = 0;
  return 0;
}

int tree_init(tree_t* d)
{
  token_set(d->m_node, TOK_INVALID, 0, 0, 0);
  return 0;
}

int tree_settok(tree_t* d, const token_t* tok)
{
  token_settok(d->m_node, tok);
  //token_fillArray(s->d->m_node);
  return 0;
}

int tree_set(tree_t* d, TokenValueType token, unsigned position)
{
  token_set(d->m_node, token, position, 0, 0);
  return 0;
}

int tree_setsize(tree_t* d, int n)
{
  int old_n = d->m_childrenlen;

  if (old_n > n) {
    for (; old_n > n;) {
      --old_n;
      tree_free(d->m_children + old_n);
    }
  }

  d->m_children = (tree_t*)realloc(d->m_children, n * sizeof(tree_t));

  if (old_n < n) {
    memset(d->m_children + old_n, 0, (n - old_n)*sizeof(tree_t));
  }

  d->m_childrenlen = n;
  return 0;
}

static tree_t* tree_addChild_n(tree_t* d, int n)
{
  int old_n = d->m_childrenlen;
  tree_setsize(d, old_n + n);
  return d->m_children + old_n;
}

static tree_t* tree_addChild1(tree_t* d, tree_t* t_child)
{
  tree_t t;
  int old_n = d->m_childrenlen;
  tree_setsize(d, d->m_childrenlen + 1);
  
  if (t_child) {
    CV_SWAP(d->m_children[old_n], *t_child, t);
  }
  
  return d->m_children + old_n;
}

static void tree_addChild2(tree_t* d, tree_t* child1, tree_t* child2)
{
  tree_t t;
  int old_n = d->m_childrenlen;
  tree_setsize(d, d->m_childrenlen + 2);

  if (child1) {
    CV_SWAP(d->m_children[old_n + 0], *child1, t);
  }

  if (child2) {
    CV_SWAP(d->m_children[old_n + 1], *child2, t);
  }
}

int tree_set1(tree_t* d, const token_t* tok, const tree_t* child1)
{
  int old_n = d->m_childrenlen;
  token_settok(d->m_node, tok);
  tree_setsize(d, d->m_childrenlen + 1);
  d->m_children[old_n] = *child1;
  return 0;
}

int tree_set2(tree_t* d, const token_t* tok, const tree_t* child1, const tree_t* child2)
{
  int old_n = d->m_childrenlen;
  token_settok(d->m_node, tok);
  tree_setsize(d, d->m_childrenlen + 2);
  d->m_children[old_n + 0] = *child1;
  d->m_children[old_n + 1] = *child2;
  return 0;
}

int tree_print(const tree_t* d, int indentlevel)
{
  int i;
  char buf[256] = {0};
  memset(buf, ' ', indentlevel);
  printf("%s", buf);
  TokenToString(d->m_node, buf, 256);
  printf("%s\n", buf);
  indentlevel += 3;

  for (i = 0; i < d->m_childrenlen; i++) {
    tree_print(d->m_children + i, indentlevel);
  }

  indentlevel -= 3;
  return 0;
}

int tree_save(const tree_t* d, serialize_t* s)
{
  int i;
  serialize_putBool(s, TRUE); // For compatibility with 3.5?
  token_save(s, d->m_node);
  serialize_putInt(s, d->m_childrenlen);

  for (i = 0; i < d->m_childrenlen; i++) {
    tree_save(d->m_children + i, s);
  }

  return 0;
}

int tree_load(tree_t* d, serialize_t* s)
{
  int i, numchildren;
  BOOL ValidTree = serialize_getBool(s);

  if (!ValidTree) {
    return 0;
  }

  token_load(s, d->m_node);
  numchildren = serialize_getInt(s);
  tree_setsize(d, numchildren);

  for (i = 0; i < numchildren; i++) {
    tree_load(d->m_children + i, s);
  }

  return 0;
}
