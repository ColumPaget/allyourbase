FLAGS=-g -O2 -O2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -fstack-protector-strong -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -D_FILE_OFFSET_BITS=64 -DUSE_SECCOMP=1 -DHAVE_LIBUSEFUL=1 -DHAVE_LIBUSEFUL=1 -DHAVE_LIBSSL=1 -DHAVE_LIBCRYPTO=1 -DHAVE_LIBZ=1
LIBUSEFUL_BUNDLED=
LIBS=$(LIBUSEFUL_BUNDLED) -lz -lcrypto -lssl -lUseful -lUseful 

all: $(LIBUSEFUL_BUNDLED)
	gcc $(FLAGS) -oayb main.c $(LIBS) 

libUseful-bundled/libUseful.a:
	make -C libUseful-bundled

clean:
	-rm *.o */*.o */*.a */*.so .*.swp *.orig tests/*.* 
	make -C libUseful-bundled clean

check: ayb
	@./check.sh
