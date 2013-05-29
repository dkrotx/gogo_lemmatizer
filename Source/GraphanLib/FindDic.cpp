#pragma warning(disable:4786)
#pragma warning(disable:4530)

#include "StdMorph.h"
#include "FindDic.h"
#include "Paradigm.h"

//================================================
CFindWordNode::CFindWordNode()
{	m_nWeight = UndescribedWeight;	
	m_nBase = 0;	
	m_nFlex = UpperBoundOfFlexiaCount;	
	m_LemmaInfo.m_FlexiaModelNo = UnknownParadigmNo;	
	m_bFound = false;	
}

bool CFindWordNode:: operator ==(const CFindWordNode& obj) const
{	return (	(m_bFound == obj.m_bFound)
			&&	(m_nBase == obj.m_nBase)
			&&	(m_nFlex == obj.m_nFlex)
			&&	(m_LemmaInfo == obj.m_LemmaInfo));	
}


bool CFindWordNode::operator <(const CFindWordNode& obj) const
{	return (
					m_nBase == obj.m_nBase
				?			m_nFlex == obj.m_nFlex	
						?		m_LemmaInfo < obj.m_LemmaInfo
							:	m_nFlex < obj.m_nFlex
							:	m_nBase < obj.m_nBase);	
}

void CFindWordNode::GetLemSign(char* Buffer) const
{
	Buffer[0] = m_bFound ? '+' : '-';
	if (m_LemmaInfo.m_CommonAncode[0] == 0)
		strncpy(Buffer+1, "??",2);
	else
		strncpy(Buffer+1, m_LemmaInfo.m_CommonAncode,2);
	Buffer[3] = 0;
};

//================================================

inline bool  LessShortString(const CShortString& obj, const char* s)
{
	return strcmp(obj.GetString(), s) < 0;
}

bool CMorphDictionary::HelperGetFormNumber(WORD flex, WORD paradigm, DWORD& form) const
{
	if (flex == UpperBoundOfFlexiaCount)
		return false;

	if (paradigm >= m_FlexModels.size())
		return false;

	CParadigmItemPtr Begin = m_FlexModels.GetVectorBegin(paradigm);
	CParadigmItemPtr End = m_FlexModels.GetVectorEnd(paradigm);
	int i = 0;
	while (Begin != End)
	{
		if (Begin->m_nFlex == flex)
		{
			form = i;
			return true;
		}
		i++;
		Begin++;
	}
	return false;
}



// ParadigmId is a unique id of word in the dictionary. Each word can be unique determined
// by its BaseNo and LemmaInfo.
// We build ParadigmId so, as m_Base2LemmaInfos.m_Base[ParadigmId-1] ==  LemmaInfo, and  
// m_Base2LemmaInfos.m_Keys[BaseNo]<ParadigmId-1< m_Base2LemmaInfos.m_Keys[BaseNo+1]
DWORD CMorphDictionary::HelperGetParadigmID(DWORD BaseNo, CLemmaInfo LemmaInfo) const
{
	if (BaseNo >= m_Base2LemmaInfos.size())
	{
		assert (false);
		return 0;
	};
	
	vector<CLemmaInfo>::const_iterator pParadigmEnd = m_Base2LemmaInfos.GetVectorEnd(BaseNo);

	//  searching  	ParadigmNo with this BaseNo
	for (vector<CLemmaInfo>::const_iterator pParadigmBegin = m_Base2LemmaInfos.GetVectorBegin(BaseNo);
			pParadigmBegin != pParadigmEnd;
			pParadigmBegin++)
	{
		if (*pParadigmBegin == LemmaInfo)
		{
			DWORD ParadigmId = pParadigmBegin - m_Base2LemmaInfos.GetVectorBegin(0)+1;
			return ParadigmId;
		};
	}

	assert (false);
	return 0;
}

bool CMorphDictionary::HelperFirstParadigmID(DWORD& newid) const 
{
	if (m_Base2LemmaInfos.empty())
		return false;

	newid = 1;
	return true;

}




bool CMorphDictionary::HelperSplitParadigmID(DWORD ParadigmId, DWORD& BaseNo, CLemmaInfo& LemmaInfo) const
{
	DWORD Paradigm;
	if (!m_Base2LemmaInfos.GetKeyByBaseNo(ParadigmId-1, BaseNo, Paradigm)) return false;
	LemmaInfo =  m_Base2LemmaInfos.GetVectorBegin(BaseNo)[Paradigm];
	return true;
}


bool CMorphDictionary::HelperNextParadigmID(DWORD oldid, DWORD& newid) const
{
	newid = oldid+1;
	return (newid+1 < m_Base2LemmaInfos.GetBaseSize());
}


