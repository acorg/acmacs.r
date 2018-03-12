\name{acmacs.Chart.from.scratch}
\title{Example of creating chart from scratch.}
\alias{acmacs.Chart.from.scratch}
\section{Example}{
\code{library(acmacs.r)}\cr\cr
\bold{# == Function to print table ==}
\preformatted{
print_table <- function(chart) {
    table <- chart$titers$all()
    rownames(table) <- sapply(chart$antigens, toString)
    colnames(table) <- sapply(chart$sera, toString)
    cat(sprintf("antigens: \%d, sera: \%d\n", chart$number_of_antigens, chart$number_of_sera))
    options(width = 500)
    print(table)
    cat(sprintf("\n"))
}
}
\bold{# == Creating chart from scratch ==}
\preformatted{
num_antigens <- 10
num_sera <- 5
chart <- new(acmacs.Chart, num_antigens, num_sera)
# upon creation antigens and sera have some names, all titers are *
print_table(chart)
}
\bold{# == Set/modify titers ==}
\preformatted{
# Set titers at random
titer_variants <- c("<10", "20", "40", "80", "160", "320", "640", "1280", "2560", ">2560");
for (ag in 1:chart$number_of_antigens) {
    for (sr in 1:chart$number_of_sera) {
        chart$titers$set_titer(ag, sr, sample(titer_variants, 1))
    }
}
print_table(chart)

# Set titers of the last serum to *
chart$titers$set_dontcare_for_serum(chart$number_of_sera)
# Set titers of the second antigen to *
chart$titers$set_dontcare_for_antigen(2)
# Multiply titers of the first antigen by 2
chart$titers$multiply_by_for_antigen(1, 2)
# Multiply titers of the second serum by 0.5
chart$titers$multiply_by_for_serum(2, 0.5)
print_table(chart)
}
\bold{# == Insert/remove antigens/sera ==}
\preformatted{
# insert new antigen before the first one
ag1 <- chart$insert_antigen(1)
# it has random name and all titers set to *
print(ag1$full_name)

# insert new serum after the last one
sr1 <- chart$insert_serum(chart$number_of_sera + 1)
print(sr1$full_name)

# remove two last sera
chart$remove_sera((chart$number_of_sera-1):chart$number_of_sera)

# remove two antigens in the middle
chart$remove_antigens(5:6)

# resulting table
print_table(chart)
}
\bold{# == Change antigen/serum name and other fields ==}
\preformatted{
# change antigen fields
ag <- chart$antigens[[1]]
ag$name <- "Kleine Hexe"
ag$date <- "2018-03-22"
ag$lineage <- "YAMAGATA"
ag$reassortant <- "NYMC-37B"
ag$reference <- TRUE
ag$set_passage("E4")
# you can add annotation multiple times, but it appears just once in the result
ag$add_annotation("NOT-NEW")
ag$add_annotation("*HOT*")
ag$add_annotation("*HOT*")
ag$add_annotation("NOT-NEW")
ag$remove_annotation("NOT-NEW")
print(ag$full_name)

# you have to assign antigen/serum reference to a variable before
# modifying fields. the following code will fail (limitation of Rcpp?):
# chart$antigens[[1]]$name <- "Where is my name?"
# you may use set_*() instead
chart$antigens[[1]]$set_name("Hermione")
chart$antigens[[1]]$set_passage("MDCK1")
chart$antigens[[1]]$set_date("2001-09-11")

# change serum fields
sr <- chart$sera[[2]]
sr$name <- "Hans im Glueck"
sr$lineage <- "YAMAGATA"
sr$reassortant <- "HGR"
sr$set_passage("E4")
sr$set_serum_id("F18/18")
sr$set_serum_species("VULCAN")
sr$add_annotation("NOT-NEW")
print(sr$full_name)

# resulting table
print_table(chart)
}
}
\docType{package}
\keyword{ package }
\seealso{
  \code{\link{acmacs.r-package}}\cr
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Example}}\cr
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
