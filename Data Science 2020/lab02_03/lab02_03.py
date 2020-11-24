# -*- coding: utf-8 -*-
"""
@author: Judyta Gogolewska
"""
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import scipy.stats as st

#MANIPULOWANIE DANYMI
data = pd.date_range('2020-03-01', periods = 5, freq = 'D')
tabela1 = pd.DataFrame({'data': data, 'A': np.random.randn(len(data)), 'B': np.random.randn(len(data)), 'C': np.random.randn(len(data))})
print(tabela1.to_string(index=False))
#
tabela2 = pd.DataFrame({ 'A': np.random.randn(20), 'B': np.random.randn(20), 'C': np.random.randn(20)})
tabela2.index.names = ['id']
print(tabela2)
a = tabela2.iloc[:3]
print(a)
b = tabela2.iloc[-3:]
print(b)
c = tabela2.index.name
print(c)
d = tabela2.columns.values.tolist()
print(d)
e = tabela2.to_string(header = False, index = False)
print(e)
f = tabela2.sample(5)
print(f)
g = tabela2.loc[:, 'A']
print(g)
h = tabela2.loc[:, ['A', 'B']]
print(h)
i = tabela2.iloc[:3, 0:2]
print(i)
j = tabela2.iloc[4, :]
print(j)
k = tabela2.iloc[[0, 5, 6, 7], 0:2]
print(k)
#
print(tabela2.describe())
print(tabela2>0)
print(tabela2[tabela2 > 0])
print(tabela2.loc[:, 'A'][tabela2.loc[:, 'A'] > 0])
print(tabela2.mean(axis = 0))
print(tabela2.mean(axis = 1))
#
#CONCAT
#TABELA 2 3 I 4 POLACZONE
tablica3 = pd.DataFrame({'x': [1, 1], 'y': ['a', 'b']})
tablica4 = pd.DataFrame({'x': [ 2, 2], 'y': ['c','d']})
tablicaPolaczona =pd.concat([tablica3, tablica4])
print(tablicaPolaczona)
print(np.transpose(tablicaPolaczona))

#SORTOWANIE W TABELACH
tabela5 = pd.DataFrame({"x": [1, 2, 3, 4, 5], "y": ['a', 'b', 'a', 'b', 'b']}, index = np.arange(5))
tabela5.index.name = 'id'
print(tabela5)
print(tabela5.sort_values(by='id'))
print(tabela5.sort_values(by='y', ascending = False))

#GRUPOWANIE DANYCH
slownik = {'Day': ['Mon', 'Tue', 'Mon', 'Tue', 'Mon'], 'Fruit': ['Apple', 'Apple', 'Banana', 'Banana', 'Apple'], 'Pound': [10, 15, 50, 40, 5], 'Profit':[20, 30, 25, 20, 10]}
tabela6 = pd.DataFrame(slownik)
print(tabela6)
print(tabela6.groupby('Day').sum())
print(tabela6.groupby(['Day','Fruit']).sum())

#WYPEŁNIANIE DANYCH
tabela7=pd.DataFrame(np.random.randn(20, 3), index=np.arange(20), columns=['A','B','C'])
tabela7.index.name='id'
print(tabela7)
tabela7['B'] = 1 
print(tabela7)
#przypisujemy wartosć '1' do wartosci wybranej kolumny, czyli 'B'
tabela7.iloc[1,2] = 10
print(tabela7)
#przypisujemy wartosć '10' dla pola [1,2], czyli pierwszy wiersz, kolumna C
tabela7[tabela7 < 0] = -tabela7
print(tabela7)
#dla wartosci w tabeli, które są mniejsze od 0, przypisujemy do nich liczbę przeciwną

