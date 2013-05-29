// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1996-2001)

//  Здесь собраны функции, которые находят вилочные дескрипторы. Например, обороты.
//	Сборка таких групп возможна только потсроения графематической таблицы.

#include "StdGraph.h"
#include "GraphmatFile.h"
#include "GraphanDicts.h"
#include "abbrev.h"



const size_t BigTextLengthInFilledLines  = 100;


// пункты перечисления
bool CGraphmatFile::DealBullet (size_t i, size_t HB)
{
	if (i == 0)    return false;
	if (i  >= HB )  return false;

	// проверяем, является ли данная графема перечислителем
	if (!IsBulletWord (i))  return false;

	if (HasDescr(i,OBullet)) return false;
	size_t nt = PSpace(i+1,HB);
	size_t nh = BSpace(i-1);
	// перед перечислителя должен стоять конец строки 
	if ((nh > 0) && !GetUnits()[nh].IsEOLN()) return false;

	// удостоверяемся, что данная графема  не вошла  в графематические группы.
	if (GetUnits()[nh].IsGrouped()) return false;

	// после перечислителя должно стоять либо закрывающая скобка, либо точка
	if (nt == HB) return false;
	if (!HasDescr(nt,OCls) && !IsOneFullStop(nt)) return false;

	// проверяем, относится ли закрывающая скобка к пункту перечисления,
	// или спереди есть открывающая скобка, в таком случае, это не пункт перечисления  
	size_t TempLowBorder = 0;

	if (i > 100)
		TempLowBorder = i-100;

	if  (HasDescr(nt,OCls))
		for (size_t k=i; k > TempLowBorder; k--)
			if (HasDescr (k,OCls)) // встретилась другая закрывающая  скобка 
				break;
			else
				if  (HasDescr (k,OOpn)) // встретилась открывающая скобка 
					return false;



	// проверяем, стоит ли после перечислителя знак препинания, если
	// стоит, то это не пункт перечисления
	nt++;
	if (nt == HB) return false;

	nt = PSpace(nt,HB);
	if (nt == HB) return false; 

	if (HasDescr (nt,OPun)) return false;

	if (IsBulletWord (nt))  
		SetState(i,nt+1,stGrouped);


	SetDes (i,OBullet);



	if (!HasDescr (i,OPar))
		SetDes(i,OPar);

	return true; 
}


// пункты перечисления, которые начинаются со '*'
struct CAsteriskHyp{
	size_t UnitNo;
	size_t LineNo;
};

const int MaxBulletSectionSize = 40;

bool CGraphmatFile::DealAsteriskBullet (size_t LB, size_t HB)
{
	vector<CAsteriskHyp> Hyps;
	size_t LineNo = 0;
	size_t i;

	for (i = LB; i< HB; i++)
	{
		if (GetUnits()[i].IsEOLN()) LineNo++;
	
		// проверяем, является ли данная графема звездочкой
		if (!GetUnits()[i].IsAsterisk()) continue;

		// удостоверяемся, что данная графема  не вошла  в графематические группы
		if (GetUnits()[i].IsGrouped() || HasDescr(i,OBullet)) continue;

		size_t nh = BSpace(i-1);

		size_t nt = PSpace(i+1,HB);

		// перед перечислителя должен стоять конец строки 
		if ((nh > 0) && !GetUnits()[nh].IsEOLN()) continue;

		// проверяем, стоит ли после перечислителя знак препинания, если
		// стоит, то это не пункт перечисления
		if (nt == HB) continue; 
		if (HasDescr (nt,OPun)) continue;

		CAsteriskHyp A;
		A.LineNo =  LineNo;
		A.UnitNo = i;
		Hyps.push_back(A);
	};

	for (i=0; i<Hyps.size(); i++)
		if (	(i+1 == Hyps.size()) 
			||	(i == 0) 
			||	(		(i+1 < Hyps.size())
					&&	(Hyps[i+1].LineNo <  Hyps[i].LineNo + MaxBulletSectionSize)
				)
			||	(		(i > 0)
					&&	(Hyps[i-1].LineNo + MaxBulletSectionSize <  Hyps[i].LineNo)
				)
			)
		{   
			SetDes (Hyps[i].UnitNo,OBullet);

			if (!HasDescr (Hyps[i].UnitNo,OPar))
				SetDes(Hyps[i].UnitNo,OPar); 
		};

	return true; 

}

bool DealIndention  (CGraphmatFile& G, size_t i, size_t Offset, const vector<WORD>& LeftMargins)
{
	if (i == 0) return true;
	if ( G.GetUnits()[i].IsSoft()) return true;

	size_t nh = G.BSpace(i-1);

	if (!G.GetUnits()[nh].IsGrouped())
		if (G.GetUnits()[nh].IsEOLN() )
		{

			if (	(		LeftMargins[i] >= (Offset + G.m_MinParOfs)
						&&	LeftMargins[i] <= (Offset + G.m_MaxParOfs)
					)
				||	(		(i > 0)
						&&	( G.GetUnits()[i-1].GetTokenLength() >= 1 )
						&&  ( G.GetUnits()[i-1].GetToken()[0] == '\t' )
					)
				)
				G.SetDes(i,OPar);
		}

	return true;
}


