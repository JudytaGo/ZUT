"""
@author: Judyta Gogolewska
"""
import sklearn.datasets as datasets
import matplotlib.pyplot as plt
import math
import numpy as np
import scipy.stats as stats
import sklearn.neighbors as skn
import pandas as pd
from sklearn import datasets
from sklearn.decomposition import PCA
from sklearn.model_selection import LeaveOneOut, train_test_split, KFold
from sklearn.metrics import mean_squared_error, r2_score
from timeit import default_timer as timer


def euclideanDistance(sample, point):
    return math.sqrt(sum((sample - point)**2))

def calculateDistance(samples, points):
    clusterCount = points.shape[1]
    samplesCount = samples.shape[0]
    distances = np.zeros((clusterCount, samplesCount))
    for clusterIndex in range(clusterCount):
        for sampleIndex, sample in enumerate(samples):
            distances[clusterIndex,sampleIndex] = euclideanDistance(sample, points[:, clusterIndex])
    return distances

class NearestNeighbors():
    def __init__(self, n_neighbors = 1, use_KDTree = False):
        self.n_neighbors = n_neighbors
        self.use_KDTree = use_KDTree

    def fit(self, X, y):
        self.X = X        
        if self.use_KDTree:
            self.KDTree = skn.KDTree(X)
        self.y = y
        return self
    
    def predict(self, X):
        if self.use_KDTree:
            neighborsIndices = self.KDTree.query(X, self.n_neighbors, False)
        else:
            distances = calculateDistance(self.X, X.T)
            neighborsIndices = np.argpartition(distances, self.n_neighbors)[:,self.n_neighbors:]
        XLabels = []
        for indices in neighborsIndices:
            closestNeighborsLabels = self.y[indices]
            mode, _ = stats.mode(closestNeighborsLabels)
            XLabels.append(mode[0])
        return np.array(XLabels)
    def predict_reg(self, X):
        if self.use_KDTree:
            neighborsIndices = self.KDTree.query(X, self.n_neighbors, False)
        else:
            distances = calculateDistance(self.X, X.T)
            neighborsIndices = np.argpartition(distances, self.n_neighbors)[:,self.n_neighbors]
        XValues = []
        for indices in neighborsIndices:
            XValues.append(self.y[indices].mean())
        return np.array(XValues)

    def score(self, X, y):
        yTest = self.predict(X)
        return np.array(y == yTest).mean()
    def score_reg(self, X, y):
        yPredicted = self.predict(X)
        return mean_squared_error(y, yPredicted)
    
def generateGridPoints(xMin, xMax, yMin, yMax, LINSPACE_SIZE):
    xx = np.linspace(xMin, xMax, LINSPACE_SIZE)
    yy = np.linspace(yMin, yMax, LINSPACE_SIZE)
    XX, YY = np.meshgrid(xx, yy)
    points = np.vstack((XX.flatten(), YY.flatten())).transpose()
    return points

#Zadanie 3.1
X, y = datasets.make_classification(
    n_samples = 100,
    n_features = 2,
    n_informative = 2,
    n_redundant = 0,
    n_repeated = 0,
    random_state = 3
)
#Zadanie 3.2
K = 1
LINSPACE_SIZE = 100
xMin, xMax = X[:,0].min(), X[:,0].max()
yMin, yMax = X[:,1].min(), X[:,1].max()
points = generateGridPoints(xMin, xMax, yMin, yMax, LINSPACE_SIZE)
nnc = NearestNeighbors(K, True)
nnc.fit(X, y)
xLabels = nnc.predict(points).reshape(LINSPACE_SIZE, LINSPACE_SIZE)

#Zadanie 3.3
plt.contour(points[:,0].reshape(LINSPACE_SIZE, LINSPACE_SIZE), points[:,1].reshape(LINSPACE_SIZE, LINSPACE_SIZE), xLabels)
plt.scatter(X[:,0], X[:,1], c=y)
plt.title('2D')

#Zadanie 3.4
iris = datasets.load_iris()
X = iris.data
y = iris.target
#Zadanie 3.5
pca = PCA(n_components=2)
X_reduced = pca.fit_transform(X)

