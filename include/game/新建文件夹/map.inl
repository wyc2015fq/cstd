
BOOL InitDate(int map[9][8], IPOINT flagpt[10], int& flagn, IPOINT& babypoint, int level)
{
  if (level == 1) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 1, 1, 1, 1, 2, 2, 2,
      1, 1, 0, 0, 1, 1, 2, 2,
      1, 0, 0, 0, 0, 0, 1, 1,
      1, 0, 0, 1, 3, 3, 0, 1,
      1, 0, 0, 0, 0, 3, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 3;
    flagpt[0].y = 6;
    flagpt[1].x = 5;
    flagpt[1].y = 6;
    flagpt[2].x = 6;
    flagpt[2].y = 6;
    flagn = 3;
    babypoint.x = 6;
    babypoint.y = 5;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 2) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 1, 0, 0, 0, 1,
      1, 0, 3, 3, 3, 0, 1, 1,
      1, 0, 0, 1, 0, 0, 1, 2,
      1, 0, 0, 1, 1, 1, 1, 2,
      1, 1, 1, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 1;
    flagpt[0].y = 4;
    flagpt[1].x = 2;
    flagpt[1].y = 6;
    flagpt[2].x = 5;
    flagpt[2].y = 5;
    flagn = 3;
    babypoint.x = 2;
    babypoint.y = 3;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 3) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 1, 0, 1, 1, 3, 1,
      1, 0, 1, 0, 0, 3, 0, 1,
      1, 0, 3, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 0, 0, 1,
      2, 2, 2, 2, 1, 0, 0, 1,
      2, 2, 2, 2, 1, 1, 1, 1
    };
    flagpt[0].x = 1;
    flagpt[0].y = 5;
    flagpt[1].x = 4;
    flagpt[1].y = 5;
    flagpt[2].x = 6;
    flagpt[2].y = 3;
    flagn = 3;
    babypoint.x = 3;
    babypoint.y = 4;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 4) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 1, 1, 1, 1, 1, 2, 2,
      2, 1, 0, 0, 0, 1, 1, 1,
      2, 1, 0, 0, 3, 0, 0, 1,
      2, 1, 1, 3, 3, 0, 0, 1,
      2, 2, 1, 0, 0, 0, 0, 1,
      2, 2, 1, 1, 0, 1, 0, 1,
      2, 2, 2, 1, 0, 0, 0, 1,
      2, 2, 2, 2, 1, 1, 1, 1
    };
    flagpt[0].x = 6;
    flagpt[0].y = 4;
    flagpt[1].x = 5;
    flagpt[1].y = 5;
    flagpt[2].x = 6;
    flagpt[2].y = 7;
    flagn = 3;
    babypoint.x = 3;
    babypoint.y = 5;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 5) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 2, 2, 2,
      1, 0, 0, 0, 1, 1, 1, 1,
      1, 0, 3, 3, 0, 0, 0, 1,
      1, 0, 0, 1, 0, 0, 0, 1,
      1, 0, 0, 1, 1, 0, 1, 1,
      1, 0, 0, 1, 1, 3, 1, 2,
      1, 0, 0, 0, 0, 0, 1, 2,
      1, 1, 1, 1, 1, 1, 1, 2
    };
    flagpt[0].x = 2;
    flagpt[0].y = 4;
    flagpt[1].x = 4;
    flagpt[1].y = 4;
    flagpt[2].x = 5;
    flagpt[2].y = 7;
    flagn = 3;
    babypoint.x = 1;
    babypoint.y = 7;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 6) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 1, 2, 2,
      1, 0, 0, 0, 0, 1, 2, 2,
      1, 0, 0, 1, 0, 1, 1, 1,
      1, 0, 0, 3, 3, 0, 0, 1,
      1, 0, 3, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 2;
    flagpt[0].y = 3;
    flagpt[1].x = 2;
    flagpt[1].y = 4;
    flagpt[2].x = 3;
    flagpt[2].y = 6;
    flagn = 3;
    babypoint.x = 1;
    babypoint.y = 3;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 7) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 1, 0, 0, 1,
      1, 0, 0, 3, 0, 0, 0, 1,
      1, 0, 0, 1, 3, 0, 0, 1,
      1, 0, 0, 3, 0, 0, 1, 1,
      1, 1, 1, 0, 0, 1, 2, 2,
      2, 2, 1, 0, 0, 1, 2, 2,
      2, 2, 2, 1, 1, 2, 2, 2
    };
    flagpt[0].x = 3;
    flagpt[0].y = 2;
    flagpt[1].x = 2;
    flagpt[1].y = 3;
    flagpt[2].x = 5;
    flagpt[2].y = 3;
    flagn = 3;
    babypoint.x = 2;
    babypoint.y = 2;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 8) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 3, 0, 0, 3, 0, 1,
      1, 0, 3, 0, 1, 1, 1, 1,
      1, 0, 0, 1, 1, 2, 2, 2,
      1, 0, 0, 1, 2, 2, 2, 2,
      1, 0, 0, 1, 2, 2, 2, 2,
      1, 1, 1, 1, 2, 2, 2, 2
    };
    flagpt[0].x = 5;
    flagpt[0].y = 2;
    flagpt[1].x = 1;
    flagpt[1].y = 4;
    flagpt[2].x = 3;
    flagpt[2].y = 4;
    flagn = 3;
    babypoint.x = 6;
    babypoint.y = 2;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 9) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 1, 1, 1, 1, 1, 1, 2,
      2, 1, 0, 0, 0, 0, 1, 2,
      2, 1, 0, 0, 1, 0, 1, 2,
      2, 1, 0, 0, 3, 0, 1, 1,
      2, 1, 1, 3, 1, 0, 0, 1,
      2, 1, 0, 0, 0, 1, 0, 1,
      2, 1, 0, 0, 3, 0, 0, 1,
      2, 1, 1, 1, 1, 1, 1, 1
    };
    flagpt[0].x = 3;
    flagpt[0].y = 2;
    flagpt[1].x = 2;
    flagpt[1].y = 7;
    flagpt[2].x = 4;
    flagpt[2].y = 7;
    flagn = 3;
    babypoint.x = 5;
    babypoint.y = 3;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 10) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 1, 0, 0, 0, 1,
      1, 0, 3, 0, 0, 3, 0, 1,
      1, 0, 3, 0, 3, 0, 1, 1,
      1, 0, 3, 0, 0, 3, 0, 1,
      1, 0, 0, 1, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 2;
    flagpt[0].y = 3;
    flagpt[1].x = 2;
    flagpt[1].y = 4;
    flagpt[2].x = 2;
    flagpt[2].y = 5;
    flagpt[3].x = 3;
    flagpt[3].y = 3;
    flagpt[4].x = 3;
    flagpt[4].y = 4;
    flagpt[5].x = 3;
    flagpt[5].y = 5;
    flagn = 6;
    babypoint.x = 1;
    babypoint.y = 4;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 11) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 0, 0, 0, 1,
      1, 0, 3, 0, 0, 0, 0, 1,
      1, 0, 3, 0, 1, 0, 0, 1,
      1, 0, 1, 0, 0, 3, 0, 1,
      1, 0, 0, 0, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 2;
    flagpt[0].y = 3;
    flagpt[1].x = 5;
    flagpt[1].y = 2;
    flagpt[2].x = 5;
    flagpt[2].y = 3;
    flagn = 3;
    babypoint.x = 3;
    babypoint.y = 3;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 12) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 1, 1, 1, 1, 2, 2,
      1, 1, 1, 0, 0, 1, 1, 1,
      1, 0, 0, 0, 0, 3, 0, 1,
      1, 0, 0, 3, 3, 0, 0, 1,
      1, 1, 1, 1, 0, 0, 0, 1,
      2, 2, 2, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 2;
    flagpt[0].y = 4;
    flagpt[1].x = 3;
    flagpt[1].y = 4;
    flagpt[2].x = 6;
    flagpt[2].y = 4;
    flagn = 3;
    babypoint.x = 6;
    babypoint.y = 5;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 13) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 1, 1, 1, 1,
      2, 2, 2, 2, 1, 0, 0, 1,
      1, 1, 1, 1, 1, 0, 0, 1,
      1, 0, 3, 0, 3, 0, 3, 1,
      1, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 0, 0, 0, 0, 1,
      2, 2, 1, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 6;
    flagpt[0].y = 4;
    flagpt[1].x = 4;
    flagpt[1].y = 5;
    flagpt[2].x = 4;
    flagpt[2].y = 6;
    flagn = 3;
    babypoint.x = 5;
    babypoint.y = 2;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 14) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 1, 0, 0, 1,
      1, 0, 1, 0, 3, 0, 3, 1,
      1, 0, 0, 0, 3, 0, 0, 1,
      1, 1, 1, 1, 1, 0, 0, 1,
      2, 2, 1, 0, 0, 0, 0, 1,
      2, 2, 1, 0, 0, 0, 0, 1,
      2, 2, 2, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 3;
    flagpt[0].y = 3;
    flagpt[1].x = 5;
    flagpt[1].y = 5;
    flagpt[2].x = 6;
    flagpt[2].y = 7;
    flagn = 3;
    babypoint.x = 6;
    babypoint.y = 6;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 15) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 1, 1, 1, 1, 2,
      2, 2, 1, 1, 0, 0, 1, 1,
      2, 1, 1, 0, 0, 0, 0, 1,
      1, 1, 0, 3, 1, 3, 1, 1,
      1, 0, 0, 0, 3, 0, 0, 1,
      1, 0, 0, 1, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 2, 2
    };
    flagpt[0].x = 5;
    flagpt[0].y = 3;
    flagpt[1].x = 6;
    flagpt[1].y = 3;
    flagpt[2].x = 5;
    flagpt[2].y = 5;
    flagn = 3;
    babypoint.x = 4;
    babypoint.y = 2;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 16) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 1, 2, 2,
      1, 0, 0, 0, 0, 1, 2, 2,
      1, 0, 3, 3, 1, 1, 1, 1,
      1, 0, 3, 0, 0, 0, 0, 1,
      1, 1, 0, 1, 0, 1, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1
    };
    flagpt[0].x = 4;
    flagpt[0].y = 4;
    flagpt[1].x = 4;
    flagpt[1].y = 5;
    flagpt[2].x = 1;
    flagpt[2].y = 6;
    flagn = 3;
    babypoint.x = 4;
    babypoint.y = 2;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 17) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 1, 1, 1, 1, 2,
      2, 2, 2, 1, 0, 0, 1, 2,
      1, 1, 1, 1, 0, 3, 1, 1,
      1, 0, 0, 3, 0, 0, 0, 1,
      1, 0, 1, 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1, 1, 0, 1,
      1, 0, 0, 0, 3, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1
    };
    flagpt[0].x = 4;
    flagpt[0].y = 4;
    flagpt[1].x = 4;
    flagpt[1].y = 7;
    flagpt[2].x = 6;
    flagpt[2].y = 7;
    flagn = 3;
    babypoint.x = 2;
    babypoint.y = 4;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 18) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 1, 1, 1, 1, 1,
      2, 2, 2, 1, 0, 0, 0, 1,
      2, 1, 1, 1, 0, 0, 0, 1,
      2, 1, 0, 3, 0, 3, 1, 1,
      1, 1, 0, 3, 0, 0, 1, 2,
      1, 0, 0, 0, 1, 0, 1, 2,
      1, 0, 0, 0, 0, 0, 1, 2,
      1, 1, 1, 1, 1, 1, 1, 2
    };
    flagpt[0].x = 1;
    flagpt[0].y = 6;
    flagpt[1].x = 1;
    flagpt[1].y = 7;
    flagpt[2].x = 2;
    flagpt[2].y = 7;
    flagn = 3;
    babypoint.x = 5;
    babypoint.y = 2;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 19) {
    int themap[9][8] = {
      2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 0, 0, 0, 1,
      1, 0, 0, 0, 3, 0, 0, 1,
      1, 0, 1, 0, 0, 1, 1, 1,
      1, 0, 3, 0, 3, 0, 0, 1,
      1, 1, 1, 1, 0, 0, 0, 1,
      2, 2, 2, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 2, 2, 2
    };
    flagpt[0].x = 3;
    flagpt[0].y = 3;
    flagpt[1].x = 4;
    flagpt[1].y = 2;
    flagpt[2].x = 6;
    flagpt[2].y = 5;
    flagn = 3;
    babypoint.x = 6;
    babypoint.y = 2;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  if (level == 20) {
    int themap[9][8] = {
      2, 2, 1, 1, 1, 1, 2, 2,
      1, 1, 1, 0, 0, 1, 1, 2,
      1, 0, 3, 0, 3, 0, 1, 1,
      1, 0, 0, 3, 0, 0, 0, 1,
      1, 0, 1, 0, 3, 0, 0, 1,
      1, 0, 0, 3, 0, 0, 1, 1,
      1, 0, 3, 0, 3, 0, 1, 2,
      1, 1, 0, 0, 1, 1, 1, 2,
      2, 2, 2, 2, 2, 2, 2, 2,
    };
    flagpt[0].x = 2;
    flagpt[0].y = 5;
    flagpt[1].x = 3;
    flagpt[1].y = 2;
    flagpt[2].x = 3;
    flagpt[2].y = 4;
    flagpt[3].x = 3;
    flagpt[3].y = 6;
    flagpt[4].x = 4;
    flagpt[4].y = 3;
    flagpt[5].x = 4;
    flagpt[5].y = 5;
    flagpt[6].x = 5;
    flagpt[6].y = 4;
    flagn = 7;
    babypoint.x = 1;
    babypoint.y = 4;
    memcpy(map, themap, sizeof(themap));
    return TRUE;
  }

  return FALSE;
}

int test_InitDate() {
  int i, j, x, y;
  const char* sstr = " +-#.$^*";
  for (i=0; i<20; ++i) {
    int map[9][8] = {0};
    IPOINT flagpt[10] = {0};
    IPOINT babypoint;
    int flagn = 0;
    int* p;
    InitDate(map, flagpt, flagn, babypoint, i+1);
    for (j=0; j<flagn; ++j) {
      x = flagpt[j].x, y = flagpt[j].y;
      p = &map[y][x];
      *p = *p ? 5 : 4;
    }
    x = babypoint.x, y = babypoint.y;
    p = &map[y][x];
    *p = *p ? 7 : 6;
    printf("M\n");
    for (y=0; y<9; ++y) {
      for (x=0; x<8; ++x) {
        int t = map[y][x];
        printf("%c", sstr[t]);
      }
      printf("\n");
    }
  }
  return 0;
}