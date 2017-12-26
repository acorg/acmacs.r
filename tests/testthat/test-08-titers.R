context("Test chart$titers")

chart <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")
titers <- chart$titers
all <- titers$all()

test_that("titer", {
    expect_equal(titers$titer(1, 1), "320");
    expect_equal(titers$titer(1, 2), "<10");
    expect_equal(titers$titer(2, 1), "<10");
    expect_equal(titers$titer(2, 2), "1280");
    expect_equal(titers$titer(2, 3), "2560");
})

test_that("titer", {
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
