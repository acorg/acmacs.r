context("test-08-titers.R: test chart$titers")

chart <- new(acmacs.Chart, "bv.acd1.xz")
titers <- chart$titers

chart <- new(acmacs.Chart, "cdc-h1pdm-2009.acd1.bz2")
titers <- chart$titers

test_that("titer of layer", {
    expect_equal(titers$titer_of_layer(1, 1, 1), "2560");
    expect_equal(titers$titer_of_layer(2, 1, 1), "1280");
    expect_equal(titers$titer_of_layer(4, 1, 1), "*");
})

chart <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")
titers <- chart$titers
all <- titers$all()

test_that("titer get", {
    expect_equal(titers$titer(1, 1), "320");
    expect_equal(titers$titer(1, 2), "<10");
    expect_equal(titers$titer(2, 1), "<10");
    expect_equal(titers$titer(2, 2), "1280");
    expect_equal(titers$titer(2, 3), "2560");
})

test_that("titer from all", {
    expect_equal(all[1, 1], "320");
    expect_equal(all[1, 2], "<10");
    expect_equal(all[2, 1], "<10");
    expect_equal(all[2, 2], "1280");
    expect_equal(all[2, 3], "2560");
})

test_that("titer==all[ag,sr]", {
    for (ag in 1:chart$number_of_antigens) {
        for (sr in 1:chart$number_of_sera) {
            expect_equal(titers$titer(ag, sr), all[ag, sr])
        }
    }
})

test_that("titer-set", {
    titers$set_titer(1, 1, "640");
    expect_equal(titers$titer(1, 1), "640");
    expect_that(titers$set_titer(1, 1, "-40"), throws_error())
    expect_that(titers$set_titer(1, 1, "?"), throws_error())
    expect_that(titers$set_titer(1, 1, "*40"), throws_error())
    expect_that(titers$set_titer(1, 1, "40.51"), throws_error())
    expect_that(titers$set_titer(1, 1, "-40.51"), throws_error())
    expect_equal(titers$titer(1, 1), "640");
})
