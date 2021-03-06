#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

/* TESTS
 *
 * */

// Test 1
/*int valuations[3][3] = {*/
    /*{1, 2, 10},*/
    /*{10, 2, 1},*/
    /*{2, 10, 1}*/
/*};*/

/*int test1 = 1000;*/

// Test 2
/*int valuations[3][6] = {*/
    /*{1, 2, 3, 4, 5, 6},*/
    /*{3, 2, 1, 5, 6, 4},*/
    /*{2, 3, 1, 6, 4, 5}*/
/*};*/

/*int test2 = 729;*/

// Test 3
/*int valuations[3][4] = {*/
    /*{1, 2, 3, 4},*/
    /*{2, 3, 4, 1},*/
    /*{4, 1, 2, 3}*/
/*};*/
/*int test3 = 112;*/

// Test 4
int valuations[4][12] = {
    {2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1},
    {0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 1}
};
/*int test4 = 0;*/

// Test 5
/*int valuations[2][6] = {*/
    /*{2, 0, 2, 0, 4, 0},*/
    /*{0, 2, 1, 4, 0, 2}*/
/*};*/
/*int test5 = 24;*/

// Test 6
/*int valuations[4][5] = {*/
    /*{2, 0, 2, 0, 4},*/
    /*{0, 2, 1, 4, 0},*/
    /*{5, 2, 4, 2, 1},*/
    /*{0, 2, 1, 1, 0}*/
/*};*/
/*int test6 = 288;*/


/*int valuations[2][2] = {*/
    /*{1, 10},*/
    /*{1, 10}*/
/*};*/

int vals_r, vals_c;
const int n_rows = sizeof(valuations)/sizeof(valuations[0]);
const int n_cols = sizeof(valuations[0])/sizeof(valuations[0][0]);

/* FUNCTIONS
 *     mnw_allocation()
 *     nash_product()
 *     k_ary()
 *     envy_free()
 *     envy_free_1_any()
 *     envy_free_1_all()
 *
 */




float rand_gauss (void) {
  float v1,v2,s;

  do {
    v1 = 2.0 * ((float) rand()/RAND_MAX) - 1;
    v2 = 2.0 * ((float) rand()/RAND_MAX) - 1;

    s = v1*v1 + v2*v2;
  } while ( s >= 1.0 || s == 0 );

  if (s == 0.0)
    return 0.0;
  else
    return (v1*sqrt(-2.0 * log(s) / s));
}

/* Generate a random number from a normal distribution */
float log_normal() {
    float normal = rand_gauss();
    return exp(normal);
}


/* Generate a random 2-D array of valuations */
float ** generate_valuations(int n_rows, int n_cols) {
    
    /*float * values = calloc(n_rows * n_cols, sizeof(float));*/
    /*float ** r = malloc(n_rows*sizeof(float*));*/

    int i, j; 
    float row_sum;
   
    float ** r = (float **) malloc(n_rows * sizeof(float *));
    for (i = 0; i < n_rows; i++) {
        r[i] = (float*) malloc(n_cols * sizeof(float));
    }
    
    /*for (i = 0; i < n_rows; i++) {*/
        /*r[i] = values + i*n_cols;*/
    /*}*/

    for (i = 0; i < n_rows; i++) {
        for (j = 0; j < n_cols; j++) {
            // r[i][j] = (float) rand()/RAND_MAX;
            r[i][j] = (float) log_normal();
        }
    }
    
    // normalize each row
    for (i = 0; i < n_rows; i++) {
        row_sum = 0;
        for (j = 0; j < n_cols; j++) {
            row_sum += r[i][j];
        }
        for (j = 0; j < n_cols; j++) {
            r[i][j] /= row_sum;
        }
    }

    return r;
}

void destroy_array(float ** arr) {
    free(*arr);
    free(arr);
}



/* Given valuations and partition, determine if envy free */
float envy_free(float ** vals, int partition[n_cols], int vals_r, int vals_c) {
    int self, i, j, envy, other;
    float self_score, other_score, alpha, temp_alpha;
    envy = 0;
    alpha = 0.0;
    for (self = 0; self < vals_r; self++) {
        self_score = 0;
        for (i = 0; i < vals_c; i++) {
            if (partition[i] == self) {
                self_score += vals[self][i];
            }
        }
        for (other = 0; other < vals_r; other++) {
            if (other == self) {
                continue;
            }
            other_score = 0;
            for (j = 0; j < vals_c; j++) {
                if (partition[j] == other) {
                    other_score += vals[self][j];
                }
            }
            if (other_score > self_score) {
                envy = 1;
                temp_alpha = (float)other_score/(float)self_score;
                if (temp_alpha > alpha) {
                    alpha = temp_alpha;
                }

            }
        }
    }
    //return envy;
    return alpha;
}

