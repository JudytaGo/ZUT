# -*- coding: utf-8 -*-
"""

@author: Judyta Gogolewska
"""

from copy import deepcopy
import scipy as sp
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
plt.rcParams['figure.figsize'] = (16, 9)
plt.style.use('ggplot')

data = pd.read_csv('autos.csv')
data.head()

k = 7

f1 = data['length'].values
f2 = data['width'].values
X = np.array(list(zip(f1, f2)))
plt.scatter(f1, f2, c='black', s=7)

def diste(X,C):
    distances=np.zeros((len(X),len(C)))
    for i in range(len(X)):
        for j in range(len(C)):
            a = (X[i]-C[j])
            a=a.reshape(a.shape+(1,))
            b=a.transpose()
            distances[i][j] = np.sqrt(b.dot(a))
    return distances
    

def diste1(X,C):
    distances=np.zeros((len(X),1))
    for i in range(len(X)):
        for j in range(1):
            a = (X[i]-C[j])
            a=a.reshape(a.shape+(1,))
            b=a.transpose()
            distances[i][j] = np.sqrt(b.dot(a))
    return distances
    
def distm(X,C):
    V=np.cov(X.T)
    V=np.linalg.inv(V)
    distances=np.zeros((len(X),len(C)))
    for i in range(len(X)):
        for j in range(len(C)):
            a = (X[i]-C[j])
            a=a.reshape(a.shape+(1,))
            a=V.dot(a)
            b=a.transpose()
            distances[i][j] = np.sqrt(b.dot(a))
    return distances

def funVal(C,Cx,X):
    bc=0
    wc=0
    for i in range(k):
        points = np.array([X[j] for j in range(len(X)) if Cx[j] == i])
        if len(points) > 0 :
            wc=wc+sum(diste1(points,C[i]))
    dystans=diste(C,C)
    for i in range(k-1):
        for j in range(i+1,k):
            bc=bc+dystans[i][j]
    return bc/wc
    
    


def kmeans6(X,k):
    C_x = np.random.randint(np.min(f1), np.max(f1), size=k)

    C_y = np.random.randint(np.min(f2), np.max(f2), size=k)
    C = np.array(list(zip(C_x, C_y)), dtype=np.float32)
    
    plt.scatter(f1, f2, c='#050505', s=7)
    plt.scatter(C_x, C_y, marker='o', s=200, c='g')

    C_old = np.zeros(C.shape)
    Cx = np.zeros(len(X))
    error = 1
    while error != 0:
        distances=diste(X,C)
        for i in range(len(X)):
            cluster = np.argmin(distances[i])
            Cx[i] = cluster
            C_old = deepcopy(C)
            for i in range(k):
                points = [X[j] for j in range(len(X)) if Cx[j] == i]
                if len(points) > 0:
                    C[i] = np.mean(points, axis=0)
        error = (C==C_old).all()
    return C,Cx
    
    
    
C,Cx=kmeans(X,k)
colors = ['r', 'g', 'b', 'y', 'c', 'm','w']
fig, ax = plt.subplots()
for i in range(k):
    points = np.array([X[j] for j in range(len(X)) if Cx[j] == i])
    if len(points) > 0 :
        ax.scatter(points[:, 0], points[:, 1], s=7, c=colors[i])
        ax.scatter(C[i, 0], C[i, 1], marker='o', s=200, c=colors[i])
        
ocena=funVal(C,Cx,X)
print(ocena)
