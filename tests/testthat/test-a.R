# cat(getwd(), "\n")

chart <- new(acmacs.Chart, "test-2004-3.ace")
cat("number_of_antigens:", chart$number_of_antigens, "\n")
cat("number_of_sera:", chart$number_of_sera, "\n")
test_that("number of antigens != number of sera", { expect_equal(chart$number_of_antigens, 22) })
test_that("number of antigens != number of sera", { expect_equal(chart$number_of_sera, 6) })
# test_that("number of antigens != number of sera", { expect_equal(chart$number_of_antigens, chart$number_of_sera) })
