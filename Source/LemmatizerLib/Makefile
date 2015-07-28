# common_parms := -I. -I../AgramtabLib -I../GraphanLib -I../StructDictLib

VPATH = ../

common_parms := -I.
include ../common/common_mak
LibBaseName = Lemmatizer


object_base_names = Lemmatizers \
		    Paradigm \
		    CreatePredictionBase \
		    PLMLineCollection \
		    Statistic \
		    MorphAutomat \
		    MorphAutomBuilder \
		    MorphDict \
		    MorphDictBuilder \
		    Predict \
		    ../common/PlmLine \
		    ../common/MorhologyHolder

libs = ../AgramtabLib/libAgramtab$(ct).$(lib_ext) \
       ../GraphanLib/libGraphan$(ct).$(lib_ext) \
       ../MorphWizardLib/libMorphWizard$(ct).$(lib_ext)


include ../common/common_lib_mak
