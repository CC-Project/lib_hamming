#ifndef DATA_H
    #define DATA_H
    #include "base.h"

    struct Data
    {
        struct Base data_base;  // Base de travail
        uint16_t data_number;   // Nombre de donn�es contenu dans le tableau "data_array"
        uint8_t* data_array;  // Tableau de uint8_t contenant les donn�es
    };

    struct Data data_generate(struct Base base, uint16_t data_number);
    void data_delete(struct Data* d);
    uint8_t data_get(uint16_t n, struct Data* d); //Returns the n-th data stored. Starting from 0.
    void data_set(uint16_t n, uint8_t data, struct Data* d);
#endif // DATA_H

