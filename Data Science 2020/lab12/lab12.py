"""
@author: Judyta Gogolewska
"""
import pandas as pd
import matplotlib.pyplot as plt
import scipy as sc
import numpy as np

from sklearn import datasets, svm
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression, Perceptron
from sklearn.multiclass import OneVsOneClassifier, OneVsRestClassifier
from sklearn.metrics import accuracy_score, auc, f1_score, precision_score, recall_score, roc_auc_score, roc_curve
from sklearn import preprocessing

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

def draw_roc_curve(X_train, X_test, y_train, y_test, y_score, classifier_name):

    
    plt.figure(figsize=(16,9))

    classes = list(range(0, 4))
    y_test_dummies = pd.get_dummies(y_test, drop_first=False).values
    for i in classes:
        fpr, tpr, thresholds = roc_curve(y_test_dummies[:, i], y_score[:, i])
        accuracy = auc(fpr, tpr)
        plt.plot(fpr, tpr, label="accuracy={:.4f}, class={}".format(accuracy, i))

    plt.plot(fpr, fpr, c='r', linestyle='--')
    plt.title("ROC Curve of " + classifier_name)
    plt.xlabel("False positive rate")
    plt.ylabel("True positive rate")

def draw_discrimination_curve(X, y, classifier, classifier_name):
    LINSPACE_SIZE = 100
    xMin, xMax = X['x'].min(), X['x'].max()
    yMin, yMax = X['y'].min(), X['y'].max()
    points = generate_grid_points(xMin, xMax, yMin, yMax, LINSPACE_SIZE)
    xLabels = classifier.predict(points).reshape(LINSPACE_SIZE, LINSPACE_SIZE)

    plt.figure(figsize=(8,8))
    plt.xlim(xMin, xMax)
    plt.ylim(yMin, yMax)
    plt.contourf(points[:,0].reshape(LINSPACE_SIZE, LINSPACE_SIZE), points[:,1].reshape(LINSPACE_SIZE, LINSPACE_SIZE), xLabels, cmap='viridis', alpha=0.2)
    plt.scatter(X['x'], X['y'], c=y, alpha=0.75)
    plt.title('Discimination curve of ' + classifier_name)

def generate_dataset():
    X, y = datasets.make_classification(
        n_samples=1500,
        n_features=2,
        n_informative=2,
        n_classes=4,
        n_clusters_per_class=1,
        n_redundant=0,
        n_repeated=0,
        random_state=3
    )
    X = pd.DataFrame(X, columns=["x", "y"])
    y = pd.Series(y)
    return X, y


def get_classifiers():
    return {
        'ovo_svc_linear': OneVsOneClassifier(svm.SVC(kernel='linear', probability=True), n_jobs=-1),
        'ovr_svc_linear': OneVsRestClassifier(svm.SVC(kernel='linear', probability=True), n_jobs=-1),
        'ovo_svc_rbf': OneVsOneClassifier(svm.SVC(kernel='rbf', probability=True), n_jobs=-1),
        'ovr_svc_rbf': OneVsRestClassifier(svm.SVC(kernel='rbf', probability=True), n_jobs=-1),
        'ovo_logistic_regression': OneVsOneClassifier(LogisticRegression(), n_jobs=-1),
        'ovr_logistic_regression': OneVsRestClassifier(LogisticRegression(), n_jobs=-1),
        'ovo_perceptron': OneVsOneClassifier(Perceptron(), n_jobs=-1),
        'ovr_perceptron': OneVsRestClassifier(Perceptron(), n_jobs=-1),
    }

def draw_difference_plot(X_train, X_test, y_train, y_test, y_predicted, classifier_name):
    CLUSTER_COUNT = max(y_test)+1

    y_predicted = match_labels(CLUSTER_COUNT, y_test, y_predicted)
    prediction_difference = y_predicted != y_test

    plt.figure(figsize=(12,6))

    plt.scatter(X_train['x'], X_train['y'], c=y_train, cmap='viridis')
    plt.scatter(X_test['x'], X_test['y'], c=y_predicted, cmap='viridis')
    plt.title(classifier_name + ' classification')
    
    plt.figure(figsize=(12,6))
    
    plt.scatter(X_test['x'], X_test['y'], c=np.array(prediction_difference), cmap='viridis')
    plt.scatter(X_train['x'], X_train['y'], c=np.tile(1, len(y_train)), cmap='viridis')
    plt.title(classifier_name + ' differance with original')

def draw_results(results, classifiers_keys):
    WIDTH = 0.1
    POSITION = np.arange(5)

    plt.figure(figsize=(16, 9))

    for i, classifier_name in enumerate(classifiers_keys):
        plt.bar(POSITION + WIDTH * (i - 3.5), results.loc[classifier_name,:], WIDTH, label=classifier_name)
    plt.xticks(POSITION, results.columns)

    plt.legend()

def get_accuracy_scores(y_test, y_pred, classifier_name):
    accuracy = accuracy_score(y_test, y_pred)
    recall = recall_score(y_test, y_pred, average='macro')
    precision = precision_score(y_test, y_pred, average='macro')
    f1 = f1_score(y_test, y_pred, average='macro')

    y_pred = preprocessing.label_binarize(y_pred, classes=[0, 1, 2, 3])

    auc = roc_auc_score(y_test, y_pred, multi_class=classifier_name[0:3], average='macro')

    row = pd.Series(
        {
            "accuracy" : accuracy,
            "recall" : recall,
            "precision" : precision,
            "f1" : f1,
            "auc" : auc,
        }, name=classifier_name
    )
    return row

def draw_original_points(X, y):
    plt.figure(figsize=(7, 7))
    plt.scatter(X['x'], X['y'], c = y)
    plt.title("Original points")

X, y = generate_dataset()
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.5, random_state=43)
classifiers = get_classifiers()
results = pd.DataFrame()

for name, classifier in classifiers.items():
    classifier.fit(X_train, y_train)
    y_pred = classifier.predict(X_test)
    draw_difference_plot(X_train, X_test, y_train, y_test, y_pred, name)

    results = results.append(accuracy_score)

    draw_discrimination_curve(X, y, classifier, name)
    y_score = classifier.decision_function(X_test)
    draw_roc_curve(X_train, X_test, y_train, y_test, y_score, name)

    plt.close('all')
draw_results(results, classifiers.keys())
draw_original_points(X, y)