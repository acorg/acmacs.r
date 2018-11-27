context("Test procrustes")

chart1 <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")

pc12 <- acmacs.procrustes(chart1$projections[[1]], chart1$projections[[2]], FALSE, "auto")
test_that("rms 1 vs 2", { expect_equal(pc12$rms, 2.19270062023382195093290647491813) })
test_that("transformation 1 vs 2", { expect_equal(pc12$transformation, matrix(c(-0.620903999610312951062951469793916,-0.783886613782832530894495448592352,1,0.783886613782832641916797911108006,-0.620903999610313284129858857340878,1,1.5005094548823645350665856312844,-0.431584164567283545821396728570107,1), nrow=3, byrow=TRUE)) })

pc23 <- acmacs.procrustes(chart1$projections[[2]], chart1$projections[[3]], FALSE, "auto")
test_that("rms 2 vs 3", { expect_equal(pc23$rms, 2.58129384332588074357772711664438) })
test_that("transformation 2 vs 3", { expect_equal(pc23$transformation, matrix(c(-0.765071115988800287688320622692117,0.643945795451489888527873972634552,1,0.643945795451489888527873972634552,0.765071115988800287688320622692117,1,0.986496633717912629890633979812264,-0.977312730986796607268729530915152,1), nrow=3, byrow=TRUE)) })

# ======================================================================

chart2 <- new(acmacs.Chart, "2004-01.ace")
chart3 <- new(acmacs.Chart, "2004-02.ace")
pc4 <- acmacs.procrustes(chart2$projections[[1]], chart3$projections[[1]])
test_that("rms 2004-01.ace vs. 2004-02.ace", { expect_equal(pc4$rms, 1.86222026762498882312968362384709) })
pc5 <- acmacs.procrustes(chart2$projections[[1]], chart3$projections[[1]], subset="antigens")
test_that("rms 2004-01.ace vs. 2004-02.ace (antigens only)", { expect_equal(pc5$rms, 1.12880200217834958387186361505883) })
pc6 <- acmacs.procrustes(chart2$projections[[1]], chart3$projections[[1]], subset="sera")
test_that("rms 2004-01.ace vs. 2004-02.ace (sera only)", { expect_equal(pc6$rms, 2.3673757463275220125353826006176) })

# ======================================================================
