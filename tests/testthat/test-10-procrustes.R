context("Test procrustes")

chart <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")

pc12 <- acmacs.procrustes(chart$projections[[1]], chart$projections[[2]], FALSE, "auto")
test_that("rms 1 vs 2", { expect_equal(pc12$rms, 2.2550068885169016752456627727952) })
test_that("transformation 1 vs 2", { expect_equal(pc12$transformation, matrix(c(-0.6101506307143977902995857220958,-0.79228543331227696544516447829665,1,0.79228543331227696544516447829665,-0.61015063071439790132188818461145,1,1.5012699551969956868191502508125,-0.44411424252385989230873519773013,1), nrow=3, byrow=TRUE)) })

pc23 <- acmacs.procrustes(chart$projections[[2]], chart$projections[[3]], FALSE, "auto")
test_that("rms 2 vs 3", { expect_equal(pc23$rms, 2.6565462537356832051216315448983) })
test_that("transformation 2 vs 3", { expect_equal(pc23$transformation, matrix(c(-0.75148433030009376132341003540205,0.65975093884997171311823649375583,1,0.65975093884997160209593403124018,0.75148433030009376132341003540205,1,0.9693630110835171898031603632262,-0.98569537201010137827950075006811,1), nrow=3, byrow=TRUE)) })

# ======================================================================
