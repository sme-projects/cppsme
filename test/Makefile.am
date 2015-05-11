noinst_PROGRAMS = adder ring sleeping_ring ring_dynamic
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

# var-tracking makes the build take forever
ring_CPPFLAGS = -I../src -fno-var-tracking
ring_LDADD = ../src/libsme.la
ring_LDFLAGS = -static
ring_SOURCES = ring.cc

sleeping_ring_CPPFLAGS = -I../src -fno-var-tracking
sleeping_ring_LDADD = ../src/libsme.la
sleeping_ring_LDFLAGS = -static
sleeping_ring_SOURCES = sleeping_ring.cc

ring_dynamic_CPPFLAGS = -I../src -fno-var-tracking
ring_dynamic_LDADD = ../src/libsme.la
ring_dynamic_LDFLAGS = -static
ring_dynamic_SOURCES = ring_dynamic.cc


ring.cc: ring.py
	$(PYTHON) $< > $@
