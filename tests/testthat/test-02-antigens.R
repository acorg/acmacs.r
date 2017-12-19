context("Test chart$antigens")

ad_antigen_list <- system("ad chart-names test-2004-3.ace | grep '^AG'", intern=TRUE)
#write(antigen_list, stderr())

chart <- new(acmacs.Chart, "test-2004-3.ace")
test_that("number of antigens", { expect_equal(chart$number_of_antigens, 22) })

ags <- chart$antigens
# write(paste(sapply(ags, function(a) { paste("AG", a$name, collapse=" ") }), collapse="\n"), stderr())
r_antigen_list <- paste(sapply(1:chart$number_of_antigens, function(no) { paste("AG", format(no - 1, justify="right", width=2), ags[[no]]$name, collapse=" ") }), collapse="\n")
write("\n", stderr())
write(length(r_antigen_list), stderr())

test_that("antigen list", { expect_equal(ad_antigen_list, r_antigen_list) })
