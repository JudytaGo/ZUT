# import warnings filter
from warnings import simplefilter
# ignore all future warnings
simplefilter(action='ignore', category=FutureWarning)
#Niektore warningi spowodowane sa zbyt duza iloscia wykresow

import matplotlib.pyplot as plt
import pandas as pd
import time
import scipy as sc
import numpy as np

from sklearn import datasets
from sklearn.naive_bayes import GaussianNB
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.svm import SVC
from sklearn.model_selection import GridSearchCV, train_test_split
from sklearn.metrics import accuracy_score, f1_score, precision_score, recall_score, roc_auc_score, roc_curve
from sklearn.linear_model import LogisticRegression

def generate_grid_points(xMin, xMax, yMin, yMax, LINSPACE_SIZE):
    xx = np.linspace(xMin, xMax, LINSPACE_SIZE)
    yy = np.linspace(yMin, yMax, LINSPACE_SIZE)
    XX, YY = np.meshgrid(xx, yy)
    points = np.vstack((XX.flatten(), YY.flatten())).transpose()
    return points

def match_labels(clusters, realLabels, predictedLabels):
    realLabels = np.array(realLabels)
    labels=[]
    for i in range(clusters):
        idx = predictedLabels == i
        newlabel=sc.stats.mode(realLabels[idx])[0][0]
        labels.append(newlabel)
    return np.array([labels[label] for label in predictedLabels])


def draw_discrimination_curve(X, y, classifier, classifier_name):
    LINSPACE_SIZE = 100
    xMin, xMax = X['x'].min(), X['x'].max()
    yMin, yMax = X['y'].min(), X['y'].max()
    points = generate_grid_points(xMin, xMax, yMin, yMax, LINSPACE_SIZE)
    xLabels = classifier.predict(points).reshape(LINSPACE_SIZE, LINSPACE_SIZE)

    plt.figure(figsize=(8,8))
    plt.contour(points[:,0].reshape(LINSPACE_SIZE, LINSPACE_SIZE), points[:,1].reshape(LINSPACE_SIZE, LINSPACE_SIZE), xLabels)
    plt.scatter(X['x'], X['y'], c=y)
    plt.title('Discimination curve of ' + classifier_name)

def draw_roc_curve(X_train, X_test, y_train, y_test, y_predicted, classifier_name):

    regression = LogisticRegression()
    regression.fit(X_train, y_train)
    probability = regression.predict_proba(X_test)[:, 1]
    plt.figure(figsize=(16,6))

    fpr, tpr, thresholds = roc_curve(y_predicted, probability)
    accuracy = roc_auc_score(y_test, probability)

    plt.plot(fpr, fpr, c='r', linestyle='--')
    plt.plot(fpr, tpr, label="accuracy=" + str(accuracy))
    plt.title("ROC Curve of " + classifier_name)
    plt.xlabel("False positive rate")
    plt.ylabel("True positive rate")

    plt.legend()

def draw_difference_plot(X_train, X_test, y_train, y_test, y_predicted, classifier_name):
    CLUSTER_COUNT = max(y_test)+1

    if sum(y_predicted) > 0:
        y_predicted = match_labels(CLUSTER_COUNT, y_test, y_predicted)
    else:
        print('qda broken')
    prediction_difference = y_predicted != y_test

    plt.figure(figsize=(12,6)) 
    plt.scatter(X_train['x'], X_train['y'], c=y_train, cmap='viridis')
    plt.scatter(X_test['x'], X_test['y'], c=y_predicted, cmap='viridis')
    plt.title(classifier_name + ' classification')

    plt.figure(figsize=(12,6))
    plt.scatter(X_test['x'], X_test['y'], c=np.array(prediction_difference), cmap='viridis')
    plt.scatter(X_train['x'], X_train['y'], c=np.tile(1, len(y_train)), cmap='viridis')
    plt.title(classifier_name + ' difference to original')



ITERATION_COUNT = 100
X, y = datasets.make_classification(
    n_samples = 200,
    n_features = 2,
    n_informative = 2,
    n_clusters_per_class = 2,
    n_redundant=0,
    n_repeated=0,
    random_state = 3
)

plt.figure(figsize=(7, 7))
plt.scatter(X[:,0], X[:,1], c = y)
plt.title("Oryginalne punkty")

X = pd.DataFrame(X, columns=["x", "y"])
y = pd.Series(y)

classifiers = {
    "qda": QuadraticDiscriminantAnalysis(),
    "gaussian": GaussianNB(),
    "knn": KNeighborsClassifier(2),
    "dtc": DecisionTreeClassifier(max_depth=5),
    "svc": SVC(probability=True)}

results = pd.DataFrame()

