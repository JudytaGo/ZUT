"""
@author: Judyta Gogolewska
"""

import numpy as np
import matplotlib.pyplot as plt
import scipy as scipy
import numpy.random as rng
import pandas as pd
from copy import deepcopy
from sklearn.cluster import AgglomerativeClustering
from sklearn.cluster import KMeans
from sklearn import datasets
from sklearn.decomposition import PCA
from sklearn.metrics import jaccard_score
from sklearn.mixture import GaussianMixture
from scipy.spatial import ConvexHull
from scipy.cluster import hierarchy
from matplotlib.gridspec import GridSpec
from mpl_toolkits.axes_grid1 import make_axes_locatable
from mpl_toolkits.mplot3d import Axes3D

def diste(X,C):
    distances=np.zeros((len(X),len(C)))
    for i in range(len(X)):
        for j in range(len(C)):
            a = (X[i]-C[j])
            a=a.reshape(a.shape+(1,))
            b=a.transpose()
            distances[i][j] = np.sqrt(b.dot(a))
    return distances


def kmeans6(X,k):
    C_x = np.random.randint(-5, 5, size=k)

    C_y = np.random.randint(-2, 2, size=k)
    C = np.array(list(zip(C_x, C_y)), dtype=np.float32)

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
    return Cx.astype(int)


### Zadanie 2.1 ###
iris = datasets.load_iris()
X = iris.data
Y = iris.target
zoo = pd.read_csv('zoo.csv')
zooColumns = [ 'hair', 'feathers', 'eggs', 'milk', 'airborne', 'aquatic', 'predator', 
                'toothed', 'backbone', 'breathes', 'venomous', 'fins', 'legs', 'tail', 'domestic', 'catsize']
XZOO = np.array(zoo[zooColumns])
YZOO = np.array(zoo.type.astype('category').cat.codes)
### Zadanie 2.2 ###
def NarysujSubplotsAggloClus(idx,linkage):
    plt.subplot(1, 4, idx + 1)
    model = AgglomerativeClustering(linkage=linkage,n_clusters=3)
    model.fit(X)
    plt.scatter(X[:, 0], X[:, 1], c=model.labels_,cmap=plt.cm.nipy_spectral)
    plt.title(linkage)

figure = plt.figure(figsize=(20,5))
NarysujSubplotsAggloClus(0,'average')
NarysujSubplotsAggloClus(1,'complete')
NarysujSubplotsAggloClus(2,'ward')
NarysujSubplotsAggloClus(3,'single')
    
#Odp. Metody Aglomeracyjne działają tak, że zaczynają łączenie od 1-elementowych clustrów, łącząc je w coraz większe zbiory.

### Zadanie 2.3 ###

def ﬁnd_perm(clusters, Y_real, Y_pred):
    perm=[] 
    for i in range(clusters):
        idx = Y_pred == i
        new_label=scipy.stats.mode(Y_real[idx])[0][0]
        perm.append(new_label)
    return [perm[label] for label in Y_pred]

Y_real = np.array([0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2])
Y_pred1 = np.array([1, 1, 1, 2, 2, 2, 2, 2, 0, 0, 0, 0])
Y_pred2 = np.array([1, 1, 1, 2, 1, 2, 0, 2, 0, 2, 0, 0])
print(ﬁnd_perm(3, Y_real, Y_pred1))
print(ﬁnd_perm(3, Y_real, Y_pred2))
#Odp. Funkcja ta próbuje znaleźć najbardziej odpowiadające ułożenie Y_real do Y_pred

### Zadanie 2.4 ###

print(jaccard_score(Y_real, Y_pred1, average=None))
print(jaccard_score(Y_real, Y_pred2, average=None))
print(jaccard_score(Y_real, ﬁnd_perm(3, Y_real, Y_pred1), average=None))
print(jaccard_score(Y_real, ﬁnd_perm(3, Y_real, Y_pred2), average=None))
#Odp. Indeks Jaccarda mówi nam jak podobne są dwa klastry, im bardziej są podobne tym większy współczynnik


def plotConvex(samples, clusterCount, clusters):
    for i in range(clusterCount):
        points = samples[i == clusters]
        if len(points) > 2:
            hull = ConvexHull(points)
            for simplex in hull.simplices:
                plt.plot(points[simplex, 0], points[simplex, 1], 'k-')

