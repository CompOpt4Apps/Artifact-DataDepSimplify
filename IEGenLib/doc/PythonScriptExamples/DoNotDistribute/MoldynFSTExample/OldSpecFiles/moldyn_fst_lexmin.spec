#Include the moldyn_fst computation specification
iegen.include('moldyn_fst_comp.spec')

#Define the desired transformations
spec.add_transformation(
    type=iegen.trans.IterPermuteTrans,
    name='lexmin',
    reordering_name='delta',
    iter_sub_space_relation='{[c0,s,c1,i,c2]->[i]: c1=1}',
    target_data_arrays=['x','fx'],
    erg_func_name='ERG_lexmin',
    iter_space_trans=Relation("{[c0,s1,c1,i,c2] -> [c3,s2,c4,j,c5] : s1=s2 && c0=0 && c1=0 && c2=0 && c3=0 && c4=0 && c5=0 && i=j}").union(Relation("{[c6,s3,c7,ii,x] -> [c8,s4,c9,j,y] : s3=s4 && j = delta(ii) && c6=0 && c8=0 && c7=1 && c9=1 && x=y }")))
