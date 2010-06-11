#!/usr/bin/env make

#
# Toolchain bootstrapping build system, ver. N + 1
#
# This exists as an attempt to unify building inside and outside of buildbot.
# Each step of the build system has an alias target that can be called
# externally; all steps _up_to_ the target will be executed.  If no target is
# specified, by default it will build a binary tarball (with matching source
# tarball) for 64bit Windows.
#
# The goal is to be able to replace makebuildroot.sh with all its options
# available.
#

all:: # default target

########################################
# Configurable variables
########################################
TARGET_ARCH ?= x86_64-w64-mingw32
HOST_ARCH ?=
ALL_UPDATE ?= # force update everything
BINUTILS_UPDATE ?= ${ALL_UPDATE} # force update binutils
BINUTILS_CONFIG_EXTRA_ARGS ?=
GCC_CONFIG_EXTRA_ARGS ?= --enable-fully-dynamic-string --disable-multilib
GCC_BRANCH ?= trunk # "tags/gcc_4_4_0_release" or "branches/gcc-4_4-branch"
GCC_REVISION ?= head # revision id "146782" or date "2009-04-25"
GCC_UPDATE ?= ${ALL_UPDATE} # force update gcc
GMP_VERSION ?= 5.0.1 # GMP release version
MPFR_VERSION ?= 3.0.0 # MPFR release version
MPC_VERSION ?= 0.8.2 # MPC release version
MINGW_BRANCH ?= trunk # ... not that we have any!
MINGW_REVISION ?= HEAD
MINGW_UPDATE ?= ${ALL_UPDATE} # force update mingw
MINGW_CONFIG_EXTRA_ARGS ?=
SRC_ARCHIVE ?= mingw-w64-src.tar.bz2
BIN_ARCHIVE ?= mingw-w64-bin_$(shell uname -s).tar.bz2

########################################
# Configure
########################################

#Tools
#  CVS     Compression defaulted to 9
#  SVN     Default options for all svn commands
#  SVN_CO  Override this checkout command to "export" if desired
#  TAR     Prefer gnutar to tar (useful for darwin/bsdtar issues)
#  WGET    Try wget, fall back to curl
######

TAR := $(or $(shell which gnutar 2>/dev/null),$(shell which tar 2>/dev/null),tar)
CVS=cvs -z9
SVN=svn --non-interactive --no-auth-cache
SVN_CO=co
WGET=wget -c -t0 -T60 -O
ifeq (,$(shell which wget 2>/dev/null))
  WGET=curl -o
endif

ifeq (,$(filter-out x86_64-%,${TARGET_ARCH}))
  MINGW_LIBDIR := lib64
else ifeq (,$(filter-out i386-% i486-% i586-% i686-%,${TARGET_ARCH}))
  MIGNW_LIBDIR := lib
else
  $(error Unknown CPU for target arch ${TARGET_ARCH})
endif

# the type of _host_ to run on (n.b. not build)
HOST_TYPE := posix
ifneq (,$(filter MINGW%,$(shell uname -s)))
  HOST_TYPE := windows
endif
ifneq (,${HOST_ARCH})
  HOST_TYPE := posix
  BINUTILS_CONFIG_HOST_ARGS = --host=${HOST_ARCH}
  GCC_CONFIG_HOST_ARGS = --host=${HOST_ARCH}
endif
ifneq (,$(filter %-mingw32,${HOST_ARCH}))
  HOST_TYPE := windows
endif



########################################
# Pull mingw-w64-specific patches
########################################

patch-pull: \
    src/patches/.patches.pull.marker

src/patches/.patches.pull.marker: \
    src/patches/.mkdir.marker
	cd $(dir $@) && \
	$(SVN) $(SVN_CO) \
	       https://mingw-w64.svn.sourceforge.net/svnroot/mingw-w64/experimental/patches/ .
	@touch $@


########################################
# Pull binutils
########################################

binutils-pull: \
    src/binutils/.binutils.pull.marker

src/binutils/.binutils.pull.marker: \
    src/binutils/.mkdir.marker
	### XXX Mook: todo: specify revision
ifeq (,$(strip ${BINUTILS_UPDATE}))
	cd $(dir $@) && \
	$(CVS) -d ":pserver:anoncvs@sourceware.org:/cvs/src" \
	    checkout -d . -N binutils
	@touch $@
