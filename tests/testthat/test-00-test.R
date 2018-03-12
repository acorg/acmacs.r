context("Debugging test")

print_table <- function(chart, title) {
    table <- chart$titers$all()
    rownames(table) <- sapply(chart$antigens, toString)
    colnames(table) <- sapply(chart$sera, toString)
    cat(sprintf("%s (antigens: %d, sera: %d)\n", title, chart$number_of_antigens, chart1$number_of_sera))
    print(table)
    cat(sprintf("\n"))
}

cat(sprintf("\n\n"))

chart1 <- new(acmacs.Chart, 5, 3)
print_table(chart1, "initial")
# cat(sprintf("initial:\n antigens (%d): \"%s\"\n sera (%d): \"%s\"\n", chart1$number_of_antigens, paste0(sapply(chart1$antigens, toString), collapse="\" \""), chart1$number_of_sera, paste0(sapply(chart1$sera, toString), collapse="\" \"")))

ag0 <- chart1$insert_antigen(1)
print_table(chart1, "after insert")
# cat(sprintf("insert:\n antigens (%d): \"%s\"\n sera (%d): \"%s\"\n", chart1$number_of_antigens, paste0(sapply(chart1$antigens, toString), collapse="\" \""), chart1$number_of_sera, paste0(sapply(chart1$sera, toString), collapse="\" \"")))

ag <- chart1$antigens[[1]]
ag$name <- "AG-1"
ag$date <- "2018-01-01"
ag$lineage <- "YAMAGATA"
ag$reassortant <- "NYMC-000"
ag$reference <- TRUE
ag$set_passage("E4")
ag$add_annotation("NOT-NEW")
ag$add_annotation("*HOT*")
ag$add_annotation("*HOT*")
ag$add_annotation("NOT-NEW")
ag$remove_annotation("NOT-NEW")

chart1$antigens[[1]]$set_name("AG-1set")
# fails: property is read-only: chart1$antigens[[1]]$name <- "AG-1A"

sr <- chart1$sera[[2]]
sr$name <- "SR-2"
sr$lineage <- "YAMAGATA"
sr$reassortant <- "HGR"
sr$set_passage("E4")
sr$set_serum_id("F18/18")
sr$set_serum_species("VULCAN")
sr$add_annotation("NOT-NEW")

print_table(chart1, "after name modifications")
# cat(sprintf("after mods:\n antigens (%d): \"%s\"\n sera (%d): \"%s\"\n", chart1$number_of_antigens, paste0(sapply(chart1$antigens, toString), collapse="\" \""), chart1$number_of_sera, paste0(sapply(chart1$sera, toString), collapse="\" \"")))

cat(sprintf("\n\n"))
