#Parallel program to find interaction energy between particles

from mpi4py import MPI
import numpy as np
import math
import random
import time

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
nprocs = comm.Get_size()
start=time.time()
#POSITIONS IN 2D OF N PARTICLES
x=[]
y=[]
N=100
L=10
for i in range(0,N):
	x.append(random.randrange(-10,10,2))
	y.append(random.randrange(-10,10,2))
#print (x)
#print (y)

rho=N/(L**2)
rc=20
A=(4*math.pi*rc**2)
size1=int(2*rho*A)
#print ('c',size1)

#NEIGHBOUR LIST CREATION
def neighbour(N):
	M=np.zeros((N,size1))
	for i in range(0,N,1):
		M[i][0]=0
		for j in range(0,N,1):
			if (i!=j):
				x1=x[i]
				y1=y[i]
				x2=x[j]
				y2=y[j]
				d=math.sqrt((x1-x2)**2+(y1-y2)**2)
				#print ("d",d)
				if d<rc:
					n=M[i][0]
					n=int(n)
					M[i][n+1]=j
					M[i][0]=M[i][0]+1
	return M
                    

count=int(N/(nprocs-1))
#print ("count",count)
remainder=N%(nprocs-1)
#print ("remainder",remainder)
if rank==0:
	data=np.zeros((N))
	for l in range(1,nprocs):
		count=int(N/(nprocs-1))
		if l<=remainder:
			count=count+1
			for n in range(0,count):
				r,Energy=comm.recv(source=l)
				data[r]=Energy
				#print ("Energy of particle",r,"=",Energy)
			
		else:
			for n in range(0,count):
				r,Energy=comm.recv(source=l)
				data[r]=Energy
				#print ("Energy of particle",r,"=",Energy)
	end=time.time()
	print('time taken',end-start)
	
				
		
			
else:
	k=rank-1
	start=0
	stop=0
	
	if (k<remainder):
		start=k*(count+1)
		stop=start+(count+1)
	else:
		start=k*(count)+remainder
		stop=start+(count)
	for i in range(start,stop,1):
		U=0
		c=neighbour(N)
		#print ("c",c)
		for j in range(1,int(c[i][0]),1):
			if i!=j:
				x1=x[i]
				y1=y[i]
				index=int(c[i][j])
				x2=x[index]
				y2=y[index]
				r=math.sqrt((x1-x2)**2+(y1-y2)**2)
				if r>0:
					U1=4*((1/r**12)-(1/r**6))
					U=U+U1	

		comm.send((i,U),dest=0)

		
		
	
	
	
	
