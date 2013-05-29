// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1996-2001)

#include "StdGraph.h"
#include  "../common/Graspace.h"
#include  "graline.h"
#include  "GraphmatFile.h"
#include "GraphanDicts.h"

const int NumberOfGraphematicalDescriptors  = 57;
const int ODesLen    = 8;                   /* the length of unit's descriptor */

const char DesArray[NumberOfGraphematicalDescriptors][ODesLen] = 
{
		     /*the first position : */ //0-4
    	     "BEG","RLE", "LLE","DEL","PUN",
		     "DC", "DSC","EOT","_UNK_", // 5-8
		     /*the second position :*/
		     "SPC","HYP","EOLN","EOS","EOP", //9-13
		     "PLP","AA", "aa", "Aa", // 14-17
			 "NAM?", "OPN","CLS", // 18-20
			 "EMSYM", "BUL", "INDENT", // 20-23
			 "PASS", "DPUN", "PAR_SYM", // 24-26

             // macrosyntax
             "CS?","CS","QUA","CS_PRNT","HDNG","CS_AUX","DOC", // 27-33

		     // pair descriptors : 
			 // if d is the first descriptor, then it should be always even, and d+1 should  be the corresponding second descriptor
		     "EXPR1","EXPR2", // 34-35
		     "FAM1","FAM2", // 36-37
			 "HP1","HP2", // 38-29
             "RE1",  "RE2", // 40-41
			 "DT1","DT2", // 42-43
             "PPAR1", "PPAR2", // 44-45
			 "EA1", "EA2", // 46-37
			 "ABB1", "ABB2", // 48-49
			 "SENT1", "SENT2", // 50-51
			 "KEY1", "KEY2", // 52-53
			 "GDC1", "GDC2" // 54-55 // German divided compounds

};


bool IsFirstMemberOfPairDesciptor(Descriptors d) 
{
	return		(d%2 == 0) 
			&&	(d >= (int)OOb1) 
			&&  (d < NumberOfGraphematicalDescriptors);
};
bool IsSecondMemberOfPairDesciptor(Descriptors d) 
{
	return		(d%2 != 0) 
			&&	(d >= (int)OOb2)
			&&  (d < NumberOfGraphematicalDescriptors);
};

Descriptors GetSecondMemberByTheFirst(Descriptors d) 
{
	assert   (IsFirstMemberOfPairDesciptor(d));
	return (Descriptors)(d+1);
};
Descriptors GetFirstMemberByTheSecond(Descriptors d) 
{
	assert   (IsSecondMemberOfPairDesciptor(d));
	return (Descriptors)(d-1);
};

bool GetDescriptorStr(int DescriptorNo, string& Result)
{
	if (DescriptorNo >= NumberOfGraphematicalDescriptors)
		return false;

	Result = DesArray[DescriptorNo];
	return true;
};

const char* GetDescriptorStr(int DescriptorNo)
{
	assert (DescriptorNo < NumberOfGraphematicalDescriptors);
	return DesArray[DescriptorNo];
};



/*
 all descriptors which end a text period like a sentence or a paragraph
*/
bool IsEndTextPeriodDescriptor (Descriptors d)
{
	return     (d == CS_Undef)
			|| (d == CS_Simple)
			|| (d == CS_Parent)
			|| (d == CS_Quasi)
			|| (d == CS_Heading)
			|| (d == CS_Explan)
			|| (d == OSentEnd);
};


CGraLine::CGraLine()  
{
	m_Status = 0;
	unit = NULL;
	m_Descriptors = 0;
	ulen = slen = 0;
	m_InputOffset = 0;
};

bool CGraLine :: HasMacroSyntaxDelimiter () const
{
	return	(		m_Descriptors 
				&	(		_QM(CS_Undef )
						|	_QM( CS_Simple )
						|	_QM( CS_Parent )
						|	_QM( CS_Heading )
						|	_QM( CS_Explan )
				    ) 
			) > 0;
};

