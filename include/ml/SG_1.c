// F.2 Source code of the SG-1 algorithm
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h> 
//#define DEBUG
extern unsigned _stklen = 50000U;
/***************************************************************************/
int id3( int file_code ) {
#define M1 15 // no of rows in learn.dat > no of items clustered
#define M2 150 // no of atts for ID3 > atts in attrib.dat //it was 150 before
#define M3 5 // length of string describing attribute //it was 4 before
#define M4 141 // no of attributes in attrib.dat + 1 (for EOS) //it was 101 before
  int data_load( char string[ M1 ][ M2 ][ M3 ], int *, int *, char title[ M2 ][ M3 ] );
  int check_all_positive( char string[ M1 ][ M2 ][ M3 ], int, int );
  int check_all_negative( char string[ M1 ][ M2 ][ M3 ], int, int );
  int get_diff_att_types( char valid[ M1 ], char string[ M1 ][ M2 ][ M3 ],
                          char att_names[ M1 ][ M3 ], int, int );
  int create_tree( char rule[ M4 ], char avail_att[ M2 ], FILE * ofp,
                   FILE * nfp, FILE * pfp, char string[ M1 ][ M2 ][ M3 ],
                   char valid[ M1 ], int, int, char title[ M2 ][ M3 ], int );
  int not_all_same( char valid[ M1 ], char string[ M1 ][ M2 ][ M3 ], int attributes );
  int attributes = 0, rows = 0, tab_cnt = -1;
  char string[ M1 ][ M2 ][ M3 ];
  char title[ M2 ][ M3 ];
  char valid[ M1 ];
  char avail_att[ M2 ];
  char rule[ M4 ];
  FILE *ofp, *pfp, *nfp;
  if ( data_load( string, &attributes, &rows, title ) == 999 ) {
    //printf("load\n");
    return 0;
  }
  //printf("%d\n", attributes);
  //printf("%d\n", rows);
  if ( file_code == 1 ) {
    if ( ( ofp = fopen( "d_tree.dat", "w" ) ) == NULL ) {
      printf( "File error : Cannot create output file TREE.DAT\n" );
      return 0;
    }
    if ( ( pfp = fopen( "d_pos.dat", "w" ) ) == NULL ) {
      printf( "File error : Cannot create output file POSITIVE.DAT\n" );
      return 0;
    }
    if ( ( nfp = fopen( "d_neg.dat", "w" ) ) == NULL ) {
      printf( "File error : Cannot create output file NEGATIVE.DAT\n" );
      return 0;
    }
  } else {
    if ( ( ofp = fopen( "d_tree.dat", "a" ) ) == NULL ) {
      printf( "File error : Cannot create output file TREE.DAT\n" );
      return 0;
    }
    if ( ( pfp = fopen( "d_pos.dat", "a" ) ) == NULL ) {
      printf( "File error : Cannot create output file POSITIVE.DAT\n" );
      return 0;
    }
    if ( ( nfp = fopen( "d_neg.dat", "a" ) ) == NULL ) {
      printf( "File error : Cannot create output file NEGATIVE.DAT\n" );
      return 0;
    }
  }
  fprintf( pfp, "rule\n" );
  fprintf( nfp, "rule\n" );
  fprintf( ofp, "\n" );
  if ( check_all_positive( string, attributes, rows ) ) {
    fprintf( ofp, "HALT:all_positive\n" );
    fprintf( pfp, "HALT:all_positive\n" );
    fprintf( nfp, "HALT:all_positive\n" );
    fprintf( pfp, "rule_end\n" );
    fprintf( nfp, "rule_end\n" );
    fclose( ofp );
    fclose( nfp );
    fclose( pfp );
    return 1;
  }
  if ( check_all_negative( string, attributes, rows ) ) {
    fprintf( ofp, "HALT:all_negative\n" );
    fprintf( pfp, "HALT:all_negative\n" );
    fprintf( nfp, "HALT:all_negative\n" );
    fprintf( pfp, "rule_end\n" );
    fprintf( nfp, "rule_end\n" );
    fclose( ofp );
    fclose( nfp );
    fclose( pfp );
    return 1;
  }
  memset ( valid, 42, rows ); // set to '*'
  memset ( avail_att, 42, M2 ); // set to '*'
  memset ( rule, 45, M4 ); // set to '-'
  if ( create_tree( rule, avail_att, ofp, nfp, pfp, string, valid, rows, attributes, title, tab_cnt ) == 999 ) {
    return 0;
  }
  fprintf( pfp, "rule_end\n" );
  fprintf( nfp, "rule_end\n" );
  fclose( ofp );
  fclose( pfp );
  fclose( nfp );
  return 1;
}
/***************************************************************************/
int create_tree( char rule[ M4 ],
                 char avail_att[ M2 ],
                 FILE *ofp,
                 FILE *nfp,
                 FILE *pfp,
                 char string[ M1 ][ M2 ][ M3 ],
                 char valid[ M1 ],
                 int rows, int attributes,
                 char title[ M2 ][ M3 ],
                 int tab_cnt ) {
  int get_diff_att_types( char valid[ M1 ],
                          char string[ M1 ][ M2 ][ M3 ],
                          char att_names[ M1 ][ M3 ],
                          int,
                          int );
  int not_all_same( char valid[ M1 ],
                    char string[ M1 ][ M2 ][ M3 ],
                    int attributes );
  int find_att( char avail_att[ M2 ],
                char string[ M1 ][ M2 ][ M3 ],
                char valid[ M1 ],
                int attributes,
                int rows,
                int function_code,
                int which_best );
  char att_names[ M1 ][ M3 ] = {" "};
  char valid_2[ M1 ];
  char avail_att_2[ M2 ];
  char rule_2[ M4 ];
  char rule_work[ M4 ];
  int j;
  int l;
  int i;
  int ret;
  int tot_diff_atts;
  int att_no;
  
  int function_code;
  int equal_best;
  int which_best;
  int for_each_rule;
  // for (i=0;i<tab_cnt+tab_cnt;++i)
  // {
  // fprintf(ofp,"\t");
  // }
  tab_cnt++;
  which_best = 999;
  function_code = 1;
  if ( ( equal_best = find_att( avail_att, string, valid, attributes, rows, function_code,
                                which_best ) ) == 999 ) {
    return 999;
  }
  // printf("Equal best : %d \n", equal_best); getche();
  for ( for_each_rule = 1; for_each_rule <= equal_best; for_each_rule++ ) {
    // printf("for each rule : %d \n", for_each_rule); getche();
    function_code = 3;
    which_best = for_each_rule;
    if ( ( att_no = find_att( avail_att, string, valid, attributes, rows, function_code,
                              which_best ) ) == 999 ) {
      return 999;
    }
    rule[ M4 -1 ] = '\0'; //make string
    avail_att[ M2 - 1 ] = '\0'; //make string
    strcpy( rule_work, rule );
    strcpy( avail_att_2, avail_att );
    avail_att_2[ att_no ] = ' ';
    for ( i = 0;i < tab_cnt + tab_cnt;++i ) {
      fprintf( ofp, "\t" );
    }
    // tab_cnt++;
    fprintf( ofp, "[%s]\n", title[ att_no ] );
    tot_diff_atts = get_diff_att_types( valid, string, att_names, att_no, rows );
    for ( j = 0;j < tot_diff_atts;++j ) {
      valid[ M1 - 1 ] = '\0';
      strcpy( valid_2, valid );
      for ( l = 0;l < rows;++l ) {
        if ( strcmp( att_names[ j ], string[ l ][ att_no ] ) != 0 ) {
          valid_2[ l ] = ' ';
        }
      }
      if ( ( ret = not_all_same( valid_2, string, attributes ) ) == 1 ) {
        for ( i = 0;i < tab_cnt + tab_cnt + 1;++i ) {
          fprintf( ofp, "\t" );
        }
        fprintf( ofp, ".%s\n", att_names[ j ] );
        rule_work[ att_no - 1 ] = att_names[ j ][ 0 ];
        strcpy( rule_2, rule_work );
        if ( create_tree( rule_2, avail_att_2, ofp, nfp, pfp,
                          string, valid_2, rows, attributes, title, tab_cnt ) == 999 ) {
          return 999;
        }
      } else {
        for ( i = 0;i < tab_cnt + tab_cnt + 1;++i ) {
          fprintf( ofp, "\t" );
        }
        if ( ret == 2 ) {
          fprintf( ofp, ".%s\t - YES\n", att_names[ j ] );
          rule_work[ att_no - 1 ] = att_names[ j ][ 0 ];
          fprintf( pfp, "%s\n", rule_work );
        } else {
          fprintf( ofp, ".%s\t - NO\n", att_names[ j ] );
          rule_work[ att_no - 1 ] = att_names[ j ][ 0 ];
          fprintf( nfp, "%s\n", rule_work );
        }
      }
    }
  } // end for each of the best
  return 1;
}
/***************************************************************************/
int find_att( char avail_att[ M2 ],
              char string[ M1 ][ M2 ][ M3 ],
              char valid[ M1 ],
              int attributes,
              int rows,
              int function_code,
              int which_best ) {
  int get_diff_att_types( char valid[ M1 ],
                          char string[ M1 ][ M2 ][ M3 ],
                          char att_names[ M1 ][ M3 ],
                          int,
                          int );
  void disaster( int );
  int i, j, l, y_tot = 0, n_tot = 0, y_tot_2, n_tot_2;
  int tot_diff_atts;
  int att_no = 0;
  double max_inf_gain = -1.0;
  double entropy, entropy_2, r_entropy_tot;
  double att_entropy[ M2 ];
  char att_names[ M1 ][ M3 ] = {" "};
  char valid_2[ M1 ];
  int equal_best;
  // CHOSE ONE OF THE FOLLOWING
  // THIS IS THE MAX INFO GAIN
  for ( i = 0;i < M2;++i ) {
    att_entropy[ i ] = -2.0;
  }
  // THIS IS THE MIN INFO GAIN
  // for (i=0;i<M2;++i)
  // {
  // att_entropy[i] = 2.0;
  // }
  // CHOSE ONE OF THE ABOVE
  for ( i = 1;i <= M1;i++ ) {
    if ( valid[ i ] == '*' ) {
      if ( strcmp( string[ i ][ attributes ], "yes" ) == 0 ) {
        ++y_tot;
      }
      if ( strcmp( string[ i ][ attributes ], "no" ) == 0 ) {
        ++n_tot;
      }
    }
  }
  if ( y_tot == 0 || n_tot == 0 ) {
    entropy = 0.0;
  } else {
    entropy = 0.0 - ( ( y_tot / ( double ) ( y_tot + n_tot ) )
                      * log( ( y_tot / ( double ) ( y_tot + n_tot ) ) ) )
              - ( ( n_tot / ( double ) ( y_tot + n_tot ) )
                  * log( ( n_tot / ( double ) ( y_tot + n_tot ) ) ) );
  }
  for ( i = 1;i < attributes;++i ) {
    if ( avail_att[ i ] == '*' ) {
      r_entropy_tot = 0.0;
      
      tot_diff_atts = get_diff_att_types( valid, string, att_names, i, rows );
      for ( j = 0;j < tot_diff_atts;++j ) {
        memset ( valid_2, 32, M1 );
        for ( l = 1;l <= rows;++l ) {
          if ( ( strcmp( att_names[ j ], string[ l ][ i ] ) == 0 ) && ( valid[ l ] ==
               '*' ) ) {
            valid_2[ l ] = '*';
          }
        }
        y_tot_2 = 0;
        n_tot_2 = 0;
        for ( l = 1;l <= M1;l++ ) {
          if ( valid_2[ l ] == '*' ) {
            if ( strcmp( string[ l ][ attributes ], "yes" ) == 0 ) {
              ++y_tot_2;
            }
            if ( strcmp( string[ l ][ attributes ], "no" ) == 0 ) {
              ++n_tot_2;
            }
          }
        }
        if ( n_tot_2 == 0 || y_tot_2 == 0 ) {
          entropy_2 = 0.0;
        } else {
          entropy_2 = 0.0 - ( ( y_tot_2 / ( double ) ( y_tot_2 + n_tot_2 ) )
                              *
                              log( ( y_tot_2 / ( double ) ( y_tot_2 + n_tot_2 ) ) ) )
                      -
                      ( ( n_tot_2 / ( double ) ( y_tot_2 + n_tot_2 ) )
                        *
                        log( ( n_tot_2 / ( double ) ( y_tot_2 + n_tot_2 ) ) ) );
        }
        r_entropy_tot = r_entropy_tot + ( entropy_2
                                          *
                                          ( ( n_tot_2 + y_tot_2 ) / ( double ) ( n_tot + y_tot ) ) );
      }
      att_entropy[ i ] = entropy - r_entropy_tot;
    }
  }
  // CHOSE ONE OF THE FOLLOWING
  // THIS IS THE MAX INFO GAIN
  equal_best = 0;
  for ( l = 0;l < M2;++l ) {
    if ( att_entropy[ l ] >= max_inf_gain ) {
      // printf("Att entropy : %f \n", att_entropy[l]); getche();
      // if ((att_entropy[l] == max_inf_gain) && (max_inf_gain >= 0.0))
      // {
      // equal_best++;
      // }
      max_inf_gain = att_entropy[ l ];
      att_no = l;
    }
  }
  if ( function_code == 1 ) {
    for ( l = 0;l < M2;++l ) {
      if ( att_entropy[ l ] >= max_inf_gain ) {
        // printf("Att entropy : %f \n", att_entropy[l]); getche();
        equal_best++;
      }
    }
  }
  // printf("Max info gain : %f \n", max_inf_gain); getche();
  if ( function_code == 3 ) {
    equal_best = 0;
    for ( l = 0;l < M2;++l ) {
      // printf("Att entropy : %f \n", att_entropy[l]); getche();
      if ( att_entropy[ l ] >= max_inf_gain ) {
        att_no = l;
        equal_best++;
      }
      if ( which_best == equal_best ) {
        break;
      }
    }
  }
  // if (equal_best > 0)
  // {
  // printf("Equal best : %d %f %d \n", equal_best, max_inf_gain, att_no); getche();
  // }
  if ( max_inf_gain == 0.0 ) {
    disaster( 1 );
    return 999;
  }
  // THIS IS THE MIN INFO GAIN
  // max_inf_gain = 1.99;
  // for (l=0;l<M2;++l)
  // {
  // if (att_entropy[l] <= max_inf_gain)
  // {
  // max_inf_gain = att_entropy[l];
  // att_no = l;
  // }
  // }
  // if (max_inf_gain == 1.99)
  // {
  // disaster(1);
  // return 999;
  // }
  // CHOOSE ONE OF THE ABOVE
  if ( function_code == 0 ) {
    return att_no;
  }
  if ( function_code == 1 ) {
    return equal_best;
  }
  if ( function_code == 3 ) {
    return att_no;
  }
  return 0;
}
/***************************************************************************/
int not_all_same( char valid[ M1 ], char string[ M1 ][ M2 ][ M3 ], int attributes ) {
  int i, y_tot = 0, n_tot = 0;
  for ( i = 0;i < M1;i++ ) {
    if ( valid[ i ] == '*' ) {
      if ( strcmp( string[ i ][ attributes ], "yes" ) == 0 )
        ++y_tot;
      if ( strcmp( string[ i ][ attributes ], "no" ) == 0 )
        ++n_tot;
    }
  }
  if ( n_tot == 0 )
    return 2; /* all yes */
  else if ( y_tot == 0 )
    return 3; /* all no */
  else
    return 1;
}
/***************************************************************************/
int get_diff_att_types( char valid[ M1 ], char string[ M1 ][ M2 ][ M3 ],
                        char
                        att_names[ M1 ][ M3 ], int att, int max_row ) {
  int j, l, k;
  char att_temp[ M1 ][ M3 ];
  
  for ( j = 0;j < max_row;j++ ) {
    strcpy( att_names[ j ], string[ j ][ att ] );
  }
  for ( l = 0;l < j;++l ) {
    if ( valid[ l ] != '*' )
      memset( att_names[ l ], 42, M3 - 1 );
  }
  for ( j = 0;j < max_row;j++ ) {
    l = 1;
    for ( l = l + j;l < max_row;l++ ) {
      if ( strcmp( att_names[ j ], att_names[ l ] ) == 0 ) {
        memset( att_names[ l ], 42, M3 - 1 );
      }
    }
  }
  for ( l = 0, k = 0;l < j;l++ ) {
    if ( att_names[ l ][ 0 ] != '*' ) {
      strcpy( att_temp[ k ], att_names[ l ] );
      k++;
    }
  }
  for ( l = 0;l < j;++l ) {
    memset( att_names[ l ], 42, M3 - 1 );
  }
  for ( l = 0;l < k;++l ) {
    strcpy( att_names[ l ], att_temp[ l ] );
  }
  for ( l = 0, k = 0;l < j;l++ ) {
    if ( att_names[ l ][ 0 ] != '*' )
      ++k;
  }
  return k;
}
/***************************************************************************/
int check_all_positive( char string[ M1 ][ M2 ][ M3 ], int attributes, int rows ) {
  int i;
  for ( i = 0;i < rows;++i ) {
    if ( strcmp( string[ i ][ attributes ], "no" ) == 0 ) {
      return 0;
    }
  }
  return 1;
}
/***************************************************************************/
int check_all_negative( char string[ M1 ][ M2 ][ M3 ], int attributes, int rows ) {
  int i;
  for ( i = 0;i < rows;++i ) {
    if ( strcmp( string[ i ][ attributes ], "yes" ) == 0 ) {
      return 0;
    }
  }
  return 1;
}
/***************************************************************************/
int data_load( char string[ M1 ][ M2 ][ M3 ], int* a, int* b, char title[ M2 ][ M3 ] ) {
  char linebuff[ 20 ];
  int k = 0;
  FILE *ifp;
  if ( ( ifp = fopen( "d_learn.dat", "r" ) ) == NULL ) {
    printf( "File error : Cannot open input file LEARN.DAT\n" );
    return 999;
  }
  do {
    fscanf( ifp, "%s", title[ k ] );
  } while ( title[ k++ ][ 0 ] != '*' );
  do {
    ( *a ) = 0;
    do {
      fscanf( ifp, "%s", linebuff );
      strcpy( string[ ( *b ) ][ ( *a ) ], linebuff );
      ( *a ) ++;
    } while ( linebuff[ 0 ] != '*' );
    ( *b ) ++;
  } while ( linebuff[ 1 ] != '*' );
  *a = *a - 2;
  fclose( ifp );
  return 1;
}
/***************************************************************************/
void disaster( int i ) {
  switch ( i ) {
  case 1:
    printf( "** ID3 failure **\n" );
    //system("cls");
    //printf("\nA serious error has occured.\n\n");
    //printf("All output files may be corrupt.\n\n");
    //printf("Possible inconistancies or contradictory\n");
    //printf("input cases may be the cause.\n\n");
    //printf("\n\nPress any key");
    //getche();
    break;
  }
}


