# -*- coding: utf-8 -*-
"""
@author: Judyta Gogolewska

"""

print("test")
import numpy as np

#TABLICE
a = np.array([1,2,3,4,5,6,7])
b = np.array([[1,2,3,4,5],[6,7,8,9,10]])
bT = np.transpose(b)
print(bT)
print(np.arange(100))
print(np.arange(0,100,10))
#2
c = np.arange(0,100,5)
print(c)

#LICZBY LOSOWE
d = np.random.normal(size = 20)
d = np.round(d,2)
print(d)
e = np.random.randint(low = 1, high = 1000, size = 100)
print(e)
print(np.zeros((3,2)))
print(np.ones((3,2)))
print(np.random.randint(-2**31,2**31,size = (5,5),dtype=np.int32))
#zadanie
aZ = np.random.uniform(0,10,10)
print(aZ)
bZ = aZ.astype(int)
print("int1",bZ)
cZ = np.around(aZ, decimals = 0)
print(cZ)
dZ = cZ.astype(int)
print("int2",dZ)
#zamiana wartoci na int (wyniki przechowywane w b) spowodowała odcięcię wartosci po przecinku
#drugi sposób z zaokrągleniem, jest dokładniejszy
#np. 4,9 po zamianie na 'integer' i wstawieniu w tablicę b wynosi 4
#a po wykorzystaniu funkcji 'around' wynosi 5

#SELEKCJA DANYCH
b = np.array([[1,2,3,4,5], [6,7,8,9,10]],dtype=np.int32)
print(b)
print("l. wymiarów:", b.ndim)
print("l. elementów:", b.size)
print(b[0,[1,3]])
print(b[0])
print(b[:,0])
mL = np.random.randint(low = 0, high = 100, size =(20,7))
print(mL)
print(mL[:,0:4])

#OPERACJE MATEMATYCZNE I LOGICZNE
aM = np.random.random_sample(size=(3,3))*10
print(aM)
bM = np.random.random_sample(size = (3,3))*10
print(bM)
print(np.add(aM,bM))
print(np.matmul(aM,bM))
print(np.divide(aM,bM))
print(np.power(aM,bM))
aMw=np.linalg.det(aM)
print(aMw)
print(aMw>4)
print(aMw!=4 and aMw>1)
print(np.trace(bM))

#DANE STATYSTYCZNE
print(bM.sum())
print(bM.min())
print(bM.max())
print(bM.std())
print(bM.mean(axis=0))
print(bM.mean(axis=1))

#RZUTOWANIE WYMIARÓW ZA POMOCĄ SHAPE LUB RESIZE
t = np.arange(50)
print(t)
print(np.reshape(t,(10,5)))
print(np.resize(t,(10,5)))
tm = np.resize(t,(10,5))
print(np.ravel(tm))
#ravel przekształca tablicę w jednowymiarową
tA = np.arange(5)
print(tA)
tB = np.arange(4)
print(tB)
tB.resize(tA.shape)
print(np.add(tA,tB))
tN = tA[:,np.newaxis]
print(tN)

#SORTOWANIE DANYCH
a=np.random.randn(5,5)
print(np.sort(a,axis=0))
print(np.sort(a,axis=1)[::-1])
dtype = [('1', int), ('2', 'U2'), ('3', 'U20')]
b=np.array([(1,'MZ','mazowieckie'),(2,'ZP','zachodniopomorskie'),(3,'ML','małopolskie')],dtype=dtype)
bs=np.sort(b,order='2')
print(bs)
print(bs[2][2])

#3.Zadania Podsumowujące
#1.
a=np.random.randint(0,50,size=(10,5))
print(np.trace(a))
print(np.diag(a))

#2.
a=np.random.normal(size=10)
b=np.random.normal(size=10)
print(np.multiply(a,b))

#3.
a=np.random.randint(1,100,size=(15))
b=np.random.randint(1,100,size=(15))
a=np.reshape(a,(-1,5))
b=np.reshape(b,(-1,5))
print(np.add(a,b))

#4.
a=np.random.randint(0,10,size=(5,4))
b=np.random.randint(0,10,size=(4,5))
print(np.add(a,np.transpose(b)))

#5.
print(np.multiply(a[:,2],np.transpose(a[:,3])))
print(np.multiply(b[:,2],np.transpose(b[:,3])))

#6.
nor=np.random.normal(size=10)
uni=np.random.uniform(size=10)
NORsr=np.mean(nor)
UNIsr=np.mean(uni)
NORostd=np.std(nor)
UNIostd=np.std(uni)
NORwar=np.var(nor)
UNIwar=np.var(uni)
print(NORsr,UNIsr)
print(NORostd,UNIostd)
print(NORwar,UNIwar)
#srednia rozkładu normalnego jest bardziej rozbieżna, a srednia jednostajnego jest mniej robieżna

#7.
mK1 = np.arange(4)
mK1 =np.reshape(mK1,(2,2))
mK2 = np.arange(4)
mK2 =np.reshape(mK2,(2,2))
print(mK1 * mK2)
print(np.dot(mK1,mK2))
#Pierwsze mnozenie jest mnozeniem pol, a drugie jest mnozeniem macierzowym

#8.
from numpy.lib.stride_tricks import as_strided
a = np.arange(0,24).reshape(4,6)
s1 = a.strides[0]
s2 = a.strides[1]
a1 = as_strided(a,strides=(s1,s2),shape=(3,5))
print(a1)

#9.
b = np.arange(24,48).reshape(4,6)
print (np.vstack((a,b)))
print (np.hstack((a,b)))
#vstack to zlaczenie macierzy jedna pod druga
#hstack to zlaczenie macierzy obok siebie

#10
w1 = np.amax(as_strided(a,strides=(s1,s2),shape=(2,3)))
w2 = np.amax(as_strided(a+3,strides=(s1,s2),shape=(2,3)))
w3 = np.amax(as_strided(a+12,strides=(s1,s2),shape=(2,3)))
w4 = np.amax(as_strided(a+15,strides=(s1,s2),shape=(2,3)))
print(w1,w2,w3,w4)


