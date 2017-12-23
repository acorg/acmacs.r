\name{acmacs.Example}
\title{Functions and methods of the acmacs.r package in an example.}
\alias{acmacs.Example}
\alias{acmacs.example}
% library(acmacs.r, lib.loc="/r/R/library"); ?acmacs.example
\section{Example}{
\code{library(acmacs.r)}\cr\cr
\bold{# load chart from file}\cr
\code{\var{chart} <- new(acmacs.Chart, "table.ace")} # .save, .ace, .acd1 files are supported\cr
\cr
\bold{# load chart from remote file}\cr
\code{
  \var{url} <- "https://github.com/acorg/acmacs.r/blob/master/tests/testthat/cdc-h1pdm-20090612.ace?raw=true"\cr
  \var{connection} <- file(\var{url}, open="rb")\cr
  \var{raw_data} <- readBin(\var{connection}, "raw", n=999999)\cr
  close(connection) # to avoid warning during garbage collection
  \cr
  \var{chart} <- new(acmacs.Chart, \var{raw_data})\cr
}\cr
\bold{# Info about chart}\cr
\code{
  cat(paste("chart name:", chart$name, "\n"))\cr
  cat(paste0("chart multi-line info:\n\n", chart$info, "\n\n"))\cr
  cat(paste("number of points (sum of antigens and sera):", chart$number_of_points, "\n"))\cr
  cat(paste("lineage (influenza B only):", chart$lineage, "\n"))\cr
}\cr

\bold{# Antigens}\cr
\code{cat(paste("number of antigens:", chart$number_of_antigens, "\n"))}\cr
\cr
\code{# full names of all antigens (full name includes annotations, reassortant, passage)}\cr
\code{cat(paste(sapply(chart$antigens, toString), collapse="\n"))}\cr
\preformatted{
# enumerate antigens and extract all info about them into a list
#   passage is an object, use paste(passage) to convert it to a string
get_ag <- function(ag_no) \{
  ag <- chart$antigens[[ag_no]]
  data <- list("name"=ag$name, "full_name"=ag$full_name, "abbreviated_name"=ag$abbreviated_name,
               "date"=ag$date, "passage"=paste(ag$passage), "lineage"=ag$lineage,
               "reassortant"=ag$reassortant, "reference"=ag$reference, "lab_ids"=ag$lab_ids,
               "annotations"=ag$annotations)
\}
lapply(1:chart$number_of_antigens, get_ag)
}\cr

\bold{# Sera}\cr
\code{cat(paste("number of sera:", chart$number_of_sera, "\n"))}\cr
\preformatted{
# enumerate sera and extract all info about them into a list
get_sr <- function(sr_no) \{
  sr <- chart$sera[[sr_no]]
  data <- list("name"=sr$name, "full_name"=sr$full_name, "abbreviated_name"=sr$abbreviated_name,
               "passage"=paste(sr$passage), "lineage"=sr$lineage, "reassortant"=sr$reassortant,
               "annotations"=sr$annotations, "serum_id"=sr$serum_id, "serum_species"=sr$serum_species)
\}
lapply(1:chart$number_of_sera, get_sr)
}
\cr

\bold{# Projections (maps)}\cr
\code{cat(paste("number of projections (maps):", chart$number_of_projections, "\n"))}\cr
\cr
\bold{# Save chart}\cr
\code{chart$save("/tmp/chart.ace")}\cr
\code{chart$save("/tmp/chart.save")}\cr
\code{chart$save("/tmp/chart.save.xz")}\cr
\cr
\bold{# Plot spec}\cr
\cr
}
\docType{package}
\examples{

  # print information about antigens
  ags <- chart$antigens
  format_antigen <- function(a) \{ paste(c(a$name, a$passage, a$reassortant, a$reference, a$annotations, paste("[", a$date, "]", sep="", collapse=""), a$lab_ids), collapse=" ", sep="") \}
  cat("antigens:", length(ags), "\n ", paste(sapply(ags, format_antigen), collapse="\n  "), "\n")
  # print full names of all antigens (full name includes annotations, reassortant, passage)
  cat(paste(sapply(chart$antigens, toString), collapse="\n"))
}
\keyword{ package }
\seealso{
  \code{\link{acmacs.r-package}}
  \code{\link{acmacs.Chart}}
  \code{\link{acmacs.Antigen}}
  \code{\link{acmacs.Serum}}
  \code{\link{acmacs.Projection}}
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