int CGraphmatFile::DealFloatNumbers (size_t  StartPos, size_t  EndPos)
{
   if   (   !HasDescr(StartPos, ODg) )  return 0;
   

   // проверим, вдруг это перечислитель
   if (StartPos == 0) return false;
   size_t nh = BSpace (StartPos - 1, 0);
   if (nh == 0 ) return false;
   if (GetUnits()[nh].IsEOLN()) return false;

   size_t nt = StartPos+1;
   if (nt == EndPos) return false;
   if ( !IsOneFullStop(nt)) return false;
   nt++;
   if (nt == EndPos) return false;
   if   (   !HasDescr(nt, ODg) )  return false;
   SetDes (StartPos, OFloat1);
   SetDes (nt, OFloat2);
   SetState(StartPos,nt+1,stGrouped);
   return true;
};

// пункты перечисления типа (1) или [1]
int  CGraphmatFile::DealBulletsWithTwoBrackets (size_t StartPos, size_t EndPos)
{
  size_t i = StartPos;

  if   (   !HasDescr(i, OOpn) )  return false;

  if   (   HasDescr(i, OBullet) )  return false;

  size_t nt = PSpace(i+1, EndPos);

  if (nt == EndPos) return false;
    
   // проверяем, является ли данная графема перечислителем
  if (!IsBulletWord(nt) )  return false;

  size_t BulletWordNo = nt;

  // удостоверяемся, что данная графема  не вошла  в графематические группы
  size_t  nh = (i>0) ? BSpace(i-1) : 0;

  // перед перечислителем должен стоять конец строки 
  if (   !GetUnits()[nh].IsEOLN()
	  && (nh > 0)) return false;

  if (nt + 1 >= EndPos) return false;
  nt = PSpace(nt+1,EndPos);

  // после перечислителя должно стоять либо закрывающая скобка, либо точка
  if (nt == EndPos) return false;

  if   (   !HasDescr(nt, OCls) )  return false;

  if (HasGrouped(StartPos,nt+1)) return false;

  SetDes (StartPos, OBullet);
  if (!HasDescr (StartPos, OPar))
	SetDes(StartPos, OPar);

  SetState(StartPos,nt+1,stGrouped);

  return true;
};




bool CGraphmatFile::CanBeElectronicAddressCompound (size_t i)
{
	return    IsOneFullStop(i) 
           || HasDescr (i, OHyp)
           || HasDescr (i, OLLE)
		   || HasDescr (i, ODgCh)
           || HasDescr (i, ODg);

     
};

// электронные адреса типа sokirko@medialingua.ru 
int CGraphmatFile::DealEmailAddresses (size_t StartPos, size_t EndPos)
{
  size_t i = StartPos;

  if (    (GetUnits()[i].GetTokenLength() != 1) 
	   || (((unsigned char)GetUnits()[i].GetToken()[0]) != '@') )
        return false;

  if (i  == 0) return false;

  int nh;
  for (nh=i-1; nh>=0; nh--)
	  if (!CanBeElectronicAddressCompound (nh))
		  break;
  nh++; 

  if (nh == i) return false;


  int nt;
  for (nt = i+1; nt<EndPos; nt++)
  	  if (!CanBeElectronicAddressCompound (nt))
		  break;
 
  if (nt == i + 1) return false;

  if (HasGrouped(StartPos,nt)) return false;

  SetDes (nh, OElectAddr1);
  SetDes (nt-1, OElectAddr2);
  SetState(StartPos,nt,stGrouped);

  return true;
};


// электронные адреса типа http:\\www.medialingua.ru 
// ftp:\\medialingua.ru 
bool CGraphmatFile::IsProtocolAbbr (size_t i)
{
return     (     (GetUnits()[i].GetTokenLength() == 4)
              && !strncmp(GetUppercaseToken(i), "HTTP", 4) 
           )||
           (     (GetUnits()[i].GetTokenLength() == 3)
              && !strncmp(GetUppercaseToken(i), "FTP", 3) 
           );
};

int CGraphmatFile::DealInternetAddresses (size_t StartPos, size_t  EndPos)
{
    size_t  i = StartPos;
    if (!IsProtocolAbbr(i)) return false;
    i++;
    if (i == EndPos) return false;
	if (    (GetUnits()[i].GetTokenLength() != 1) 
	   || (((unsigned char)GetUnits()[i].GetToken()[0]) != ':') )
        return false;


    for (; 
	        (i<EndPos) 
		 && (    HasDescr (i, OPun)
			  || HasDescr (i, OLLE) 
			  || HasDescr (i, ODg)) ; 
		 i++);

   if (HasGrouped(StartPos,i)) return false;

    SetDes (StartPos, OElectAddr1);
    SetDes (i-1, OElectAddr2);
    SetState(StartPos,i,stGrouped);

    return true;
};


// aaaa.a.a
// Свободный адрес должен содержать не менее двух точек
bool CGraphmatFile::DealFreeInternetAddresses (size_t StartPos, size_t EndPos)
{
	size_t i = StartPos;
	size_t CountFullStops = 0;
	bool DigitAddress =  true;
	while (i < EndPos)
	{
		if	(		(i == EndPos) 
				||	(    !HasDescr(i,OLLE)
						&& !HasDescr(i,ODg)
						&& !HasDescr(i,ODgCh)
					) 
				||	GetUnits()[i].HasSingleSpaceAfter()
			) 
			break;

		if (!HasDescr(i,ODg))  DigitAddress = false;

		i++;
		if ((i == EndPos) || !IsOneFullStop(i)) break;
		i++;
		CountFullStops++;
	};

	if (   ((CountFullStops >= 2) && !DigitAddress)
		|| ((CountFullStops >= 3) && DigitAddress)
		)
	{
		if (HasGrouped(StartPos,i)) return false;
		SetDes (StartPos, OElectAddr1);
		SetDes (i-1, OElectAddr2);
		SetState(StartPos,i,stGrouped);
		return true;
	}
	else
		return false;
};


