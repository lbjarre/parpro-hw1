#!/usr/bin/env python3

import sys
import numpy as np
import matplotlib.pyplot as plt

def load_image(filepath):
    
    read_vals = []

    with open(filepath, 'r') as f:
        for row in f.readlines():
            read_vals.append(row.rstrip().split(' '))

    return np.array(read_vals, dtype=np.uint8)

if __name__ == '__main__':
    try:
        image_path = sys.argv[1]
    except IndexError:
        image_path = 'color.txt'
    image_vals = load_image(image_path)
    plt.imshow(image_vals)
    plt.show()
