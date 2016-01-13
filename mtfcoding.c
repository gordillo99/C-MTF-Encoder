#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtfcoding.h"
/*for better readability          -------------->          :set tabstop=4                           */


/*prototypes of decode/encode*/
int decode(FILE * input, FILE *output);
int encode(FILE * input, FILE *output);

/*EXTRA FUNCTIONALITY: MULTIPLE FILE ENCODING*/
int encode_multiple(char ** arr, int number_of_files)
{
	int counter = 0; /*local counter for pushing back*/

	for (; counter < number_of_files; counter++) /*loop that pushes back starting from the back until it reaches the desired position in the array*/
	{
		char * txt_name = *(arr+counter);
		FILE * txt = fopen(txt_name, "r"); 
		if (txt == NULL)
		{
			printf("Couldn't open txt file.\n");
			exit(-1);
		}

		char mtf_name[100];
		strcpy(mtf_name, txt_name); /*copies the name of the text file to variable that will hold name of mtf file*/
        char * beginning_of_txt = strstr(mtf_name,".txt"); /*leads the pointer beginning_of_text to where the chars .txt are located*/

        if(beginning_of_txt == NULL) /*double checks that indicated file has .txt extension*/
        {
                printf("Indicated file is not a .txt file.\n");
                exit(-1);
        }

        /*replaces txt by mtf in the name of the file*/
        *(beginning_of_txt + 1) = 'm';
        *(beginning_of_txt + 2) = 't';
        *(beginning_of_txt + 3) = 'f';
		
		FILE * mtf = fopen(mtf_name, "w"); /*creates/opens mtf file*/

		encode(txt, mtf); /*encodes the file*/
		/*closes both files*/
		fclose(txt);
		fclose(mtf);
	}
}

/*EXTRA FUNCTIONALITY: MULTIPLE FILE DECODING*/
int decode_multiple(char ** arr, int number_of_files)
{
    int counter = 0; /*local counter for pushing back*/

    for (; counter < number_of_files; counter++) /*loop that pushes back starting from the back until it reaches the desired position in the array*/
    {
        char * mtf_name = *(arr+counter);
        FILE * mtf = fopen(mtf_name, "r");
        if (mtf == NULL)
        {
            printf("Couldn't open mtf file.\n");
            exit(-1);
        }

        char txt_name[100];
        strcpy(txt_name, mtf_name); /*copies the name of the text file to variable that will hold name of txt file*/
        char * beginning_of_mtf = strstr(txt_name,".mtf"); /*leads the pointer beginning_of_mtf to where the chars .mtf are located*/

        if(beginning_of_mtf == NULL) /*double checks that indicated file has .mtf extension*/
        {
                printf("Indicated file is not a .mtf file.\n");
                exit(-1);
        }

        /*replaces mtf by txt in the name of the file*/
        *(beginning_of_mtf + 1) = 't';
        *(beginning_of_mtf + 2) = 'x';
        *(beginning_of_mtf + 3) = 't';

        FILE * txt = fopen(txt_name, "w"); /*creates/opens txt file*/

        decode(mtf, txt); /*decodes the file*/
        /*closes both files*/
        fclose(txt);
        fclose(mtf);
    }
}

/*EXTRA FUNCTIONALITY: RENAMING OF ENCODING OUTPUT FILE*/
/*the encoded file can renamed by the user*/
int encode_with_rename(char * txt_name, char * mtf_rename)
{
        FILE * txt = fopen(txt_name, "r");
        if (txt == NULL)
        {
            printf("Couldn't open txt file.\n");
            exit(-1);
        }

        char mtf_name[100] = "";
        strcpy(mtf_name, mtf_rename); /*adds mtf extension*/
		strcpy(mtf_name, ".mtf");
        FILE * mtf = fopen(mtf_name, "w"); /*creates/opens mtf file*/

        encode(txt, mtf); /*encodes the file*/
        /*closes both files*/
        fclose(txt);
        fclose(mtf);
}

/*EXTRA FUNCTIONALITY: RENAMING OF DECODING OUTPUT FILE*/
int decode_with_rename(char * mtf_name, char * txt_rename)
{
        FILE * mtf = fopen(mtf_name, "r");
        if (mtf == NULL)
        {
            printf("Couldn't open mtf file.\n");
            exit(-1);
        }

        char txt_name[100] = "";
        strcpy(txt_name, txt_rename); /*adds txt extension*/
		strcpy(txt_name, ".txt");
        FILE * txt = fopen(txt_name, "w"); /*creates/opens txt file*/

        decode(mtf, txt); /*decodes the file*/
        /*closes both files*/
        fclose(txt);
        fclose(mtf);
}