// электронные адреса типа www.medialingua.ru 
// ftp.medialingua.ru 
bool CGraphmatFile::DealAbbrigedInternetAddresses(size_t  StartPos, size_t EndPos)
{
 size_t  i = StartPos;
 if (     (GetUnits()[i].GetTokenLength() != 3)
	   || (   strncmp(GetUppercaseToken(i), "FTP", 3) 
           && strncmp(GetUppercaseToken(i), "WWW", 3)
          ) 
    )
 return false;   

 i++;
 if (i == EndPos) return false;
 if (!IsOneFullStop(i)) return false;
 for (; (i<EndPos) && 
	    (    HasDescr (i, OPun)
		  || HasDescr (i, OLLE)
		  || HasDescr (i, ODg)
		  || HasDescr (i, ODgCh)
		);
       i++);

 if (HasGrouped(StartPos,i)) return false;
 SetDes (StartPos, OElectAddr1);
 SetDes (i-1, OElectAddr2);
 SetState(StartPos,i,stGrouped);

 return true;
};


/*
	Building units like  "Bill J. Bush"
*/
bool CGraphmatFile::DealEnglishStyleFIO (size_t StartPos, size_t EndPos)
{
  size_t  i = StartPos;
  if (!GetUnits()[i].IsEnglishName()) return false;
  i = PSoft(i+1,EndPos);
  if (   (i == EndPos) 
	  || !(     GetUnits()[i].GetTokenLength() == 1
	         && HasDescr (i, OLLE)
	      )
	 )	   return  false;
  i = PSoft(i+1,EndPos);
  if ((i == EndPos) || !IsOneFullStop(i)) return  false;
  i = PSoft(i+1,EndPos);
  if ((i == EndPos) || !HasDescr(i, OLLE)) return  false;
  if (HasGrouped(StartPos,i+1)) return false;

  SetDes (StartPos, OFAM1);
  SetDes (i, OFAM2);

  SetState(StartPos,i+1,stGrouped);
  return true;
};


/*
	Building units like  "Bill Bush"
*/
static bool DealSimpleEnglishNames (CGraphmatFile& C, size_t StartPos, size_t EndPos)
{
	size_t  i = StartPos;
	if (!(C.GetUnits()[i].IsEnglishName())) return false;

	for (i++;(i<EndPos) && C.GetUnits()[i].IsSoft(); i++)
		if (		C.GetUnits()[i].IsParagraphTag()
				||	C.HasDescr(i, OPar)
			)
		return false;

	if (	(i == EndPos) 
		|| !C.HasDescr(i, OLLE) 
		|| !C.HasDescr(i, OUpLw)
		) return  false;
	if (C.HasGrouped(StartPos,i+1)) return false;

	C.SetDes (StartPos, OFAM1);
	C.SetDes (i, OFAM2);
	C.SetState(StartPos,i+1,stGrouped);
	return true;
};




/*
HP1-HP2
Дефисной группой считается последовательность графематических слов, разделенных 
дефисом,например:
  красно-зеленый
  Комсомольск-на-Амуре
  В дефисной группе запрещены пробелы, но могут попадаться концы строк.
  Дефис с пробелами считается тире.
  Эта функция находит дефисные пары, и если начало дефисной пары совпадает 
  с концом предыдущей дефисной  пары,   тогда объединяет эти пары в одну группу
*/
int CGraphmatFile::DealHyphenPaar (size_t i,  size_t HB)
{
	size_t nh,nt;
	bool fl_spc=false;
	if (i==0) return false;
	if (!IsHyphen(i)) return false;

	for (nh=i-1;  nh>0 && GetUnits()[nh].IsSoft(); nh--)
		fl_spc = fl_spc || GetUnits()[nh].IsSpace ();

	for (nt=i+1;  nt<HB && GetUnits()[nt].IsSoft(); nt++)
		fl_spc = fl_spc || GetUnits()[nt].IsSpace ();

	if (  !fl_spc 
		&& nh > 0 
		&& nt < HB 
		&& (HasDescr (nh,ORLE) || HasDescr (nh,OLLE))
		&& (HasDescr (nt,ORLE) || HasDescr (nt,OLLE))
		)
	{ 

		/*
		если начало дефисной группы совпадает с концом предыдущей дефисной  группы,
		тогда нужно объединить эти группы, например:
		Комсомольск-на-Амуре
		Но кол-во дефисов в группе не должно быть больше трех (ad-hoc)
		*/
		if (HasDescr (nh,OSHyp2))
		{
			int i;
			for (i=nh-1; i > 0; i--)
				if (HasDescr (i,OSHyp1) )
					break;
			if (nt - i <= 4) // четыре слова - это три дефиса
			{
				DeleteDescr(nh,OSHyp2);
				nh = i;
				SetDes (nt,OSHyp2);
			}
			else
				return false;
		}
		else
		{
			SetDes (nh,OSHyp1);
			SetDes (nt,OSHyp2);
		};

		// если в дефисную группу попал однословный  оборот, то нужно отменить его,  например, может быть найден 
		// оборот ЗГЛ=что  в дефисной группе что-нибудь. 

		long l=nh;
		for (;  l<=nt; l++)
			if (	   HasDescr (l,OOb1) 
				&& HasDescr (l,OOb2) 
			 )
			{
				DeleteDescr(l,OOb1);
				DeleteDescr(l,OOb2);

				SetOborotNo(l, -1);
			};
		// если в дефисная группа пересекается  с оборотом, но оборот не вкладывает ее и не вкладывается в нее,
		// тогда  нужно отменить дефисныую группу. Я это видеол только на ошибочном примере "бригад-тем не менее"
		// это какая-та ошибка  при сканировании, но лучше ее обработать, иначе валится синтаксис
		bool bInOborot = false;
		for (l=nh;  l<=nt; l++)
		{
			if ( HasDescr (l,OOb1)  )
			{
				bInOborot = true;
			};

			if	(HasDescr (l,OOb2)  )
			{
				if (		(!bInOborot)
					&&	( l<nt)
					)
				{
					bInOborot = true;
					break;
				};
				bInOborot = false;
			};
		};
		if (bInOborot)
		{
			DeleteDescr(nh,OSHyp1);
			DeleteDescr(nt,OSHyp2);

			return false;
		};

		return true; 
	}

	return false;
}


