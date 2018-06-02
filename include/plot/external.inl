
#include "external.h"

#ifdef HAVE_EXTERNAL_FUNCTIONS

#include "alloc.h"
#include "command.h"  /* for c_token and dummy_func */
#include "util.h" /* for int_warn, int_error */
#include "plot.h" /* for gp_expand_tilde */

#include <string.h>

typedef struct value(*exfn_t)(int, struct value*, void*);
typedef void* (*infn_t)(exfn_t);
typedef void (*fifn_t)(void*);

struct exft_entry {
  exfn_t exfn;
  fifn_t fifn;
  struct udft_entry* args;
  void* private;
};

void
f_calle(union argument* x)
{
  struct value r = x->exf_arg->exfn(x->exf_arg->args->dummy_num,
      x->exf_arg->args->dummy_values,
      x->exf_arg->private);

  if (r.type == INVALID_VALUE) {
    r = udv_NaN->udv_value;
  }

  push(&r);
}

/*
  Parse the sring argument for a dll filename and function.  Create a
  one-item action list that calls a plugin function.  Call the _init,
  if present.  _init may return a pointer to private data that is
  handed over to the external function for each call.
*/

struct at_type*
external_at(const char* func_name) {
  char* file = NULL;
  char* func;
  gp_dll_t dl;
  exfn_t exfn;
  infn_t infn;
  fifn_t fifn;
  struct at_type* at = NULL;

  if (!isstring(c_token)) {
    int_error(c_token, "expecting external function filename");
  }

  /* NB: cannot use try_to_get_string() inside an expression evaluation */
  m_quote_capture(&file, c_token, c_token);

  if (!file) {
    int_error(c_token, "expecting external function filename");
  }

  gp_expand_tilde(&file);

  func = strrchr(file, ':');

  if (func) {
    func[0] = 0;
    func++;
  }
  else {
    func = (char*)func_name;
  }

  /* 1st try:  "file" */
  dl = DLL_OPEN(file);

  if (!dl) {
    char* err = DLL_ERROR(dl);
    char* s;
#ifdef DLL_PATHSEP
    int no_path = !(s = strrchr(file, DLL_PATHSEP[0]));
#else
    int no_path = 0;
#endif
#ifdef DLL_EXT
    int no_ext  = !strrchr(no_path ? file : s, '.');
#else
    int no_ext = 0;
#endif

    if (no_path || no_ext) {
      char* nfile = gp_alloc(strlen(file) + 7, "exfn filename");

#ifdef DLL_EXT

      if (no_ext) {
        strcpy(nfile, file);
        strcat(nfile, DLL_EXT);
        /* 2nd try:  "file.so" */
        dl = DLL_OPEN(nfile);
      }

#endif
#ifdef DLL_PATHSEP

      if (!dl && no_path) {
        strcpy(nfile, "." DLL_PATHSEP);
        strcat(nfile, file);
        /* 3rd try:  "./file" */
        dl = DLL_OPEN(nfile);
#ifdef DLL_EXT

        if (!dl && no_ext) {
          strcat(nfile, DLL_EXT);
          /* 4th try:  "./file.so" */
          dl = DLL_OPEN(nfile);
        }

#endif
      }

#endif
      free(nfile);
    }

    if (!dl) {
      if (!err || !*err) {
        err = "cannot load external function";
      }

      int_warn(c_token, err);
      goto bailout;
    }

  }

  exfn = (exfn_t)DLL_SYM(dl, func);

  if (!exfn) {
    char* err = DLL_ERROR(dl);

    if (!err) {
      err = "external function not found";
    }

    int_warn(c_token, err);
    goto bailout;
  }

  infn = (infn_t)DLL_SYM(dl, "gnuplot_init");
  fifn = (fifn_t)DLL_SYM(dl, "gnuplot_fini");

  if (!(at = gp_alloc(sizeof(struct at_type), "external_at"))) {
    goto bailout;
  }

  memset(at, 0, sizeof(*at));   /* reset action table !!! */

  at->a_count = 1;
  at->actions[0].index = CALLE;

  at->actions[0].arg.exf_arg = gp_alloc(sizeof(struct exft_entry), "external_at");

  if (!at->actions[0].arg.exf_arg) {
    free(at);
    at = NULL;
    goto bailout;
  }

  at->actions[0].arg.exf_arg->exfn = exfn;  /* must be freed later */
  at->actions[0].arg.exf_arg->fifn = fifn;
  at->actions[0].arg.exf_arg->args = dummy_func;

  if (!infn) {
    at->actions[0].arg.exf_arg->private = 0x0;
  }
  else {
    at->actions[0].arg.exf_arg->private = (*infn)(exfn);
  }

bailout:
  c_token++;
  free(file);
  return at;
}

/*
   Called with the at of a UDF about to be redefined.  Test if the
   function was external, and call its _fini, if any.
*/

void
external_free(struct at_type* at)
{
  if (at
      && at->a_count == 1
      && at->actions[0].index == CALLE
      && at->actions[0].arg.exf_arg->fifn) {
    (*at->actions[0].arg.exf_arg->fifn)(at->actions[0].arg.exf_arg->private);
  }
}

#endif /* HAVE_EXTERNAL_FUNCTIONS */
