#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

def load_image(filepath):
    
    read_vals = []

    with open(filepath, 'r') as f:
        for row in f.readlines():
            read_vals.append(row.rstrip().split(' '))

    return np.array(read_vals, dtype=np.uint8)

if __name__ == '__main__':
    image_vals = load_image('color.txt')
    plt.imshow(image_vals)
    plt.show()
