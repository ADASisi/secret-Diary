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


struct linkedlist {
    struct linkedlist_node* head;
    int count;
};


struct linkedlist_node {
    char* sourceForHash;
    char* secondValue;
    struct linkedlist_node* next;
};

struct hashtable {
    int num_of_buckets;
    struct linkedlist** buckets;
    int max_elements_per_bucket;
};

struct hashtable* hashtable_add(struct hashtable* table, char* sourceForHash, char* secondValue);

int linkedlist_contains(struct linkedlist* ll, char* str) {
    struct linkedlist_node* curr = ll->head;
    while(curr != NULL) {
        if (equals(str, curr->sourceForHash)) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
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
        table->buckets[i]->count = 0;
    }

    return table;
}

struct linkedlist_node* hashtable_contains(struct hashtable* table, char* str){
    int bucket_index = hash(str) % table->num_of_buckets;
    struct linkedlist* ll = table->buckets[bucket_index];

    for(struct linkedlist_node *curr = ll->head; curr != NULL; curr = curr->next){// worst case O(3)
        if(!strcmp(str, curr->sourceForHash)) return curr;
    }

    return NULL;
}

struct hashtable* hashtable_resize(struct hashtable* old_table) {
    struct hashtable* new_table = hashtable_init((old_table->num_of_buckets)*2, old_table->max_elements_per_bucket);

    for(int i = 0; i < old_table->num_of_buckets; i++){
        for(struct linkedlist_node* curr = old_table->buckets[i]->head; curr != NULL; curr = curr->next){
            new_table = hashtable_add(new_table, curr->sourceForHash, curr->secondValue);
        }
    }

    return new_table;
}

void linkedlist_add(struct linkedlist* ll, char *sourceForHash, char* secondValue){
    if(ll->head == NULL){
        ll->head = calloc(1, sizeof(struct linkedlist_node));
        ll->head->next = NULL;
        ll->head->sourceForHash = strdup(sourceForHash);
        ll->head->secondValue = strdup(secondValue);
        ll->count++;
    }
    else {
        struct linkedlist_node* curr = ll->head;
        for(; curr->next != NULL; curr = curr->next){}
        curr->next = calloc(1, sizeof(struct linkedlist_node));
        curr->next->next = NULL;
        curr->next->sourceForHash = strdup(sourceForHash);
        curr->next->secondValue = strdup(secondValue);
        ll->count++;
   }
}

struct hashtable* hashtable_add(struct hashtable* table, char* sourceForHash, char* secondValue) {
    int bucket_index = hash(sourceForHash) % table->num_of_buckets;
    struct linkedlist* ll = table->buckets[bucket_index];

    if (!linkedlist_contains(ll, sourceForHash)) {
        linkedlist_add(ll, sourceForHash, secondValue);
        
        if (ll->count >= table->max_elements_per_bucket) {
            table = hashtable_resize(table);
        }
    }

    return table;
}

int main(){
    struct hashtable* table_for_dates = hashtable_init(5, 3);
    struct hashtable* table_for_titles = hashtable_init(5, 3);


    table_for_dates = hashtable_add(table_for_dates, "23/04/2000", "title1");
    table_for_dates = hashtable_add(table_for_dates, "12/12/2019", "title2");
    table_for_dates = hashtable_add(table_for_dates, "24/12/2002", "title3");
    table_for_dates = hashtable_add(table_for_dates, "31/12/2023", "title4");
    table_for_dates = hashtable_add(table_for_dates, "22/02/2015", "title5");

    table_for_titles = hashtable_add(table_for_titles, "title1", "23/04/2000");
    table_for_titles = hashtable_add(table_for_titles, "title2", "12/12/2019");
    table_for_titles = hashtable_add(table_for_titles, "title3", "24/12/2002");
    table_for_titles = hashtable_add(table_for_titles, "title4", "31/12/2023");
    table_for_titles = hashtable_add(table_for_titles, "title5", "22/02/2015");


    for(int i = 0; i < table_for_dates->num_of_buckets; i++){
        for(struct linkedlist_node* curr = table_for_dates->buckets[i]->head; curr != NULL; curr = curr->next){
            printf("%d: %s, %s\n", i, curr->sourceForHash, curr->secondValue);
        }
    }

    struct linkedlist_node *curr = calloc(1, sizeof(struct linkedlist_node));

    if((curr = hashtable_contains(table_for_dates, "22/02/2015")) != NULL) printf("Search by date: %s -> %s\n", curr->sourceForHash, curr->secondValue);
    else printf("does not contain\n");

    if((curr = hashtable_contains(table_for_titles, "title5")) != NULL) printf("Search by title: %s -> %s\n", curr->sourceForHash, curr->secondValue);
    else printf("does not contain\n");


    return 0;
}
