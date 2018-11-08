#include <omp.h>
#include "LevelSchedule.hpp"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stdlib.h>
//CSR A;
using namespace std;
void ilu0RefCHiLL(int m,int *rowptr, int *colidx, int *diagptr, double*values )
{

  int i,k;
  double tmp;
  for(i=0; i < m; i++)
    {
       for(k= rowptr[i]; k < diagptr[i]; k++)
        {
          values[k] = values[k]/values[diagptr[colidx[k]]]; // a_ik /= a_kk
          int j1 = k + 1;
          int j2 = diagptr[colidx[k]] + 1;

          while (j1 < rowptr[i + 1] && j2 <rowptr[colidx[k] + 1]) {
            if(colidx[j1] == colidx[j2]){
              values[j1] -= values[k]*values[j2]; // a_ij -= a_ik*a_kj
              ++j1; ++j2;
            }
            else if (colidx[j1] < colidx[j2]) ++j1;
            else
                ++j2;
          }
        }
    }
}

int main(){

  int rowptr[501];
    int diagptr[500];
  int colidx[5000];
  double values[5000];
  ilu0RefCHiLL(500, rowptr, colidx, diagptr, values);

  return 0;
}

