import subprocess
import sys
import numpy as np
import shutil
import os
import csv

# 1. Build the software
compile_cmd = ["g++", "-fopenmp", 
               #"par_block.cc", 
               "seq_main.cc",
               "matrix.cc",
               "std_libraries.cc",
               "-o", 
               #"parallel"] 
               "sequel"]

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
#args = [matrix_size, "4", "1000", "50"]
args = [matrix_size, "1000", "50"]

print("Execution:\n")
#subprocess.run( ["./parallel"] + args)

T = int(sys.argv[1]) #-> sempre 6 matrici

output_csv = "SEQ_timings.csv"

# 1. Inizializza file CSV
with open(output_csv, "w", newline="") as f_csv:
    writer = csv.writer(f_csv)
    writer.writerow(["T", "Time"])

for cycle in range(T):

    print(f"Execution {cycle+1} -> {args[0]}:")
    
    # 1. Esegui il programma
    subprocess.run(["./sequel"] + args, check=True)

    # 2. Copia da temp_out.txt in SEQ_output_<cycle>.txt
    src = "temp_output.txt"
    dst = f"SEQ_output_{cycle}.txt"

    if not os.path.exists(src):
        print(f"File temporaneo '{src}' non trovato.")
        sys.exit(1)
    
    try:
        shutil.copyfile(src, dst)
    except Exception as e:
        print(f"Errore durante la copia di {src} in {dst}: {e}")
        sys.exit(1)

    # Lettura time.csv
    time = []
    try:
        with open("time.csv") as f_in:
            for idx, line in enumerate(f_in):
                if idx < WARMUP:
                    continue
                try:
                    t = float(line.strip())
                    time.append(t)

                    # Scrivi subito su CSV (modalità append)
                    with open(output_csv, "a", newline="") as f_out:
                        writer = csv.writer(f_out)
                        writer.writerow([cycle + 1, t])
                except ValueError:
                    continue
    except FileNotFoundError:
        print("File 'time.csv' non trovato.")
        sys.exit(1)

    if not time:
        print("No data founded.\n")
        sys.exit(1)

    # 4. Statistiche
    media = np.mean(time)
    dev_std = np.std(time)

    print(f"Mean on {len(time)} elements: {media:.6f} (s)")
    print(f"Standard deviation: {dev_std:.6f} (s)\n")
    print("\n--------\n")

    # 5. Aggiorna argomento
    args[0] = str(int(args[0]) * 2)

#for cycle in range(T):
#
#    print(f"Execution {cycle+1} -> {args[0]}:")
#    
#    #subprocess.run(["./parallel"] + args)
#    subprocess.run(["./sequel"] + args)
#
#    # 3. Read data
#    time = []
#    with open("time.csv") as my_file:
#        for idx, line in enumerate(my_file):
#            if idx < WARMUP:
#                continue # Skip first values
#            try:
#                time.append(float(line.strip()))
#            except ValueError:
#                continue
#
#    if not time:
#        print("No data founded.\n")
#        sys.exit(1)
#
#    # 4. Statistics
#    media = np.mean(time)
#    dev_std = np.std(time)
#
#    print(f"Mean on {idx + 1 - WARMUP} elements: {media:.6f} (s)")
#    print(f"Standard deviation: {dev_std:.6f} (s)\n")
#    print("\n--------\n")
#
#    args[0] = str( int(args[0]) * 2 )

