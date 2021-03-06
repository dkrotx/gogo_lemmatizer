// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1996-2001)

#ifndef __GRAPHMAT_DICTS_H
#define __GRAPHMAT_DICTS_H

#include "graline.h"       
#include "abbrev.h"
#include "../common/util_classes.h"


const int MaxSpaceWordLen = 100;

const size_t MaxNameSize = 100;

class CEnglishName {
public:

	char name[MaxNameSize];

	bool operator==(const CEnglishName& comparand) const
	{ return strcmp (name, comparand.name) == 0;};

	bool operator<(const CEnglishName& comparand) const
	{ return strcmp (name, comparand.name) < 0;};

};
inline bool EnglishNameLess (const CEnglishName& comparand1, const char* comparand2) 
{ 
	return strcmp (comparand1.name, comparand2) < 0;
};







class CGraphanDicts {
public:

	MorphLanguageEnum m_Language;

	//  should  we unit a hyphened word if it written without spaces
	bool					m_bCreateHyphenedWords;

	CGraphanDicts(MorphLanguageEnum Language);
	~CGraphanDicts();
	void FreeData();

	

	struct CSpacedWord 
	{
	   char m_SpacedWord[MaxSpaceWordLen];
	   int  m_SpacedWordLen; 
	};
	vector<CSpacedWord> m_Spaces;
	bool   ReadSpaces (string FileName);
	const char*  SearchSpace (const char *In ,int *len ) const;



	// ������������� - ��� ���-��, ��� �������� � ����  ���� ����������, �� �������� ����� ������
	// ���� C++, TCP/IP.
	StringVector			m_Idents[256];
	bool FindInIdents (const char* s, BYTE& ResultLen) const;
	bool ReadIdents (string FileName);


	// John, Bill
	vector<CEnglishName>	m_EnglishNames;
	bool		ReadENames (string FileName);
	

	mutable _share_pointer_t<CDictionary*>		m_pOborDic;
	const CDictionary* GetOborDic() const
	{
		return m_pOborDic.m_Pointer;
	};

	void  SetOborDic(CDictionary* Dic) 
	{
		m_pOborDic.SetPointer(Dic, false );
	};

	
	map<WORD, vector<WORD> >		m_OborottosFirstWordIndex;
	vector<CGraphemOborot>			m_Oborottos;
	StringVector					m_OborotTokens;
	

	
	
	void		BuildOborot (const string& s, int OborotNo,bool bFixedFet);
	void		BuildOborottos ();
    
	vector<CAbbrev>		m_Abbrevs;
	bool		ReadAbbrevations();

    
    
	StringVector m_KeyModifiers;
	bool		IsKeyModifier(const char* Str, size_t Len) const;

	StringVector m_Keys;
	bool		ReadKeyboard(string FileName);
	

	StringVector m_Extensions;
	bool			IsExtension(const char * UpperStr, BYTE Len) const;
	bool		  ReadExtensions(string FileName);
	


};



#endif