else
	cd $(dir $@) && \
	$(CVS) -d ":pserver:anoncvs@sourceware.org:/cvs/src" \
	    update
	@touch $@
.PHONY: src/binutils/.binutils.pull.marker
endif

########################################
# Pull GCC
########################################
## find a gcc revision (r???) and a stamp for the file name ${GCC_REV_STAMP}
ifneq (,$(findstring -,${GCC_REVISION}))
  # GCC_REVISION is a date
  GCC_REV_STAMP := d$(subst -,,${GCC_REVISION})
  GCC_REVISION := $(shell TZ=Z $(SVN) log \
                                       -r "{$(subst -,,${GCC_REVISION})T0000Z}:{$(subst -,,${GCC_REVISION})T0030Z}" \
                                       svn://gcc.gnu.org/svn/gcc/trunk | \
                          grep gccadmin | \
                          grep ${GCC_REVISION} | \
                          cut -d ' ' -f 1)
  GCC_REVISION := $(subst r,,${GCC_REVISION})
else ifeq (_,$(if ${GCC_REVISION},,false)_$(strip \
              $(subst 0,, $(subst 1,, $(subst 2,, \
              $(subst 3,, $(subst 4,, $(subst 5,, \
              $(subst 6,, $(subst 7,, $(subst 8,, \
              $(subst 9,, $(subst 0,, ${GCC_REVISION} )))))))))))))
  # GCC_REVISION is a number
  GCC_REVISION := ${GCC_REVISION}
  GCC_REV_STAMP := r${GCC_REVISION}
else
  # GCC_REVISION is some opaque string (e.g. "head")
  GCC_REV_STAMP := r${GCC_REVISION}
endif # GCC revision

gcc-pull: \
    src/gcc/gcc/.gcc.pull.marker

src/gcc/gcc/.gcc.pull.marker: \
    src/gcc/gcc/.mkdir.marker
	cd $(dir $@) && \
	$(SVN) $(SVN_CO) --revision ${GCC_REVISION} \
	       svn://gcc.gnu.org/svn/gcc/$(strip ${GCC_BRANCH})/ .
	@touch $@

  ifneq (,$(strip ${GCC_UPDATE}))
.PHONY: src/gcc/gcc/.gcc.pull.marker
  endif

########################################
# Download gmp
########################################

gmp-download: \
    src/gmp.tar.bz2

src/gmp.tar.bz2: \
    src/.mkdir.marker
	$(WGET) $@ ftp://ftp.gnu.org/gnu/gmp/gmp-$(strip ${GMP_VERSION}).tar.bz2

########################################
# Extract gmp
########################################

gmp-extract: \
    src/gmp/.gmp.extract.marker

src/gmp/.gmp.extract.marker: \
    src/gmp.tar.bz2 \
    src/gmp/src/.mkdir.marker
	$(TAR) -C $(dir $@)/src --strip-components=1 -xjvf $<
	@touch $@

########################################
# Execute autoconf for gmp
########################################

gmp-autoconf: \
    src/gmp/src/configure

src/gmp/src/configure: \
    src/gmp/.gmp.extract.marker
	cd $(dir $@) && \
	autoconf

########################################
# Download mpfr
########################################

mpfr-download: \
    src/mpfr.tar.bz2

src/mpfr.tar.bz2: \
    src/.mkdir.marker
	$(WGET) $@ http://www.mpfr.org/mpfr-$(strip ${MPFR_VERSION})/mpfr-$(strip ${MPFR_VERSION}).tar.bz2

########################################
# Extract mpfr
########################################

mpfr-extract: \
    src/mpfr/.mpfr.extract.marker

src/mpfr/.mpfr.extract.marker: \
    src/mpfr.tar.bz2 \
    src/mpfr/src/.mkdir.marker
	$(TAR) -C $(dir $@)/src --strip-components=1 -xjvf $<
	@touch $@

########################################
# Download mpc
########################################

mpc-download: \
    src/mpc.tar.gz

src/mpc.tar.gz: \
    src/.mkdir.marker
	$(WGET) $@ http://www.multiprecision.org/mpc/download/mpc-$(strip ${MPC_VERSION}).tar.gz

########################################
# Extract mpc
########################################

mpc-extract: \
    src/mpc/.mpc.extract.marker

src/mpc/.mpc.extract.marker: \
    src/mpc.tar.gz \
    src/mpc/src/.mkdir.marker
	$(TAR) -C $(dir $@)/src --strip-components=1 -xzvf $<
	@touch $@

