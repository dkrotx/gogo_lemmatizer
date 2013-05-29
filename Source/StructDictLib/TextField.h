// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Structural Dictionary (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1998-2002)


#ifndef __TEXTFIELD_H_
#define __TEXTFIELD_H_



struct CTextField  
{
	BYTE  FieldNo;
	BYTE  LeafId;
	BYTE  BracketLeafId;
	DWORD StartLine;
	DWORD EndLine;

	CTextField() {};
	CTextField (BYTE _FieldNo, BYTE _LeafId, BYTE _BracketLeafId)
	{
	  LeafId = _LeafId;
	  FieldNo = _FieldNo;
	  BracketLeafId = _BracketLeafId;
	};

	bool operator==(const CTextField& _X) const
	{
		  return    (FieldNo == _X.FieldNo)
			        && (LeafId  == _X.LeafId)
					&& (BracketLeafId  == _X.BracketLeafId);
	}

};


#endif //__TEXTFIELD_H_
