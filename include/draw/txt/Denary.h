www.pudn.com _ denaryÈý½ÇÆÊ·Ö.rar _ Denary.h_ change_2004-05-13_size_3397b

    

  
// Denary.h: interface for the CDenary class. 
/* 
	// TODO: Add extra initialization here 
	  FILE *fp; 
	  char str[2048]; 
	  fp=fopen("test.dat","r"); 
	  int n=0; 
	  while(fgets(str,2048,fp)) 
	  { 
		  n++; 
	  }; 
	  fclose(fp); 
	  CDenary mydenary; 
	  mydenary.alloc_memory(n); 
	  fp=fopen("test.dat","r"); 
	  for (int i = 0; i < n; i++) 
	  { 
		  fscanf(fp,"%f %f", &mydenary.p_array[i].x, &mydenary.p_array[i].y); 
	  } 
	  fclose(fp); 
	  mydenary.Compute(n); 
	  CString cs; 
	  for(i=0;i<mydenary.ary1.GetSize();i++) 
	  { 
		  cs.Format("%d %d %d",mydenary.ary1.GetAt(i),mydenary.ary2.GetAt(i),mydenary.ary3.GetAt(i)); 
		  AfxMessageBox(cs); 
	  } 
*/ 
////////////////////////////////////////////////////////////////////// 
 
#if !defined(AFX_DENARY_H__A9951392_28B0_43B2_82C1_6FF55F3F1065__INCLUDED_) 
#define AFX_DENARY_H__A9951392_28B0_43B2_82C1_6FF55F3F1065__INCLUDED_ 
 
#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 
#define SYSV 
#define OUTPUT 
#define TIME 
 
#ifndef NULL 
#define  NULL  0 
#endif 
#define  TRUE  1 
#define  FALSE  0 
#define  Org(e)    ((e)->org) 
#define  Dest(e)    ((e)->dest) 
#define  Onext(e)  ((e)->onext) 
#define  Oprev(e)  ((e)->oprev) 
#define  Dnext(e)  ((e)->dnext) 
#define  Dprev(e)  ((e)->dprev) 
 
#define  Other_point(e,p)  ((e)->org == p ? (e)->dest : (e)->org) 
#define  Next(e,p)  ((e)->org == p ? (e)->onext : (e)->dnext) 
#define  Prev(e,p)  ((e)->org == p ? (e)->oprev : (e)->dprev) 
 
#define  Visited(p)  ((p)->f) 
 
#define Identical_refs(e1,e2)  (e1 == e2) 
#define Vector(p1,p2,u,v) (u = p2->x - p1->x, v = p2->y - p1->y) 
 
#define Cross_product_2v(u1,v1,u2,v2) (u1 * v2 - v1 * u2) 
 
#define Cross_product_3p(p1,p2,p3) ((p2->x - p1->x) * (p3->y - p1->y) - (p2->y - p1->y) * (p3->x - p1->x)) 
 
#define Dot_product_2v(u1,v1,u2,v2) (u1 * u2 + v1 * v2) 
/* Edge sides. */ 
typedef enum {right, left} side; 
 
/* Geometric and topological entities. */ 
typedef  unsigned int index; 
typedef  unsigned int cardinal; 
typedef  int integer; 
typedef  float  real; 
typedef  float  ordinate; 
typedef  unsigned char boolean; 
typedef  struct   point   point; 
typedef  struct  edge  edge; 
 
struct point { 
  ordinate x,y,z; 
  edge *entry_pt; 
}; 
 
struct edge { 
  point *org; 
  point *dest; 
  edge *onext; 
  edge *oprev; 
  edge *dnext; 
  edge *dprev; 
}; 
 
class CDenary   
{ 
public: 
	CDenary(); 
	virtual ~CDenary(); 
	void Compute(cardinal n); 
	void alloc_memory(cardinal n); 
	point *p_array; 
	CArray<int,int>ary1; 
	CArray<int,int>ary2; 
	CArray<int,int>ary3; 
private: 
	edge * CDenary::make_edge(point *u, point *v); 
	void merge_sort(point *p[], point *p_temp[], index l, index r); 
	void divide(point *p_sorted[], index l, index r, edge **l_ccw, edge **r_cw); 
	void splice(edge *a, edge *b, point *v); 
	edge * join(edge *a, point *u, edge *b, point *v, side s); 
	void merge(edge *r_cw_l, point *s, edge *l_ccw_r, point *u, edge **l_tangent); 
	void lower_tangent(edge *r_cw_l, point *s, edge *l_ccw_r, point *u, 
			  edge **l_lower, point **org_l_lower, 
			  edge **r_lower, point **org_r_lower); 
	void delete_edge(edge *e); 
	void free_edge(edge *e); 
	edge * get_edge(); 
	void free_memory(); 
	void print_results(cardinal n); 
	edge *e_array; 
	edge **free_list_e; 
	cardinal n_free_e; 
}; 
 
#endif // !defined(AFX_DENARY_H__A9951392_28B0_43B2_82C1_6FF55F3F1065__INCLUDED_) 

 
