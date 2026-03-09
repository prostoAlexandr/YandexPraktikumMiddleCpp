```bash
g++ --std=gnu++23 main.cpp -o app -ljemalloc
MALLOC_CONF=prof_leak:true,lg_prof_sample:0,prof_final:true,prof:true ./app

Number of allocations:   241
Number of deallocations: 75
Number of array allocations:   20
Number of array deallocations: 17

Number of leaks in total: 169

Jemalloc approx. number of leaks:   0
<jemalloc>: Leak approximation summary: ~885760 bytes, ~171 objects, >= 63 contexts
<jemalloc>: Run jeprof on dump output for leak detail
```