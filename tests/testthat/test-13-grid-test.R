context("Grid test")

test_chart <- function(filename, expected_rows) {
    c1 <- new(acmacs.Chart, filename)
    g1 <- new(acmacs.GridTest, c1)
    r1 <- g1$test()
    test_that(paste("number of hemi and trapped points in", filename), { expect_equal(nrow(r1), expected_rows) })
    print(r1)
}

test_chart("2004-3.ace", 0)
test_chart("cdc-h1pdm-2009.acd1.bz2", 3)
test_chart("cdc-h1pdm-20090612.ace", 1)
