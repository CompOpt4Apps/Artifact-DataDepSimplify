#Loop nest we are targeting:
#  for (s=0; s<T; s++) {
#  for (j=0; j<N; j++) {
#S1:     x[j] = fx[j] * 1.25;
#  }
#
#    for (i=0; i<n_inter; i++) {
#        // simplified computations
#S2:     fx[inter1[i]] += x[inter1[i]] - x[inter2[i]];
#S3:     fx[inter2[i]] += x[inter1[i]] - x[inter2[i]];
#    }
#  }

#Define the symbolic constants for the computation
spec.add_symbolic(
    name='T',
    type='int %s',
    lower_bound=1,
    upper_bound=10) #Number of time steps
spec.add_symbolic(
    name='N',
    type='int %s',
    lower_bound=1) #Number of atoms
spec.add_symbolic(
    name='n_inter',
    type='int %s',
    lower_bound=1) #Number of interactions between atoms

#Define the data arrays for the computation
spec.add_data_array(
    name='x',
    type='double *%s',
    elem_size='sizeof(double)',
    bounds='{[k]: 0<=k && k<N}')

spec.add_data_array(
    name='fx',
    type='double *%s',
    elem_size='sizeof(double)',
    bounds='{[k]: 0<=k && k<N}')

#Define the index arrays for the computation
spec.add_index_array(
    name='inter1',
    type='int *%s',
    input_bounds='{[k]: 0<=k && k<n_inter}',
    output_bounds='{[k]: 0<=k && k<N}')

spec.add_index_array(
    name='inter2',
    type='int *%s',
    input_bounds='{[k]: 0<=k && k<n_inter}',
    output_bounds='{[k]: 0<=k && k<N}')

#Define the statements for the computation
spec.add_statement(
    name='S1',
    text='x[%(a1)s] = fx[%(a2)s] * 1.25;',
    iter_space='{[s,j]: 0<=s && s<T && 0<=j && j<N}',
    scatter='{[s,j]->[c0,s,c1,j,c2]: c0=0 && c1=0 && c2=0}')

spec.add_statement(
    name='S2',
    text='fx[%(a3)s] += x[%(a4)s] - x[%(a5)s];',
    iter_space='{[s,i]: 0<=s && s<T && 0<=i && i<n_inter}',
    scatter='{[s,i]->[c0,s,c1,i,c2]: c0=0 && c1=1 && c2=0}')

spec.add_statement(
    name='S3',
    text='fx[%(a6)s] += x[%(a7)s] - x[%(a8)s];',
    iter_space='{[s,i]: 0<=s && s<T && 0<=i && i<n_inter}',
    scatter='{[s,i]->[c0,s,c1,i,c2]: c0=0 && c1=1 && c2=1}')

#Define the access relations for the statements
spec.add_access_relation(
    statement_name='S1',
    name='a1',
    data_array='x',
    iter_to_data='{[s,i]->[i]}')

spec.add_access_relation(
    statement_name='S1',
    name='a2',
    data_array='fx',
    iter_to_data='{[s,i]->[i]}')

spec.add_access_relation(
    statement_name='S2',
    name='a3',
    data_array='fx',
    iter_to_data='{[s,i]->[k]: k=inter1(i)}')

spec.add_access_relation(
    statement_name='S2',
    name='a4',
    data_array='x',
    iter_to_data='{[s,i]->[k]: k=inter1(i)}')

spec.add_access_relation(
    statement_name='S2',
    name='a5',
    data_array='x',
    iter_to_data='{[s,i]->[k]: k=inter2(i)}')

spec.add_access_relation(
    statement_name='S3',
    name='a6',
    data_array='fx',
    iter_to_data='{[s,i]->[k]: k=inter2(i)}')

spec.add_access_relation(
    statement_name='S3',
    name='a7',
    data_array='x',
    iter_to_data='{[s,i]->[k]: k=inter1(i)}')

spec.add_access_relation(
    statement_name='S3',
    name='a8',
    data_array='x',
    iter_to_data='{[s,i]->[k]: k=inter2(i)}')

#Data dependences
spec.add_data_dependence(
    name='D0',
    dep_rel='{[c0,s,c0,i,c0] -> [c0,s,c1,ii,c0] : c0=0 && c1=1 && i=inter1(ii)}')
