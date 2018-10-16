context("Test chart$projections")

test_chart <- function(filename, expected_num_projections, expected_stress, expected_minimum_column_basis) {
    chart <- new(acmacs.Chart, filename)

    layout <- chart$projections[[1]]$layout
    test_that("number of points", { expect_equal(chart$number_of_points, nrow(layout)) })
    test_that("number of dimensions", { expect_equal(chart$projections[[1]]$number_of_dimensions, ncol(layout)) })

    test_that("number of projections", { expect_equal(chart$number_of_projections, expected_num_projections) })
    test_that("stress", { expect_equal(chart$projections[[1]]$stress, expected_stress) })
    test_that("minimum_column_basis", { expect_equal(chart$projections[[1]]$minimum_column_basis, expected_minimum_column_basis) })
    test_that("layout vs. transformed_layout vs. transformation", { expect_equal(chart$projections[[1]]$layout %*% chart$projections[[1]]$transformation, chart$projections[[1]]$transformed_layout) })

    pr_new <- chart$new_projection(chart$projections[[1]]$minimum_column_basis, chart$projections[[1]]$number_of_dimensions);
    test_that("new projection stress", { expect_lt(chart$projections[[1]]$stress, pr_new$stress) })
    pr_new$layout <- layout
    test_that("new projection stress", { expect_equal(chart$projections[[1]]$stress, pr_new$stress) })
    test_that("layouts equal", { expect_equal(chart$projections[[1]]$layout, pr_new$layout) })
    pr_new$layout <- matrix(c(0), nrow=chart$number_of_points, ncol=chart$projections[[1]]$number_of_dimensions)
    test_that("new projection stress", { expect_lt(chart$projections[[1]]$stress, pr_new$stress) })

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

    chart$projections[[1]]$flip_north_south()
    chart$projections[[1]]$flip_east_west()
    chart$projections[[1]]$rotate_degrees(180)
    tr5 <- chart$projections[[1]]$transformation
    test_that("after flip_ns, flip_ew, rot180", { expect_equal(tr1, tr5) })

    point_no <- chart$number_of_points
    move_to <- c(1967.0, -2017.0)
    chart$projections[[1]]$move_point(point_no, move_to)
    test_that("last point moved", { expect_equal(chart$projections[[1]]$layout[point_no,], move_to) })

    # remove all projections (must be the last test)
    chart$remove_all_projections()
    test_that("number of projections upon removal all ones", { expect_equal(chart$number_of_projections, 0) })
}

test_chart("2004-3.ace", 1, 71.790977, "none")
test_chart("cdc-h1pdm-2009.acd1.bz2", 1, 738.02039, "1280")

# ======================================================================
