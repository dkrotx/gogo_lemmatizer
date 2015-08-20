include ../common/common_mak

ExeBaseName = FileLem

object_base_names = FileLem  \
		    ../common/MorhologyHolder

# The libs order must be from libraries that need symbols to the ones that
# provide them, otherwise the static linker would fail:
libs = ../GraphanLib/libGraphan$(ct).$(lib_ext) \
       ../LemmatizerLib/libLemmatizer$(ct).$(lib_ext) \
       ../MorphWizardLib/libMorphWizard$(ct).$(lib_ext) \
       ../AgramtabLib/libAgramtab$(ct).$(lib_ext) \
       ../StructDictLib/libStructDict$(ct).$(lib_ext)

include ../common/common_exe_mak

