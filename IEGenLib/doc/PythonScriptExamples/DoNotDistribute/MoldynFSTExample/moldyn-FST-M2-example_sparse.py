# Set and Relation operations needed for M2 example.
#
# See OldSpecFiles/moldyn-FST-example.pdf for corresponding writeup.
# Copied from RTRTJournalShared/moldyn-FST-example.tex.
# Also copied .spec files from git repository of python prototype.
#
# Need to keep in mind that the M2 example is not doing pointer update.  
# I want to keep it that way so that we end up with some pretty deep 
# nesting of UFS.
#
# Michelle Strout
# modified to work with iegenlib by Mohammed Al-Refai
# modified again by Michelle Strout to work with iegenlib-1.0.0

import cProfile
import pstats

import iegenlib
from iegenlib import *

###############################################################
# Original computation
#   for (s=0; s<T; s++) {
#    for (i=0; i<N; i++) {
#S1:     x[i] = fx[i]*1.25;
#    }
#    for (ii=0; ii<n_inter; ii++) {
#S2:     fx[inter1[ii]] += x[inter1[ii]] - x[inter2[ii]]; 
#S3:     fx[inter2[ii]] += x[inter1[ii]] - x[inter2[ii]]; 
#    }
#  }
###############################################################


##### Compose scheduling/scattering function for each statement
##### with the statement's original iteration space.

# S1
print "==== S1, text='x[%(a1)s] = fx[%(a2)s] * 1.25;'"
S1_I = Set("[T, N]->{[s,i]: 0<=s && s<T && 0<=i && i<N}")
print S1_I

print "-->S1 schedule, mapping to unified iteration space"
S1_sched = Relation("{[s,i]->[0,s,0,i,0]}")
print S1_sched

print "-->S1 domain in unified iteration space"
S1_full_I = S1_sched.Apply(S1_I)
print S1_full_I

# S2
print
print "==== S2, text='fx[%(a3)s] += x[%(a4)s] - x[%(a5)s];'"
S2_I = Set("[T,N,n_inter]->{[s,i]: 0<=s && s<T && 0<=i && i<n_inter}")
print S2_I

print "-->S2 schedule, mapping to unified iteration space"
S2_sched = Relation("{[s,i]->[0,s,1,i,0]}")
print S2_sched

print "-->S2 domain in unified iteration space"
S2_full_I = S2_sched.Apply(S2_I)
print S2_full_I

# S3
print
print "==== S3, text='fx[%(a6)s] += x[%(a7)s] - x[%(a8)s];'"
S3_I = Set("[T,N,n_inter]->{[s,i]: 0<=s && s<T && 0<=i && i<n_inter}")
print S3_I

print "-->S3 schedule, mapping to unified iteration space"
S3_sched = Relation("{[s,i]->[0,s,1,i,1]}")
print S3_sched

print "-->S3 domain in unified iteration space"
S3_full_I = S3_sched.Apply(S3_I)
print S3_full_I

# full iteration space
print
print "==== Full/Unified Iteration Space"
full_I = S1_full_I.Union(S2_full_I.Union(S3_full_I))
print full_I

#### Specifying the data spaces
print
print "==== x data space"
x_0 = Set("[N] -> {[i] : 0<=i && i<N}")
print x_0


#### Specifying the access functions and then modifying
#### them so that their source is the unified iteration space.

# A1, access functions for S1, targets data array x
print
print "==== A1, access relation for S1 that targets data array x"
a1 = Relation("{[s,i]->[i] }")
print a1
print "Modified a1, or a1_0: "
print "    a1 is original iteration space to data space"
print "    a1_0 is unified iteration space to data space"
print "    a1_0 =  a1 compose (inverse (S1 -> unified))"
print "S1_sched.Inverse() = ", S1_sched.Inverse()
a1_0 = a1.Compose(S1_sched.Inverse())
#a1_0 = Relation("{ [0, s, 0, i, 0] -> [i1] : i - i1 = 0 }")
print a1_0
print a1_0.toString()


# A4, access relation for S2, targets data array x
print
print "==== A4, access relation for S2"
a4 = Relation("{[s,i]->[k]: k=inter1(i)}")
print a4
print "Modified a4, or a4_0 = a4 compose (inverse (S2 -> unified))"
a4_0 = a4.Compose(S2_sched.Inverse())
print a4_0

