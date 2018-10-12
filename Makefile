# -*- Makefile -*-

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

ifeq ($(shell uname),Linux)
LD_LIBRARY_PATH = "$(LIB_DIR)/acmacs.r/libs:$$LD_LIBRARY_PATH"
else
LD_LIBRARY_PATH = ""
endif


all: bin

install: build | $(LIB_DIR)
	R CMD INSTALL --clean --debug -l $(LIB_DIR) $(PKG_FILE)

bin: build | $(LIB_DIR)
	cd $(ROOT_DIR); env LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) R CMD INSTALL --build --clean --debug -l $(LIB_DIR) $(PKG_FILE)

build: compile-attributes | $(PKG_DIR)
	D=$$(pwd) && cd $(PKG_DIR) && R CMD build "$$D"

check: build | $(OUT_DIR) $(LIB_DIR)
	R CMD check -o $(OUT_DIR) -l $(LIB_DIR) --no-examples --no-manual $(PKG_FILE)

compile-attributes:
	Rscript -e "library(Rcpp); compileAttributes(verbose=TRUE);"

test:
	R_LIBS=$(LIB_DIR) Rscript --vanilla --default-packages=methods,utils,devtools,stats -e 'devtools::test()'

clean:
	rm -rf $(OUT_DIR) $(ROOT_DIR)/$(PKG_NAME)_*.tgz $(PKG_FILE) $(LIB_DIR)/$(PKG_NAME) src/*.o src/*.so

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(PKG_DIR):
	mkdir -p $(PKG_DIR)
