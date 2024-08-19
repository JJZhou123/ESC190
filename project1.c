#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
Project 1 -- Autocomplete
=========================

In this project, you will write a fast implementation of the autocomplete functionality.

You will work with files that contain a large number of terms, along with the importance weight associated with those terms.

The autocomplete task is to quickly retrieve the top terms that match a query string. For example, if the query string is "Eng", you matches might be "English",
"Engineering", and "EngSci".

To accomplish the task, you will:

* Read in the entire file of terms, and sort the terms in lexicographic ordering. This sorted list will be reused for multiple queries.

* Use binary search to find the location of the first term in lexicographic ordering that matches the query string, as well as the last term in lexicographic oredering that matches the query string.

* Extract the terms that match the query string, and sort them by weight.

* Extract the top matching terms by weight

Throughtout, you will use the following struct:
struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
};


Part 1
======
Write a function with the signature 

void read_in_terms(struct term **terms, int *pnterms, char *filename);

The function takes in a pointer to a pointer to struct term, a pointer to an int, and the name of a file that is formatted like cities.txt.

The function allocates memory for all the terms in the file and stores a pointer to the block in *terms. The function stores the number of terms in *pnterms. The function reads in all the terms from filename, and places them in the block pointed to by *terms.

The terms should be sorted in lexicographic order.
*/

struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
};

int comparefn(const void * p, const void * q){
    struct term *term_A = (struct term *)p;
    struct term *term_B = (struct term *)q;
    int result = strcmp(term_A->term,term_B->term);
    return result;
}
int comparefn_w(const void * p, const void * q){
    struct term *term_A = (struct term *)p;
    struct term *term_B = (struct term *)q;
    int result = term_B->weight - term_A->weight;
    return result;


}
void read_in_terms(struct term **terms, int *pnterms, char *filename){
    FILE *fp = fopen(filename,"r");
    char line[200];
    char term[200];
    int termlen;
    double realweight;
    fgets(line, sizeof(line), fp);
    int number = atoi(line);
    *pnterms = number;
    //printf("Number of terms is %d\n",number);
    *terms  = (struct term*) malloc(sizeof(struct term)*number);
    for(int i = 0; i < number; i++){ 
        fgets(line, sizeof(line), fp);
        realweight = atoi(line);
        int counter = 0;
        for (int a=0;a<strlen(line);a++){
            if (((line[a]>='a'&&line[a]<='z') || (line[a]>='A'&&line[a]<='Z' || line[a]=='\0'))){
                break;
            }
            else{
                counter = counter+1;
            }
        }
        termlen = strlen(line) - counter -1;
        printf("length is %d\n",termlen);
        for (int b=0;b<termlen;b++){
            term[b] = line[counter+b];
        }
        term[termlen] = '\0';
        strcpy((*terms)[i].term,term);
        (*terms)[i].weight = realweight;//////////
        //printf("%s\n",(*terms)[i].term);
    }
    

    //char temp[200];  //Can't be deleted!!!!!!!!!!!!!!!!!!!!!
    //double tempweight;
    qsort(*terms,number,sizeof(struct term),comparefn);
/*
    //Printing
    for (int g=0;g<number;g++){
        printf("String: %s\n",(*terms)[g].term);
        printf("Weight: %f\n",(*terms)[g].weight);
    }
*/
}


/*

Part 2(a)
=========
Write a function with the signature
int lowest_match(struct term *terms, int nterms, char *substr);

The function returns the index in terms of the first term in lexicographic ordering that matches the string substr.

This function must run in O(log(nterms)) time, where nterms is the number of terms in terms.

You can assume that terms is sorted in ascending lexicographic ordering.
*/

int lowest_match(struct term *terms, int nterms, char *substr){
    int low = 0;
    int high = nterms - 1;
    int mid; 
    //char no_use[200]; //// Can't be deleted
    char wordcmp[200];
    printf("Number of terms is %d\n",nterms);
    while (high >= low){
        mid = (low + high)/2;
        printf("String is between %s and %s\n",terms[low].term,terms[high].term);
        printf("Mid term is %s\n",terms[mid].term);
        strncpy(wordcmp,terms[mid].term,strlen(substr));
        wordcmp[strlen(substr)] = '\0';
        printf("Wordcmp is %s\n",wordcmp);
        if (high == low){
            if (strcmp(wordcmp,substr) == 0){
                return high;
            }
            else{
                return -1;
            }
        }
        if (strlen(substr)>strlen(terms[mid].term)){
            if (strcmp(terms[mid].term,substr) > 0){
                high = mid-1;
            }
            if (strcmp(terms[mid].term,substr) < 0){
                low = mid+1;
            }
            continue;
        }
        if (strcmp(wordcmp,substr)==0){
            printf("YESSS\n");
            high = mid;
            continue;
        }
        if (strcmp(wordcmp,substr) > 0){
            high = mid-1;
            continue;
        }
        if (strcmp(wordcmp,substr) < 0){
            low = mid+1;
            continue;
        }
    }
    return -1;
}


