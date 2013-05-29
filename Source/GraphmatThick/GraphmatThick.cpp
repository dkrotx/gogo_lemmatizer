// GraphmatThick.cpp : Defines the entry point for the console application.
//

#include "../common/util_classes.h"
#include "../GraphanLib/GraphmatFile.h"



void PrintUsageAndExit()
{
	printf ("GraphmatThick, Test for Dialing Graphematical Analysis(www.aot.ru)\n");
	printf ("Usage: GraphmatThick <language> <file> <opt>\n\n");
	printf ("where language can be Russian, English, or German\n");
	printf (" file is an text or html file  in  CP-1251 coding\n");
	printf ("    1. Russian -  CP-1251 coding\n");
	printf ("    2. German  -  ISO-8859-1 coding\n");
	printf (" Here are possible options:\n");
	printf (" if \"-gra\" is specified then the program prints .gra file with the g-table \n");
	printf (" if \"-xml\" is specified then the program prints .xml file with macrosyntax structure \n");
    printf (" if \"-sents\" is specified then the program prints .sents file with one sentence per line\n");
	printf (" if \"-abbrs\" is specified then the program prints .abbrs file with one abbreviation per line\n");
	printf (" if \"-disable-sent-breaker\" is specified then the program disabled sentence breaker\n");
	printf (" if \"-use-paragraph-tag\" is specified then the program considers \"</p>\" as  a paragraph delimeter\n");
	
	
	exit(1);
	
};

int main(int argc, char* argv[])
{

	try 
	{
		if (argc  < 3) 
			PrintUsageAndExit();
		MorphLanguageEnum Langua;
		if (!GetLanguageByString (argv[1], Langua))
		{
			printf ("False language - \"%s\"\n",argv[1]);
			return 1;
		};

		bool bWriteXml = false;
		bool bWriteGra = false;
		bool bSentsPrint = false;	
		bool bAbbrsPrint = false;
		bool bEnableSentenceBreaker = true;
		bool bUseParagraphTagToDivide = false;
		string FileName;
		for (size_t i=3; i<argc; i++)
		{
			string s = argv[i];
			EngMakeLower(s);
			if (		(s == "-help") 
					||	(s == "--help") 
					||	(s == "/?") 
				)
				PrintUsageAndExit();
			else
			if (s == "-xml")
				bWriteXml = true;
			else
			if (s == "-gra")
				bWriteGra = true;
			else
			if (s == "-sents")
				bSentsPrint = true;
			else
			if (s == "-abbrs")
				bAbbrsPrint = true;
			else
			if (s == "-disable-sent-breaker")
				bEnableSentenceBreaker = false;
			else
			if (s == "-use-paragraph-tag")
				bUseParagraphTagToDivide = true;
			else
				PrintUsageAndExit();

		};

		
		CGraphmatFile Graphan;
		Graphan.m_Language = Langua;
		if (!Graphan.LoadDicts())
		{
			printf ("Cannot load dictionaries \n");
			printf ("Error: %s\n", Graphan.GetLastError().c_str());
			return 1;
		};
	
		Graphan.m_bWriteGraFile = bWriteGra;
		//Graphan.m_bUseParagraphTagToDivide = true;
		Graphan.m_bWriteXmlMacSyn = bWriteXml;
		Graphan.m_bSentBreaker = bEnableSentenceBreaker;
		Graphan.m_bUseParagraphTagToDivide = bUseParagraphTagToDivide;
		
	
		Graphan.m_bUseIndention = true;
		printf ("Loading file %s\n", argv[2]);
		CMyTimeSpanHolder  Profiler;
		Profiler.StartTimer("LoadFileToGraphan",1);
		if (!Graphan.LoadFileToGraphan(argv[2]))
		{
			printf ("Cannot load file %s \n", argv[2]);
			printf ("Error: %s\n", Graphan.GetLastError().c_str());
			return 1;
		};
		Profiler.EndTimer("LoadFileToGraphan");
		printf ("%s ", Profiler.GetStrRepresentation().c_str());
		int Count = Graphan.GetUnits().size();
		if (bSentsPrint)
		{
			string FileName = MakeFName (argv[2],"sents");
			printf ("Creating %s\n", FileName.c_str());
			FILE * sents = fopen (FileName.c_str(), "w");
			if (!sents)
			{
				printf ("cannot open sents file %s\n",FileName.c_str()),
				exit(1);
			};
			for (size_t i=1; i < Count; i++)
			{
				string s = Graphan.GetToken(i);
				if (!Graphan.GetUnit(i).IsSoft())
					fprintf (sents,"%s ",s.c_str());
				if (Graphan.HasDescr(i,OSentEnd))
					fprintf (sents, "\n",s.c_str());

			};
			fclose (sents);
		};
		if (bAbbrsPrint)
		{
			string FileName = MakeFName (argv[2],"abbrs");
			printf ("Creating %s\n", FileName.c_str());
			FILE * abbrs = fopen (FileName.c_str(), "w");
			if (!abbrs)
			{
				printf ("cannot open sents file %s\n",FileName.c_str()),
				exit(1);
			};
			bool bAbbrStart	 = false;
			for (size_t i=1; i < Count; i++)
			{
				if (Graphan.HasDescr(i,OAbbr1))
					bAbbrStart = true;

				if (bAbbrStart)
				{
					string s = Graphan.GetToken(i);
					if (!Graphan.GetUnit(i).IsSoft())
						fprintf (abbrs,"%s ",s.c_str());
					
					if (Graphan.HasDescr(i,OAbbr2))
					{
						bAbbrStart = false;
						fprintf (abbrs,"\n");
					};
				};

			};
			fclose (abbrs);
		};
				
		for (size_t i=1; i < Count; i++)
		{
			string s1 = Graphan.GetUppercaseToken(i);
			string s2 = Graphan.GetToken(i);
			RmlMakeUpper(s2, Graphan.m_Language);
			assert (s1 == s2);
		};

		printf ("Found Units %u\n", Count);

		/*for (size_t i=0; i < 256; i++)
			if (isdelim(i))
				if (i > 32)
					printf ("1, // %c\n",(BYTE)i);
				else
					printf ("1, // non-printable\n",(BYTE)i);
			else
					printf ("0, \n");*/

		return 0;
	}
	catch (CExpc C)
	{
		printf ("An exception occurred:%s!\n", C.m_strCause.c_str());
	}
	catch (...)
	{
		printf ("An exception occurred!\n");
		
	};
	
	return 1;
}