/* Envy-Free up to 1 good, for any good you choose 
 *  */
int envy_free_1_any(float ** vals, int partition[n_cols], int vals_r, int vals_c) {
    // create list of other's items
    // loop through i=0 to len(list)-1 times, each time leaving out the i-th item
    // if self_score < other_score for any of these, ef_1 = 0
    // else: ef_1 = 1

    int self, i, j, ef_1, other;
    float self_score, other_score;
    ef_1 = 0;

    int others_items[vals_c];
    for (i = 0; i < vals_c; i++) {
        others_items[i] = -1;
    }

    for (self = 0; self < vals_r; self++) {
        self_score = 0;
        for (i = 0; i < vals_c; i++) {
            if (partition[i] == self) {
                self_score += vals[self][i];
            }
        }

        for (other = 0; other < vals_r; other++) {
            if (other == self) {
                continue;
            }

            int count = 0;
            for (j = 0; j < vals_c; j++) {
                if (partition[j] == other) {
                    others_items[count] = j;
                    count++;
                }
            }
            for (i = 0; i < vals_c; i++) {
                int skip = others_items[i];
                other_score = 0;
                for (j = 0; j < vals_c; j++) {
                    if (others_items[j] != -1 & others_items[j] != skip) {
                        other_score += vals[self][j];
                    }
                }
                printf("self: %f, ", self_score);
                printf("other: %f\n", other_score);
                if (self_score > other_score) {
                    ef_1 = 1;
                }
            }
        }
    }

    return ef_1;
}

/* Envy-Free up to 1 good, for all possible goods chosen */
int envy_free_1_all(float ** vals, int partition[n_cols], int vals_r, int vals_c) {
    int self, i, j, ef_1, other;
    float self_score, other_score;
    ef_1 = 1;

    int others_items[vals_c];
    for (i = 0; i < vals_c; i++) {
        others_items[i] = -1;
    }

    for (self = 0; self < vals_r; self++) {
        self_score = 0;
        for (i = 0; i < vals_c; i++) {
            if (partition[i] == self) {
                self_score += vals[self][i];
            }
        }

        for (other = 0; other < vals_r; other++) {
            if (other == self) {
                continue;
            }
            int count = 0;
            for (j = 0; j < vals_c; j++) {
                if (partition[j] == other) {
                    others_items[count] = j;
                    count++;
                }
            }
            
            for (i = 0; i < vals_c; i++) {
                int skip = others_items[i];
                other_score = 0;
                for (j = 0; j < vals_c; j++) {
                    if (others_items[j] > 0 & others_items[j] != skip & skip > -1) {
                        other_score += vals[self][j];
                    }
                }
                if (self_score < other_score) {
                    ef_1 = 0;
                }
            }
        }
    }

    return ef_1;
}


/* Given decimal n, returns k-ary equivalent */
int64_t k_ary(int n, int k) {
    int rem;
    rem = 1;
    int64_t new, base;
    new = 0;
    base = 1;
    while (n > 0) {
        rem = n % k;
        new = new + rem*base;
        n /= k;
        base *= 10;
    }
    return new;
}

/* Convert number to char array */
void fix_format(char buffer[], int vals_c) {
    int i;
    char temp[vals_c];
    for (i = 0; i < vals_c; i++) {
        if (buffer[i] == '\0') {
            temp[vals_c - i - 1] = '0';
        } else {
            temp[vals_c - i - 1] = buffer[i];
        }
    }
    for (i = 0; i < vals_c; i++) {
        buffer[i] = temp[i];
    }
}

