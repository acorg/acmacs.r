context("Debugging test")

cat(sprintf("\n\n"))

chart1 <- new(acmacs.Chart, 5, 3)
cat(sprintf("initial:\n antigens (%d): \"%s\"\n sera (%d): \"%s\"\n", chart1$number_of_antigens, paste0(sapply(chart1$antigens, toString), collapse="\" \""), chart1$number_of_sera, paste0(sapply(chart1$sera, toString), collapse="\" \"")))

ag <- chart1$antigens[[1]]
ag$name <- "AG-1"
ag$date <- "2018-01-01"
ag$lineage <- "YAMAGATA"
ag$reassortant <- "NYMC-000"
ag$reference <- TRUE
ag$set_passage("E4")

chart1$antigens[[1]]$set_name("AG-1set")
# fails: property is read-only: chart1$antigens[[1]]$name <- "AG-1A"

cat(sprintf("after mods:\n antigens (%d): \"%s\"\n sera (%d): \"%s\"\n", chart1$number_of_antigens, paste0(sapply(chart1$antigens, toString), collapse="\" \""), chart1$number_of_sera, paste0(sapply(chart1$sera, toString), collapse="\" \"")))

cat(sprintf("\n\n"))
