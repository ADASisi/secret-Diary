#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int hash(char* str) {
    int h = 0;
    for(int i = 1; i < strlen(str); i++) {
        h += i * str[i] * str[i];
    }
    return h;
}

int equals(char* a, char* b) {
    return strcmp(a, b) == 0;
}

#define STEP 2
#define STRINGSIZE 40
#define CAPACITY 10

struct dynamic_array_t {
	char** buff; // 8
	int count; // 4
	int capacity; // 4
};

struct dynamic_array_t* dynamic_array_init(int capacity) {
    struct dynamic_array_t *arr = calloc(capacity, sizeof(struct dynamic_array_t));
	arr->buff = calloc(capacity, sizeof(char*));
    for(int i = 0; i < capacity; i++){
        arr->buff[i] = calloc(STRINGSIZE, sizeof(char));
    }
	arr->capacity = capacity;
	arr->count = 0;

	return arr;
}

void extendArray(struct dynamic_array_t *arr){
    arr->capacity = arr->capacity * STEP;
    arr->buff = realloc(arr->buff, arr->capacity * sizeof(char*));
    for(int i = arr->count; i < arr->capacity; i++){
        arr->buff[i] = calloc(STRINGSIZE, sizeof(char));
    }
}


void dynamic_array_add(struct dynamic_array_t *arr, char *value){
    if(arr->count >= arr->capacity){
        //fprintf(stderr, "tyk\n");
        extendArray(arr);
    }
    strcpy(arr->buff[arr->count],  value);
    arr->count++;
};

struct linkedlist {
    struct linkedlist_node* head;
    int nodesCount;
};

struct linkedlist_node {
    char* sourceForHash;
    struct dynamic_array_t* secondValue;
    struct dynamic_array_t* thirdValue;
    struct linkedlist_node* next;
};

struct hashtable {
    int num_of_buckets;
    struct linkedlist** buckets;
    int max_elements_per_bucket;
};

struct hashtable* hashtable_add(struct hashtable* table, char* sourceForHash, char* secondValue, char* thirdValue);

