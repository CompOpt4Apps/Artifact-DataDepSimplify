
//#include <math.h>

extern double sqrt(double in);
//  return in/10;
//}

void ic0_csc(int n, double *val, int * colPtr, int *rowIdx)
{
  int i, k,l,m;
  double temp;
  for (i = 0; i < n - 1; i++){
    temp = val[colPtr[i]];
    val[colPtr[i]] = val[colPtr[i]]/sqrt(temp);//S1

    for (m = colPtr[i] + 1; m < colPtr[i+1]; m++){
      val[m] = val[m] / val[colPtr[i]];//S2
    }

    for (m = colPtr[i] + 1; m < colPtr[i+1]; m++) {
      for (k = colPtr[rowIdx[m]] ; k < colPtr[rowIdx[m]+1]; k++){
        for ( l = m; l < colPtr[i+1] ; l++){
          if (rowIdx[l] == rowIdx[k] ){
            if(rowIdx[l+1] <= rowIdx[k]){
              val[k] -= val[m]* val[l]; //S3
            }
          }
        }
      }
    }
  }
}

