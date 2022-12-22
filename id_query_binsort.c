#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include "record.h"
#include "id_query.h"

struct index {
    int64_t osm_id;
    int original_index;
};

struct indexed_data {
    struct record *rs;
    struct index *index;
    int n; 
};

int cmpfnc (const void* a, const void* b){
    if(((struct index*)a)->osm_id - ((struct index*)b)->osm_id < 0ll){
        return -1;
    }
    if (((struct index*)a)->osm_id - ((struct index*)b)->osm_id > 0ll) {
        return 1;
    }
    else{
        return 0;
    }
}

struct indexed_data* mk_binsort (struct record* rs, int n){
    struct indexed_data* indexed_data = malloc(sizeof(struct indexed_data));
    struct index* index = malloc(n*sizeof(struct index));
    for(int i = 0; i < n; i++){ 
        index[i].osm_id = rs[i].osm_id;
        index[i].original_index = i;
    }
    qsort(index, n, sizeof(struct index), cmpfnc);
    indexed_data->index = index;
    indexed_data->rs = rs;
    indexed_data->n = n;
    return indexed_data;    
}

void free_indexed(struct indexed_data* data){
    free(data->index);
    free(data);
    return;
}

const struct record* lookup_binsort(struct indexed_data *data, int64_t needle){
    int n = data->n;
    struct index* index = data->index;
    int low = 0, high = n;
    int i = 0;
    while(high-low > 1){
        i = (low+high)/2;
        if(index[i].osm_id == needle){
            return &data->rs[index[i].original_index];
        }
        if (index[i].osm_id > needle){
            high=i;
        }
        else{
            low=i;
        }
    }
    if(index[high].osm_id == needle){ // If high-low=1, sometimes high will not be checked in the above loop due integer division rounding down.
        return &data->rs[index[i].original_index];
    }
    return NULL;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_binsort,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_binsort);
}
