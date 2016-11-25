bin_PROGRAMS = hfst-ospell-cg

hfst_ospell_cg_SOURCES = main.cpp util.hpp json.hpp ospell-cg.cpp ospell-cg.hpp cxxopts.h

hfst_ospell_cg_CXXFLAGS = $(HFSTOSPELL_CFLAGS) $(HFST_CFLAGS) $(PUGIXML_CFLAGS)
hfst_ospell_cg_LDADD = $(HFSTOSPELL_LIBS) $(HFST_LIBS) $(PUGIXML_LIBS)
