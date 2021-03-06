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
### TODO:
### Add clean-foo targets.
### Add GDB buildstep.
### Add XZ Compressed tarballs.
### Add Some checks for required installed tools before starting.

all:: # default target

########################################
# Build configurable variables
########################################
# TARGET_ARCH - Toolchain default target arch
TARGET_ARCH ?= x86_64-w64-mingw32
BUILD_ARCH ?= x86_64-w64-mingw32
HOST_ARCH ?= ${BUILD_ARCH}
MAKE_OPTS ?=
BUILD_DIR ?= build

########################################
# Makefile output options
########################################
SRC_ARCHIVE ?= mingw-w64-src.tar.xz
BIN_ARCHIVE ?= mingw-w64-bin_$(shell uname -s).tar.xz

########################################
# Y/N options
########################################
# ENABLE_MULTILIB - Set to Y to use Multi-lib
# GCC_ADA - Set to Y to build ADA
# GCC_CPP - C++ support
# GCC_FORTRAN - Fortran support
# GCC_OBJC - Objective C support
# GCC_JAVA - Java support
ENABLE_MULTILIB ?= N
GCC_ADA ?= N
GCC_CPP ?= Y
GCC_FORTRAN ?= Y
GCC_OBJC ?= Y
GCC_JAVA ?= N

########################################
# Version number variables
########################################
GMP_VERSION ?= 5.0.1
MPFR_VERSION ?= 3.0.0
MPC_VERSION ?= 0.8.2
PPL_VERSION ?= 0.10.2
CLOOG_VERSION ?= 0.15.9
MINGW_BRANCH ?= trunk
MINGW_REVISION ?= HEAD
GCC_BRANCH ?= "tags/gcc_4_5_2_release"#trunk # "tags/gcc_4_4_0_release" or "branches/gcc-4_4-branch"
GCC_REVISION ?= "167946" #head # revision id "146782" or date "2009-04-25"

########################################
# Download URLs
########################################
PTHREADS_CVS_PULL ?= :pserver:anoncvs@sourceware.org:/cvs/pthreads-win32
PTHREADS_CVS_DATE ?= 'November 16, 2010'

########################################
# Configure options
########################################
BINUTILS_CONFIG_EXTRA_ARGS ?=
GCC_CONFIG_EXTRA_ARGS ?= --enable-fully-dynamic-string --enable-libgomp --with-dwarf2 --enable-sjlj-exceptions --enable-version-specific-runtime-libs
PTHREADS_MAKE_ARGS ?= clean GC
MINGW_CONFIG_EXTRA_ARGS ?=

########################################
# Update sources?
########################################
ALL_UPDATE ?= # force update everything
BINUTILS_UPDATE ?= ${ALL_UPDATE} # force update binutils
GCC_UPDATE ?= ${ALL_UPDATE} # force update gcc
MINGW_UPDATE ?= ${ALL_UPDATE} # force update mingw
PTHREADS_UPDATE ?= ${ALL_UPDATE}

#########################################
# DO NOT EDIT PASS THIS LINE
#########################################

BUILDDIRW=${shell cd ${BUILD_DIR} ; pwd -W}
BUILDDIR=${shell cd ${BUILD_DIR} ; pwd}

########################################
# Multilib helper
########################################
BINUTILS_CONFIG_EXTRA_ARGS_MULTI_Y := --enable-targets=x86_64-w64-mingw32,i686-w64-mingw32
BINUTILS_CONFIG_EXTRA_ARGS_MULTI_N :=
GCC_CONFIG_EXTRA_ARGS_MULTI_Y := --enable-multilib
GCC_CONFIG_EXTRA_ARGS_MULTI_N := --disable-multilib
MINGW_CONFIG_EXTRA_ARGS_MULTI_Y := --enable-lib32 --enable-lib64
MINGW_CONFIG_EXTRA_ARGS_MULTI_N :=

########################################
# MISC helper
########################################

GCC_ADA_Y := ",ada"
GCC_ADA_N :=
GCC_CPP_Y := ",c++"
GCC_CPP_N :=
GCC_FORTRAN_Y := ",fortran"
GCC_FORTRAN_N :=
GCC_OBJC_Y := ",objc,obj-c++"
GCC_OBJC_N :=
GCC_JAVA_Y := ",java"
GCC_JAVA_N :=

########################################
# Configure
########################################
#Prefer gnutar to tar
TAR := $(or $(shell which gnutar 2>/dev/null),$(shell which tar 2>/dev/null),tar)

ifeq (,$(filter-out x86_64-%,${TARGET_ARCH}))
  MINGW_LIBDIR := lib64
else ifeq (,$(filter-out i386-% i486-% i586-% i686-%,${TARGET_ARCH}))
  MINGW_LIBDIR := lib32
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

ifneq (,${BUILD_ARCH})
  CONFIG_BUILD_ARGS = --build=${BUILD_ARCH}
endif

ifneq (,$(filter %-mingw32,${HOST_ARCH}))
  HOST_TYPE := windows
endif

WGET=wget -c -t0 -T60 -O
ifeq (,$(shell which wget 2>/dev/null))
  WGET=curl -o
endif

ifneq ($(strip $(HOST_ARCH)),$(strip $(BUILD_ARCH)))
  CANADIAN_CROSS_BUILD := Y
else
  ADD_BIN_PATH ?= PATH=$(realpath ${BUILD_DIR}/root/bin):$$PATH
endif

########################################
# Init buildroot
########################################

#${BUILD_DIR}/%/${TARGET_ARCH}/.mkdir.marker:
#ifneq (,$(filter MINGW%,$(shell uname -s)))
#	test -e $(@D)  || \
#	  cmd //C mklink //D ${subst /,\\,${shell cd $(dir $(@D)) ; pwd -W}/${TARGET_ARCH}} "${subst /,\\,${shell cd $(dir $(@D)) ; pwd -W}}"
#	test -e $(@D)
#else
#	test -h $(@D)  || \
#	  ln -s "$(@D)/.." $(@D)/../${TARGET_ARCH}
#	test -h $(@D)
#endif
#	@touch -t 197001020101 $@