/*int nash_product(int vals[n_rows][n_cols], int part[vals_c], int vals_r, int vals_c) {*/
float nash_product(float ** vals, int part[vals_c], int vals_r, int vals_c) {
    float sums[vals_r];
    int i, j, player;
    float n_p;
    
    for (i = 0; i < vals_r; i++) {
        sums[i] = 0;
    }

    for (i = 0; i < vals_c; i++) {
        player = part[i];
        /*printf("part: [ ");*/
        /*for (j = 0; j < vals_c; j++) {*/
            /*printf("%d ", part[j]);*/
        /*}*/
        /*printf("]\n");*/
        sums[player] += vals[player][i];
    }

    n_p = 1.0;    
    for (i = 0; i < vals_r; i++) {
        n_p *= sums[i];
    }

    return n_p;
}


// int mnw_allocation(float ** vals, int vals_r, int vals_c) {
float mnw_allocation(float ** vals, int vals_r, int vals_c) {
    float max_product = -1.0;
    int max_partition[vals_c];

    int i, j, k;
    int64_t curr;
    int lim = pow(vals_r, vals_c);
    float curr_product;

    /* iterate through all partitions */
    for (i = 0; i < lim; i++) {
        int curr_partition[vals_c]; 
        curr = k_ary(i, vals_r);
        /*printf("%" PRId64 "\n", curr);*/
        char buffer[100];
        sprintf(buffer, "%0*" PRId64, vals_c, curr);

        for (j = 0; j < vals_c; j++) {
            int player = buffer[j] - '0';
            curr_partition[j] = player;
        }

        curr_product = nash_product(vals, curr_partition, vals_r, vals_c);
        
        if (curr_product > max_product) {
            max_product = curr_product;
            for (k = 0; k < vals_c; k++) {
                max_partition[k] = curr_partition[k];
            }
        }
    }

    //int envy = envy_free(vals, max_partition, vals_r, vals_c);
    float alpha = envy_free(vals, max_partition, vals_r, vals_c);
    

    //int ef_1_any = envy_free_1_any(vals, max_partition, vals_r, vals_c);
    
    //int ef_1_all = envy_free_1_all(vals, max_partition, vals_r, vals_c);
    
    /*printf("MNW solution: [");*/
    /*for (int i = 0; i < vals_c; i++) {*/
        /*printf(" %d", max_partition[i]);*/
    /*}*/
    /*printf(" ]\n");*/
    /*printf("MNW product: %f\n", max_product);*/
    /*printf("Envy-Free: %d\n", 1 - envy);*/

    // return ef_1_all;
    
    // return ef_1_any;

    // return (1 - envy);
    return alpha;
}

int mnw_allocation_ef1_all(float ** vals, int vals_r, int vals_c) {
    float max_product = -1.0;
    int max_partition[vals_c];

    int i, j, k;
    int64_t curr;
    int lim = pow(vals_r, vals_c);
    float curr_product;

    /* iterate through all partitions */
    for (i = 0; i < lim; i++) {
        int curr_partition[vals_c]; 
        curr = k_ary(i, vals_r);
        /*printf("%" PRId64 "\n", curr);*/
        char buffer[100];
        sprintf(buffer, "%0*" PRId64, vals_c, curr);

        for (j = 0; j < vals_c; j++) {
            int player = buffer[j] - '0';
            curr_partition[j] = player;
        }

        curr_product = nash_product(vals, curr_partition, vals_r, vals_c);
        
        if (curr_product > max_product) {
            max_product = curr_product;
            for (k = 0; k < vals_c; k++) {
                max_partition[k] = curr_partition[k];
            }
        }
    }

    /*printf("MNW solution: [");*/
    /*for (int i = 0; i < vals_c; i++) {*/
        /*printf(" %d", max_partition[i]);*/
    /*}*/
    /*printf(" ]\n");*/
    /*printf("MNW product: %f\n", max_product);*/
    /*printf("Envy-Free: %d\n", 1 - envy);*/
    
    int ef_1_all = envy_free_1_all(vals, max_partition, vals_r, vals_c);
    
    return ef_1_all;
}


/* Each trial: Generate a random 2-D NxM arrays of valuations, and determine if envy-free
 * Run K trials, and create array of K values (0, 1)
 * Return the percent of arrays that are EF */

float percent_ef_test(int k, int n_rows, int n_cols) {
    printf("%d items\n", n_cols); 
    
    clock_t start = clock(), diff;

    srand((time(NULL)));
    int i, num_ef;
    float total_alpha;
    num_ef = 0;

    for (i = 0; i < k; i++) {
        float ** curr_valuations = generate_valuations(n_rows, n_cols);
        // num_ef += mnw_allocation(curr_valuations, n_rows, n_cols);
        total_alpha += mnw_allocation(curr_valuations, n_rows, n_cols);
        destroy_array(curr_valuations);
    }

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time Elapsed: %ds %dms\n", msec/1000, msec%1000);
    printf("EF: %f\n", num_ef/(float)k);

    // return num_ef/(float)k;
    return total_alpha/(float)k;
}

