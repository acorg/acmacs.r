acmacs.r package
================

This package provides R 4.0 with ability to read, modify, save the data stored in acmacs and
lispmds files.

Supported platforms
===================

 - macOS 10.14.6

 - Linux

Dependencies
=============

Rcpp 1.0.3 or later is required.

```R
install.packages("Rcpp")
```

Installation
============

https://cran.r-project.org/web/packages/Rcpp/index.html
R 4.0 is required, other versions are not supported!

Package is supposed to be installed from a binary archive for the
specific platform. Building package from sources is complicated and it
cannot be done within R.

Installation on macOS 10.14
---------------------

```R
install.packages(c("Rcpp", "devtools", "testthat", "roxygen2"))
install.packages(Find(function(asset) { grepl("macOS", asset$name, fixed=TRUE) }, devtools:::github_GET("repos/acorg/acmacs.r/releases/latest")$assets)$browser_download_url)
```


Installation on Linux
-----

```R
install.packages(c("Rcpp", "devtools", "testthat", "roxygen2"))
install.packages(Find(function(asset) { grepl("linux", asset$name, fixed=TRUE) }, devtools:::github_GET("repos/acorg/acmacs.r/releases/latest")$assets)$browser_download_url)

```

Loading
---------------------

```R
library(acmacs.r)
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