########################################
# Pull mingw
########################################

mingw-pull: \
    src/mingw/.mingw.pull.marker

src/mingw/.mingw.pull.marker: \
    src/mingw/.mkdir.marker
	$(SVN) $(SVN_CO) --revision ${MINGW_REVISION} \
	    https://mingw-w64.svn.sourceforge.net/svnroot/mingw-w64/$(strip ${MINGW_BRANCH})/ \
	    $(dir $@)
	@touch $@

  ifneq (,$(strip ${MINGW_UPDATE}))
.PHONY: src/mingw/.mingw.pull.marker
  endif

## XXX Mook: stamp the source with the revision used

########################################
# Create source tarball
########################################

src-archive:  ${SRC_ARCHIVE}

ifeq (,$(wildcard ${SRC_ARCHIVE}))
${SRC_ARCHIVE}: \
    src/binutils/.binutils.pull.marker \
    src/gcc/gcc/.gcc.pull.marker \
    src/gmp/src/configure \
    src/mpfr/.mpfr.extract.marker \
    src/mpc/.mpc.extract.marker \
    src/mingw/.mingw.pull.marker
endif

${SRC_ARCHIVE}:
	$(TAR) vcjf $@ --owner 0 --group 0 --exclude=.svn \
	    --exclude=.*.marker --exclude=CVS --exclude=gmp.tar.bz2 \
	    --exclude=mpfr.tar.bz2 --exclude=mpc.tar.gz \-C src .

################################################################################
# Build
################################################################################

BUILD_DIR := build

########################################
# Extract source tarball
########################################
src-extract:: \
    build/.extract.marker

build/.extract.marker: \
    ${SRC_ARCHIVE}
	-mkdir -p $(dir $@)
	$(TAR) -C $(dir $@) -xvjpf $<
	@touch $@

${BUILD_DIR}/root/.root.init.marker: \
    ${BUILD_DIR}/root/${TARGET_ARCH}/.mkdir.marker \
    build/.extract.marker
ifneq (,$(filter MINGW%,$(shell uname -s)))
	test -e ${BUILD_DIR}/root/mingw  || \
	  junction ${BUILD_DIR}/root/mingw "${BUILD_DIR}/root/${TARGET_ARCH}"
	test -e ${BUILD_DIR}/root/mingw
else
	test -h ${BUILD_DIR}/root/mingw  || \
	  ln -s "${TARGET_ARCH}" ${BUILD_DIR}/root/mingw
	test -h ${BUILD_DIR}/root/mingw
endif
	@touch $@

########################################
# Configure mingw-w64 headers
########################################
headers-configure: \
    ${BUILD_DIR}/mingw-headers/obj/.config.marker

${BUILD_DIR}/mingw-headers/obj/.config.marker: \
    ${BUILD_DIR}/root/.root.init.marker \
    ${BUILD_DIR}/mingw-headers/obj/.mkdir.marker
	cd $(dir $@) && \
	  ${CURDIR}/${BUILD_DIR}/mingw/mingw-w64-headers/configure \
	  --prefix=${CURDIR}/${BUILD_DIR}/root \
	  --with-sysroot=${CURDIR}/${BUILD_DIR}/root \
	  --host=${TARGET_ARCH}
	@touch $@

########################################
# Install mingw-w64 headers
########################################
headers-install: \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker

${BUILD_DIR}/mingw-headers/obj/.install.marker: \
    ${BUILD_DIR}/mingw-headers/obj/.config.marker
	make -C $(dir $@) install
	@touch $@

########################################
# Configure binutils
########################################
binutils-configure: \
    ${BUILD_DIR}/binutils/obj/.config.marker

${BUILD_DIR}/binutils/obj/.config.marker: \
    ${BUILD_DIR}/binutils/obj/.mkdir.marker \
    ${BUILD_DIR}/root/.root.init.marker
	cd $(dir $@) && \
	../../../build/binutils/src/configure \
	    --target=${TARGET_ARCH} \
	    ${BINUTILS_CONFIG_HOST_ARGS} \
	    --prefix=${CURDIR}/${BUILD_DIR}/root \
	    --with-sysroot=${CURDIR}/${BUILD_DIR}/root \
	    ${BINUTILS_CONFIG_EXTRA_ARGS}
	@touch $@

