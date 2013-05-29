// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Lemmatizer (www.aot.ru)
// ==========  Copyright by Alexey Sokirko, Andrey Putrin

#ifndef __LEMMATIZERS_H_
#define __LEMMATIZERS_H_

// #pragma warning (disable : 4786)

#include "MorphDict.h"
#include "Paradigm.h"
#include "Statistic.h"
#include "Predict.h"

// #pragma warning (disable : 4250)

class CGraphmatFile;

typedef enum {	subjFinance = 1, subjComputer = 2, subjLiterature = 4	} SubjectEnum;


class CLemmatizer  : public CMorphDict
{
public:
	string				m_Registry;
	
	StringVector		m_HyphenPostfixs;
	CStatistic			m_Statistic;
	CPredictBase		m_Predict;
	set<string>			m_PrefixesSet;


	virtual void			FilterSrc(string& src) const = 0;

	string					GetRegistryString()  const	{return m_Registry;	};
	string					GetPath()  const;
	void					ReadOptions(string FileName);
		
	bool					FormatResults(const string& InpuWordStr, const vector<CAutomAnnotationInner>& src, StringVector& results, bool bFound) const;
	bool					LemmatizeWord(string& InputWordStr, const bool cap, const bool predict, vector<CAutomAnnotationInner>& results, bool bGetLemmaInfos) const;
	void					AssignWeightIfNeed(vector<CAutomAnnotationInner>& FindResults) const;

	// prediction by suffix
	bool					CheckAbbreviation(string InputWordStr,vector<CAutomAnnotationInner>& FindResults, bool is_cap) const;
	CAutomAnnotationInner	ConvertPredictTupleToAnnot(const CPredictTuple& input) const;
	void					PredictByDataBase(string InputWordStr,  vector<CAutomAnnotationInner>&  results,bool is_cap) const;
	bool					IsPrefix(const string& Prefix) const;
	

public:
	bool					m_bLoaded;
	bool					m_bUsePrediction;
	bool					m_bMaximalPrediction;
	bool					m_bUseStatistic;
	bool					m_bAllowRussianJo;

	
	
	
	CLemmatizer(MorphLanguageEnum Language);
	virtual ~CLemmatizer();

	//  basic methods
	MorphLanguageEnum GetLanguage()  const {return m_pFormAutomat->m_Language; };
	const CStatistic& GetStatistic() const;
	bool	CheckABC(const string& WordForm) const;
	bool	IsHyphenPostfix(const string& Postfix) const;

	
	//  loading
	bool	LoadDictionariesRegistry() ;
	bool	LoadStatisticRegistry(SubjectEnum subj);

	//  main interfaces
	bool	LemmatizeWordForPlmLines(string& InpuWordStr, const bool cap, const bool predict,  StringVector& results) const;
	bool	CreateParadigmCollection(bool bNorm, string& WordStr, bool capital, vector<CFormInfo>& Result) const;
	void	GetAllAncodesQuick(const BYTE* WordForm, bool capital, BYTE* OutBuffer) const;
	//string	GetAllAncodesAndLemmasQuick(string& InputWordStr, bool capital) const;
	bool	GetAllAncodesAndLemmasQuick(string& InputWordStr, bool capital, char* OutBuffer, size_t MaxBufferSize) const;
	bool	CreateParadigmFromID(DWORD id, CFormInfo& Result) const;
	bool	ProcessHyphenWords(CGraphmatFile* piGraphmatFile) const;



};





/////////////////////////////////////////////////////////////////////////////
class CLemmatizerRussian : public CLemmatizer
{
public:
	CLemmatizerRussian();
	virtual ~CLemmatizerRussian() {};
	void FilterSrc(string& src) const;
};


class  CLemmatizerEnglish : public CLemmatizer
{
	void FilterSrc(string& src) const;
public:
	CLemmatizerEnglish();
	virtual ~CLemmatizerEnglish() {};
	
};



/////////////////////////////////////////////////////////////////////////////

class  CLemmatizerGerman: public CLemmatizer
{
	void FilterSrc(string& src) const;
public:
	CLemmatizerGerman();
	virtual ~CLemmatizerGerman() {};
	
};





#endif //__LEMMATIZERS_H_