# A8, access relation for S3, targets data array x
print
print "==== A8, access relation for S3"
a8 = Relation("{[s,i]->[k]: k=inter2(i)}")
print a8
print "Modified a8, or a8_0"
a8_0 = a8.Compose(S3_sched.Inverse())
print a8_0


#### DataPermuteTrans
print
print "==== DataPermuteTrans"

# transformation specification
print
iegenlib.appendCurrEnv("sigma() = inverse sigma_inv()");
R_x0_x1 = Relation("{[k] -> [j] : j = sigma(k)}")
print "data reordering specification: (R_x0_x1) = ", R_x0_x1

# straight-forward specification of input to cpack
print
print "UFS specification for sigma"
cpack_in = Relation("{[ii] -> [p] : p=inter1(ii) } union {[ii] -> [p] : p=inter2(ii) }")
print "    input to cpack: \n        cpack_in = ", cpack_in


# FIXME: Here, we are projecting out variables with the composition
# but we do not have two functions or two function inverses.
# BUT in the paper we were just providing these.
## more involved computation of input to cpack that does
## some testing of the union, inverse, and compose operations
#print "    Computing input to cpack with full iter to subspace of interest"
#all_ar = a1_0.Union( a4_0 )
#all_ar = a8_0.Union( all_ar )
#print "        all_ar = ", all_ar
#iter_sub_space_relation = Relation("{[0,s,1,i,q]->[i]}")
#print "        iter_sub_space_relation = ", iter_sub_space_relation
#print "        ars of interest = inverse (iter_sub_space_relation compose (inverse all_ar))"
#all_ar_inv = all_ar.Inverse()
#print "all_ar.Inverse() = ", all_ar.Inverse()
#ars_of_interest = iter_sub_space_relation.Compose( all_ar.Inverse() ).Inverse()
#print "        = ", ars_of_interest

sigma_domain = x_0
sigma_range = x_0
print
print "    sigma input domain: ", sigma_domain
print "    sigma output range: ", sigma_range

print
print
print "Update modified access relations based on data reordering specification."
print "    data reordering specification (R_x0_x1) = ", R_x0_x1
print
a1_1 = R_x0_x1.Compose(a1_0)
print "    a1_1 = R_x0_x1 compose a1_0 = ", a1_1
print
a4_1 =  R_x0_x1.Compose(a4_0)
print "    a4_1 = R_x0_x1 compose a4_0 = ", a4_1
print
a8_1 =  R_x0_x1.Compose(a8_0)
print "    a8_1 = R_x0_x1 compose a8_0 = ", a8_1



#### Loop Alignment
print
print "==== Loop Alignment"
print "The transformation relation: "
T_I0_to_I1 = Relation('{[0, s, 0, i, 0] -> [0, s, 0, j, 0] : j=sigma(i)}')
T_I0_to_I1 = T_I0_to_I1.Union( Relation('{[0, s, 1, ii, v] -> [0, s, 1, ii, v]}') )
print "    T_I0_to_I1 = ", T_I0_to_I1

print
print "Updating access relations due to T_I0_to_I1: "
a1_2 =  a1_1.Compose( T_I0_to_I1.Inverse() )
print "    a1_2  = a1_1 compose (inverse T_I0_to_I1) [NOTE: needs sigma_inv]"
print "          = ", a1_2
print
a4_2 =  a4_1.Compose( T_I0_to_I1.Inverse() )
print "    a4_2  = a4_1 compose (inverse T_I0_to_I1)"
print "          = ", a4_2
print
a8_2 =  a8_1.Compose( T_I0_to_I1.Inverse() )
print "    a8_2  = a8_1 compose (inverse T_I0_to_I1)"
print "          = ", a8_2
print
print "Updating a data dependence (works because dependence is function inverse: "
D_I0_to_I0 = Relation('{[0,s,0,i,0] -> [0,s,1,ii,0]: i=inter1(ii)}')
print "    D_I0_to_I0 = ", D_I0_to_I0
print
print "Updating the data dependence due to the loop alignment transformation."
print "    D_I1_to_I1 = T_I0_to_I1 compose ( D_I0_to_I0 compose (inverse T_I0_to_I1)"
D_I1_to_I1 = T_I0_to_I1.Compose( D_I0_to_I0.Compose( T_I0_to_I1.Inverse() ) )
print "               = ", D_I1_to_I1





