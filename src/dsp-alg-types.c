
#include <stdlib.h>
#include <stdio.h>

#include <dsp-alg-types.h>

t_num pi_sr(size_t sample_rate)
{
    return PI * (t_num) sample_rate;
}

t_num tau_sr(size_t sample_rate)
{
    return TAU * (t_num) sample_rate;
}


size_t control_rate(size_t sample_rate, size_t block_size)
{
    return sample_rate / block_size;
}


t_num *sig_init(size_t block_size)
{
    return (t_num *) calloc(block_size, sizeof(t_num));
}

void sig_free(t_num *a)
{
    free(a);
}


t_bool *sig_bool_init(size_t block_size)
{
    return (t_bool *) calloc(block_size, sizeof(t_bool));
}

void sig_bool_free(t_bool *a)
{
    free(a);
}

void sig_print(size_t n, const t_num *in)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("%.2f", *in++);
        if (i % (n-1) != 0 || i == 0)
            printf(", ");
    }
    printf("\n");
}



