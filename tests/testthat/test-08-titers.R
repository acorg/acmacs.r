context("Test chart$titers")

chart <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")
titers <- chart$titers

test_that("titer", {
    expect_equal(titers$titer(1, 1), "320");
    expect_equal(titers$titer(1, 2), "<10");
    expect_equal(titers$titer(2, 1), "<10");
    expect_equal(titers$titer(2, 2), "1280");
    expect_equal(titers$titer(2, 3), "2560");
})