bool CMorphDictionary::HelperPrevParadigmID(DWORD oldid, DWORD& newid) const
{
	if (oldid == 1) return false;
	newid = oldid-1;
	return true;
}


void CMorphDictionary::HelperGetInfo(const WORD nFlex, const WORD nParadigm, char* buffer) const 
{
	CParadigmItemPtr Begin = m_FlexModels.GetVectorBegin(nParadigm);
	
	DWORD nSize = m_FlexModels.GetVectorLength(nParadigm);

	for (int i = 0; i < nSize; i++)
	{
		if (Begin->m_nFlex == nFlex)
			strcat(buffer, m_Infos[Begin->m_AncodeNo].GetString());

		Begin++;
	};
}



char* CMorphDictionary::HelperGetNorm(const string& InputWordStr, const bool bFound, const DWORD nBase, const WORD nFlex, const WORD nParadigm, char* OutBuffer) const 
{
	OutBuffer[0] = 0;
	if (!bFound)
	{
		size_t base_size = (InputWordStr.length() - m_Flexs[nFlex].GetLength());
		memcpy(OutBuffer,  InputWordStr.c_str(), base_size);
		OutBuffer[base_size] = 0;
	}

	else
	{
		//result = string(m_Bases[nBase].GetString()).substr(0, m_Bases[nBase].GetLength());
		size_t base_size = m_Bases[nBase].GetLength();
		memcpy(OutBuffer,  m_Bases[nBase].GetString(), base_size);
		OutBuffer[base_size] = 0;
	};

	WORD nIndex = m_FlexModels.GetVectorBegin(nParadigm)->m_nFlex;
	strcat(OutBuffer, m_Flexs[nIndex].GetString()); 


	return OutBuffer;
}





/*
	It returns the ordinal number of  flex  in  m_Flexs.
	If not found, returns UpperBoundOfFlexiaCount
*/
WORD CMorphDictionary::GetFlexNoByFlexStr(const char* flex, size_t curlen) const 
{
	assert( !m_FlexLengthTable.empty() );
	WORD idxlow = m_FlexLengthTable[curlen];
	if (idxlow == MaxFlexiaLength) return UpperBoundOfFlexiaCount;

	WORD idxhigh = idxlow;
	WORD Count = m_FlexLengthTable.size();
	for (int j = curlen + 1; j < Count; j++)
	{
		// passing all lengths for not existing  flexia
		if (m_FlexLengthTable[j] == MaxFlexiaLength) continue;

		idxhigh = m_FlexLengthTable[j];
		break;
	}
	if (idxhigh == idxlow)
		idxhigh = m_Flexs.size();

	/*
		period [idxlow..idxhigh) contains a sorted vector of flexia of length curlen
	*/

	vector<CShortString>::const_iterator locflex = lower_bound(	
							m_Flexs.begin() + idxlow,
							m_Flexs.begin() + idxhigh,
							flex, LessShortString);

	if (	(locflex != m_Flexs.begin() + idxhigh)
		&&	!strcmp(flex,locflex->GetString())
	   )
		return locflex - m_Flexs.begin();
	else
		return UpperBoundOfFlexiaCount;
}


bool	CMorphDictionary::FlexFind(const string& InputWordStr,  CResultVector& FlexResults)  const
{
	FlexResults.Clear();
	size_t length = InputWordStr.length();

	for (int curlen = min(length, m_FlexLengthTable.size() - 1);
		curlen != -1;
		curlen -- )
	{
		WORD index = GetFlexNoByFlexStr(InputWordStr.c_str()+length-curlen, curlen);
		if (index != UpperBoundOfFlexiaCount)
			FlexResults.Add(index);
	}
	return !FlexResults.empty();
}


void CMorphDictionary::BaseFind(const string& InputWordStr, const WORD flex, CFindWordNode& node) const
{
	node.m_bFound  = false;
	WORD nFlexLen = m_Flexs[flex].GetLength();
	WORD nBaseLen = InputWordStr.length() - nFlexLen;
	node.m_nFlex = flex;

	char find_src[MAX_STRING_LEN];
	assert(InputWordStr.length() - nFlexLen + 1 < MAX_STRING_LEN);
	strncpy(find_src, InputWordStr.c_str(), InputWordStr.length() - nFlexLen);
	find_src[InputWordStr.length() - nFlexLen] = 0;

	vector<CShortString>::const_iterator it =  lower_bound(m_Bases.begin(),  m_Bases.end(), find_src, LessShortString);
	node.m_nBase = it - m_Bases.begin();
	if (it == m_Bases.end()) return;
	const char* debug = it->GetString();
	if ( strcmp(it->GetString(), find_src) ) return;
	node.m_bFound = true;
}