void CGraphmatFile::DealOborotto(size_t  HB)
{

	string s;
	vector<WORD> OborotIds;
	OborotIds.resize(HB);

	int i=1;
	for (; i<HB; i++)
	{
		s = GetUppercaseToken(i);
		StringVector::const_iterator it = lower_bound(m_pDicts->m_OborotTokens.begin(),  m_pDicts->m_OborotTokens.end(), s);
		
		if (		(it != m_pDicts->m_OborotTokens.end())
				&&	(s == *it)
			)
			OborotIds[i] = it - m_pDicts->m_OborotTokens.begin();
		else
			OborotIds[i] = 0xffff;
	};

	for (i=1; i<HB; i++)
	if (		!HasDescr(i, OPass) 
			&&	!GetUnits()[i].IsSoft() 
			&&	(OborotIds[i] != 0xffff)
		)
	{
		short OborotNo = -1;
		size_t nt = FindOborotto (i,HB, OborotNo, OborotIds); 
		if (OborotNo != -1)
			SetOborotNo(i, OborotNo);
		
		if ((nt - i) == 0) continue;
		SetDes(i,OOb1);
		SetDes(nt-1,OOb2);
		SetState(i,nt,stGrouped);
	};
}

// РЕ1 - РЕ2
int CGraphmatFile::DealReferences (size_t i,size_t HB)
 {
   // ч. 1 ст. 2
   // п. 1 ст. 2 
   // п. 1.1 ст. 2

   bool is_part_string =  (GetUnits()[i].GetTokenLength() == 1)  &&  (GetUnits()[i].GetToken()[0] == 'ч');
   bool is_bullet_string =  (GetUnits()[i].GetTokenLength() == 1)  &&  (GetUnits()[i].GetToken()[0] == 'п');

   if (!is_part_string && !is_bullet_string) return false;

   size_t nt = i+1;
   if (nt == HB) return false;
   if ( !IsOneFullStop(nt)) return false;
   nt ++;
   if (nt == HB) return false;

   size_t k = PSpace (nt,HB);

   if ((k == HB)  || ((k - nt) > 10)) return false; 

   nt = FSpace (k,HB);
   nt = PSpace (nt,HB);

   if ( (k == HB) || ((nt -k) > 20)) return false;

   if	(		(GetUnits()[nt].GetTokenLength() != 2) 
			||	strncmp (GetUppercaseToken(nt), "ст",2)
		) 
	  return false;

   nt++;

   if (nt == HB) return false;

   if ( !IsOneFullStop(nt)) return false;

   nt++;

   if (nt == HB) return false;

   nt = PSpace (nt,HB);

   if (nt == HB) return false;

   SetDes (i,ORef1);
   SetDes (nt, ORef2);
   SetState(i,nt+1,stGrouped);

   return true;    
}


// DT1 - DT2
int CGraphmatFile::DealDates (size_t i,size_t HB)
{
   // DD.MM.YY
   if ( (GetUnits()[i].GetTokenLength() != 2)  && (GetUnits()[i].GetTokenLength() != 1)) return false;
   int day = GetUnits()[i].ToInt();
   if ((day <1) || (day>31)) return false;
   size_t nt = i+1;
   if (nt == HB) return false;
   if ( !IsOneFullStop(nt) && !(GetUnits()[nt].GetToken()[0] == '/')) return false;
   nt++;
   if (nt == HB) return false;
   int month = GetUnits()[nt].ToInt();
   if ((month <1) || (month>12)) return false;
   nt++;
   if (nt == HB) return false;
   if ( !IsOneFullStop(nt) && !(GetUnits()[nt].GetToken()[0] == '/')) return false;
   nt++;
   if (nt == HB) return false;
   int year =  GetUnits()[nt].ToInt();
   if ((year <1) || (year>2000)) return false;
   SetDes (i,ODate1);
   SetDes (nt, ODate2);
   SetState(i,nt+1,stGrouped);

   return true;    
}

//  21. Juni 
//  22. Juli
/*int CGraphmatFile::DealGermanDates (size_t i,size_t HB)
{
   if ( (GetUnits()[i].GetTokenLength() != 2)  && (GetUnits()[i].GetTokenLength() != 1)) return false;
   int day = GetUnits()[i].ToInt();
   if ((day <1) || (day>31)) return false;
   size_t nt = i+1;
   if (nt == HB) return false;
   if ( !IsOneFullStop(nt)) return false;
   nt++;
   if (nt == HB) return false;
   if (
   int month = GetUnits()[nt].ToInt();
   if ((month <1) || (month>12)) return false;
   nt++;
   if (nt == HB) return false;
   if ( !IsOneFullStop(nt) && !(GetUnits()[nt].GetToken()[0] == '/')) return false;
   nt++;
   if (nt == HB) return false;
   int year =  GetUnits()[nt].ToInt();
   if ((year <1) || (year>2000)) return false;
   SetDes (i,ODate1);
   SetDes (nt, ODate2);
   SetState(i,nt+1,stGrouped);

   return true;    
}
*/



