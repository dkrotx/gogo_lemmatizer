#include "../../AgramtabLib/RusGramTab.h"
#include "../../AgramtabLib/EngGramTab.h"
#include "../../AgramtabLib/GerGramTab.h"
#include "../../LemmatizerLib/Lemmatizers.h"
#include "../../LemmatizerLib/Paradigm.h"

#include <stdlib.h>

#include "CMorphology.hpp"

#ifndef _SIMPLE_LEMMATISER_HPP__
#define _SIMPLE_LEMMATISER_HPP__ 1


class SimpleLemmatiser
{
public:
	SimpleLemmatiser(int Lang);
	~SimpleLemmatiser();
	const char * Lemmatise(char * szSource);
	const char * LemmaAndCodes(char * szSource);
	char ** FirstForm(char * szSource);
	bool LoadDictionary();
private:
	MorphLanguageEnum Language;
	CMorphology     * M;
	int               iLanguage;
};

#endif
