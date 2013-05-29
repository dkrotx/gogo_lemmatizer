// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Lemmatizer (www.aot.ru)
// ==========  Copyright by Alexey Sokirko, Andrey Putrin

#ifndef FindDic_h
#define FindDic_h

#if _MSC_VER > 1000
#pragma once
#endif 

#include "../common/util_classes.h"
#include "../MorphWizardLib/wizard.h"

#include "Statistic.h"
#include "Predict.h"
#include "morphcst.h"
#include "StaticVectorMap.h"



typedef enum {	subjFinance = 1, subjComputer = 2, subjLiterature = 4	} SubjectEnum;



struct	CFindWordNode
{
	DWORD		m_nBase;
	WORD		m_nFlex;
	CLemmaInfo	m_LemmaInfo;

	//  This is the source homonym weight. It is used only in 
	// plm-lines lemmatization process, otherwise it is uninitialized.
	int			m_nWeight;

	bool		m_bFound;

	CFindWordNode();

	bool	operator ==(const CFindWordNode& obj) const;
	bool	operator <(const CFindWordNode& obj) const;
	void	GetLemSign(char* Buffer) const;

};

typedef CSmallVector<WORD,64> CResultVector;


struct CParadigmItem
{
	// a ordinal number of flexion in the m_Flexs
	WORD	m_nFlex;

	// a ordinal number of ancode
	WORD	m_AncodeNo;

};


inline size_t get_size_in_bytes (const CParadigmItem& t)
{
	return get_size_in_bytes(t.m_nFlex) + get_size_in_bytes(t.m_AncodeNo);
};

inline size_t save_to_bytes(const CParadigmItem& i, BYTE* buf)
{
	buf += save_to_bytes(i.m_nFlex, buf);
	buf += save_to_bytes(i.m_AncodeNo, buf);
	return get_size_in_bytes(i);
}

inline size_t restore_from_bytes(CParadigmItem& i, const BYTE* buf)
{
	buf += restore_from_bytes(i.m_nFlex, buf);
	buf += restore_from_bytes(i.m_AncodeNo, buf);
	return get_size_in_bytes(i);
}

typedef vector<CParadigmItem>::const_iterator CParadigmItemPtr;
typedef vector<WORD>::const_iterator CWordPtr;


class CMorphDictionary  
{
public:
	MorphLanguageEnum					m_Language;
	CShortStringHolder					m_Bases;
	CShortStringHolder					m_Flexs;
	CShortStringHolder					m_Infos;
	CStaticVectorMap<CParadigmItem>		m_FlexModels;
	CStaticVectorMap<BYTE>				m_AccentModels;
	CStatistic							m_Statistic;

	
	

	bool	HelperGetFormNumber(WORD flex, WORD paradigm, DWORD& form) const;

	DWORD	HelperGetParadigmID(DWORD base, CLemmaInfo paradigm) const;
	bool	HelperSplitParadigmID(DWORD ParadigmId, DWORD& base, CLemmaInfo& LemmaInfo) const;
	bool	HelperFirstParadigmID(DWORD& newid) const;
	bool	HelperNextParadigmID(DWORD oldid, DWORD& newid) const;
	bool	HelperPrevParadigmID(DWORD oldid, DWORD& newid) const;

	void	HelperGetInfo(const WORD nFlex, const WORD nParadigm, char*	result_buffer) const;
	char*	HelperGetNorm(const string& InputWordStr, const bool bFound, const DWORD nBase, const WORD nFlex, const WORD nParadigm, char* OutBuffer) const;
	WORD	GetFlexNoByFlexStr(const char* flex, size_t curlen) const;
	bool	Load(MorphLanguageEnum Language, const string& path);
	bool	IsNorm(const WORD flex, const WORD paradigm) const;
	bool	FindWord(string& InputWordStr,  bool cap, bool predict, bool max_predict, vector<CFindWordNode>& results) const;

protected:
	//  m_FlexLengthTable is a vector which holds all the lengths of all flexia.
	// if m_FlexLengthTable[i] = j, then
	// the flexia of length i starts from  position j in m_Flexs.
	// if m_FlexLengthTable[i] = MaxFlexiaLength, then there is no flexia of length i.
	vector<WORD>						m_FlexLengthTable;

	//  a map from a flexia index to a set of flexia models
	CStaticVectorMap<WORD>				m_Flex2FlexiaModels;

	// a map from a base (by an idnex) to all possible lemma infos 
	CStaticVectorMap<CLemmaInfo>			m_Base2LemmaInfos;
	
	CPredictBase						m_Predict;
	vector<CPredictEndRoot>				m_PredictEndRoots;

	bool	FindBases(const string& InputWordStr, const  CResultVector& FlexResults, vector<CFindWordNode>& results) const;
	bool	PredictByEndRoot(const string& InputWordStr,  vector<CFindWordNode>& results) const;
	bool	PredictByDataBase(const string& InputWordStr,  const  CResultVector& FlexResults,  vector<CFindWordNode>& results,bool is_cap,bool max_predict) const;
	void	BaseFind(const string& InputWordStr, const WORD flex, CFindWordNode& node) const;
	bool	FlexFind(const string& InputWordStr,  CResultVector& FlexResults)  const;
	void	LoadFrequentRoots(string path);

};



#endif 
