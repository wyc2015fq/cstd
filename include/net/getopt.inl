#ifndef _GETOPT_INL_
#define _GETOPT_INL_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// static const char* ID = "$Id: getopt.c,v 1.2 2003/10/26 03:10:20 vindaci Exp $";
char* optarg = NULL;
int optind = 0;
int opterr = 1;
int optopt = '?';
static char** prev_argv = NULL;
static int prev_argc = 0;
static int argv_index = 0;
static int argv_index2 = 0;
static int opt_offset = 0;
static int dashdash = 0;
static int nonopt = 0;
static void increment_index()
{
  if (argv_index < argv_index2) {
    while (prev_argv[++argv_index] && prev_argv[argv_index][0] != '-'
        && argv_index < argv_index2 + 1);
  }
  else {
    argv_index++;
  }
  opt_offset = 1;
}
static int permute_argv_once()
{
  if (argv_index + nonopt >= prev_argc) {
    return 1;
  }
  else {
    char* tmp = prev_argv[argv_index];
    memmove(&prev_argv[argv_index], &prev_argv[argv_index + 1],
        sizeof(char**) * (prev_argc - argv_index - 1));
    prev_argv[prev_argc - 1] = tmp;
    nonopt++;
    return 0;
  }
}
int getopt(int argc, char** argv, char* optstr)
{
  int c = 0;
  if (prev_argv != argv || prev_argc != argc) {
    prev_argv = argv;
    prev_argc = argc;
    argv_index = 1;
    argv_index2 = 1;
    opt_offset = 1;
    dashdash = 0;
    nonopt = 0;
  }
getopt_top:
  optarg = NULL;
  if (argv[argv_index] && !strcmp(argv[argv_index], "--")) {
    dashdash = 1;
    increment_index();
  }
  if (argv[argv_index] == NULL) {
    c = -1;
  }
  else if (dashdash || argv[argv_index][0] != '-' || !strcmp(argv[argv_index], "-")) {
    if (optstr[0] == '-') {
      c = 1;
      optarg = argv[argv_index];
      increment_index();
    }
    else if (optstr[0] == '+' || getenv("POSIXLY_CORRECT")) {
      c = -1;
      nonopt = argc - argv_index;
    }
    else {
      if (!permute_argv_once()) {
        goto getopt_top;
      }
      else {
        c = -1;
      }
    }
  }
  else {
    char* opt_ptr = NULL;
    c = argv[argv_index][opt_offset++];
    if (optstr[0] == '-') {
      opt_ptr = strchr(optstr + 1, c);
    }
    else {
      opt_ptr = strchr(optstr, c);
    }
    if (!opt_ptr) {
      if (opterr) {
        fprintf(stderr, "%s: invalid option -- %c\n", argv[0], c);
      }
      optopt = c;
      c = '?';
      increment_index();
    }
    else if (opt_ptr[1] == ':') {
      if (argv[argv_index][opt_offset] != '\0') {
        optarg = &argv[argv_index][opt_offset];
        increment_index();
      }
      else if (opt_ptr[2] != ':') {
        if (argv_index2 < argv_index) {
          argv_index2 = argv_index;
        }
        while (argv[++argv_index2] && argv[argv_index2][0] == '-');
        optarg = argv[argv_index2];
        if (argv_index2 + nonopt >= prev_argc) {
          optarg = NULL;
        }
        increment_index();
      }
      else {
        increment_index();
      }
      if (optarg == NULL && opt_ptr[2] != ':') {
        optopt = c;
        c = '?';
        if (opterr) {
          fprintf(stderr, "%s: option requires an argument -- %c\n",
              argv[0], optopt);
        }
      }
    }
    else {
      if (argv[argv_index][opt_offset] == '\0') {
        increment_index();
      }
    }
  }
  if (c == -1) {
    optind = argc - nonopt;
  }
  else {
    optind = argv_index;
  }
  return c;
}
#ifdef TEST
#define FLAG_INTERACT 0x0001
#define FLAG_FORCE 0x0002
#define FLAG_RECURSIVE 0x0004
int flags = 0;
int verbose = 5;
const char* in_fname = NULL;
const char* out_fname = NULL;
int arg_to_int(const char* arg, int min, int max, int defalt, const char* opt)
{
  int i = defalt;
  int rv;
  if (!arg) {
    goto done;
  }
  rv = sscanf(arg, "%d", &i);
  if (rv != 1) {
    fprintf(stderr, "%s: integer argument required.\n", opt);
    i = defalt;
    goto done;
  }
  if (i < min || max < i) {
    fprintf(stderr, "%s: argument out of integer range.\n", opt);
    i = defalt;
    goto done;
  }
done:
  return i;
}
void help()
{
  printf(
      "getopt test program\n"
      "Usage: test [OPTION] [INPUT]\n"
      " INPUT set input filename (doesn't do anything)\n"
      " -h help menu (this screen)\n"
      " -i interactive mode (doesn't do anything)\n"
      " -f force mode (doesn't do anything)\n"
      " -r recursive mode (doesn't do anything)\n"
      " -v[level] set verbosity level (5 is default; doesn't do anything)\n"
      " -o filename set output filename (doesn't do anything)\n"
  );
}
int test_getopt(int argc, char* argv[])
{
  while (1) {
    int c = getopt(argc, argv, "-ifrhv::o:");
    if (c == -1) {
      break;
    }
    switch (c) {
    case 'i':
      flags |= FLAG_INTERACT;
      break;
    case 'f':
      flags |= FLAG_FORCE;
      break;
    case 'r':
      flags |= FLAG_RECURSIVE;
      break;
    case 'h':
      help();
      exit(0);
    case 'v':
      verbose = arg_to_int(optarg, 0, 10, 5, "v");
      break;
    case 'o':
      out_fname = optarg;
      break;
    case 1:
      in_fname = optarg;
      break;
#ifdef DEBUG
    default:
      printf("Option '%c' (%d) with '%s'\n", c, c, optarg);
#endif
    }
  }
#ifdef DEBUG
  printf("optind at %d; argv[optind] = '%s'\n", optind, argv[optind]);
#endif
  if (flags & FLAG_INTERACT) {
    printf("in interactive mode\n");
  }
  else {
    printf("not in interactive mode\n");
  }
  if (flags & FLAG_FORCE) {
    printf("in force mode\n");
  }
  else {
    printf("not in force mode\n");
  }
  if (flags & FLAG_RECURSIVE) {
    printf("in recursive mode\n");
  }
  else {
    printf("not in recursive mode\n");
  }
  printf("verbosity level: %d\n", verbose);
  if (in_fname) {
    printf("input filename: %s\n", in_fname);
  }
  else {
    printf("no input filename\n");
  }
  if (out_fname) {
    printf("output filename: %s\n", out_fname);
  }
  else {
    printf("no output filename\n");
  }
  return 0;
}
#endif // TEST
#endif // _GETOPT_INL_

