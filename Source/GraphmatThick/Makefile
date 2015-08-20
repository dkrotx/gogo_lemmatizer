include ../common/common_mak

ExeBaseName = GraphmatThick

object_base_names = GraphmatThick

# The libs order must be from libraries that need symbols to the ones that
# provide them, otherwise the static linker would fail:
libs = ../GraphanLib/libGraphan$(ct).$(lib_ext) \
       ../StructDictLib/libStructDict$(ct).$(lib_ext)

include ../common/common_exe_mak
