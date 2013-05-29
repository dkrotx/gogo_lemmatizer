#include "rus_numerals.h"


const CNumeralToNumber NumeralToNumber[NumeralToNumberCount] = 
{
{"����","������","","","I",1}, 
{"�������","","","","",1.5}, 
{"���","������","������","����","II",2}, 
{"���","������","������","����","III",3}, 
{"������","���������","���������","�������","IV",4}, 
{"����","�����","��������","������","V",5}, 
{"�����","������","���������","�������","VI",6}, 
{"����","�������","��������","������","VII",7}, 
{"������","�������","����������","��������","VIII",8}, 
{"������","�������","����������","��������","IX",9 }, 
{"������","�������","����������","��������","X",10}, 
{"�����������","������������","���������������","","XI",11}, 
{"����������","�����������", "��������������","","XII",12}, 
{"����������","�����������", "��������������","","XIII",13}, 
{"������������","�������������","����������������","","XIV",14}, 
{"����������","�����������", "��������������","","XV",15}, 
{"�����������","������������", "���������������","","XVI",16}, 
{"����������","�����������", "��������������","","XVII",17},  
{"������������","�������������", "����������������","","XIII",18},  
{"������������","�������������", "����������������","","XIX",19}, 
{"��������","���������", "������������","","XX",20}, 
{"��������","���������", "������������","","XXX",30}, 
{"�����","���������", "","","XL",40}, 
{"���������","�����������", "��������������","","L",50},  
{"����������","������������", "���������������","","LX",60}, 
{"���������","�����������", "��������������","","LXX",70},  
{"�����������","�������������", "����������������","","LXXX",80}, 
{"���������","����������", "","","XC",90}, 
{"���","�����","","","C",100}, 
{"������","���������","","","CC",200}, 
{"������","���������","","","CCC",300}, 
{"���������","������������","","CD","I",400}, 
{"�������","���������", "","","D",500}, 
{"��������","����������", "","","DC",600}, 
{"�������","���������", "","","DCC",700}, 
{"���������","�����������", "","","DCCC",800}, 
{"���������","�����������","","","CM",900}, 
{"������","��������","","","M",1000}, 
{"�������","����������","","","",1000000},
{"��������","�����������","","","",1000000000}
};



string IntToStr (double i) 
{
	if (i == 1.5) return "1.5";
	char s[55];
	QWORD  _i = i;
	#ifdef WIN32
		_i64toa(_i, s, 10);
	#else
			sprintf (s, "%Ld", _i);
	#endif

	return s;
};

string FindByNumber(QWORD Number)
{
	for(int i = 0 ; i < NumeralToNumberCount ; i++ )
		if( Number == NumeralToNumber[i].m_Number)
			return NumeralToNumber[i].m_Cardinal;
	return "";
};


int IsAdverbRule(const string& Lemma)
{
	
	for(int i = 0 ; i < NumeralToNumberCount ; i++ )
		if (!strcmp (Lemma.c_str(), NumeralToNumber[i].m_Adverb))
			return NumeralToNumber[i].m_Number;

   return -1;
};


double GetCardinalNumeral(const string& word)
{
	for(int i = 0 ; i < NumeralToNumberCount ; i++ )
		if( word == NumeralToNumber[i].m_Cardinal )
			return NumeralToNumber[i].m_Number;
	return -1;
};

QWORD GetOrdinalNumeral(const string& word)
{
	for(int i = 0 ; i < NumeralToNumberCount; i++ )
		if( word == NumeralToNumber[i].m_Ordinal )
			return NumeralToNumber[i].m_Number;
	return -1;
};

QWORD GetNounNumeral(const string& word)
{
	for(int i = 0 ; i < NumeralToNumberCount; i++ )
		if( word == NumeralToNumber[i].m_Noun )
			return NumeralToNumber[i].m_Number;
	return -1;
};

