// Forward-Solve CSC
// From:
//    Kazem Cheshmi, Shoaib Kamil, Michelle Mills Strout, and Maryam Mehri Dehnavi. 2017. 
//    Sympiler: Transforming Sparse Matrix Codes by Decoupling Symbolic Analysis. 
//    In Proceedings of the International Conference for High Performance Computing, Networking, Storage and Analysis (SC ’17).

void fs_csc(int n, int* Lp, int* Li, double* Lx, double *x)
{
  int j, p;    
  for (j = 0 ; j < n ; j++)
  {
    x[j] /= Lx[Lp[j]] ; // Diagonal operation
    for (p = Lp[j]+1 ; p < Lp[j+1] ; p++)
    {
      x[Li[p]] -= Lx[p] * x[j] ; // off-diagonal 
    }
  }
}

