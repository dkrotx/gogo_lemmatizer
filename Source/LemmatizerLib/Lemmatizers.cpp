// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Lemmatizer (www.aot.ru)
// ==========  Copyright by Alexey Sokirko
#include "StdMorph.h"
#include "Lemmatizers.h"
#include "Paradigm.h"


CLemmatizer::CLemmatizer(MorphLanguageEnum Language) : CMorphDict(Language), m_Predict(Language)
{	
	m_bLoaded = false;
	m_bUsePrediction = true;
	m_bUseStatistic = false;
	m_bMaximalPrediction = false;
	m_bAllowRussianJo = false;
	InitAutomat( new CMorphAutomat(Language) );
};

CLemmatizer::~CLemmatizer()
{
};

string CLemmatizer::GetPath()  const
{
	string RegStr = GetRegistryString();
	string load_path = ::GetRegistryString( RegStr );
	if (	(load_path.length() > 0)	
		&&	(load_path[load_path.length() - 1] != '\\')
		&&	(load_path[load_path.length() - 1] != '/')
		)
		load_path += "/";

	return load_path;
};;


bool CLemmatizer::CheckABC(const string& WordForm) const
{
	return m_pFormAutomat->CheckABC(WordForm);
};

bool CLemmatizer::CreateParadigmFromID(DWORD id, CFormInfo& Result) const 
{
	Result.AttachLemmatizer(this);
	return Result.SetParadigmId(id);
}

bool CLemmatizer::IsHyphenPostfix(const string& Postfix) const
{
	return find(m_HyphenPostfixs.begin(), m_HyphenPostfixs.end(), Postfix) != m_HyphenPostfixs.end();
};


const CStatistic& CLemmatizer::GetStatistic() const 
{	
	return m_Statistic;	
}


bool CLemmatizer::FormatResults(const string& InputWordStr, const vector<CAutomAnnotationInner>& src, StringVector& results, bool bFound) const
{

	for (int i = 0; i < src.size(); i++)
	{
		const CAutomAnnotationInner& A = src[i];
		const CLemmaInfo& I = m_LemmaInfos[A.m_LemmaInfoNo].m_LemmaInfo;
		const CFlexiaModel& M = m_FlexiaModels[A.m_ModelNo];
		const CMorphForm& F = M.m_Flexia[A.m_ItemNo];

		string Line;
		// printing lem sign
		{
			Line += bFound ? '+' : '-';
			if (I.m_CommonAncode[0] == 0)
				Line += "??";
			else
				Line += string(I.m_CommonAncode, I.m_CommonAncode+2);
			Line +=  " ";
		};

		// print Lemma
		{

			string Lemma = InputWordStr.substr(0, InputWordStr.length() - F.m_FlexiaStr.length())+M.get_first_flex();
			if (Lemma.substr(0, F.m_PrefixStr.length()) == F.m_PrefixStr)
				Lemma.erase(0, F.m_PrefixStr.length());
			Line +=  Lemma;
			Line +=  " ";
		};

		// print ancode
		Line +=  F.m_Gramcode;
		
		//  print paradigm id and weight
		if (bFound) 
			Line += Format(" %lu %i", A.GetParadigmId(), A.m_nWeight);
		else
			Line += " -1 0";
				
		

		results.push_back(Line);
	}

	return !results.empty();
};

bool CLemmatizer::LemmatizeWordForPlmLines(string& InputWordStr, const bool cap, const bool predict,  StringVector& results) const 
{
	results.clear();
	vector<CAutomAnnotationInner>	FindResults;

	FilterSrc(InputWordStr);

	bool bFound = LemmatizeWord(InputWordStr, cap, predict, FindResults, true);

	AssignWeightIfNeed(FindResults);

	return  FormatResults(InputWordStr, FindResults, results, bFound);
}

bool CLemmatizer::IsPrefix(const string& Prefix) const
{
	return m_PrefixesSet.find(Prefix) != m_PrefixesSet.end();

};

