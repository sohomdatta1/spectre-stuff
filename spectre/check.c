#include <stdio.h>
#include <stdint.h>
#include <x86intrin.h>

#include "./libcache/cacheutils.h"

int main()
{
    unsigned int dummy = 0;
    if(!CACHE_MISS) 
        CACHE_MISS = detect_flush_reload_threshold();
    fprintf(stderr, "[\x1b[33m*\x1b[0m] Flush+Reload Threshold: \x1b[33m%zd\x1b[0m\n", CACHE_MISS);

    volatile uint8_t *addr;
    register uint64_t i;
    unsigned int junk = 0;
    register uint64_t t1, t2;

    char str[16];
    long long int sum = 0;
    char * arb_ptr = str + 8;
    fprintf(stderr, "Accessing 8th byte in %p for reload detection\n", str);
    maccess(arb_ptr);
    for(int i = 0; i < 1000000; i++) {
        int start = __rdtscp(&dummy);
        maccess(arb_ptr);
        int end = __rdtscp(&dummy);
        printf("--> %d\n", end - start);
        sum += (end - start);
        _mm_mfence();
        _mm_clflush(arb_ptr);
    }
    sum /= 1000000;
    char new_str[16];
    unsigned int a = (sum);
    printf("-----------------------------\n");
    //for(int i = 0; i < 100; i++)
    maccess(new_str + 10);
    fprintf(stderr, "AVG time for flush + reload = %d\n", sum);
    fprintf(stderr, "Accessed %p + 10", new_str);
    for(int i = 0; i < 16; i++) {
        int setup_sum = 0;
        int start = __rdtscp(&dummy);
        maccess(new_str + i);
        int end = __rdtscp(&dummy);
        _mm_mfence();
        _mm_clflush(new_str+i);
        setup_sum = (end - start);
        if ( setup_sum < CACHE_MISS)
            fprintf(stderr, "Possibly accessed %p + %d according to *canella* thresholding since access timing is %d\n", new_str, i, setup_sum);
        else
            fprintf(stdout, "Possibly accessed %p + %d according to *canella* thresholding since access timing is %d\n", new_str, i, setup_sum);

        if ( setup_sum < a)
            fprintf(stderr, "Possibly accessed %p + %d according to thresholding at home since access timing is %d\n", new_str, i, setup_sum);
        else
            fprintf(stdout, "Possibly accessed %p + %d according to thresholding at home since access timing is %d\n", new_str, i, setup_sum);
    }
    sched_yield();
    
}