#### IterPermuteTrans
print
print "==== IterPermuteTrans"
iegenlib.appendCurrEnv("delta() = inverse delta_inv()");
T_I1_to_I2 = Relation("{[0,s,0,i,0] -> [0,s,0,i,0]}")
T_I1_to_I2 = T_I1_to_I2.Union( Relation("{[0,s,1,ii,0] -> [0,s,1,j,0] : j = delta(ii) }"))
print "T_I1_to_I2 = ", T_I1_to_I2

# FIXME maybe: again trying to project out variables.  In current
# system just specify the symbolic input to the run-time heuristic.
## straight-forward specification of input to ERG that will create delta 
#print
#print "UFS specification for delta"
#print "    accesses to x in ii loop"
#all_ar = a1_2.Union( a4_2.Union( a8_2 ) )
#print "    all_ar = <CURRENTLY BROKEN>" , all_ar
#delta_ERG_in = iter_sub_space_relation.Inverse().Compose( all_ar )
#print "    input to delta_ERG: "
#print "        delta_ERG_in = <CURRENTLY BROKEN>", delta_ERG_in

delta_domain = Set("{[ii] : 0<=ii and ii<n_inter }")
delta_range = Set("{[ii] : 0<=ii and ii<n_inter }")
print
print "    delta input domain: ", delta_domain
print "    delta output range: ", delta_range

print
print "Updating access relations due to T_I1_to_I2: "
print
a1_3 =  a1_2.Compose( T_I1_to_I2.Inverse() )
print "    a1_3  = a1_2 compose (inverse T_I1_to_I2) = ", a1_3
print
a4_3 =  a4_2.Compose( T_I1_to_I2.Inverse() )
print "    a4_3  = a4_2 compose (inverse T_I1_to_I2)", a4_3
print
a8_3 =  a8_2.Compose( T_I1_to_I2.Inverse() )
print "    a8_3  = a8_2 compose (inverse T_I1_to_I2)", a8_3

print
print "Updating a data dependence: "
print "    D_I1_to_I1 = ",D_I1_to_I1
print
print "Updating the data dependence due to the loop alignment transformation."
D_I2_to_I2 = T_I1_to_I2.Compose( D_I1_to_I1.Compose( T_I1_to_I2.Inverse() ) )
print "    D_I2_to_I2 = T_I1_to_I2 compose ( D_I1_to_I1 compose (inverse T_I1_to_I2)"
print "               = ", D_I2_to_I2





##### SparseTileTrans
print
print "======== sparse tiling transformation"
print "The transformation relation: "
T_I2_to_I3 = Relation("{[0,s,x,i,y] -> [0,s,0,t,x,i,y] : t=theta(x,i)}")
print "    T_I2_to_I3 = ", T_I2_to_I3


# FIXME: again input ER for the sparse tiling function
## straight-forward specification of input to ERG that will create theta 
#print
#print "UFS specification for theta"
#print "    accesses to x in ii loop"
##all_ar = a1_2.Union( a4_2.Union( a8_2 ) )
#print "    all_ar = <CURRENTLY BROKEN>", all_ar
##delta_ERG_in = iter_sub_space_relation.Inverse().Compose( all_ar )
#print "    input to delta_ERG: "
#print "        delta_ERG_in = <CURRENTLY BROKEN>", delta_ERG_in

delta_domain = Set("{[ii] : 0<=ii and ii<n_inter }")
delta_range = Set("{[ii] : 0<=ii and ii<n_inter }")
print
print "    delta input domain: ", delta_domain
print "    delta output range: ", delta_range



print
print "===== Below here has not been edited, MMS 10/12/11"



iter_sub_space_relation=Relation('{[x,s,y,i,z]->[y,i]}')

iter_seed_space_relation=Relation('{[x,s,1,i,z]->[1,i]}')

to_deps=Relation('{[0,i,x] -> [1,j,y] : i=sigma(inter1(delta_inv(j)))}').Union(Relation('{[0,i,x] -> [1,j,y] : i=sigma(inter2(delta_inv(j)))}'))


