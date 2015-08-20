ifndef mode
	mode = release
endif

include ../../../Source/common/common_mak

cparams := $(cparams) -I../../../Source/Interfaces/cpp

ExeBaseName = lemmatize2

object_base_names = lemmatize2

# The libs order must be from libraries that need symbols to the ones that
# provide them, otherwise the static linker would fail:
libs = ../../../Source/Interfaces/c/libclemmatiser$(ct).$(lib_ext) \
       ../../../Source/Interfaces/cpp/libSimpleLemmatiser$(ct).$(lib_ext) \
       ../../../Source/LemmatizerLib/libLemmatizer$(ct).$(lib_ext) \
       ../../../Source/MorphWizardLib/libMorphWizard$(ct).$(lib_ext) \
       ../../../Source/AgramtabLib/libAgramtab$(ct).$(lib_ext) \
       ../../../Source/GraphanLib/libGraphan$(ct).$(lib_ext) \
       ../../../Source/StructDictLib/libStructDict$(ct).$(lib_ext)

include ../../../Source/common/common_exe_mak
