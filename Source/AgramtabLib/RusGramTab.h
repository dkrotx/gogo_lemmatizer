// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Lemmatizer (www.aot.ru)
// ==========  Copyright by Alexey Sokirko

#ifndef __RUSGRAMTAB_H_
#define __RUSGRAMTAB_H_


#include "agramtab_.h"       
#include "rus_consts.h"       

/////////////////////////////////////////////////////////////////////////////

const unsigned int  StartUp =  0xC0E0; //��
const unsigned int  EndUp = 0x10000;  //��
const unsigned int MaxGrmCount  = EndUp -StartUp; // // 16159  (16 ��) 
const BYTE rPartOfSpeechCount = 22;
// �� ������ ���� �������� ����� ������� 
const char rPartOfSpeeches[rPartOfSpeechCount][20] = 
{	"�",  // 0
	"�", // 1
	"�", // 2
	"��", // 3
	"��-�", // 4
	"��-�����", // 5
	"����", // 6
	"����-�", // 7
	"�", // 8
	"�����", //9 
	"�����", // 10
	"����", // 11
	"����", // 12
	"����", // 13
	"�����",// 14
	"����", // 15
	"����", // 16
	"��_����",  // 17
	"���������", //18
	"������������", //19
	"��_���������", // 20
	"���������"  //21
};

const short GrammemsCount = 43;
const char Grammems[GrammemsCount][10] = { 
					// 0..1
				   	"��","��",
					// 2..7
					"��","��","��","��","��","��",
					// ��� 8-11
					"��","��","��","��-��",
					// 12
					"��",
					// 13..15
					"���","���","���",
					// 16..18
					"1�","2�","3�",	
					// 19
					"���",
					// 20..21
					"��","��",	
					// 22
					"�����",
					// 23..24
					"��","��",	
					// 25..26
					"��","��",
					// 27..28
					"���","���",
					// 29-31
					"0", "����", "���",
					// 32-33
					"���", "���",
					// 34-35
					"���", "����",
					// 36-37 (�������)
					"����", "�����",
					// 38..39
					"���","���",
					// 40
					"����",
					// 41,42
					"����", "���"
					};




const int rClauseTypesCount = 12;
const char rClauseTypes [rClauseTypesCount][30] = 
{
	"��_����", 
	"���",
	"��_���",
	"��_����",
	"�����",
	"���",	 
	"���",	
	"����",
	"����",
	"���",
	"�����",
	"�����"
};




class CRusGramTab : public CAgramtab{
	public:
	CAgramtabLine*  Lines[MaxGrmCount];
	CRusGramTab();
	~CRusGramTab();

    BYTE			GetPartOfSpeechesCount () const {return rPartOfSpeechCount;};
	const char*		GetPartOfSpeechStr(BYTE i) const {return rPartOfSpeeches[i];};
	size_t			GetGrammemsCount()  const {return GrammemsCount;};
	const char*		GetGrammemStr(size_t i) const {return Grammems[i];};
	bool			IsAdditionalGrammem (const char* s) const;

	size_t GetMaxGrmCount() const {return MaxGrmCount;}
	;
	CAgramtabLine*& GetLine(size_t LineNo) {return Lines[LineNo];}
	const CAgramtabLine* GetLine(size_t LineNo) const {return Lines[LineNo];}

	size_t s2i(const char * s )  const
	{ 
		return  (unsigned char) s[0]*0x100+(unsigned char) s[1] - StartUp;
	};

	string i2s(WORD i) const
	{ 
		i += StartUp;
		char res[3];

		res[0] = (i >> 8);
		res[1] = (0xFF & i);
		res[2] = 0;
		return  res;
	};

    bool  ProcessPOSAndGrammems (const char* tab_str, BYTE& PartOfSpeech,  QWORD& grammems) const;

	

	const char* GetRegistryString() const 
	{
		return "Software\\Dialing\\Lemmatizer\\Russian\\Rgramtab";
	};
	

	bool GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const;
	bool GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const;
	QWORD GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const;

	bool GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const;
	bool GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const;
	long GetClauseTypeByName(const char* TypeName) const;

	const char* GetClauseNameByType(long type) const;

	size_t GetClauseTypesCount() const 
	{
		return rClauseTypesCount;
	};
	bool IsStrongClauseRoot(const DWORD Poses) const;
	bool is_month (const char* lemma) const;
	bool is_small_number (const char* lemma) const;
	bool IsMorphNoun (int Poses)  const;
	bool is_morph_adj (int Poses) const;
	bool is_morph_participle (int Poses) const;
	bool is_morph_pronoun (int Poses) const;
	bool is_morph_pronoun_adjective(int Poses) const;
	bool is_left_noun_modifier  (int Poses, QWORD grammems) const;
	bool is_numeral (int poses) const;
	bool is_verb_form (int poses) const;
	bool is_infinitive(int poses) const;
	bool is_morph_predk(int poses) const;
	bool is_morph_adv(int poses) const;
	bool is_morph_article(int poses) const;
	bool is_morph_personal_pronoun (int poses, QWORD grammems) const;
	bool IsParticle(const char* lemma, int poses) const;
	bool IsSynNoun(int Poses, const char* Lemma) const;
	bool IsStandardParamAbbr (const char* WordStrUpper) const;
	BYTE	GetTagId(const char* gram_code) const;	
};


#endif //__RUSGRAMTAB_H_