float percent_ef1_any_test(int k, int n_rows, int n_cols) {
    
    int i, j, idx, num_ef;
    num_ef = 0;
    
    for (idx = 0; idx < k; idx++) {
        /*float ** curr_valuations = generate_valuations(n_rows, n_cols);*/
        float ** ex_valuations = (float **) malloc(n_rows * sizeof(float *));
        for (i = 0; i < n_rows; i++) {
            ex_valuations[i] = (float*) malloc(n_cols * sizeof(float));
        }
    
        /*for (i = 0; i < n_rows; i++) {*/
            /*for (j = 0; j < n_cols; j++) {*/
                /*if (i == j) {*/
                    /*ex_valuations[i][j] = (float) 1;*/
                /*} else {*/
                    /*ex_valuations[i][j] = (float) 0;*/
                /*}*/
            /*}*/
        /*}*/

        ex_valuations[0][0] = 6;
        ex_valuations[0][1] = 6;
        ex_valuations[0][2] = 1;
        ex_valuations[0][3] = 0;
        ex_valuations[1][0] = 0.9;
        ex_valuations[1][1] = 0.9;
        ex_valuations[1][2] = 1;
        ex_valuations[1][3] = 0.9;
        ex_valuations[2][0] = 3;
        ex_valuations[2][1] = 3;
        ex_valuations[2][2] = 2;
        ex_valuations[2][3] = 1;

        int max_partition[4] = {0, 0, 2, 1};
        int ef_1_any = envy_free_1_any(ex_valuations, max_partition, n_rows, n_cols);
        printf("got: %d\n", ef_1_any);
        printf("expected: %d\n", 0);

        // num_ef += mnw_allocation_ef1_any(ex_valuations, n_rows, n_cols);
        destroy_array(ex_valuations);
    }

    return num_ef/(float)k;
}


float percent_ef1_all_test(int k, int n_rows, int n_cols) {
    
    int i, j, idx, num_ef1_all;
    num_ef1_all = 0;
    
    for (idx = 0; idx < k; idx++) {
        /*float ** curr_valuations = generate_valuations(n_rows, n_cols);*/
        float ** ex_valuations = (float **) malloc(n_rows * sizeof(float *));
        for (i = 0; i < n_rows; i++) {
            ex_valuations[i] = (float*) malloc(n_cols * sizeof(float));
        }
    
        for (i = 0; i < n_rows; i++) {
            for (j = 0; j < n_cols; j++) {
                if (i == j) {
                    ex_valuations[i][j] = (float) 1;
                } else {
                    ex_valuations[i][j] = (float) 0;
                }
            }
        }
        
        num_ef1_all += mnw_allocation_ef1_all(ex_valuations, n_rows, n_cols);
        destroy_array(ex_valuations);
    }

    return num_ef1_all/(float)k;
}

/* MAIN
 *
 */

int main() {

    /*clock_t start = clock(), diff;*/
    /*mnw_allocation(valuations, n_rows, n_cols);*/
    /*diff = clock() - start;*/
    /*int msec = diff * 1000 / CLOCKS_PER_SEC;*/
    /*printf("Time Elapsed: %ds %dms\n", msec/1000, msec%1000);*/
    /*srand(time(NULL));*/
    /*float ** arr = generate_valuations(3, 10);*/
    /*mnw_allocation(arr, 3, 10);*/
    /*destroy_array(arr);*/

    /*test proportion of envy-free*/
    /*int i;*/
    /*float p;*/
    /*for (i = 1; i <= 10; i++) {*/
        /*p = percent_ef_test(10, 3, i);*/
        /*printf("Percent Envy Free for %d item(s): %f\n", i, p);*/
    /*}*/
    /*float percent = percent_ef_test(1000, 3, 5);*/
    /*printf("Percent Envy Free = %f\n", percent);*/

    /*float res = percent_ef1_any_test(1, 3, 4);*/

    /*printf("Expected: %f\n", 0.0);*/
    /*printf("Actual: %f\n", res);*/

    return 0;
}



