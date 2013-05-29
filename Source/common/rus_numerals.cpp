#include "rus_numerals.h"


const CNumeralToNumber NumeralToNumber[NumeralToNumberCount] = 
{
{"ндхм","оепбши","","","I",1}, 
{"онкрнпю","","","","",1.5}, 
{"дбю","брнпни","бдбнел","дбне","II",2}, 
{"рпх","рперхи","брпнел","рпне","III",3}, 
{"вершпе","вербепрши","бвербепнл","вербепн","IV",4}, 
{"оърэ","оърши","боърепнл","оърепн","V",5}, 
{"ьеярэ","ьеярни","бьеярепнл","ьеярепн","VI",6}, 
{"яелэ","яедэлни","бяелепнл","яелепн","VII",7}, 
{"бняелэ","бняэлни","ббняэлепнл","бняэлепн","VIII",8}, 
{"дебърэ","дебърши","бдебърепнл","дебърепн","IX",9 }, 
{"деяърэ","деяърши","бдеяърепнл","деяърепн","X",10}, 
{"ндхммюджюрэ","ндхммюджюрши","бндхммюджюрепнл","","XI",11}, 
{"дбемюджюрэ","дбемюджюрши", "бдбемюджюрепнл","","XII",12}, 
{"рпхмюджюрэ","рпхмюджюрши", "брпхмюджюрепнл","","XIII",13}, 
{"вершпмюджюрэ","вершпмюджюрши","бвершпмюджюрепнл","","XIV",14}, 
{"оърмюджюрэ","оърмюджюрши", "боърмюджюрепнл","","XV",15}, 
{"ьеярмюджюрэ","ьеярмюджюрши", "бьеярмюджюрепнл","","XVI",16}, 
{"яелмюджюрэ","яелмюджюрши", "бяелмюджюрепнл","","XVII",17},  
{"бняелмюджюрэ","бняелмюджюрши", "ббняелмюджюрепнл","","XIII",18},  
{"дебърмюджюрэ","дебърмюджюрши", "бдебърмюджюрепнл","","XIX",19}, 
{"дбюджюрэ","дбюджюрши", "бдбюджюрепнл","","XX",20}, 
{"рпхджюрэ","рпхджюрши", "брпхджюрепнл","","XXX",30}, 
{"янпнй","янпнйнбни", "","","XL",40}, 
{"оърэдеяър","оърхдеяърши", "боърхдеяърепнл","","L",50},  
{"ьеярэдеяър","ьеярхдеяърши", "бьеярхдеяърепнл","","LX",60}, 
{"яелэдеяър","яелхдеяърши", "бяелхдеяърепнл","","LXX",70},  
{"бняелэдеяър","бняэлхдеяърши", "ббняэлхдеяърепнл","","LXXX",80}, 
{"дебъмнярн","дебъмнярши", "","","XC",90}, 
{"ярн","янрши","","","C",100}, 
{"дбеярх","дбсуянрши","","","CC",200}, 
{"рпхярю","рпеуянрши","","","CCC",300}, 
{"вершпеярю","вершпеуянрши","","CD","I",400}, 
{"оърэянр","оърхянрши", "","","D",500}, 
{"ьеярэянр","ьеярхянрши", "","","DC",600}, 
{"яелэянр","яелхянрши", "","","DCC",700}, 
{"бняелэянр","бняэлхянрши", "","","DCCC",800}, 
{"дебърэянр","дебърхянрши","","","CM",900}, 
{"ршяъвю","ршяъвмши","","","M",1000}, 
{"лхккхнм","лхккхнммши","","","",1000000},
{"лхккхюпд","лхккхюпдмши","","","",1000000000}
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

