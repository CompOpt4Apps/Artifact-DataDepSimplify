

// example from the CHiLL manual page 13

void mm(float **A, float **B, float **C, int ambn, int an, int bm) {
   int wat;
   int i, j, n;
   for(i = 0; i < an; i++)  // loop 1
      wat = i;

         
   for(j = 0; j < bm; j++) { // loop 2
      C[i][j] = 0.0f;
      wat = j;
      for(n = 0; n < ambn; n++) {  // loop 3  with UNKNOWN END CONDITION
         C[i][j] += A[i][n] * B[n][j];
      }
   }
}