void _lemmatizer_intersection(
		vector<CLemmaInfo>::const_iterator _First1, vector<CLemmaInfo>::const_iterator _Last1,
		vector<WORD>::const_iterator _First2, vector<WORD>::const_iterator _Last2, 
		vector<CFindWordNode>& results,
		CFindWordNode& Prototype)
{	
	for (; _First1 != _Last1 && _First2 != _Last2; )
		if (_First1->m_FlexiaModelNo < *_First2)
			++_First1;
		else if (*_First2 < _First1->m_FlexiaModelNo)
			++_First2;
		else
		{
			Prototype.m_LemmaInfo = *_First1;
			results.push_back(Prototype);
			_First1++;
			_First2++;
		};
		
}


bool CMorphDictionary::FindBases(const string& InputWordStr, const  CResultVector& FlexResults, vector<CFindWordNode>& results) const 
{
	CFindWordNode node;
	for (int i = 0; i < FlexResults.size(); i++)
	{
		
		BaseFind(InputWordStr, FlexResults[i], node);

		if (node.m_bFound)
		{

			vector<CLemmaInfo>::const_iterator base_start = m_Base2LemmaInfos.GetVectorBegin(node.m_nBase);
			vector<CLemmaInfo>::const_iterator base_end = m_Base2LemmaInfos.GetVectorEnd(node.m_nBase);
			vector<WORD>::const_iterator flex_start = m_Flex2FlexiaModels.GetVectorBegin(node.m_nFlex);
			vector<WORD>::const_iterator flex_end = m_Flex2FlexiaModels.GetVectorEnd(node.m_nFlex);
			_lemmatizer_intersection(	base_start, base_end, 
											flex_start, flex_end,
											results, node);

		}
	}
	return !results.empty();
}



// loading simple list of words from a file, lemmatizing it, and storing
void CMorphDictionary::LoadFrequentRoots(string path)
{
	string load_path = path+"predictroots.txt";
	FILE*fp =  fopen (load_path.c_str(), "r");
	if (!fp) return;
	char buffer[1000];
	while (fgets (buffer, 1000, fp))
	{
		string WordStr = buffer;
		Trim(WordStr);
		if (WordStr.empty()) continue;
		RmlMakeUpper (WordStr, m_Language);
		vector<CFindWordNode> FindResults;
		bool retval = FindWord(WordStr, true, false, false, FindResults);
		if (!retval) continue;
		set<size_t> UsedFlexia;
		for (size_t i=0; i<FindResults.size(); i++)
		{
				CFormInfo P;
				P.Create(	this, 
							FindResults[i].m_nBase,
							FindResults[i].m_LemmaInfo,
							FindResults[i].m_nFlex
						);
				for (size_t j=0; j < P.GetCount(); j++)
				{
					size_t FlexNo = P.GetFlexNoOfForm(j);
					if (UsedFlexia.find(FlexNo) == UsedFlexia.end())
					{
						CPredictEndRoot R;
						R.m_BaseNo = FindResults[i].m_nBase;
						R.m_LemmaInfo = FindResults[i].m_LemmaInfo;
						R.m_EndRoot = P.GetWordForm(j);
						reverse(R.m_EndRoot.begin(),R.m_EndRoot.end());
						
						R.m_FlexNo = FlexNo;
						UsedFlexia.insert(FlexNo);
						m_PredictEndRoots.push_back(R);
					};
				};
			

		};

	};
	fclose(fp);
	sort(m_PredictEndRoots.begin(), m_PredictEndRoots.end());
};


bool CMorphDictionary::PredictByEndRoot(const string& InputWordStr,  vector<CFindWordNode>& results) const  
{
	if (m_PredictEndRoots.empty()) return false;
	assert(results.empty());
	

	CPredictEndRoot R;
	R.m_EndRoot = InputWordStr;
	reverse(R.m_EndRoot.begin(), R.m_EndRoot.end());
	vector<CPredictEndRoot>::const_iterator it = lower_bound(m_PredictEndRoots.begin(), m_PredictEndRoots.end(),R);
	if (it == m_PredictEndRoots.begin()) return false;
	it--;
	while (	it->m_EndRoot == R.m_EndRoot.substr(0,it->m_EndRoot.length()) )
	{
		string debug = it->m_EndRoot;
		CFindWordNode node;
		node.m_bFound = false;
		node.m_nBase = it->m_BaseNo;
		node.m_nFlex = it->m_FlexNo;
		node.m_LemmaInfo = it->m_LemmaInfo;
		node.m_nWeight = UndescribedWeight;
		results.push_back(node);
		it--;
		if (it == m_PredictEndRoots.begin()) break;
	};
	

	return !results.empty();
}

