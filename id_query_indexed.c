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

struct indexed_data* mk_indexed(struct record* rs, int n){
    struct indexed_data* indexed_data = malloc(sizeof(struct indexed_data));
    indexed_data->irs = malloc(n*sizeof(struct index_record));
    for(int i = 0; i < n; i++){  
        indexed_data->irs[i] = (struct index_record){rs[i].osm_id, &rs[i]};
      }
    indexed_data->n = n;
    return indexed_data;    
}

void free_indexed(struct indexed_data* data){
    //Jeg er ikke sikker på om dette virker eller om der bliver leaket memory
    free_records(data->irs->record, data->n);
    free(data);
    return;
}

const struct record* lookup_indexed(struct indexed_data *data, int64_t needle){
    int n = data->n;
    struct index_record* irs = data->irs;
    for (int i = 0; i<n; i++){
        if(irs[i].osm_id == needle){
            return irs[i].record;
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