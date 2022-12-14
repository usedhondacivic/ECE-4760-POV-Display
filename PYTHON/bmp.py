import numpy as np
import pandas as pd
from PIL import Image
from matplotlib import pyplot as plt

path = "./pov_images/color_wheel.bmp"

# img = np.array(Image.open(path))
# print('\n\n', img.shape)

def bmp_to_rad(path, num_pix, res):
    img = np.array(Image.open(path))

    w, h, _ = img.shape
    out_r = min(int(w/2)-1,int(h/2)-1)
    ths = np.linspace(-np.pi*0.6, 1.4*np.pi, res)
    rs = np.linspace(0, out_r, num_pix)
    xs = np.array([rs * np.sin(th) for th in ths], dtype=int) + int(w/2)
    ys = -((np.array([rs * np.cos(th) for th in ths], dtype=int) + int(h/2)))
    ret = img[xs, ys]
    # print(ret.shape)
    return ret, xs, ys

def display_bmp():
    rad, x, y = bmp_to_rad(path, 40, 50)

    color_rad = rad / 255

    m, n, _ = color_rad.shape

    print(x)
    print('----')
    print(y)
    print(color_rad)
    cs = []
    for i in range(m):
        for j in range(n):
            cs.append(tuple(color_rad[i,j]))

    plt.scatter(y, -x, c=cs)
    plt.show()

# display_bmp()