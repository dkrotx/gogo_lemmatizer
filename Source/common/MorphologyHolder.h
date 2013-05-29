#ifndef MorphologyHolder_H
#define MorphologyHolder_H

#include "utilit.h"
#include "../LemmatizerLib/PLMLineCollection.h"
#include "../GraphanLib/GraphmatFile.h"
#include "../AgramtabLib/RusGramTab.h"
#include "../AgramtabLib/GerGramTab.h"
#include "../AgramtabLib/EngGramTab.h"

class CMorphologyHolder
{
public:
	bool							m_bTimeStatis;
	MorphLanguageEnum				m_CurrentLanguage;
	CPlmLineCollection				m_PlmLines;
	CAgramtab*						m_pGramTab;
	CLemmatizer*					m_pLemmatizer;
	CGraphmatFile					m_Graphan;

	CMorphologyHolder();
	~CMorphologyHolder();

	bool GetMorphology(string str, bool bFile, int& CountOfWords);
	bool LoadGraphanAndLemmatizer(MorphLanguageEnum langua);
	void DeleteProcessors();
};



#endif
