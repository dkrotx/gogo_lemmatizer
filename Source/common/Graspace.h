
// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1996-2001)

#ifndef graspace_h
 #define graspace_h

   #define   SEOLN        (unsigned char) '´'
   #define   SSpace       (unsigned char) 'Å'
   #define   STab         '\x10'

   inline bool is_gra_space (int c)
    { return c==SEOLN || c == SSpace || c == STab;}

   const size_t MaxGraLen = 60;

#endif