for i in range(ITERATION_COUNT):
    X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=i)
    for name, classifier in classifiers.items():

        training_time = time.perf_counter()
        classifier.fit(X_train, y_train)
        training_time = time.perf_counter() - training_time

        prediction_time = time.perf_counter()
        y_pred = classifier.predict(X_test)
        prediction_time = time.perf_counter() - prediction_time

        accuracy = accuracy_score(y_test, y_pred)
        recall = recall_score(y_test, y_pred)
        precision = precision_score(y_test, y_pred)
        f1 = f1_score(y_test, y_pred)
        auc = roc_auc_score(y_test, y_pred)

        row = pd.Series(
            {
                "index" : 0,
                "iteration": i,
                "classifier" : name,
                "accuracy" : accuracy,
                "recall" : recall,
                "precision" : precision,
                "f1" : f1,
                "auc" : auc,
                "training_time" : training_time,
                "prediction_time" : prediction_time,
            }
        )
        results = results.append(row, ignore_index=True)
        if i == ITERATION_COUNT - 1:
            draw_difference_plot(X_train, X_test, y_train, y_test, y_pred, name)
            draw_roc_curve(X_train, X_test, y_train, y_test, y_pred, name)
            draw_discrimination_curve(X, y, classifier, name)
print(results.groupby('classifier').mean())





ITERATION_COUNT = 100
X, y = datasets.make_classification(
    n_samples = 500,
    n_features = 2,
    n_informative = 2,
    n_clusters_per_class = 2,
    n_redundant=0,
    n_repeated=0,
    random_state = 3
)

grid_params = {
    'n_neighbors': [1, 2, 3, 5, 7, 9, 11, 13, 15, 17, 21, 34],
    'weights': ['uniform', 'distance'],
    'metric': ['euclidean', 'manhattan', 'chebyshev', 'minkowski'],
    'p': [1, 1.25, 1.5, 1.75, 2, 2.5, 3, 5]
}
gs_cv = GridSearchCV(KNeighborsClassifier(), grid_params, verbose=1, n_jobs=-1, scoring='accuracy')
gs_results = gs_cv.fit(X, y)
cv_results = pd.DataFrame(gs_results.cv_results_)

effect_score = {}
for param in grid_params.keys():
    effect_score[param] = cv_results.groupby('param_' + param).mean()['mean_test_score'].std()
effect_score_sorted = sorted(effect_score, key=effect_score.get)[::-1]
first_imp_param = 'param_' + effect_score_sorted[0]
second_imp_param = 'param_' + effect_score_sorted[1]
results = pd.DataFrame(gs_results.cv_results_)[['mean_test_score', first_imp_param, second_imp_param]]
first_imp_values = results[first_imp_param].unique()
second_imp_values = results[second_imp_param].unique()
mappedResults = pd.DataFrame(np.zeros((len(first_imp_values), len(second_imp_values))), index=first_imp_values, columns=second_imp_values)
for column in range(len(second_imp_values)):
    for row in range(len(first_imp_values)):
        original_index = (results[first_imp_param] == first_imp_values[row]) & (results[second_imp_param] == second_imp_values[column])

        mappedResults.iat[row, column] = results[original_index].mean()['mean_test_score']
plt.figure(figsize=(4.5, 8))



classifier = KNeighborsClassifier(**gs_results.best_params_)
name = 'detailed_knn'
X = pd.DataFrame(X, columns=["x", "y"])
y = pd.Series(y)
results = pd.DataFrame()

for i in range(ITERATION_COUNT):
    X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=i)

    training_time = time.perf_counter()
    classifier.fit(X_train, y_train)
    training_time = time.perf_counter() - training_time

    prediction_time = time.perf_counter()
    y_pred = classifier.predict(X_test)
    prediction_time = time.perf_counter() - prediction_time

    accuracy = accuracy_score(y_test, y_pred)
    recall = recall_score(y_test, y_pred)
    precision = precision_score(y_test, y_pred)
    f1 = f1_score(y_test, y_pred)
    auc = roc_auc_score(y_test, y_pred)

    row = pd.Series(
        {
            "index" : 0,
            "iteration": i,
            "classifier" : name,
            "accuracy" : accuracy,
            "recall" : recall,
            "precision" : precision,
            "f1" : f1,
            "auc" : auc,
            "training_time" : training_time,
            "prediction_time" : prediction_time,
        }
    )
    results = results.append(row, ignore_index=True)
    if i == ITERATION_COUNT - 1:
        draw_difference_plot(X_train, X_test, y_train, y_test, y_pred, name)
        draw_roc_curve(X_train, X_test, y_train, y_test, y_pred, name)
        draw_discrimination_curve(X, y, classifier, name)


print(gs_results.best_score_)
print(gs_results.best_params_)
print(results.groupby('classifier').mean())
