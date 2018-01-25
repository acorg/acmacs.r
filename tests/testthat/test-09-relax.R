context("Test chart$projections relax")

test_relax_existing <- function(filename) {
    chart <- new(acmacs.Chart, filename)
    projection <- chart$projections[[1]]
    stress1 = projection$stress
    projection$relax()
    stress2 = projection$stress
    test_that("stress after relax", { expect_equal(stress1, stress2) })
}

test_relax <- function(filename, num_optimizations, expected_stress) {
    chart <- new(acmacs.Chart, filename)
    print(paste("initial projections:", chart$number_of_projections, "best:", chart$projections[[1]]$stress))
    chart$relax_many("1280", 2, num_optimizations, FALSE)
    ## for (p_no in 1:chart$number_of_projections) {
    ##     write(paste(p_no, chart$projections[[p_no]]$stress), file="")
    ## }
    # test_that("stress after N optimizations", { expect_equal(chart$projections[[1]]$stress, expected_stress) })
    test_that("stress after N optimizations", { expect_true(chart$projections[[2]]$stress < expected_stress) })
}

test_relax_existing("2004-3.ace")
test_relax_existing("cdc-h1pdm-2009.acd1.bz2")
test_relax("2004-3.ace", 20, 72)
test_relax("cdc-h1pdm-2009.acd1.bz2", 5, 900)

# ======================================================================
