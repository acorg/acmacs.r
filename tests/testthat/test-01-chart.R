context("Test chart")

chart1 <- new(acmacs.Chart, "2004-3.ace")
test_that("number of antigens", { expect_equal(chart1$number_of_antigens, 22) })
test_that("number of sera", { expect_equal(chart1$number_of_sera, 6) })
test_that("number of points", { expect_equal(chart1$number_of_points, chart1$number_of_antigens + chart1$number_of_sera) })
test_that("number of projections", { expect_equal(chart1$number_of_projections, 1) })

chart2 <- new(acmacs.Chart, "cdc-h1pdm-2009.acd1.bz2")
test_that("number of antigens", { expect_equal(chart2$number_of_antigens, 303) })
test_that("number of sera", { expect_equal(chart2$number_of_sera, 31) })
test_that("number of points", { expect_equal(chart2$number_of_points, chart2$number_of_antigens + chart2$number_of_sera) })
test_that("number of projections", { expect_equal(chart2$number_of_projections, 1) })
