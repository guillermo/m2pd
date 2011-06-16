CFLAGS=-g -Wall -arch i386 -lzmq
LIBRARY_PATH="../../libzeromq-32/lib"
CFLAGS=-g -arch i386 -lzmq

PDROOT=/Applications/Pd-extended.app/Contents/Resources
PDSRCDIR=$(PDROOT)/src
PDLIBDIR=$(PDROOT)/bin
PDINCLUDE=$(PDROOT)/include

UNCRUSTIFY=/usr/local/Cellar/uncrustify/0.57/bin/uncrustify
UNCRUSTIFY_CFG=/usr/local/Cellar/uncrustify/0.57/share/uncrustify/defaults.cfg
DARWINCFLAGS = -O2 -Wall -W -Wshadow -Wstrict-prototypes -W 

m2pd.pd_darwin: m2_client.c m2pd.c m2pd.h m2_client.h zhelpers.h
	LIBRARY_PATH=$(LIBRARY_PATH) $(CC) $(CFLAGS) -I$(PDINCLUDE) -bundle -undefined suppress -flat_namespace -O2 -Wall -Wstrict-prototypes -o m2pd.pd_darwin m2_client.c m2pd.c


all: m2pd.pd_darwin

pretty: 
	$(UNCRUSTIFY) -c $(UNCRUSTIFY_CFG) --replace --no-backup -l C *.c *.h

clean:
	rm -Rf *.dSYM client server a.out m2pd m2pd.pd_darwin
