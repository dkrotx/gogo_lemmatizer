// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1996-2001)

#ifndef  Descriptors_h
#define Descriptors_h

#ifndef __GRAPHANLib_LIBRARY_DEFINED__

enum Descriptors 
{
	 BTxt  = 0, ORLE   = 1, OLLE  = 2, ODel  = 3, OPun  = 4, 
	 ODg   = 5, ODgCh = 6, ETxt  = 7, OUnk  = 8, 
	 OSpc  = 9, OHyp  = 10, OEOLN = 11, OEOS  = 12, OEOP  = 13, 
	 OPlu  = 14, OUp   = 15, OLw   = 16, OUpLw = 17, 
	 ONam  = 18, OOpn  = 19, OCls  = 20,
	 ONil  = 21, OBullet = 22, OPar    = 23,
	 OPass = 24, OLongDelimSeq = 25, OParagraph = 26,


	 CS_Undef   = 27, CS_Simple  = 28, CS_Quasi   = 29, 
	 CS_Parent  = 30, CS_Heading = 31, CS_Explan  = 32, CS_Doc     = 33, 


	 OOb1    = 34, OOb2    = OOb1+1,    
	 OFAM1   = 36, OFAM2   = OFAM1+1, 
	 OSHyp1  = 38, OSHyp2  = OSHyp1+1, 
	 ORef1      = 40, ORef2      = ORef1+1, 
	 ODate1     = 42, ODate2     = ODate1+1,
	 OFloat1 = 44, 	 OFloat2 = OFloat1+1, 
	 OElectAddr1 = 46, OElectAddr2 = OElectAddr1+1,
	 OAbbr1 = 48, OAbbr2 = OAbbr1+1,
	 OSentStart = 50, OSentEnd = OSentStart+1, 
	 OKey1 =  52, OKey2 =  OKey1+1,
	 OGerDivComp1 =  54, OGerDivComp2 =  OGerDivComp1+1,
};
#endif

#endif
