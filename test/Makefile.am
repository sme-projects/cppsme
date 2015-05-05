noinst_PROGRAMS = adder ring
EXTRA_DISt = ring.py
CLEANFILES = ring.cc

adder_CPPFLAGS = -I../src
adder_LDADD = ../src/libsme.la
adder_LDFLAGS = -static
adder_SOURCES = adder.cc

#frogger_CPPFLAGS = -I../src
#frogger_LDADD = ../src/libsme.la
#frogger_LDFLAGS = -static
#frogger_SOURCES = frogger.cc

#simplecpu_CPPFLAGS = -I../src
#simplecpu_LDADD = ../src/libsme.la 
#simplecpu_LDFLAGS = -static
#simplecpu_SOURCES = simplecpu.cc

ring_CPPFLAGS = -I../src
ring_LDADD = ../src/libsme.la
ring_LDFLAGS = -static
ring_SOURCES = ring.cc

ring.cc: ring.py
	$(PYTHON) $< > $@
