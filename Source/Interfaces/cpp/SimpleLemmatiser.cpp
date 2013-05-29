#include "SimpleLemmatiser.hpp"
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>

SimpleLemmatiser::SimpleLemmatiser(int Lang)
{
	iLanguage = Lang;
	
	M = new CMorphology;
	switch (iLanguage)
	{
		case 0:
			Language = morphRussian;
			break;
		case 1:
			Language = morphEnglish;
			break;
		case 2:
			Language = morphGerman;
			break;
	}
}

bool SimpleLemmatiser::LoadDictionary()
{
	if (Language == morphRussian)
	{
		if (!M -> Agramtab.LoadFromRegistry()) { return false; }
		if (!M -> RussianDict.LoadDictionariesRegistry()) { return false; }
	}
	else if (Language == morphEnglish)
	{
		if (!M -> Egramtab.LoadFromRegistry()) { return false; }
		if (!M -> EnglishDict.LoadDictionariesRegistry()) { return false; }
	}
	else if (Language == morphGerman)
	{
		if (!M -> Ggramtab.LoadFromRegistry()) { return false; }
		if (!M -> GermanDict.LoadDictionariesRegistry()) { return false; }
	}
	return true;
}

SimpleLemmatiser::~SimpleLemmatiser()
{
	delete M;
}

const char * SimpleLemmatiser::Lemmatise(char * szSource)
{
	string  s(szSource);
	string  Result = M -> GetAllForms ( Language, s );

	if (!Result.empty()) { return Result.c_str(); }
	else { return NULL; }
}

const char * SimpleLemmatiser::LemmaAndCodes(char * szSource)
{
	string  s(szSource);
	string  sCodes = M -> GetLemmaAndCodes(Language, s,  true);

	if (!sCodes.empty()) { return sCodes.c_str(); }
	else { return NULL; }

}

char ** SimpleLemmatiser::FirstForm(char * szSource)
{
	char ** pszFirstForms = NULL;
	string  s(szSource);
	vector <string> VS = M -> GetFirstForm(Language, s);
	
	int iVectorSize = VS.size();
	pszFirstForms = (char **)malloc((iVectorSize + 1) * sizeof(char *));
	
	for (int i =0; i < iVectorSize; i++)
	{
		int iStrLength = VS[i].length();
		char * szForm = (char *)(malloc(iStrLength + 1));
		strncpy(szForm , VS[i].c_str(), iStrLength);
		szForm[iStrLength] = 0;
		pszFirstForms[i] = szForm;
	}
	pszFirstForms[iVectorSize] = NULL;

	return pszFirstForms;
}
