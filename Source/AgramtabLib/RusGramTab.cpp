#include "StdGramtab.h"

// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Lemmatizer (www.aot.ru)
// ==========  Copyright by Alexey Sokirko

#include "RusGramTab.h"




CRusGramTab :: CRusGramTab()
{
	size_t i=0;

	for (; i<GetMaxGrmCount(); i++) 
	    GetLine(i) = NULL;

}

CRusGramTab :: ~CRusGramTab()
{
	for (size_t i=0; i<GetMaxGrmCount(); i++) 
		if (GetLine(i) != NULL) 
			delete GetLine(i);

}


bool CRusGramTab :: IsAdditionalGrammem (const char* s) const 
{
	return  !strcmp (s, "прч")  
		  || !strcmp (s, "дпр")  
	      || !strcmp (s, "инф");
};

bool CRusGramTab :: ProcessPOSAndGrammems (const char* tab_str, BYTE& PartOfSpeech,  QWORD& grammems) const
{
	if (!CAgramtab::ProcessPOSAndGrammems(tab_str, PartOfSpeech, grammems)) return false;

    if (PartOfSpeech == VERB)
 		if (strstr (tab_str, "прч"))  
			PartOfSpeech = PARTICIPLE;
		else
		if (strstr (tab_str,"дпр"))
			PartOfSpeech = ADVERB_PARTICIPLE;
		else
		  if (strstr (tab_str,"инф"))
			PartOfSpeech = INFINITIVE;

    
    
   // неизменяемые слова как будто принадлежат всем падежам
   if (			(_QM(rIndeclinable) & grammems)
		&&		(PartOfSpeech != PREDK )
	   )
           grammems |= rAllCases;

   if ( (_QM(rIndeclinable) & grammems) 
	    && (PartOfSpeech == PRONOUN_P )
	  )
   grammems |= rAllGenders | rAllNumbers;


    // слова общего рода ('сирота') могут  использованы как 
	// слова м.р., так и как слова ж.р.
   if (_QM(rMascFem) & grammems)
		 grammems |=   _QM(rMasculinum) | _QM(rFeminum);
		   
   // слово 'пальто' не изменяется по числам, поэтому может
   // быть использовано в обоих числах
   if (PartOfSpeech != PREDK )
	   if ( (_QM(rIndeclinable) & grammems) &&  !(_QM(rSingular) & grammems))
			grammems |=   _QM(rPlural) | _QM(rSingular);

   return true;
};






// Стандартное согласование между двумя именами  по  числу и падежу
inline bool CaseNumber (QWORD g1, QWORD g2) 
  {
      return ((rAllCases  & g1 & g2) > 0) &&
		     ((rAllNumbers & g1 & g2) > 0) ;
  };

// Стандартное согласование между двумя именами  по  падежу, причем первый код должен
// иметь граммему множественного числа 
inline bool CaseFirstPlural (QWORD g1, QWORD g2) 
  {
      return (     ( (rAllCases  & g1 & g2) > 0) 
		       &&  ( ((1<<rPlural)  & g1)  > 0)
			 );
  };


// Стандартное согласование между двумя именами  по роду и числу 
inline bool GenderNumber(QWORD g1, QWORD g2)
	{
		 
		return 	((rAllNumbers & g1 & g2) > 0) 
			 && (    ((g1 & g2 & _QM(rPlural)) > 0 ) 
				  || ((rAllGenders & g1 & g2) > 0)
				);
	}

// Стандартное согласование по лицу и числу (для 1 и 2 лица)
inline bool PersonNumber(QWORD g1, QWORD g2)
{
	size_t t1 = (rAllNumbers & g1 & g2) > 0;
	size_t t2 = ((rAllPersons & g1 & g2) > 0);
	return t1 && t2;
}

