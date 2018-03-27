\name{acmacs.Example}
\title{Functions and methods of the acmacs.r package in an example.}
\alias{acmacs.Example}
\alias{acmacs.example}
% library(acmacs.r, lib.loc="/r/R/library"); ?acmacs.example
\section{Example}{
\code{# library(acmacs.r)}\cr\cr
\bold{# == Loading chart from file ==}\cr
\preformatted{chart <- new(acmacs.Chart, path.expand("~/tables/table.ace")) # .save, .ace, .acd1 files are supported
#  Automatic expansion of ~ in filenames is not supported, please use path.expand
}\cr
\bold{# == Loading chart from remote file ==}\cr
\code{
  \var{url} <- "https://github.com/acorg/acmacs.r/blob/master/tests/testthat/cdc-h1pdm-20090612.ace?raw=true"\cr
  \var{connection} <- file(\var{url}, open="rb")\cr
  \var{raw_data} <- readBin(\var{connection}, "raw", n=999999)\cr
  close(connection) # to avoid warning during garbage collection
  \cr
  \var{chart} <- new(acmacs.Chart, \var{raw_data})\cr
}\cr
\bold{# == Creating chart from scratch ==}\cr
\preformatted{
num_antigens <- 5
num_sera <- 3
chart <- new(acmacs.Chart, num_antigens, num_sera)
}
See \code{\link{acmacs.Chart.from.scratch}} and
\code{\link{acmacs.Chart.from.text.file}} for more complete examples\cr\cr
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
# modifying them does not affect corresponding objects inside the chart
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
\preformatted{
# list with styles for each point
get_style <- function(no) \{
  s <- chart$plot_spec$styles[[no]]
  list("shown"=s$shown, "size"=s$size, "fill"=s$fill, "outline"=s$outline, "outline_width"=s$outline_width,
       "rotation"=s$rotation, "aspect"=s$aspect, "shape"=s$shape)
\}
sapply(1:chart$number_of_points, get_style)
# modifying resulting list does not affect plot style stored in the chart

# drawing order: point indexes in the order of drawing,
# last indexes in the list are drawn on top of others
chart$plot_spec$drawing_order
# modifying resulting vector does not drawing order stored in the chart

# raise reference antigens
ref_indexes <- 1:chart$number_of_antigens
ref_indexes <- ref_indexes[lapply(ref_indexes, function(no) { chart$antigens[[no]]$reference})==TRUE]
chart$plot_spec$raise(ref_indexes)
chart$plot_spec$drawing_order

# lower all sera
chart$plot_spec$lower_sera(1:chart$number_of_sera)
chart$plot_spec$drawing_order

# change size of test antigens
test_indexes <- 1:chart$number_of_antigens
test_indexes <- test_indexes[lapply(test_indexes, function(no) { chart$antigens[[no]]$reference})==FALSE]
chart$plot_spec$set_size(test_indexes, 3)
# change fill color of test antigens
chart$plot_spec$set_fill(test_indexes, "pink")
# change outline color of test antigens
chart$plot_spec$set_fill(test_indexes, "#C0C0C0")
}\cr
\bold{# ========= Titers =========}\cr
\preformatted{
# extract the first titer in the table
chart$titers$titer(1, 1)
# extract the last titer in the table
chart$titers$titer(chart$number_of_antigens, chart$number_of_sera)
# extract all titers into a matrix (it's a function, not property!)
chart$titers$all()
# invariant
identical(sapply(1:chart$number_of_antigens, function(no) { chart$titers$titer(no, 1) }), chart$titers$all()[,1])
}\cr
\bold{# ========= Making maps =========}\cr
\preformatted{
# make new projection in 2d without minimum column basis with random layout and relax it
new_projection <- chart$relax("none", 2)
new_projection$stress
# make new projection in 2d without minimum column basis with random layout and relax it
# use rough optimization which is 30\% faster
new_projection <- chart$relax("none", 2, TRUE)
new_projection$stress
# sort all projections of the chart by stress
chart$sort_projections()
# make 10 new projections in 2d without minimum column basis with random layout and relax them using fine optimization
# relaxation is perfomed in parallel, if multiple CPU cores available
chart$relax_many("none", 2, 10, FALSE)
# get all stresses
sapply(chart$projections, function(p) {p$stress})
# relax existing projection, e.g. upon moving the first antigen
chart$projections[[1]]$move_point(1, c(100, 100))
chart$projections[[1]]$stress
chart$projections[[1]]$relax()
chart$projections[[1]]$stress
}\cr
\bold{# ========= Procrustes =========}\cr
\preformatted{
# make procrustes between two projections of the same or different charts
# 3rd argument is scaling (usually false)
# 4th argument is passage and serum_id matching: "auto", "strict", "relaxed", "ignore"
#   (just the first letter is enough: "a", "s", "r", "i")
# function returns acmacs.ProcrustesData object
pc12 <- acmacs.procrustes(chart$projections[[1]], chart$projections[[2]], FALSE, "auto")
# get RMS value (real) of the procrustes
rms <- pc12$rms
# get transformation matrix that rotates the second projection to be as
# close as possible to the first projection.
# result is a 3x3 matrix, where the third column is 1, and the third row
# containts translation.
transformation_matrix <- pc12$transformation
}\cr
\bold{# ========= Grid test =========}\cr
\preformatted{
# Grid test is similar to hemi/local/grid test of acmacs-web
gr <- new(acmacs.GridTest, chart)
gr_result <- gr$test()
# gr_result is data frame with the test results
# if there are trapped points, you may create new projection with better
# positions of the trapped points and relax it
new_proj <- gr$make_new_projection_and_relax()
# gr is automatically adjusted to handle new projection
# new layout
new_proj$layout
# test new projection
new_result <- gr$test()
}\cr
}
\docType{package}
\keyword{ package }
\seealso{
  \code{\link{acmacs.r-package}}\cr
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
  \code{\link{acmacs.GridTest}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
