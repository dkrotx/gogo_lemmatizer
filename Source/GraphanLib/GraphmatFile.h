// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1996-2001)

#ifndef __GRAPHMATFILE_H_
#define __GRAPHMATFILE_H_

#include "UnitHolder.h"

class CGraphanDicts;
class CDictionary;

class CGraphmatFile  : public CUnitHolder
{
	string					m_SaveTxtName;
	string					m_XmlMacSynName;
	string					m_GraFileName;
	string					m_SourceFileName;
	string					m_LastError;


	// Analyzing
	bool		IsKey(size_t LB, size_t HB, size_t& GraLast) const;
	bool		FindKeySequence (const char* title, size_t i,size_t HB, size_t& GraLast)  const;
    bool		DealBullet (size_t i, size_t HB);
	bool		DealAsteriskBullet (size_t LB, size_t HB);
	int			DealFloatNumbers (size_t  StartPos, size_t  EndPos);
	int			DealBulletsWithTwoBrackets (size_t StartPos, size_t EndPos);
	bool		CanBeElectronicAddressCompound (size_t i);
	int			DealEmailAddresses (size_t StartPos, size_t EndPos);
	bool		IsProtocolAbbr (size_t i);
	bool		DealFreeInternetAddresses (size_t StartPos, size_t EndPos);
	bool		DealAbbrigedInternetAddresses(size_t  StartPos, size_t EndPos);
	bool		DealEnglishStyleFIO (size_t StartPos, size_t EndPos);
	bool		DealAbbrev (size_t  StartPos, size_t EndPos);
	int			DealHyphenPaar (size_t i,  size_t HB);
	size_t		FindOborotto (size_t i, size_t HB, short& OborotNo, vector<WORD>& OborortIds) const;
	void		DealOborotto(size_t  HB);
	int			DealReferences (size_t i,size_t HB);
	int			DealDates (size_t i,size_t HB);
	int			DealStop (size_t i);
	void		DealModifierKey (size_t LB, size_t HB);
	void		DealSimpleKey (size_t LB, size_t HB);
	void		DealKeySequence(size_t LB, size_t  HB);
	void		DealQuotedKeySequence(size_t LB, size_t  HB);
	void		DealGermanDividedCompounds(size_t LB, size_t  HB);
	void		DealExtensionsAndLocalFileNames(size_t LB, size_t  HB);
	int			DealInternetAddresses (size_t StartPos, size_t  EndPos);
	int			HasIndention  (size_t LB, size_t HB);
	int			CountEndL  (size_t LB, size_t HB);
	int			CountSpaces  (size_t LB, size_t HB);
	int			DealFIO (size_t i,size_t HB);
	void		DealNames (size_t LB, size_t  HB);
	bool		DealSentBreaker ();
	void		InitNonContextDescriptors (CGraLine& L);
	bool		GraphmatMain ();
	int			InitContextDescriptors (size_t LB, size_t HB);			
	void		MacSynHierarchy ();

	void		WriteGraphMat (const char* FName) const;


public:
	const CGraphanDicts*	m_pDicts;
	bool					m_bWriteTextBuffer;
	bool					m_bOnlyNonContextDescriptors;

	size_t					m_MinParOfs;
	size_t					m_MaxParOfs;
	size_t					m_TabSize;
	// максимальное число  строк в одном условно предложении 
	size_t					m_MaxUpLen; 
	// Нужно ли записывать файл *.gra?
	bool					m_bWriteGraFile;
	// Нужно ли записывать макросинтаксическую структуру в файл
	bool					m_bWriteXmlMacSyn;
	// Нужно ли запускать делилку на предложения. 
	bool					m_bSentBreaker;
	// нужжно и строить иерархию 
	bool					m_bMacSynHierarchy;
	// С каким УП нужно связывать УП, которые не могут связаться  по правилам?
	// Если m_bSubdueWrongMacroSynUnitToMainRoot = true, то программа подчиняет такие УП корню макросинтаксичесого дерева.
	// Если m_bSubdueWrongMacroSynUnitToMainRoot = false, то программа связывает такие УП корню с предыдущим УП отношение примыкания
	bool					m_bSubdueWrongMacroSynUnitToMainRoot;
	// считать, что предложение рапвно параграфу
	bool					m_bSentenceAsParagraph;
	// нужно ли конвертировать латиницу в кириллицу внутри русского слова, если это возможно
	// (исправление ошибок)
	bool					m_bForceToRus;

	// пустая строка является разделителем абазцев и предложений
	bool					m_bEmptyLineIsSentenceDelim;

	// использовать таг </p> для разделения на параграфы
	bool					m_bUseParagraphTagToDivide;

	// использовать красную строку для разделения на параграфы
	bool					m_bUseIndention;

	// сохранять htmlfile как плайн текст в tds файл
	bool					m_bSaveHtmlFileToTdsFile;


	CGraphmatFile();
	~CGraphmatFile();
	
	// processing 
	bool		LoadDicts();
	bool		LoadStringToGraphan(const string& szBuffer);
	bool		LoadFileToGraphan (const string&  CommandLine); 
	void		FreeDicts();
	const string&	GetLastError() const;
	
	//  browsing results 
	
	void				GetGraphematicalLine (char* line, size_t NumLine) const;
	MorphLanguageEnum	GetTokenLanguage (int LineNo) const;
	bool				StartsFixedOborot(size_t LineNo) const;
	const CDictionary*	GetOborDic() const;

	


	
};


#endif //__GRAPHMATFILE_H_
