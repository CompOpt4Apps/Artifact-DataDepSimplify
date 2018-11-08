# lcpc12-iegenlib-paper.py
#
# Example used in the LCPC12 submission for IEGenLib.

from iegenlib import *

# The Matrix Powers kernel using the COO sparse matrix data structure.
#   for (k=1; k<=N_k; k++) { 
#     for (p=0; p<nz; p++) { 
#       x[k][row[p]] += a[p]*x[k-1][col[p]];
#     }
#   }

# The iteration space set.
I = Set("{[k,p]  : 0 <= k && k < N_k  && 0 <= p && p < nz }")
print "\nI                   = ", I

# The access relation for array reference x[k-1][col[p]]
A1_I_to_X = Relation("{[k,p] -> [v,w] : v=k-1 && w=col(p) }")
print "\nA1_I_to_X           = ", A1_I_to_X

# The access relation for array reference x[k][row[p]]
A2_I_to_X = Relation("{[k,p] -> [v,w] : v=k && w=row(p) }")
print "\nA2_I_to_X           = ", A2_I_to_X


#### Reorder the second dimension of the X data space.
# Essentially reordering the rows and columns of the square, sparse A matrix.
R_X_to_Xprime = Relation("{[k,i] -> [k,i'] : i' = sigma(i) }")
print "\nR_X_to_Xprime       = ", R_X_to_Xprime

#### Modify the access relations based on R_X_to_X'.
# A1_I_to_X' = R_X_to_X' compose A1_I_to_X
A1_I_to_Xprime = R_X_to_Xprime.Compose(A1_I_to_X)
print "\nA1_I_to_Xprime      = ", A1_I_to_Xprime

# A2_I_to_X' = R_X_to_X' compose A2_I_to_X
A2_I_to_Xprime = R_X_to_Xprime.Compose(A2_I_to_X)
print "\nA2_I_to_Xprime      = ", A2_I_to_Xprime

#### Do not need to modify the iteration space after a data reordering.

#### Specify an overlapping tile schedule.
# Note that the tiling is being performed on the reordered rows.
T_I_to_Iprime = Relation("{[k,p] -> [t,k,i,p] : t=tile(k,i) && i=sigma(row(p)) && 0 <= t && t < N_t && 0 <= i && i < N_r }")
print "\nT_I_to_Iprime       = ", T_I_to_Iprime

#### Modify the access relation based on T_I_to_I'
# A1_I'_to_X' = A1_I_to_X' compose T_I'_to_I
A1_Iprime_to_Xprime = A1_I_to_Xprime.Compose( T_I_to_Iprime.Inverse() )
print "\nA1_Iprime_to_Xprime = ", A1_Iprime_to_Xprime

# A2_I'_to_X' = A2_I_to_X' compose T_I'_to_I
A2_Iprime_to_Xprime = A2_I_to_Xprime.Compose( T_I_to_Iprime.Inverse() )
print "\nA2_Iprime_to_Xprime = ", A2_Iprime_to_Xprime

#### Transform the iteration space with T_I_to_I'
# I' = T_I_to_I'(I)
Iprime = T_I_to_Iprime.Apply( I )
print "\nIprime              = ", Iprime
print "\n"