//   CLemmatizer::LemmatizeWord should return true if 
// the word was found in the dictionary, if it was predicted, then it returns false
bool CLemmatizer::LemmatizeWord(string& InputWordStr, const bool cap, const bool predict, vector<CAutomAnnotationInner>& results, bool bGetLemmaInfos) const
{
	
	RmlMakeUpper (InputWordStr, GetLanguage());

	size_t WordOffset = 0;
	

	m_pFormAutomat->GetInnerMorphInfos(InputWordStr, 0, results);

	bool bResult = !results.empty();

	if (results.empty())
	{
		if (m_bUsePrediction)
		{
			PredictBySuffix(InputWordStr, WordOffset, 4, results); // the length of the minal suffix is 4 


			if (InputWordStr[WordOffset-1] != '-') //  and there is no hyphen
			{
				size_t  KnownPostfixLen = InputWordStr.length()-WordOffset;
				size_t  UnknownPrefixLen = WordOffset;
				if (KnownPostfixLen < 6)// if  the known part is too short
					//if	(UnknownPrefixLen > 5)// no prediction if unknown prefix is more than 5
					{
						if (!IsPrefix(InputWordStr.substr(0, UnknownPrefixLen)))
							results.clear();
					};
			};

			// отменяем предсказание по местоимениям, например "Семыкиным"
			for (size_t i=0; i<results.size(); i++)
				if (m_NPSs[results[i].m_ModelNo] == UnknownPartOfSpeech)
				{
					results.clear();
					break;
				};

		};
	};

	if (!results.empty())
	{
		if (bGetLemmaInfos)
			GetLemmaInfos(InputWordStr, WordOffset, results);
	}
	else
		if (m_bUsePrediction)
		{
			PredictByDataBase(InputWordStr, results,cap);
		};

	return bResult;
}






void CLemmatizer::AssignWeightIfNeed(vector<CAutomAnnotationInner>& FindResults) const
{

	for (size_t i = 0; i < FindResults.size(); i++)
	{
		CAutomAnnotationInner& A = FindResults[i];
		if (m_bUseStatistic)
			A.m_nWeight = m_Statistic.get_HomoWeight(A.GetParadigmId(), A.m_ItemNo);
		else
			A.m_nWeight = 0;
	}

}



void CLemmatizer::GetAllAncodesQuick(const BYTE* WordForm, bool capital, BYTE* OutBuffer) const
{
	*OutBuffer = 0;
	string InputWordStr = (const char*)WordForm;
	FilterSrc(InputWordStr);
	vector<CAutomAnnotationInner>	FindResults;
	LemmatizeWord(InputWordStr, capital, m_bUsePrediction, FindResults, false);
	size_t Count = FindResults.size();
	for (DWORD i=0; i < Count; i++)
	{
		const CAutomAnnotationInner& A = FindResults[i];
		const CFlexiaModel& M = m_FlexiaModels[A.m_ModelNo];
		

		strcat ((char*)OutBuffer, M.m_Flexia[A.m_ItemNo].m_Gramcode.c_str());
		strcat ((char*)OutBuffer,"#");
	};
}

/*string CLemmatizer::GetAllAncodesAndLemmasQuick(string& InputWordStr, bool capital) const
{
	FilterSrc(InputWordStr);

	vector<CAutomAnnotationInner>	FindResults;

	bool bFound = LemmatizeWord(InputWordStr, capital, m_bUsePrediction, FindResults, false);

	size_t Count = FindResults.size();
	string Result;
	for (DWORD i=0; i < Count; i++)
	{
		const CAutomAnnotationInner& A = FindResults[i];
		const CFlexiaModel& M = m_FlexiaModels[A.m_ModelNo];
		const CMorphForm& F = M.m_Flexia[A.m_ItemNo];

		string Lemma = InputWordStr.substr(0, InputWordStr.length() - F.m_FlexiaStr.length()) + M.m_Flexia[0].m_FlexiaStr;

		size_t len = F.m_PrefixStr.length();

		if	(		bFound 
				||	(InputWordStr.substr(0, len) == F.m_PrefixStr)
			)
			Lemma.erase(0, len);

		Result += 	Lemma;
		Result += 	" ";
		Result += 	M.m_Flexia[A.m_ItemNo].m_Gramcode;
		Result += 	"#";
		
	};

	return Result;
}

*/

