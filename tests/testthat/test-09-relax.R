context("test-09-relax.R: Test chart$projections relax")

test_relax_existing <- function(filename) {
    chart <- new(acmacs.Chart, filename)
    projection <- chart$projections[[1]]
    stress1 = projection$stress
    projection$relax()
    stress2 = projection$stress
    test_that("stress after relax", { expect_equal(stress1, stress2) })
}

test_relax_many <- function(filename, num_optimizations, expected_stress, dimension_annealing="no-dimension-annealing") {
    chart <- new(acmacs.Chart, filename)
    # chart$relax_many("1280", 2, num_optimizations, FALSE)
    chart$relax_many(2, num_optimizations, "1280", dimension_annealing)
    ## for (p_no in 1:chart$number_of_projections) {
    ##     write(paste(p_no, chart$projections[[p_no]]$stress), file="")
    ## }
    # test_that("stress after N optimizations", { expect_equal(chart$projections[[1]]$stress, expected_stress) })
    test_that(paste("stress after ", num_optimizations, " parallel optimizations: ", chart$projections[[2]]$stress, "  expected: ", expected_stress, sep=""), { expect_true(chart$projections[[2]]$stress < expected_stress) })
}

test_relax_seed <- function(filename, seed, legacy, dimension_annealing, expected_stress1, expected_stress2) {
    chart <- new(acmacs.Chart, filename)
    chart$remove_all_projections()
    if (legacy) {
        chart$relax("1280", 2, FALSE, seed)
    }
    else if (dimension_annealing) {
        chart$relax(2, seed, "1280", "dimension-annealing")
    }
    else {
        chart$relax(2, seed, "1280", "no-dimension-annealing")
    }
    stress <- chart$projections[[1]]$stress
    test_that(paste("[", filename, "] stress after optimization with randomization seed (", seed, " legacy:", legacy, " dim-ann:", dimension_annealing, "): ", stress, "  expected between ", expected_stress1, " and ", expected_stress2, sep=""), { expect_true(stress > expected_stress1 && stress < expected_stress2) })
    for (i in 1:10) {
        chart$relax("1280", 2, FALSE, seed)
        test_that(paste("    [", filename, "] stress after ", i, " optimization with randomization seed: ", stress, " expected: ", chart$projections[[1]]$stress, sep=""), { expect_equal(stress, chart$projections[[1]]$stress) })
    }
}

test_stress <- function(filename) {
    chart <- new(acmacs.Chart, filename)
    stress <- chart$stress_evaluator(2, "1280")
    layout <- matrix(c(0.10246913026410391089893181515436, 2.7251657776193636273376341705443, 1.6395800390714652117196692415746, -0.69115775901217124665976143660373, -3.0029837606701805796660664782394, -0.72614046041864199754201081304927, -3.1400652079505193547959152056137, -0.091278421759516489841779218750162, -2.8695138011149468049154620530317, 0.41193817446764668943615106400102, -3.8688847474942966719879677839344, -1.4836865438979829345811367602437, -3.3739100413912921361259122932097, -1.9475013743511639940919621949433, -2.5705312360046641551036827877397, 0.27668594538744145649999950364872, -2.5705312360198520060805549292127, 0.27668594537809682032403202356363, -1.2555525087442220844025086989859, 0.21999837018528450860443967940228, -0.25716879074202741994881193932088, 0.65092765927208873222298279870301, -0.10330705951227608929698931206076, 0.40322310751003648254808808815142, -0.52636947953023216495438418860431, 0.68926342541246865547321931444458, -0.65175671912716148792554804458632, 2.2298083057067246137705751607427, 1.0409239724961207862463652418228, -0.91282344206990118085087715371628, 0.56638065517549451843848373755463, 0.44755774656755265006324862042675, 4.2599605622061620380236490746029, 0.98205548928422048415853851111024, 2.3977528259996363502182248339523, -1.6802569317660251968504780961666, 3.8335939905144185146923518914264, 1.1062772532624676191659318646998, 2.5903387540093203433855251205387, -1.985248548211675290176003727538, 2.1126711621972988908169099886436, -2.0448766762290215837083451333456, 2.0693914795266481476687658869196, -1.6494402256853970545336096620304, -4.0890852993951041227660425647628, 0.49698653849485657785223224891524, -1.0539203982444169671595091131167, -0.34316054481408131415776097128401, 0.82112874432933968815717662437237, 0.60022032909499445363366021410911, 0.71606886484690290828325487382244, 2.8403171350597546940264237491647, 1.9011687605276976764656637897133, -2.8287123512879785991458447824698, 5.2821513447765919835319436970167, 2.0271720768005527801847165392246), ncol=2, byrow=TRUE)
    test_that("stress value for layout", { expect_equal(stress$value(layout), 71.790977) })
    test_that("stress gradient for layout", { expect_equal(stress$gradient(layout), numeric(56), tolerance=1e-7) })
}

