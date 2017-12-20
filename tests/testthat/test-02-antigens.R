context("Test chart$antigens")

ad_antigen_list <- system("ad chart-names test-2004-3.ace | grep '^AG'", intern=TRUE)
#write(antigen_list, stderr())

chart <- new(acmacs.Chart, "test-2004-3.ace")

ags <- chart$antigens
r_antigen_list <- sapply(1:chart$number_of_antigens, function(no) { paste("AG", format(no - 1, justify="right", width=2), ags[[no]]$name, collapse=" ") })

## r_passage_list <- sapply(1:chart$number_of_antigens, function(no) { paste("AG", format(no - 1, justify="right", width=2), ags[[no]]$passage, collapse=" ") })
## write("\n", stderr())
## write(r_passage_list, stderr())

# ======================================================================

test_that("number of antigens", { expect_equal(chart$number_of_antigens, 22) })
test_that("antigen list", { expect_equal(ad_antigen_list, r_antigen_list) })
