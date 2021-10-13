import sys

#print 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)
ptr=int(sys.argv[1])
n1=int(sys.argv[2])
n2=int(sys.argv[3])
inc=int(sys.argv[4])
f_out=open('params.txt','w')
for i in range(n1,n2+1,inc+1):
  f_out.write(str(ptr)+","+str(i)+","+str((inc))+"\n")
f_out.close()
