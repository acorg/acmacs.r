context("Test chart$plot_spec")

chart <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")
plot_spec <- chart$plot_spec

test_that("size", { expect_equal(plot_spec$styles[[1]]$size, 8); expect_equal(plot_spec$styles[[22]]$size, 5) })
test_that("fill", { expect_equal(plot_spec$styles[[1]]$fill, "transparent"); expect_equal(plot_spec$styles[[22]]$fill, "green") })
test_that("outline", { expect_equal(plot_spec$styles[[1]]$outline, "black"); expect_equal(plot_spec$styles[[22]]$outline, "black") })
test_that("outline_width", { expect_equal(plot_spec$styles[[1]]$outline_width, 1); expect_equal(plot_spec$styles[[22]]$outline_width, 1) })
