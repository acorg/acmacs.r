# -*- Makefile -*-

PKG_NAME = acmacs.r

ifneq ($(R),)
  ROOT_DIR = $(R)/R
else ifeq ($(ROOT_DIR),)
  $(error Please provide ROOT_DIR, e.g. make ROOT_DIR=/my/local/r/installation)
endif

PKG_DIR = $(ROOT_DIR)/pkg
LIB_DIR = $(ROOT_DIR)/library
OUT_DIR = $(ROOT_DIR)/$(PKG_NAME)

ifneq ($(T),"")
  export TMPDIR = $(T)
endif

all: bin

install: build | $(LIB_DIR)
	R CMD INSTALL --clean --debug -l $(LIB_DIR) $(PKG_DIR)/$(PKG_NAME)_*.tar.gz

bin: build | $(LIB_DIR)
	cd $(ROOT_DIR); R CMD INSTALL --build --clean --debug -l $(LIB_DIR) $(PKG_DIR)/$(PKG_NAME)_*.tar.gz

build: compile-attributes | $(PKG_DIR)
	D=$$(pwd) && cd $(PKG_DIR) && R CMD build "$$D"

check: build | $(OUT_DIR) $(LIB_DIR)
	R CMD check -o $(OUT_DIR) -l $(LIB_DIR) --no-examples --no-manual $(PKG_DIR)/$(PKG_NAME)_*.tar.gz

compile-attributes:
	Rscript -e "library(Rcpp); compileAttributes(verbose=TRUE);"

test:
	R_LIBS=$(LIB_DIR) Rscript --vanilla --default-packages=methods,utils,devtools,stats -e 'devtools::test()'
	#rm -f src/*.o src/*.so src/RcppExports.cpp

clean:
	rm -rf $(OUT_DIR) $(ROOT_DIR)/$(PKG_NAME)_*.tgz $(PKG_DIR)/$(PKG_NAME)_*.tar.gz $(LIB_DIR)/$(PKG_NAME) src/*.o src/*.so

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(PKG_DIR):
	mkdir -p $(PKG_DIR)