.SECONDEXPANSION:
${BUILD_DIR}/%/.root.init.marker: \
    $$(dir $$(@D))/.mkdir.marker \
    $$(@D)/.mkdir.marker \
    $$(@D)/bin/.mkdir.marker \
    $$(@D)/include/.mkdir.marker \
    $$(@D)/lib32/.mkdir.marker \
    $$(@D)/lib64/.mkdir.marker \
    $$(@D)/share/.mkdir.marker \
    $$(@D)/${TARGET_ARCH}/.mkdir.marker \
    ${BUILD_DIR}/.extract.marker
	test -e $(@D)/lib  || \
	  cmd //C mklink //D ${subst /,\\,${shell cd $(@D) ; pwd -W}/lib} "${subst /,\\,${shell cd $(@D) ; pwd -W}/${MINGW_LIBDIR}}"
	test -e $(@D)/lib
	test -e $(@D)/${TARGET_ARCH}/lib32  || \
	  cmd //C mklink //D ${subst /,\\,${shell cd $(@D) ; pwd -W}/${TARGET_ARCH}/lib32} "${subst /,\\,${shell cd $(@D) ; pwd -W}/lib32}"
	test -e $(@D)/${TARGET_ARCH}/lib32
	test -e $(@D)/${TARGET_ARCH}/lib64  || \
	  cmd //C mklink //D ${subst /,\\,${shell cd $(@D) ; pwd -W}/${TARGET_ARCH}/lib64} "${subst /,\\,${shell cd $(@D) ; pwd -W}/lib64}"
	test -e $(@D)/${TARGET_ARCH}/lib64
	test -e $(@D)/${TARGET_ARCH}/lib  || \
	  cmd //C mklink //D ${subst /,\\,${shell cd $(@D) ; pwd -W}/${TARGET_ARCH}/lib} "${subst /,\\,${shell cd $(@D) ; pwd -W}/lib}"
	test -e $(@D)/${TARGET_ARCH}/lib
	test -e $(@D)/${TARGET_ARCH}/include  || \
	  cmd //C mklink //D ${subst /,\\,${shell cd $(@D) ; pwd -W}/${TARGET_ARCH}/include} "${subst /,\\,${shell cd $(@D) ; pwd -W}/include}"
	test -e $(@D)/${TARGET_ARCH}/include
	test -e $(@D)/${TARGET_ARCH}/share  || \
	  cmd //C mklink //D ${subst /,\\,${shell cd $(@D) ; pwd -W}/${TARGET_ARCH}/share} "${subst /,\\,${shell cd $(@D) ; pwd -W}/share}"
	test -e $(@D)/${TARGET_ARCH}/share
	test -e $(@D)/${TARGET_ARCH}/bin  || \
	  cmd //C mklink //D ${subst /,\\,${shell cd $(@D) ; pwd -W}/${TARGET_ARCH}/bin} "${subst /,\\,${shell cd $(@D) ; pwd -W}/bin}"
	test -e $(@D)/${TARGET_ARCH}/bin
	test -e $(@D)/mingw  || \
	  cmd //C mklink //D ${subst /,\\,${shell cd $(@D) ; pwd -W}/mingw} "${subst /,\\,${shell cd $(@D) ; pwd -W}/${TARGET_ARCH}}"
	test -e $(@D)/mingw
	@touch $@

########################################
# Init installroot
########################################
#  ${BUILD_DIR}/inst/binutils/.root.init.marker \
#  ${BUILD_DIR}/inst/gmp/.root.init.marker \
#  ${BUILD_DIR}/inst/mpfr/.root.init.marker \
#  ${BUILD_DIR}/inst/mpc/.root.init.marker \
#  ${BUILD_DIR}/inst/ppl/.root.init.marker \
#  ${BUILD_DIR}/inst/cloog/.root.init.marker \
#  ${BUILD_DIR}/inst/gcc-stage-1/.root.init.marker \
#  ${BUILD_DIR}/inst/mingw-w64-crt/.root.init.marker \
#  ${BUILD_DIR}/inst/libgcc/.root.init.marker \
#  ${BUILD_DIR}/inst/pthreads/.root.init.marker \
#  ${BUILD_DIR}/inst/gcc-full/.root.init.marker

${BUILD_DIR}/inst/%/.mkdir.marker: \
	-mkdir -p $(dir $@)
	@touch -t 197001020101 $@

########################################
# Pull mingw-w64-specific patches
########################################

patch-pull: \
    src/patches/.patches.pull.marker

src/patches/.patches.pull.marker: \
    src/patches/.mkdir.marker
	cd $(dir $@) && \
	svn co --non-interactive --no-auth-cache \
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
	cvs -d ":pserver:anoncvs@sourceware.org:/cvs/src" -z3 \
	    checkout -d . -N binutils
	@touch $@
