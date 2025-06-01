import subprocess
import sys
import numpy as np

# 1. Build the software
compile_cmd = ["g++", "-fopenmp", 
               "par_block.cc", 
               "matrix.cc",
               "std_libraries.cc",
               "-o", "parallel"]

compilation = subprocess.run(compile_cmd, capture_output=True, text=True)
if compilation.returncode != 0:
    print("Build error: ", compilation.stderr)
    sys.exit(1)
else:
    print("Build succeeded.")

# 2. Execute the software
WARMUP = 3
N = 50  # exe. repetitions
args = ["32", "4", "1000", "50"]

print("Execution:")
subprocess.run( ["./parallel"] + args)

#for i in range(N + WARMUP):
#    print(f"Execution {i+1}/{N}:")
#    subprocess.run(["./par_block"] + args)

# 3. Read data
time = []
with open("par_time.csv") as my_file:
    for idx, line in enumerate(my_file):
        if idx < WARMUP:
            continue # Skip first values
        try:
            time.append(float(line.strip()))
        except ValueError:
            continue

if not time:
    print("No data founded.\n")
    sys.exit(1)

# 4. Statistics
media = np.mean(time)
dev_std = np.std(time)

print(f"Mean on {idx + 1 - WARMUP} elements: {media:.6f} (s)")
print(f"Standard deviation: {dev_std:.6f} (s)\n")
