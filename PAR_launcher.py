import subprocess
import sys
import numpy as np
import shutil
import os
import csv

# 1. Build the software
compile_cmd = ["g++", "-fopenmp", 
               "par_main.cc", 
               "matrix.cc",
               "std_libraries.cc",
               "-o", 
               "parallel"]

compilation = subprocess.run(compile_cmd, capture_output=True, text=True)
if compilation.returncode != 0:
    print("Build error: ", compilation.stderr)
    sys.exit(1)
else:
    print("Build succeeded.")

# 2. Execute the software
WARMUP = 3
N = 50  # exe. repetitions
THD = 2

T = int(sys.argv[1]) #-> sempre 6 matrici

while THD <= 2:

    matrix_size = 32
    args = [str(matrix_size), str(THD), "1000", "50"]

    print("Execution:\n")
    #subprocess.run( ["./parallel"] + args)

    output_csv = "PAR_timings.csv"

    # 1. Inizializza file CSV
    with open(output_csv, "w", newline="") as f_csv:
        writer = csv.writer(f_csv)
        writer.writerow(["THD", "T", "Time"])

    for cycle in range(T):

        print(f"Execution {cycle+1} -> {args[0]}:")

        # 1. Esegui il programma
        subprocess.run(["./parallel"] + args, check=True)

        # 2. Copia da temp_out.txt in SEQ_output_<cycle>.txt
        src = "temp_output.txt"
        dst = f"PAR_output_{THD}_{cycle}.txt"

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
                            writer.writerow([THD,cycle + 1, t])
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

        # 5. Aggiorna variabili
        matrix_size *= 2
        THD *= 2

