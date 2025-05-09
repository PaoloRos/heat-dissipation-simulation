#!/usr/bin/env python3

import numpy as np

def save_numbers(n=32):
	X = np.zeros((n, n), dtype=float)

	file_name = 'output.txt'
	row_strings = []

	for row in X:
		line_string = ''
		for num in row:
			# add the number to the line string
			line_string += f"{num:3.2f} "
		# remove the last whitespace and add newline
		line_string = line_string[:-1] + "\n"

		
		row_strings.append(line_string)

	# remove newline from last string
	row_strings[-1] = row_strings[-1][:-1]

	with open(file_name, 'w') as f:
		f.write("".join(row_strings))

def read_numbers(file_name='output.txt'):
	with open(file_name, 'r') as f:
		string_data = f.read()

def main():
	save_numbers()

if __name__ == '__main__':
	main()