########################################
# Compile binutils
########################################
binutils-compile: \
    ${BUILD_DIR}/binutils/obj/.compile.marker

${BUILD_DIR}/binutils/obj/.compile.marker: \
    ${BUILD_DIR}/binutils/obj/.config.marker
	make -C $(dir $@)
	@touch $@

########################################
# Install binutils
########################################
binutils-install: \
    ${BUILD_DIR}/binutils/obj/.install.marker

${BUILD_DIR}/binutils/obj/.install.marker: \
    ${BUILD_DIR}/binutils/obj/.compile.marker
	make -C $(dir $@) install
	@touch $@

########################################
# GCC cross compiling support - winsup
########################################
gcc-winsup: \
    build/gcc/gcc/.winsup.marker

build/gcc/gcc/.winsup.marker: \
    ${BUILD_DIR}/.extract.marker \
    ${BUILD_DIR}/root/.root.init.marker
ifneq (,$(filter MINGW%,$(shell uname -s)))
	test -e build/gcc/gcc/winsup  || \
	  junction build/gcc/gcc/winsup "${BUILD_DIR}/root"
	test -e build/gcc/gcc/winsup
else
	test -h build/gcc/gcc/winsup  || \
	  ln -s "../../../${BUILD_DIR}/root" build/gcc/gcc/winsup
	test -h build/gcc/gcc/winsup
endif
	@touch $@

########################################
# Configure GMP
########################################
gmp-configure: \
    ${BUILD_DIR}/gmp/obj/.config.marker

${BUILD_DIR}/gmp/obj/.config.marker: \
    ${BUILD_DIR}/gmp/obj/.mkdir.marker \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker \
    ${BUILD_DIR}/root/.root.init.marker
	cd $(dir $@) && \
	../../../build/gmp/src/configure \
	    $(or ${GCC_CONFIG_HOST_ARGS},--host=none-none-none) \
	    --disable-shared \
	    --prefix=${CURDIR}/${BUILD_DIR}/gmp/install
	@touch $@

########################################
# Compile GMP
########################################
gmp-compile: \
    ${BUILD_DIR}/gmp/obj/.compile.marker

${BUILD_DIR}/gmp/obj/.compile.marker: \
    ${BUILD_DIR}/gmp/obj/.config.marker
	$(MAKE) -C $(dir $@)
	@touch $@

########################################
# Install GMP
########################################
gmp-install: \
    ${BUILD_DIR}/gmp/install/.install.marker

${BUILD_DIR}/gmp/install/.install.marker: \
    ${BUILD_DIR}/gmp/obj/.compile.marker \
    ${BUILD_DIR}/gmp/install/.mkdir.marker
	$(MAKE) -C ${BUILD_DIR}/gmp/obj install
	@touch $@

########################################
# Configure MPFR
########################################
mpfr-configure: \
    ${BUILD_DIR}/mpfr/obj/.config.marker

${BUILD_DIR}/mpfr/obj/.config.marker: \
    ${BUILD_DIR}/mpfr/obj/.mkdir.marker \
    ${BUILD_DIR}/root/.root.init.marker \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker \
    ${BUILD_DIR}/gmp/install/.install.marker
	cd $(dir $@) && \
	../../../build/mpfr/src/configure \
	    ${GCC_CONFIG_HOST_ARGS} \
	    --disable-shared \
	    --prefix=${CURDIR}/${BUILD_DIR}/mpfr/install \
            --with-gmp=${CURDIR}/${BUILD_DIR}/gmp/install
	@touch $@

########################################
# Compile MPFR
########################################
mpfr-compile: \
    ${BUILD_DIR}/mpfr/obj/.compile.marker

${BUILD_DIR}/mpfr/obj/.compile.marker: \
    ${BUILD_DIR}/mpfr/obj/.config.marker
	$(MAKE) -C $(dir $@)
	@touch $@

########################################
# Install MPFR
########################################
mpfr-install: \
    ${BUILD_DIR}/mpfr/install/.install.marker

${BUILD_DIR}/mpfr/install/.install.marker: \
    ${BUILD_DIR}/mpfr/obj/.compile.marker \
    ${BUILD_DIR}/mpfr/install/.mkdir.marker
	$(MAKE) -C ${BUILD_DIR}/mpfr/obj install
	@touch $@

########################################
# Configure MPC
########################################
mpc-configure: \
    ${BUILD_DIR}/mpc/obj/.config.marker