## Creating the IDG nodes for SparseTileTrans
# See m2-idg-after-sparse-tile.pdf in paper.
# 1) For input need the dependences to and from the seed space.  Currently the
#    user is providing them so no set and/or relation operations are needed.
#    Will need "construct explicit relation" nodes and then the resulting ER 
#    spec nodes.  Will need inputs to the construct node.  For this example, 
#    will need sigma, inter1, inter2, and delta_inv nodes as input to 
#    construct node.    
# 
# 2) Then need an ERG node for full sparse tiling, which takes the created 
#    input nodes as input.
#
# 3) Then need an ER spec for the tiling function theta, which is generated 
#    by the full sparse tiling function.

#tiled_sub_space = iter_sub_space_relation.Apply(I2)



# Alan: When generating the code for the unified iteration spaces
# that result from these schedule changes, you will need to collect
# the non-affine constraints into a guard for the statement.
print
print "Modifying the scheduling function:"
S1_sched = Relation("{[s,i]->[0,s,0,i,0]}")
print "    S1_sched = "
print S1_sched
print "    T_I2_to_I3 compose S1_sched = "
print 'T_I2_to_I3:',T_I2_to_I3
print 'S1_sched:',S1_sched
S1_sched = T_I2_to_I3.Compose(S1_sched)
print 'T_I2_to_I3.compose(S1_sched):',S1_sched
print S1_sched



# The access relations end up with that extra constraint involving the 
# iteration variable from the loops being tiled and theta.  
# When generating code, we will have to have logic that ignores this
# extra constraint.  If we had the equivalent of a gist operation we 
# could get rid of that constraint.
print
print "Updating access relations due to T_I2_to_I3: "
print
print "    a1_4  = a1_3 compose (inverse T_I2_to_I3)"
#print '___START___'
#print 'a1_3:',a1_3
#print 'T_I2_to_I3:',T_I2_to_I3
#print 'T_I2_to_I3.inverse():',T_I2_to_I3.inverse()
a1_4 =  a1_3.Compose( T_I2_to_I3.Inverse() )
#print 'a1_3.compose( T_I2_to_I3.inverse() ):',a1_4
#print '___END___'
print a1_4

print
print "    a4_4  = a4_3 compose (inverse T_I2_to_I3)"
a4_4 =  a4_3.Compose( T_I2_to_I3.Inverse() )
print a4_4

print
print "    a8_4  = a8_3 compose (inverse T_I2_to_I3)"
a8_4 =  a8_3.Compose( T_I2_to_I3.Inverse() )
print a8_4
print

print
print "Updating the data dependences due to T_I2_to_I3:"
print "    D_I2_to_I2 = "
print D_I2_to_I2
print "        D_I3_to_I3 = T_I2_to_I3 compose ( D_I2_to_I2 compose (inverse T_I2_to_I3) ) ) ="
D_I3_to_I3 = T_I2_to_I3.Compose( D_I2_to_I2.Compose( T_I2_to_I3.Inverse() ) )
print "    D_I3_to_I3 = "
print D_I3_to_I3


##### SparseLoopOpt
# The goal of the sparse loop optimization is to get rid of any
# guards in the inner loop that involve UFS.
#spec.add_transformation(
#    type=iegen.trans.SparseLoopOpt,
#    name='sparseLoop',     # name used in IDG I think?
#)
#
# The sparse loop optimization doesn't need much specification because
# initially I think we should have it apply to all loops where there
# is a UFS constraint in the schedule.  Later we can specify individual loops
# for the transformation.  Is this going to be enough info or does user
# have to specify relation for each loop?
# The main questions are (1) which statement does the opt apply,
# (2) which constraint to remove from statement's schedule, 
# (3) relation for projecting out innermost loop from schedule, and
# (4) sparse loop relation with schedule output tuple vars as input
# vars and innermost loop var as an output tuple var.
# The access relations will stay the same so it is important to maintain
# tuple variable names (FIXME: this could be a problem).
#
# Assumption: All statements within a loop have been fused into one statement.
#   Only have one inner sparse loop.
#
# The SparseLoopOpt transformation should work as follows:
#   1) Iterate through all statements and find those with a UFS constraint
#      in the schedule.  For now let's focus on those where the UFS constraint
#      involves the innermost iterator variable.  If we find a case where 
#      this is not true, then assert.
#   2) 







