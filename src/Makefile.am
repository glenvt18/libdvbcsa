
SUBDIRS = dvbcsa

lib_LTLIBRARIES = libdvbcsa.la

libdvbcsa_la_SOURCES = \
	dvbcsa_algo.c dvbcsa_block.c dvbcsa_stream.c dvbcsa_key.c \
	dvbcsa_bs_algo.c dvbcsa_bs_block.c dvbcsa_bs_stream.c dvbcsa_bs_key.c \
	dvbcsa_bs_transpose_block.c \
	dvbcsa_bs_stream_kernel.inc dvbcsa_bs_stream_kernel.h \
	dvbcsa_bs.h dvbcsa_pv.h dvbcsa_bs_transpose.h \
	dvbcsa_bs_uint32.h dvbcsa_bs_uint64.h \
	dvbcsa_bs_mmx.h dvbcsa_bs_sse.h \
	dvbcsa_bs_avx2.h \
	dvbcsa_bs_altivec.h dvbcsa_bs_neon.h

if TRANSPOSE_32
libdvbcsa_la_SOURCES += dvbcsa_bs_transpose_stream32.c
else
libdvbcsa_la_SOURCES += dvbcsa_bs_transpose_stream.c
endif

libdvbcsa_la_LDFLAGS = -version-info 1:1:0 $(libtool_flags)

