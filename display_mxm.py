#!/usr/bin/env python3

#import numpy as np
#import matplotlib.pyplot as plt
#
#def load_numbers(file_name='output.txt'):
#	with open(file_name, 'r') as f:
#		string_data = f.read()
#	data = []
#	for line in string_data.split('\n'):
#		if line == "":
#			continue
#
#		numbers = line.split(" ")
#		numbers = [float(x) for x in numbers]
#		data.append(numbers)
#
#	return np.array(data)
#
#def main():
#	X = load_numbers()
#	plt.imshow(X, cmap="plasma")
#	plt.show()
#
#if __name__ == '__main__':
#	main()


# ---- se il make file contiene il nome del file ----
import numpy as np
import matplotlib.pyplot as plt
import argparse

def load_numbers(file_name='output.txt'):
    with open(file_name, 'r') as f:
        string_data = f.read()
    data = []
    for line in string_data.split('\n'):
        if line == "":
            continue
        numbers = line.split()
        numbers = [float(x) for x in numbers]
        data.append(numbers)
    return np.array(data)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', type=str)
    args = parser.parse_args()

    X = load_numbers(args.filename)
    plt.imshow(X, cmap="plasma")
    plt.show()

if __name__ == '__main__':
    main()