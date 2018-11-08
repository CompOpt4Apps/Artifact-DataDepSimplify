#include <stdio.h>
#include <stdlib.h>

#include <iegen.h>

/*********** Common code blocks ***********/
#define MOLDYN_FST_COMMON_DECLS /* Symbolic constants */ \
	int T=1,N=10,n_inter=5; \
\
	/* Data arrays */ \
	double *x_orig,*fx_orig,*x_notrans,*fx_notrans,*x_trans,*fx_trans;\
\
	/* Index arrays */\
	int *inter1_orig,*inter2_orig,*inter1_notrans,*inter2_notrans,*inter1_trans,*inter2_trans;

#define MOLDYN_FST_COMMON_ALLOC /* Allocate data/index arrays */\
	printf("Allocating data/index arrays... ");\
	alloc_arrays(&x_orig,&fx_orig,&inter1_orig,&inter2_orig,N,n_inter);\
	alloc_arrays(&x_notrans,&fx_notrans,&inter1_notrans,&inter2_notrans,N,n_inter);\
	alloc_arrays(&x_trans,&fx_trans,&inter1_trans,&inter2_trans,N,n_inter);\
	printf("done\n");

#define MOLDYN_FST_COMMON_INIT /* Init data/index arrays */\
	printf("Initializing data/index arrays... ");\
	init_arrays(x_orig,fx_orig,inter1_orig,inter2_orig,N,n_inter);\
	init_arrays(x_notrans,fx_notrans,inter1_notrans,inter2_notrans,N,n_inter);\
	init_arrays(x_trans,fx_trans,inter1_trans,inter2_trans,N,n_inter);\
	printf("done\n");

#define MOLDYN_FST_COMMON_ORIG /* Print data/index arrays before original computation */\
	printf("Before original computation: \n");\
	print_arrays(x_orig,fx_orig,inter1_orig,inter2_orig,N,n_inter);\
\
	/* Perform the original computation */\
	printf("Performing original computation... ");\
	executor_orig(x_orig,fx_orig,inter1_orig,inter2_orig,T,N,n_inter);\
	printf("done\n");\
\
	/* Print data/index arrays after original computation */\
	printf("After original computation: \n");\
	print_arrays(x_orig,fx_orig,inter1_orig,inter2_orig,N,n_inter);

#define MOLDYN_FST_COMMON_COMPARE /* Compare data/index arrays to ensure they are the same */\
	int *index_arrays[][3]={{inter1_orig,inter1_notrans,inter1_trans},\
	                     {inter2_orig,inter2_notrans,inter2_trans}};\
	double *data_arrays[][3]={{x_orig,x_notrans,x_trans},\
	                          {fx_orig,fx_notrans,fx_trans}};\
	const char* row_labels[6]={"inter1","inter2","x","fx"};\
	const char* col_labels[3]={"Orig-NoTrans","Orig-Trans","NoTrans-Trans"};\
	print_comparison(index_arrays,2,n_inter,data_arrays,2,N,row_labels,col_labels);

#define MOLDYN_FST_COMMON_FREE /* Free space allocated for data/index arrays */\
	free_arrays(&x_orig,&fx_orig,&inter1_orig,&inter2_orig);\
	free_arrays(&x_notrans,&fx_notrans,&inter1_notrans,&inter2_notrans);\
	free_arrays(&x_trans,&fx_trans,&inter1_trans,&inter2_trans);
/******************************************/

void alloc_arrays(double **x,double **fx,int **inter1,int **inter2,int N,int n_inter)
{
	alloc_double_array(x,N);
	alloc_double_array(fx,N);
	alloc_int_array(inter1,n_inter);
	alloc_int_array(inter2,n_inter);
}

void free_arrays(double **x,double **fx,int **inter1,int **inter2)
{
	free_double_array(x);
	free_double_array(fx);
	free_int_array(inter1);
	free_int_array(inter2);
}

void init_arrays(double *x,double *fx,int *inter1,int *inter2,int N,int n_inter)
{
	int i;

	/* Init data arrays */
	for(i=0;i<N;i++)
	{
		x[i]=1.0*i;
		fx[i]=2.0*i;
	}

	/* Init index arrays */
	for(i=0;i<n_inter;i++)
	{
		inter1[i]=i;
		inter2[i]=n_inter-i-1;
	}
}

void print_arrays(double *x,double *fx,int *inter1,int *inter2,int N,int n_inter)
{
	printf("x:      "); print_double_array(x,N); printf("\n");
	printf("fx:     "); print_double_array(fx,N); printf("\n");
	printf("inter1: "); print_int_array(inter1,n_inter); printf("\n");
	printf("inter2: "); print_int_array(inter2,n_inter); printf("\n");
}

void executor_orig(double *x,double *fx,int *inter1,int *inter2,int T,int N,int n_inter)
{
	int s,i;

	for(s=0; s<T; s++)
	{
		for(i=0; i<N; i++)
		{
			x[i] = fx[i] * 1.25;
		}

		for(i=0; i<n_inter; i++)
		{
			fx[inter1[i]] += x[inter1[i]] - x[inter2[i]];
			fx[inter2[i]] += x[inter1[i]] - x[inter2[i]];
		}
	}
}

/* Include the untransformed code */
#include "moldyn_fst_notrans.c"

/* Include the transformed code */
#include "moldyn_fst_trans.c"
