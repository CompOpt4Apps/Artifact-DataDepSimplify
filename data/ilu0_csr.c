
void ilu0_csr(int m,int *rowptr, int *colidx, int *diagptr, double*values ) 
{ 

  int i,k, j1, j2,st;
  double tmp;
  for(i=0; i < m; i++) 
  { 
    for(k= rowptr[i]; k < diagptr[i]; k++)
    { 
      //st = diagptr[colidx[k]];
      values[k] = values[k]/values[diagptr[colidx[k]]]; // a_ik /= a_kk
      j1 = k + 1;
      j2 = diagptr[colidx[k]] + 1;
 
      while (j1 < rowptr[i + 1] && j2 <rowptr[colidx[k] + 1])
      {
        if(colidx[j1] == colidx[j2])
        { 
          values[j1] -= values[k]*values[j2]; // a_ij -= a_ik*a_kj
          ++j1; ++j2; 
        }
	else if (colidx[j1] < colidx[j2]) ++j1;
	else                              ++j2;	
      } 
    } 
  } 
}



