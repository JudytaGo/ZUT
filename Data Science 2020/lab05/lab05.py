"""
@author: judyta gogolewska
"""
import pandas as pd
import matplotlib.pyplot as plt
import scipy as sc
import numpy as np
import math

from sklearn import datasets, svm
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression, Perceptron
from sklearn.multiclass import OneVsOneClassifier, OneVsRestClassifier
from sklearn.metrics import accuracy_score, auc, f1_score, precision_score, recall_score, roc_auc_score, roc_curve
from sklearn import preprocessing
from sklearn.datasets import fetch_rcv1
from scipy import sparse

rcv1=fetch_rcv1()
X=rcv1["data"]
Y=rcv1.target[:,87]
data=pd.read_csv('zoo.csv')

#1
def freq(values, probability = True):
    valuesCounts = {}
    for value in values:
        if value in valuesCounts:
            valuesCounts[value] += 1
        else:
            valuesCounts[value] = 1
    uniques = valuesCounts.keys()
    counts = list(valuesCounts.values())
    if probability:
        return uniques, list(map(lambda x : x / len(values), counts))
    else:
        return uniques, counts

#2
def freq2(df,c1,c2,prob=True):
    if isinstance(df,pd.DataFrame):
        if c1 in df and c2 in df and c1!=c2:
            t=df.groupby([c1,c2]).size().unstack(fill_value=0)
            if prob==True:
                t=t/t.sum().sum()
                return t
            else:
                return None

#3
def entropy(x):
    x=x[x != 0]
    return -np.sum(x*np.log2(x))

def infogain(list1,list2):
    x=np.array(list1)
    y=np.array(list2)
    tmp,px=freq(x, True)
    tmp,py=freq(y, True)
    rows,cols,pxy=freq2(x,y,True)
    pe=np.zeros((len(rows),len(cols)))
    for i in range(len(rows)):
        for j in range(len(cols)):
            if pxy[i][j]!=0:
                pe[i][j]=pxy[i][j]*math.log2(pxy[i][j]/px[i])
            else:
                pe[i][j]=0
    sumy=entropy(y)
    sumxy=-np.sum(np.sum(pe,axis=1),axis=0)
    lsum=sumy-sumxy
    return lsum
for i in data.keys():
    print(infogain(data,i))
#4
    for i in X.keys():
        print(infogain(X['animal'],X[i]))

