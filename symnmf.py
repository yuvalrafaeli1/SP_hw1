import math
import numpy as np
import pandas as pd
import sys
import symnmf as symc

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

#MAIN:
np.random.seed(0)
args = sys.argv
args = args[1:]

if (len(args)!=3):
    print("An Error Has Occurred")
    sys.exit()

k = int(args[0])
data_list, d, N = get_data(args[2])
goal = args[1]

if (goal=="symnmf"):
    W = symc.norm(data_list, N, d)
    H = initial_H(N, k, W)
    result = symc.symnmf(W, H, N ,d, k)
elif (goal=="sym"):
    result = symc.sym(data_list, N, d)
elif (goal=="ddg"):
    result = symc.ddg(data_list, N, d)
elif (goal=="norm"):
    result = symc.norm(data_list, N, d)
else:
    print("An Error Has Occurred")
    sys.exit()


for num in result:
    print(",".join(str("{:.4f}".format(round(x, 4))) for x in num))



#################################


