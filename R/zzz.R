loadModule("acmacs", TRUE)
loadModule("acmacs_grid_test", TRUE)

.onAttach <- function (libname, pkgname) {
    acmacs.init_cout_cerr()
}