bool CLemmatizer::GetAllAncodesAndLemmasQuick(string& InputWordStr, bool capital, char* OutBuffer, size_t MaxBufferSize) const
{
	FilterSrc(InputWordStr);

	vector<CAutomAnnotationInner>	FindResults;

	bool bFound = LemmatizeWord(InputWordStr, capital, m_bUsePrediction, FindResults, false);

	size_t Count = FindResults.size();
	size_t  OutLen = 0;
	for (DWORD i=0; i < Count; i++)
	{
		const CAutomAnnotationInner& A = FindResults[i];
		const CFlexiaModel& M = m_FlexiaModels[A.m_ModelNo];
		const CMorphForm& F = M.m_Flexia[A.m_ItemNo];
		size_t PrefixLen = F.m_PrefixStr.length();
		size_t BaseStart  = 0;
		if	(		bFound 
				||	!strncmp(InputWordStr.c_str(), F.m_PrefixStr.c_str(), PrefixLen)
			)
			BaseStart = PrefixLen;
		int BaseLen  = InputWordStr.length() - F.m_FlexiaStr.length() - BaseStart;
		if (BaseLen < 0) BaseLen = InputWordStr.length();
		size_t GramCodeLen = M.m_Flexia[A.m_ItemNo].m_Gramcode.length();
		size_t FlexiaLength = M.m_Flexia[0].m_FlexiaStr.length();
		if (BaseLen+FlexiaLength+3+GramCodeLen > MaxBufferSize-OutLen) return false; 

		strncpy(OutBuffer+OutLen, InputWordStr.c_str()+BaseStart, BaseLen);
		OutLen += BaseLen;

		strncpy(OutBuffer+OutLen, M.m_Flexia[0].m_FlexiaStr.c_str(), FlexiaLength);
		OutLen += FlexiaLength;

		OutBuffer[OutLen] = ' ';
		OutLen++;

		strncpy(OutBuffer+OutLen, M.m_Flexia[A.m_ItemNo].m_Gramcode.c_str(), GramCodeLen);
		OutLen += GramCodeLen+1;
		OutBuffer[OutLen-1] = 	'#';

	};
	OutBuffer[OutLen] = 0;
	return true;
	
}


//////////////////////////////////////////////////////////////////////////////

void CLemmatizer::ReadOptions(string FileName)
{
	string Options;
	LoadFileToString(FileName, Options);
	StringTokenizer lines(Options.c_str(), "\r\n");
	while (lines())
	{
		string line = lines.val();
		Trim(line);
		if (line.empty()) continue;
		if (line == "AllowRussianJo")
			m_bAllowRussianJo = true;
	};
};

bool CLemmatizer::LoadDictionariesRegistry()
{
	try
	{
		string load_path = GetPath();
		m_bLoaded = Load(load_path+MORPH_MAIN_FILES);
		if (!m_bLoaded) return false;

		// implicity load homonyms statistic for literature
		m_Statistic.Load(load_path + "l");
		m_bUseStatistic = true;
		m_Predict.Load(load_path + PREDICT_BIN_PATH);



		ReadOptions(load_path + OPTIONS_FILE);
		m_PrefixesSet.clear();
		m_PrefixesSet.insert(m_Prefixes.begin(), m_Prefixes.end() );

		return m_bLoaded;
	}
	catch(...)
	{
		return false;
	}
}



bool CLemmatizer::CreateParadigmCollection(bool bNorm, string& InputWordStr, bool capital, vector<CFormInfo>& Result) const
{
	FilterSrc(InputWordStr);
	vector<CAutomAnnotationInner>	FindResults;
	bool bFound = LemmatizeWord(InputWordStr, capital, m_bUsePrediction, FindResults, true);
		
	if (FindResults.empty())
	{
		// the word cannot be predicted or m_bUsePrediction is false
		return false;
	};
	
	AssignWeightIfNeed(FindResults);

	for (size_t i = 0; i < FindResults.size(); i++)
	{
		const CAutomAnnotationInner& A = FindResults[i];
		// if bNorm, then  ignore words which are not lemma
		if (   bNorm && (A.m_ItemNo!=0)) continue;
		
		CFormInfo P;
		P.Create(this, A, InputWordStr, bFound);

		Result.push_back(P);
	}

	return true;
}