// Стандартное согласование между подлежащим и сказуемым
inline bool SubjectPredicate(QWORD subj, QWORD verb)
{
	if(    !( subj & _QM(rNominativ)) 
	  )
	return false;


	if (    ( ( verb & _QM(rPastTense)) > 0)
		 || ( ( verb & _QM(rShortForm)) > 0)
	  )
	{
		// ты вышел 
		// я вышел
		// ты был
		// я красива
		// мы шли
		// вы шли
		if(subj & (_QM(rFirstPerson) | _QM(rSecondPerson)))
			return		(verb & subj & _QM(rPlural) )
					||	(		(verb & ( _QM(rMasculinum) | _QM(rFeminum) ) )
						  &&	(verb & subj & _QM(rSingular) )
						);	
		else
			// он вышел
			// поезд ушел
			// девочка красива
			// девочки красивы 	
			// мальчик красив
			return GenderNumber(subj,verb);
	}
	else 
	if(		( verb & _QM(rPresentTense)) 
		||	( verb & _QM(rFutureTense )) )
	{
		//  я выйду
		//  ты выедешь
		//  мы выйдем
		if (	( subj & (_QM(rFirstPerson) | _QM(rSecondPerson)))
			||	(verb & (_QM(rFirstPerson) | _QM(rSecondPerson))) )
			return PersonNumber(subj, verb);
		else
			return (rAllNumbers & subj & verb) > 0;
	}
	else 
	if( verb & _QM(rImperative))
			return		((subj & _QM(rSecondPerson)) > 0)
					&&	((rAllNumbers & subj & verb) > 0);

	return false;
}


// Стандартное согласование между двумя именами  по  падежу 
inline bool Case(QWORD g1, QWORD g2)
	{
		 
		return 	((rAllCases & g1 & g2) > 0); 
	}


// Стандартное согласование между существительным и прилашательнымпо роду, числу и падежу, если 
// если существительное одушевленное
bool GenderNumberCaseAnimRussian (QWORD gram_noun, QWORD gram_adj) 
{	
	return			((rAllCases  & gram_noun & gram_adj) > 0) 
		    	 &&	((rAllNumbers & gram_noun & gram_adj) > 0) 
				 &&	(		((_QM(rAnimative) & gram_adj ) > 0) 
						||	((rAllAnimative& gram_adj) == 0)
					)
		         && (    ((rAllGenders & gram_noun & gram_adj) > 0)
					  || ((rAllGenders & gram_noun) == 0)
					  || ((rAllGenders & gram_adj) == 0)
					);
};

// Стандартное согласование между существительным и прилашательнымпо роду, числу и падежу, если 
// если существительное неодушевленное
bool GenderNumberCaseNotAnimRussian (QWORD gram_noun, QWORD gram_adj) 
{	
	return			((rAllCases  & gram_noun & gram_adj) > 0) 
		    	 &&	((rAllNumbers & gram_noun & gram_adj) > 0) 
				 &&	(		((_QM(rNonAnimative) & gram_adj ) > 0) 
						||	((rAllAnimative& gram_adj) == 0)
					)
		         && (    ((rAllGenders & gram_noun & gram_adj) > 0)
					  || ((rAllGenders & gram_noun) == 0)
					  || ((rAllGenders & gram_adj) == 0)
					);
};

// Стандартное согласование между существительным и прилашательнымпо роду, числу и падежу, если 
// если существительное не неодушевленное и не одушевленное
//  (для местоимений, например, "все это было хорошо")
bool GenderNumberCaseRussian (QWORD gram_noun, QWORD gram_adj) 
{	
	return			((rAllCases  & gram_noun & gram_adj) > 0) 
		    	 &&	((rAllNumbers & gram_noun & gram_adj) > 0) 
		         && (    ((rAllGenders & gram_noun & gram_adj) > 0)
					  || ((rAllGenders & gram_noun) == 0)
					  || ((rAllGenders & gram_adj) == 0)
					);
};




/*
Примеры работы функции GleicheGenderNumberCase:
- красивого стол;
- красивых столы;
- красивый пирата;
- красивые пиратов;
- достойных жизни;
- достойные матерей;
- длинные чудовищ
+ на этот дом;
+ красивую маму;
+ в красивый  дом;
+ в длинное облако;
+ примерить его цилиндр;
+ все это 
*/
QWORD CRusGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	if	(		(common_gram_code_noun == 0)
			||	!strcmp(common_gram_code_noun, "??")
			||	(strlen (common_gram_code_noun) == 0)
		)
			// нет сведений об одушевленности
		return Gleiche(GenderNumberCaseRussian, gram_code_noun, gram_code_adj);
	else
		if ((GetLine(s2i(common_gram_code_noun))->m_Grammems&_QM(rNonAnimative)) >0)
				// неодушевленный
			return Gleiche(GenderNumberCaseNotAnimRussian, gram_code_noun, gram_code_adj);
		else
			if ((GetLine(s2i(common_gram_code_noun))->m_Grammems&_QM(rAnimative)) >0)
				// одушевленный
				return Gleiche(GenderNumberCaseAnimRussian, gram_code_noun, gram_code_adj);
			else
				// нет сведений об одушевленности
				return Gleiche(GenderNumberCaseRussian, gram_code_noun, gram_code_adj);

}



