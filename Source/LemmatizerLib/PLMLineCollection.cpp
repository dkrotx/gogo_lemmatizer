// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Lemmatizer (www.aot.ru)
// ==========  Copyright by Alexey Sokirko, Andrey Putrin

#include "StdMorph.h"
#include "Lemmatizers.h"
#include "PLMLineCollection.h"
#include	 "../GraphanLib/GraphmatFile.h"

#ifdef DETECT_MEMORY_LEAK
	#ifdef _DEBUG
	#define new DEBUG_NEW

	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#endif
#endif


CPlmLineCollection::CPlmLineCollection()
{
	m_pLemmatizer = NULL;
};

CPlmLineCollection::~CPlmLineCollection()
{
}



bool IsFound(const StringVector& results)
{
	return !results.empty() && (results[0][0] == '+');

};

void CreateDecartProduction (const StringVector& results1, const StringVector& results2, StringVector& results)
{
	assert (!results1.empty());
	assert (!results2.empty());
	results.clear();
	for (size_t i=0; i<results1.size(); i++)
	{
		char Lemma1[256];
		assert (results1[i].size() > 4);
		sscanf (results1[i].c_str()+4, "%s", Lemma1);

		for (size_t k=0; k<results2.size(); k++)
		{
			char Sign[4];
			char Lemma2[256];
			char Ancodes[256];
			int check =  sscanf (results2[k].c_str(), " %s %s %s", Sign,Lemma2, Ancodes);
			assert (check == 3);
			assert (strlen(Sign) == 3);

			// printing with unknown Paradigm Id, since this word was not found in the dictionary
			string Out = Format(" -%s %s-%s %s -1 0",  Sign+1, Lemma1, Lemma2, Ancodes);
			results.push_back(Out);


		};

	};
};



bool CPlmLineCollection::ProcessPlmLines(const CGraphmatFile* piGraphmatFile)
{
	int LineNo = 0;
	try
	{
		if (!m_pLemmatizer)
			return false;

		ClearVector(m_Items);

		StringVector results;
		results.reserve(5);


		StringVector results1;
		results1.reserve(5);

		StringVector results2;
		results2.reserve(5);
		
		const CGraphmatFile* Gr = piGraphmatFile;
		bool bInFixedExpression = false;
		
		for (LineNo = 0; LineNo < Gr->GetTokensCount(); LineNo++)
		{
			// take a line from Graphematics
			char buffer[1000];
			Gr->GetGraphematicalLine(buffer, LineNo);
		 	string strProcess = buffer;

			//=====   do not lemmatize oborots with EXPR=Fixed!
			if (Gr->StartsFixedOborot(LineNo))
			{
				bInFixedExpression = true;
			};

			if (bInFixedExpression)
			{
				m_Items.push_back(strProcess);
				if (Gr->HasDescr(LineNo, OOb2))	
					bInFixedExpression = false;
				continue;
			};
			//=====   



			if (m_pLemmatizer->GetLanguage() == Gr->GetTokenLanguage(LineNo))
			{
				string InputWordStr = Gr->GetToken(LineNo);

				m_pLemmatizer->LemmatizeWordForPlmLines(InputWordStr, !Gr->HasDescr(LineNo, OLw), true, results);

				if (!IsFound(results) ) // not found or predicted
				{
					// if the word was not found in the dictionary 
					// and the word contains a hyphen 
					// then we should try to predict each parts of the hyphened word separately

					int hyph = InputWordStr.find("-");
					if (hyph != string::npos)
					{
						// try to lemmatize each parts without predictions
						string UpperWord = Gr->GetUppercaseToken(LineNo);
						string first_part = UpperWord.substr(0, hyph);
						string second_part = UpperWord.substr(hyph+1);
						m_pLemmatizer->LemmatizeWordForPlmLines(first_part, !Gr->HasDescr(LineNo, OLw), false, results1);

						/*
						 if the first part is equal to the second part  or the second part is an unimportant: Russian postfix
						 then we should use only the first part 
						*/
						if	(			(first_part == second_part)
								||		m_pLemmatizer->IsHyphenPostfix(second_part)
							)
							results = results1;
						else
						{
							m_pLemmatizer->LemmatizeWordForPlmLines(second_part, false, false, results2);
							if (IsFound(results1) && IsFound(results2))
								// if both words were found in the dictionary
								// then we should create a decart production
								CreateDecartProduction(results1, results2, results);

						}
						
							
					};
				};
				if (results.empty() )
				{
					
					m_Items.push_back(strProcess+ Format(" ??? %s ?? -1 0",Gr->GetUppercaseToken(LineNo)) );
				}
				else
					for( int i=0; i<results.size(); i++ )
					{
						string Line;
						if (i > 0) Line = 	"  ";
						Line +=  strProcess+" "+results[i];
						m_Items.push_back(Line);
					}
			}
			else
			{
				m_Items.push_back(strProcess);
			}
		}
	}
	catch (...)
	{

		return false;
	}
	return true;
}




bool CPlmLineCollection::SaveToFile(string filename) const
{
	try
	{
		FILE* fp = fopen (filename.c_str(),"wb");
		if (!fp) return false;
		for (int i = 0; i < m_Items.size(); i++)
			fprintf (fp, "%s\n",m_Items[i].c_str());
		fclose (fp);
	}
	catch(...)
	{
		return false;
	}
	return true;
}


