#ifndef _CMORPHOLOGY_HPP__
#define _CMORPHOLOGY_HPP__ 1
//================================
// ==========   Dialing Lemmatizer (www.aot.ru)
//================================

#pragma warning(disable:4786)
#pragma warning(disable:4530)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <locale.h>

#include "../../AgramtabLib/RusGramTab.h"
#include "../../AgramtabLib/EngGramTab.h"
#include "../../AgramtabLib/GerGramTab.h"
#include "../../LemmatizerLib/Lemmatizers.h"
#include "../../LemmatizerLib/Paradigm.h"

#include <vector>
#include <iostream>

struct  CMorphology
{

	CLemmatizerRussian RussianDict;
	CRusGramTab Agramtab;
	CLemmatizerEnglish EnglishDict;
	CEngGramTab Egramtab;

	CLemmatizerGerman GermanDict;
	CGerGramTab Ggramtab;
	const CLemmatizer* GetLemmatizer(MorphLanguageEnum Lang) const;
	const CAgramtab* GetGramTab(MorphLanguageEnum Lang) const;

	string  GetAllForms (MorphLanguageEnum Language, string  Form);
	
	string  GetLemmaAndCodes (MorphLanguageEnum Language, string  Form, bool bScreenGramInfo);

	vector<string> GetFirstForm (MorphLanguageEnum Language, string  Form);

};
#endif 
