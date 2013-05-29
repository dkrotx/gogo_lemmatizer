// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Posmorphological Module (www.aot.ru)
// ==========  Copyright by Dmitry Pankratov, Alexey Sokirko (1999-2002)

#pragma warning(disable:4786) 
#include "../common/util_classes.h"
#include "PlmLine.h"

#ifdef DETECT_MEMORY_LEAK
	#ifdef _DEBUG
	#define new DEBUG_NEW

	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#endif
#endif


CPlmLine::CPlmLine ()
{
	m_bQuoteMarks = false;
	m_bFirstUpperAlpha = false;
	m_bToDelete = false;
	m_bFI1 = false;;
	m_bFI2 = false;
	m_bName = false;;
	m_bSent1 = false;
	m_bSent2 = false;
	m_bHyphenWord = false;
	m_bMorphName = false;
	m_bMorphSurname = false;
	m_Type = OTHER;
	m_Grammems = 0;
	m_bOborot1 = false;
	m_bOborot2 = false;
	m_Pos= UnknownPartOfSpeech;
	m_bRomanNumber = false;
	m_FilePosition = 0;
	m_TokenLengthInFile = 0;
	m_HomoWeight = "0";
	m_ParadigmId = "-1";
	m_MorphSign = 0;

};




// выдает смещение, начиная с которого стоит знак морфологического  словаря
// если такого знака нет, то выдается -1
// [-+][:alpha:\?][:alpha:\?]
int GetMorphSignPosition(const char* LineStr) 
{
	int LineLength= strlen(LineStr);
	for (int i = 0; i < LineLength - 3; i++)
	if( 		 ( (LineStr [i] == '+')			 || (LineStr [i] == '-') || (LineStr [i] == '?'))
			&&	 ( is_alpha((BYTE)LineStr [i+1])  || (LineStr [i+1] == '?') )
			&&	 ( is_alpha((BYTE)LineStr [i+2])  || (LineStr [i+2] == '?') )
		)   
		return i;

	return -1; 	
};


bool  IsHomonym(const char* str)
{ 
	return (str[0] == ' '); 
}


string GetWordForm(const string& LineStr)
{
	int ii = 0;
	if( IsHomonym(LineStr.c_str()) )
		ii = 2;
	int word_len = strcspn(LineStr.c_str() + ii," ");
	if (word_len == 0) return "";
	return LineStr.substr(ii, word_len);
};




bool CPlmLine::HasGraDescr(const char* GraphDescr) const
{
  int i =  m_GraphDescr.find(GraphDescr);

  if ( i == -1) return false;

  if (   (i != 0)  
	  && ((unsigned char)m_GraphDescr[i-1] != ' ')
	 )
	 return false;

  int len = strlen(GraphDescr);
  if (   (i+len != len - 1 )  
	  && ((unsigned char)m_GraphDescr[i+len] != ' ')
	 )
	 return false;

  return true;
};





void CPlmLine :: DeleteOb1() 
{
	m_bOborot1 = false;
	for (long i=0; i < m_GraphDescr.length(); i++)
	if (m_GraphDescr.substr(i,5) == "EXPR1")
	{
		m_GraphDescr.erase(i,5);
	}
	else
	if (m_GraphDescr.substr(i,7) == "EXPR_NO")
	{
		m_GraphDescr.erase(i,7);

		while	(			(i < m_GraphDescr.length())
					&&		(isdigit((unsigned char)m_GraphDescr[i]))
				)
		m_GraphDescr.erase(i, 1);
	};
};

void CPlmLine :: DeleteDescr(const char* Descr) 
{
	 long len =  strlen(Descr);
	 for (long i=0; i < m_GraphDescr.length(); i++)
	  if (m_GraphDescr.substr(i,len) == Descr)
	  {
		  m_GraphDescr.erase(i,len);
	  }

};
void CPlmLine :: DeleteOb2() 
{
	m_bOborot2 = false;
	DeleteDescr("EXPR2");
};


bool init_flag(string& GraphDescr, const char* flag_str)
{

  int i =  GraphDescr.find(flag_str);

  if ( i == string::npos) return false;

  if (   (i != 0)  
	  && ((unsigned char)GraphDescr[i-1] != ' ')
	 )
	 return  false;

  int len = strlen(flag_str);
  if (   (i+len != GraphDescr.length() )  
	  && ((BYTE)GraphDescr[i+len] != ' ')
	 )
	 return  false;

  GraphDescr.erase(i, len);
  return true;
};

