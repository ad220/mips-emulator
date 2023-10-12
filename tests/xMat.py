def produitMatriciel(A,B): return [[sum([A[i][k]*B[k][j] for k in range(3)]) for j in range(3)] for i in range(3)]

A=[[8,1,0],[0,3,9],[7,0,8]]
B=[[7,8,0],[0,6,0],[2,0,6]]

print(produitMatriciel(A,B))