context("Test chart merge")

chart1 <- new(acmacs.Chart, "2004-01.table.ace")
chart2 <- new(acmacs.Chart, "2004-02.table.ace")
expected1 <- new(acmacs.Chart, "2004-01-02.incremental.ace");
merge1 <- acmacs.merge(chart1, chart2);
test_that("number of antigens", { expect_equal(merge1$number_of_antigens, expected1$number_of_antigens) })
test_that("number of sera", { expect_equal(merge1$number_of_sera, expected1$number_of_sera) })
test_that("number of layers", { expect_equal(merge1$titers$number_of_layers, expected1$titers$number_of_layers) })
test_that("column bases", { expect_equal(merge1$column_bases(), expected1$column_bases()) })

# merge1 <- acmacs.merge_incremental(chart1, chart2);
