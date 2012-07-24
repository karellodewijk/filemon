prefix=${EXEC_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: filemon
Description: A file monitoring library
Version: ${filemon_VERSION}
Libs: -lfilemon
Cflags: -I${INCLUDE_INSTALL_DIR}
