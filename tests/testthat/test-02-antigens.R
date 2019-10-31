context("Test chart$antigens")

test_chart <- function(filename, expected_num_antigens, test_passages) {
    ad_antigen_list <- trimws(system(paste("chart-names -f '{ag_sr} {no0} {full_name_with_passage} [{date}] {lab_ids} {ref}'", filename, " | grep '^AG'"), intern=TRUE))
                                        #write(antigen_list, stderr())

    chart <- new(acmacs.Chart, filename)

    ags <- chart$antigens
    r_antigen_list <- sapply(1:chart$number_of_antigens, function(no) {
        trimws(paste("AG", format(no - 1, justify="right", width= if (expected_num_antigens <100) 2 else 3), ags[[no]]$full_name, paste0("[", ags[[no]]$date, "]"), ags[[no]]$lab_ids, collapse=" "))
    })

    test_that("number of antigens", { expect_equal(chart$number_of_antigens, expected_num_antigens) })
    test_that("antigen list", { expect_equal(ad_antigen_list, r_antigen_list) })

    ## if (test_passages) {
    ##     r_passage_list <- sapply(1:chart$number_of_antigens, function(no) { paste("AG", format(no - 1, justify="right", width=2), ags[[no]]$passage, ags[[no]]$passage$type(), ags[[no]]$passage$without_date(), collapse=" ") })
    ##     write("\n", stderr())
    ##     write(r_passage_list, stderr())
    ## }
}

test_chart("2004-3.ace", 22, FALSE)
test_chart("cdc-h1pdm-2009.acd1.bz2", 303, TRUE)

# ======================================================================