bool CPlmLine :: LoadPlmLineFromString (string LineStr, bool bStartLine, const CAgramtab* pRusGramTab) 
{
	m_bToDelete = false;
	m_bQuoteMarks = false;
	
	if (!bStartLine)
	{
		m_bHomonym = (LineStr[0] == ' ');

		Trim(LineStr);
		SetWord(::GetWordForm(LineStr) );
	}
	else
	{
		m_bHomonym = false;
		SetWord( "" );

	};


	/*
		слово может превышать MaxGraLen, если оно дефисное
	*/
	long i = MaxGraLen;
	for (; i < LineStr.length(); i++)
		if (!isspace((unsigned char) LineStr[i]) == 0)
			break;

	for (; i < LineStr.length(); i++)
		 if (		(isdigit((unsigned char) LineStr[i]) == 0)
				&& (isspace((unsigned char) LineStr[i]) == 0)
				&& (((unsigned char)LineStr[i]) != '-')
			)
			break;
	string Address = LineStr.substr (MaxGraLen, i - MaxGraLen  -1);
	if (sscanf (Address.c_str(), "%i %i", 		&m_FilePosition, &m_TokenLengthInFile) != 2)
		return false;

	int MorphSignPos = GetMorphSignPosition(LineStr.c_str()+MaxGraLen);
	if (MorphSignPos == -1)
		MorphSignPos = LineStr.length();
	else
		MorphSignPos += MaxGraLen;
	m_GraphDescr = LineStr.substr (i, MorphSignPos - i);
	/* вставим пробел в начало, потому что часто ищут графету с пробелом в начале,
	например, " ЛЕ"*/
	m_GraphDescr = " " + m_GraphDescr;

	if (MorphSignPos != LineStr.length())
	{
		StringTokenizer tok(LineStr.c_str()+MorphSignPos," ");

		if (!tok() ) return false;
		string MorphSign  = tok.val();
		if (MorphSign.length() != 3) return false;
		m_MorphSign = MorphSign[0];
		m_CommonGramCode = MorphSign.substr(1);

		
		if (!tok() ) return false;
		m_Lemma = tok.val();
		if (m_Lemma.empty())  return false;
		
		if (!tok() ) return false;
		SetGramCodes ( tok.val(), pRusGramTab);

		if (!tok() ) return false;
		m_ParadigmId = tok.val();

		if (!tok() ) return false;
		m_HomoWeight = tok.val();

	};


	if (init_flag (m_GraphDescr, "RLE"))
		m_Type = LE;
	else
	if (init_flag (m_GraphDescr, "LLE"))
		m_Type = ILE;
	else
	if (init_flag (m_GraphDescr, "DC"))
		m_Type = NUM;
	else
	if (init_flag (m_GraphDescr, "DSC"))
		m_Type = NUM_CHAR;
	else
	if (init_flag (m_GraphDescr, "PUN"))
		m_Type = PUNCTUAT;
	else
		m_Type = OTHER;

	if (init_flag (m_GraphDescr, "Aa"))
		m_Register = UpLow;
	else
	if (init_flag (m_GraphDescr, "AA"))
		m_Register = UpUp;
	else
	{
		init_flag (m_GraphDescr, "aa");
		m_Register = LowLow;		
	};

	m_bFirstUpperAlpha =  (m_Register == UpUp) || (m_Register == UpLow);

	m_bFI1 = init_flag (m_GraphDescr, "FAM1");
	m_bFI2 = init_flag (m_GraphDescr, "FAM2");
	m_bName = init_flag (m_GraphDescr, "NAM?");
	m_bSent1 = init_flag (m_GraphDescr, "SENT1");
	m_bSent2 = init_flag (m_GraphDescr, "SENT2");
	int hyphen_occur = m_Word.find("-");
	m_bHyphenWord = (hyphen_occur != string::npos) && ( (m_Type == LE) ||(m_Type == ILE));

	m_bOborot1 = (m_GraphDescr.find("EXPR1") != string::npos);
	m_bOborot2 = (m_GraphDescr.find("EXPR2") != string::npos);
	m_bRomanNumber = is_roman_number(m_Word.c_str(), m_Word.length());
	
	if ((hyphen_occur != string::npos) && (hyphen_occur!=0))
	{
		// "Павла I-го" 
		// "I-го" - одно слово
		m_bRomanNumber = is_roman_number(m_Word.c_str(), hyphen_occur);
	};
	if (m_bRomanNumber)
	{
		m_Type = NUM;
		m_CommonGramCode = "";
		m_MorphSign = 0;
		m_ParadigmId = "";
	};
	Trim(m_GraphDescr);
	return true;

};



