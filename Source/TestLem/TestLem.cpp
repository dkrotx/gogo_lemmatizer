#include "../LemmatizerLib/Lemmatizers.h"                                                                                           
#include "../LemmatizerLib/Paradigm.h"     

#include <string>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <sys/time.h>

class CLemmatizer *pLemmatizer;
std::vector<CFormInfo> vParadigms;

inline bool strNormalize(std::string& str)
{
	bool bProcess = true;
	std::string::iterator it = str.begin();
	for (; it != str.end(); it++)
	{
		char &c = *it;
		bool bRus = false, bEng = false;
		if ((c >= 'à' && c <= 'ÿ') ||
			(c >= 'À' && c <= 'ß') ||
			 c == '-')
		{
			bRus = true;
		}
		else if ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z'))
		{
			bEng = true;
		}
		
		if (bEng || bRus)
		{
			c &= ~0x20;
		}
		
		if (!bRus)
		{
			bProcess = false;
		}
	}
	return bProcess;
}

inline int lemmatize(std::string &sWord)
{
	return 0;
}

timeval tStart, tEnd;

int main()
{
	int wordCount = 0;

	pLemmatizer = new CLemmatizerRussian();
//	pLemmatizer->m_bUsePrediction=false;
	if (!pLemmatizer->LoadDictionariesRegistry())
	{
		printf ("can't load lemmatizer dictionary\n");
		return 0;
	}

	std::string str;
	float fSumTime = 0;
	char b[256];
	while (!cin.eof())
	{
		cin >> str;

		bool bProcess = strNormalize(str);
		if (bProcess)
		{
			vParadigms.clear();
			wordCount++;
			gettimeofday(&tStart, NULL);
//			pLemmatizer -> CreateParadigmCollection(false, str, true, vParadigms);
			pLemmatizer->GetAllAncodesAndLemmasQuick(str, false, b, 255);
//			printf("%s: %s\n", str.c_str(), b);
			gettimeofday(&tEnd, NULL);
			float fSecs = float (tEnd.tv_sec - tStart.tv_sec) + float (tEnd.tv_usec - tStart.tv_usec) / 1000000.0f;
			fSumTime+= fSecs;
		}	
	}	
	printf("%d words per second\n", long(wordCount / fSumTime));
}




