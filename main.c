/*
 * Ashley Jones and Laura Slade
 * DUE: 9/28/14
 * author name(s), date, and other info here
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "list.h"

void process_data(FILE *input_file) {
    // !! your code should start here.  the input_file parameter
    // is an already-open file.  you can read data from it using
    // the fgets() C library function.  close it with the fclose()
    // built-in function
    struct node *head = malloc(sizeof(struct node));
    head = NULL;
    char raw_data[1024];
    
    //while(strcmp(raw_data,"\n") != 0) {
    while(fgets(raw_data, 1024, input_file) != NULL){
        //pass value into tokenify
        char ** tokenify(char *); //NOTE: need to initialize/declare a function before it appears! (else assumes tokenify to return an int)
        char **str = tokenify(raw_data);
        //convert values returned into an int
        int i = 0;
        if(str[i] == NULL) { 
            free(str);
        }
        while(str[i] != NULL) {
            int value = strtol(str[i], NULL, 10); // atoi(*str);
            //add value to linked list
            list_insert(value, &head); 
            //update counter
            i++;
    
        }
        //get next line from fgets
        //fgets(raw_data, 1024, input_file);

        //free the tokens
        free(str);
    }
    list_print(head);
    //system call stuff

    struct rusage usage;
    struct timeval userinfo;
    struct timeval systeminfo;
    int u = getrusage(RUSAGE_SELF, &usage);
    userinfo = usage.ru_utime;
    systeminfo = usage.ru_stime;
    
    if(u == 0) { //print out
        printf("User time: %ld.%ld\n", userinfo.tv_sec, userinfo.tv_usec);
        printf("System time: %ld.%ld\n", systeminfo.tv_sec, systeminfo.tv_usec);
    }
    else {
        fprintf(stderr,"ERROR ALERT OH NOES!!");
    }
    
    
    list_clear(head); //free everything in the list at the end
}

int is_valid(char *tok) {
    //fist "digit" could be negative sign or 0-9
    if(!isdigit(tok[0])) { //not a digit
        if(tok[0] != '-') { //not a negative sign
            return 0;
        }
	if(!isdigit(tok[1])){
	    return 0;
	}
    }
    int len = strlen(tok);
    if(len == 1){
        return 1;
    }
    for(int i=1; i<len; i++) {
        if(!isdigit(tok[i])) { //not a digit
            return 0;
        }
    }    
    return 1;
}

char** tokenify(char *s) { //was const char *s --> made it char *s)
    // your code here
	char *temp = strdup (s);
	char *temp2 = strdup (s);
	char *token_count = strtok (temp, " \t\n");
	int num_tok = 1;
	while (token_count != NULL){	//get token count
		//printf ("token = %s\n", token);
		token_count = strtok (NULL, " \t\n");
		num_tok ++;
		//printf ("number of tokens = %d\n", num_tok);
		}
	char **token_list = malloc (num_tok *sizeof (char *)); //create pointer array for tokens
	char *token = strtok (temp2, " \t\n");	//get tokens and use for loop to place them in token array
	int i = 0;
	for (; token != NULL; i ++) {
		char *tok = NULL;
                //call is_valid, if == true, want to add to tok_list
                if(is_valid(token)) {
			tok = strdup (token);
                        token_list [i] = tok;
		}
		token = strtok (NULL, " \t\n");	//get new token
		
	}
	token_list [i] = NULL;
	free (temp);	//free up extra arrays used by strdup
	free (temp2);
	return token_list;
}





void usage(char *program) {
    fprintf(stderr, "usage: %s [<datafile>]\n", program);
    exit(1);
}

#ifndef AUTOTEST
int main(int argc, char **argv) {
    FILE *datafile = NULL;

    /* find out how we got invoked and deal with it */
    switch (argc) {
        case 1:
            /* only one program argument (the program name) */ 
            /* just equate stdin with our datafile */
            datafile = stdin;        
            break;

        case 2:
            /* two arguments: program name and input file */
            /* open the file, assign to datafile */
            datafile = fopen(argv[1], "r");
            if (datafile == NULL) {
                printf("Unable to open file %s: %s\n", argv[1], strerror(errno));
                exit(-1);
            }
            break;

        default:
            /* more than two arguments?  throw hands up in resignation */
            usage(argv[0]);
    }

    /* 
     * you should be able to just read from datafile regardless 
     * whether it's stdin or a "real" file.
     */
    process_data(datafile);
    fclose(datafile);
    return 0;
}
#endif
