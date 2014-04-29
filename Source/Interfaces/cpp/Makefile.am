lib_LTLIBRARIES = libSimpleLemmatiser.la

INCLUDES = -I$(top_builddir)/Sources

libSimpleLemmatiser_la_SOURCES = CMorphology.cpp CMorphology.hpp converts.cpp LemInterface.cpp \
                                 LemInterface.hpp SimpleLemmatiser.cpp SimpleLemmatiser.hpp \
                                 utf_cp.cpp utf_cp.hpp

libSimpleLemmatiser_la_LIBADD = $(top_srcdir)/Source/LemmatizerLib/libLemmatizer.la   \
                                $(top_srcdir)/Source/AgramtabLib/libAgramtab.la       \
                                $(top_srcdir)/Source/GraphanLib/libGraphan.la         \
                                $(top_srcdir)/Source/MorphWizardLib/libMorphWizard.la \
                                $(top_srcdir)/Source/StructDictLib/libStructDict.la