int highest_match(struct term *terms, int nterms, char *substr){
    int low = 0;
    int high = nterms - 1;
    int mid; 
    char temp[200];
    printf("Number of terms is %d\n",nterms);
    while (high >= low){
        //mid = 0.5 + (low + high)/2;
        mid = (low + high+1)/2;
        printf("String is between %s and %s\n",terms[low].term,terms[high].term);
        printf("Mid term is %s\n",terms[mid].term);
        strncpy(temp,terms[mid].term,strlen(substr));
        temp[strlen(substr)] = '\0';
        //strncpy(temp,wordcmpp,strlen(substr));
        printf("Wordcmp is %s\n",temp);
        if (high == low){
            if (strcmp(temp,substr) == 0){
                return high;
            }
            else{
                return -1;
            }
        }
        if (strlen(substr)>strlen(terms[mid].term)){
            if (strcmp(terms[mid].term,substr) > 0){
                high = mid-1;
            }
            if (strcmp(terms[mid].term,substr) < 0){
                low = mid+1;
            }
            continue;
        }
        if (strcmp(temp,substr)==0){
            printf("YESSS\n");
            low = mid;
            continue;
        }
        if (strcmp(temp,substr) > 0){
            high = mid-1;
            continue;
        }
        if (strcmp(temp,substr) < 0){
            low = mid+1;
            continue;
        }

    }
    return -1;
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){
    // Takes in terms, find the answer, write them into *answer
    int begin = lowest_match(terms,nterms,substr);
    printf("Low is %d\n",begin);
    int end = highest_match(terms,nterms,substr);
    printf("high is %d\n",end);
    *n_answer = end - begin +1;
    if (begin == -1){
        *n_answer = 0;
    }
    *answer  = (struct term*) malloc(sizeof(struct term)*(*n_answer)); // Might be an error
    for(int i = 0; i < *n_answer; i++){
        strcpy((*answer)[i].term,terms[i+begin].term);
        (*answer)[i].weight = terms[i+begin].weight;
    }
/*
    for (int j = 0;j<*n_answer;j++){
        printf("String is %s\n Weight is %f\n",(*answer)[j].term,(*answer)[j].weight);
    }
*/
    for (int k = 0;k<*n_answer;k++){
        printf("String is %s\nWeight is %f\n",(*answer)[k].term,(*answer)[k].weight);
    }
    qsort(*answer,*n_answer,sizeof(struct term),comparefn_w);
    printf("----------\n");
/*
    for (int k = 0;k<*n_answer;k++){
        printf("String is %s\n Weight is %f\n",(*answer)[k].term,(*answer)[k].weight);
    }
*/

    for (int k = 0;k<*n_answer;k++){
        printf("String is %s\nWeight is %f\n",(*answer)[k].term,(*answer)[k].weight);
    }

    printf("Number of answers: %d\nAnswer: %s\n",*n_answer,(*answer)[0].term);
}
/*
int main(void){
    struct term *terms;
    struct term *answer;
    int nterms;
    int n_answer;
    read_in_terms(&terms, &nterms, "file.txt");

    autocomplete(&answer, &n_answer,terms,nterms,"A");
    //int a = lowest_match(terms,nterms,"ABCDEFGHI");
    //int b = highest_match(terms, nterms, "AEFD");
    //printf("%d\n",a);
    //printf("%d\n",b);
    return 0;
}
*/




/*
Part 2(b)
=========
Write a function with the signature
int highest_match(struct term *terms, int nterms, char *substr);

The function returns the index in terms of the last term in lexicographic order that matches the string substr.

This function must run in O(log(nterms)) time, where nterms is the number of terms in terms.

You can assume that terms is sorted in ascending lexicographic ordering.

Part 3
======
Write a function with the signature 

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr);

The function takes terms (assume it is sorted lexicographically), the number of terms nterms, and the query string substr, and places the answers in answer, with *n_answer being the number of answers. The answer should be sorted by weight.


Reading in files
================
The following code can be used to read in N lines of a text file line-by-line.

char line[200];
FILE *fp = fopen(filename, "r");
for(int i = 0; i < N; i++){
    fgets(line, sizeof(line), fp); //read in at most sizeof(line) characters
                                   //(including '\0') into line.
}

Sorting with qsort
==================
See here: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm


Compilation instructions
========================
You should use the provided autocomplete.h. Your code should compile without errors with

gcc -Wall autocomplete.c main.c

with autocomplete.h placed in the same folder as the C files. You should modify your main.c to test your work, but we will be compiling your autocomplete.c with our version of autocomplete.h (which is the same as yourse) and our own main.c.

Submission instructions
=======================
TBA



Credit: the assignment was designed by Kevin Wayne, and adapted to C by Michael Guerzhoy
*/