struct linkedlist_node* linkedlist_contains_sourceForHash(struct linkedlist* ll, char* sourceForHash) {
    struct linkedlist_node* curr = ll->head;
    while(curr != NULL) {
        if (equals(sourceForHash, curr->sourceForHash)) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

struct hashtable* hashtable_init(int num_of_buckets,int max_elements_per_bucket) {
    struct hashtable *table = calloc(1, sizeof(struct hashtable));
    table->num_of_buckets = num_of_buckets;
    table->max_elements_per_bucket = max_elements_per_bucket;

    table->buckets = calloc(num_of_buckets, sizeof(struct linkedlist));
    for(int i = 0; i < num_of_buckets; i++){
        table->buckets[i] = calloc(1, sizeof(struct linkedlist));
    }
    for(int i = 0; i < num_of_buckets; i++){
        table->buckets[i]->head = NULL;
        table->buckets[i]->nodesCount = 0;
    }

    return table;
}

struct linkedlist_node* hashtable_contains_sourceForHash(struct hashtable* table, char* sourceForHash){
    int bucket_index = hash(sourceForHash) % table->num_of_buckets;
    struct linkedlist* ll = table->buckets[bucket_index];

    for(struct linkedlist_node *curr = ll->head; curr != NULL; curr = curr->next){// worst case O(3)
        if(!strcmp(sourceForHash, curr->sourceForHash)) return curr;
    }

    return NULL;
}

int node_contains(struct linkedlist_node *curr, char* secondValue){
    for(int i = 0; i < curr->secondValue->count; i++){
        if(!strcmp(secondValue, curr->secondValue->buff[i])) return i;
    }
    return -1;
}

char* hashtable_contains(struct hashtable* table, char* sourceForHash, char* secondValue){
    int bucket_index = hash(sourceForHash) % table->num_of_buckets;
    struct linkedlist* ll = table->buckets[bucket_index];

    for(struct linkedlist_node *curr = ll->head; curr != NULL; curr = curr->next){// worst case O(3)
        if(!strcmp(sourceForHash, curr->sourceForHash)) {
            for(int i = 0; i < curr->secondValue->count; i++){
                if(!strcmp(secondValue, curr->secondValue->buff[i])) return curr->thirdValue->buff[i];
            }
        }
    }

    return NULL;
}

struct hashtable* hashtable_resize(struct hashtable* old_table) {
    struct hashtable* new_table = hashtable_init((old_table->num_of_buckets)*2, old_table->max_elements_per_bucket);

    for(int i = 0; i < old_table->num_of_buckets; i++){
        for(struct linkedlist_node* curr = old_table->buckets[i]->head; curr != NULL; curr = curr->next){
            for(int j = 0; j < curr->secondValue->count; j++){
                new_table = hashtable_add(new_table, curr->sourceForHash, curr->secondValue->buff[j], curr->thirdValue->buff[j]);
            }
        }
    }

    return new_table;
}

void linkedlist_add_node(struct linkedlist* ll, char *sourceForHash, char* secondValue, char* thirdValue){
    if(ll->head == NULL){
        ll->head = calloc(1, sizeof(struct linkedlist_node));
        ll->head->next = NULL;
        ll->head->secondValue = dynamic_array_init(CAPACITY);
        ll->head->thirdValue = dynamic_array_init(CAPACITY);
        ll->head->sourceForHash = strdup(sourceForHash);
        dynamic_array_add(ll->head->secondValue, secondValue);
        dynamic_array_add(ll->head->thirdValue, thirdValue);
        ll->nodesCount++;
    }
    else {
        struct linkedlist_node* curr = ll->head;
        for(; curr->next != NULL; curr = curr->next){}
        curr->next = calloc(1, sizeof(struct linkedlist_node));
        curr->next->next = NULL;
        curr->next->sourceForHash = strdup(sourceForHash);
        curr->next->secondValue = dynamic_array_init(CAPACITY);
        curr->next->thirdValue = dynamic_array_init(CAPACITY);
        dynamic_array_add(curr->next->secondValue, secondValue);
        dynamic_array_add(curr->next->thirdValue, thirdValue);
        ll->nodesCount++;
   }
}

struct hashtable* hashtable_add(struct hashtable* table, char* sourceForHash, char* secondValue, char* thirdValue) {
    int bucket_index = hash(sourceForHash) % table->num_of_buckets;
    struct linkedlist* ll = table->buckets[bucket_index];

    if (linkedlist_contains_sourceForHash(ll, sourceForHash) == NULL) {
        linkedlist_add_node(ll, sourceForHash, secondValue, thirdValue);
        
        if (ll->nodesCount >= table->max_elements_per_bucket) {
            table = hashtable_resize(table);
        }
    }
    else if (node_contains(linkedlist_contains_sourceForHash(ll, sourceForHash), secondValue) == -1){
        struct linkedlist_node* curr = linkedlist_contains_sourceForHash(ll, sourceForHash);
        dynamic_array_add(curr->secondValue, secondValue);
        dynamic_array_add(curr->thirdValue, thirdValue);
    }

    return table;
}

void free_hashtable(struct hashtable* table){
    for(int i = 0; i < table->num_of_buckets; i++){
        for(struct linkedlist_node* curr = table->buckets[i]->head; curr != NULL; curr = curr->next){
            for(int j = curr->secondValue->capacity; j >= 0; j--){
                free(curr->secondValue->buff[j]);
                free(curr->thirdValue->buff[j]);
            }
            free(curr->secondValue->buff);
            free(curr->thirdValue->buff);
            free(curr->secondValue);
            free(curr->thirdValue);
        }
    }

    for(int i = 0; i < table->num_of_buckets; i++){
        for(struct linkedlist_node* curr = table->buckets[i]->head; curr != NULL; curr = curr->next){
            if(curr->next != NULL){
                struct linkedlist_node* curr = table->buckets[i]->head;
                while(1){
                    if(curr->next->next == NULL) {
                        free(curr->next);
                        break;
                    }
                    curr = curr->next;
                }
            }
            else{
                free(curr);
            }
        }
    }

    for(int i = table->num_of_buckets; i >= 0; i--){
        free(table->buckets[i]);
    }

    free(table->buckets);
    free(table);
}

int main(){
    struct hashtable* table_for_dates = hashtable_init(5, 3);
    struct hashtable* table_for_titles = hashtable_init(5, 3);


    table_for_dates = hashtable_add(table_for_dates, "23/04/2000", "title", "fileName1");
    table_for_dates = hashtable_add(table_for_dates, "12/12/2019", "title", "fileName2");
    table_for_dates = hashtable_add(table_for_dates, "12/12/2019", "title3", "fileName3");
    table_for_dates = hashtable_add(table_for_dates, "31/12/2023", "title4", "fileName4");
    table_for_dates = hashtable_add(table_for_dates, "22/02/2015", "title5", "fileName5");

    table_for_titles = hashtable_add(table_for_titles, "title", "23/04/2000", "fileName1");
    table_for_titles = hashtable_add(table_for_titles, "title", "12/12/2019", "fileName2");
    table_for_titles = hashtable_add(table_for_titles, "title3", "12/12/2019", "fileName3");
    table_for_titles = hashtable_add(table_for_titles, "title4", "31/12/2023", "fileName4");
    table_for_titles = hashtable_add(table_for_titles, "title5", "22/02/2015", "fileName5");


    for(int i = 0; i < table_for_dates->num_of_buckets; i++){
        for(struct linkedlist_node* curr = table_for_dates->buckets[i]->head; curr != NULL; curr = curr->next){
            for(int j = 0; j < curr->secondValue->count; j++){
                printf("%d: %s, %s, %s\n", i, curr->sourceForHash, curr->secondValue->buff[j], curr->thirdValue->buff[j]);
            }
        }
    }
    printf("\n");
    for(int i = 0; i < table_for_titles->num_of_buckets; i++){
        for(struct linkedlist_node* curr = table_for_titles->buckets[i]->head; curr != NULL; curr = curr->next){
            for(int j = 0; j < curr->secondValue->count; j++){
                printf("%d: %s, %s, %s\n", i, curr->sourceForHash, curr->secondValue->buff[j], curr->thirdValue->buff[j]);
            }
        }
    }

    struct linkedlist_node *curr = calloc(1, sizeof(struct linkedlist_node));

    if((hashtable_contains(table_for_dates, "22/02/2015", "title5")) != NULL) printf("Search by date - file name: %s\n", hashtable_contains(table_for_dates, "22/02/2015", "title5"));
    else printf("does not contain\n");

    if((hashtable_contains(table_for_titles, "df", "22/02/2015")) != NULL) printf("Search by title - file name: %s\n", hashtable_contains(table_for_titles, "title5", "22/02/2015"));
    else printf("does not contain\n");

    free(table_for_dates);
    free(table_for_titles);

    return 0;
}
