noinst_PROGRAMS = adder ring sleeping_ring ring_dynamic validate ring_heavy ring_uneven
EXTRA_DISt = ring.py
CLEANFILES = ring.cc

static_gcc = -static-libstdc++ -static-libgcc

adder_CPPFLAGS = -I../src
adder_LDADD = ../src/libsme.la
adder_LDFLAGS = -static $(static_gcc)
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
ring_LDFLAGS = -static $(static_gcc)
ring_SOURCES = ring.cc

sleeping_ring_CPPFLAGS = -I../src -fno-var-tracking
sleeping_ring_LDADD = ../src/libsme.la
sleeping_ring_LDFLAGS = -static $(static_gcc)
sleeping_ring_SOURCES = sleeping_ring.cc

ring_heavy_CPPFLAGS = -I../src -fno-var-tracking
ring_heavy_LDADD = ../src/libsme.la
ring_heavy_LDFLAGS = -static $(static_gcc)
ring_heavy_SOURCES = ring_heavy.cc

ring_dynamic_CPPFLAGS = -I../src -fno-var-tracking
ring_dynamic_LDADD = ../src/libsme.la
ring_dynamic_LDFLAGS = -static $(static_gcc)
ring_dynamic_SOURCES = ring_dynamic.cc

validate_CPPFLAGS = -I../src -fno-var-tracking
validate_LDADD = ../src/libsme.la
validate_LDFLAGS = -static $(static_gcc)
validate_SOURCES = validate.cc

ring_uneven_CPPFLAGS = -I../src -fno-var-tracking
ring_uneven_LDADD = ../src/libsme.la
ring_uneven_LDFLAGS = -static $(static_gcc)
ring_uneven_SOURCES = ring_uneven.cc

ring.cc: ring.py
	$(PYTHON) $< > $@
