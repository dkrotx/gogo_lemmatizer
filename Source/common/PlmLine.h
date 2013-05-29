// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Posmorphological Module (www.aot.ru)
// ==========  Copyright by Dmitry Pankratov, Alexey Sokirko (1999-2002)

#ifndef __PLMLINE_H_
#define __PLMLINE_H_

#include "../common/Graspace.h"
#include "../AgramtabLib/agramtab_.h"

enum MainWordTypeEnum {LE, ILE, NUM, NUM_CHAR, PUNCTUAT, OTHER};


class CPlmLine {
	string	m_GramCodes;
	string	m_Word;
	string	m_UpperWord;
	char	m_MorphSign;
	string	m_CommonGramCode;

public:
	int		m_FilePosition;
	int		m_TokenLengthInFile;
	string m_GraphDescr;
	bool   m_bHomonym;
	
	string m_Lemma;
	
	string m_ParadigmId;
	string m_HomoWeight;
	MainWordTypeEnum   m_Type;
	RegisterEnum	   m_Register;	
	bool	m_bToDelete;
	bool	m_bFirstUpperAlpha;
	bool	m_bQuoteMarks;	
	bool	m_bFI1;
	bool	m_bFI2;
	
	bool	m_bName;
	bool	m_bSent1;
	bool	m_bSent2;
	bool	m_bOborot1;
	bool	m_bOborot2;
	bool	m_bHyphenWord;
	bool	m_bMorphName;	
	bool	m_bMorphSurname;
	bool	m_bRomanNumber;
	QWORD	m_Grammems;	
	BYTE	m_Pos;	


	CPlmLine ();

	bool    LoadPlmLineFromString (string Buffer, bool bStartLine, const CAgramtab* pRusGramTab);
	string  GetStr() const;
	void	DeleteDescr(const char* Descr);
	void	DeleteOb1();
	void	DeleteOb2();
	bool	HasGraDescr(const char* GraphDescr) const;
	void	SetGramCodes(const string& NewGramCodes, const CAgramtab* pRusGramTab);
	const string&	GetGramCodes() const { return m_GramCodes;};
	bool	IsPartOfNonSingleOborot() const;
	bool	IsFoundInMorphology() const;
	void	SetWord(string s);
	const string&  GetWord() const;
	const string&  GetUpperWord() const;
	const string&   GetCommonGramCode() const;
	void			SetMorphUnknown();
	void			SetMorph(char MorphSign, const string& CommonAncode, const DWORD& ParadigmId);
	
};





#endif