${BUILD_DIR}/mpc/obj/.config.marker: \
    ${BUILD_DIR}/mpc/obj/.mkdir.marker \
    ${BUILD_DIR}/root/.root.init.marker \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker \
    ${BUILD_DIR}/gmp/install/.install.marker \
    ${BUILD_DIR}/mpfr/install/.install.marker
	cd $(dir $@) && \
	../../../build/mpc/src/configure \
	    ${GCC_CONFIG_HOST_ARGS} \
            --enable-static --disable-shared \
	    --prefix=${CURDIR}/${BUILD_DIR}/mpc/install \
            --with-gmp=${CURDIR}/${BUILD_DIR}/gmp/install \
            --with-mpfr=${CURDIR}/${BUILD_DIR}/mpfr/install
	@touch $@

########################################
# Compile MPC
########################################
mpc-compile: \
    ${BUILD_DIR}/mpc/obj/.compile.marker

${BUILD_DIR}/mpc/obj/.compile.marker: \
    ${BUILD_DIR}/mpc/obj/.config.marker
	$(MAKE) -C $(dir $@)
	@touch $@

########################################
# Install MPC
########################################
mpc-install: \
    ${BUILD_DIR}/mpc/install/.install.marker

${BUILD_DIR}/mpc/install/.install.marker: \
    ${BUILD_DIR}/mpc/obj/.compile.marker \
    ${BUILD_DIR}/mpc/install/.mkdir.marker
	$(MAKE) -C ${BUILD_DIR}/mpc/obj install
	@touch $@

########################################
# Configure GCC
########################################
gcc-configure: \
    ${BUILD_DIR}/gcc/obj/.config.marker

ifneq (,$(filter %-mingw32,${HOST_ARCH}))
${BUILD_DIR}/gcc/obj/.config.marker: \
    ${BUILD_DIR}/gcc/gcc/.winsup.marker
endif

${BUILD_DIR}/gcc/obj/.config.marker: \
    ${BUILD_DIR}/gcc/obj/.mkdir.marker \
    ${BUILD_DIR}/binutils/obj/.install.marker \
    ${BUILD_DIR}/gmp/install/.install.marker \
    ${BUILD_DIR}/mpfr/install/.install.marker \
    ${BUILD_DIR}/mpc/install/.install.marker \
    ${BUILD_DIR}/root/.root.init.marker
	cd $(dir $@) && \
	../../../build/gcc/gcc/configure \
	    --target=${TARGET_ARCH} \
	    ${GCC_CONFIG_HOST_ARGS} \
	    --prefix=${CURDIR}/${BUILD_DIR}/root \
	    --with-sysroot=${CURDIR}/${BUILD_DIR}/root \
            --with-gmp=${CURDIR}/${BUILD_DIR}/gmp/install \
            --with-mpfr=${CURDIR}/${BUILD_DIR}/mpfr/install \
            --with-mpc=${CURDIR}/${BUILD_DIR}/mpc/install \
	    --enable-languages=all,obj-c++ \
	    ${GCC_CONFIG_EXTRA_ARGS}
	@touch $@

########################################
# Compile GCC stage 1
########################################
gcc-bootstrap-compile: \
    build/gcc/obj/.bootstrap.compile.marker

build/gcc/obj/.bootstrap.compile.marker: \
    build/gcc/obj/.config.marker \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker
	found_asm=yes make -C $(dir $@) all-gcc
	@touch $@

########################################
# Install GCC stage 1
########################################
gcc-bootstrap-install: \
    build/gcc/obj/.bootstrap.install.marker

build/gcc/obj/.bootstrap.install.marker: \
    build/gcc/obj/.bootstrap.compile.marker
	make -C $(dir $@) install-gcc
	@touch $@

########################################
# Configure mingw-w64 CRT
########################################
crt-configure: \
    ${BUILD_DIR}/mingw/obj/.config.marker

${BUILD_DIR}/mingw/obj/.config.marker: \
    build/gcc/obj/.bootstrap.install.marker \
    ${BUILD_DIR}/mingw/obj/.mkdir.marker
	cd $(dir $@) && \
	PATH=$(realpath build/root/bin):$$PATH \
	../../../build/mingw/mingw-w64-crt/configure \
	    --host=${TARGET_ARCH} \
	    --prefix=${CURDIR}/${BUILD_DIR}/root \
	    --with-sysroot=${CURDIR}/${BUILD_DIR}/root \
	    ${MINGW_CONFIG_EXTRA_ARGS}
	@touch $@

