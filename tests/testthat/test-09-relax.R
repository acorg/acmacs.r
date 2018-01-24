context("Test chart$projections relax")

test_relax_existing <- function(filename) {
    chart <- new(acmacs.Chart, filename)
    projection <- chart$projections[[1]]
    stress1 = projection$stress
    projection$relax()
    stress2 = projection$stress
    test_that("stress after relax", { expect_equal(stress1, stress2) })
}

test_relax <- function(filename, num_optimizations) {
}

test_relax_existing("2004-3.ace")
test_relax_existing("cdc-h1pdm-2009.acd1.bz2")
test_relax("2004-3.ace", 20)

# ======================================================================
