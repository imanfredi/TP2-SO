#include <selfLib.h>
#include <stringFunctions.h>
#include <test_util.h>
#include <utils.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 * 1024 * 100  //Should be around 80% of memory managed by the MM
#define NULL 0

typedef struct MM_rq {
    void *address;
    uint32_t size;
} mm_rq;

int test_mm() {
    mm_rq mm_rqs[MAX_BLOCKS];
    
    
    printf("Testing MM\n");
    while (1) {
        uint8_t rq = 0;
        uint32_t total = 0;

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < MAX_MEMORY) {
            mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
            mm_rqs[rq].address = (void *)malloc2(mm_rqs[rq].size);
            if (mm_rqs[rq].address == NULL) {
                printf("Antes del error veo si es null!\n");
                return 1;
            }
            total += mm_rqs[rq].size;
            rq++;
        }

        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address != 0)
                memset(mm_rqs[i].address, i, mm_rqs[i].size);

        // Check
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address != 0)
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                    printf("ERROR!\n");

        // Free
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address != 0)
                free2(mm_rqs[i].address);
    }
}
