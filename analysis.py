import math
import numpy as np
import pandas as pd
import sys
import symnmf as symc
from sklearn.metrics import silhouette_score

def get_data(f):
    df = pd.read_csv(f, header=None)
    data_list = df.values.tolist()
    #return list, dim, N
    return data_list, len(data_list[0]), len(data_list)


def initial_H(N, k, W):
    H = [[0] * k for i in range(N)]
    m = 0
    for i in range(N):
        for j in range(N):
            m+=W[i][j]
    m = m/(math.pow(N,2))
    rand_numbers = np.random.uniform(0,(2*(math.sqrt((m/k)))), (N*k))
    cnt = 0
    for i in range(N):
        for j in range(k):
            H[i][j] = rand_numbers[cnt]
            cnt+=1
    return H

def differenceV1V2(v1, v2):  # Calculates distance between two vectors
    diffarray = [(v1[i] - v2[i]) for i in range(len(v2))]
    finaldiff = 0
    for j in range(len(v1)):
        finaldiff += pow(diffarray[j], 2)
    return finaldiff


def sum(l1, l2):
    arr = []
    for a in range(len(l1)):
        arr.append(l1[a] + l2[a])
    return arr


def Dmin(centroidslist, v):  # Calculates the closest cluster to a certain vector
    closecluster = 0
    MINdis = differenceV1V2(v, centroidslist[0])
    for j in range(len(centroidslist)):
        dis = differenceV1V2(v, centroidslist[j])
        if dis < MINdis:
            MINdis = dis
            closecluster = j
    return closecluster


def D(v, n):
    for a in range(len(v)):
        v[a] = v[a] / n
    return v


def kmeans(vectors, iter, k):
    e = 0.00001
    centroids = []
    for i in range(k):
        centroids.append(vectors[i])
    it = 0
    cond = False  # condition:updated centroid-pervios centroid<e
    # main code
    while it < iter and cond is False:
        groupvec = [[0, []] for j in range(k)]
        for i in range(len(vectors)):
            centroidi = Dmin(centroids, vectors[i])
            groupvec[centroidi][0] = groupvec[centroidi][0] + 1
            if len(groupvec[centroidi][1]) == 0:  # empty
                for q in range(len(vectors[i])):
                    groupvec[centroidi][1].append(vectors[i][q])
            else:
                updated = sum(groupvec[centroidi][1], vectors[i])
                groupvec[centroidi][1] = updated
        pervius = [[centroids[a][b] for b in range(len(centroids[a]))] for a in
                   range(len(centroids))]
        for z in range(k):
            centroids[z] = D(groupvec[z][1], groupvec[z][0])

        for z in range(k):
            if differenceV1V2(centroids[z], pervius[z]) > e:
                cond = False
                break
            else:
                cond = True
        it = it + 1
    return centroids

def get_centroids_index_kmeans(centroids, vectors):
    index = [0 for i in range(N)]
    for i in range(N):
        index[i] = Dmin(centroids, vectors[i])
    return index

def get_centroids_index_nmf(H,N, k):
    index = [0 for i in range(N)]
    for i in range(N):
        value = H[i][0]
        max_ind = 0
        for j in range(1,k):
            if (H[i][j] > value):
                max_ind = j
                value = H[i][j]
        index[i] = max_ind
    return index

#MAIN:
np.random.seed(0)
args = sys.argv
args = args[1:]

if (len(args)!=2):
    print("An Error Has Occurred")
    sys.exit()

k = int(args[0])
data_list, d, N = get_data(args[1])

#symnmf calculation
W = symc.norm(data_list, N, d)
H = initial_H(N, k, W)
final_H = symc.symnmf(W, H, N ,d, k)
index_nmf = get_centroids_index_nmf(final_H, N, k)

#kmeans calculation
index_kmeans = get_centroids_index_kmeans(kmeans(data_list,300, k), data_list)

kmeans_silhouette = silhouette_score(data_list, index_kmeans, metric='euclidean')
nmf_silhouette = silhouette_score(data_list, index_nmf, metric='euclidean')

print("nmf: " + nmf_silhouette)
print("kmeans: " + kmeans_silhouette)


