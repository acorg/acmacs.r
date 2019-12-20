context("Test map_resolution_test")

chart1 <- new(acmacs.Chart, "2004-01.ace")
## res <- acmacs.map_resolution_test(chart=chart1, number_of_dimensions=2:3, proportions_to_dont_care=c(0.1, 0.5), number_of_random_replicates_for_each_proportion=5,
##                                   number_of_optimizations=1000, save_charts_to="/d/1")
## print(paste("\n"))
res <- acmacs.map_resolution_test(chart=chart1, number_of_dimensions=2:3, proportions_to_dont_care=c(0.1, 0.5), number_of_optimizations=1000)
## print(res)
## res <- acmacs.map_resolution_test(chart=chart1, number_of_dimensions=2:3, proportions_to_dont_care=c(0.1, 0.5), number_of_optimizations=1000)
## print(res)
## res <- acmacs.map_resolution_test(chart=chart1, number_of_dimensions=2:3, proportions_to_dont_care=c(0.1, 0.5), number_of_optimizations=1000)
## print(res)
