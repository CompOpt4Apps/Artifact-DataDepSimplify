int lsolve (int n, int* Lp, int* Li, double* Lx, double *x)
{
    if (!Lp || !Li || !x) return (0) ;  /* check inputs */
    for (int j = 0 ; j < n ; j++)
    {
        x [j] /= Lx [Lp [j]] ; // Diagonal operation
        for (int p = Lp [j]+1 ; p < Lp [j+1] ; p++)
        {
            x [Li [p]] -= Lx [p] * x [j] ; // off-diagonal 
        }
    }
    return (1) ;
}