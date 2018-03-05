loadModule("acmacs", TRUE)

.onAttach <- function (libname, pkgname) {
    acmacs.init_cout_cerr()
}
