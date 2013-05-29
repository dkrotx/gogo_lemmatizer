/* lemmatizer using example: 
 * print firstform of each word from command line
 */
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include <clemmatiser.h>

int main(int argc, char *argv[])
{
    lemmatiser_t lem = lemmatiser_init(LEMMATISER_LANG_RUS);
   
    if (!lem || !lemmatiser_load_dictionary(lem))
        err(EXIT_FAILURE, "Error Initializing lemmatizer"); 

    for (++argv; *argv; argv++) {
        char **forms = lemmatiser_first_form(lem, *argv);
        printf("%s\n", forms[0]);
    }
    exit(EXIT_SUCCESS);
}

