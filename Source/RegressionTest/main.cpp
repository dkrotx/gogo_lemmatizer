#include <cstdio>
#include <cstdlib>
#include <err.h>

#include <clemmatiser.h>

#define ASIZE(a) ( sizeof(a)/sizeof(a[0]) )

int main()
{
    lemmatiser_t lem;
    static char *words[] = { 
                             "рыбному", "пробежкой", "вылетами",
                             "читать", NULL 
                           };
    char **cur;

    lem = lemmatiser_init(LEMMATISER_LANG_RUS);
    if (lemmatiser_load_dictionary(lem)) {
        for (cur = words; *cur; cur++)
        {
            printf( "SOURCE: %s\n", *cur );
            printf( "LEMMATISE: %s\n", lemmatiser_lemmatise(lem, *cur) );
            printf( "CODES: %s\n", lemmatiser_codes(lem, *cur) );

            char **forms = lemmatiser_first_form(lem, *cur);
            while (*forms)
                printf("FIRST FORM: %s\n", *forms++);
        }
	}
	else { 
        errx(EXIT_FAILURE, "Error Loading Dictionary"); 
    }

    return EXIT_SUCCESS;
}
