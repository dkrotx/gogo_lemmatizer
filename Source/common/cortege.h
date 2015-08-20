// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Structural Dictionary (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1998-2002)


#ifndef cortege_h
 #define cortege_h

#include <assert.h>
#include "../common/utilit.h"

const BYTE ErrUChar	= 254;

struct TCortege10;

const char _FieldFormat[] = "%-8s= %s";
const char _FieldFormatEqual[] = "%-8s== %s";


template <int MaxNumDom>
struct TBasicCortege {
public:
	BYTE m_FieldNo;      // индекс в массиве Ross :: Fields
	BYTE m_SignatNo;  // индекс в массиве Ross :: Fields[FunNo].m_Signats
	BYTE m_LevelId;
	BYTE m_LeafId;
	BYTE m_BracketLeafId;
	int  m_DomItemNos[MaxNumDom];  

	long GetItem  (size_t index)  const
	{
		assert (index<MaxNumDom);
		return m_DomItemNos[index];
	};
	long SetItem  (size_t index, long Value) 
	{
		assert (index<MaxNumDom);
		return m_DomItemNos[index] =  Value;
	};

	TBasicCortege()
	{
	  for (size_t i=0; i< MaxNumDom;i++)
		  SetItem(i,-1);
	  m_SignatNo = 0;
	  m_FieldNo = ErrUChar;
	  m_LeafId = 0;
	  m_BracketLeafId = 0;
	};
	BYTE GetSignatNo()  const
	{
	  return (m_SignatNo  & (~128)) ;
	};

	void SetSignatNo(BYTE SignatNo) 
	{
	  m_SignatNo = SignatNo | (128 & m_SignatNo);
	};

	bool IsEqual() const
	{
	   return (m_SignatNo  & 128) > 0 ;
	};

	void SetEqual()
	{
	   m_SignatNo  |=  128;
	};
	const  char* GetFieldFormat() const
	{
	  return IsEqual() ? _FieldFormatEqual : _FieldFormat;

	};

	bool HasEqualItems(const TBasicCortege& value, BYTE maxNumDom)  const 
	{  
		for (BYTE i=0; i< maxNumDom; i++)
			if (GetItem(i) != value.GetItem(i))
				return false;
		return true;
	};

	bool EqualCortege(const TBasicCortege& value, BYTE maxNumDom)  const 
	{  
	  return    (m_FieldNo  == value.m_FieldNo)  
	             && (m_SignatNo == value.m_SignatNo)
				 && (m_LevelId  == value.m_LevelId)
				 && (m_LeafId   == value.m_LeafId)
				 && (m_BracketLeafId   == value.m_BracketLeafId)
				 && HasEqualItems (value, maxNumDom);
	};

	bool IsEqualWithWildCard(const TBasicCortege& value, WORD EmptyDomItem, BYTE maxNumDom) const
	{  
		if    (!(   (m_FieldNo  == value.m_FieldNo)  
			 && (      (m_LevelId == ErrUChar)
					|| (value.m_LevelId == ErrUChar)
					|| (m_LevelId == value.m_LevelId)
				)
			 && (      (m_LeafId == ErrUChar)
					|| (value.m_LeafId == ErrUChar)
					|| (m_LeafId == value.m_LeafId)
				)
			 && (      (m_BracketLeafId == ErrUChar)
					|| (value.m_BracketLeafId == ErrUChar)
					|| (m_BracketLeafId == value.m_BracketLeafId)
				)

			 )) return false;

		for (size_t i = 0; i < maxNumDom; i++)
		   if (     (GetItem(i)  != value.GetItem(i))
				&&  (GetItem(i)  != EmptyDomItem)
				&&  (value.GetItem(i) != EmptyDomItem)
			  )
		   return false; 

		return true;
	};

	TBasicCortege<MaxNumDom>& operator = (const TBasicCortege<10>& value)
	{
		m_FieldNo = value.m_FieldNo;
		m_LeafId = value.m_LeafId;
		m_BracketLeafId = value.m_BracketLeafId;
		m_LevelId = value.m_LevelId;
		m_SignatNo = value.m_SignatNo;
		for (int i = 0; i < MaxNumDom; i++)
			SetItem(i, value.GetItem(i));

		return *this;
	};

};


template <int MaxNumDom>
size_t get_size_in_bytes (const TBasicCortege<MaxNumDom>& t)
{

	return	get_size_in_bytes(t.m_FieldNo) 
			+ get_size_in_bytes(t.m_SignatNo) 
			+ get_size_in_bytes(t.m_LevelId) 
			+ get_size_in_bytes(t.m_LeafId) 
			+ get_size_in_bytes(t.m_BracketLeafId) 
			+ get_size_in_bytes(t.m_DomItemNos[0])*MaxNumDom; 
};

template <int MaxNumDom>
size_t save_to_bytes(const TBasicCortege<MaxNumDom>& i, BYTE* buf)
{
	buf += save_to_bytes(i.m_FieldNo, buf);
	buf += save_to_bytes(i.m_SignatNo, buf);
	buf += save_to_bytes(i.m_LevelId, buf);
	buf += save_to_bytes(i.m_LeafId, buf);
	buf += save_to_bytes(i.m_BracketLeafId, buf);
	for (int j = 0;  j < MaxNumDom; j++)
		buf += save_to_bytes(i.m_DomItemNos[j], buf);

	return get_size_in_bytes(i);
};

template <int MaxNumDom>
size_t restore_from_bytes(TBasicCortege<MaxNumDom>& i, const BYTE* buf)
{
	buf += restore_from_bytes(i.m_FieldNo, buf);
	buf += restore_from_bytes(i.m_SignatNo, buf);
	buf += restore_from_bytes(i.m_LevelId, buf);
	buf += restore_from_bytes(i.m_LeafId, buf);
	buf += restore_from_bytes(i.m_BracketLeafId, buf);
	for (int j = 0;  j < MaxNumDom; j++)
		buf += restore_from_bytes(i.m_DomItemNos[j], buf);
	return get_size_in_bytes(i);
};




struct TCortege10 : public TBasicCortege<10> 
{
	TCortege10()
	{
		for (size_t i=0; i< 10;i++)
			SetItem(i,-1);
		m_SignatNo = 0;
		m_FieldNo = ErrUChar;
		m_LeafId = 0;
		m_BracketLeafId = 0;
	};

	TCortege10 (const TBasicCortege<3>& copySource)
	{
		m_FieldNo = copySource.m_FieldNo;
		m_LeafId = copySource.m_LeafId;
		m_BracketLeafId = copySource.m_BracketLeafId;
		m_LevelId = copySource.m_LevelId;
		m_SignatNo = copySource.m_SignatNo;
		for (int i = 0; i < 3; i++)
			SetItem(i, copySource.GetItem(i));
	};
	TCortege10 (const TBasicCortege<10>& copySource)
	{
		m_FieldNo = copySource.m_FieldNo;
		m_LeafId = copySource.m_LeafId;
		m_BracketLeafId = copySource.m_BracketLeafId;
		m_LevelId = copySource.m_LevelId;
		m_SignatNo = copySource.m_SignatNo;
		for (int i = 0; i < 10; i++)
			SetItem(i, copySource.GetItem(i));
	};


};







#endif
