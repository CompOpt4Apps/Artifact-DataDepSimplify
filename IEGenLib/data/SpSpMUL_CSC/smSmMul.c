//1) Original Sparse Matrix multiply from CSparse:

  for (j = 0 ; j < n ; j++)
  {
S1: Cp [j] = nz ;                        
    for (p1 = Bp [j] ; p1 < Bp [j+1] ; p1++)
    {
      for (p2 = Ap [Bi[p1]] ; p2 < Ap [Bi[p1]+1] ; p2++)
      {
        if (w [Ai [p2]] < j+1)
        {
          w [Ai [p2]] = j+1 ;
S2:    Ci [nz++] = Ai [p2] ;         
          x [Ai [p2]] = Bx [p1] * Ax [p2] ;
        }
        else {
          x [Ai [p2]] += Bx [p1] * Ax [p2] ;
        }
      }
    }

    for (p = Cp [j] ; p < nz ; p++){
S3:  Cx [p] = x [Ci [p]] ;
    }
  }
  Cp [n] = nz ;


//2) My modified version,

  for (j = 0 ; j < n ; j++)
  {
    std::fill_n(x,n,0); //Zero initialization

S1: Cp [j] = nz ;                        
    for (p1 = Bp [j] ; p1 < Bp [j+1] ; p1++)
    {
      for (p2 = Ap [Bi[p1]] ; p2 < Ap [Bi[p1]+1] ; p2++)
      {
        if ( x[Ai [p2]] == 0)
S2:     Ci [nz++] = Ai [p2] ;      
        x [Ai [p2]] += Bx [p1] * Ax [p2] ;
      }
     }

    for (p = Cp [j] ; p < nz ; p++){
S3: Cx [p] = x [Ci [p]] ;
    }
  }
  Cp [n] = nz ;


//3) My last modified version, assuming we calculate upper bounds 
//    for Cp

  for (j = 0 ; j < n ; j++){
    std::fill_n(x,n,0); //Zero initialization

    int nz = Cp [j];                        
    for (p1 = Bp [j] ; p1 < Bp [j+1] ; p1++){
      for (p2 = Ap [Bi[p1]] ; p2 < Ap [Bi[p1]+1] ; p2++){
        if ( x[Ai [p2]] == 0)
S1:       Ci [nz++] = Ai [p2];      
        x [Ai [p2]] += Bx [p1] * Ax [p2] ;
      }
    }

    for (p = Cp [j] ; p < nz ; p++){
S2:   Cx [p] = x [Ci [p]] ;
    }
  }



