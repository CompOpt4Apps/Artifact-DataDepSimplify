import iegenlib 

# Figure 3 iteration space.
I = iegenlib.Set("[T, R]->{ [time, tri] : 0<=time and time<=T and 0<=tri and tri<=R}")
# Original scattering function
SF = iegenlib.Relation("{ [time, tri]->[0, time, 1, tri, 0] }")

# A statement embedded in the full iteration space.
Ifull = SF.Apply(I)
print "Ifull = ", Ifull


# Original access function
Aorig = iegenlib.Relation("{ [time, tri]->[k] : k = n1(tri) }")
# Access function when the iteration space is embedded in full.
Aorigfull = Aorig.Compose(SF.Inverse())
print "Aorigfull = ", Aorigfull

Inverse operation : 
------------------------
R1 = iegenlib.Relation("{ [time, tri]->[0, time, 1, tri, 0] }")
resultRelation = R1.Inverse()
print  resultRelation

The result is supposed to be: {[0,time,1,tri,0]->[time,tri]}

Actual output:
{ [0, time, 1, tri, 0] -> [time, tri] : time - time = 0 && tri - tri = 0 }


Union operation : 
------------------------
S1 = iegenlib.Set("[n,m]->{ [i,j] : 0<=i and i<=n and 5<=j }")
S2 = iegenlib.Set("[n,m]->{ [i,j] : 0<=i and i<=n and m+5<=j }")
S3 = S1.Union(S2)
print S3

The result is supposed to be: 
    [m,n]->{[i,j] : -m+j-5>=0 and i>=0 and n-i>=0; [i,j] : 
                                                 j-5>=0 and i>=0 and n-i>=0}

Actual output:
{ [i, j] : i + 0 >= 0 && -i + n >= 0 && j - 5 >= 0 } 
               union { [i, j] : i + 0 >= 0 && -i + n >= 0 && j - m - 5 >= 0 }
