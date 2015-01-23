#include "data.h"

// Data generation/free
struct Data* data_generate(uint16_t data_number)
{
    //Allocates memory for the struct
    struct Data* d = malloc(sizeof(struct Data));
    if (d == NULL)
         error("ERROR : data_generate : Dynamic allocation not possible for the data structure");

    //Allocates memory for the array
    uint16_t n = (data_number - 1)/8 + 1; // Number of byte needed
    d->data_array = calloc(n,sizeof(uint8_t)); //Automatically sets the bits to zero
    if (d->data_array == NULL)
         error("ERROR : data_generate : Dynamic allocation not possible for the array");

    //Sets the data_number field
    d->data_number = data_number;

    return d;
}

void data_free(struct Data* d)
{
    free(d->data_array);
    free(d);
}



// Data operations
uint8_t data_get(uint16_t n, struct Data* d)
{
    if(n < d->data_number)
    {
        //printf(" %d-", n - 8*(n/8));
        return ( d->data_array[n/8] >> ( n - 8*(n/8) ) ) & 1;
    }
    else
        error("ERROR: Incorect data number. Function data_get");
    return EXIT_FAILURE;
}

void data_set(uint16_t n, uint8_t data, struct Data* d)
{
    if(n < d->data_number)
    {
        if (data)
        {
            d->data_array[n / 8] |=  (1 << ( n - 8*(n/8) ));
        }
        else
        {
            d->data_array[n / 8] &=  ~(1 << ( n - 8*(n/8) ));
        }
    }
    else
        error("ERROR: Incorect data number. Function data_set.");
}

void data_delete(uint16_t n, struct Data* d)
{
    if (0 <= n && n < d->data_number)
    {
        for(uint16_t i = n; i < d->data_number - 1; i++)
            data_set(i, data_get(i + 1, d), d);

        /*
            Determines wether or not it is necessary to allocate a new block of memory.
            Number of byte needed. Taken from data_generate: floor((d->data_base.l * d->data_number - 1)/8) + 1
        */
        uint16_t a = (BASE_L * (d->data_number - 1) - 1)/8 + 1;
        uint16_t b = (BASE_L * (d->data_number / 8)) + 1;

        if(a < b)
            d->data_array = realloc(d->data_array, a);
        else
            data_set(d->data_number - 1, 0, d);

        d->data_number -= 1;
    }
    else
        error("ERROR: Deleting a wrong block. Function data_delete.");
}


#ifdef DEBUG
    void data_show(struct Data* d)
    {
        uint8_t data;
        #ifdef __AVR__

        #else
            printf("\nShow %d data :\n(", d->data_number);
            for(uint16_t i = 0; i < d->data_number; i++)
            {
                data = data_get(i, d);
                printf("%d ", data);
            }
            printf(")\n\n");
        #endif // __AVR__
    }
#endif
