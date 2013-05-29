#include "CMorphology.hpp"
extern void KOI8ToWin (string& s);
extern void WinToKOI8 (string& s);


const CAgramtab* CMorphology::GetGramTab(MorphLanguageEnum Lang) const
{
	switch (Lang)
	{
		case morphRussian : return   &Agramtab;
		case morphEnglish : return	 &Egramtab;
		default: return &Ggramtab;
	}
};

const CLemmatizer* CMorphology::GetLemmatizer(MorphLanguageEnum Lang) const
{
	switch (Lang)
	{
		case morphRussian : return   &RussianDict;
		case morphEnglish : return	 &EnglishDict;
		default: return &GermanDict;
	}
};

string GetGramInfo (const CAgramtab* A, const char* tab_str)
{
	short   PartOfSpeech = A->GetPartOfSpeech (tab_str);
	QWORD  AllGrammems = 0;
	for (long i=0; i < strlen(tab_str); i+=2)
		{
		  QWORD G;
		 	A->GetGrammems(tab_str+i, G);
			AllGrammems  |= G;
		};
	string  Result	= A->GetPartOfSpeechStr(PartOfSpeech) + string(" ") ;
	Result	+= A->GrammemsToStr (AllGrammems);
	return Result;
}

string GetPartOfSppech (const CAgramtab* A, const char* tab_str)
{
	short   PartOfSpeech = A->GetPartOfSpeech (tab_str);
	string  Result	= A->GetPartOfSpeechStr(PartOfSpeech) ;
	return Result;
}


char UpperAlphas[] = "ÜÄÖQWERTYUIOPASDFGHJKLZXCVBNMêãõëåîçûıúèÿæù÷áğòïìäöüñşóíéôøâà_";
bool IsUpperAlpha(int i )
{
	return strchr (UpperAlphas, i ) != 0;
};

void RussianConvertToDictionary (string& S)
{
	#ifdef WIN32
		OemToCharBuff((char*)S.c_str(), (char*)S.c_str(), S.length());		 
	#else
		//KOI8ToWin(S);
	#endif

};

void RussianConvertToScreen (string& S)
{
	#ifdef WIN32
		CharToOemBuff((char*)S.c_str(), (char*)S.c_str(), S.length());		 
	#else
		//WinToKOI8(S);
	#endif

};

string  CMorphology::GetLemmaAndCodes (MorphLanguageEnum Language, string  Form, bool bScreenGramInfo)
{
	string  Result;

	const CLemmatizer* R = GetLemmatizer(Language);
	const CAgramtab* A =  GetGramTab(Language);

	bool bCapital   = IsUpperAlpha((unsigned char)Form[0]);
	if (Language == morphRussian) 	
		RussianConvertToDictionary(Form);

	vector<CFormInfo> Paradigms;
	R->CreateParadigmCollection(false, Form, bCapital, Paradigms);
	for (int i =0; i < Paradigms.size(); i++)
	{
		string GramCodes =	Paradigms[i].GetSrcAncode();
		string GramInfo =   (bScreenGramInfo) ? GetGramInfo(A, GramCodes.c_str()) : GramCodes;
		string  Found = Paradigms[i].m_bFound ? "+" : "-";
		Result += Found + " "+Paradigms[i].GetWordForm(0) + " " +GramInfo + "\n";
   	};

	if  ( morphRussian == Language)    
		RussianConvertToScreen(Result);

	return Result;
};


string  CMorphology::GetAllForms (MorphLanguageEnum Language, string  Form)
{
	string  Result;
	const CLemmatizer* R = GetLemmatizer(Language);
	const CAgramtab* A =  GetGramTab(Language);

	bool bCapital   = IsUpperAlpha((unsigned char)Form[0]);
	if (Language == morphRussian) 	RussianConvertToDictionary(Form);

	vector<CFormInfo> Paradigms;
	R->CreateParadigmCollection(false, Form, bCapital, Paradigms);

	for (int i =0; i < Paradigms.size(); i++)
	{
		string GramCodes =	Paradigms[i].GetSrcAncode();
		string POS =   GetPartOfSppech(A, GramCodes.c_str()) ;
		Result += POS;
		Result += ":";

		for (int k=0; k < Paradigms[i].GetCount (); k++)
		{
			Result += Paradigms[i].GetWordForm(k);
			Result += " ";
		};
		Result += "\n";
	}
	if  ( morphRussian == Language)    RussianConvertToScreen(Result);

	return Result;
};

vector<string> CMorphology::GetFirstForm (MorphLanguageEnum Language, string  Form)
{

	// Ğåçóëüòàò ïîèñêà
	vector<string> Result;

	const CLemmatizer* R = GetLemmatizer(Language);
	const CAgramtab* A =  GetGramTab(Language);

	bool bCapital = IsUpperAlpha((unsigned char)Form[0]);
	if (Language == morphRussian) 	RussianConvertToDictionary(Form);

	vector<CFormInfo> Paradigms;
	R -> CreateParadigmCollection(false, Form, bCapital, Paradigms);

	for (int i =0; i < Paradigms.size(); i++)
	{
		string sFirstForm = Paradigms[i].GetWordForm(0);
		if  ( morphRussian == Language) { RussianConvertToScreen(sFirstForm); }

		Result.push_back(sFirstForm);
	}


return Result;
};