########################################
# Compile mingw-w64 CRT
########################################
crt-compile: \
    ${BUILD_DIR}/mingw/obj/.compile.marker

${BUILD_DIR}/mingw/obj/.compile.marker: \
    ${BUILD_DIR}/mingw/obj/.config.marker
	PATH=$(realpath build/root/bin):$$PATH \
	make -C $(dir $@)
	@touch $@

########################################
# Install mingw-w64 CRT
########################################
crt-install: \
    ${BUILD_DIR}/mingw/obj/.install.marker

${BUILD_DIR}/mingw/obj/.install.marker: \
    ${BUILD_DIR}/mingw/obj/.compile.marker
	PATH=$(realpath build/root/bin):$$PATH \
	make -C $(dir $@) install
	@touch $@

########################################
# Compile full GCC
########################################
gcc-compile: \
    ${BUILD_DIR}/gcc/obj/.compile.marker \
    ${BUILD_DIR}/mingw/obj/.install.marker

${BUILD_DIR}/gcc/obj/.compile.marker: \
    ${BUILD_DIR}/gcc/obj/.config.marker \
    ${BUILD_DIR}/mingw/obj/.install.marker
	PATH=$(realpath build/root/bin):$$PATH \
	make -C $(dir $@)
	@touch $@

########################################
# Install full GCC
########################################
gcc-install: \
    ${BUILD_DIR}/gcc/obj/.install.marker

${BUILD_DIR}/gcc/obj/.install.marker: \
    ${BUILD_DIR}/gcc/obj/.compile.marker
	PATH=$(realpath build/root/bin):$$PATH \
	make -C $(dir $@) install
	@touch $@

########################################
# Create release tarball
########################################
release-archive: \
    ${BIN_ARCHIVE}

${BIN_ARCHIVE}: \
    ${BUILD_DIR}/gcc/obj/.install.marker
ifeq (windows,${HOST_TYPE})
	cd ${BUILD_DIR}/root && \
	zip -r -9 ../../$(patsubst %.tar.bz2,%.zip,$@) \
	     . -x .*.marker *.*.marker
else
	$(TAR) vcjf $@ -C ${BUILD_DIR}/root --owner 0 --group 0 \
	    --exclude=CVS --exclude=.svn --exclude=.*.marker \
            .
endif

################################################################################
# Native (only active when native_dir != build_dir)
################################################################################

NATIVE_DIR := native

ifneq (${NATIVE_DIR},${BUILD_DIR})

########################################
# Initialize build root
########################################

${NATIVE_DIR}/root/.root.init.marker: \
    ${NATIVE_DIR}/root/${TARGET_ARCH}/.mkdir.marker \
    ${BUILD_DIR}/.extract.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Configure mingw-w64 headers
########################################
native-headers-configure: \
    ${NATIVE_DIR}/mingw-headers/obj/.config.marker

${NATIVE_DIR}/mingw-headers/obj/.config.marker: \
    ${NATIVE_DIR}/root/.root.init.marker \
    ${NATIVE_DIR}/mingw-headers/obj/.mkdir.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Install mingw-w64 headers
########################################
native-headers-install: \
    ${NATIVE_DIR}/mingw-headers/obj/.install.marker

${NATIVE_DIR}/mingw-headers/obj/.install.marker: \
    ${NATIVE_DIR}/mingw-headers/obj/.config.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Configure binutils
########################################
native-binutils-configure: \
    ${NATIVE_DIR}/binutils/obj/.config.marker

${NATIVE_DIR}/binutils/obj/.config.marker: \
    ${BUILD_DIR}/gcc/obj/.install.marker \
    ${NATIVE_DIR}/binutils/obj/.mkdir.marker \
    ${NATIVE_DIR}/root/.root.init.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Compile binutils
########################################
native-binutils-compile: \
    ${NATIVE_DIR}/binutils/obj/.compile.marker

${NATIVE_DIR}/binutils/obj/.compile.marker: \
    ${NATIVE_DIR}/binutils/obj/.config.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Install binutils
########################################
native-binutils-install: \
    ${NATIVE_DIR}/binutils/obj/.install.marker

${NATIVE_DIR}/binutils/obj/.install.marker: \
    ${NATIVE_DIR}/binutils/obj/.compile.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

