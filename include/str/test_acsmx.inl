//This is a Aho-Corasick state machine used in multiple patterns matching
//2008-11-13
#include "acsm.h"
#define MAXLEN 256
unsigned char tet[MAXLEN];
main(int argc, char** argv)
{
  // int i,
  int nline = 1, k;
  FILE* fd, *fp;
  char filename[MAXLEN];
  unsigned char* text; //the lengh of pattern's file name
  STATEMACHINE* acsm; //the struct of acsm
  STATE* s;
  s = smalloc(); //state 0
  acsm = acsmNew();
  acsm->aStateMachine = s;
  for (k = 0; k < MAXLEN; k++) {
    s->aNextState[k] = NULL;
  }
  s->FailState = s;
  // s->FailState = NULL;
  s->MatchList = NULL;
  strcpy(filename, argv[1]);
  fd = fopen(filename, "r"); //open the patterns file
  if (fd == NULL) {
    fprintf(stderr, "Open pattern file error!\n");
    exit(1);
  }
  while (fgets(tet, MAXLEN, fd)) { //add patterns
    text = tet;
    AddPatternState(acsm, text, strlen(text));
  }
  Build_DFA(acsm);
  strcpy(filename, argv[2]);
  fp = fopen(filename, "r"); //open the text file
  if (fp == NULL) {
    fprintf(stderr, "Open text file error!\n");
    exit(1);
  }
  while (fgets(tet, MAXLEN, fp)) { //search patterns
    text = tet;
    acsmSearch(acsm, text, strlen(text), nline);
    nline++;
  }
  acsmFree(acsm); //free the memory
  printf("THE END!");
  return 0;
}

