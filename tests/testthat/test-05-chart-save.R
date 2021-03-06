context("Test chart save")

output_filename_ace <- "/tmp/acmacs.r.test05.ace"
output_filename_save <- "/tmp/acmacs.r.test05.save.xz"

new_name <- "rotated"
chart1 <- new(acmacs.Chart, "2004-3.ace")
chart1$name <- new_name
test_that("name updated", { expect_equal(substr(chart1$name, 0, nchar(new_name)), new_name) })
chart1$projections[[1]]$rotate_degrees(30)
chart1$save(output_filename_save)
chart1$save(output_filename_ace)

chart2 <- new(acmacs.Chart, output_filename_save)
test_that("reloaded chart (save) has new name", { expect_equal(substr(chart2$name, 0, nchar(new_name)), new_name) })
test_that("reloaded chart (save) transformation", { expect_equal(chart1$projections[[1]]$transformation, chart2$projections[[1]]$transformation) })

chart3 <- new(acmacs.Chart, output_filename_ace)
test_that("reloaded chart (ace) has new name", { expect_equal(substr(chart3$name, 0, nchar(new_name)), new_name) })
test_that("reloaded chart (ace) transformation", { expect_equal(chart1$projections[[1]]$transformation, chart3$projections[[1]]$transformation) })

unlink(c(output_filename_ace, output_filename_save))
