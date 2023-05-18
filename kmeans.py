import sys

def calc_sum(list1, list2):   # A function that returns the sum of two vectors
    summation = []
    for i in range(len(list1)):
        summation.append(list1[i] + list2[i])
    return summation
def divide(vector, num):  # dividing all elements in a vector
    for i in range(len(vector)):
        vector[i] = vector[i]/num
    return vector

def differenceV1V2(v1,v2): #Calculates distance between two vectors
    diffarray = [(v1[i]-v2[i]) for i in range(len(v2))]
    finaldiff=0
    for j in range(len(v1)):
        finaldiff+=pow(diffarray[j],2)
    return finaldiff    

def Dmin(centroidslist,v): #Calculates the closest cluster to a certain vector
    closecluster=0
    MINdis=differenceV1V2(v,centroidslist[0])
    for j in range(len(centroidslist)):
        dis=differenceV1V2(v,centroidslist[j])
        if dis<MINdis:
            MINdis=dis
            closecluster=j
    return closecluster

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
                updated=calc_sum(groupvec[centroidi][1],vectors[i])
                groupvec[centroidi][1]=updated
        pervius=[[centroids[a][b] for b in range(len(centroids[it])) for a in range(len(centroids))]] # not shure about it
        for z in range(k):
            centroids[z]=divide(groupvec[z][1], groupvec[z][0])

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
    if (args[1].isdigit) and (args[1]<=1 or args[1]>=1000):
        print("Invalid maximum iteration!")
    kmeansMAIN(args[2], int(args[1]),int(args[0]))

if len(args)==2:
    if not(args[0].isdigit):
        print("Invalid number of clusters!")
    kmeansMAIN(args[2], defultiter,int(args[0])) 