else
	cd $(dir $@) && \
	cvs -d ":pserver:anoncvs@sourceware.org:/cvs/src" -z3 \
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
  GCC_REVISION := $(shell TZ=Z svn log --non-interactive --no-auth-cache \
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
	svn co --non-interactive --no-auth-cache --revision ${GCC_REVISION} \
	       svn://gcc.gnu.org/svn/gcc/$(strip ${GCC_BRANCH})/ .
	@touch $@

  ifneq (,$(strip ${GCC_UPDATE}))
.PHONY: src/gcc/gcc/.gcc.pull.marker
  endif

########################################
# Pull pthreads-win32 from cvs
########################################
pthreads-download: \
    src/pthreads/.pthreads.download.marker

src/pthreads/.pthreads.download.marker: \
    src/.mkdir.marker
	cd src && cvs -z9 -d ${PTHREADS_CVS_PULL} checkout -D ${PTHREADS_CVS_DATE} pthreads
	@touch $@

ifneq (,$(strip ${PTHREADS_UPDATE}))
.PHONY: src/pthreads/.pthreads.download.marker
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
    src/gmp/src/.mkdir.marker \
    src/patches/.patches.pull.marker
	$(TAR) -C $(dir $@)/src --strip-components=1 -xjvf $<
	cd $(dir $@)src && patch -Np0 -i ../../patches/experimental/lrn_patches/gmp-compilers.patch
	@touch $@

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
    src/mpfr/src/.mkdir.marker \
    src/patches/.patches.pull.marker
	$(TAR) -C $(dir $@)/src --strip-components=1 -xjvf $<
	cd $(dir $@)src && patch -Np1 -i ../../patches/mpfr-3.0.0/allpatches-p3
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
# Download PPL
########################################

ppl-download: \
    src/ppl.tar.gz

src/ppl.tar.gz: \
    src/.mkdir.marker
	$(WGET) $@ ftp://gcc.gnu.org/pub/gcc/infrastructure/ppl-$(strip ${PPL_VERSION}).tar.gz

########################################
# Extract PPL
########################################

ppl-extract: \
    src/ppl/.ppl.extract.marker

src/ppl/.ppl.extract.marker: \
    src/ppl.tar.gz \
    src/ppl/src/.mkdir.marker
	$(TAR) -C $(dir $@)/src --strip-components=1 -xzvf $<
	@touch $@

########################################
# Download CLooG
########################################

cloog-download: \
    src/cloog.tar.gz

src/cloog.tar.gz: \
    src/.mkdir.marker
	$(WGET) $@ ftp://gcc.gnu.org/pub/gcc/infrastructure/cloog-ppl-$(strip ${CLOOG_VERSION}).tar.gz

########################################
# Extract CLooG
########################################

cloog-extract: \
    src/cloog/.cloog.extract.marker

src/cloog/.cloog.extract.marker: \
    src/cloog.tar.gz \
    src/cloog/src/.mkdir.marker
	$(TAR) -C $(dir $@)/src --strip-components=1 -xzvf $<
	@touch $@

########################################
# Pull mingw
########################################

mingw-pull: \
    src/mingw/.mingw.pull.marker

src/mingw/.mingw.pull.marker: \
    src/mingw/.mkdir.marker
	svn checkout --non-interactive --no-auth-cache --revision ${MINGW_REVISION} \
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
    src/gmp/.gmp.extract.marker \
    src/mpfr/.mpfr.extract.marker \
    src/mpc/.mpc.extract.marker \
    src/ppl/.ppl.extract.marker \
    src/cloog/.cloog.extract.marker \
    src/mingw/.mingw.pull.marker \
    src/pthreads/.pthreads.download.marker
endif

${SRC_ARCHIVE}:
	find src/ >/dev/null
	$(TAR) vcJf $@ --owner 0 --group 0 --exclude=.svn\
	    --exclude=.*.marker --exclude=CVS --exclude=gmp.tar.bz2 \
	    --exclude=mpfr.tar.bz2 --exclude=mpc.tar.gz \-C src .

########################################
# Extract source tarball
########################################
src-extract:: \
    ${BUILD_DIR}/.extract.marker

${BUILD_DIR}/.extract.marker: \
    ${SRC_ARCHIVE}
	-mkdir -p $(dir $@)
	$(TAR) -C $(dir $@) -xvJpf $<
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
	  --host=${TARGET_ARCH} \
	  ${CONFIG_BUILD_ARGS}
	@touch $@

########################################
# Install mingw-w64 headers
########################################
headers-install: \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker

${BUILD_DIR}/mingw-headers/obj/.install.marker: \
    ${BUILD_DIR}/inst/mingw-w64-headers/.mkdir.marker \
    ${BUILD_DIR}/inst/mingw-w64-headers/.root.init.marker \
    ${BUILD_DIR}/mingw-headers/obj/.config.marker
	${MAKE} -C $(dir $@) install
	${MAKE} -C $(dir $@) install prefix=/mingw DESTDIR=${BUILDDIR}/inst/mingw-w64-headers
	@touch $@

########################################
# Configure binutils
########################################
binutils-configure: \
    ${BUILD_DIR}/binutils/obj/.config.marker

${BUILD_DIR}/binutils/obj/.config.marker: \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker \
    ${BUILD_DIR}/binutils/obj/.mkdir.marker \
    ${BUILD_DIR}/root/.root.init.marker
	cd $(dir $@) && \
	../../../${BUILD_DIR}/binutils/src/configure \
	    --target=${TARGET_ARCH} \
	    ${BINUTILS_CONFIG_HOST_ARGS} \
	    ${CONFIG_BUILD_ARGS} \
	    --prefix=${CURDIR}/${BUILD_DIR}/root \
	    --with-sysroot=${CURDIR}/${BUILD_DIR}/root \
	    ${BINUTILS_CONFIG_EXTRA_ARGS_MULTI_${ENABLE_MULTILIB}} \
	    ${BINUTILS_CONFIG_EXTRA_ARGS}
	@touch $@

########################################
# Compile binutils
########################################
binutils-compile: \
    ${BUILD_DIR}/binutils/obj/.compile.marker

${BUILD_DIR}/binutils/obj/.compile.marker: \
    ${BUILD_DIR}/binutils/obj/.config.marker
	${MAKE} ${MAKE_OPTS} -C $(dir $@)
	@touch $@

########################################
# Install binutils
########################################
binutils-install: \
    ${BUILD_DIR}/binutils/obj/.install.marker

${BUILD_DIR}/binutils/obj/.install.marker: \
    ${BUILD_DIR}/inst/binutils/.mkdir.marker \
    ${BUILD_DIR}/inst/binutils/.root.init.marker \
    ${BUILD_DIR}/binutils/obj/.compile.marker
	${MAKE} -C $(dir $@) install
	${MAKE} -C $(dir $@) install prefix=/mingw DESTDIR=${BUILDDIR}/inst/binutils
	@touch $@

########################################
# GCC cross compiling support - winsup
########################################
gcc-winsup: \
    ${BUILD_DIR}/gcc/gcc/.winsup.marker

${BUILD_DIR}/gcc/gcc/.winsup.marker: \
    ${BUILD_DIR}/.extract.marker \
    ${BUILD_DIR}/root/.root.init.marker
	test -e ${BUILD_DIR}/gcc/gcc/winsup  || \
	  cmd //C mklink //D ${subst /,\\,${BUILDDIRW}/gcc/gcc/winsup} "${subst /,\\,${BUILDDIRW}/root}"
	test -e ${BUILD_DIR}/gcc/gcc/winsup
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
	../../../${BUILD_DIR}/gmp/src/configure \
	    $(or ${GCC_CONFIG_HOST_ARGS},--host=none-none-none) \
	    $(CONFIG_BUILD_ARGS) \
	    CPPFLAGS=-fexceptions \
	    --enable-cxx \
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
    ${BUILD_DIR}/inst/gmp/.mkdir.marker \
    ${BUILD_DIR}/inst/gmp/.root.init.marker \
    ${BUILD_DIR}/gmp/obj/.compile.marker \
    ${BUILD_DIR}/gmp/install/.mkdir.marker
	$(MAKE) -C ${BUILD_DIR}/gmp/obj install
	$(MAKE) -C ${BUILD_DIR}/gmp/obj install prefix=/mingw DESTDIR=${BUILDDIR}/inst/gmp
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
	../../../${BUILD_DIR}/mpfr/src/configure \
	    ${GCC_CONFIG_HOST_ARGS} \
	    $(CONFIG_BUILD_ARGS) \
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
    ${BUILD_DIR}/inst/mpfr/.mkdir.marker \
    ${BUILD_DIR}/inst/mpfr/.root.init.marker \
    ${BUILD_DIR}/mpfr/obj/.compile.marker \
    ${BUILD_DIR}/mpfr/install/.mkdir.marker
	$(MAKE) -C ${BUILD_DIR}/mpfr/obj install
	$(MAKE) -C ${BUILD_DIR}/mpfr/obj install prefix=/mingw DESTDIR=${BUILDDIR}/inst/mpfr
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
	../../../${BUILD_DIR}/mpc/src/configure \
	    ${GCC_CONFIG_HOST_ARGS} \
	    $(CONFIG_BUILD_ARGS) \
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
    ${BUILD_DIR}/inst/mpc/.mkdir.marker \
    ${BUILD_DIR}/inst/mpc/.root.init.marker \
    ${BUILD_DIR}/mpc/obj/.compile.marker \
    ${BUILD_DIR}/mpc/install/.mkdir.marker
	$(MAKE) -C ${BUILD_DIR}/mpc/obj install
	$(MAKE) -C ${BUILD_DIR}/mpc/obj install prefix=/mingw DESTDIR=${BUILDDIR}/inst/mpc
	@touch $@

########################################
# Configure PPL
########################################
ppl-configure: \
    ${BUILD_DIR}/ppl/obj/.config.marker

${BUILD_DIR}/ppl/obj/.config.marker: \
    ${BUILD_DIR}/ppl/obj/.mkdir.marker \
    ${BUILD_DIR}/root/.root.init.marker \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker \
    ${BUILD_DIR}/gmp/install/.install.marker
	cd $(dir $@) && \
	../../../${BUILD_DIR}/ppl/src/configure \
        ${GCC_CONFIG_HOST_ARGS} \
        $(CONFIG_BUILD_ARGS) \
        CPPFLAGS="-D__GMP_BITS_PER_MP_LIMB=GMP_LIMB_BITS" \
        --enable-static --disable-shared \
        --prefix=${CURDIR}/${BUILD_DIR}/ppl/install \
        --with-libgmp-prefix=${CURDIR}/${BUILD_DIR}/gmp/install \
        --with-libgmpxx-prefix=${CURDIR}/${BUILD_DIR}/gmp/install
	@touch $@

# CPPFLAGS="-D__GMP_BITS_PER_MP_LIMB=GMP_LIMB_BITS"
# This is used to work around PPL not detecting gmp-5.0.0 properly

########################################
# Compile PPL
########################################
ppl-compile: \
    ${BUILD_DIR}/ppl/obj/.compile.marker

${BUILD_DIR}/ppl/obj/.compile.marker: \
    ${BUILD_DIR}/ppl/obj/.config.marker
	$(MAKE) -C $(dir $@)
	@touch $@

########################################
# Install PPL
########################################
ppl-install: \
    ${BUILD_DIR}/ppl/install/.install.marker

${BUILD_DIR}/ppl/install/.install.marker: \
    ${BUILD_DIR}/inst/ppl/.mkdir.marker \
    ${BUILD_DIR}/inst/ppl/.root.init.marker \
    ${BUILD_DIR}/ppl/obj/.compile.marker \
    ${BUILD_DIR}/ppl/install/.mkdir.marker
	$(MAKE) -C ${BUILD_DIR}/ppl/obj install
	$(MAKE) -C ${BUILD_DIR}/ppl/obj install prefix=/mingw DESTDIR=${BUILDDIR}/inst/ppl
	@touch $@

########################################
# Configure CLooG
########################################
cloog-configure: \
    ${BUILD_DIR}/cloog/obj/.config.marker

${BUILD_DIR}/cloog/obj/.config.marker: \
    ${BUILD_DIR}/cloog/obj/.mkdir.marker \
    ${BUILD_DIR}/root/.root.init.marker \
    ${BUILD_DIR}/mingw-headers/obj/.install.marker \
    ${BUILD_DIR}/gmp/install/.install.marker \
    ${BUILD_DIR}/ppl/install/.install.marker
	cd $(dir $@) && \
	../../../${BUILD_DIR}/cloog/src/configure \
        ${GCC_CONFIG_HOST_ARGS} \
        $(CONFIG_BUILD_ARGS) \
            --enable-static --disable-shared \
            --prefix=${CURDIR}/${BUILD_DIR}/cloog/install \
            --with-gmp=${CURDIR}/${BUILD_DIR}/gmp/install \
            --with-ppl=${CURDIR}/${BUILD_DIR}/ppl/install \
            --with-host-libstdcxx="-lstdc++ -lsupc++ -lm"
	@touch $@

# -lm workarounds some weirdness where -lm was missing when linking backends

########################################
# Compile CLooG
########################################
cloog-compile: \
    ${BUILD_DIR}/cloog/obj/.compile.marker

${BUILD_DIR}/cloog/obj/.compile.marker: \
    ${BUILD_DIR}/cloog/obj/.config.marker
	$(MAKE) -C $(dir $@)
	@touch $@

########################################
# Install CLooG
########################################
cloog-install: \
    ${BUILD_DIR}/cloog/install/.install.marker

${BUILD_DIR}/cloog/install/.install.marker: \
    ${BUILD_DIR}/inst/cloog/.mkdir.marker \
    ${BUILD_DIR}/inst/cloog/.root.init.marker \
    ${BUILD_DIR}/cloog/obj/.compile.marker \
    ${BUILD_DIR}/cloog/install/.mkdir.marker
	$(MAKE) -C ${BUILD_DIR}/cloog/obj install
	$(MAKE) -C ${BUILD_DIR}/cloog/obj install prefix=/mingw DESTDIR=${BUILDDIR}/inst/cloog
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
    ${BUILD_DIR}/ppl/install/.install.marker \
    ${BUILD_DIR}/cloog/install/.install.marker \
    ${BUILD_DIR}/root/.root.init.marker
	cd $(dir $@) && \
	../../../${BUILD_DIR}/gcc/gcc/configure \
        --target=${TARGET_ARCH} \
        ${GCC_CONFIG_HOST_ARGS} \
        $(CONFIG_BUILD_ARGS) \
        --prefix=${CURDIR}/${BUILD_DIR}/root \
        --with-sysroot=${CURDIR}/${BUILD_DIR}/root \
            --with-gmp=${CURDIR}/${BUILD_DIR}/gmp/install \
            --with-mpfr=${CURDIR}/${BUILD_DIR}/mpfr/install \
            --with-mpc=${CURDIR}/${BUILD_DIR}/mpc/install \
            --with-ppl=${CURDIR}/${BUILD_DIR}/ppl/install \
            --with-cloog=${CURDIR}/${BUILD_DIR}/cloog/install \
            --with-host-libstdcxx="-lstdc++ -lsupc++ -lm" \
	    --with-boot-ldflags="-L${shell cd /mingw/${BUILD_ARCH}/lib ; pwd}" \
        --enable-languages=c${GCC_CPP_${GCC_CPP}}${GCC_FORTRAN_${GCC_FORTRAN}}${GCC_OBJC_${GCC_OBJC}}${GCC_JAVA_${GCC_JAVA}}${GCC_ADA_${GCC_ADA}} \
        ${GCC_CONFIG_EXTRA_ARGS_MULTI_${ENABLE_MULTILIB}} \
        ${GCC_CONFIG_EXTRA_ARGS}
	@touch $@
# -lm workarounds some weirdness where -lm was missing when linking backends

########################################
# Compile GCC stage 1
########################################
gcc-bootstrap-compile: \
    ${BUILD_DIR}/gcc/obj/.bootstrap.compile.marker

${BUILD_DIR}/gcc/obj/.bootstrap.compile.marker: \
    ${BUILD_DIR}/gcc/obj/.config.marker
	${MAKE} ${MAKE_OPTS} -C $(dir $@) all-gcc
	@touch $@

########################################
# Install GCC stage 1
########################################
gcc-bootstrap-install: \
    ${BUILD_DIR}/gcc/obj/.bootstrap.install.marker

${BUILD_DIR}/gcc/obj/.bootstrap.install.marker: \
    ${BUILD_DIR}/inst/gcc-stage-1/.mkdir.marker \
    ${BUILD_DIR}/inst/gcc-stage-1/.root.init.marker \
    ${BUILD_DIR}/gcc/obj/.bootstrap.compile.marker
	${MAKE} -C $(dir $@) install-gcc
	${MAKE} -C $(dir $@) install-gcc prefix=/mingw DESTDIR=${BUILDDIR}/inst/gcc-stage-1
	@touch $@

########################################
# Configure mingw-w64 CRT
########################################
crt-configure: \
    ${BUILD_DIR}/mingw/obj/.config.marker

${BUILD_DIR}/mingw/obj/.config.marker: \
    ${BUILD_DIR}/gcc/obj/.bootstrap.install.marker \
    ${BUILD_DIR}/mingw/obj/.mkdir.marker
	cd $(dir $@) && \
	$(ADD_BIN_PATH) ../../../${BUILD_DIR}/mingw/mingw-w64-crt/configure \
	    $(CONFIG_BUILD_ARGS) \
	    --host=${TARGET_ARCH} \
	    --prefix=${CURDIR}/${BUILD_DIR}/root \
	    --with-sysroot=${CURDIR}/${BUILD_DIR}/root \
	    ${MINGW_CONFIG_EXTRA_ARGS_MULTI_${ENABLE_MULTILIB}} \
	    ${MINGW_CONFIG_EXTRA_ARGS}
	@touch $@

########################################
# Compile mingw-w64 CRT
########################################
crt-compile: \
    ${BUILD_DIR}/mingw/obj/.compile.marker

${BUILD_DIR}/mingw/obj/.compile.marker: \
    ${BUILD_DIR}/mingw/obj/.config.marker
	$(ADD_BIN_PATH) ${MAKE} ${MAKE_OPTS} -C $(dir $@)
	@touch $@

########################################
# Install mingw-w64 CRT
########################################
crt-install: \
    ${BUILD_DIR}/mingw/obj/.install.marker

${BUILD_DIR}/mingw/obj/.install.marker: \
    ${BUILD_DIR}/inst/mingw-w64-crt/.mkdir.marker \
    ${BUILD_DIR}/inst/mingw-w64-crt/.root.init.marker \
    ${BUILD_DIR}/mingw/obj/.compile.marker
	$(ADD_BIN_PATH) ${MAKE} -C $(dir $@) install
	$(ADD_BIN_PATH) ${MAKE} -C $(dir $@) install prefix=/mingw DESTDIR=${BUILDDIR}/inst/mingw-w64-crt
	@touch $@

########################################
# Compile libgcc
########################################

gcc-libgcc-compile: \
    ${BUILD_DIR}/gcc/obj/.libgcc.compile.marker

${BUILD_DIR}/gcc/obj/.libgcc.compile.marker: \
    ${BUILD_DIR}/mingw/obj/.install.marker
	${MAKE} ${MAKE_OPTS} -C $(dir $@) all-target-libgcc
	@touch $@

########################################
# Install libgcc
########################################
gcc-libgcc-install: \
    ${BUILD_DIR}/gcc/obj/.libgcc.install.marker

${BUILD_DIR}/gcc/obj/.libgcc.install.marker: \
    ${BUILD_DIR}/inst/libgcc/.mkdir.marker \
    ${BUILD_DIR}/inst/libgcc/.root.init.marker \
    ${BUILD_DIR}/gcc/obj/.libgcc.compile.marker
	${MAKE} -C $(dir $@) install-target-libgcc
	${MAKE} -C $(dir $@) install-target-libgcc prefix=/mingw DESTDIR=${BUILDDIR}/inst/libgcc
	@touch $@

########################################
# Prep pthreads-win32 from cvs
########################################
pthreads-prep: \
    ${BUILD_DIR}/pthreads/.pthreads.prep

${BUILD_DIR}/pthreads/.pthreads.prep: \
    ${BUILD_DIR}/root/.root.init.marker \
    ${BUILD_DIR}/inst/pthreads/.mkdir.marker \
    ${BUILD_DIR}/inst/pthreads/.root.init.marker \
    src/patches/.patches.pull.marker \
    src/pthreads/.pthreads.download.marker
	cp -prf src/pthreads ${BUILD_DIR}/
	cp -p src/patches/pthreads_win32/lrn-pthreads-w64sup.patch $(dir $@)
	cd $(dir $@) && patch -Np1 -i lrn-pthreads-w64sup.patch
	cp -p $(dir $@)/GNUmakefile $(dir $@)GNUmakefile.ori
	@touch $@

########################################
# Build pthreads-win32 from cvs
########################################
pthreads-build: \
    ${BUILD_DIR}/pthreads/.pthreads.build.${ENABLE_MULTILIB}

${BUILD_DIR}/pthreads/.pthreads.build.Y: \
    ${BUILD_DIR}/pthreads/.pthreads.build.x86_64-w64-mingw32 \
    ${BUILD_DIR}/pthreads/.pthreads.build.i686-w64-mingw32
	@touch $@

${BUILD_DIR}/pthreads/.pthreads.build.N: \
    ${BUILD_DIR}/pthreads/.pthreads.build.${TARGET_ARCH}
	@touch $@

${BUILD_DIR}/pthreads/.pthreads.build.x86_64-w64-mingw32: \
    ${BUILD_DIR}/pthreads/.pthreads.prep \
    ${BUILD_DIR}/mingw/obj/.install.marker \
    ${BUILD_DIR}/gcc/obj/.libgcc.install.marker
	sed \
	  -e 's/^\(OPT\s*=.*\)$$/\1 -m64/' \
	  -e 's/^\(RCFLAGS\s*=.*\)$$/\1 -F pe-x86-64/' \
	  < $(dir $@)GNUmakefile.ori > $(dir $@)GNUmakefile
	$(ADD_BIN_PATH) ${MAKE} ${MAKE_OPTS} -C $(dir $@) CROSS=${TARGET_ARCH}- LIBDEST="./${TARGET_ARCH}/" DLLDEST="./${TARGET_ARCH}/" DLLTOOL_AT_FLAGS="-m i386:x86-64" DLLTOOL_DEF_FLAGS="-m i386:x86-64" $(PTHREADS_MAKE_ARGS)
	@touch $@

${BUILD_DIR}/pthreads/.pthreads.build.i686-w64-mingw32: \
    ${BUILD_DIR}/pthreads/.pthreads.prep \
    ${BUILD_DIR}/mingw/obj/.install.marker \
    ${BUILD_DIR}/gcc/obj/.libgcc.install.marker
	sed \
	  -e 's/^\(OPT\s*=.*\)$$/\1 -m32/' \
	  -e 's/^\(RCFLAGS\s*=.*\)$$/\1 -F pe-i386/' \
	  < $(dir $@)GNUmakefile.ori > $(dir $@)GNUmakefile
	$(ADD_BIN_PATH) ${MAKE} ${MAKE_OPTS} -C $(dir $@) CROSS=${TARGET_ARCH}- LIBDEST="./${TARGET_ARCH}/" DLLDEST="./${TARGET_ARCH}/" DLLTOOL_AT_FLAGS="-m i386" DLLTOOL_DEF_FLAGS="-m i386" $(PTHREADS_MAKE_ARGS)
	@touch $@

########################################
# Install pthreads-win32 from cvs
########################################
ifeq (${TARGET_ARCH}, x86_64-w64-mingw32)
  PTHREAD_DLL := ${TARGET_ARCH}/libpthread_gc-2.dll
  PTHREAD_LIB := ${TARGET_ARCH}/libpthread_gc.a
else
  PTHREAD_DLL := ${TARGET_ARCH}/libpthread_gc-2.dll
  PTHREAD_LIB := ${TARGET_ARCH}/libpthread_gc.a
endif

pthreads-install: \
    ${BUILD_DIR}/pthreads/.pthreads.install.${ENABLE_MULTILIB}

${BUILD_DIR}/pthreads/.pthreads.install.common: \
    ${BUILD_DIR}/pthreads/.pthreads.prep
	cp -p ${BUILD_DIR}/pthreads/pthread.h \
	  ${BUILD_DIR}/pthreads/sched.h \
	  ${BUILD_DIR}/pthreads/semaphore.h \
	  ${BUILD_DIR}/root/${TARGET_ARCH}/include/
	cp -p ${BUILD_DIR}/pthreads/${PTHREAD_DLL} \
	  ${BUILD_DIR}/root/${TARGET_ARCH}/bin/
	cp -p ${BUILD_DIR}/pthreads/${PTHREAD_LIB} \
	  ${BUILD_DIR}/root/${TARGET_ARCH}/lib/
	cp -p ${BUILD_DIR}/pthreads/pthread.h \
	  ${BUILD_DIR}/pthreads/sched.h \
	  ${BUILD_DIR}/pthreads/semaphore.h \
	  ${BUILD_DIR}/inst/pthreads/${TARGET_ARCH}/include/
	cp -p ${BUILD_DIR}/pthreads/${PTHREAD_DLL} \
	  ${BUILD_DIR}/inst/pthreads/${TARGET_ARCH}/bin/
	cp -p ${BUILD_DIR}/pthreads/${PTHREAD_LIB} \
	  ${BUILD_DIR}/inst/pthreads/${TARGET_ARCH}/lib/
	@touch $@

${BUILD_DIR}/pthreads/.pthreads.install.Y: \
    ${BUILD_DIR}/pthreads/.pthreads.build.Y \
    ${BUILD_DIR}/pthreads/.pthreads.install.common \
    ${BUILD_DIR}/pthreads/.pthreads.install.x86_64-w64-mingw32 \
    ${BUILD_DIR}/pthreads/.pthreads.install.i686-w64-mingw32
	@touch $@

${BUILD_DIR}/pthreads/.pthreads.install.N: \
    ${BUILD_DIR}/pthreads/.pthreads.build.N \
    ${BUILD_DIR}/pthreads/.pthreads.install.common \
    ${BUILD_DIR}/pthreads/.pthreads.install.${TARGET_ARCH}
	@touch $@

${BUILD_DIR}/pthreads/.pthreads.install.x86_64-w64-mingw32: \
    ${BUILD_DIR}/pthreads/.pthreads.build.x86_64-w64-mingw32
	cp -p ${BUILD_DIR}/pthreads/x86_64-w64-mingw32/libpthread_gc-2.dll \
 	  ${BUILD_DIR}/root/x86_64-w64-mingw32/bin
	cp -fp ${BUILD_DIR}/pthreads/x86_64-w64-mingw32/libpthread_gc.a \
 	  ${BUILD_DIR}/root/x86_64-w64-mingw32/lib64
	cp -p ${BUILD_DIR}/pthreads/x86_64-w64-mingw32/libpthread_gc-2.dll \
 	  ${BUILD_DIR}/inst/pthreads/x86_64-w64-mingw32/bin
	cp -fp ${BUILD_DIR}/pthreads/x86_64-w64-mingw32/libpthread_gc.a \
 	  ${BUILD_DIR}/inst/pthreads/x86_64-w64-mingw32/lib64
	test ! -e ${BUILD_DIR}/root/x86_64-w64-mingw32/lib64/libpthread.a || \
	  cmd //C del ${subst /,\\,${BUILDDIRW}/root/x86_64-w64-mingw32/lib64/libpthread.a}
	cmd //C mklink ${subst /,\\,${BUILDDIRW}/root/x86_64-w64-mingw32/lib64/libpthread.a} "${subst /,\\,${BUILDDIRW}/root/x86_64-w64-mingw32/lib64/libpthread_gc.a}"
	test ! -e ${BUILD_DIR}/root/x86_64-w64-mingw32/bin/libpthread-2.dll || \
	  cmd //C del ${subst /,\\,${BUILDDIRW}/root/x86_64-w64-mingw32/bin/libpthread-2.dll}
	cmd //C mklink ${subst /,\\,${BUILDDIRW}/root/x86_64-w64-mingw32/bin/libpthread-2.dll} "${subst /,\\,${BUILDDIRW}/root/x86_64-w64-mingw32/bin/libpthread_gc-2.dll}"
	test ! -e ${BUILD_DIR}/inst/pthreads/x86_64-w64-mingw32/lib64/libpthread.a || \
	  cmd //C del ${subst /,\\,${BUILDDIRW}/inst/pthreads/x86_64-w64-mingw32/lib64/libpthread.a}
	cmd //C mklink ${subst /,\\,${BUILDDIRW}/inst/pthreads/x86_64-w64-mingw32/lib64/libpthread.a} "${subst /,\\,${BUILDDIRW}/inst/pthreads/x86_64-w64-mingw32/lib64/libpthread_gc.a}"
	test ! -e ${BUILD_DIR}/inst/pthreads/x86_64-w64-mingw32/bin/libpthread-2.dll || \
	  cmd //C del ${subst /,\\,${BUILDDIRW}/inst/pthreads/x86_64-w64-mingw32/bin/libpthread-2.dll}
	cmd //C mklink ${subst /,\\,${BUILDDIRW}/inst/pthreads/x86_64-w64-mingw32/bin/libpthread-2.dll} "${subst /,\\,${BUILDDIRW}/inst/pthreads/x86_64-w64-mingw32/bin/libpthread_gc-2.dll}"
	@touch $@

${BUILD_DIR}/pthreads/.pthreads.install.i686-w64-mingw32: \
    ${BUILD_DIR}/pthreads/.pthreads.build.i686-w64-mingw32
	cp -p ${BUILD_DIR}/pthreads/i686-w64-mingw32/libpthread_gc-2.dll \
 	  ${BUILD_DIR}/root/bin
	cp -fp ${BUILD_DIR}/pthreads/i686-w64-mingw32/libpthread_gc.a \
	  ${BUILD_DIR}/root/i686-w64-mingw32/lib32
	cp -p ${BUILD_DIR}/pthreads/i686-w64-mingw32/libpthread_gc-2.dll \
 	  ${BUILD_DIR}/inst/pthreads/bin
	cp -fp ${BUILD_DIR}/pthreads/i686-w64-mingw32/libpthread_gc.a \
	  ${BUILD_DIR}/inst/pthreads/i686-w64-mingw32/lib32
	test ! -e ${BUILD_DIR}/root/i686-w64-mingw32/lib32/libpthread.a || \
	  cmd //C del ${subst /,\\,${BUILDDIRW}/root/i686-w64-mingw32/lib32/libpthread.a}
	cmd //C mklink ${subst /,\\,${BUILDDIRW}/root/i686-w64-mingw32/lib32/libpthread.a} "${subst /,\\,${BUILDDIRW}/root/i686-w64-mingw32/lib32/libpthread_gc.a}"
	test ! -e ${BUILD_DIR}/root/bin/libpthread-2.dll || \
	  cmd //C del ${subst /,\\,${BUILDDIRW}/root/bin/libpthread-2.dll}
	cmd //C mklink ${subst /,\\,${BUILDDIRW}/root/bin/libpthread-2.dll} "${subst /,\\,${BUILD_DIR}/root/bin/libpthread_gc-2.dll}"
	test ! -e ${BUILD_DIR}/inst/pthreads/i686-w64-mingw32/lib32/libpthread.a || \
	  cmd //C del ${subst /,\\,${BUILDDIRW}/inst/pthreads/i686-w64-mingw32/lib32/libpthread.a}
	cmd //C mklink ${subst /,\\,${BUILDDIRW}/inst/pthreads/i686-w64-mingw32/lib32/libpthread.a} "${subst /,\\,${BUILDDIRW}/inst/pthreads/i686-w64-mingw32/lib32/libpthread_gc.a}"
	test ! -e ${BUILD_DIR}/inst/pthreads/bin/libpthread-2.dll || \
	  cmd //C del ${subst /,\\,${BUILDDIRW}/inst/pthreads/bin/libpthread-2.dll}
	cmd //C mklink ${subst /,\\,${BUILDDIRW}/inst/pthreads/bin/libpthread-2.dll} "${subst /,\\,${BUILD_DIR}/inst/pthreads/bin/libpthread_gc-2.dll}"
	@touch $@

########################################
# Compile full GCC
########################################
gcc-compile: \
    ${BUILD_DIR}/gcc/obj/.compile.marker \
    ${BUILD_DIR}/mingw/obj/.install.marker \

${BUILD_DIR}/gcc/obj/.compile.marker: \
    ${BUILD_DIR}/gcc/obj/.config.marker \
    ${BUILD_DIR}/mingw/obj/.install.marker \
	${BUILD_DIR}/pthreads/.pthreads.install.${ENABLE_MULTILIB}
	$(ADD_BIN_PATH) ${MAKE} ${MAKE_OPTS} -C $(dir $@)
	@touch $@

########################################
# Install full GCC
########################################
gcc-install: \
    ${BUILD_DIR}/gcc/obj/.install.marker

${BUILD_DIR}/gcc/obj/.install.marker: \
    ${BUILD_DIR}/inst/gcc-full/.mkdir.marker \
    ${BUILD_DIR}/inst/gcc-full/.root.init.marker \
    ${BUILD_DIR}/gcc/obj/.compile.marker
	$(ADD_BIN_PATH) ${MAKE} -C $(dir $@) install
	$(ADD_BIN_PATH) ${MAKE} -C $(dir $@) install prefix=/mingw DESTDIR=${BUILDDIR}/inst/gcc-full
	@touch $@

########################################
# Create release tarball
########################################
release-archive: \
    ${BIN_ARCHIVE}

${BIN_ARCHIVE}: \
    ${BUILD_DIR}/gcc/obj/.install.marker \
    ${BUILD_DIR}/pre.pack
	find ${BUILD_DIR}/root/ >/dev/null
	$(TAR) vcJf $@ -C ${BUILD_DIR}/root --owner 0 --group 0 \
	    --exclude=CVS --exclude=.svn --exclude=.*.marker \
            .

########################################
# Pre-pack cleanups
########################################
# Nothing yet
pre-pack: \
    ${BUILD_DIR}/pre.pack

${BUILD_DIR}/pre.pack: \
    ${BUILD_DIR}/gcc/obj/.install.marker
	@touch $@

################################################################################
# Helper targets
################################################################################

%/.mkdir.marker:
	-mkdir -p $(dir $@)
	@touch -t 197001020101 $@

################################################################################
# Host Tools Check (Check for required tools)
################################################################################
host-tools-check-helper = \
	@echo -n "Checking for${1}" \
	&& ($(call host-tools-check-helper-alternator,${2},${3}) \
	(echo -ne "\n[ Error $$? - Checked " && $(call host-tools-check-helper-alternator-error,${2}))) \
	&& echo -e "[OK]"

host-tools-check-helper-alternator = \
	$(foreach ex,${1},($(ex) $(or ${2},--version) &> /dev/null) ||)

host-tools-check-helper-alternator-error = \
	$(foreach ex,${1},echo -n "$(ex) " && ) \
	echo "]" && \
	false

host-tools-check-helper-gcc-list := gcc cc
host-tools-check-helper-cpp-list := g++ c++
host-tools-check-helper-wget-list := wget curl

host-tools-check::
	@echo Checking host tools
	@echo Note that the minimum required versions are not checked.
	$(call host-tools-check-helper, Concurrent Versions System, cvs)
	$(call host-tools-check-helper, Subversion, svn)
	$(call host-tools-check-helper, GNU Compiler Collection (gcc), $(host-tools-check-helper-gcc-list))
	$(call host-tools-check-helper, GNU Compiler Collection (g++), $(host-tools-check-helper-cpp-list))
	$(call host-tools-check-helper, GNU sed, sed)
	$(call host-tools-check-helper, GNU Awk, gawk)
	$(call host-tools-check-helper, patch, patch)
	$(call host-tools-check-helper, GNU tar, tar)
	$(call host-tools-check-helper, bzip2, bzip2,--help)
	$(call host-tools-check-helper, gzip, gzip)
	$(call host-tools-check-helper, wget or curl, $(host-tools-check-helper-wget-list))
	$(call host-tools-check-helper, flex, flex)
	$(call host-tools-check-helper, GNU bison, bison)
	$(call host-tools-check-helper, GNU gperf, gperf)
	$(call host-tools-check-helper, GNU texinfo (makeinfo), makeinfo)
	$(call host-tools-check-helper, Perl, perl)
	@echo All needed host tools seem to work fine!

help::
	@echo Available targets:
	@echo -e $(foreach t,all ${TARGETS} $@,\\t${t}\\n)

# build only the cross-compiler by default
all:: \
  ${BIN_ARCHIVE}

TARGETS := \
  host-tools-check \
  patch-pull \
  binutils-pull \
  gcc-pull \
  gmp-download \
  gmp-extract \
  mpfr-download \
  mpfr-extract \
  mpc-download \
  mpc-extract \
  ppl-download \
  ppl-extract \
  cloog-download \
  cloog-extract \
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
  ppl-configure \
  ppl-compile \
  ppl-install \
  cloog-configure \
  cloog-compile \
  cloog-install \
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
  pthreads-download \
  pthreads-prep \
  pthreads-build \
  pthreads-install \
  gcc-libgcc-compile \
  gcc-libgcc-install \
  pre-pack \
  ${NULL}

.PHONY: \
  all \
  ${TARGETS} \
  help \
  ${NULL}
