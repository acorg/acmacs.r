acmacs.r package
================

This package provides R 3.6 with ability to read, modify, save the data stored in acmacs and
lispmds files.

Supported platforms
===================

 - macOS 10.12 and 10.13

 - Linux

Features
========

To Do
-----
- Merging two tables
- Merging a table into an optimised map, freezing that map and only optimised the data added
- Overlay merge of two maps. Layouts are oriented using procrustes, merged, then relaxed from the merging state. (not used very often)

Implemented
-----------
- [2.1] Grid test to find trapped and hemisphering points on a map, using multiple threads to perform optimization in parallel
- [2.0] Creating chart (table) from scratch, modifying antigen and serum fields, inserting and removing antigens/sera
- [1.0] Reading .ace, .acd1, .save, writing .ace, .save, modifying plot spec


Dependencies
=============

Rcpp 1.0.1 or later is required.

```R
install.packages("Rcpp")
```

Installation
============
https://cran.r-project.org/web/packages/Rcpp/index.html
R 3.6 is required, other versions are not supported!

Package is supposed to be installed from a binary archive for the
specific platform. Building package from sources is complicated and it
cannot be done within R.

Package is in an alpha stage, it's worth perhaps installing it into a
temporary directory, that directory (/tmp/R/library in the example
below) must exist before you submit install.packages command.

Required R libraries
--------------------

```R
install.packages(c("Rcpp", "devtools", "testthat", "roxygen2"))
```

macOS 10.14
---------------------

Installation size: ~5Mb

Please replace /tmp/R/library with your local directory for packages
or remove lib="/tmp/R/library" argument.

```R
install.packages("https://github.com/acorg/acmacs.r/releases/download/v3.0/acmacs.r_3.0.macOS-10.14.tgz", repos=NULL, lib="/tmp/R/library")
```

Linux
-----

```R
install.packages("https://github.com/acorg/acmacs.r/releases/download/v3.0/acmacs.r_3.0_R_x86_64-pc-linux-gnu.tar.gz", repos=NULL, lib="/tmp/R/library")
```

Usage example
=============

Complete example with the most of available functions can be accessed within R using ?acmacs.Example

```R
library(acmacs.r)
chart <- new(acmacs.Chart, "/tmp/file.ace")
cat(chart$info)

ags <- chart$antigens
format_antigen <- function(a) {
  paste(c(a$name, a$passage, a$reassortant, a$reference, a$annotations, paste("[", a$date, "]", sep="", collapse=""), a$lab_ids), collapse=" ", sep="")
}
cat("antigens:", length(ags), "\n ", paste(sapply(ags, format_antigen), collapse="\n  "), "\n")
```

Docs
=======

[Rcpp](https://cran.r-project.org/web/packages/Rcpp/index.html)
