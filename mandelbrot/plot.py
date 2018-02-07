#!/usr/bin/env python3

import sys
import numpy as np
import matplotlib.cm as cm
from PIL import Image

COLORMAP = 'coolwarm'
DEFAULT_PATH = 'color.txt'
SAVED_IMAGE_EXT = '.bmp'

def load_image(filepath):
    
    read_vals = []

    with open(filepath, 'r') as f:
        for row in f.readlines():
            read_vals.append(row.rstrip().split(' '))

    return np.array(read_vals, dtype=np.uint8)

if __name__ == '__main__':
    try:
        filepath = sys.argv[1]
    except IndexError:
        filepath = DEFAULT_PATH

    image_vals = load_image(filepath)
    cmap = cm.get_cmap(COLORMAP)
    image_vals = np.uint8(255 * cmap(image_vals))
    image = Image.fromarray(image_vals)

    savepath = filepath.split('.')[0] + SAVED_IMAGE_EXT
    image.save(savepath)

