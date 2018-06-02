
#ifndef  __CLUSTERING_H__
#define  __CLUSTERING_H__

#include <stdio.h>

typedef struct tagVQ_VECTOR {
  double* Data;    // Input vector
  int nDimension; // Dimension of input vector
  int nCluster;   // Class the vector belong to during clustering
  //Value may changed every epoch
  //char* pFileName;
}
VQ_VECTOR;

typedef struct tagVQ_CENTER {
  double* Data;   // Clustering center vector
  int nDimension; // Dimension of center vector
  int Num;       // Number of vectors belong to the clustering
}
VQ_CENTER;

//Calculate the correlation of two vectors
/*********************************************************************
/* Name:  GetCorrelation
/* Function: Calculate correlation of two vectors
/* Parameter: X -- Vector one
/*    Y -- Vector two
/*    nDimension -- Dimension of the vectors
/* Return:  Correlation of two vectors
/*
/*********************************************************************/
double GetCorrelation(const double* X, const double* Y, int nDimension);

//LBG clustering algorithm
/******************************************************************************
/* Name:  LBGCluster
/* Function: Clustering input vectors using LBG algorithm
/*    Using Euclidean distance
/* Parameter: X -- Input vecters
/*    N -- Number of input vectors
/*    Y -- Clustering result
/*    M -- Number of clustering center
/* Return:  0 -- Correct
/*    1 -- Error
/*
/******************************************************************************/
int LBGCluster(VQ_VECTOR* X, int N, VQ_CENTER* Y, int M);

//LBG clustering algorithm using correlation distance
/******************************************************************************
/* Name:  LBGClusterCor
/* Function: Clustering input vectors using LBG algorithm
/*    Using correlation distance
/* Parameter: X -- Input vecters
/*    N -- Number of input vectors
/*    Y -- Clustering result
/*    M -- Number of clustering center
/* Return:  0 -- Correct
/*    1 -- Error
/*
/******************************************************************************/
int LBGClusterCor(VQ_VECTOR* X, int N, VQ_CENTER* Y, int M);

//Max clustering algorithm
/******************************************************************************
/* Name:  MaxCluster
/* Function: Clustering input vectors using Maximum algorithm
/* Parameter: X -- Input vecters
/*    N -- Number of input vectors
/*    Y -- Clustering result
/*    M -- Number of clustering center
/* Return:  0 -- Correct
/*    1 -- Error
/*
/******************************************************************************/
int MaxCluster(VQ_VECTOR* X, int N, VQ_CENTER* Y, int M);


//Dump clustering result to text file for debugging
/******************************************************************************
/* Name:  DumpClusterData
/* Function: Dump clustering result to a text file for debugging
/* Parameter: fp -- Dump text file name
/*    X -- Input vecters
/*    N -- Number of input vectors
/*    Y -- Clustering result
/*    M -- Number of clustering center
/* Return:  0 -- Correct
/*    1 -- Error
/*
/******************************************************************************/
int DumpClusterData(FILE* fp, VQ_VECTOR* X, int N, VQ_CENTER* Y, int M);

#endif //__CLUSTERING_H__