// Здесь вычисляется среднее значение левого отступа (MinSpace) в отрезке [LB,HB].
// Значение MinSpace будет использовавано для обнаружения красных строк.

// LeftMargins[5] - число строк в тексте с отсупом 5
// LeftMargins[10] - число строк в тексте с отсупом 10


// Используется для fuzzy определeния минимального левого отступа 
const size_t MaxLeftMargin = 300;


void MapCorrectMinSpace (const CGraphmatFile& G, size_t LB, size_t HB, WORD& FuzzyMinSpace, WORD& MinSpace, int& NumOfFilledLines, const vector<WORD>& gLeftMargins )
{
	size_t					LeftMargins [MaxLeftMargin];

	MinSpace = 100;

	//инициализция  частотоного массива  левых отступов
	size_t k;
	for ( k=0; k<MaxLeftMargin; k++)
		LeftMargins[k] = 0;

	//вычисление частотного массива левых отступов и минимального левого отступа
	for (size_t i=LB; i<HB; i++)
	if ((i==1) || G.GetUnits()[i].IsEOLN())
	{
		i++;
		if (i == HB) break;
		i = G.PSpace (i,HB);
		if (i == HB) break;
		if (!G.GetUnits()[i].IsGrouped())
		{
			if (MinSpace < gLeftMargins[i])
				MinSpace =  gLeftMargins[i];
			NumOfFilledLines ++;
			if (gLeftMargins[i] < MaxLeftMargin)
				LeftMargins[gLeftMargins[i]]++;
		}
	};

	FuzzyMinSpace = MinSpace;
	for (k=0; k<MaxLeftMargin; k++)
	if (LeftMargins[k] > (NumOfFilledLines/100))
			{
				FuzzyMinSpace = k;	 
				break;
			};

 }



int CGraphmatFile::HasIndention  (size_t LB, size_t HB)
 {
  for (size_t i = LB; i<=HB; i++)
   if (HasDescr (i,OPar))
      return 1;

  return 0;
 }

int CGraphmatFile::CountEndL  (size_t LB, size_t HB)
{
  int count = 0;

  for (size_t i = LB; i<=HB; i++)
   if (GetUnits()[i].IsEOLN ())
    count += GetUnits()[i].GetScreenLength();

  return count;
}

int CGraphmatFile::CountSpaces  (size_t LB, size_t HB)
{
  int count = 0;
  for (size_t i = LB; i<=HB; i++)
   if (GetUnits()[i].IsSpace ())
    count += GetUnits()[i].GetScreenLength();
  return count;
}


bool  CanBeRussianInitial(const CGraphmatFile&C, size_t LineNo) 
{
	return  C.HasDescr(LineNo,OUp) && C.GetUnits()[LineNo].GetTokenLength()==1 && C.HasDescr(LineNo,ORLE) ;
};


int CGraphmatFile::DealFIO (size_t i,size_t HB)
{
	size_t nh = i,nt;
	if ((i == HB) || !CanBeRussianInitial(*this, nh)) return nh+1;
	if (HasDescr(i,OOb1) != HasDescr(i,OOb2))  return nh+1;
	

	i = PSoft(i+1,HB);
	if ((i == HB) || (!IsOneFullStop(i))) return nh+1;

	i = PSoft(i+1,HB);
	if ((i == HB) || !CanBeRussianInitial(*this, i) )return nh+1;
	if (HasDescr(i,OOb1) != HasDescr(i,OOb2))  return nh+1;

	i = PSoft(i+1,HB);
	if ( (i == HB) || (!IsOneFullStop(i)))return nh+1;


	size_t l = PSoft(i+1,HB);

	// на первой строке граф. таблицы стоит строка с дескриптором НЧТ,
	// поэтому первая строка не может быть началом ФИО

	if  (nh == 0) return nh+1;

	size_t k = BSoft(nh-1);

	bool flag_surname_is_after = 
						(l <= HB)
					&& FirstUpper(l) 
					&& !HasIndention(nh,l)
					&& (GetUnits()[l].GetTokenLength() > 1)

					//// иначе "Т.Е. ОТКАЗАТЬСЯ" будет ФИО
					&& (HasDescr(l,OOb1) == HasDescr(l,OOb2));

	bool flag_surname_is_before = 
					FirstUpper(k) 
				&& !HasIndention(k,i) 
				&& (GetUnits()[k].GetTokenLength() > 1)

				//// иначе "Т.Е. ОТКАЗАТЬСЯ" будет ФИО
				&& (HasDescr(k,OOb1) == HasDescr(k,OOb2));

	if (!flag_surname_is_before && !flag_surname_is_after)
	return nh+1;

	if (flag_surname_is_after && flag_surname_is_after)
	{
		// берем число линий в первой последовательности
		int num1 = CountEndL (nh,l);
		int num2 = CountEndL (k,i);
		// считаем число строк, разделяющих фамилию и инициалы 
		if (num1 < num2)
		flag_surname_is_before = 0;
		else
		if (num1 > num2)
		flag_surname_is_after = 0;
		else
		{
			num1 = CountSpaces (nh,l);
			num2 = CountSpaces (k,i);
			if (num1 < num2)
				flag_surname_is_before = 0;
			else
				flag_surname_is_after = 0;
		}
	}

	 // остался только один вариант
    if (flag_surname_is_before)
    { nh = k;
		nt = i;  
	}
    else
		nt = l;


	SetDes(nh,OFAM1);
	SetDes(nt,OFAM2);
	SetState(nh,nt+1,stGrouped);
	return nt+1;
}


