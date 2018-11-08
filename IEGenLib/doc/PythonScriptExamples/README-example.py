# Examples from the README, which are also on the web page.
import iegenlib

S1 = iegenlib.Set("{[s,i]: 0<=s && s<T && 0<=i && i<N}")
print "S1 = ", S1
        
R1 = iegenlib.Relation("{[s,i]->[0,s,1,i,0]}")
print "R1 = ", R1

# The following Python statements are examples on how to apply operations 
# (such as Apply, Union, Inverse, and Compose) between sets/relations:

S2 = R1.Apply(S1)           # Apply operation  #
print "S2 = ", S2
        
S3 = iegenlib.Set("{[i,j]: 0<=i and i<n and 5<=j and j<m}")
print "S3 = ", S3
        
S4 = iegenlib.Set("{[i,j]: 0<=i and i<n and m+5<=j and j<m+10}")
print "S4 = ", S4
        
S4 = S3.Union(S4)           # Union operation #
print "S4 = ", S4
        
R2 = iegenlib.Relation("{[i,j]->[ip,jp]: ip=f(i) and jp=2j}")
print "R2 = ", R2
       
R3 = iegenlib.Relation("{[q,r]->[i,j]: i=q and j=r}")
print "R3 = ", R3
        
R3 = R3.Compose(R2)         # Compose operation #
print "R3 = ", R3
        
R2 = R2.Inverse()           # Inverse operation #
print "R2 = ", R2


# Some examples that use function inverse declarations.
iegenlib.appendCurrEnv("f() = inverse f_inv()");

S5 = iegenlib.Set("{[i,j]:i=f(f_inv(j))}")
print "S5 = ", S5

R4 = iegenlib.Relation("{[i,j]->[ip,j]: ip=f(i)}")
print "R4 = ", R4

S6 = R4.Apply(S5)
print "S6 = ", S6

# The user can obtain dot files for SparseConstraints objects of sets/relations 
# from the IEGenlib Python Interface by using toDotString() methods. Python 
# methods open(filename, mode) and write(item) can be used to open and write to 
# the output file, as in the following examples:

S3 = iegenlib.Set("[n,m]->{[i,j]: 0<=i and i<n and 5<=j and j<m}")
         
file = open("S3.dot", 'w')
         
file.write(S3.toDotString())
         
file.close()