def odpowiedniaKlasteryzacja(X, Y, typ, klastry):
    X_reduced = X
    labels = []
    if typ == 'kmeans':
        model = KMeans(klastry)
        model.fit(X_reduced)
        labels = model.labels_
    elif typ == 'kmeans6':
        labels = kmeans6(X_reduced, klastry)
    elif typ == 'agglo':
        model = AgglomerativeClustering(klastry, linkage='ward')
        model.fit(X_reduced)
        labels = model.labels_
    elif typ == 'GMM':
        model = GaussianMixture(klastry)
        model.fit(X_reduced)
        labels = model.predict(X_reduced)

    klasteryzacja = ﬁnd_perm(klastry, Y, labels)
    klasteryzacja = np.array(klasteryzacja)
    return X_reduced, klasteryzacja

### Zadanie 2.5 ###
    
def WizualizacjaDanych2D(X, Y, typ):
    klastry = max(Y)+1
    X = X.copy()
    X_reduced, klasteryzacja = odpowiedniaKlasteryzacja(X, Y, typ, klastry)

    roznica = klasteryzacja == Y

    figure = plt.figure(figsize=(15,5))

    plt.subplot(1, 3, 1)
    plt.scatter(X_reduced[:,0],X_reduced[:,1], c=Y, cmap='viridis')
    plt.title('oryginalny')
    for i in range(klastry):
        points = X_reduced[i == Y]
        if len(points) > 2:
            hull = ConvexHull(points)
            for simplex in hull.simplices:
                plt.plot(points[simplex, 0], points[simplex, 1], 'k-')
    plt.subplot(1, 3, 2)
    plt.scatter(X_reduced[:,0],X_reduced[:,1], c=klasteryzacja)
    for i in range(klastry):
        points = X_reduced[i == klasteryzacja]
        if len(points) > 2:
            hull = ConvexHull(points)
            for simplex in hull.simplices:
                plt.plot(points[simplex, 0], points[simplex, 1], 'k-')
    plt.title('klasteryzacja')
    plt.subplot(1, 3, 3)
    plt.scatter(X_reduced[:,0],X_reduced[:,1], c=roznica, cmap='viridis')
    plt.title('roznice')


pca = PCA(n_components=2)
X2D = pca.fit_transform(X)


WizualizacjaDanych2D(X2D, Y, 'agglo')

### Zadanie 2.6 ###
def WizualizacjaDanych3D(X, Y, typ):
    klastry = max(Y)+1
    X = X.copy()
    X_reduced, klasteryzacja = odpowiedniaKlasteryzacja(X, Y, typ, klastry)
    roznice = klasteryzacja == Y
    figure = plt.figure(figsize=(15,5))

    ax = figure.add_subplot(1, 3, 1, projection='3d')
    ax.scatter(X_reduced[:,0],X_reduced[:,1],X_reduced[:,2], c=Y)
    ax.set_title('oryginalne')
    ax = figure.add_subplot(1, 3, 2, projection='3d')
    ax.scatter(X_reduced[:,0],X_reduced[:,1],X_reduced[:,2], c=klasteryzacja)
    ax.set_title('klasteryzacja')
    ax = figure.add_subplot(1, 3, 3, projection='3d')
    ax.scatter(X_reduced[:,0],X_reduced[:,1],X_reduced[:,2], c=roznice)
    ax.set_title('roznice')

pca = PCA(n_components=3)
X3D = pca.fit_transform(X)

WizualizacjaDanych3D(X3D, Y, 'agglo')

### Zadanie 2.7 ###
plt.figure(figsize=(15,5))
hierarchy.dendrogram(hierarchy.linkage(X2D))
plt.figure(figsize=(15,5))
hierarchy.dendrogram(hierarchy.linkage(X3D))

### Zadanie 2.8 ###
WizualizacjaDanych2D(X2D, Y, 'kmeans')
WizualizacjaDanych3D(X3D, Y, 'kmeans')
WizualizacjaDanych2D(X2D, Y, 'kmeans6')
WizualizacjaDanych2D(X2D, Y, 'GMM')
WizualizacjaDanych3D(X3D, Y, 'GMM')

### Zadanie 2.9 ###
pca = PCA(n_components=2)
XZ2D = pca.fit_transform(XZOO)
pca = PCA(n_components=3)
XZ3D = pca.fit_transform(XZOO)

