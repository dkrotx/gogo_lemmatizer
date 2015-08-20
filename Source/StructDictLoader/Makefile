include ../common/common_mak

ExeBaseName = StructDictLoader

object_base_names = Main

# The libs order must be from libraries that need symbols to the ones that
# provide them, otherwise the static linker would fail:
libs = ../StructDictLib/libStructDict$(ct)

include ../common/common_exe_mak