/*ACTUAL ASSIGNMENT RELATED CODE*/
int encode(FILE *input, FILE *output) /*function that calls the main encoding method. Receives the input/output streams from file given (mtfencode)*/
{
	if(start_encoding(input, output) == 0) /*controls the value returned*/
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


int decode(FILE *input, FILE *output)   /*function that calls the main decoding method. Receives the input/output streams from file given (mtfdecode)*/
{
	if(start_decoding(output, input) == 0) /*controls the value returned*/
	{
		return 0;
	}
	else
	{
	    return -1;
	}
}

/*definition of the Nameval struct*/
typedef struct Nameval Nameval;
struct Nameval 
{
 	char *name; /*pointer to chars*/
	int value; /*int value*/
	Nameval *next; /* in list */
};

void *emalloc(int n) /*definition of emalloc for when the malloc fails (auto-checking)*/
{
 	void *p;
 	p = malloc(n); /*allocates memory*/
 	if (p == NULL)  /* checks the allocation was successful*/
	{
		/*indicates error and terminates program*/
	 	fprintf(stderr, "malloc of %u bytes failed", n);
 		exit(1);
 	}
 	return p; /*returns pointer to allocated memory*/
}

Nameval *newitem (char *name, int value) /*function to create a new node for the nameval list*/
{
 	Nameval *newp;
 	newp = (Nameval *) emalloc(sizeof(Nameval)); /*new nameval is crearted with dynamic allocation*/
	/*sets needed values for the variables in the node*/
 	newp->name = name;
	newp->value = value;
 	newp->next = NULL;
	/*returns address to new node*/
 	return newp;
}

/*function that adds a node to the front of the list*/
Nameval *addfront(Nameval *listp, Nameval *newp)
{
	/*puts node at front*/
 	newp->next = listp;
 	return newp; /*returns address of new list*/
}

Nameval *addend(Nameval *listp, Nameval *newp) /*adds node to the end of the list*/
{
	Nameval *p;
	/*handles case of emtpy list*/
 	if (listp == NULL) 
	{
 		return newp;
 	}
	/* goes to the end of the list to add the node*/
 	for (p = listp; p->next != NULL; p = p->next);
 	p->next = newp;
	/*returns addres to modified list*/
 	return listp;
}

/*function that takes care of deallocating all the memory allocated dynamically*/ 
void freeall(Nameval *listp)
{
 	Nameval *next;
	/*traverses list dealocating memory*/
 	for ( ; listp != NULL; listp = next ) {
 		next = listp->next;
		/*dealocates the dynamic string first*/
 		free(listp->name);
 		free(listp); /*dealocates the node afterwards*/
 	}
}

/*function that deletes a node if it matches the string passed as an argument*/ 
Nameval *delitem (Nameval *listp, char *name, int * position)
{
	/*variables for adjusting list*/
	*position = 0;
 	Nameval *curr, *prev;
 	prev = NULL;
	
	/*loop for traversing the list*/
 	for (curr = listp; curr != NULL; curr = curr-> next) 
	{
 		if (strcmp(name, curr->name) == 0)  /*if the name matches the parameter*/
		{
 			if (prev == NULL) /*if it is the first element*/
			{
 				listp = curr->next;
 			}
			else  /*if it is not the first element*/
			{
 				prev->next = curr->next;
 			}
			/*dealocates the dynamic string (if it was allocated)*/
			if(curr->name != NULL)
			{
				free(curr->name);
			}
			/*frees node*/
 			free(curr);
			/*returns address of modified list*/
 			return listp;
 		}
		/*continues traversing the list*/
 		prev = curr;
		*position = *position + 1;
 	}
 	/* Ungraceful error handling, but gets the point across. */
 	fprintf(stderr, "delitem: %s not in list", name);
 	exit(1);
}

/*deletes a specific item in the list based on its position on the list*/
Nameval *delitem_by_position (Nameval *listp, char  name[], int * value, int position)
{
	/*sets variables for traversal*/
    Nameval *curr, *prev;
    prev = NULL;
	int counter = 0;
	/*traverses the list to reach the required position to remove*/
    for (curr = listp; counter < position; curr = curr-> next, counter++)
	{
		if(counter < position) /*sets a pointer to the element before the one that will be deallocated*/
		{
			prev = curr; /*updates the pointer*/
		}
	}
	/*grabs the name and the value from the node that will be deleted*/
  	strcpy(name, curr->name);
	*value = curr->value;
 	/*if the list is empty*/
	if (prev == NULL)
	{
		listp = curr->next;
	}
	else /*if the list is not empty*/
	{
		prev->next = curr->next;
	}
	/*deallocates the dynamic string*/
	if(curr->name != NULL)
	{
		free(curr->name);
	}
	/*deallocates the node*/
	free(curr);
	return listp; /*returnts address of modified list*/
}

int lookup_name(Nameval *listp, char *name) /*checks to see if a specific string exists in the list*/
{
	Nameval * tmp = listp; /*tmp pointer for traversal*/
 	for ( ; tmp != NULL; tmp = tmp->next)  /*loop for list traversal*/
	{
 		if (strcmp(name, tmp->name) == 0) /*if the string matches the target string*/
		{
 			return 1; /*indicate the presence of the string*/
 		}
 	}
 	return 0; /*no string was found*/
}

int lookup_int(Nameval *listp, int value) /*checks to see if an int exists in the list*/
{
	Nameval * tmp = listp; /* tmp pointer for list traversal*/ 
	for(; tmp != NULL; tmp = tmp->next) /*loop for list traversal*/
	{
		if(tmp->value == value) /*if int matches the target int*/
		{
			return 0; /*indicate that the int exists in the list*/
		}
	}
	return 1; /*indicate that the int does not exist in the list*/
}

Nameval * get_at(Nameval * list, int position) /*returns the address of the node a specific position*/
{
	Nameval *head = list; /*pointer for traversal*/
	int counter; /*counter (obviously)*/

	for(counter = 0; counter < position; head = head->next, counter++); /*loop to reach desired element*/
	return head;  /*returns the address of desired element*/
}

void print_stuff(Nameval * head) /*prints out the list (used for debugging)*/
{
	Nameval * listp = head; /*tmp pointer for traversal*/
    for(; listp != NULL; listp = listp->next)  /*traverses the list*/ 
    {
        printf("%s\n", listp->name); /*prints out the strings in the list*/
    }  
}

char * string_duplicator(char *input) 
{ /*allocates memory for the strings in the list nodes*/
    char *copy;
    copy = (char *) malloc(sizeof(char) * strlen(input) + 1); /*allocates the memory*/

    if (copy == NULL) /*checks to see that memory was successfully allocated*/
    {
        fprintf(stderr, "error instring_duplicator");
        exit(1);
    }

    strncpy(copy, input, strlen(input)+1); /*copies the string to allocated memory*/
    return copy;	/*returns the copy*/
}


Nameval * move_to_front_and_return_previous_position(Nameval * listp, char * value_to_find, int * position) /* moves a specific node to the front of the list*/
{
	listp = delitem(listp, value_to_find, position); /*removes node at the specified location*/
	/*creates node with the same string as the one deleted (this is used for encoding so the int value is not necessary)*/
	Nameval * new = newitem(string_duplicator(value_to_find), 0); 
	/*moves new node to the front of the list*/
	listp = addfront(listp, new);
	return listp; /*returns address of modified list*/
}

Nameval * move_node_at_p_to_front(Nameval * list, int position) /*moves node in certain position to the front based on position*/
{	/*variables to save info of the node that will move*/
	char prev_name[400]; 
	int prev_value; 
	/*deletes the item based on position*/
	list = delitem_by_position (list, prev_name, &prev_value, position);
	Nameval * new = newitem(string_duplicator(prev_name), prev_value); /*creates new node with info of old node*/
	list = addfront(list, new); /*puts the node in the front of the list*/
    return list; /*returns address to the modified list*/
}

FILE * write_magic_numbers(FILE * mtf_file) /*writes the magic number sequence to the files*/
{
	fputc(0xfa, mtf_file);
	fputc(0xce, mtf_file);
	fputc(0xfa, mtf_file);
	fputc(0xdf, mtf_file);
	return mtf_file; 
}

void write_char_value(int number, FILE* mtf_file) /*writes the appropriate value to the file based on the agreed coding*/
{
	if (number <= 120 && number >= 0) /*lower number codes*/
	{
		 putc((char)number + 128, mtf_file);
	}
	else if (number >= 121 && number <= 375) /*medium number codes (uses two bytes)*/
	{
		putc((char) 249, mtf_file);
		putc((char) (number - 121), mtf_file);
	}
	else if(number >= 376 && number <= 65912) /*high number codes (uses three bytes)*/
	{
		putc((char) 250, mtf_file);

		double num1 = number - 376.0;
		double denom1 = 256.0; 		
		int division = (int) (num1/denom1);
		putc((char) division, mtf_file);
		putc((char) ((number - 376) % 256), mtf_file);
	}
}

void write_char_to_mtf(char thing_to_write, FILE* mtf_file) /*writes the char to the mtf file*/
{
	fputc(thing_to_write, mtf_file); /*puts the char*/
}

void write_char_ptr_to_mtf(char * word_to_write, FILE*mtf_file) /*writes a string to the mtf file*/
{
	if (strlen(word_to_write) > 0) /*to avoid errors...*/
	{
		fputs(word_to_write, mtf_file); /*puts the string*/
	}
}


int start_encoding (FILE * txt_file, FILE *mtf_file)
{
	Nameval* word_order = NULL; /*variable that points to the list of different words read from the file*/
	char * line = NULL; /*char pointer used to hold one line of input from the text file */
	int total_number_of_words = 0; /*counter for the number of words in the 2D array*/
	ssize_t read = 0; /*integer that counts line length of line retrieved from file */
	size_t len = 0;

	if (txt_file == NULL) /*error checking for the txt file*/
	{
		printf("Error in reading text file\n");
		fclose(txt_file);
		fclose(mtf_file);
		exit(1);
	}

    if(mtf_file == NULL) /*if output mtf file cannot be read, exit program*/
    {
    	printf("mtf file cannot be opened.\n");
		fclose(mtf_file);
		fclose(txt_file);
		exit(-1);	    
    }
    else 
    {
		/*puts the magic numbers in the mtf file*/
		mtf_file = write_magic_numbers(mtf_file);

		while ((getline(&line, &len, txt_file)) != -1) 
		{
			char* tok; /*char array used to hold the current tokenized set of characters*/
			char copy_of_line[500]; /*local copy of input line for tokenizing*/
			/*copy of input line gets tokenize based on spaces*/
			strcpy(copy_of_line, line);
            tok = strtok(copy_of_line, " ");

			while(tok != NULL) /*loop that iterates while there exist tokenized words*/
			{
				int all_words_traversal_counter = 0; /*counter used to traverse the array that has the order of the words collected from text file*/
				int value_in_file = 0; /*value of the number used for coding a word and that will be written in mtf file (after adding 128 to it)*/
				int words_are_the_same = 1; /*flag that says whether words are the same or not*/
				int has_new_line = 0;/*flag that says whether current tokenized word has a \n character or not*/
				int x; /*counter for traversing the tokenized word*/
				int length_of_tok; /*variable that holds the current value of the tokenized word*/
				char char_value;  /*char value of the value_in_file int variable + 128*/

				length_of_tok = strlen(tok);

				for(x = 0; x < length_of_tok; x++) /*loop that traverses through tokenized word looking for new lines*/
				{
					if(*(tok + x) == '\n')
					{
						has_new_line = 1;
						*(tok + x) = '\0'; 
						break;
					}
				}

				Nameval * traversal_pointer = word_order; 
				int position = 0; 

				/*loop traverses through collected words from text file*/
				for(all_words_traversal_counter = 0; traversal_pointer != NULL; all_words_traversal_counter++, traversal_pointer = traversal_pointer->next)
				{
					/*compares chars in current tokenized word with the value of the current word (if it is not NULL). Otherwise, give value of 1 to flag*/
					words_are_the_same = (traversal_pointer->name != NULL) ? strcmp(tok, traversal_pointer->name) : 1;
									
					if(words_are_the_same == 0 && strlen(tok) > 0) /*code for when the tokenized word is found within the already collected words*/ 
					{	
						word_order = move_to_front_and_return_previous_position(word_order, tok, &position);						
						value_in_file = all_words_traversal_counter + 1; /*the value used for coding in mtf file is grabbed from the former position of the word in word_order*/ 
						break;
					}
				}

				if(words_are_the_same != 0 && length_of_tok > 0 && *(tok) != '\0' && *(tok) != '\n') /*if the tokenized word is not found in the collected words, */ 
				{
					Nameval * new = newitem(string_duplicator(tok), 0);
					word_order = addfront(word_order, new);
					value_in_file = ++total_number_of_words; /*value of the coding for this word is the last position in word order. Increase word counter.*/
				}
				
				if((has_new_line != 0 && length_of_tok >1) || has_new_line == 0) /*coding value is put on file (for words not for \n characters)*/
					write_char_value(value_in_file, mtf_file);
								
				if(tok != NULL && words_are_the_same != 0) /*the tokenized word gets put on the file if it was just added to the collected words*/
					write_char_ptr_to_mtf(tok, mtf_file);					

				if(has_new_line != 0) /*if the tokenized word had a new line, add it to the mtf file*/
					write_char_to_mtf('\n', mtf_file);				
				
				tok = strtok(NULL, " "); /*gets next tokenized word*/
			}
		}

		if (line) /*frees the line used for reading from txt file*/
        {
            free(line);
        }

		freeall(word_order); /*frees the entire list*/
	}
	return 0;
}

FILE* verify_magic_numbers_exist_in_mtf_file(FILE *mtf) /*checks for the sequences of magic numbers 0xfa, 0xce, 0xfa, and 0xde or 0xdf*/
{  /*variables for checking the first 4 bytes*/
	char magic_numbers[] = {0Xfa, 0Xce, 0xfa};
	char *c;
	char file_char; 
	int counter = 0;
	
	for (c = magic_numbers; counter < 3; c++, counter++) /*loop for checkin the first 3 bytes*/
	{
		file_char = fgetc(mtf); /*grabs the next byte*/
		if (*c != (char) file_char) /*compares byte with current magic number*/
		{
			printf("Magic numbers 0xfa, 0xce, 0xfa not found.\n");
			exit(-1);
		}
	}	

	file_char = fgetc(mtf); /*grabs next byte*/
	if(file_char != (char)0xde && file_char != (char)0xdf) /*checks the fourth byte with either possibility 0xde or 0xdf*/
	{
		printf("Last magic number (0xde or 0xdf) is missing.");
		exit(-1);
	}
	return mtf; /*returns the file stream*/
} 

void add_new_word_to_list_and_write_to_file(char * new_word, Nameval** words, FILE*txt, int number_to_add) /*adds a new word to the list and writes to the txt file*/
{	/*variables for strcpy*/
	char copy_of_word[400] = "";
	strcpy(copy_of_word, new_word);
	int len = strlen(new_word);

	int str_counter;

	for(str_counter = 0; str_counter < len; str_counter++) /*loop that traverses through new word looking for new lines*/
	{
		if(*(copy_of_word + str_counter) == '\n') /*takes out the new line so that the word in the list doesn't have it*/
		{
			*(copy_of_word + str_counter) = '\0';
			break;
		}
	}

	Nameval * new = newitem(string_duplicator(copy_of_word), number_to_add); /*creates node with the modified word (without new line)*/
	*words = addfront(*words, new); /*puts word on the front*/

	if(strstr(new_word, "\n") == NULL) /*writes to the file and puts space if necessary*/
	{
		strcat(new_word, " "); 
	}	
	fputs(new_word, txt); /*writes the word to the txt file*/
}

void create_next_word(unsigned char* byte, int* byte_int, Nameval** words, FILE*txt, FILE*mtf, int number_to_add) /*creates a new word from the bytes of the mtf file*/
{
	char new_word[400] = ""; /*collects the bytes of from the mtf file*/
	int counter = 0;
		
	for(;;counter++) /*counter goes until it finds a byte that is encoding and not part of the word*/
	{
		unsigned char new_char = fgetc(mtf); /*gets next byte from file*/
		*byte = new_char;
		*byte_int = (int) new_char; /*converts byte to int*/
		
		if(*byte_int < 128 && *byte != -1) /*if the byte is not an encoding byte, add it to the word being built*/
		{
			new_word[counter] = *byte; /*construct the new word*/
		}
		else /*if this byte is from encoding, then finish the word and add it to the list*/
		{
			add_new_word_to_list_and_write_to_file(new_word, words, txt, number_to_add); /*adds the word to list and writes to txt file*/
			break; /*breaks the loop (the new word has been constructed)*/
		}
	}
}

/*if the coding byte is repeated, use the index to find the word in the list and move it to the front*/
void handle_repeated_word(int * byte_int, unsigned char* current_byte, FILE * txt, FILE * mtf ,Nameval ** words) 
{
	int index = *byte_int;  /*get the index*/
	*current_byte = fgetc(mtf); /*move to next byte*/
	
	Nameval * tmp = get_at(*words, index); /*gets node at the index*/	
	char word_to_write[400] = ""; /*used for string copy*/
	strcpy(word_to_write, tmp->name); /*copies the string*/

	if (*current_byte != '\n') /*if it doesn't have a new line, put a space there*/
	{
		strcat(word_to_write, " "); /*adds the space*/
	}
	fputs(word_to_write, txt); /*writes to the txt file*/
}

/*checks to see if the byte is a new line*/
FILE * if_byte_equals_new_line_write_to_file(unsigned char current_byte, FILE* txt) 
{
	if(current_byte == '\n') /*if it is, write it to the file*/
	{
		fputc('\n', txt); /*writes to the file*/
	}
	return txt; /*returns file stream*/
}

int start_decoding(FILE * txt_file, FILE * mtf_file)
{
	mtf_file = verify_magic_numbers_exist_in_mtf_file(mtf_file); /*checks for magic numbers in mtf file*/
	Nameval * word_order = NULL;	/*pointer for the list*/
	unsigned char current_byte = (unsigned char) fgetc(mtf_file); /*grabs first byte after the magic numbers*/

    while(current_byte != EOF ) /*loop to read file until it ends*/
	{
		unsigned int int_from_byte = (int)current_byte; /*converts the byte to an int*/

		if (int_from_byte >= 128 && int_from_byte <= 248) /*decoding for low coding numbers*/
		{
			int_from_byte -= 129; /*translates to index of the list*/
			int number_to_add_to_list = int_from_byte; 
			if(lookup_int(word_order, int_from_byte) == 1) /*if false (the int is not in the list)*/
			{
				create_next_word(&current_byte, &int_from_byte, &word_order, txt_file, mtf_file, number_to_add_to_list); /*creates file from mtf file's bytes*/				
			}
			else
			{
				handle_repeated_word(&int_from_byte, &current_byte, txt_file, mtf_file, &word_order); /*if repeated index, handle the word*/
				word_order = move_node_at_p_to_front(word_order, int_from_byte); /*move repeated word to front*/
			}			
		}
		else if(int_from_byte == 249) /*decoding for medium coding numbers (uses two bytes)*/
		{
			current_byte = (unsigned char) fgetc(mtf_file); /*reads next byte*/
			int_from_byte = (int) current_byte + 121 -1; /*translation according to coding convention*/
			int number_to_add_to_list = int_from_byte; /*after translation to index of list*/
			if(lookup_int(word_order, int_from_byte) == 1) /*if false (the int is not in the list)*/
			{
				create_next_word(&current_byte, &int_from_byte, &word_order, txt_file, mtf_file, number_to_add_to_list); /*creates file from mtf file's bytes*/
			}
			else
			{
				handle_repeated_word(&int_from_byte, &current_byte, txt_file, mtf_file, &word_order); /*if repeated index, handle the word*/
				word_order = move_node_at_p_to_front(word_order, int_from_byte); /*move repeated word to front*/
			}
		}
		else if(int_from_byte == 250) /*decoding for high coding numbers (uses three bytes)*/
		{ 
			current_byte = (unsigned char) fgetc(mtf_file); /*next byte*/
			int quotient = (int) current_byte;  /*get the quotient from byte*/
			current_byte = (unsigned char) fgetc(mtf_file); /*next byte*/
			int remainder = (int) current_byte; /*get the remainder from byte*/

			int_from_byte = 256 * quotient + remainder + 376 - 1; /*translate to list index*/
			int number_to_add_to_list = int_from_byte; 
			if(lookup_int(word_order, int_from_byte) == 1) /*if false (the int is not in the list)*/
			{
				create_next_word(&current_byte, &int_from_byte, &word_order, txt_file, mtf_file, number_to_add_to_list); /*creates file from mtf file's bytes*/
			}
			else
			{
				handle_repeated_word(&int_from_byte, &current_byte, txt_file, mtf_file, &word_order); /*if repeated index, handle the word*/
				word_order = move_node_at_p_to_front(word_order, int_from_byte); /*move repeated word to front*/
			}	
		}		
		else
		{
			if(int_from_byte == 255) /*if byte indicates the end of the file*/
			{
				break; /*get out of main loop*/
			}

			txt_file = if_byte_equals_new_line_write_to_file(current_byte, txt_file); /*if it is a new line, write it to the file*/
			current_byte = (unsigned char) fgetc(mtf_file); /*move on to the next byte*/
		}
	}
	
	freeall(word_order); /*free the entire list*/
	return 0;
}


