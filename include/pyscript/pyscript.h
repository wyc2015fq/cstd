#include <stdlib.h>
struct PyObj;
struct PyObjHead {
  const char* type;
  int size;
  void(*__del__)(PyObj* self);
};

#define PYOBJ_HEAD int ref; PyObjHead* type

struct PyObj {
  PYOBJ_HEAD;
};
void _PyObj_Del(PyObj* self) { free(self); }
PyObj* PyObj_New(PyObjHead* type) {
  PyObj* self = (PyObj*)malloc(type->size);
  memset(self, 0, type->size);
  self->type = type;
  self->ref = 1;
  return self;
}

PyObj* PyNulbj_New() {
  static PyObjHead type = { "nul" , sizeof(PyObj), _PyObj_Del };
  PyObj* self = PyObj_New(&type);
  return (PyObj*)self;
}

struct PyIntObj {
  PYOBJ_HEAD;
  int ival;
};


PyObj* PyIntObj_New(int i) {
  static PyObjHead type = { "int" , sizeof(PyIntObj), _PyObj_Del };
  PyIntObj* self = (PyIntObj*)PyObj_New(&type);
  self->ival = i;
  return (PyObj*)self;
}

struct PyFloatObj {
  PYOBJ_HEAD;
  double fval;
};

PyObj* PyIntObj_New(double f) {
  static PyObjHead type = { "float" , sizeof(PyFloatObj), _PyObj_Del };
  PyFloatObj* self = (PyFloatObj*)PyObj_New(&type);
  self->fval = f;
  return (PyObj*)self;
}

#define PYOBJ_VARHEAD PYOBJ_HEAD; int len

struct PyStrObj {
  PYOBJ_VARHEAD;
  char* s;
};
void _PyStrObj_Del(PyObj* self) {
  free(((PyStrObj*)self)->s);
  _PyObj_Del(self);
}

PyObj* PyStrObj_New(const char* s) {
  static PyObjHead type = { "str" , sizeof(PyStrObj), _PyStrObj_Del };
  PyStrObj* self = (PyStrObj*)PyObj_New(&type);
  self->s = _strdup(s);
  self->len = strlen(s);
  return (PyObj*)self;
}

struct PyNameObj {
  PYOBJ_HEAD;
  char* id;
};
PyObj* PyNameObj_New(const char* id) {
  static PyObjHead type = { "name" , sizeof(PyStrObj), _PyObj_Del };
  PyNameObj* self = (PyNameObj*)PyObj_New(&type);
  self->id = _strdup(id);
  return (PyObj*)self;
}

int __statement(PyScanner* env)
{
  switch (env->tok->type) {
    //case OP_VAR: TRY_RETURN(__var_define(env)); break;
  case OP_INDENTIFIER:
  {
    var_t tmp[1] = { 0 };
    TRY_RETURN(__var_assign(env, tmp));
    var_free(tmp);
  }
  break;
  case OP_FUNCTION:
    TRY_RETURN(__func_define(env));
    break;
  case OP_IF:
    TRY_RETURN(__ifcondit(env));
    break;
  case OP_FOR:
    TRY_RETURN(__forloop(env));
    break;
  case OP_WHILE:
    TRY_RETURN(__whileloop(env));
    break;
  case OP_SWITCH:
    TRY_RETURN(__switchcondit(env));
    break;
  case OP_BRACE_L:
    TRY_RETURN(__block(env));
    break;
  case OP_BREAK:
    TRY_RETURN(__break(env));
    break;
  case OP_RETURN:
    TRY_RETURN(__return(env));
    return EXCEP_NOP;
  case OP_SEMICOLON:
    GetToken(env->tokok, env->tok);
    break;
  default:
    THROW(EXCEP_KEYWORD);
  }
  return EXCEP_NOP;
}
PyObj* __exec(PyScanner* s)
{
  PyErr err = PyScanner_next(s);
  PyObj* ret = 0;
  if (err != PyErr_OK) {
    while (s->tok.t != OP_EOF) {
      ret = __statement(s);
    }
  }
  return NULL;
}
PyObj* PyScript_run(PyScanner* s) {
  return __exec(s);
}
