context("Test chart$projections relax")

test_chart <- function(filename, expected_stress) {
    chart <- new(acmacs.Chart, filename)
    projection <- chart$projections[[1]]
    print(paste("stress", projection$stress))
    projection$relax("cg", FALSE)
    print(paste("stress", projection$stress))
}

test_chart("2004-3.ace", 71.790977)
test_chart("cdc-h1pdm-2009.acd1.bz2", 738.02039)

# ======================================================================