bool CGraLine :: IsWordOrNumberOrAbbr() const
{
	return 
	(		m_Descriptors 
		&	(		_QM ( ORLE )
				|	_QM ( OLLE )
				|	_QM ( ODg )
				|	_QM ( ODgCh )
				|	_QM ( OAbbr1 )
			)
	) != 0;
}


int get_descriptor_len ( const char * s)
{

	for (int i = 0; i< NumberOfGraphematicalDescriptors; i++)
	{
		size_t len = strlen (DesArray[i]);

		if (strncmp(s,DesArray[i],len) == 0)

		return strlen(DesArray[i]);
	}

	return 0;
}
 


bool CGraLine :: IsSingleSpaceToDelete() const
{
	return 		(GetTokenLength() == 1)
			&&  (GetToken()[0] == ' ') // we delete only space, leaving alone tabulations  
			&&	(m_Status == stSpace); // this line means that there is no other meaning for this 
										// space, for example, it prevents to consider a converted "</p" to be deleted 
};




void CGraLine :: MakeSpaces(size_t SpacesLength)
{
	m_Descriptors = _QM(ODel) | _QM(OSpc);
	m_Status = stSpace;
	ulen = SpacesLength;
	slen = SpacesLength;
	memset(const_cast<char*>(unit), ' ', SpacesLength);
};


int  CGraLine :: ToInt () const 
{	
	char s[100];
	strncpy (s,unit,ulen);
	s[ulen] = 0;
	return atoi (s);
};


bool CGraLine::IsNotPrint  () const 
{
	return (m_Status & stNotPrint) != 0;
};
bool CGraLine::IsEnglishName () const 
{
	return (m_Status & stEnglishName) != 0;
};
bool CGraLine::IsIdent () const 
{ 
	return (m_Status & stIdent) != 0; 
};;

bool CGraLine::IsGrouped  () const 
{ 
	return (m_Status & stGrouped) != 0;
};
bool CGraLine::IsAbbreviation  () const 
{ 
	return (m_Status & stAbbreviation) != 0;
};
bool CGraLine::IsParagraphChar () const  
{ 
	return (m_Status & stParagraphChar) != 0;
};
bool CGraLine::IsPageBreak() const  
{ 
	return (m_Status & stPageBreak) != 0;
};
bool CGraLine::IsParagraphTag () const  
{ 
	return (m_Status & stParagraphTag) != 0;
};

bool CGraLine::IsKeyModifier()  const 
{ 
	return (m_Status & stKeyModifier) != 0;
};
bool CGraLine::IsChar (int c) const  
{ 
	return (ulen == 1) && (unit[0] == c);
};
bool CGraLine::IsAsterisk () const 
{ 
	return   IsChar((unsigned char)'*') || IsChar(149);
};

bool CGraLine::HasSingleSpaceAfter()  const { return (m_Status & stSingleSpaceAfter) > 0;};


bool CGraLine::IsString (const char* s) const 
{ 
	return (s[ulen] == 0) && (!strncmp(unit,s,ulen));
};
void CGraLine::SetSpace ()  
{
	m_Status |=  stSpace;
};
void CGraLine::SetEOLN  ()  
{ 
	m_Status |=  stEOLN;
};
void CGraLine::SetNotPrint()  
{ 
	m_Status |=  stNotPrint;
};
void CGraLine::SetPunct ()  
{ 
	m_Status |=  stPunct;
};

void CGraLine::SetParagraphChar()  
{ 
	m_Status |=  stParagraphChar;
};
void CGraLine::SetParagraphTag()  
{ 
	m_Status |=  stParagraphTag;
};
void CGraLine::SetKeyModifier()  
{ 
	m_Status |=  stKeyModifier;
};
void CGraLine::SetSingleSpaceAfter()  
{
	m_Status |=  stSingleSpaceAfter;
};
void CGraLine::SetIdent()  
{ 
	m_Status |=  stIdent;
};
void CGraLine::SetPageBreak()  
{ 
	m_Status |=  stPageBreak;
};
void CGraLine::SetEnglishName () 
{ 
	m_Status |=  stEnglishName;
};

