context("Debugging test")

chart1 <- new(acmacs.Chart, 5, 3)
cat(sprintf("\nantigens:%d sera:%d\n", chart1$number_of_antigens, chart1$number_of_sera))
print(sapply(chart1$antigens, toString))
ag <- chart1$antigens[[1]]
ag$name <- "AG-1"
# fails: property is read-only: chart1$antigens[[1]]$name <- "AG-1A"
print(sapply(chart1$antigens, toString))
