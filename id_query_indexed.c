#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"


struct indexed_data {
    struct record *rs;
    int64_t *osm_id;
    int n; 
};

struct indexed_data* mk_indexed(struct record* rs, int n){
    struct indexed_data* indexed_data = malloc(sizeof(struct indexed_data));
    indexed_data->osm_id = malloc(n*sizeof(int64_t));
    for(int i = 0; i < n; i++){  
        indexed_data->osm_id[i] = rs[i].osm_id;
    }
    indexed_data->rs = rs;
    indexed_data->n = n;    
    return indexed_data;    
}

void free_indexed(struct indexed_data* data){
    free(data->osm_id);
    free(data);
    return;
}

const struct record* lookup_indexed(struct indexed_data *data, int64_t needle){
    int n = data->n;
    for (int i = 0; i < n; i++){
        if(data->osm_id[i] == needle){
            return &data->rs[i];
        }
    }
    return NULL;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_indexed);
}
