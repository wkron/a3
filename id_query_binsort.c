#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include "record.h"
#include "id_query.h"

struct index_record {
    int64_t osm_id;
    struct record *record;
};

struct indexed_data {
    struct index_record *irs;
    int n; 
};

int cmpfnc (const void* a, const void* b){
    if(((struct record*)a)->osm_id - ((struct record*)b)->osm_id < 0ll){
        return -1;
    }
    if (((struct record*)a)->osm_id - ((struct record*)b)->osm_id > 0ll) {
        return 1;
    }
    else{
        return 0;
    }
}

struct indexed_data* mk_binsort (struct record* rs, int n){
    struct indexed_data* indexed_data = malloc(sizeof(struct indexed_data));
    indexed_data->irs = malloc(n*sizeof(struct index_record));
    qsort(rs, n, sizeof(struct record), cmpfnc);
    for(int i = 0; i < n; i++){  
        indexed_data->irs[i] = (struct index_record){rs[i].osm_id, &rs[i]};
      }
    indexed_data->n = n;
    return indexed_data;    
}

void free_indexed(struct indexed_data* data){
    free_records(data->irs->record, data->n);
    free(data->irs);
    free(data);
    return;
}

const struct record* lookup_binsort(struct indexed_data *data, int64_t needle){
    int n = data->n;
    struct index_record* irs = data->irs;
    int64_t low = 0ll;
    int64_t high = (int64_t)n;
    int64_t i = 0ll;
    while(high-low > 1ll){
        i = (low+high)/2ll;
        if(irs[i].osm_id == needle){
            return irs[i].record;
        }
        if (irs[i].osm_id > needle){
            high=i;
        }
        else{
            low=i;
        }
    }
    if(irs[high].osm_id == needle){
        return irs[high].record;
    }
    return NULL;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_binsort,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_binsort);
}
