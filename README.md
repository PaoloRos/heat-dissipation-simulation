[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/NNiiXw-q)

* non lavorare su sotto matrici locali -> avrebbe senso su una gpu ->

Idea:
    * far fare l'attualizzazione e le operazioni sullo stesso thread (tutto già in cache) -> risolvere solo l'interfaccia tra i diversi thread
    n.b. matrice temporanea -> la matrice t-1 è già in cache


NON fare sotto cicli che controllino i 5,5 e 20,20: -> NON fare mettere if:

cicle
{
    if(5,5)
        ...
    else
}

-> sol: fai attualizzazione e poi imponi a valore costante


Test prestazioni:

- media (+ sd)

- almeno con tre matrici di dimensioni diverse


# Cosa aggiungere

## matrix

[] sovraccarica l'accesso [][]

## display matrix

[] scala di colori sulla mappa stampata
