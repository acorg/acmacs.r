context("test-04-projections.R: Test chart$projections")

test_chart <- function(filename, expected_num_projections, expected_stress, expected_minimum_column_basis) {
    # print(filename)
    chart <- new(acmacs.Chart, filename)
    prj1 <- chart$projections[[1]]
    layout <- prj1$layout

    test_that("number of points", { expect_equal(chart$number_of_points, nrow(layout)) })
    test_that("number of dimensions", { expect_equal(prj1$number_of_dimensions, ncol(layout)) })

    test_that("number of projections", { expect_equal(chart$number_of_projections, expected_num_projections) })
    test_that("stress", { expect_equal(prj1$stress, expected_stress) })
    test_that("minimum_column_basis", { expect_equal(prj1$minimum_column_basis, expected_minimum_column_basis) })
                                        # print("\n"); print(prj1$transformation)
    test_that("layout vs. transformed_layout vs. transformation", { expect_equal(prj1$layout %*% prj1$transformation, prj1$transformed_layout) })

    pr_new <- chart$new_projection(prj1$minimum_column_basis, prj1$number_of_dimensions);
    test_that("new projection stress", { expect_lt(prj1$stress, pr_new$stress) })
    pr_new$layout <- layout
    test_that("new projection stress", { expect_equal(prj1$stress, pr_new$stress) })
    test_that("layouts equal", { expect_equal(prj1$layout, pr_new$layout) })
    pr_new$layout <- matrix(c(0), nrow=chart$number_of_points, ncol=prj1$number_of_dimensions)
    test_that("new projection stress", { expect_lt(prj1$stress, pr_new$stress) })

    tr1 <- prj1$transformation
    prj1$rotate_degrees(30)
    tr2 <- prj1$transformation
                                        # cat("\n\ntr1: ", tr1, "\ntr2: ", tr2, "\n")
    test_that("after rotation (degrees)", { expect_false(isTRUE(all.equal(tr1, tr2))); })
    prj1$rotate_degrees(-30)
    tr3 <- prj1$transformation
    test_that("after rotation back (degrees)", { expect_false(isTRUE(all.equal(tr2, tr3))); expect_equal(tr1, tr3) })
    prj1$rotate_degrees(30)
    prj1$rotate_radians(-30 * pi /180)
    tr4 <- prj1$transformation
    test_that("after rotation back (radians)", { expect_equal(tr1, tr4) })

    prj1$flip_north_south()
    prj1$flip_east_west()
    prj1$rotate_degrees(180)
    tr5 <- prj1$transformation
    test_that("after flip_ns, flip_ew, rot180", { expect_equal(tr1, tr5) })

    new_tr_2d <- matrix(c(0.5, 0.3, 0.7, 0.5), nrow=2, byrow=TRUE)

    point_no <- chart$number_of_points
    move_to <- c(1967.0, -2017.0)
    prj1$move_point(point_no, move_to)
    test_that("last point moved", { expect_equal(prj1$layout[point_no,], move_to) })

    chart$projection(1)$set_transformation(new_tr_2d)
    chart$projections[[1]]$set_transformation(new_tr_2d)
    test_that("set transformation for projection 1", { expect_equal(chart$projection(1)$transformation, new_tr_2d) })

                                        # cat("\n forced_column_bases: ", chart$projection(1)$forced_column_bases, " ", typeof(chart$projection(1)$forced_column_bases), "\n")
    test_that("original forced column bases", { expect_equal(chart$projection(1)$forced_column_bases, as.double(c(NA))) })
    orig_col_bases <- chart$column_bases();
    new_col_bases <- sapply(orig_col_bases, function(val) (val + 1));
    chart$projection(1)$set_column_bases(new_col_bases);
    test_that("new forced column bases", { expect_equal(chart$projection(1)$forced_column_bases, new_col_bases) })
                                        # cat("\n orig_col_bases ", orig_col_bases, " new:", new_col_basese, "\n");
    new_col_bases[[2]] = 11.11
    chart$projection(1)$set_column_basis(2, new_col_bases[[2]])
    test_that("new and updated forced column bases", { expect_equal(chart$projection(1)$forced_column_bases, new_col_bases) })

    pr_new_2d <- chart$new_projection("none", 2);
    tr_new_2d <- pr_new_2d$transformation
                                        # cat("\ntr_new_2d: ", nrow(tr_new_2d), " ", ncol(tr_new_2d), "\n")
    test_that("transformation for the new 2D projection", { expect_equal(nrow(tr_new_2d), 2); expect_equal(ncol(tr_new_2d), 2); expect_equal(tr_new_2d, matrix(ncol=2, c(1,0,0,1))) })
    pr_new_2d$set_transformation(new_tr_2d)
    test_that("set transformation for the 2D projection", { expect_equal(pr_new_2d$transformation, new_tr_2d) })

    pr_new_3d <- chart$new_projection("none", 3);
    tr_new_3d <- pr_new_3d$transformation
                                        # cat("\ntr_new_3d: ", nrow(tr_new_3d), " ", ncol(tr_new_3d), "\n")
    test_that("transformation for the new 3D projection", { expect_equal(nrow(tr_new_3d), 3); expect_equal(ncol(tr_new_3d), 3) })
    test_that("transformation for the new 3D projection", { expect_equal(tr_new_3d, matrix(ncol=3, c(1,0,0,0,1,0,0,0,1))) })

    # remove all projections (must be the last test)
    chart$remove_all_projections()
    test_that("number of projections upon removal all ones", { expect_equal(chart$number_of_projections, 0) })
}

test_clone_projection <- function(filename) {
    chart <- new(acmacs.Chart, filename)

    p1 <- chart$clone_projection(1)
    test_that("stress after clone 1", { expect_equal(chart$projections[[1]]$stress, p1$stress) })
    p2 <- chart$clone_projection(2)
    test_that("stress after clone 2", { expect_equal(chart$projections[[2]]$stress, p2$stress) })
    # print(sapply(1:chart$number_of_projections, function(no) { chart$projections[[no]]$info }))
}

test_chart("2004-3.ace", 1, 71.790977, "none")
test_chart("cdc-h1pdm-2009.acd1.bz2", 1, 738.02039, "1280")
test_clone_projection("cdc-h1pdm-20090612.ace")

# ======================================================================
