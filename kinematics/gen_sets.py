import sys

#print 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)
ptr=int(sys.argv[1])
n1=int(sys.argv[2])
inc=int(sys.argv[3])
n2_fin=[490,244,519,271,439,585]
f_out=open('params.txt','w')
for i in range(n1,n2_fin[ptr],inc):
  f_out.write(str(ptr)+","+str(i)+","+str((inc))+"\n")
f_out.close()