test_relax_with_dimension_annealing <- function(filename, num_dim_1 = 5, num_dim_2 = 2) {
    chart <- new(acmacs.Chart, filename)
    chart$remove_all_projections()
    projection <- chart$new_projection("1280", num_dim_1)
    stresses = c(projection$stress)
    projection$relax()
    test_that("test_relax_with_dimension_annealing: starting number of dimensions", { expect_equal(projection$number_of_dimensions, num_dim_1) })
    stresses = c(stresses, projection$stress)
    projection$dimension_annealing(num_dim_2)
    stresses = c(stresses, projection$stress)
    projection$relax()
    # cat(paste("\n\nND: ", projection$number_of_dimensions), "\n\n", sep="")
    # test_that("test_relax_with_dimension_annealing: number of dimensions after dimension annealing", { expect_equal(projection$number_of_dimensions, 2) })
    stresses = c(stresses, projection$stress)

    # cat("\n\ntest_relax_with_dimension_annealing", sep="")
    # print(stresses)
    # # cat("\n", sep="")
    # # print(projection$layout)
    # cat("\n\n", sep="")
}

test_relax_with_unmovable <- function(filename) {
    unmovable <- 1:10
    chart <- new(acmacs.Chart, filename)
    chart$remove_all_projections()
    projection <- chart$new_projection("1280", 2)
    initial_layout <- projection$layout
    projection$set_unmovable(unmovable)
    projection$relax()
    test_that("upon relaxing with unmovable 1", { expect_equal(initial_layout[unmovable], projection$layout[unmovable]) })
    test_that("upon relaxing with unmovable 2", { expect_true(identical(initial_layout[unmovable], projection$layout[unmovable])) })
    test_that("upon relaxing with unmovable 3", { expect_false(identical(initial_layout, projection$layout)) })
}

test_relax_existing("2004-3.ace")
test_relax_existing("cdc-h1pdm-2009.acd1.bz2")
test_relax_many("2004-3.ace", 100, 72)
# test_relax_many("2004-3.ace", 10, 72)
# test_relax_many("2004-3.ace", 2, 72, "dimension-annealing")
# test_relax_many("2004-3.ace", 2, 76.15)
test_relax_many("cdc-h1pdm-2009.acd1.bz2", 5, 901, "dimension-annealing")
test_relax_many("cdc-h1pdm-2009.acd1.bz2", 5, 901)
test_relax_seed("cdc-h1pdm-2009.acd1.bz2", 778, TRUE,  TRUE, 868.0, 869.0)
test_relax_seed("cdc-h1pdm-2009.acd1.bz2", 778, FALSE, TRUE, 868.0, 869.0)
test_relax_seed("cdc-h1pdm-2009.acd1.bz2", 778, FALSE, FALSE, 1091.0, 1092.0)
test_relax_seed("cdc-h1pdm-2009.acd1.bz2", 666, TRUE,  TRUE, 880.0, 885.0)
test_relax_seed("cdc-h1pdm-2009.acd1.bz2", 666, FALSE, TRUE, 880.0, 885.0)
test_relax_seed("cdc-h1pdm-2009.acd1.bz2", 666, FALSE, FALSE, 750.0, 760.0)
test_relax_with_dimension_annealing("cdc-h1pdm-2009.acd1.bz2")
test_relax_with_unmovable("2004-3.ace")

test_stress("2004-3.ace")

# ======================================================================
