// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1996-2001)

#ifndef abbrev_h
#define abbrev_h

#include  "list"

const  size_t WordSize  = 50;
const  size_t AbbrevSize  = 200;

enum AbbrevItemTypeEnum  {abString, abNumber, abUpperCase, abAny};
struct  CAbbrevItem 
{
	AbbrevItemTypeEnum	m_Type;
	string				m_ItemStr;
	bool operator == (const CAbbrevItem _X) const;
	bool operator < (const CAbbrevItem _X) const;

	
};
typedef list<CAbbrevItem> CAbbrev;

/*struct  CAbbrev  {
	
	char				m_AbbrevStr[AbbrevSize];
	size_t				m_AbbrevLen;
	size_t				m_MinLenInText;
	char				m_FirstWordStr[WordSize];
	size_t				m_FirstWordLen;
};*/



#endif
