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
test_that("column bases", { expect_equal(chart2$column_bases(), c(7.33360262826828, 9, 9, 8, 9, 9, 5.25171909260596, 6, 5, 8, 9, 9, 9, 9, 11, 8, 10, 10, 9, 9, 7, 9, 9, 8, 10, 11, 11, 10, 10, 8, 9)) })