string CPlmLine :: GetStr ()  const
{

	string Result = m_Word;
	if (m_bHomonym) Result = "  "+Result;
	for (long i = m_Word.length(); i < MaxGraLen+1; i++)
		Result += " ";
    Result += Format (" %i %i ", m_FilePosition, m_TokenLengthInFile);

	if (m_Type == LE)
		Result +=  " RLE ";
	else
	if (m_Type == ILE)
		Result +=  " LLE ";
	else
	if (m_Type == NUM)
		Result +=  " DC ";
	else
	if (m_Type == NUM_CHAR)
		Result +=  " DSC ";
	else
	if (m_Type == PUNCTUAT)
		Result +=  " PUN ";
		

	if (   (m_Type == LE)
		|| (m_Type == ILE)
	   )
	{
		if (m_Register == LowLow)
			Result +=  " aa ";
		else
		if (m_Register == UpLow)
			Result +=  " Aa ";
		else
		if (m_Register == UpUp)
			Result +=  " AA ";
	};

	
	Result += m_GraphDescr + " ";

	if (m_bQuoteMarks)
		Result +=  " #ЗКВЧ ";

	if (m_bFI1) 
		Result +=  " FAM1 ";
	if (m_bFI2) 
		Result +=  " FAM2 ";
	if (m_bName) 
		Result +=  " NAM? ";
	if (m_bSent1) 
		Result +=  " SENT1 ";
	if (m_bSent2) 
		Result +=  " SENT2 ";

	if (m_MorphSign != 0)
	{
		Result += m_MorphSign+m_CommonGramCode + " " + m_Lemma + " " + m_GramCodes + " ";
		Result +=  m_ParadigmId + " " + m_HomoWeight;
	};

	return  Result;
};

void	CPlmLine :: SetGramCodes(const string& NewGramCodes, const CAgramtab* pRusGramTab)
{
	m_GramCodes = NewGramCodes;

	if	(		(m_GramCodes != "") 
			&&	(m_GramCodes != "??")
		)
	{
		m_Grammems = pRusGramTab->GetAllGrammems(m_GramCodes.c_str());
		m_Pos = pRusGramTab->GetPartOfSpeech(m_GramCodes.c_str());
	}
	else
	{
		m_Grammems = 0;
		m_Pos = 255;
	};

	if ( (m_CommonGramCode.length() == 2) && (m_CommonGramCode != "??") )
		m_Grammems |= pRusGramTab->GetAllGrammems(m_CommonGramCode.c_str());

};



bool CPlmLine::IsPartOfNonSingleOborot() const
{
	return m_bOborot1 !=  m_bOborot2;
};


bool	CPlmLine::IsFoundInMorphology() const
{
	return	(			(m_Type == LE)
				|| 		(m_Type == ILE)
			)
			&&	(		(m_MorphSign != '-') 
					&&	(m_MorphSign != '?')
				);
};


void	CPlmLine::SetWord(string s)
{
	m_Word = s;
	m_UpperWord = s;
	EngRusMakeUpper(m_UpperWord);
};

const string&  CPlmLine::GetWord() const
{
	return m_Word;
};
const string&  CPlmLine::GetUpperWord() const
{
	return m_UpperWord;
};

const string&  CPlmLine::GetCommonGramCode() const
{
	return m_CommonGramCode;
};

void  CPlmLine::SetMorphUnknown()
{
	m_CommonGramCode = "??";
	m_MorphSign = '-';
	m_ParadigmId = "-1";
};

void  CPlmLine::SetMorph(char MorphSign, const string& CommonAncode, const DWORD& ParadigmId)
{
	m_CommonGramCode = CommonAncode;
	m_MorphSign = MorphSign;
	m_ParadigmId = Format("%i",ParadigmId);
};

