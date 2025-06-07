# Progetto Heat Dissipation

**Corso di Programmazione Avanzata**, _Università di Trento_

**Autore:** Paolo Rossi

**Data:** 07/06/2025

## Istruzioni sull'uso

1. **Installazione:** compilare il progetto digitando da `riga di comando` uno dei eguenti _target_:
    ```
    make seq #per il codice sequenziale
    make par #per il codice parallelo
    ```
    Questi produrranno il file eseguibile del programma.

2. **Test:** è possibile verificare il corretto funzionamento del software digitando da `riga di comando`:
    ```
    ./seq 32 #per il codice sequenziale
    ./par 32 4 #per il codice parallelo
    ```
    Questo effettuerà un calcolo su di una matrice 32x32.

4. **Raccolta dati:** digitare da riga di comando:
    ```
    python3 SEQ_launcher.py <num_matrici> #per il codice sequenziale
    python3 PAR_launcher.py <num_matrici> #per il codice parallelo
    ```
    Dove <num_matrici> rappresenta il numero di matrici per le quali si vuole raccogliere le misure. Si noti che la dimensione delle matrici è sempre potenza di due a partire da 32.

5. **Visualizzazione grafica**: digitare da riga di comando:
    ```
    make fig f=<nome_file>
    ```
    Sostituendo <nome_file> con il nome dell'output prodotto.

5. **Disinstallazione:** è possibile disinstallare gli eseguibili e cancellare i file di testo digitando da `riga di comando`:
    ```
    make clean
    make ctxt
    ```