context("Grid test")

test_chart <- function(filename, expected_rows, verbose=FALSE) {
    c1 <- new(acmacs.Chart, filename)
    g1 <- new(acmacs.GridTest, c1)
    r1 <- g1$test()

    if (verbose) {
        cat("\n\ngrid test\n", sep="")
        print(r1)
        cat("\n", sep="")
    }

    test_that(paste("number of hemi and trapped points in", filename), { expect_equal(nrow(r1), expected_rows) })
    if (nrow(r1) > 0) {
        diags <- unique(r1["diagnosis"])
        # print(r1)
        test_that(paste("only hemi points in", filename), { expect_true(diags == list("hemisphering")) })
    }
}

test_chart_with_step <- function(filename, step, expected_rows, verbose=FALSE) {
    c1 <- new(acmacs.Chart, filename)
    g1 <- new(acmacs.GridTest, c1, step)
    r1 <- g1$test()

    if (verbose) {
        cat(paste("\n\ngrid test with step: ", step), "\n", sep="")
        print(r1)
        cat("\n", sep="")
    }

    test_that(paste("number of hemi and trapped points in", filename), { expect_equal(nrow(r1), expected_rows) })
    if (nrow(r1) > 0) {
        diags <- unique(r1["diagnosis"])
        # print(r1)
        test_that(paste("only hemi points in", filename), { expect_true(diags == list("hemisphering")) })
    }
}

test_chart("2004-3.ace", 0)
test_chart_with_step("2004-3.ace", 2.25, 0)
test_chart("cdc-h1pdm-2009.acd1.bz2", 3, verbose=TRUE)
test_chart_with_step("cdc-h1pdm-2009.acd1.bz2", 2.0, 1, verbose=TRUE)
test_chart("cdc-h1pdm-20090612.ace", 1)
