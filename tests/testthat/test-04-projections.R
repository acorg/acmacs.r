context("Test chart$projections")

test_chart <- function(filename, expected_num_projections, expected_stress) {
    chart <- new(acmacs.Chart, filename)

    layout <- chart$projections[[1]]$layout

    test_that("number of projections", { expect_equal(chart$number_of_projections, expected_num_projections) })
    test_that("stress", { expect_equal(chart$projections[[1]]$stress, expected_stress) })
    test_that("minimum_column_basis", { expect_equal(chart$projections[[1]]$minimum_column_basis, "none") })
    test_that("layout vs. transformed_layout vs. transformation", { expect_equal(chart$projections[[1]]$layout %*% chart$projections[[1]]$transformation, chart$projections[[1]]$transformed_layout) })

    tr1 <- chart$projections[[1]]$transformation
    chart$projections[[1]]$rotate_degrees(30)
    tr2 <- chart$projections[[1]]$transformation
    test_that("after rotation (degrees)", { expect_error(expect_equal(tr1, tr2), "4/4 mismatches") })
    chart$projections[[1]]$rotate_degrees(-30)
    tr3 <- chart$projections[[1]]$transformation
    test_that("after rotation back (degrees)", { expect_error(expect_equal(tr2, tr3), "4/4 mismatches"); expect_equal(tr1, tr3) })
    chart$projections[[1]]$rotate_degrees(30)
    chart$projections[[1]]$rotate_radians(-30 * pi /180)
    tr4 <- chart$projections[[1]]$transformation
    test_that("after rotation back (radians)", { expect_equal(tr1, tr4) })
}

test_chart("2004-3.ace", 1, 71.790977)
test_chart("cdc-h1pdm-2009.acd1.bz2", 1, 738.02039)

# ======================================================================
