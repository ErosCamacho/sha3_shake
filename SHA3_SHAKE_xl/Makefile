
CC=/usr/bin/cc
LDFLAGS=-lcrypto -lpynq -lcma -lpthread -lm
CFLAGS = -DCRYPTO_NAMESPACE\(s\)=SHA3_\#\#s

SOURCES_TEST = sha3/sw/sha3.c sha3/hw/sha3_hw.c sha3/common/Funcion_Test.c sha3/common/Funciones_HW.c sha3_test.c
HEADERS_TEST = sha3/sw/sha3.h sha3/hw/sha3_hw.h sha3/common/Funcion_Test.h sha3/common/Funciones_HW.h 

SOURCES_DEMO = sha3/sw/sha3.c sha3/hw/sha3_hw.c sha3/common/Funcion_Test.c sha3/common/Funciones_HW.c sha3_demo.c
HEADERS_DEMO = sha3/sw/sha3.h sha3/hw/sha3_hw.h sha3/common/Funcion_Test.h sha3/common/Funciones_HW.h 

SOURCES_TEST_SHAKE = sha3/sw/sha3.c sha3/hw/shake_hw.c sha3/common/Funcion_Test.c sha3/common/Funciones_HW.c shake_test.c
HEADERS_TEST_SHAKE = sha3/sw/sha3.h sha3/hw/shake_hw.h sha3/common/Funcion_Test.h sha3/common/Funciones_HW.h 

SOURCES_DEMO_SHAKE = sha3/sw/sha3.c sha3/hw/shake_hw.c sha3/common/Funcion_Test.c sha3/common/Funciones_HW.c shake_demo.c
HEADERS_DEMO_SHAKE = sha3/sw/sha3.h sha3/hw/shake_hw.h sha3/common/Funcion_Test.h sha3/common/Funciones_HW.h 

all: sha3_512_spirs sha3_384_spirs sha3_256_spirs sha3_224_spirs sha3_512_test sha3_384_test sha3_256_test sha3_224_test shake_128_test shake_128_spirs shake_256_test shake_256_spirs
#all: shake_128_test shake_128_spirs shake_256_test shake_256_spirs

sha3_512_test: $(HEADERS_TEST) $(SOURCES_TEST)
	$(CC) -DSHA3_512 -o $@ $(SOURCES_TEST) $(LDFLAGS)

sha3_384_test: $(HEADERS_TEST) $(SOURCES_TEST)
	$(CC) -DSHA3_384 -o $@ $(SOURCES_TEST) $(LDFLAGS)

sha3_256_test: $(HEADERS_TEST) $(SOURCES_TEST)
	$(CC) -DSHA3_256 -o $@ $(SOURCES_TEST) $(LDFLAGS)

sha3_224_test: $(HEADERS_TEST) $(SOURCES_TEST)
	$(CC) -DSHA3_224 -o $@ $(SOURCES_TEST) $(LDFLAGS)

sha3_512_demo: $(HEADERS_DEMO) $(SOURCES_DEMO)
	$(CC) -DSHA3_512 -o $@ $(SOURCES_DEMO) $(LDFLAGS)

sha3_384_demo: $(HEADERS_DEMO) $(SOURCES_DEMO)
	$(CC) -DSHA3_384 -o $@ $(SOURCES_DEMO) $(LDFLAGS)

sha3_256_demo: $(HEADERS_DEMO) $(SOURCES_DEMO)
	$(CC) -DSHA3_256 -o $@ $(SOURCES_DEMO) $(LDFLAGS)

sha3_224_demo: $(HEADERS_DEMO) $(SOURCES_DEMO)
	$(CC) -DSHA3_224 -o $@ $(SOURCES_DEMO) $(LDFLAGS)

shake_128_test: $(HEADERS_TEST_SHAKE) $(SOURCES_TEST_SHAKE)
	$(CC) -DSHAKE128 -o $@ $(SOURCES_TEST_SHAKE) $(LDFLAGS)

shake_128_demo: $(HEADERS_DEMO_SHAKE) $(SOURCES_DEMO_SHAKE)
	$(CC) -DSHAKE128 -o $@ $(SOURCES_DEMO_SHAKE) $(LDFLAGS)

shake_256_test: $(HEADERS_TEST_SHAKE) $(SOURCES_TEST_SHAKE)
	$(CC) -DSHAKE256 -o $@ $(SOURCES_TEST_SHAKE) $(LDFLAGS)

shake_256_demo: $(HEADERS_DEMO_SHAKE) $(SOURCES_DEMO_SHAKE)
	$(CC) -DSHAKE256 -o $@ $(SOURCES_DEMO_SHAKE) $(LDFLAGS)

#sha3_spirs: $(HEADERS_SPIRS) $(SOURCES_SPIRS)
#	$(CC) -DCRYPTO_NAMESPACE\(s\)=SHA3_\#\#s -o $@ $(SOURCES_SPIRS) $(LDFLAGS)

.PHONY: all sha3_512_test sha3_384_test sha3_256_test sha3_224_test sha3_512_spirs sha3_384_spirs sha3_256_spirs sha3_224_spirs clean
#.PHONY: all sha256_nist clean

clean:
	-rm sha3_512_test sha3_384_test sha3_256_test sha3_224_test sha3_512_spirs sha3_384_spirs sha3_256_spirs sha3_224_spirs
