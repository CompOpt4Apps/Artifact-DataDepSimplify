
for(i=0;i<n;i++) {
    tmp = b[i];
    for (j=rowPtr[i]; j<rowPtr[i+1]-1;j++) {
S1:     tmp -= lowerTriSpMatrix[j]*x[colIdx[j]];
    }
S2: x[i] = tmp / lowerTriSpMatrix[rowPtr[i+1]-1];
}
