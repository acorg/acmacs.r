\name{acmacs.Example}
\title{Functions and methods of the acmacs.r package in an example.}
\alias{acmacs.Example}
\alias{acmacs.example}
% library(acmacs.r, lib.loc="/r/R/library"); ?acmacs.example
\section{Example}{
\code{# library(acmacs.r)}\cr\cr
\bold{# load chart from file}\cr
\code{\var{chart} <- new(acmacs.Chart, "table.ace")} # .save, .ace, .acd1 files are supported\cr
\cr
\bold{# == load chart from remote file ==}\cr
\code{
  \var{url} <- "https://github.com/acorg/acmacs.r/blob/master/tests/testthat/cdc-h1pdm-20090612.ace?raw=true"\cr
  \var{connection} <- file(\var{url}, open="rb")\cr
  \var{raw_data} <- readBin(\var{connection}, "raw", n=999999)\cr
  close(connection) # to avoid warning during garbage collection
  \cr
  \var{chart} <- new(acmacs.Chart, \var{raw_data})\cr
}\cr
\bold{# ====== Info about chart ======}\cr
\code{
  cat(paste("chart name:", chart$name, "\n"))\cr
  cat(paste0("chart multi-line info:\n\n", chart$info, "\n\n"))\cr
  cat(paste("number of points (sum of antigens and sera):", chart$number_of_points, "\n"))\cr
  cat(paste("lineage (influenza B only):", chart$lineage, "\n"))\cr
}\cr
\bold{# ========== Antigens ==========}\cr
\preformatted{
cat(paste("number of antigens:", chart$number_of_antigens, "\n"))

# vector of strings with full names of all antigens (full name includes annotations, reassortant, passage)
sapply(chart$antigens, toString)

# list with all info about antigens
#   passage is an object, use paste(passage) to convert it to a string
#   lab_ids is a list of strings
#   annotations is an (often empty) list of strings
get_ag <- function(ag_no) \{
  ag <- chart$antigens[[ag_no]]
  list("name"=ag$name, "full_name"=ag$full_name, "abbreviated_name"=ag$abbreviated_name,
       "date"=ag$date, "passage"=paste(ag$passage), "lineage"=ag$lineage,
       "reassortant"=ag$reassortant, "reference"=ag$reference, "lab_ids"=ag$lab_ids,
       "annotations"=ag$annotations)
\}
sapply(1:chart$number_of_antigens, get_ag)

# Antigens, their passages and info about passages
#  passage has no properties, just methods
#  to get passage as string use toString(), as.character(), paste()
get_ag_passage <- function(ag_no) \{
  ag <- chart$antigens[[ag_no]]
  passage <- ag$passage
  list("name"=ag$name, "passage"=toString(passage), "without_date"=passage$without_date(), "type"=passage$type(),
       "is_egg"=passage$is_egg(), "is_cell"=passage$is_cell())
\}
sapply(1:chart$number_of_antigens, get_ag_passage)
}\cr
\bold{# ============ Sera ============}\cr
\preformatted{
cat(paste("number of sera:", chart$number_of_sera, "\n"))

# vector of strings with full names of all sera (full name includes annotations, reassortant, serum_id)
sapply(chart$sera, toString)

# list with all info about sera
#   passage is an object, use paste(passage) to convert it to a string
#   annotations is an (often empty) list
get_sr <- function(sr_no) \{
  sr <- chart$sera[[sr_no]]
  list("name"=sr$name, "full_name"=sr$full_name, "abbreviated_name"=sr$abbreviated_name,
       "passage"=paste(sr$passage), "lineage"=sr$lineage, "reassortant"=sr$reassortant,
       "annotations"=sr$annotations, "serum_id"=sr$serum_id, "serum_species"=sr$serum_species)
\}
sapply(1:chart$number_of_sera, get_sr)
}\cr
\bold{# ===== Projections (maps) =====}\cr
\preformatted{
cat(paste("number of projections (maps):", chart$number_of_projections, "\n"))

# list with brief info (string) about each projection
#   it includes stress, dimensionality, minimum column basis, forced column bases
sapply(1:chart$number_of_projections, function(no) { chart$projections[[no]]$info })

# list with more detailed info about each projection
get_projection <- function(no) \{
  p <- chart$projections[[no]]
  list("stress"=p$stress, "comment"=p$comment, "minimum_column_basis"=p$minimum_column_basis,
       "forced_column_bases"=p$forced_column_bases, "transformation"=p$transformation,
       "layout"=p$layout, "transformed_layout"=p$transformed_layout)
\}
sapply(1:chart$number_of_projections, get_projection)

# transfomation matrix of the first projection (usually best, if chart loaded from acmacs file)
# makes sense for 2d maps only
chart$projections[[1]]$transformation

# stored layout of the first projection
chart$projections[[1]]$layout

# layout of the first projection with the transformation matrix applied
chart$projections[[1]]$transformed_layout

# invariant
identical(chart$projections[[1]]$transformed_layout, chart$projections[[1]]$layout \%*\% chart$projections[[1]]$transformation)

}\cr
\bold{# ===== Modifying projection =====}\cr
\preformatted{
# transformation, layout, transformed_layout create copies on accessing
# modifying them does not affect corresponding objects inside a chart
save_transformation <- chart$projections[[1]]$transformation
save_layout <- chart$projections[[1]]$layout
save_transformed_layout <- chart$projections[[1]]$transformed_layout

# rotate the first projection 30 degrees counter-clockwise
chart$projections[[1]]$rotate_degrees(30)
# transformed_layout changed
identical(chart$projections[[1]]$transformed_layout, save_transformed_layout)
# transformation matrix changed
identical(chart$projections[[1]]$transformation, save_transformation)
# stored layout is untouched
identical(chart$projections[[1]]$layout, save_layout)

# rotate 30 degrees converted to radians clockwise
chart$projections[[1]]$rotate_radians(-30 * pi /180)
# now transformed layout and transformation are the same as saved
identical(chart$projections[[1]]$transformed_layout, save_transformed_layout)
identical(chart$projections[[1]]$transformation, save_transformation)
# and stored layout is still untouched
identical(chart$projections[[1]]$layout, save_layout)

# flip twice and rotate 180 degrees
chart$projections[[1]]$flip_north_south()
chart$projections[[1]]$flip_east_west()
chart$projections[[1]]$rotate_degrees(180)
# now transformed layout and transformation are (approximately) the same as saved
all.equal(chart$projections[[1]]$transformed_layout, save_transformed_layout)
all.equal(chart$projections[[1]]$transformation, save_transformation)

# move the first antigen to some place far away
chart$projections[[1]]$move_point(1, c(100, 100))
chart$projections[[1]]$layout[1,]
# move the first serum to some place far away
chart$projections[[1]]$move_point(chart$number_of_antigens + 1, c(100, 100))
# move the last serum to some place far away
chart$projections[[1]]$move_point(chart$number_of_antigens + chart$number_of_sera, c(100, 100))
# direct assignment to layout fails
# does not work: chart$projections[[1]]$layout[1,] <- c(10, 10)

}\cr
\bold{# ========= Save chart =========}\cr
\preformatted{
# output format depends on suffix used, only .ace and .save are supported
chart$save("/tmp/chart.ace")
chart$save("/tmp/chart.save")
# save can be compressed
chart$save("/tmp/chart.save.xz")
}\cr
\bold{# ========= Plot spec =========}\cr
\cr
}
\docType{package}
\keyword{ package }
\seealso{
  \code{\link{acmacs.r-package}}
  \code{\link{acmacs.Chart}}
  \code{\link{acmacs.Antigen}}
  \code{\link{acmacs.Serum}}
  \code{\link{acmacs.Projection}}
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
