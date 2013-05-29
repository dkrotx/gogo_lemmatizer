// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Lemmatizer (www.aot.ru)
// ==========  Copyright by Alexey Sokirko
#include "StdGramtab.h"
#include "EngGramTab.h"


CEngGramTab :: CEngGramTab()
{
	for (size_t i = 0; i<GetMaxGrmCount(); i++) 
	    GetLine(i) = NULL;

}

CEngGramTab :: ~CEngGramTab()
{
	int Num = 0;
	for (size_t i=0; i<GetMaxGrmCount(); i++) 
		if (GetLine(i) != NULL) 
		{
			delete GetLine(i);
			Num++;
		};
};





bool CEngGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	return  false;
}

bool CEngGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	return  false;
}


bool CEngGramTab::IsStrongClauseRoot(const DWORD Poses)  const
{
	return	false;
};


bool CEngGramTab::is_small_number (const char* lemma) const  
{
   return false;
}

bool CEngGramTab::is_month (const char* lemma) const
{
	return false;
};

bool CEngGramTab::IsMorphNoun (int Poses)  const
{
	return  false;
};

bool CEngGramTab::is_morph_adj (int Poses) const
{
	return  false;
};

bool CEngGramTab::is_morph_participle (int Poses) const
{
	return  false;
};
 
bool CEngGramTab::is_morph_pronoun (int Poses) const
{
	return  false;
};

bool CEngGramTab::is_morph_pronoun_adjective(int Poses) const
{
	return  false;
};

bool CEngGramTab::is_left_noun_modifier (int Poses, QWORD grammems) const
{
	return     false;
}


bool CEngGramTab::is_numeral (int poses) const
{ 
	return  false;
};

bool CEngGramTab::is_verb_form (int poses) const
{
	return     false;
};



bool CEngGramTab::is_infinitive(int poses) const
{
	return false; 
}

bool CEngGramTab::is_morph_predk(int poses) const
{
	return false;
}

bool CEngGramTab::is_morph_adv(int poses) const
{
	return false; 
}

bool CEngGramTab::is_morph_personal_pronoun (int poses, QWORD grammems) const
{
	return		false;
};

bool CEngGramTab::IsParticle(const char* lemma, int poses) const
{
	return false;
}







bool CEngGramTab::IsSynNoun(int Poses, const char* Lemma) const
{
	return false;
};

bool CEngGramTab::IsStandardParamAbbr (const char* WordStrUpper) const
{
	return false;
};



bool CEngGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	return false;
};
bool CEngGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	return false;
};
QWORD CEngGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const 
{
	return 0;
};

bool CEngGramTab::is_morph_article(int poses)  const 
{
	return false;
};

BYTE	CEngGramTab::GetTagId(const char* gram_code) const
{
	return GetPartOfSpeech(gram_code);
};
