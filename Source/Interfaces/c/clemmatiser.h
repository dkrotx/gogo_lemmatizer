#ifndef CLEMMATISER_H__
#define CLEMMATISER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void *lemmatiser_t;

#define LEMMATISER_LANG_RUS 0
#define LEMMATISER_LANG_ENG 1
#define LEMMATISER_LANG_GER 2
/* initialize lemmatizer.
 * returns NULL on error, non-zero pointer otherwise
 */
lemmatiser_t lemmatiser_init(int lang);

int lemmatiser_load_dictionary(lemmatiser_t lem);
const char *lemmatiser_lemmatise(lemmatiser_t lem, char *szRequest);
const char *lemmatiser_codes(lemmatiser_t lem, char *szRequest);
char **lemmatiser_first_form(lemmatiser_t lem, char *szRequest);
void lemmatiser_done(lemmatiser_t lem);
#define lemmatiser_free(x) lemmatiser_done(x)

#ifdef __cplusplus
}
#endif

#endif /* CLEMMATISER_H__ */

