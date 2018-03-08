context("Debugging test")

chart1 <- new(acmacs.Chart, 5, 3)
cat(sprintf("\nantigens:%d sera:%d\n", chart1$number_of_antigens, chart1$number_of_sera))
print(sapply(chart1$antigens, toString))