#UZUPEŁNIANIE DANYCH
tabela7.iloc[[0, 3], 1] = np.nan
print(tabela7)
#przypisujemy NaN (brak wartosci) dla wierszy 0 raz 3, kolumny B
tabela7.fillna(0, inplace=True)
print(tabela7)
#przypisujemy wartosc '0' w pola, w których były 'braki wartosci' (NaN) w miejscu (inplace)
tabela7.iloc[[0, 3], 1] = np.nan
tabela7 = tabela7.replace(to_replace=np.nan, value=-9999)
print(tabela7)
#przypisujemy wartosc '-9999' w miejsca NaN
tabela7.iloc[[0, 3], 1] = np.nan
print(pd.isnull(tabela7))
#w polach, w których występował NaN wyswietlamy prawdę (true)

#TABELA DO ZADAŃ
tablica = pd.DataFrame({'x': [1, 2, 3, 4, 5], 'y': ['a', 'b', 'a', 'b', 'b']})

#ZADANIE 1
#w zadaniu oznaczenia sa na odwrot??? x- symboliczne? y - numeryczne?
print(tablica.groupby('y').mean())

#ZADANIE 2
print(tablica['y'].value_counts())

#ZADANIE 3
loadtxt = np.loadtxt("C:\\Users\\judyt\\OneDrive\\Pulpit\\PiAD-lab\\autos.csv", delimiter=",", dtype='str')
print(loadtxt)
#loadtxt wczytuje dane z plików tekstowych, a read_csv przystosowane do wczytywania plików csv
autos = pd.read_csv('C:\\Users\\judyt\\OneDrive\\Pulpit\\PiAD-lab\\autos.csv')
print(autos)

#ZADANIE 4
z4 = autos.groupby('make').mean()[['city-mpg','highway-mpg']]
print(z4)

#ZADANIE 5
z5 = autos.groupby('make')['fuel-type'].value_counts()
print(z5)

#ZADANIE 6
wielomian1 = np.polyfit(autos['length'], autos['city-mpg'], 1)
print(wielomian1)
wielomian2 = np.polyfit(autos['length'], autos['city-mpg'], 2)
print(wielomian2) 

#ZADANIE 7
z7 = st.pearsonr(autos['length'], autos['city-mpg'])
print(z7)

#ZADANIE 8
z8 = np.linspace(autos['length'].min(), autos['length'].max(), 500)
plt.scatter(autos['length'], autos['city-mpg'], label = 'próbki')
plt.xlabel('length')
plt.ylabel('city-mpg')
plt.plot(z8, np.polyval(wielomian1, z8), label = 'wielomian^1')
plt.plot(z8, np.polyval(wielomian2, z8), label = 'wielomian^2')
plt.legend()

#ZADANIE9
z9gauss = st.gaussian_kde(autos['length'])
plt.figure()
plt.plot(z8, z9gauss(z8), label = 'estymator funkcji gestosci')
plt.scatter(autos['length'], z9gauss(autos['length']), label = 'próbki')
plt.legend()

#ZADANIE 10
plt.figure()
axL = plt.subplot(2, 1, 1)
axL.plot(z8, z9gauss(z8), label = 'estymator funkcji')
axL.scatter(autos['length'], z9gauss(autos['length']), label = 'próbki')
axL.set_title("rozkład dla zmiennej  length")
plt.legend()
x = np.linspace(autos['width'].min(), autos['width'].max(), 500)
z10gauss = st.gaussian_kde(autos['width'])
axW = plt.subplot(2, 1, 2)
axW.plot(x, z10gauss(x), label = 'estymator funkcji')
axW.scatter(autos['width'], z10gauss(autos['width']), label = 'próbki')
axW.set_title("rozkład dla zmiennej 'width'")
plt.legend()

#ZADANIE 11
xmin, xmax, ymin, ymax = autos['width'].min(), autos['width'].max(), autos['length'].min(), autos['length'].max()
xx, yy = np.mgrid[xmin:xmax:100j, ymin:ymax:100j]
z11p = np.vstack([xx.ravel(), yy.ravel()])
z11w = np.vstack([autos['width'], autos['length']])
kernel = st.gaussian_kde(z11w)
z11f = np.reshape(kernel(z11p).T, xx.shape)
fig = plt.figure()
ax = fig.gca()
cset = ax.contour(xx, yy, z11f, colors='b')
plt.savefig('zadanie11.png')
plt.savefig('zadanie11.pdf')
plt.show()