bool CRusGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	return  Gleiche(CaseNumber, gram_code1, gram_code2) != 0;
}
bool CRusGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	return  Gleiche(GenderNumber, gram_code1, gram_code2) != 0;
}
bool CRusGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	return  Gleiche(SubjectPredicate, gram_code1, gram_code2) != 0;
}

bool CRusGramTab::GleicheCase(const char* gram_code1, const char* gram_code2) const
{
	return  Gleiche(Case, gram_code1, gram_code2) != 0;
}


long CRusGramTab::GetClauseTypeByName(const char* TypeName) const
{
	for(int i = 0 ; i < rClauseTypesCount ; i++ )
	{
		if( !strcmp(rClauseTypes[i], TypeName) )
		{
			return i;
		}
	}

	return -1;
}


const char* CRusGramTab::GetClauseNameByType(long type) const
{
	if (type >= rClauseTypesCount) return 0;
	if (type < 0) return 0;

	return rClauseTypes[type];	
}










/*
	истина для предикативных типов клауз.
*/
bool CRusGramTab::IsStrongClauseRoot(const DWORD Poses) const
{
	return		(Poses == (1<<VERB))
			||	(Poses == (1<<ADVERB_PARTICIPLE)) // субъект деепричастия  совпадлает с субъектом
												 // главной  клаузы 
			||	(Poses == (1<<PARTICIPLE_SHORT))
			||	(Poses == (1<<ADJ_SHORT))
			||	(Poses == (1<<PREDK));
};





char months[12][10] = {"ЯНВАРЬ","ФЕВРАЛЬ","МАРТ",
		       "АПРЕЛЬ","МАЙ","ИЮНЬ",
		       "ИЮЛЬ","АВГУСТ","СЕНТЯБРЬ"
		       ,"ОКТЯБРЬ","НОЯБРЬ","ДЕКАБРЬ"};

bool CRusGramTab::is_month (const char* lemma) const
{
   if( !lemma ) return false;
   for(size_t i = 0 ; i < 12 ; i++ )
     if( !strcmp(lemma,months[i]))
        return true;
   return false;
}

char SmallNumbers[5][10] = {"ДВА","ТРИ","ЧЕТЫРЕ","ОБА","ПОЛТОРА"};

bool CRusGramTab::is_small_number (const char* lemma) const  
{
   if( !lemma ) return false;
   for(size_t i = 0 ; i < 5 ; i++ )
     if( !strcmp(lemma,SmallNumbers[i]))
        return true;
   return false;
}


bool CRusGramTab::IsMorphNoun (int Poses)  const
{
	return  ( Poses & ( 1 << NOUN)   );
};




bool CRusGramTab::is_morph_adj (int Poses) const
{
	return		( Poses & (1 <<ADJ_FULL  ))
			|| ( Poses & (1 << ADJ_SHORT ) );
};

bool CRusGramTab::is_morph_participle (int Poses) const
{
	return  ( Poses & (1 <<PARTICIPLE  ))
			|| ( Poses & (1 << PARTICIPLE_SHORT   ));
};
 
bool CRusGramTab::is_morph_pronoun (int Poses) const
{
	return  ( Poses & (1 <<PRONOUN  )) != 0;
};


bool CRusGramTab::is_morph_pronoun_adjective(int Poses) const
{
	return  ( Poses & (1 <<PRONOUN_P )) != 0;
};


bool CRusGramTab::is_left_noun_modifier  (int Poses, QWORD grammems) const
{
	return     ( Poses & (1 << ADJ_FULL  ))
			|| ( Poses & (1 << NUMERAL_P ))
			|| ( Poses & (1 << PRONOUN_P ))
			|| ( Poses & (1 << PARTICIPLE  )) ;
}


bool CRusGramTab::is_numeral (int poses) const
{ 
	return  ( poses & (1 << NUMERAL ) ) != 0;
};

bool CRusGramTab::is_verb_form (int poses) const
{
	return     is_morph_participle(poses) 
			||  (poses & ( 1 << VERB )) 
			||  (poses & ( 1 << INFINITIVE ))
			||  (poses & ( 1 << ADVERB_PARTICIPLE ));
};



