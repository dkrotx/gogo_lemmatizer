// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Lemmatizer (www.aot.ru)
// ==========  Copyright by Alexey Sokirko

#include "MorphologyHolder.h"

#ifdef DETECT_MEMORY_LEAK
	#ifdef _DEBUG
	#define new DEBUG_NEW

	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#endif
#endif


CMorphologyHolder::CMorphologyHolder()
{
	m_pLemmatizer = 0;
	m_pGramTab = 0;
	m_bTimeStatis = false;
};

CMorphologyHolder::~CMorphologyHolder()
{
	DeleteProcessors();
};

void CMorphologyHolder::DeleteProcessors()
{
	if (m_pLemmatizer) delete m_pLemmatizer;
	if (m_pGramTab) delete m_pGramTab;

};


bool CMorphologyHolder::LoadGraphanAndLemmatizer(MorphLanguageEnum langua)
{
	try
	{
		DeleteProcessors();

		m_Graphan.FreeDicts();
		m_Graphan.m_Language = langua;

		if (langua == morphRussian)
		{
			m_pGramTab = new CRusGramTab;
			m_pLemmatizer = new CLemmatizerRussian;
		}
		else
			if (langua == morphGerman)
			{
				m_pGramTab = new CGerGramTab;
				m_pLemmatizer = new CLemmatizerGerman;
			}
			else
			if (langua == morphEnglish)
			{
				m_pGramTab = new CEngGramTab;
				m_pLemmatizer = new CLemmatizerEnglish;
			}
			else
			{
				ErrorMessage ("unsupported language");
				return false;
			};
				

        if (!m_Graphan.LoadDicts())
		{	
			ErrorMessage("Cannot load graphan\n");
			return false;
		}

		if (!m_pLemmatizer->LoadDictionariesRegistry())
		{
			ErrorMessage("Cannot load morphology\n");
			return false;
		};
		if (!m_pGramTab->LoadFromRegistry())
		{	
			ErrorMessage("Cannot load gramtab\n");
			return false;
		};

		m_PlmLines.m_pLemmatizer = m_pLemmatizer;
		return true;
	}
	catch(...)
	{
		return false;
	};

};


bool CMorphologyHolder::GetMorphology(string str, bool bFile, int& CountOfWords)
{
	clock_t t1,t2;
	CountOfWords = 0;

	try {
		

		// ============  Graphematics =======================
		if (m_bTimeStatis) t1= clock();
		bool bResult = (bFile) ? m_Graphan.LoadFileToGraphan(str.c_str()):m_Graphan.LoadStringToGraphan(str.c_str());
		if (!bResult)
		{
			fprintf (stderr, "Graphan has crushed!\n");
			return false;
		};
		
		if (m_bTimeStatis) 
		{
			t2 = clock();
			size_t TokensCount = m_Graphan.GetTokensCount();
			for (int i = 0; i <TokensCount; i++)
				if (m_Graphan.GetTokenLanguage(i) == m_CurrentLanguage )
							CountOfWords++;
			fprintf (stderr,"CountOfWords = %i\n",CountOfWords);

			double speed =  ((double)CountOfWords)/((t2-t1)/((double)CLOCKS_PER_SEC));
			fprintf(stderr,"Graphan: Ticks = %i Speed = %6.0f\n", t2-t1, speed );
			

		};

		// ============  Morphology =======================
		if (m_bTimeStatis) t1= clock();

		m_pLemmatizer->ProcessHyphenWords(&m_Graphan);

		if (!m_PlmLines.ProcessPlmLines(&m_Graphan))
		{
			fprintf (stderr, "  Cannot get morph. interpretation from Lemmatizer\n");;
			return false;
		};

		if (m_bTimeStatis) 
		{
			t2 = clock();
			double speed =  ((double)CountOfWords)/((t2-t1)/((double)CLOCKS_PER_SEC));
			fprintf(stderr,"Morphology: Ticks = %i Speed = %6.0f\n", t2-t1, speed );
		};
		m_Graphan.FreeTable();
		return true;
	}
	catch (...)
	{
		return false;
	};


};