#########################################
# Configure GMP
########################################
native-gmp-configure: \
    ${NATIVE_DIR}/gmp/obj/.config.marker

${NATIVE_DIR}/gmp/obj/.config.marker: \
    ${BUILD_DIR}/gcc/obj/.install.marker \
    ${NATIVE_DIR}/mingw-headers/obj/.install.marker \
    ${NATIVE_DIR}/gmp/obj/.mkdir.marker \
    ${NATIVE_DIR}/root/.root.init.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Compile GMP
########################################
native-gmp-compile: \
    ${NATIVE_DIR}/gmp/obj/.compile.marker

${NATIVE_DIR}/gmp/obj/.compile.marker: \
    ${NATIVE_DIR}/gmp/obj/.config.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Install GMP
########################################
native-gmp-install: \
    ${NATIVE_DIR}/gmp/install/.install.marker

${NATIVE_DIR}/gmp/install/.install.marker: \
    ${NATIVE_DIR}/gmp/obj/.compile.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Configure MPFR
########################################
native-mpfr-configure: \
    ${NATIVE_DIR}/mpfr/obj/.config.marker

${NATIVE_DIR}/mpfr/obj/.config.marker: \
    ${BUILD_DIR}/gcc/obj/.install.marker \
    ${NATIVE_DIR}/mingw-headers/obj/.install.marker\
    ${NATIVE_DIR}/mpfr/obj/.mkdir.marker \
    ${NATIVE_DIR}/gmp/install/.install.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Compile MPFR
########################################
native-mpfr-compile: \
    ${NATIVE_DIR}/mpfr/obj/.compile.marker

${NATIVE_DIR}/mpfr/obj/.compile.marker: \
    ${NATIVE_DIR}/mpfr/obj/.config.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Install MPFR
########################################
native-mpfr-install: \
    ${NATIVE_DIR}/mpfr/install/.install.marker

${NATIVE_DIR}/mpfr/install/.install.marker: \
    ${NATIVE_DIR}/mpfr/obj/.compile.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Configure MPC
########################################
native-mpc-configure: \
    ${NATIVE_DIR}/mpc/obj/.config.marker

${NATIVE_DIR}/mpc/obj/.config.marker: \
    ${BUILD_DIR}/gcc/obj/.install.marker \
    ${NATIVE_DIR}/mingw-headers/obj/.install.marker \
    ${NATIVE_DIR}/mpc/obj/.mkdir.marker \
    ${NATIVE_DIR}/mpfr/install/.install.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Compile MPC
########################################
native-mpc-compile: \
    ${NATIVE_DIR}/mpc/obj/.compile.marker

${NATIVE_DIR}/mpc/obj/.compile.marker: \
    ${NATIVE_DIR}/mpc/obj/.config.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Install MPC
########################################
native-mpc-install: \
    ${NATIVE_DIR}/mpc/install/.install.marker

${NATIVE_DIR}/mpc/install/.install.marker: \
    ${NATIVE_DIR}/mpc/obj/.compile.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# GCC cross compiling support - winsup
########################################
native-gcc-winsup: \
    ${NATIVE_DIR}/gcc/gcc/.winsup.marker

${NATIVE_DIR}/gcc/gcc/.winsup.marker: \
    build/.extract.marker \
    ${NATIVE_DIR}/gcc/gcc/.mkdir.marker \
    ${NATIVE_DIR}/root/.root.init.marker
ifneq (,$(filter MINGW%,$(shell uname -s)))
	-test -e build/gcc/gcc/winsup  && \
	  junction -d build/gcc/gcc/winsup
	junction build/gcc/gcc/winsup "${NATIVE_DIR}/root"
	test -e build/gcc/gcc/winsup
else
	-test -h build/gcc/gcc/winsup && \
	  rm build/gcc/gcc/winsup
	ln -s "../../../${NATIVE_DIR}/root" build/gcc/gcc/winsup
	test -h build/gcc/gcc/winsup
endif
	@touch $@

########################################
# Configure GCC
########################################
native-gcc-configure: \
    ${NATIVE_DIR}/gcc/obj/.config.marker

${NATIVE_DIR}/gcc/obj/.config.marker: \
    ${NATIVE_DIR}/gcc/gcc/.winsup.marker \
    ${NATIVE_DIR}/gcc/obj/.mkdir.marker \
    ${NATIVE_DIR}/binutils/obj/.install.marker \
    ${NATIVE_DIR}/gmp/install/.install.marker \
    ${NATIVE_DIR}/mpfr/install/.install.marker \
    ${NATIVE_DIR}/mpc/install/.install.marker \
    ${NATIVE_DIR}/root/.root.init.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Configure mingw-w64 CRT
