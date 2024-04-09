/*
 * Corso di Sistemi Operativi 2020
 * Schema soluzione seconda verifica: semafori
 *
 * Author: Riccardo Focardi
 */
#include <semaphore.h>
#include <stdio.h>
// dichiarazione semafori e variabili globali
sem_t mutex_my_bowls;
sem_t solo_mutex_per_sto_giro;
int numero_di_palline_uscite_dal_manuto_e_che_vorrebbero_alliberare_le_altre;
int numero_vero_di_palline_reali_virtuali_di_Manuto;
// inizializza semafori e variabili
// ATTENZIONE dim è la dimensione del gruppo che deve entrare
// nell'imbuto.
void inizializza_sem(int dim) {
    sem_init(&mutex_my_bowls, 0, dim);
    sem_init(&solo_mutex_per_sto_giro, 0, 1);
    numero_vero_di_palline_reali_virtuali_di_Manuto = dim;
    numero_di_palline_uscite_dal_manuto_e_che_vorrebbero_alliberare_le_altre = 0;
}
 
// distruggi i semafori
void distruggi_sem() {
    sem_destroy(&mutex_my_bowls);
}
 
// attende di entrare nell'imbuto
void entra_imbuto() {
    sem_wait(&mutex_my_bowls);
}
 
// esce dall'imbuto
// ATTENZIONE usare una variabile intera condivisa per sapere 
// quante palline sono uscite (da proteggere con una sezione critica)!
void esci_imbuto() {
    sem_wait(&solo_mutex_per_sto_giro);
    numero_di_palline_uscite_dal_manuto_e_che_vorrebbero_alliberare_le_altre++;
    sem_post(&solo_mutex_per_sto_giro);

    if(numero_di_palline_uscite_dal_manuto_e_che_vorrebbero_alliberare_le_altre == numero_vero_di_palline_reali_virtuali_di_Manuto){
        for (int i = 0; i < numero_vero_di_palline_reali_virtuali_di_Manuto; ++i) {
            sem_post(&mutex_my_bowls);
        }
        numero_di_palline_uscite_dal_manuto_e_che_vorrebbero_alliberare_le_altre = 0;
    }


}