WizualizacjaDanych2D(XZ2D, YZOO, 'agglo')
WizualizacjaDanych3D(XZ3D, YZOO, 'agglo')
WizualizacjaDanych2D(XZ2D, YZOO, 'kmeans')
WizualizacjaDanych3D(XZ3D, YZOO, 'kmeans')
WizualizacjaDanych2D(XZ2D, YZOO, 'kmeans6')
WizualizacjaDanych2D(XZ2D, YZOO, 'GMM')
WizualizacjaDanych3D(XZ3D, YZOO, 'GMM')

plt.figure(figsize=(15,5))
hierarchy.dendrogram(hierarchy.linkage(XZ2D))
plt.figure(figsize=(15,5))
hierarchy.dendrogram(hierarchy.linkage(XZ3D))

def plotImage(fig, gs, x, y, image, title):
    ax = fig.add_subplot(gs[x, y])

    ax.tick_params(labelbottom = False, labelleft = False)
    ax.set_title(title)

    if (len(image.shape) == 2):
        divider = make_axes_locatable(ax)
        cax = divider.append_axes('right', size='5%', pad=0.05)
        im = ax.imshow(image, cmap='plasma')
        fig.colorbar(im, cax=cax, orientation='vertical')
    else:
        ax.imshow(image)

def calcualteDifference(image, secondImage):
    difference = ((image-secondImage)**2).sum(axis = 2)/3
    return difference

def plotImages(image, quantizedImage, quantizedImage_N, quantizedImage_P):

    difference = calcualteDifference(image, quantizedImage)
    difference_N = calcualteDifference(image, quantizedImage_N)
    difference_P = calcualteDifference(image, quantizedImage_P)
    difference_PB = calcualteDifference(quantizedImage, quantizedImage_P)
    difference_NB = calcualteDifference(quantizedImage, quantizedImage_N)
    fig = plt.figure(figsize = (12, 8))
    gs = GridSpec(3, 3, figure=fig)

    plotImage(fig, gs, 0, 0, difference_NB, 'Roznica skwantyzowanego i skwantyzowanego z n')
    plotImage(fig, gs, 0, 1, quantizedImage_N, 'Kwantyzacja z n')
    plotImage(fig, gs, 0, 2, difference_N, 'Roznica oryginalnego i skwantyzowanego z n')
    plotImage(fig, gs, 1, 0, image, 'Oryginal')
    plotImage(fig, gs, 1, 1, quantizedImage, 'Skwantyzowany')
    plotImage(fig, gs, 1, 2, difference, 'Roznica oryginalnego i skwantyzowanego')
    plotImage(fig, gs, 2, 0, difference_PB, 'Roznica skwantyzowanego i skwantyzowanego z permutacja')
    plotImage(fig, gs, 2, 1, quantizedImage_P, 'Skwantyzowany i zpermutowany')
    plotImage(fig, gs, 2, 2, difference_P, 'Roznica oryginalu i skwantyzowanego z permutacja')

### Zadanie 3.1 ###
filename = "image.png"
N = 3
QUANTIZATION_PARAMETER = 2 ** N
image = plt.imread(filename)
image = image[:,:,:3]


### Zadanie 3.2,3.8,3.9 ###
vectorizedImage = image.reshape(image.shape[0]*image.shape[1], image.shape[2])
vectorizedImage_N = image.reshape(int(image.shape[0]*image.shape[1] / QUANTIZATION_PARAMETER), int(image.shape[2] * QUANTIZATION_PARAMETER))
permutationImageMatrix = np.arange(len(vectorizedImage))
permutationImageMatrix = rng.shuffle(permutationImageMatrix)
vectorizedImage_P = vectorizedImage[permutationImageMatrix].reshape(image.shape[0]*image.shape[1], image.shape[2])

### Zadanie 3.3 ###
clusterCounts = [2, 3, 6, 10, 30, 60, 100]
#kmeans
for clusterCount in clusterCounts:
    model = KMeans(clusterCount)
    model.fit(vectorizedImage)
    quantizedImage = np.zeros(vectorizedImage.shape)
    for i in range(clusterCount):
        quantizedImage[model.labels_ == i] = model.cluster_centers_[i]
    quantizedImage = quantizedImage.reshape(image.shape)
    
    model = KMeans(clusterCount)
    model.fit(vectorizedImage_N)
    quantizedImage_N = np.zeros(vectorizedImage_N.shape)
    for i in range(clusterCount):
        quantizedImage[model.labels_ == i] = model.cluster_centers_[i]
    quantizedImage_N = quantizedImage_N.reshape(image.shape)
    
    model = KMeans(clusterCount)
    model.fit(vectorizedImage_P)
    quantizedImageShuffled_P = np.zeros(vectorizedImage_P.shape)
    for i in range(clusterCount):
        quantizedImageShuffled_P[model.labels_ == i] = model.cluster_centers_[i]
    quantizedImageShuffled_P = quantizedImageShuffled_P.reshape(image.shape).reshape(image.shape[0]*image.shape[1], image.shape[2])
    
    quantizedImage_P = np.zeros(vectorizedImage_P.shape)
    quantizedImage_P[permutationImageMatrix] = quantizedImageShuffled_P
    quantizedImage_P = quantizedImage_P.reshape(image.shape)
    
    plotImages(image, quantizedImage, quantizedImage_N, quantizedImage_P)
    