void CalculateLMarg (const CGraphmatFile& G, vector<WORD>& gLeftMargins)
{
	WORD lm = 0;
	gLeftMargins.resize(G.GetUnits().size());
	size_t HB = G.GetUnits().size();
	for (size_t i=1; i<HB; i++)
	{
		gLeftMargins[i] = lm;
	    lm += G.GetUnits()[i].GetScreenLength();
    	if (G.GetUnits()[i].IsEOLN())
			lm = 0;
	}
}


static int flEOS=1;
void CGraphmatFile::DealNames (size_t LB, size_t  HB)
{
 for (size_t i = LB; i<HB; i++)
  {
   if (!flEOS && HasDescr(i,OUpLw) && !HasDescr(i,ONam)) SetDes (i,ONam);
   if(!GetUnits()[i].IsSoft()) flEOS = HasDescr(i,OEOS);
  }
}










/*
  readme.txt - > EA1 - EA2
  мама.txt - > EA1 - EA2
*/

/*
истина, если строка может быть названием файла (до расширения), например:
1.txt
1a.txt
a.txt
мама.txt
*.txt
..\t.txt
*/

bool CanBeFileName(const CGraphmatFile& F, size_t LB)  
{
 return (   F.HasDescr (LB, OLLE) 
	     || F.HasDescr (LB, ORLE)
	     || F.HasDescr (LB, ODgCh)
	     || F.HasDescr (LB, ODg)
		 || (     ((F.GetUnits()[LB].GetTokenLength()) ==  1)
	          &&  (((unsigned char)F.GetUnits()[LB].GetToken()[0]) == '*')
			)
		 || (     (F.GetUnits()[LB].GetTokenLength() ==  2)
	          &&  ((unsigned char)F.GetUnits()[LB].GetToken()[0] == '.')
			  &&  ((unsigned char)F.GetUnits()[LB].GetToken()[1] == '.')
			)
	 
	    );
};

void CGraphmatFile::DealExtensionsAndLocalFileNames(size_t LB, size_t  HB)
{
  if (LB < 2) return;
  int i = LB;
  if ((i == HB) || !IsOneFullStop(i)) return;
  //if (GetUnits()[i].IsAbbreviation()) return;
  i++;
  if ((i == HB) || !m_pDicts->IsExtension(GetUppercaseToken(i), GetUnits()[i].GetTokenLength()) ) return;
  //if (GetUnits()[i].IsAbbreviation()) return;
  if (!CanBeFileName(*this, LB-1))
  {
	   /*
		   Пример "рассмотрим файлы .txt"
       */
	  if ( !GetUnits()[LB-1].IsSoft() && (LB != 1)) 
		 return;
	  if (HasAbbreviation(LB,i)) return;
      SetDes (LB, OAbbr1);
      SetDes (i, OAbbr2);
      SetState(LB,i+1,stAbbreviation);
	  return;
  };

  size_t k =  LB-2;
  /*
   рассмотрим случай "c:\drivers\text.txt" 
					 и "\drivers\text.txt" 
					 и "..\drivers\text.txt" 
					 и "..\drivers\*.txt" 
					 и "drivers\*.txt" 

  */
  if (k > 1) 
  while (true)
  {
	  if (    (GetUnits()[k].GetTokenLength() !=  1)
	      || ((unsigned char)GetUnits()[k].GetToken()[0] != '\\')
		 )
	  {
		  k++;
		  break;
	  };

	  if (k <= 1) break;
	  k--;
	  
	  if (    (GetUnits()[k].GetTokenLength() ==  1)
	      &&  ((unsigned char)GetUnits()[k].GetToken()[0] == ':')
		 )
	  {
		  k--;
		  if (      (GetUnits()[k].GetTokenLength() ==  1)
				&&  HasDescr (LB, OLLE)
			 )
			  break; /* найдена цепочка c:\test.txt  к - может указывать на первую строку файла*/
		  else
		  {
			  k+=2;
			  break;
		  };

	  };
	  if (!CanBeFileName(*this, k)) 
	  {
		  k++; // "к" должно указывать на первый элемент названия файла 
		  break;
	  };
	  k--;
  }
  else
  {
	  k = LB-1;
  };


  
  if (HasGrouped(k,i)) return;
  

  SetDes (k, OElectAddr1);
  SetDes (i, OElectAddr2);
  SetState(LB,i+1,stGrouped);


  return;
};




bool CGraphmatFile :: FindKeySequence (const char* title, size_t i,size_t HB, size_t& GraLast) const
{
	size_t k=0;
	bool OborotHasSpaceBefore = true;
	bool TextHasSpaceBefore = true;

	do 
	{
		title ++;
		k++;
		if (k == GetUnits()[i].GetTokenLength())
		{
			k =0;
			i++;
		};

		bool ObororCharIsPunct = ispunct((unsigned char)*title) != 0;

		long space_size = strspn (title," ");
		title += space_size;
		OborotHasSpaceBefore = (space_size > 0) || ObororCharIsPunct;

		long j = PSoft(i, HB);
		TextHasSpaceBefore = (j > i) ||  HasDescr (i,OPun);
		i = j;

	}
	while	(		*title 
				&&	(i < HB)
				&&	!(GetUnits()[i].IsGrouped())
				&&	IsSuperEqualChar ((unsigned char)*title,(unsigned char)GetUnits()[i].GetToken()[k], m_Language)
				&&	(OborotHasSpaceBefore == TextHasSpaceBefore)
			);

	if (*title || k != 0) 
		return false;

	GraLast  =  BSoft(i-1)+1;
	return  true;
}