bool CLemmatizer::LoadStatisticRegistry(SubjectEnum subj)
{
	try
	{
		string load_path = GetPath();
		string prefix;
		switch (subj)
		{
		case subjFinance:
			prefix += "f";
			break;
		case subjComputer:
			prefix += "c";
			break;
		case subjLiterature:
			prefix += "l";
			break;
		default:
			return false;
		}
		m_Statistic.Load(load_path + prefix);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

CAutomAnnotationInner  CLemmatizer::ConvertPredictTupleToAnnot(const CPredictTuple& input) const  
{
	CAutomAnnotationInner node;
	node.m_LemmaInfoNo = input.m_LemmaInfoNo;
	node.m_ModelNo = m_LemmaInfos[node.m_LemmaInfoNo].m_LemmaInfo.m_FlexiaModelNo;
	node.m_nWeight = 0;
	node.m_PrefixNo = 0;
	node.m_ItemNo = input.m_ItemNo;
	return node;
};


bool CLemmatizer::CheckAbbreviation(string InputWordStr,vector<CAutomAnnotationInner>& FindResults, bool is_cap) const
{
	for(size_t i=0; i <InputWordStr.length(); i++)
		if (!is_upper_consonant((BYTE)InputWordStr[i], GetLanguage()))
			return false;

	vector<CPredictTuple> res;
	m_Predict.Find(CriticalNounLetterPack,res); 
	FindResults.push_back(ConvertPredictTupleToAnnot(res[0]));
	return true;
};

void CLemmatizer::PredictByDataBase(string InputWordStr,  vector<CAutomAnnotationInner>& FindResults,bool is_cap) const  
{

	vector<CPredictTuple> res;
	if (CheckAbbreviation(InputWordStr, FindResults, is_cap))
		return;

	if (CheckABC(InputWordStr)) // if the ABC is wrong this prediction yuilds to many variants
	{		
		reverse(InputWordStr.begin(),InputWordStr.end());
		m_Predict.Find(InputWordStr,res);
	}

	vector<bool> has_nps(32,false); // assume not more than 32 different pos
	for( int j=0; j<res.size(); j++ )
	{
		BYTE PartOfSpeechNo = res[j].m_PartOfSpeechNo;
		if	(		!m_bMaximalPrediction 
				&& has_nps[PartOfSpeechNo] 
			)
			continue;

		has_nps[PartOfSpeechNo] = true;

		FindResults.push_back(ConvertPredictTupleToAnnot(res[j]));
	}

	if	(		(!has_nps[0]) // no noun
			||	( is_cap && (GetLanguage() != morphGerman)) // or can be a proper noun (except German, where all nouns are written uppercase)
		)
	{
		m_Predict.Find(CriticalNounLetterPack,res); 
		FindResults.push_back(ConvertPredictTupleToAnnot(res.back()));
	};

}


bool CLemmatizer::ProcessHyphenWords(CGraphmatFile* piGraphmatFile) const 
{
	try
	{
		
		size_t LinesCount = piGraphmatFile->GetTokensCount();
	

		for (int LineNo = 1; LineNo+1 < LinesCount; LineNo++)
		{
			if	(		piGraphmatFile->HasDescr(LineNo, OHyp)
					&&	(GetLanguage() == piGraphmatFile->GetTokenLanguage(LineNo-1)

					// and if no single space was deleted between the first word and the hyphen
					&&	!piGraphmatFile->GetUnits()[LineNo-1].HasSingleSpaceAfter()

					&&	!piGraphmatFile->HasDescr(LineNo-1, OSentEnd)
					&&	!piGraphmatFile->HasDescr(LineNo, OSentEnd)

					// it is not possible to create a hyphen word when this part of the text has an intersection 
					// with an oborot which is marked as "fixed" in the dictionary 
					&&	!piGraphmatFile->StartsFixedOborot(LineNo))
					&&	!piGraphmatFile->StartsFixedOborot(LineNo-1)
				)
			{
				//  creating a concatenation if it possible
				size_t NextWord = piGraphmatFile->PSoft(LineNo+1, LinesCount);
				if (NextWord == LinesCount) continue;

				// it is not possible to create a hyphen word  when this part of the text has an intersection 
				// with an oborot which is marked as "fixed" in the dictionary 
				if (piGraphmatFile->StartsFixedOborot(NextWord)) continue;
				
				if (GetLanguage() != piGraphmatFile->GetTokenLanguage(NextWord)) continue;
				string HyphenWord = piGraphmatFile->GetToken(LineNo-1)+"-"+piGraphmatFile->GetToken(NextWord);


				vector<CAutomAnnotationInner>	FindResults;
				if (LemmatizeWord(HyphenWord, !piGraphmatFile->HasDescr(LineNo-1,OLw), false,FindResults, false))
				{
					// uniting words LineNo-1, LineNo,  and NextWord
					piGraphmatFile->MakeOneWord(LineNo-1, NextWord+1);

					//  update LinesCount
					LinesCount = piGraphmatFile->GetTokensCount();
				};
			};

		};

	}
	catch (...)
	{
		return false;
	}
	return true;

};



CLemmatizerRussian::CLemmatizerRussian() : CLemmatizer(morphRussian)
{
	m_Registry = "Software\\Dialing\\Lemmatizer\\Russian\\DictPath";
	m_HyphenPostfixs.push_back("КА");
	m_HyphenPostfixs.push_back("ТО");
	m_HyphenPostfixs.push_back("С");
};


void CLemmatizerRussian::FilterSrc(string& src) const	
{
	if (!m_bAllowRussianJo)
		ConvertJO2Je(src); 
};

CLemmatizerEnglish:: CLemmatizerEnglish() : CLemmatizer(morphEnglish)
{
	m_Registry = "Software\\Dialing\\Lemmatizer\\English\\DictPath";
};

void CLemmatizerEnglish:: FilterSrc(string& src) const
{
};

CLemmatizerGerman:: CLemmatizerGerman() : CLemmatizer(morphGerman)
{
	m_Registry = "Software\\Dialing\\Lemmatizer\\German\\DictPath";
};

void CLemmatizerGerman:: FilterSrc(string& src) const
{
};


