context("Test chart$plot_spec")

chart <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")
plot_spec <- chart$plot_spec

test_that("shown", { expect_equal(plot_spec$styles[[1]]$shown, TRUE); expect_equal(plot_spec$styles[[22]]$shown, TRUE) })
test_that("size", {
    expect_equal(plot_spec$styles[[1]]$size, 8);
    expect_equal(plot_spec$styles[[22]]$size, 5);
    expect_equal(plot_spec$styles[[55]]$size, 6.5);
})
test_that("fill", { expect_equal(plot_spec$styles[[1]]$fill, "transparent"); expect_equal(plot_spec$styles[[22]]$fill, "green") })
test_that("outline", { expect_equal(plot_spec$styles[[1]]$outline, "black"); expect_equal(plot_spec$styles[[22]]$outline, "black") })
test_that("outline_width", { expect_equal(plot_spec$styles[[1]]$outline_width, 1); expect_equal(plot_spec$styles[[22]]$outline_width, 1) })
test_that("rotation", {
    expect_equal(plot_spec$styles[[1]]$rotation, 0);
    expect_equal(plot_spec$styles[[10]]$rotation, 0.5);
    expect_equal(plot_spec$styles[[22]]$rotation, 0);
})
test_that("aspect", {
    expect_equal(plot_spec$styles[[1]]$aspect, 0.75);
    expect_equal(plot_spec$styles[[3]]$aspect, 1);
    expect_equal(plot_spec$styles[[10]]$aspect, 0.75);
    expect_equal(plot_spec$styles[[14]]$aspect, 0.75);
    expect_equal(plot_spec$styles[[15]]$aspect, 1);
    expect_equal(plot_spec$styles[[22]]$aspect, 1);
})
test_that("shape", {
    expect_equal(plot_spec$styles[[1]]$shape, "CIRCLE");
    expect_equal(plot_spec$styles[[55]]$shape, "BOX");
})
