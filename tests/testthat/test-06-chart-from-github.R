context("Test reading chart from remote file")

raw_data <- readBin(file("https://github.com/acorg/acmacs.r/blob/master/tests/testthat/2004-3.ace?raw=true", open="rb"), "raw", n=999999)
chart1 <- new(acmacs.Chart, raw_data)

chart2 <- new(acmacs.Chart, "2004-3.ace")

test_that("number of antigens", { expect_equal(chart1$number_of_antigens, chart2$number_of_antigens) })
test_that("layout", { expect_equal(chart1$projections[[1]]$layout, chart2$projections[[1]]$layout) })
