context("Test chart$projections relax")

test_relax_existing <- function(filename) {
    chart <- new(acmacs.Chart, filename)
    projection <- chart$projections[[1]]
    stress1 = projection$stress
    projection$relax()
    stress2 = projection$stress
    test_that("stress after relax", { expect_equal(stress1, stress2) })
}

test_relax <- function(filename, num_optimizations, expected_stress) {
    chart <- new(acmacs.Chart, filename)
    print(paste("initial projections:", chart$number_of_projections, "best:", chart$projections[[1]]$stress))
    for (iter in 1:num_optimizations) {
        p <- chart$relax("1280", 2)
        print(p$stress)
    }
    print("sorting")
    chart$sort_projections()
    print(chart$number_of_projections)
    print(chart$projections[[1]]$stress)
    test_that("stress after N optimizations", { expect_equal(chart$projections[[1]]$stress, expected_stress) })
}

test_relax_existing("2004-3.ace")
test_relax_existing("cdc-h1pdm-2009.acd1.bz2")
test_relax("2004-3.ace", 20, 71.7909773681828)
test_relax("cdc-h1pdm-2009.acd1.bz2", 50, 738.020389741808)

# ======================================================================