##### SparseTileTrans OLD
## All of the below is how we were going to attempt to figure out the dependences to
## input to Sparse tiling.  Now we are just going to have the user provide them.
## Eventually we need to automate this.
#print
#print "==== SparseTileTrans"
#print "Data dependences to and from seed partitioning space."
#print "Will be computed in calc_input method"
#print "(See iegen/doc/sparse-tile-design.txt for algorithm)."
#print
#print "Input:"
#print "    Direct data dependences:"
#D_1_2 = Relation("{[c0,s,c1,i,c2] -> [c0,s,c3,ii,c2] : i = inter1(ii) && c1=0 && c2=0 && c0=0 && c3=1}")
#D_1_2 = D_1_2.union(Relation("{[c0,s,c1,i,c2] -> [c0,s,c3,ii,c2] : i = inter2(ii) && c1=0 && c2=0 && c0=0 && c3=1}"))
#D_1_3 = D_1_2
#D_2_1 = Relation("{[c0,s,c1,ii,c0] -> [c0,s2,c0,i,c0] : s2 > s && i=inter1(ii) && c0=0 && c1=1}")
#D_2_1 = D_2_1.union(Relation("{[c0,s,c1,ii,c0] -> [c0,s2,c0,i,c0] : s2 > s && i = inter2(ii) && c0=0 && c1=1}"))
#print "        D_1_2 = D_1_3 = "
#print D_1_2
##PrettyPrintVisitor().visit(D_1_2)
#print
#print "        D_2_1 = "
#print D_2_1
##PrettyPrintVisitor().visit(D_2_1)
#print
#print "    Direct data dependences modified by the previous transformations:"
#print
#print "        D_1_2 = T_I0_to_I1 compose ( D_1_2 compose (inverse T_I0_to_I1) ) ) = "
##### some profiling to figure out why compose is so slow
##cProfile.run('T_I0_to_I1.compose( D_1_2.compose( T_I0_to_I1.inverse() ) )','prof')
##p = pstats.Stats('prof')
##p.strip_dirs()
##p.sort_stats('cumulative').print_stats(20)
##p.sort_stats('time').print_stats(20)
##p.print_callers(20)
#####
#
##print '___START___'
##print 'T_I0_to_I1:',T_I0_to_I1
##print 'T_I0_to_I1.inverse():',T_I0_to_I1.inverse()
##print 'D_1_2:',D_1_2
#D_1_2 = T_I0_to_I1.compose( D_1_2.compose( T_I0_to_I1.inverse() ) )
##print 'T_I0_to_I1.compose( D_1_2.compose( T_I0_to_I1.inverse() ) ):',D_1_2
##print '___END___'
#print D_1_2
##PrettyPrintVisitor().visit(D_1_2)
#print
#print "        D_1_2 = T_I1_to_I2 compose ( D_1_2 compose (inverse T_I1_to_I2) ) ) ="
#D_1_2 = T_I1_to_I2.compose( D_1_2.compose( T_I1_to_I2.inverse() ) )
#print D_1_2
##PrettyPrintVisitor().visit(D_1_2)
#print
#print "        D_1_3 = T_I0_to_I1 compose ( D_1_3 compose (inverse T_I0_to_I1) ) ) ="
#D_1_3 = T_I0_to_I1.compose( D_1_3.compose( T_I0_to_I1.inverse() ) )
#print D_1_3
##PrettyPrintVisitor().visit(D_1_3)
#print
#print "        D_1_3 = T_I1_to_I2 compose ( D_1_3 compose (inverse T_I1_to_I2) ) ) ="
#D_1_3 = T_I1_to_I2.compose( D_1_3.compose( T_I1_to_I2.inverse() ) )
#print D_1_3
##PrettyPrintVisitor().visit(D_1_3)
#print
#print "        full_I = ", full_I
#iter_sub_space_relation = Relation("{[c0,s,x,i,y]->[x,i]}")
#iter_seed_space_relation = Relation("{[c0,s,c1,i,c2]->[i] : c1=1}")
#print
#print "        iter_sub_space_relation (issr) = "
#print iter_sub_space_relation
##PrettyPrintVisitor().visit(iter_sub_space_relation)
#print
#print "        iter_seed_space_relation (iseedsr) = "
#print iter_seed_space_relation
##PrettyPrintVisitor().visit(iter_seed_space_relation)
#print
#print "Algorithm:"
#print "        # Dependences that exist within space being sparse tiled"
#D = D_1_3.union(D_1_2)
#print "        relevant dependences = D_1_2 union D_1_3 = ",
#print D
##PrettyPrintVisitor().visit(D)
#print
#print "        Make it so data dependence relations start and end in sparse tiling subspace"
#print "            issr compose D ="
#print iter_sub_space_relation.compose(D)
##PrettyPrintVisitor().visit(iter_sub_space_relation.compose(D))
#print
#print "        D_ST = inverse (issr compose (inverse (issr compose D)))"
##print '___START___'
##print 'iter_sub_space_relation:',iter_sub_space_relation
##print 'iter_sub_space_relation.inverse():',iter_sub_space_relation.inverse()
##print 'D:',D
#D_ST = iter_sub_space_relation.compose(iter_sub_space_relation.compose(D).inverse()).inverse()
##print 'iter_sub_space_relation.compose(iter_sub_space_relation.compose(D).inverse()).inverse():',D_ST
##print '___END___'
#print D_ST
##PrettyPrintVisitor().visit(D_ST)
#print
#print "        NOTE: not doing verification that dependences in D_ST are not loop carried because I don't know how to iterate over the disjuntions or in and out tuples yet."
#print
#print "        1) Count number of statements, for example count = 3"
#print "        2) Compute D_ST_+"
#D_ST_0 = D_ST
#print "            D_ST_0 = D_ST = "
#print D_ST_0
##PrettyPrintVisitor().visit(D_ST_0)
#print
#print "            D_ST_0 compose D_ST = "
#print D_ST_0.compose(D_ST)
##PrettyPrintVisitor().visit(D_ST_0.compose(D_ST))
##print '___START___'
##print 'D_ST_0:',D_ST_0
##print 'D_ST:',D_ST
#D_ST_1 = D_ST_0.compose(D_ST).union(D_ST)
##print 'D_ST_0.compose(D_ST).union(D_ST):',D_ST_1
##print '___END___'
#print "            D_ST_1 = (D_ST_0 compose D_ST) union D_ST = "
#print D_ST_1
##PrettyPrintVisitor().visit(D_ST_1)
#
#print "Output:"
#print "    FROM_SS = "
#print "    TO_SS = "
#
#print 














