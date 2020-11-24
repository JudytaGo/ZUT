"""
@author: Judyta Gogolewska
"""
import numpy as np
import math
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.datasets import load_iris, load_digits
from sklearn.utils.extmath import svd_flip

def wiPCA(X, componentCount):

    meanColumn = X.mean(axis = 0)
    xPca = X - meanColumn
    U, S, V = np.linalg.svd(xPca, full_matrices=False)
    U, V = svd_flip(U, V)
    explained_variance = (S ** 2) / (X.shape[0] - 1)
    components = V[:componentCount, :]
    explained_variance = explained_variance[:componentCount]

    reducedX = np.dot(xPca, components.T)
    return reducedX, explained_variance, components, meanColumn
#Zadanie1#
#a)
x = np.random.randn(200)
y = np.random.randn(200)
points = []
for px, py in zip(x, y):
    points.append([px, py])
points = np.array(points)

plt.figure()

arrowprops=dict(arrowstyle='->')
pca = PCA(2)
pca.fit_transform(points)
#b)
plt.scatter(points[:,0], points[:,1])
plt.title('Zadanie 1b')

plt.figure()

#c)
WI, explained_variance, components, meanColumn = wiPCA(points, 2)
WI1D, explained_variance1D, components1D, meanColumn1D = wiPCA(points, 1)
WI1D = np.dot(WI1D[:,:2], components1D[:,:2]) + meanColumn1D
for length, vector in zip(explained_variance, components):
    v = vector * 2 * np.sqrt(length)
    ax =plt.gca()
    ax.annotate('', meanColumn+v, meanColumn, arrowprops=arrowprops)
plt.scatter(points[:,0], points[:,1])
plt.scatter(WI1D[:,0], WI1D[:,1])
plt.title('Zadanie 1c')

#Zadanie2#
#a)
iris = load_iris()
X = iris.data
y = iris.target

pca = PCA(4)
pca.fit(X)
#b)
reducedX, explained_variance, components, meanColumn = wiPCA(X, 2)

plt.figure(figsize=(16,9))
plt.title('Zadanie 2 - Iris')
plt.scatter(reducedX[:,0], reducedX[:,1], c = y)

#Zadanie3#
#a)
digits = load_digits()
X = digits.data
y = digits.target

pca = PCA(2)
pca.fit(X)
#b)
reducedX, explained_variance, components, meanColumn = wiPCA(X, 2)

#c
plt.figure()
reducedX, explained_variance, components, meanColumn = wiPCA(X, 64)
explained_variance_ratio = explained_variance / explained_variance.sum()
plt.plot(np.cumsum(explained_variance_ratio))
plt.title('Zadanie 3c')
plt.xlabel('Numer składowei')
plt.ylabel('Skumulowana wariancja ')

#d
plt.figure()
plt.title('Zadanie 3d')
plt.scatter(reducedX[:,0], reducedX[:,1], c = y)

#e
plt.figure()
differences = []
componentCounts = list(range(1, 64))

for componentCount in componentCounts:
    reducedX, explained_variance, components, meanColumn = wiPCA(X, componentCount)
    reducedX = np.dot(reducedX[:,:64], components[:,:64]) + meanColumn
    distance=0
    for p1, p2 in zip(X, reducedX):
        distance += math.sqrt(sum((p1 - p2)**2))
    differences.append(distance)
plt.plot(componentCounts, differences)
plt.xlabel('Numer składowei')
plt.ylabel('Dystans')
plt.title('Zadanie 3e')