LINSPACE_SIZE = 150
xMin, xMax = X_reduced[:,0].min(), X_reduced[:,0].max()
yMin, yMax = X_reduced[:,1].min(), X_reduced[:,1].max()
points = generateGridPoints(xMin, xMax, yMin, yMax, LINSPACE_SIZE)
points4D = pca.inverse_transform(points)

nnc = NearestNeighbors(K, True)
nnc.fit(X, y)
xLabels = nnc.predict(points4D).reshape(LINSPACE_SIZE, LINSPACE_SIZE)
plt.figure(figsize=(12,9))
plt.contour(points[:,0].reshape(LINSPACE_SIZE, LINSPACE_SIZE), points[:,1].reshape(LINSPACE_SIZE, LINSPACE_SIZE), xLabels)
plt.scatter(X_reduced[:,0], X_reduced[:,1], c=y)
plt.title('3D')

loo = LeaveOneOut()
predictionAccuracy = []
neighborCount = list(range(1, 150))
for i in neighborCount:
    values = []
    for train_index, test_index in loo.split(X):
        nnc = NearestNeighbors(i, True)
        nnc.fit(X[train_index], y[train_index])
        values.append(nnc.score(X[test_index], y[test_index]))
    predictionAccuracy.append(np.array(values).mean()*100)

plt.figure(figsize=(12,6))
plt.xlabel('Neighbors')
plt.ylabel('Accuracy')
plt.ylim(0, 100)
plt.plot(neighborCount, predictionAccuracy)


#Zadanie 3.7
X, y = datasets.make_classification(
    n_samples = 100,
    n_features = 2,
    n_informative = 2,
    n_redundant = 0,
    n_repeated = 0,
    random_state = 3
)
for k in [1, 2, 3, 5, 8, 13]:
    for linspaceSize in [10, 20, 30, 50, 80]:
        xMin, xMax = X[:,0].min(), X[:,0].max()
        yMin, yMax = X[:,1].min(), X[:,1].max()
        points = generateGridPoints(xMin, xMax, yMin, yMax, linspaceSize)
        for kdtree_use in [True, False]:
            nnc = NearestNeighbors(k, kdtree_use)
            nnc.fit(X, y)
            start = timer()
            xLabels = nnc.predict(points).reshape(linspaceSize, linspaceSize)
            end = timer()
            print(
                'P:' + str(linspaceSize**2) + 
                ' N:' + str(k) + 
                ' KDtree: ' + str(kdtree_use) + 
                ' T: ' + str((end - start) * 1000)
                )



#Zadanie 4.1
X, y = datasets.make_regression(
    n_samples = 400,
    n_features = 1,
    noise = 25,
    n_informative = 1,
    random_state = 3
)
X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=0)


T = np.linspace(X.min(), X.max(), 500)[:,np.newaxis]
#Zadanie 4.2 i 4.3

for i, n_neighbors in enumerate([1, 3, 6, 9]):
    knn = NearestNeighbors(n_neighbors, True)
    y_ = knn.fit(X_train, y_train).predict_reg(T)
    plt.figure(figsize=(12,9))
    plt.scatter(X_train, y_train, color='yellow', label='Training score: %f' % (knn.score_reg(X_train, y_train)))
    plt.scatter(X_test, y_test, color='blue', label='Testing score: %f' % (knn.score_reg(X_test, y_test)))
    plt.plot(T, y_)
    plt.axis('tight')
    plt.legend()
    plt.title("NearestNeighborsRegressor (k = %i)" % (n_neighbors))

plt.tight_layout()


boston = datasets.load_boston()
X = boston.data
y = boston.target

kf = KFold(10, True)
predictionAccuracy = []
neighborCount = list(range(1, 150))
for i in neighborCount:
    values = []
    for train_index, test_index in kf.split(X):
        nnc = NearestNeighbors(i, True)
        nnc.fit(X[train_index], y[train_index])
        values.append(nnc.score_reg(X[test_index], y[test_index]))
    predictionAccuracy.append(np.array(values).mean())

print(pd.DataFrame({"Neighbors": neighborCount, "Score": predictionAccuracy}).to_string(index=False))


