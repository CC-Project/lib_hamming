#ifndef HAMMING_H
    #define HAMMING_H
    #pragma once

    #ifndef __AVR__
        #include <inttypes.h>
        #include <stdlib.h>
    #endif // __AVR__

    // Include
    #include "../../lib/lib_utilities/import.h"
    #include "../../lib/lib_data/import.h"
    #include "../matrix/matrix.h"
    #include"../../config.h"

    struct Hamming_config
    {
        // Corrections matrix
        struct Matrix* CONTROL_MATRIX; // Size: (2^m - 1) * m
        struct Matrix* GENERATOR_MATRIX;
        struct Data* SYNDROMES_ARRAY; // Le tableau de syndromes (m <= 8 donc les syndrome sont cod� sur 8 bits au max, d'ou le uint8_t)
    };

    // Set up
    struct Hamming_config* hamming_generate_config();
    void hamming_free_config(struct Hamming_config* conf);

    // Coding and decoding matrix
    void hamming_generate_generator_matrix(struct Hamming_config* conf);
    void hamming_generate_control_matrix(struct Hamming_config* conf);
    void hamming_generate_syndromes_array(struct Hamming_config* conf); // Generate the array of syndromes

    // Manipulation of data
    struct Matrix* hamming_encode(struct Matrix* word, struct Hamming_config* conf);
    struct Matrix* hamming_decode(struct Matrix* word, struct Hamming_config* conf);
    struct Matrix* hamming_correction(struct Matrix* word, struct Hamming_config* conf);
    struct Matrix* hamming_syndrome(struct Matrix* word, struct Hamming_config*conf); // Calcul le syndrome associ� a un code
    uint8_t hamming_check_syndrome(struct Matrix* word, struct Hamming_config* conf); // Retourne en binaire le numero du bit defectueux
#endif // HAMMING_H