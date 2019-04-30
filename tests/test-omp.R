library(acmacs.r)

rs <- function() {
    rs_ <- rowSums(data.frame(a=c(3,3,3),b=c(4,4,4)))
    cat("\nrowSums executed: ")
    cat(rs_)
    cat("\n\n")
}

rs()
chart <- new(acmacs.Chart, "testthat/cdc-h1pdm-20090612.ace")
cat("chart loaded\n")
cat(chart$info)
cat("\n\nabout to call chart$relax_many()\n\n")
chart$relax_many("none", 2, 10, TRUE)
cat(chart$info)
rs()
