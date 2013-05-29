include ../common/common_mak

cparams := $(cparams) -I../Interfaces/c

ExeName=RegressionTest$(ExeExten)

objects=main.o$(ct)

libs = ../Interfaces/c/libclemmatiser$(ct).$(lib_ext) \
	   ../Interfaces/cpp/libSimpleLemmatiser$(ct).$(lib_ext) \
	   ../LemmatizerLib/libLemmatizer$(ct).$(lib_ext) \
       ../AgramtabLib/libAgramtab$(ct).$(lib_ext)     \
       ../GraphanLib/libGraphan$(ct).$(lib_ext)       \
       ../StructDictLib/libStructDict$(ct).$(lib_ext) \
       ../MorphWizardLib/libMorphWizard$(ct).$(lib_ext) 

include ../common/common_exe_mak

