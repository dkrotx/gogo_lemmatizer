include ../common/common_mak

ExeBaseName = TestLem

object_base_names = TestLem ../common/utilit

# The libs order must be from libraries that need symbols to the ones that
# provide them, otherwise the static linker would fail:
libs = ../LemmatizerLib/libLemmatizer$(ct).$(lib_ext) \
       ../GraphanLib/libGraphan$(ct).$(lib_ext) \
       ../MorphWizardLib/libMorphWizard$(ct).$(lib_ext) \
       ../AgramtabLib/libAgramtab$(ct).$(lib_ext) \
       ../StructDictLib/libStructDict$(ct).$(lib_ext)


include ../common/common_exe_mak
