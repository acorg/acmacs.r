context("Debugging test")

print_table <- function(chart, title) {
    table <- chart$titers$all()
    rownames(table) <- sapply(chart$antigens, toString)
    colnames(table) <- sapply(chart$sera, toString)
    cat(sprintf("%s (antigens: %d, sera: %d)\n", title, chart$number_of_antigens, chart$number_of_sera))
    options(width = 500)
    print(table, zero.print = ".")
    cat(sprintf("\n"))
}

read_from_file <- function(filename) {
    cat("reading from ", filename, "\n")
    start <- proc.time()
    rows <- readLines(filename)
    serum_names <- scan(text=rows[[1]], what="c", quiet=TRUE)
    chart <- new(acmacs.Chart, length(rows) - 1, length(serum_names))
    for (sr_no in 1:length(serum_names)) {
        chart$sera[[sr_no]]$set_name(serum_names[[sr_no]])
    }
    for (row_no in 2:length(rows)) {
        name_titers <- scan(text=rows[[row_no]], what="c", quiet=TRUE)
        chart$antigens[[row_no-1]]$set_name(name_titers[[1]])
        for (sr_no in 1:length(serum_names)) {
            chart$titers$set_titer(row_no-1, sr_no, name_titers[[sr_no + 1]]);
        }
    }
    cat("reading done in ", paste((proc.time() - start)[[1]]), "\n")
    chart
}

test3 <- function() {
    # c1 <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")
    c1 <- new(acmacs.Chart, "/r/u/sam/landscapes_map.ace")
    print(c1$projections[[1]]$stress)
    g <- new(acmacs.GridTest, c1)
    r <- g$test()
    # print(r)
    p <- g$make_new_projection_and_relax()
    print(p$stress)
    r <- g$test()
    print(r)
    p <- g$make_new_projection_and_relax()
    print(p$stress)
    r <- g$test()
    print(r)
}

# ----------------------------------------------------------------------

test2 <- function() {
    # chart <- new(acmacs.Chart, "/r/landscapes_map.acd1")
    chart <- read_from_file("/r/a.table.txt")

    for(n in 1:20){
        print(n)
        try({ chart$relax("none", 2, TRUE) })
    }

    for(n in 1:20){
        start <- proc.time()
        s <- chart$projections[[n]]$stress
        cat(paste(n), " stress: ", paste(s), " time: ", paste((proc.time() - start)[[1]]), "\n")
    }
    cat("sorting projections\n")
    print(system.time(chart$sort_projections()))
    cat("sorting projections done\n")

    stresses <- sapply(chart$projections, function(x){ x$stress })
    print(stresses)
    cat("\n")

                                        # Second time it is very quick
    stresses <- sapply(chart$projections, function(x){ x$stress })
    print(stresses)
    cat("\n")

                                        # Accessing layouts is always quick
    layouts <- sapply(chart$projections, function(x){ x$layout })
    print(stresses)
    cat("\n")
}

# ----------------------------------------------------------------------

test1 <- function() {
    chart <- read_from_file("/r/a.tab")
    print_table(chart, "/r/a.tab")
    chart$relax_many("none", 2, 100, TRUE)
    print(chart$info)

    sTiters <- c("<10", "20", "40", "80", "160", "320", "640", "1280", "2560", ">2560");

    cat(sprintf("\n\n"))

    chart1 <- new(acmacs.Chart, 5, 10)
    titers <- chart1$titers
    for (ag in 1:chart1$number_of_antigens) {
        for (sr in 1:chart1$number_of_sera) {
            titers$set_titer(ag, sr, sample(sTiters, 1))
        }
    }
    print_table(chart1, "initial")
                                        # cat(sprintf("initial:\n antigens (%d): \"%s\"\n sera (%d): \"%s\"\n", chart1$number_of_antigens, paste0(sapply(chart1$antigens, toString), collapse="\" \""), chart1$number_of_sera, paste0(sapply(chart1$sera, toString), collapse="\" \"")))

    ag0 <- chart1$insert_antigen(3)
    ag0$set_passage("E4")
                                        #chart1$remove_sera(1)
    chart1$remove_sera(c(1,3,8))
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

    titers$set_dontcare_for_serum(chart1$number_of_sera)
    titers$multiply_by_for_antigen(chart1$number_of_antigens, 2)
    titers$multiply_by_for_antigen(chart1$number_of_antigens - 1, 0.5)
    titers$multiply_by_for_antigen(chart1$number_of_antigens - 1, 0.5)
    titers$multiply_by_for_antigen(chart1$number_of_antigens - 1, 0.5)
    print_table(chart1, "after titer modifications")

    cat(sprintf("\n\n"))
}

# ----------------------------------------------------------------------

cat("\n\n")
test3()

## test1()
## test2()