########################################
native-crt-configure: \
    ${NATIVE_DIR}/mingw/obj/.config.marker

${NATIVE_DIR}/mingw/obj/.config.marker: \
    build/gcc/obj/.bootstrap.install.marker \
    ${NATIVE_DIR}/mingw/obj/.mkdir.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Compile mingw-w64 CRT
########################################
native-crt-compile: \
    ${NATIVE_DIR}/mingw/obj/.compile.marker

${NATIVE_DIR}/mingw/obj/.compile.marker: \
    ${NATIVE_DIR}/mingw/obj/.config.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Install mingw-w64 CRT
########################################
native-crt-install: \
    ${NATIVE_DIR}/mingw/obj/.install.marker

${NATIVE_DIR}/mingw/obj/.install.marker: \
    ${NATIVE_DIR}/mingw/obj/.compile.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Compile full GCC
########################################
native-gcc-compile: \
    ${NATIVE_DIR}/gcc/obj/.compile.marker \
    ${NATIVE_DIR}/mingw/obj/.install.marker

${NATIVE_DIR}/gcc/obj/.compile.marker: \
    ${NATIVE_DIR}/gcc/obj/.config.marker \
    ${NATIVE_DIR}/mingw/obj/.install.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Install full GCC
########################################
native-gcc-install: \
    ${NATIVE_DIR}/gcc/obj/.install.marker

${NATIVE_DIR}/gcc/obj/.install.marker: \
    ${NATIVE_DIR}/gcc/obj/.compile.marker
	PATH=$(realpath build/root/bin):$$PATH \
	${MAKE} -f $(lastword ${MAKEFILE_LIST}) \
	     HOST_ARCH=${TARGET_ARCH} \
	     TARGET_ARCH=${TARGET_ARCH} \
	     BUILD_DIR=${NATIVE_DIR} $@

########################################
# Create release tarball
########################################
native-release-archive: \
    native-${BIN_ARCHIVE}

native-${BIN_ARCHIVE}: \
    ${NATIVE_DIR}/gcc/obj/.install.marker
	cd ${NATIVE_DIR}/root && \
	zip -r -9 ${CURDIR}/$(patsubst %.tar.bz2,%.zip,$@) \
	     . -x .*.marker *.*.marker


endif # native_dir != build_dir

################################################################################
# Helper targets
################################################################################

%/.mkdir.marker:
	-mkdir -p $(dir $@)
	@touch -t 197001020101 $@

help::
	@echo Available targets:
	@echo -e $(foreach t,all ${TARGETS} $@,\\t${t}\\n)

# build only the cross-compiler by default
all:: \
  ${BIN_ARCHIVE}

TARGETS := \
  patch-pull \
  binutils-pull \
  gcc-pull \
  gmp-download \
  gmp-extract \
  gmp-autoconf \
  mpfr-download \
  mpfr-extract \
  mpc-download \
  mpc-extract \
  mingw-pull \
  src-archive \
  src-extract \
  binutils-configure \
  binutils-compile \
  binutils-install \
  gmp-configure \
  gmp-compile \
  gmp-install \
  mpfr-configure \
  mpfr-compile \
  mpfr-install \
  mpc-configure \
  mpc-compile \
  mpc-install \
  gcc-configure \
  gcc-bootstrap-compile \
  gcc-bootstrap-install \
  headers-configure \
  headers-install \
  crt-configure \
  crt-compile \
  crt-install \
  gcc-compile \
  gcc-install \
  release-archive \
  native-binutils-configure \
  native-binutils-compile \
  native-binutils-install \
  native-gmp-configure \
  native-gmp-compile \
  native-gmp-install \
  native-mpfr-configure \
  native-mpfr-compile \
  native-mpfr-install \
  native-mpc-configure \
  native-mpc-compile \
  native-mpc-install \
  native-gcc-configure \
  native-headers-configure \
  native-headers-install \
  native-crt-configure \
  native-crt-compile \
  native-crt-install \
  native-gcc-compile \
  native-gcc-install \
  native-release-archive \
  ${NULL}


.PHONY: \
  all \
  ${TARGETS} \
  help \
  ${NULL}
