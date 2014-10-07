#include "hamming.h"
#include <math.h>

/*
struct Data hamming_encode(struct Hamming_config * conf, struct Data * word)
{
    struct Matrix word_matrix = matrix_generate(word->data_number, 1, word->data_base);
    word_matrix->data = word;

    struct Matrix result = matrix_mul(conf->control_matrix, word_matrix);
    return result.data;
}

struct Data hamming_decode(struct * Hamming_config conf, struct Data * word)
{

}

struct Data hamming_check(struct * Hamming_config conf, struct Data * word)
{
}
*/
struct Matrix hamming_generate_control_matrix(struct Hamming_config * conf)
{
    uint16_t cols = int_pow(2, conf->m);
    struct Matrix control = matrix_generate(conf->m, cols, conf->base); // Generation de la matrice de controle

    // Remplissage de la matrice de controle
    /* Principe pour la base 2 :
        On remplit la matrice de maniere recursive avec les vecteur de la base canonique de 2^m
        On supprime ensuite les lignes 2^i avec i dans [0,m-1]
    */

    for(uint8_t i = 1; i <= conf->m; i++)
        for(uint16_t j = 1; j <= cols; j++)
            if(((j - 1) / int_pow(2, conf->m - i)) % 2 == 0)
                matrix_set(&control, i, j, 0);
            else
                matrix_set(&control, i, j, 1);

    // On rend la matrice systématique
    matrix_del_col(1, &control);
    for(uint8_t j = 0; j < conf->m; j++)
        matrix_del_col(int_pow(2, j) - j, &control);

    struct Matrix identity = matrix_generate(conf->m, conf->m, conf->base);
    matrix_make_identity(&identity);

    return matrix_collapse_right(&control, &identity);
    // Remplissage de la matrice de controle
}


struct Matrix hamming_generate_gen_matrix(struct Hamming_config * conf)
{
    struct Matrix control = matrix_copy(&(conf->control_matrix)); // Récupération de la matrice de controle

    for(uint8_t i = 1; i <= conf->m; i++)
        matrix_del_col(control.cols, &control); // On supprime les m dernières colonnes qui correspondent a l'identité

    struct Matrix identity = matrix_generate(int_pow(2, conf->m) - 1 - conf->m, int_pow(2, conf->m) - 1 - conf->m, conf->base);
    matrix_make_identity(&identity);

    struct Matrix gen = matrix_collapse_down(&identity, &control); // On colle ensuite cette matrice avec l'identité
    return gen;
}
/*
uint16_t hamming_length(struct Data * word1, struct Data * word2) // Renvoie la distance de hamming entre 2 codes
{
    // Formule valable en base 2 seulement
    uint16_t r = 0;

    for(uint8_t i = 0; i < word1->data_number; i++)
        r += data_get(&word1, i) ^ data_get(&word2, i);

    return r;
}
*/
struct Hamming_config hamming_generate_config(uint8_t l, uint8_t m) // l = longueur des elements de la base, m = paramètre de hamming
{
    struct Hamming_config conf;
    struct Base base;

    // Configuration de la base
    base.d = int_pow(2, l);
    base.l = l;

    // Calcul des paramètres
    conf.total_size = (int_pow(base.d, m) - 1)/(base.d - 1);
    conf.word_size = conf.total_size - m;

    // Enregistrement des paramètres
    conf.base = base;
    conf.m = m;

    // Creation de la matrice de controle
    conf.control_matrix = hamming_generate_control_matrix(&conf);
    conf.generatrix_matrix = hamming_generate_gen_matrix(&conf);
    return conf;
}