bool CGraphmatFile::IsKey(size_t LB, size_t HB, size_t& GraLast) const
{
  if (GetUnits()[LB].GetToken() == 0) return false;
  int ch1 =  (unsigned char)GetUnits()[LB].GetToken()[0];
  int ch2 =  (unsigned char)ReverseChar((unsigned char)GetUnits()[LB].GetToken()[0], m_Language);
  GraLast = LB+1;
  if (ispunct (ch1)) return false;  

  long i=0;
  for (; i < m_pDicts->m_Keys.size(); i++)
  {
    const char* title =  m_pDicts->m_Keys[i].c_str();
    if (
	        (    (ch1 == (unsigned char)title[0])
              || (ch2 == (unsigned char)title[0])
            )
	     && FindKeySequence(title,LB, HB, GraLast)
	   )
		break;
  }

  if (i < m_pDicts->m_Keys.size())
	  return true;
 if  (IsOneAlpha(LB))
 {
	  
	   GraLast = LB+1;
	   return true;
 };
  return  false;
};

//ctrl-alt-del
//control-PgUp
//control-Num Lock
void CGraphmatFile::DealModifierKey(size_t LB, size_t  HB)
{
  size_t SaveStart = LB;
  if (LB+3 > HB) return ;
  if (!HasDescr (LB+1, OHyp)) return ;

  // ищем в модификаторах
  if (!GetUnits()[LB].IsKeyModifier()) return;
  LB += 2;
  if  (    (LB+3 <= HB)
	    && HasDescr (LB+1, OHyp)
		&& GetUnits()[LB].IsKeyModifier()
	  )
    LB+=2;


  // ищем в клавишах
  size_t GraLast;
  if (!IsKey(LB, HB, GraLast)) return;

  if (!HasGrouped(SaveStart,GraLast) )
  {
     SetDes (SaveStart, OKey1);
     SetDes (GraLast-1, OKey2);
     SetState(SaveStart,GraLast,stGrouped);

  }
    
};

// F1-z
void CGraphmatFile::DealSimpleKey(size_t LB, size_t  HB)
{
	// ищем в клавишах
  size_t GraLast;
  if (    !IsKey(LB, HB, GraLast)
	 ) return;

  if (    (LB+1 == GraLast)
	   && IsOneAlpha(LB)
	 )
	 return;

  if (!HasGrouped(LB, GraLast) )
  {
     SetDes (LB, OKey1);
     SetDes (GraLast-1, OKey2);
     SetState(LB,GraLast,stGrouped);
  }
};

void CGraphmatFile::DealKeySequence(size_t LB, size_t  HB)
{
	if (!HasDescr(LB, OKey1)) return;

	long i=LB;
	for (;  i< HB;)
	{
		if (!HasDescr(i, OKey1)) break;
		for (; i < HB; i++)
		   if (HasDescr(i, OKey2)) break;
        if (i == HB) break;
		i++;
		if (i == HB) break;
 	    if (GetUnits()[i].IsSoft())   i++;
	};
	if ((i ==  HB) || !HasDescr(i, OKey2))
	  i = BSoft(i-1);
	if (i-LB < 2)  return;
  

	for (long k=LB; k <= i; k++)
	{
		DeleteDescr(k,OKey1);
		DeleteDescr(k,OKey2);
	};

     SetDes (LB, OKey1);
     SetDes (i, OKey2);
     SetState(LB,i+1,stGrouped);
};

void CGraphmatFile::DealQuotedKeySequence(size_t LB, size_t  HB)
{
   if (!IsOneOpenQuotationMark(LB))  return;
   long i = LB + 1;
   if (i == HB) return;
   if (!HasDescr(i, OKey1)) return ;
   for (; i < HB; i++)
	  if (HasDescr(i, OKey2)) break;

   if (i == HB) return; 
   i++;
   if (i == HB) return; 
   if (!IsOneCloseQuotationMark(i))  return;



   for (long k=LB; k <= i; k++)
   {
		DeleteDescr(k,OKey1);
		DeleteDescr(k,OKey2);

   };

   SetDes (LB, OKey1);
   SetDes (i, OKey2);
   SetState(LB,i+1,stGrouped);
};

// ===========  German divides compounds  ================
// examples:
// Ost- und Westberlin
// Ost-,  Nord- oder  Westberlin

void CGraphmatFile::DealGermanDividedCompounds(size_t LB, size_t  HB)
{
	size_t i = LB;
	bool bFound = false;
	while (i < HB)
	{
		if (!HasDescr(i, OLLE)) break;
		i++;
		if (i == HB) break;

		//  no space between word and hyphen
		if (GetUnits()[i-1].GetInputOffset()+ GetUnits()[i-1].GetTokenLength()  != GetUnits()[i].GetInputOffset())
			break;

		if (!HasDescr(i, OHyp)) break;
		i = PSoft(i+1, HB);
		if (i == HB) break;
	
		
		if (	(		!strncmp(GetUppercaseToken(i), "UND", 3)
					&&	(GetUnits()[i].GetTokenLength() == 3)
				)
				|| (		!strncmp(GetUppercaseToken(i), "ODER", 4)
					&&	(GetUnits()[i].GetTokenLength() == 4)
				)
			)
		{
			i = PSoft(i+1, HB);
			if (i == HB) break;

			for (long k=LB; k <= i; k++)
				if (HasDescr(k, OGerDivComp2)) 
					return;

			SetDes (LB, OGerDivComp1);
			SetDes (i, OGerDivComp2);
			SetState(LB,i+1,stGrouped);
			return;
		};
				
				

		if (!IsOneChar(i, (BYTE)',')) break;
		i = PSoft(i+1, HB);
		if (i == HB) break;
	};

};




