import sys


def differenceV1V2(v1,v2): #Calculates distance between two vectors
    diffarray = [(v1[i]-v2[i]) for i in range(len(v2))]
    finaldiff=0
    for j in range(len(v1)):
        finaldiff+=pow(diffarray[j],2)
    return finaldiff   
 
def sum(l1, l2): 
    arr = []
    for a in range(len(l1)):
        arr.append(l1[a] + l2[a])
    return arr
def Dmin(centroidslist,v): #Calculates the closest cluster to a certain vector
    closecluster=0
    MINdis=differenceV1V2(v,centroidslist[0])
    for j in range(len(centroidslist)):
        dis=differenceV1V2(v,centroidslist[j])
        if dis<MINdis:
            MINdis=dis
            closecluster=j
    return closecluster
def D(v, n): 
    for a in range(len(v)):
        v[a] = v[a]/n
    return v

def kmeansMAIN(input_data,iter,k):
    e=0.001
    data=open(input_data,"r")
    nowvector=0
    vectors=[]
    for v in data:
        vectorinint=[]
        vectorinstring=v.split(",")
        for x in vectorinstring:
            vectorinint.append(float(x))
        vectors.append(vectorinint)
        if v != "\n":
            nowvector=nowvector+1
    if 1>=k or k>= len(vectors):
        print("Invalid number of clusters!")
        return
    centroids=[]
    for i in range(k):
        centroids.append(vectors[i])
    it=0
    cond=False #condition:updated centroid-pervios centroid<e
    #main code
    while it<iter and cond is False:
        groupvec=[[0,[]] for j in range(k)]
        for i in range(len(vectors)):
            centroidi=Dmin(centroids,vectors[i])
            groupvec[centroidi][0]=groupvec[centroidi][0]+1
            if len(groupvec[centroidi][1])== 0: #empty
                for q in range(len(vectors[i])):
                    groupvec[centroidi][1].append(vectors[i][q])
            else:
                updated=sum(groupvec[centroidi][1],vectors[i])
                groupvec[centroidi][1]=updated
        pervius=[[centroids[a][b] for b in range(len(centroids[a]))] for a in range(len(centroids))] # not shure about it
        for z in range(k):
            centroids[z]=D(groupvec[z][1], groupvec[z][0])

        for z in range(k):
            if differenceV1V2(centroids[z],pervius[z])> e:
                cond=False
                break
            else:
                cond=True
        it=it+1
    for z in range(len(centroids)):
        print(",".join([format(centroids[z][zz],".4f")for zz in range(len(centroids[z]))]))

args=sys.argv
args=args[1:]
defultiter=200
if len(args)==3:
    if not(args[0].isdigit):
        print("Invalid number of clusters!")
    if not(args[1].isdigit):
        print("Invalid maximum iteration!")
    if (args[1].isdigit):
        if(int(args[1])<=1 or int(args[1])>=1000):
            print("Invalid maximum iteration!")
    kmeansMAIN(args[2], int(args[1]),int(args[0]))

if len(args)==2:
    if not(args[0].isdigit):
        print("Invalid number of clusters!")
    kmeansMAIN(args[1], defultiter,int(args[0])) 
