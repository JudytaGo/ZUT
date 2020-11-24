# -*- coding: utf-8 -*-
"""
Created on Tue Mar 31 04:13:55 2020

@author: Judyta Gogolewska
"""
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import pandas as pd
import scipy.stats as st

#DYSKRETYZACJA
def dyskretyzacja(czestotliwosc_sygnalu, czestotliwosc_probkowania):
    t=np.linspace(0,1,czestotliwosc_probkowania);
    s=np.sin(2*np.pi*t*czestotliwosc_sygnalu);
    return (t,s);
plt.figure()
(x,y)=dyskretyzacja(10,10);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,21);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,30);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,45);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,50);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,100);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,150);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,200);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,250);
plt.plot(x,y)
plt.figure()
(x,y)=dyskretyzacja(10,1000);
plt.plot(x,y)
#Twierdzenie o próbkowaniu, twierdzenie Nyquista-Shannona
#Aliasing
methods = [None, 'none', 'nearest', 'bilinear', 'bicubic', 'spline16',
           'spline36', 'hanning', 'hamming', 'hermite', 'kaiser', 'quadric',
           'catrom', 'gaussian', 'bessel', 'mitchell', 'sinc', 'lanczos']

#Fixing random state for reproducibility
np.random.seed(19680801)

grid = mpimg.imread('Aliasing.png')

fig, axs = plt.subplots(nrows=3, ncols=6, figsize=(9, 6),
                        subplot_kw={'xticks': [], 'yticks': []})

for ax, interp_method in zip(axs.flat, methods):
    ax.imshow(grid, interpolation=interp_method, cmap='viridis')
    ax.set_title(str(interp_method))

plt.tight_layout()
plt.show()


#KWANTYZACJA

img = mpimg.imread('Aliasing.png')
print(img.shape)
print(img.shape[2])
r, g, b = img[:,:,0], img[:,:,1], img[:,:,2]
#Wyznaczenie jasności piksel
gray1=(np.maximum(r,np.maximum(g,b))+np.minimum(r,np.minimum(g,b)))/2;
gray1=(gray1*255).astype(int)
plt.figure()
plt.imshow(gray1, cmap='gray', vmin=0, vmax=255)
#Uśrednienie wartości piksela
gray2=(r+g+b)/3
gray2=(gray2*255).astype(int)
plt.figure()
plt.imshow(gray2, cmap='gray', vmin=0, vmax=255)
#Wyznaczenie luminancji piksela
gray3=0.21 *r+0.72 * g + 0.07 * b
gray3=(gray3*255).astype(int)
plt.figure()
plt.imshow(gray3, cmap='gray', vmin=0, vmax=255)
plt.figure()
plt.hist(gray1.flatten(),256)
plt.figure()
plt.hist(gray2.flatten(),256)
plt.figure()
plt.hist(gray3.flatten(),256)

plt.figure()
plt.hist(gray1.flatten(),bins=16)
gray16=(gray1/16).astype(int)*16+8
plt.figure()
plt.imshow(gray16, cmap='gray', vmin=0, vmax=255)

#BINARYZACJA
def OTSU(histogram):
    suma=sum(histogram);
    procent=histogram/suma;
    pob=np.zeros(256)
    a=0;
    ptla=np.zeros(256)
    for x in range(0,255):
        pob[x]=a+procent[x];
        ptla[x]=1-pob[x];
        a=a+procent[x];
    uob=np.zeros(256)
    utla=np.zeros(256)
    for x in range(0,255):
        a=0;
        b=0;
        for k in range(0,x):
            a=a+k*procent[k];
        for k in range(x+1,255):
            b=b+k*procent[k];
        uob[x]=a/pob[x];
        utla[x]=b/ptla[x];
    maks=0;
    for tt in range(0,255):
        new=pob[tt]*ptla[tt]*(uob[tt]-utla[tt])*(uob[tt]-utla[tt]);
        if new>maks:
            maks=new;
            t=tt;
    return t;
img2 = mpimg.imread('Binaryzacja.png')
r, g, b = img2[:,:,0], img2[:,:,1], img2[:,:,2]
gBin=0.21 *r+0.72 * g + 0.07 * b
gBin=(gBin*255).astype(int)
plt.figure()
plt.imshow(gBin, cmap='gray', vmin=0, vmax=255)
plt.figure()
plt.hist(gBin.flatten(),256)
hist,bins_edges=np.histogram(gBin.flatten(),256)
treshold=OTSU(hist)
print (treshold)
gBin[gBin<=treshold]=0;
gBin[gBin>treshold]=1;
plt.figure()
plt.imshow(gBin, cmap='gray', vmin=0, vmax=1)