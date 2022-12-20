#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include "record.h"
#include "coord_query.h"
double euclidean_distance(double x1, double y1, double x2, double y2) {
  double dx = x2 - x1;
  double dy = y2 - y1;
  return sqrt(dx*dx + dy*dy);
}
struct naive_data {
  struct record *rs;
  int n;
};

struct naive_data* mk_naive(struct record* rs, int n) {
  struct naive_data* naive_data = malloc(n*sizeof(struct naive_data));
  naive_data->rs = rs;
  naive_data->n = n;
  return naive_data;
  assert(0);
  // TODO
}

void free_naive(struct naive_data* data) {
  assert(0);
  // TODO
}

const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {
  assert(0);
  // TODO
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_naive,
                          (free_index_fn)free_naive,
                          (lookup_fn)lookup_naive);
}