static void InitEnglishNameSlot (CGraphmatFile& C)
{
	for (size_t i=1; i< C.GetUnits().size(); i++)  
	   if (   !C.GetUnits()[i].IsSoft() 
		   && !C.HasDescr(i,OPun)
		   )
	   {
			if (C.HasDescr(i, OLw)) continue;

			const char* UpperUnit = C.GetUppercaseToken(i);

			vector<CEnglishName>::const_iterator It = lower_bound (C.m_pDicts->m_EnglishNames.begin(), C.m_pDicts->m_EnglishNames.end(), UpperUnit, EnglishNameLess);

			if	(		 (It != C.m_pDicts->m_EnglishNames.end())
					&&	!strcmp(It->name, UpperUnit)
				)
			C.GetUnit(i).SetEnglishName();
	
	   };
};

int CGraphmatFile::InitContextDescriptors (size_t LB, size_t HB)
{

	int NumOfFilledLines = 0;   //  число непустых строк

	WORD		FuzzyMinSpace;  /* This variable is the fuzzy min left margin.
	It is used to determine indentions  of the text
	when the text is more than BigTextLengthInFilledLines */

	WORD		MinSpace = 100;    /* This variable is the min left margin
	of the whole text, it will be used for
	determintation of indentions. */


	size_t i;

	
    InitEnglishNameSlot(*this);


	//  === Смещение от левого края =====
	vector<WORD> gLeftMargins;
	CalculateLMarg(*this, gLeftMargins);


	//  === Обороты =====
	DealOborotto (HB);

	//  === Имена собственные (Им?) =====
	DealNames (LB,HB);

	for (i=LB; i<HB; i++)
		DealEnglishStyleFIO (i, HB);

	for (i=LB; i<HB; i++)
		DealSimpleEnglishNames(*this, i, HB);

	//  === Минимальный левый отступ =====
	MapCorrectMinSpace (*this, LB+1,HB, FuzzyMinSpace,MinSpace, NumOfFilledLines, gLeftMargins);


	for (i=LB; i<HB; i++)
		DealModifierKey(i,HB);


	for (i=1; i<HB; i++)
		DealAbbrev(i,HB);


	//  === Реф-отрезки =====
	for (i=LB; i<HB; i++)
		DealReferences(i,HB);

	//  === Даты =====
	for (i=LB; i<HB; i++)
		DealDates (i,HB);

	//=========== InternetAddresses

	for (i=LB; i<HB; i++)
		DealInternetAddresses(i,HB);

	for (i=LB; i<HB; i++)
		DealAbbrigedInternetAddresses(i, HB);


	for (i=LB; i<HB; i++)
		DealEmailAddresses(i,HB);

	for (i=LB; i<HB; i++)
		DealFreeInternetAddresses(i,HB);


	// ============  расширения файлов
	for (i=LB; i<HB; i++)
		DealExtensionsAndLocalFileNames(i,HB);




	//=========== Keys


	for (i=LB; i<HB; i++)
		DealSimpleKey(i,HB);

	for (i=LB; i<HB; i++)
		DealKeySequence(i,HB);

	if (m_Language == morphGerman)
		for (i=LB; i<HB; i++)
			DealGermanDividedCompounds(i,HB);


	for (i=LB; i<HB; i++)
		DealFloatNumbers (i, HB);

	//  === ФИО  =====
	for (i=LB; i<HB;)
		if (!HasDescr(i, OPass))
			i = DealFIO (i,HB);
		else
			i++;


	DealAsteriskBullet (LB, HB);


	//  === Все остальное =====
	size_t LastAsteriskNo = 0;
	for (i=LB; i<HB; i++)
	{
		if (!HasDescr(i, OPass))
		{

			DealHyphenPaar(i,HB);
			DealBullet(i,HB); 
			DealBulletsWithTwoBrackets (i, HB);
			if (HasDescr(i,OBullet))
				MapCorrectMinSpace (*this, i+1,HB, FuzzyMinSpace, MinSpace, NumOfFilledLines, gLeftMargins);

			size_t Offset =  (		(NumOfFilledLines < (size_t)BigTextLengthInFilledLines) 
								|| (MinSpace == FuzzyMinSpace) 
								|| (gLeftMargins[i] < FuzzyMinSpace)
							  ) ? MinSpace : FuzzyMinSpace;



			if (m_bUseIndention)
				DealIndention(*this, i, Offset, gLeftMargins);

			int LowerBorder = (gLeftMargins[i] == 0) ? gLeftMargins[i] :  gLeftMargins[i] - 1;

			if   (	   (LastAsteriskNo != 0) 
					&& (gLeftMargins[LastAsteriskNo] <= gLeftMargins[i]+1)
					&& (gLeftMargins[LastAsteriskNo] >= LowerBorder)
					&& (GetUnit(LastAsteriskNo).GetInputOffset() < GetUnit(i).GetInputOffset() + 1000)
					&& HasDescr(i, OPar)
				)
				DeleteDescr(i,OPar);



			if (HasDescr(i, OBullet) && GetUnits()[i].IsAsterisk())
			{

				LastAsteriskNo = PSpace(i+1, HB);
			};

		};
	}

	
	return 1;
}

