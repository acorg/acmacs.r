context("Test chart$projections")

test_chart <- function(filename, expected_num_projections, expected_stress) {
    chart <- new(acmacs.Chart, filename)

    test_that("number of projections", { expect_equal(chart$number_of_projections, expected_num_projections) })
    test_that("stress", { expect_equal(chart$projections[[1]]$stress, expected_stress) })
    test_that("minimum_column_basis", { expect_equal(chart$projections[[1]]$minimum_column_basis, "none") })

}

test_chart("2004-3.ace", 1, 71.790977)
test_chart("cdc-h1pdm-2009.acd1.bz2", 1, 738.02039)

# ======================================================================