#agglo - slow
for clusterCount in clusterCounts:
    model = AgglomerativeClustering(clusterCount, linkage='ward')
    model.fit(vectorizedImage)
    quantizedImage = np.zeros(vectorizedImage.shape)
    labels = model.labels_
    for i in range(clusterCount):
        quantizedImage[labels == i] = vectorizedImage[labels == i].mean(axis=0)
    quantizedImage = quantizedImage.reshape(image.shape)
    
    model = model = AgglomerativeClustering(clusterCount, linkage='ward')
    model.fit(vectorizedImage_N)
    quantizedImage_N = np.zeros(vectorizedImage_N.shape)
    labels = model.labels_
    for i in range(clusterCount):
        quantizedImage[labels == i] = vectorizedImage[labels == i].mean(axis=0)
    quantizedImage_N = quantizedImage_N.reshape(image.shape)
    
    model = model = AgglomerativeClustering(clusterCount, linkage='ward')
    model.fit(vectorizedImage_P)
    quantizedImageShuffled_P = np.zeros(vectorizedImage_P.shape)
    labels = model.labels_
    for i in range(clusterCount):
        quantizedImage[labels == i] = vectorizedImage[labels == i].mean(axis=0)
    quantizedImageShuffled_P = quantizedImageShuffled_P.reshape(image.shape).reshape(image.shape[0]*image.shape[1], image.shape[2])
    
    quantizedImage_P = np.zeros(vectorizedImage_P.shape)
    quantizedImage_P[permutationImageMatrix] = quantizedImageShuffled_P
    quantizedImage_P = quantizedImage_P.reshape(image.shape)
    
    plotImages(image, quantizedImage, quantizedImage_N, quantizedImage_P)
    
#Gaussian
for clusterCount in clusterCounts:
    model = GaussianMixture(clusterCount)
    model.fit(vectorizedImage)
    quantizedImage = np.zeros(vectorizedImage.shape)
    labels = model.predict(vectorizedImage)
    for i in range(clusterCount):
        quantizedImage[labels == i] = model.means_[i]
    quantizedImage = quantizedImage.reshape(image.shape)
    
    model = GaussianMixture(clusterCount)
    model.fit(vectorizedImage_N)
    quantizedImage_N = np.zeros(vectorizedImage_N.shape)
    labels = model.predict(vectorizedImage)
    for i in range(clusterCount):
        quantizedImage[labels == i] = model.means_[i]
    quantizedImage_N = quantizedImage_N.reshape(image.shape)
    
    model = GaussianMixture(clusterCount)
    model.fit(vectorizedImage_P)
    quantizedImageShuffled_P = np.zeros(vectorizedImage_P.shape)
    labels = model.predict(vectorizedImage)
    for i in range(clusterCount):
        quantizedImage[labels == i] = model.means_[i]
    quantizedImageShuffled_P = quantizedImageShuffled_P.reshape(image.shape).reshape(image.shape[0]*image.shape[1], image.shape[2])
    
    quantizedImage_P = np.zeros(vectorizedImage_P.shape)
    quantizedImage_P[permutationImageMatrix] = quantizedImageShuffled_P
    quantizedImage_P = quantizedImage_P.reshape(image.shape)
    
    plotImages(image, quantizedImage, quantizedImage_N, quantizedImage_P)
    
### Zadanie 3.8 ###
vectorizedImage_N = image.reshape(int(image.shape[0]*image.shape[1] / QUANTIZATION_PARAMETER), int(image.shape[2] * QUANTIZATION_PARAMETER))
permutationImageMatrix = np.arange(len(vectorizedImage))
permutationImageMatrix = rng.shuffle(permutationImageMatrix)

### Zadanie 3.9 ###
vectorizedImage_P = vectorizedImage[permutationImageMatrix].reshape(image.shape[0]*image.shape[1], image.shape[2])
