from chill import *

source('mm.c')
procedure('mm')
#format: rose
loop(0)

known('ambn > 0')
known('an > 0')
known('bm > 0')
distribute([0,1], 1)
print_code()
