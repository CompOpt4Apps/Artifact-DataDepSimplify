//
// Created by kazem on 5/18/17.
//

#ifndef TRNGULAR_LU_LEFT_04_H
#define TRNGULAR_LU_LEFT_04_H


#include <algorithm>

void lu_left_04(int n, int* c, int* r, double* values,
                int nL, int* &lC, int* &lC_up, int* &lR, double* &lValues,
                int nU, int* &uC, int* &uC_up, int* &uR, double* &uValues ){
    /*
     * Performs a LU decomposition on a given matrix (c, r, values), i.e.
     * stored in compressed column format, and produces L and U, which are
     * stored in column compressed format.
     * Pivots are assumed in the diagonals
     * Assuming QR upper bound for the col count of L and U are given ahead of time.
     * lC_up and uC_up store the upper bound of each row.
     */
    double *f = new double[n];
    double *srcCol = new double[n];
    for (int colNo = 0; colNo < n; ++colNo) {
        //Uncompressing a col into a 1D array
        std::fill_n(f,n,0);//Zero initialization
        for (int nzNo = c[colNo]; nzNo < c[colNo + 1]; ++nzNo) {
            f[r[nzNo]]=values[nzNo];//Copying nonzero of the col
        }

        for (int i = 0; i < colNo; ++i) {
            // uncompressing the src col from the factorized part of L
            std::fill_n(srcCol,n,0); //Zero initialization
            for (int l = lC[i]; l < lC_up[i] ; ++l) {
                srcCol[lR[l]]=f[i]*lValues[l];
            }
            //Applying update from the col of L matrix
            for (int j = i+1; j < n; ++j) {
                f[j] = f[j] - srcCol[j]; //f = f - f(curCol)*[zeros(curCol,1); L(curCol+1:n,curCol)]
            }
        }
        //copy the U part from f vector
        int nnzUcur=uC[colNo];
        for (int k = 0; k < colNo; ++k) {
            if(f[k] != 0){
                uR[ nnzUcur ]=k;
                uValues[ nnzUcur ]=f[k];
                nnzUcur++;
            }
        }
        uC_up[colNo]=nnzUcur;
        //copy the L part from f vector
        int nnzLcur=lC[colNo];
        for (int k = colNo; k < n; ++k) {
            if(f[k] != 0){
                lR[ nnzLcur ]=k;
                lValues[ nnzLcur ]=f[k] / f[colNo];
                nnzLcur++;
            }
        }
        lC_up[colNo]=nnzLcur;
    }
    delete []f; delete []srcCol;

}

#endif //TRNGULAR_LU_LEFT_04_H
