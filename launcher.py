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
matrix_size = "32"
args = [matrix_size, "4", "1000", "50"]

print("Execution:\n")
#subprocess.run( ["./parallel"] + args)

T = int(sys.argv[1])

for cycle in range(T):

    print(f"Execution {cycle+1} -> {args[0]}:")
    
    subprocess.run(["./parallel"] + args)

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
    print("\n--------\n")

    args[0] = str( int(args[0]) * 2 )

