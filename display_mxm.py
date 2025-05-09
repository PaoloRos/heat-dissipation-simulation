#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

def load_numbers(file_name='input_heat.txt'):
	with open(file_name, 'r') as f:
		string_data = f.read()
	data = []
	for line in string_data.split('\n'):
		if line == "":
			continue

		numbers = line.split(" ")
		numbers = [float(x) for x in numbers]
		data.append(numbers)

	return np.array(data)

def main():
	X = load_numbers()
	plt.imshow(X, cmap="gray")
	plt.show()

if __name__ == '__main__':
	main()
