#include <stdexcept>
#include <cassert>
#include <cstdio>

#include "../cpp/SimpleLemmatiser.hpp"
#include "clemmatiser.h"

lemmatiser_t 
lemmatiser_init(int lang) 
{
    SimpleLemmatiser *plem;

    try {
    	plem = new SimpleLemmatiser(lang);
    } catch(std::exception &e) {
        fprintf(stderr, "lemmatiser_init: %s\n", e.what());
        return (lemmatiser_t *)(NULL);
    }
    catch(...) {
        fprintf(stderr, "lemmatiser_init: failed to initialize\n");
        return (lemmatiser_t *)(NULL);
    }
   
    assert(plem != NULL);
    return static_cast<lemmatiser_t>(plem);
}

int 
lemmatiser_load_dictionary(lemmatiser_t lem) {
	return static_cast<SimpleLemmatiser *>(lem)->LoadDictionary();
}

const char *
lemmatiser_lemmatise(lemmatiser_t lem, char *szRequest) {
	return static_cast<SimpleLemmatiser *>(lem)->Lemmatise(szRequest);
}

const char *
lemmatiser_codes(lemmatiser_t lem, char *szRequest) {
	return static_cast<SimpleLemmatiser *>(lem)->LemmaAndCodes(szRequest);
}

char **
lemmatiser_first_form(lemmatiser_t lem, char *szRequest) {
	return static_cast<SimpleLemmatiser *>(lem)->FirstForm(szRequest);
}

void 
lemmatiser_done(lemmatiser_t lem) {
	delete static_cast<SimpleLemmatiser *>(lem);
}

