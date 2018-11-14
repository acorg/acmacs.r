context("Test transformation")

chart1 <- new(acmacs.Chart, "2004-3.ace")
prj1 <- chart1$projections[[1]]
mat1 <- matrix(c(1, 2, 3, 4), nrow=2, byrow=TRUE)
prj1$transformation <- mat1
# print("TR1\n")
# print(prj1$transformation)
test_that("new 2D transformation", { expect_equal(prj1$transformation, mat1) })
test_that("layout 2D vs. transformed_layout vs. transformation", { expect_equal(prj1$layout %*% mat1, prj1$transformed_layout) })

chart2 <- new(acmacs.Chart, "2004-3.3d.ace")
prj2 <- chart2$projections[[1]]
# print(prj2$transformed_layout)
mat2 <- matrix(c(1, 2, 3, 4, 5, 6, 7, 8, 9), nrow=3, byrow=TRUE)
prj2$transformation <- mat2
# print("TR2\n")
# print(prj2$transformation)
test_that("new 3D transformation", { expect_equal(prj2$transformation, mat2) })
test_that("layout 3D vs. transformed_layout vs. transformation", { expect_equal(prj2$layout %*% mat2, prj2$transformed_layout) })
# print(prj2$transformed_layout)
