# -*- Makefile -*-
# ----------------------------------------------------------------------

$(warning WARNING: Discontinued on 2021-05-20, Racmacs does not use acmacs.r anymore)

# for Linux and set -o pipefail
SHELL = /bin/bash

PKG_NAME = acmacs.r

ROOT_DIR = $(HOME)/R
# ifneq ($(R),)
#   ROOT_DIR = $(R)/R
#   export TMPDIR = $(R)/T
# else ifeq ($(ROOT_DIR),)
#   $(error Please provide ROOT_DIR, e.g. make ROOT_DIR=/my/local/r/installation)
# endif

PKG_DIR = $(ROOT_DIR)/pkg
LIB_DIR = $(ROOT_DIR)/library
OUT_DIR = $(ROOT_DIR)/$(PKG_NAME)
PKG_VERSION = $(shell awk '/^Version:/ {print $$2}' DESCRIPTION)
PKG_FILE = $(PKG_DIR)/$(PKG_NAME)_$(PKG_VERSION).tar.gz
INSTALLED_LIB = $(LIB_DIR)/$(PKG_NAME)/libs/acmacs.r.so

UNAME = $(shell uname)

ifeq ($(UNAME),Linux)
LD_LIBRARY_PATH = "$(LIB_DIR)/acmacs.r/libs:$$LD_LIBRARY_PATH"
else
LD_LIBRARY_PATH = ""
endif

ifeq ($(UNAME),Darwin)
  PKG_PLATFORM = macOS-$(shell /usr/bin/sw_vers -productVersion | /usr/bin/awk -F '.' '{print $$1"."$$2}')
else
  PKG_PLATFORM = $(UNAME)
endif

all: bin

NO_CLEAN_TARGET=1
include $(ACMACSD_ROOT)/share/Makefile.config

install: build | $(LIB_DIR)
	env ACMACSD_SOURCES_ROOT=$(AD_SOURCES_ROOT) R CMD INSTALL --clean --debug -l $(LIB_DIR) $(PKG_FILE)

bin: build | $(LIB_DIR)
	@echo '>>>> BIN'
	env LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) ACMACSD_SOURCES_ROOT=$(AD_SOURCES_ROOT) MAKE=$(MAKE) R CMD INSTALL --clean --build --debug --built-timestamp=$(shell date +%Y-%m-%d) --library=$(LIB_DIR) $(PKG_FILE)
	if [ -f $(ROOT_DIR)/$(PKG_NAME)_$(PKG_VERSION).tgz ]; then \
	  mv $(ROOT_DIR)/$(PKG_NAME)_$(PKG_VERSION).tgz $(ROOT_DIR)/$(PKG_NAME)_$(PKG_VERSION)_R_$(PKG_PLATFORM).tgz; \
	elif [ -f $(PKG_NAME)_$(PKG_VERSION).tgz ]; then \
	  mv $(PKG_NAME)_$(PKG_VERSION).tgz $(ROOT_DIR)/$(PKG_NAME)_$(PKG_VERSION)_R_$(PKG_PLATFORM).tgz; \
	fi
	if [ -f $(PKG_NAME)_$(PKG_VERSION)_R_x86_64-pc-linux-gnu.tar.gz ]; then \
	  mv $(PKG_NAME)_$(PKG_VERSION)_R_x86_64-pc-linux-gnu.tar.gz $(ROOT_DIR); \
	fi

build: compile-attributes | $(PKG_DIR)
	@echo '>>>> BUILD' $$(pwd)
	D=$$(pwd) && cd $(PKG_DIR) && env ACMACSD_SOURCES_ROOT=$(AD_SOURCES_ROOT) MAKE=$(MAKE) R CMD build "$$D"

check: build | $(OUT_DIR) $(LIB_DIR)
	env ACMACSD_SOURCES_ROOT=$(AD_SOURCES_ROOT) MAKE=$(MAKE) R CMD check -o $(OUT_DIR) -l $(LIB_DIR) --no-examples --no-manual $(PKG_FILE)

compile-attributes:
	Rscript -e "library(Rcpp); compileAttributes(verbose=TRUE);"

$(INSTALLED_LIB): $(wildcard src/*.cpp) $(wildcard src/*.hh)
	$(MAKE) install

test: bin
	$(MAKE) test-only

test-only:
	set -o pipefail; R_LIBS=$(LIB_DIR) Rscript --vanilla --default-packages=methods,utils,devtools,stats -e 'library("acmacs.r"); testthat::test_dir("tests/testthat/", stop_on_failure=TRUE, stop_on_warning=TRUE)' | cat 

test2: $(INSTALLED_LIB)
	@#R_LIBS=$(LIB_DIR) Rscript --vanilla -e 'library(acmacs.r); print(sessionInfo())'
	@#R_LIBS=$(LIB_DIR) Rscript --vanilla tests/test-move-point-speed.R
	R_LIBS=$(LIB_DIR) Rscript --vanilla tests/test-export-table.R

rtags:

clean:
	rm -rf $(OUT_DIR) $(ROOT_DIR)/$(PKG_NAME)_*.tgz $(PKG_FILE) $(LIB_DIR)/$(PKG_NAME) src/*.o src/*.so

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(PKG_DIR):
	mkdir -p $(PKG_DIR)

# ======================================================================
### Local Variables:
### eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
### End:
