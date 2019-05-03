loadModule("acmacs", TRUE)

.onAttach <- function (libname, pkgname) {
    acmacs.__init()
}
