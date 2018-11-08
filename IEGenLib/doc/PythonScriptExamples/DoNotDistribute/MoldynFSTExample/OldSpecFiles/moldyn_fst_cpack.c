/*! \file
 *
 * Driver for moldyn_fst_cpack example.  Written by hand.
 *
 * \author Alan LaMielle
 */
#include "moldyn_fst_common.c"

int main()
{
	MOLDYN_FST_COMMON_DECLS

	/* Explicit relations */
	ExplicitRelation *sigma;

	MOLDYN_FST_COMMON_ALLOC

	MOLDYN_FST_COMMON_INIT

	MOLDYN_FST_COMMON_ORIG

	/* Print data/index arrays before untransformed computation */
	printf("Before untransformed computation: \n");
	print_arrays(x_notrans,fx_notrans,inter1_notrans,inter2_notrans,N,n_inter);

	/* Perform the untransformed computation */
	printf("Performing untransformed computation... ");
	executor_notrans(N,T,n_inter,fx_notrans,x_notrans,inter1_notrans,inter2_notrans);
	printf("done\n");

	/* Print data/index arrays after untransformed computation */
	printf("After untransformed computation: \n");
	print_arrays(x_notrans,fx_notrans,inter1_notrans,inter2_notrans,N,n_inter);


	/* Print data/index arrays before transformed computation */
	printf("Before transformed computation: \n");
	print_arrays(x_trans,fx_trans,inter1_trans,inter2_trans,N,n_inter);

	/* Perform the transformed computation */
	printf("Calling inspector for transformed computation... ");
	inspector_trans(N,T,n_inter,fx_trans,x_trans,inter1_trans,inter2_trans,&sigma);
	printf("done\n");
	printf("Performing transformed computation... ");
	executor_trans(N,T,n_inter,fx_trans,x_trans,inter1_trans,inter2_trans,&sigma);
	printf("done\n");

	/* Print data/index arrays after transformed computation */
	printf("After transformed computation: \n");
	print_arrays(x_trans,fx_trans,inter1_trans,inter2_trans,N,n_inter);

    ExplicitRelation *sigma_inv = ER_genInverse(sigma);
    reorderArray((unsigned char*)x_trans,sizeof(double),N+-1-0,sigma_inv);
    reorderArray((unsigned char*)fx_trans,sizeof(double),N+-1-0,sigma_inv);

	MOLDYN_FST_COMMON_COMPARE

	MOLDYN_FST_COMMON_FREE
}
