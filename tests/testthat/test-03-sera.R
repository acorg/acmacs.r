context("Test chart$sera")

test_chart <- function(filename, expected_num_sera) {
    ad_serum_list <- system(paste("chart-names -f '{ag_sr} {no0} {name_full_passage} {serum_species}'", filename, " | grep '^SR'"), intern=TRUE)
                                        #write(antigen_list, stderr())

    chart <- new(acmacs.Chart, filename)

    srs <- chart$sera
    r_serum_list <- sapply(1:chart$number_of_sera, function(no) {
        paste("SR", format(no - 1, justify="right", width=if (chart$number_of_antigens <100) 2 else 3), srs[[no]]$full_name, collapse=" ")
    })

    test_that("number of sera", { expect_equal(chart$number_of_sera, expected_num_sera) })
    test_that("serum list", { expect_equal(ad_serum_list, r_serum_list) })

}

test_chart("2004-3.ace", 6)
test_chart("cdc-h1pdm-2009.acd1.bz2", 31)

# ======================================================================