bool CMorphDictionary::PredictByDataBase(const string& InputWordStr,  const  CResultVector& FlexResults,  vector<CFindWordNode>& results,bool is_cap,bool max_predict) const  
{
	if (PredictByEndRoot(InputWordStr,  results))
		return true;

	if( FlexResults.empty() )
		return false;

	vector<CPredictSet> res;

	bool bPlugWordWasInserted = false;
	if( is_cap && (m_Language != morphGerman))
	{
		m_Predict.Find(0,"***",res); 
		bPlugWordWasInserted = true;
	};
	assert( InputWordStr.length() >=  m_Predict.m_CountOfLetters );
	string Postfix = InputWordStr.substr(InputWordStr.length() -  m_Predict.m_CountOfLetters);
	
	for( int i=0; i<FlexResults.size(); i++ )
	{
		WORD flexno = FlexResults[i];
		assert(flexno < m_Flexs.size() );
		if (m_Flexs[flexno].GetLength() < InputWordStr.length())
			m_Predict.Find(flexno,Postfix,res);
	};

	// проверим наличие существительного
	bool has_noun = false;
	int j=0;
	for(; j<res.size(); j++ )
	{
		if( res[j].m_PartOfSpeechNo == 0 )
		{
			has_noun = true;
			break;
		}
	}
// и добавим его если надо		
	if( !has_noun )
		if (!bPlugWordWasInserted)
		{
			m_Predict.Find(0,"***",res); // # ** Ќ≈”Ѕ»¬ј…ћ≈Ќя
			bPlugWordWasInserted = true;
		};

	
// заполним выходной массив 
	vector<bool> has_nps(32,false); // assume not more than 32 different pos

	for( j=0; j<res.size(); j++ )
	{
		BYTE PartOfSpeechNo = res[j].m_PartOfSpeechNo;
		assert( PartOfSpeechNo < has_nps.size());
		if( !max_predict && has_nps[PartOfSpeechNo] )
			continue;

		has_nps[PartOfSpeechNo] = true;
		if( j==0 && is_cap )
			has_nps[PartOfSpeechNo] = false;

		CFindWordNode node;
		node.m_bFound = false;
		node.m_nBase = res[j].m_BaseNo;
		const char *debug = m_Bases[node.m_nBase].GetString();
		node.m_nFlex = res[j].m_FlexNo;
		node.m_LemmaInfo = res[j].m_LemmaInfo;
		node.m_nWeight = UndescribedWeight;
		results.push_back(node);
	}

	return (!results.empty());
}


bool CMorphDictionary::Load(MorphLanguageEnum Language, const string& path)
{
	
	m_Language = Language;

	if (!m_Flex2FlexiaModels.LoadVectorMap(path + FLEX2PARADIGMS_BIN_PATH))
		return false;

	if (!m_Base2LemmaInfos.LoadVectorMap(path + BASE2PARADIGMS_BIN_PATH))
		return false;

	if (!m_FlexModels.LoadVectorMap(path + PARADIGMS_BIN_PATH))
		return false;

	if (!m_AccentModels.LoadVectorMap(path + ACCENT_MODELS_BIN_PATH))
		return false;

	ReadVector(path + FLEXLEN_BIN_PATH, m_FlexLengthTable);


	if (m_FlexLengthTable.empty()) //  there should be at least empty flexia in m_FlexLengthTable
		return false;

	m_Flexs.ReadShortStringHolder(path + FLEXS_BIN_PATH);
	m_Infos.ReadShortStringHolder(path + INFOS_BIN_PATH);
	m_Bases.ReadShortStringHolder(path + BASES_BIN_PATH);

			
	m_Predict.Load(path + PREDICT_BIN_PATH);

	LoadFrequentRoots(path);
	return true;
}

bool CMorphDictionary::FindWord(string& InputWordStr, bool cap, bool predict, bool max_predict, vector<CFindWordNode>& results) const
{
	size_t nLen = InputWordStr.length();
	if (	(nLen == 0)
		||	(nLen > GLOBAL_MAX_WORD_LEN))
		return false;

	RmlMakeUpper (InputWordStr, m_Language);

	CResultVector FlexResults;

	bool retval = FlexFind(InputWordStr, FlexResults);
	if( !retval )
		return( false );

	retval = FindBases(InputWordStr, FlexResults, results);
	if( retval )
		return( true );
	
	if( !predict )
		return( false );

	
	if( InputWordStr.length() < m_Predict.m_CountOfLetters )
		return( false );

	retval = PredictByDataBase(InputWordStr, FlexResults, results,cap,max_predict);
	
	return (retval);
}

bool	CMorphDictionary::IsNorm(const WORD flex, const WORD paradigm) const
{
	CParadigmItemPtr End = m_FlexModels.GetVectorEnd(paradigm);
	CParadigmItemPtr Begin = m_FlexModels.GetVectorBegin(paradigm);
	return (	(End != Begin)
			&&	(Begin->m_nFlex == flex));
}
