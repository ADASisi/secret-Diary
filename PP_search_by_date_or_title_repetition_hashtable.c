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
	char** buff;
	int count;
	int capacity;
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
        if(!strcmp(secondValue, curr->secondValue->buff[i])) 
            return i;
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

void printSecondValues(struct hashtable* table, char* sourceForHash){
    if(hashtable_contains_sourceForHash(table, sourceForHash)){
        int bucket_index = hash(sourceForHash) % table->num_of_buckets;
        struct linkedlist* ll = table->buckets[bucket_index];

        for(struct linkedlist_node *curr = ll->head; curr != NULL; curr = curr->next){// worst case O(3)
            if(!strcmp(sourceForHash, curr->sourceForHash)) {
                for(int i = 0; i < curr->secondValue->count; i++){
                    printf("%s\n", curr->secondValue->buff[i]);
                }
            }
        }
    }
    else printf("There is no such value!\n");
}

char* dateToString(int day, int mount, int year){
    char* str;
    char* dateString = calloc(40, sizeof(char));

    sprintf(str,"%d",day);
    strcpy(dateString, str);
    strcat(dateString, ".");

    sprintf(str,"%d",mount);
    strcat(dateString, str);
    strcat(dateString, ".");
    
    sprintf(str,"%d",year);
    strcat(dateString, str);

    return dateString;
}

void hashtable_delete(struct hashtable* table, char* sourceForHash, char* secondValue){
    int bucket_index = hash(sourceForHash) % table->num_of_buckets;
    struct linkedlist* ll = table->buckets[bucket_index];
    int i = -1;

    struct linkedlist_node* curr = linkedlist_contains_sourceForHash(ll, sourceForHash);

    if (curr != NULL) {
        if ((i = node_contains(curr, secondValue)) != -1){
            if(i == 0 && curr->secondValue->count < 2) {
                curr =  ll->head;
                if(equals(sourceForHash, curr->sourceForHash)){
                    ll->head = curr->next;
                }
                while(curr != NULL && curr->next != NULL){
                    if (equals(sourceForHash, curr->next->sourceForHash)) {
                        printf("%s\n", curr->next->sourceForHash);
                        struct linkedlist_node* temp = curr->next;
                        curr->next = curr->next->next;
                        free(temp);

                    }
                    curr = curr->next;
                }
            }
            else{
                strcpy(curr->secondValue->buff[i], curr->secondValue->buff[curr->secondValue->count - 1]);
                curr->secondValue->count--;
                strcpy(curr->thirdValue->buff[i], curr->thirdValue->buff[curr->thirdValue->count - 1]);
                curr->thirdValue->count--;
            }
        }
    }
}

/*
void read_story(struct hashtable* table_for_dates, struct hashtable* table_for_titles){
        int option;
        char filename[HEADING_SUMBOLS + 4];
        char secondValue[STRINGSIZE];
        char sourceForHash[STRINGSIZE];
        getchar();
        while(1){
            printf("CHOOSE TO ENTER DATE OR TITLE OF THE STORY\n");
            printf("1) DATE\n");
            printf("2) TITLE\n");
            printf("Option: ");
            scanf("%d", &option);
            printf("\nENTER THE NAME OF THE ONE YOU WANT TO READ: ");
            fgets(stdin, STRINGSIZE, sourceForHash);

            printSecondValues(table, sourceForHash);

            printf("Enter which one to open: ");
            fgets(stdin, STRINGSIZE, secondValue);

            filename = hashtable_contains(table, name, secondValue);
            if(filename == NULL) {
                printf("There is not such story!\n");
                break;
            }
            
            strcpy(filename, name);
            strcat(filename, ".txt");
            if((read = fopen(filename, "r")) != NULL){
                printf("\n");
                make_uppercase(name);
                printf("    %s\n", name);

                while(fgets(text, NUMBER_OF_SYMBOLS, read)){
                    printf("%s\n", text);
                }
                printf("\n");
                break;
            }
            else{
                printf("ERROR WITH OPENING THE FILE\n");
            }
        }

        fclose(read);
        
        char stay[4];
        printf("\nDO YOU WANT TO KEEP READING?: ");
        scanf("%s", stay);
        if(strcmp(stay,"yes") == 0){
            //print_menu(head);
            read_story(head);
        }
    }
}
*/
int main(){
    struct hashtable* table_for_dates = hashtable_init(5, 3);
    struct hashtable* table_for_titles = hashtable_init(5, 3);


    table_for_dates = hashtable_add(table_for_dates, "23.4.2000", "title", "fileName1");
    table_for_dates = hashtable_add(table_for_dates, "12.12.2019", "title", "fileName2");
    table_for_dates = hashtable_add(table_for_dates, "12.12.2019", "title3", "fileName3");
    table_for_dates = hashtable_add(table_for_dates, "31.12.2023", "title4", "fileName4");
    table_for_dates = hashtable_add(table_for_dates, "22.2.2015", "title5", "fileName5");

    table_for_titles = hashtable_add(table_for_titles, "title", "23.4.2000", "fileName1");
    table_for_titles = hashtable_add(table_for_titles, "title", "12.12.2019", "fileName2");
    table_for_titles = hashtable_add(table_for_titles, "title3", "12.12.2019", "fileName3");
    table_for_titles = hashtable_add(table_for_titles, "title4", "31.12.2023", "fileName4");
    table_for_titles = hashtable_add(table_for_titles, "title5", "22.2.2015", "fileName5");


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

    hashtable_delete(table_for_dates, "23.4.2000", "title");
    hashtable_delete(table_for_titles, "title", "23.4.2000");

    printf("\n");
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

    if((hashtable_contains(table_for_dates, "23.4.2000", "title")) != NULL) printf("Search by date - file name: %s\n", hashtable_contains(table_for_dates, "23.4.2000", "title"));
    else printf("does not contain\n");

    if((hashtable_contains(table_for_titles, "title", "23.4.2000")) != NULL) printf("Search by title - file name: %s\n", hashtable_contains(table_for_titles, "title", "23.4.2000"));
    else printf("does not contain\n");

    char* str;
    str = dateToString(12, 12, 2019);
    printf("date: %s\n", str);
    printSecondValues(table_for_dates, str);

    free(table_for_dates);
    free(table_for_titles);

    return 0;
}