bool CRusGramTab::is_infinitive(int poses) const
{
	return (poses & (1 << INFINITIVE )) != 0; 
}

bool CRusGramTab::is_morph_predk(int poses) const
{
	return (poses & (1 << PREDK )) || 
		(poses & (1 << PRONOUN_PREDK )); 
}

bool CRusGramTab::is_morph_adv(int poses) const
{
	return (poses & (1 << ADV )) != 0; 
}


bool CRusGramTab::is_morph_personal_pronoun (int poses, QWORD grammems) const
{
	return		 ( poses & (1 <<PRONOUN  )) != 0 
			&&   ( grammems &( _QM(rFirstPerson) | _QM(rSecondPerson) | _QM(rThirdPerson)));
};



const size_t  ParticleCount = 5;
const char Particles[ParticleCount][20] = {"ЛИ","ЖЕ","БЫ","УЖ","ТОЛЬКО"};

bool CRusGramTab::IsParticle(const char* lemma, int poses) const
{
	if (!lemma) return false;
	if (!(poses & (1 << PARTICLE))) return false;
	for(int i = 0 ; i < ParticleCount ; i++ )
		if( !strcmp(lemma,Particles[i]) )
			return true;

	return false;
}









/*
	Есть такие  мест. прилагательные, которые могут выступать в роли существительных:
	"КАЖДЫЙ", "ОДИН", "ДРУГОЙ","ТОТ","КОТОРЫЙ".
	Для  них не имеет смысла вводит аналогичные местоимения,поскольку  все они могут 
	быть использованы во всех родах. Эти прилагательные могут вести себя так же, как
	существительные. Например,
	Я купил два яблока, в каждом оказалось много червяков
	Я знаю ту, которая тебя видит
	Один пришел, другой ушел.
	Я вижу дом, который разрушился.
	Вышеуказанные мест. прилагательные отличаются, например, от местоимения "этот" и "всякий", поскольку	
	трудно себе представить что-то вроде:
	"эта пришла"
	"всякая пришла"
	Но возможно:
	"всякое бывает"
	"это бывает"
	Здесь "всякое" и "это" - простые местоимения ср рода.

	!! Таким образом, я не хочу дублировать в морфологии МС-П, если они могут использованы
	!! во всех родах в качестве существительного.

	1 марта 2001 года, Сокирко
*/
bool CRusGramTab::IsSynNoun(int Poses, const char* Lemma) const
{
	return   IsMorphNoun (Poses) 
			||( Poses & (1 <<PRONOUN ))
			||(    (Poses & (1 <<PRONOUN_P ) )
				&& (	  !strcmp(Lemma, "КАЖДЫЙ") 
					||!strcmp(Lemma, "ОДИН") 
					||!strcmp(Lemma, "ДРУГОЙ") 
					||!strcmp(Lemma, "ТОТ") 
					||!strcmp(Lemma, "КОТОРЫЙ") 
					)
			);
};


const int StandardParamAbbrCount = 8;
const int StandardParamAbbrLength = 4;

const char StandardParamAbbr[StandardParamAbbrCount][StandardParamAbbrLength] =  
{ "КГ", "КМ", "СМ","МЛ", "МБ","КБ", "МГЦ", "КВ"};

bool CRusGramTab::IsStandardParamAbbr (const char* WordStrUpper) const
{
	if (strlen(WordStrUpper) > StandardParamAbbrLength) return false;
	for (long  i = 0;  i < StandardParamAbbrCount; i++)
		if ( !strcmp(WordStrUpper,StandardParamAbbr[i]) )
			return true;

	return false;
}


bool CRusGramTab::is_morph_article(int poses)  const 
{
	return  false;
};

BYTE	CRusGramTab::GetTagId(const char* gram_code) const
{
	BYTE  TagId = GetPartOfSpeech(gram_code);

	switch(TagId)
	{
		case ADJ_FULL:
		{
			if ( _QM(rShortForm) & GetLine(s2i(gram_code))->m_Grammems )
				return ADJ_SHORT;
			break;
		}
		case PARTICIPLE :
			{
				if ( _QM(rShortForm) & GetLine(s2i(gram_code))->m_Grammems )
				return PARTICIPLE_SHORT;					
				break;
			}
	}
	return TagId;

};
