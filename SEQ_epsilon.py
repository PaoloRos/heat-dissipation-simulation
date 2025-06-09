#===----------------------------------------------------------------------===//
#
# Progetto Heat Dissipation
# Corso di Tecniche di Programmazione Avanzata, Università di Trento
# Autore: Paolo Rossi
# Data: 06/06/2025
#
#===----------------------------------------------------------------------===//


import subprocess
import sys
import numpy as np
import shutil
import os
import csv

# 1. Compila il software
compile_cmd = ["g++", "-fopenmp", 
               "seq_main.cc",
               "matrix.cc",
               "std_libraries.cc",
               "-o", 
               "sequel"]

compilation = subprocess.run(compile_cmd, capture_output=True, text=True)
if compilation.returncode != 0:
    print("Build error: ", compilation.stderr)
    sys.exit(1)
else:
    print("Build succeeded.")

# 2. Lancio del software
WARMUP = 3
N = 50  # exe. repetitions
matrix_size = int(sys.argv[1])#32
args = [str(matrix_size), "10000000", "50"]

print("Execution:\n")

#T = int(sys.argv[1])

output_csv = "SEQ_timings.csv"

# 1. Inizializza il CSV
with open(output_csv, "w", newline="") as f_csv:
    writer = csv.writer(f_csv)
    writer.writerow(["T", "Time"])

#for cycle in range(T):

    #print(f"Execution {cycle+1} -> {args[0]}x{args[0]}:")
    
    # 1. Esegue il programma
    subprocess.run(["./sequel"] + args, check=True)

    # 2.Copia temp_out.txt in SEQ_output_<cycle>.txt
    #src = "temp_output.txt"
    #dst = f"SEQ_output_{cycle}.txt"
#
    #if not os.path.exists(src):
    #    print(f"Temporary '{src}' not found.")
    #    sys.exit(1)
    #
    #try:
    #    shutil.copyfile(src, dst)
    #except Exception as e:
    #    print(f"Error during the copy of {src} to {dst}: {e}")
    #    sys.exit(1)
#
    ## Lettura time.csv e scrittura su file
    #time = []
    #try:
    #    with open("time.csv") as f_in:
    #        for idx, line in enumerate(f_in):
    #            if idx < WARMUP:
    #                continue
    #            try:
    #                t = float(line.strip())
    #                time.append(t)
#
    #                # Scrivi subito su CSV (modalità append)
    #                with open(output_csv, "a", newline="") as f_out:
    #                    writer = csv.writer(f_out)
    #                    writer.writerow([cycle + 1, t])
    #            except ValueError:
    #                continue
    #except FileNotFoundError:
    #    print("File 'time.csv' not found.")
    #    sys.exit(1)
#
    #if not time:
    #    print("No data found.\n")
    #    sys.exit(1)
#
    ## 4. Statistiche
    #media = np.mean(time)
    #dev_std = np.std(time)
#
    #print(f"Mean on {len(time)} elements: {media:.6f} (s)")
    #print(f"Standard deviation: {dev_std:.6f} (s)\n")
    #print("\n--------\n")
#
    ## 5. Aggiorna dimensione
    #args[0] = str(int(args[0]) * 2)
