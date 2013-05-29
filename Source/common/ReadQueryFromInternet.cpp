#include "utilit.h"
#include "cgic.h"
extern void log(string t);
extern void print_error(const string &err);

bool HasIndexAtTheEnd(const string& s)
{
	if (s.length()<3) return false;
	if (!isdigit(s[s.length()-1]))  return false;
	if (!isdigit(s[s.length()-2]))  return false;
	return true;
}

string GetListOfSubcorpora (const string& StartSubCorpus, const string& EndSubCorpus)
{
	if (!HasIndexAtTheEnd(StartSubCorpus)) return "";
	if (!HasIndexAtTheEnd(EndSubCorpus)) return "";
	if (EndSubCorpus.length() != StartSubCorpus.length()) return "";
	size_t i = StartSubCorpus.length()-2;
	string CommonPart = StartSubCorpus.substr(0, i);;
	int start = atoi(StartSubCorpus.c_str()+i);
	int end = atoi(EndSubCorpus.c_str()+i);
	if (start >= end) 
	{
		cgiHeaderContentType("text/html");
		fprintf(cgiOut, "Invalid interval\n");
		exit(0);
	};
	string Result;
	for (; start < end; start ++)
	{
		Result += Format ("%s%02i",CommonPart.c_str(), start);
		if (start+1 != end)
			Result += ",";
	};

	Result = ":"+Result;
	return Result;

};



bool ReadFromHtml(char* CorporaName, char* SearchText, char* CleanSearchText, DWORD& StartHitNo,  int& ResultLimit, string& StartSubcorpus, string& EndSubcorpus)
{
	//_sleep(20000);
	char sStartSubcorpus[50], sEndSubcorpus[50];
	char ButtonType[30];
	cgiFormStringNoNewlines("CorporaName", CorporaName, 20);
	cgiFormStringNoNewlines("B2", ButtonType, 30);
	cgiFormStringNoNewlines("StartSubcorpus", sStartSubcorpus, 50);
	cgiFormStringNoNewlines("EndSubcorpus", sEndSubcorpus, 50);
	StartSubcorpus = sStartSubcorpus;
	EndSubcorpus = sEndSubcorpus;
	string SubcorporaList = GetListOfSubcorpora(StartSubcorpus, EndSubcorpus);
	
	
	string sButtonType =  ButtonType;
	
	EngMakeLower (sButtonType);
	if (sButtonType == "more")
	{
		char strHitNo[30];
		cgiFormStringNoNewlines("LastHitNo", strHitNo, 30);
		sscanf (strHitNo, "$HitNo%i",&StartHitNo);
		cgiFormStringNoNewlines("LastQuery", SearchText, 10240);
	}
	else
	{
		cgiFormStringNoNewlines("SearchText", SearchText, 10240);
		StartHitNo = 0;
	};

	strcpy (CleanSearchText, SearchText);

	int timeout = 300;
	char* s = strstr(CleanSearchText, "$limit");
	char* q = strstr(CleanSearchText, "$timeout");
	
	if (s)
	{
			ResultLimit = atoi(s+6);
			*s = 0;
	};

	if (q)
	{
			timeout = atoi(q+8);
			*q = 0;
	};

	{
		string r = CleanSearchText;
		Trim(r);
		if (r.empty())
		{
			print_error("Empty query!");
			log ("Empty query!");
			return false;;
		};
	};


	if (!SubcorporaList.empty())
		strcat (CleanSearchText, SubcorporaList.c_str());

	return true;
};

bool ReadFromArgument(char* CorporaName, char* SearchText, char* CleanSearchText, DWORD& StartHitNo,  int& ResultLimit, string& ConcordCfgFile, int& TimeOut)
{

	string Query = cgiQueryString;
	for (size_t i=0; i+3<Query.length(); i++)
		if (Query.substr(i, 3) == "%20")
				Query.replace(i, 3, " ");

	
	char sResultType[100];
	char sConcordCfgFile[512];
	int CountItems = sscanf (Query.c_str(), 
				"run_query %[^\x1]\x1%[^\x1]\x1%[^\x1]\x1%[^\x1]\x1%i %i %i", 
							CorporaName, SearchText, sResultType, sConcordCfgFile ,&StartHitNo, &ResultLimit, &TimeOut);
	if (CountItems != 7) 
		return false;

	ConcordCfgFile = sConcordCfgFile;

	
	string Q = SearchText;
	QPDecodeString(Q);
	strcpy(SearchText, Q.c_str());
	strcpy(CleanSearchText, Q.c_str());
	

	return true;
};