# ================== Still an issue? MMS 10/12/11
#related to bug #136
#T_I1_to_I2 = T_I1_to_I2.union( Relation("{[c0,s,c1,ii,x] -> [c7,s,c8,j,x] : j = delta(ii) && c0=0  && c1=1 }"))


# ================== Old comments, MMS, 10/14/11
#spec.add_transformation(
#    type=iegen.trans.SparseTileTrans,
#    name='fst',    # name used in IDG I think?
#    tilefunc_name='theta',  #? sparse tiling create a grouping function 
#    #tilefunc_in_arity=2,   #DON'T use, get arity from iter_sub_space_relation
#                            # out arity
#    #tilefunc_out_range='{[z] : 0<=z and z< nt}',
#    num_tile_symbol = 'nt',
#
#    # Mapping from full iteration space to sub space being tiled.
#    iter_sub_space_relation='{[x,s,y,i,z]->[y,i]}', # Tiling across inner loops
#
#    # Mapping from full iteration space to seed space.  Seed space should be 
#    # subset of subspace.
#    iter_seed_space_relation='{[x,s,c1,i,z]->[c1,i] : c1=1}', # Second inner loop
#
#    # Dependences in sub space that end in seed space (to_deps) and start 
#    # in seed space (from_deps).
#    # FIXME: Eventually we want to calculate this instead of having the user 
#    # specify it.
#    to_deps=Relation('{[c0,i,x] -> [c1,j,y] : c0=0 and c1=1 and i=sigma(inter1(delta_inv(j)))}').union(Relation('{[c0,i,x] -> [c1,j,y] : c0=0 and c1=1 and i=sigma(inter2(delta_inv(j)))}'))
#    from_deps=null,    # For M2 there is no third inner loop.
#
#    erg_func_name='ERG_fst',
#
#    iter_space_trans=Relation("{[x,s,y,i,z] -> [x,s,c0,t,y,i,z] : c0=0 and t=theta(y,i)}")
#
#    



####################### And some profiling at the end.
## And some profiling at the end.
#
#cProfile.run('T_I2_to_I3.Compose( D_I2_to_I2.Compose( T_I2_to_I3.Inverse() ) )','prof')
#p = pstats.Stats('prof')
#p.strip_dirs()
#p.sort_stats('cumulative').print_stats(20)
#p.sort_stats('time').print_stats(20)
#p.print_callers(20)