void CGraLine::DelDes(Descriptors d)  
{ 
	m_Descriptors &=  ~( _QM(d) );
};


void CGraLine::SetDes(Descriptors d)  
{
	m_Descriptors |= _QM(d);
};





void CGraLine::AddStatus(WORD add_state)
{
	m_Status |= add_state;
};

void CGraLine::AddLength(const CGraLine& L)
{
	ulen +=  L.GetTokenLength();
	slen += L.GetScreenLength();
};


void CGraLine::SetToken(const char* s)
{
	unit = s;
};



bool IsEnglPrefix (const char* s, size_t len)
{
	return     !CompareWithoutRegister(s, "re", 2, morphEnglish) 
		    || !CompareWithoutRegister(s, "pre", 3, morphEnglish) 
			|| !CompareWithoutRegister(s, "non", 3, morphEnglish);
};




// выдает длину отрезка  s до первого разделител€ и не больше  maxlen
size_t LengthUntilDelimeters (const char *s, size_t maxlen)
{
	size_t i = 0;

	//for (i=0; i<maxlen && !WordDelims[(BYTE) s[i]]; i++);
	for (i=0; i<maxlen && !is_word_delim((BYTE) s[i]); i++);
	

	return i;
}




// читает из буфера b в структуру L  
size_t CGraLine::ReadWord (size_t Offset, const CGraphmatFile* G, DWORD& PageNumber)
{
	PageNumber = 0xffffffff;
	const char *s;
	const char* In = (const char*)&(G->GetInputBuffer()[0]);
	char *Out = const_cast<char*>(unit);
	BYTE len;
	ulen = slen = 0;
	

	m_InputOffset = Offset;


	/*   If Carriage Return occurs ...*/
	if (In[Offset] == '\r')
	{
		if (In[Offset+1] != '\n')
		{
			SetNotPrint();
			Out[0] = 0;
			Offset++;
			slen = 1;
			ulen = 1;
		}
		else
		{
			do
			{
				Out[ulen] = '\r';
				Out[ulen+1] = '\n';
				if (In[Offset+1] != '\n') break;
				ulen += 2;
				slen++;
				Offset+=2;
			}
			while ((In[Offset]=='\r') && (ulen < MaxGraLen));
			SetEOLN  ();
		}
	}
	else
	if (In[Offset] == '\n')
	{
		do
		{
			Out[ulen] = '\n';
			ulen += 1;
			Offset++;
			slen++;

		}
		while ((In[Offset] == '\n') && (ulen < MaxGraLen));
		SetEOLN  ();
	}
	else
	/* if it is a "</p>" (comes  from html) */
	if (		G->m_bUseParagraphTagToDivide
			&&	!strncmp(In+Offset,"</p>",4)
	)
	{
		memset (Out, ' ', 4);
		ulen = 4;
		slen = 1;
		SetSpace();
		SetParagraphTag();
		assert (IsParagraphTag());
		Offset += 4;
	}
	else
	// &nbsp
	if (		!strncmp(In+Offset,"&nbsp;",6) 
			||	!strncmp(In+Offset,"&NBSP,",6) 
	)
	{
		
		while	(			!strncmp(In+Offset,"&nbsp;",6) 
						||	!strncmp(In+Offset,"&NBSP,",6)
				)
		{
			if (ulen+6 >= MaxGraLen) break;
			memset (Out+ulen, ' ', 6);
			ulen += 6;
			slen++;
			Offset += 6;
			
		};
		SetSpace();

	}
	else
	// <br>
	if (		!strncmp(In+Offset,"<br>;",4) 
			||	!strncmp(In+Offset,"<BR>,",4) 
	)
	{
		Out[ulen] = '\n';
		ulen += 1;
		slen += 1;
		SetEOLN  ();
		Offset += 4;
	}
	else
	// <pb 1>
	if	(		!strncmp(In+Offset,"<pb ",3) 
			&&	isdigit((BYTE)In[Offset+4])
		)
	{
		sscanf(In+Offset+4, "%u", &PageNumber);
		


		SetSpace();
		SetPageBreak();

		memset (Out, ' ', 4);
		ulen += 4;
		slen += 4;
		Offset += 4;
		while	(			isdigit(In[Offset]) 
						||	isspace(In[Offset])
				)
		{
			Out[ulen] = ' ';
			ulen ++;
			slen ++;
			Offset++;
		};
		if (In[Offset] == '>')
		{
			Out[ulen] = ' ';
			ulen ++;
			slen ++;
			Offset++;
		}
	}
	else
	/*  if TCP/IP occurs ..*/
	if (G->m_pDicts->FindInIdents ( In+Offset, len ))
	{
		slen = len;
		ulen = len;
		memcpy(Out,In+Offset,ulen);
		Offset += ulen;
		SetIdent  ();
	}
	else
	/*  if a Bracket occurs ..*/
	if (isbracket((BYTE)In[Offset]))
	{
		*Out = In[Offset];
		slen = ulen=1;
		Offset++;
		SetPunct();
	}
	else
	/* if a Space or Tabulation occurs ... */
	if (isnspace((BYTE)In[Offset]))
	{
		for (;
				isnspace (In[Offset]) && ulen <MaxGraLen;
				ulen ++, slen += (In[Offset] == '\t') ? G->m_TabSize : 1, Offset++
			)
			Out[ulen] = In[Offset];

		SetSpace();
	}
	/*else
		if (	(G->m_Language == morphEnglish)
			&&	(In[Offset] == '\'') 
			&& (		(In[Offset+1] == 's')  
					||	(In[Offset+1] == 'S')
				) 
			&& (Offset>0)
			&& is_english_alpha((BYTE)In[Offset-1])
		)
	{
		Out[ulen] = In[Offset];
		Out[ulen+1] = In[Offset+1];
		ulen =2;
		slen = 2;
		Offset+=2;
		
	}*/
	else 
	/*
	последовательность из восклицательных и вопросительных знаков 
	(используетс€ как конец предложени€) 
	*/
	if (		((BYTE)In[Offset] == '?')  
			||	((BYTE)In[Offset] == '!') 
		)
	{
		for (	int ch1 = In[Offset];
				(		(In[Offset] == '?')  
					||	(In[Offset] == '!') )
				&& (ulen < MaxGraLen);
				Offset++,
				slen++,ulen++
			)
			Out[ulen] = In[Offset];

		SetPunct();
	}
	else
	/* if it is a hard delimeter */
	if (ispunct((BYTE)In[Offset]) || is_pseudo_graph((BYTE)In[Offset]))
	{
		for (	int ch1 = In[Offset];
				(In[Offset] == ch1) && (ulen < MaxGraLen);
				Offset++,
				slen++,ulen++
			)
		Out[ulen] = ch1;

		SetPunct();
	}

	else
	/*  If it is not printable symbol */
	if (		((BYTE) In[Offset] < 32)
			||	((BYTE) In[Offset] == cIonChar)
			||	((BYTE) In[Offset] == cNumberChar)
			||	((BYTE) In[Offset] == cPiChar)
			||	((BYTE) In[Offset] == cCompanyChar)
			||	((BYTE) In[Offset] == cPseudoColon)
		)
	{
		Out[0] = ' ';
		SetNotPrint();

		if ((BYTE)In[Offset] == cParagraph)
			SetParagraphChar();	 

		Offset++;
		slen = 1;
		ulen = 1;


	}
	else
	/* If a Word occurs ...*/
	{
		
		int WordLength;

		bool bCanBeSpaceDelimitedWord = 		
								( (G->GetInputBuffer().size() - Offset) > 2) 
							&&	is_spc_fill(In[Offset+1]);

		s =  bCanBeSpaceDelimitedWord ?  G->m_pDicts->SearchSpace (In+Offset,&WordLength) : NULL;

		if (s!=NULL)
		{
			ulen = strlen(s);
			strncpy(Out,s,ulen);
			Offset += WordLength;
		}
		else
		{
			const char* CurrIn = In+Offset; 
			WordLength = LengthUntilDelimeters (CurrIn,MaxGraLen);
			
			if (G->m_pDicts->IsKeyModifier(CurrIn, WordLength))
				SetKeyModifier();	 


			// ѕоследовательности "PR/CMA" "TCP/IP" "M/B" должны быть одним словом 
			if (WordLength > 0)
				if ( G->is_latin_alpha((BYTE)CurrIn[WordLength-1]) )
					if (Offset + WordLength + 1 < G->GetInputBuffer().size())
						if ((BYTE)CurrIn[WordLength] == '/')
							{
								size_t t = LengthUntilDelimeters (CurrIn+WordLength+1,MaxGraLen - WordLength -1);
								if (t>0) // do not consider "Israel/" as one word 
										  //  it should be something after "/" in order to consider it a word	
									WordLength += t+1;
							};

		// don't divide by hyphens, when there is no spaces between words
		// Examples: "ћ»-2", "»Ћ-67", "test-west"
		
		 if (!IsKeyModifier())
		   if (WordLength > 0)
				while  (		(Offset + WordLength + 2 < G->GetInputBuffer().size())
							&& ((BYTE)CurrIn[WordLength] == '-')
							&&	(		isdigit ((BYTE)CurrIn[WordLength+1]) 
									||	(G->m_pDicts->m_bCreateHyphenedWords && is_alpha((BYTE)CurrIn[WordLength+1]) )
								)
						)
				{
					size_t t = LengthUntilDelimeters (CurrIn+WordLength+1,MaxGraLen - WordLength -1);
					if (WordLength +t+1 >= MaxGraLen)
						break;
					WordLength += t+1;
				}




		// последовательность N% считаем за один символ (обозначает номер)
		if (WordLength == 1)
			if ((BYTE)CurrIn[WordLength-1] == 'N')	
				if (Offset + WordLength < G->GetInputBuffer().size())
					if ((BYTE)CurrIn[WordLength] == '%')
						{
							WordLength++;
						};

		int SingleQuoteInd = -1;

		if (Offset + WordLength < G->GetInputBuffer().size())
			if	(		((BYTE)CurrIn[WordLength]==39)
					&&	is_alpha((BYTE)CurrIn[WordLength-1])
					&&	is_alpha((BYTE)CurrIn[WordLength+1])
				)
		{
			SingleQuoteInd = WordLength;
			size_t t = LengthUntilDelimeters (CurrIn+WordLength+1,MaxGraLen - WordLength -1);
			WordLength += t+1;
		};

		if (Offset + WordLength < G->GetInputBuffer().size())
			if ((BYTE)CurrIn[WordLength] == '-')
				if (IsEnglPrefix (CurrIn, WordLength))
				{
					size_t t = LengthUntilDelimeters (CurrIn+WordLength+1,MaxGraLen-WordLength - 1);
					WordLength += t+1;
				};

		if (Offset + WordLength >=  G->GetInputBuffer().size())
			WordLength = G->GetInputBuffer().size() -  Offset;

		strncpy(Out,CurrIn,WordLength);

		// обработка твердого знака, который иногда пишетс€ с помощью кавычки
		// типа "об'€вление" перекодируем в объ€вление
		if (G->m_Language == morphRussian)
			if (SingleQuoteInd != -1)
				Out[SingleQuoteInd] = (BYTE)'ъ';

		slen = ulen = WordLength;
		Offset += WordLength;
	}
	};
	return Offset;  
 }

