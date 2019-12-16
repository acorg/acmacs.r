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
\preformatted{
cat(paste("chart name:", chart$name, "\n"))\cr
cat(paste0("chart multi-line info:\n\n", chart$info, "\n\n"))\cr
cat(paste("number of points (sum of antigens and sera):", chart$number_of_points, "\n"))\cr
cat(paste("lineage (influenza B only):", chart$lineage, "\n"))\cr
# change chart name
chart$name <- "modified name"
cat(paste("changed chart name:", chart$name, "\n"))\cr

# cloning chart (via hidden export/import)
chart2 <- chart$clone()

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
\bold{# ============ Column bases ============}\cr
\preformatted{
chart$column_bases() # get column bases (either calculated or forced)
chart$set_column_bases(c(7,8,9,10)) # set forced column bases
chart$set_column_basis(2, 11) # update forced column basis for serum 2

}\cr
\bold{# ===== Projections (maps) =====}\cr
\preformatted{
cat(paste("number of projections (maps):", chart$number_of_projections, "\n"))

# list with brief info (string) about each projection
#   it includes stress, dimensionality, minimum column basis, forced column bases
sapply(1:chart$number_of_projections, function(no) { chart$projections[[no]]$info })

# list with more detailed info about each projection
get_projection <- function(no) \{
  p <- chart$projection(no)
  list("stress"=p$stress, "comment"=p$comment, "number_of_dimensions"=p$number_of_dimensions,
       "minimum_column_basis"=p$minimum_column_basis,
       "forced_column_bases"=p$forced_column_bases, "transformation"=p$transformation,
       "layout"=p$layout, "transformed_layout"=p$transformed_layout)
\}
sapply(1:chart$number_of_projections, get_projection)

# transfomation matrix of the first projection (usually best, if chart loaded from acmacs file)
chart$projections[[1]]$transformation
# faster variant (does not build the R list of projections)
chart$projection(1)$transformation

# stored layout of the first projection
chart$projections[[1]]$layout
# faster variant (does not build the R list of projections)
chart$projection(1)$layout

# layout of the first projection with the transformation matrix applied
chart$projections[[1]]$transformed_layout
# faster variant (does not build the R list of projections)
chart$projection(1)$transformed_layout

# invariant
identical(chart$projection(1)$transformed_layout, chart$projection(1)$layout \%*\% chart$projection(1)$transformation)

# forced column bases
chart$projection(1)$forced_column_bases()

# remove all projections
chart$remove_all_projections()

# remove all projections except the best one
chart$remove_all_projections_except(1)

}\cr
\bold{# ===== Modifying projection =====}\cr
\preformatted{
# transformation, layout, transformed_layout create copies on accessing
# modifying them does not affect corresponding objects inside the chart
save_transformation <- chart$projection(1)$transformation
save_layout <- chart$projection(1)$layout
save_transformed_layout <- chart$projection(1)$transformed_layout

# set transfomation matrix for 2D projection
chart$projection(1)$set_transformation(matrix(c(1, 0, 0, 1), nrow=2, byrow=TRUE))

# set transfomation matrix for 3D projection
chart$projection(1)$set_transformation(matrix(c(1, 0, 0, 0, 1, 0, 0, 0, 1), nrow=3, byrow=TRUE))

# rotate the first projection 30 degrees counter-clockwise
chart$projection(1)$rotate_degrees(30)
# transformed_layout changed
identical(chart$projection(1)$transformed_layout, save_transformed_layout)
# transformation matrix changed
identical(chart$projection(1)$transformation, save_transformation)
# stored layout is untouched
identical(chart$projection(1)$layout, save_layout)

# rotate 30 degrees converted to radians clockwise
chart$projection(1)$rotate_radians(-30 * pi /180)
# now transformed layout and transformation are the same as saved
identical(chart$projection(1)$transformed_layout, save_transformed_layout)
identical(chart$projection(1)$transformation, save_transformation)
# and stored layout is still untouched
identical(chart$projection(1)$layout, save_layout)

# flip twice and rotate 180 degrees
chart$projection(1)$flip_north_south()
chart$projection(1)$flip_east_west()
chart$projection(1)$rotate_degrees(180)
# now transformed layout and transformation are (approximately) the same as saved
all.equal(chart$projection(1)$transformed_layout, save_transformed_layout)
all.equal(chart$projection(1)$transformation, save_transformation)

# move the first antigen to some place far away
chart$projection(1)$move_point(1, c(100, 100))
chart$projection(1)$layout[1,]
# move the first serum to some place far away
chart$projection(1)$move_point(chart$number_of_antigens + 1, c(100, 100))
# move the last serum to some place far away
chart$projection(1)$move_point(chart$number_of_antigens + chart$number_of_sera, c(100, 100))
# direct assignment to layout fails
# does not work: chart$projection(1)$layout[1,] <- c(10, 10)

# set layout to matrix filled with 0
chart$projection(1)$set_layout(matrix(c(0), nrow=chart$number_of_points, ncol=chart$projection(1)$number_of_dimensions))

# set (forced) column bases for all sera
chart$projection(1)$set_column_bases(c(7,8,9,10,9,8,7))
# set (forced) column basis for a single serum
chart$projection(1)$set_column_basis(1, 11)

# clone the best projection
new_proj <- chart$clone_projection(1)
}\cr
\bold{# ========= Save chart =========}\cr
\preformatted{
# output format depends on suffix used, only .ace and .save are supported
chart$save("/tmp/chart.ace")
chart$save("/tmp/chart.save")
# save can be compressed
chart$save("/tmp/chart.save.xz")
# write chart into a string in the ace format (https://github.com/acorg/acmacs-chart-2/blob/master/doc/ace-format.json)
ace_format_data <- chart$save()
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

# get styles for each point as a data frame
styles <- chart$plot_spec$styles_as_data_frame()
# set fill color of the 3rd antigens
styles$fill[[3]] <- "orange"
chart$plot_spec$set_styles(styles) # send modified styles to acmacs
# color returned by styles_as_data_frame() is always in "#RRGGBB" or
# "#TTRRGGBB" format, where TT is transparency (00 - opaque, FF - transparent)
chart$plot_spec$styles_as_data_frame()$fill[[3]] == "#ffa500"

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

# change other point style attributes
chart$plot_spec$set_shown(point_indexes, FALSE)
chart$plot_spec$set_outline(point_indexes, "red")
chart$plot_spec$set_outline_width(point_indexes, 5)
chart$plot_spec$set_rotation(point_indexes, 0.3)
chart$plot_spec$set_aspect(point_indexes, 0.2)
chart$plot_spec$set_shape(point_indexes, "triangle")
chart$plot_spec$set_label_shown(point_indexes, TRUE)
chart$plot_spec$set_label_offset_x(point_indexes, 1)
chart$plot_spec$set_label_offset_y(point_indexes, 1)
chart$plot_spec$set_label_size(point_indexes, 10)
chart$plot_spec$set_label_color(point_indexes, "red")
chart$plot_spec$set_label_rotation(point_indexes, 0.3)
chart$plot_spec$set_label_slant(point_indexes, "italic")
chart$plot_spec$set_label_weight(point_indexes, "bold")
chart$plot_spec$set_label_font_family(point_indexes, "monospace")
chart$plot_spec$set_label_text(point_indexes, "display name")

}\cr
\bold{# ========= Titers =========}\cr
\preformatted{
# extract the first titer in the table
chart$titers$titer(antigen_no, serum_no)
# extract the last titer in the table
chart$titers$titer(chart$number_of_antigens, chart$number_of_sera)
# extract all titers into a matrix (it's a function, not property!)
chart$titers$all()
# invariant
identical(sapply(1:chart$number_of_antigens, function(no) { chart$titers$titer(no, 1) }), chart$titers$all()[,1])

# remove layers
# if chart is a merge, titers cannot be modified. if you
# want to change them anyway, you have to remove layers from the
# chart first. merging resulting (flattened) chart with other charts usually
# gives wrong result.
chart$remove_layers()

# set titer
# IMPORTANT: modifying titer for a chart having projections leads to undefined behaviour!
chart$titers$set_titer(antigen_no, serum_no, "640")

# layers
chart$number_of_layers
single_titer <- chart$titers$titer_of_layer(layer_no, antigen_no, serum_no)
vector_if_titers <- chart$titers$titers_for_all_layers(antigen_no, serum_no)
list_of_dense_matrice <- chart$titers$all_layers()
}\cr
\bold{# ========= Making maps =========}\cr
See \code{\link{acmacs.Relax}} for more details.\cr
\preformatted{
# make new projection in 2d without minimum column basis with random layout and relax it
new_projection <- chart$relax("none", 2)
new_projection$stress
# make new projection in 2d without minimum column basis with random layout and relax it
# use rough optimization which is 30\% faster
new_projection <- chart$relax("none", 2, TRUE)
new_projection$stress
# make new projection in 2d without minimum column basis and with the specified layout
new_projection <- chart$new_projection_with_layout("none", matrix(c(0.0), ncol=2, nrow=chart$number_of_antigens + chart$number_of_sera, byrow=TRUE))
new_projection$stress
# sort all projections of the chart by stress
chart$sort_projections()
# make 10 new projections in 2d without minimum column basis with random layout and relax them using fine optimization
# relaxation is perfomed in parallel, if multiple CPU cores available
chart$relax_many("none", 2, 10, FALSE)
# get all stresses
sapply(chart$projections, function(p) {p$stress})
# relax existing projection, e.g. upon moving the first antigen
chart$projection(1)$move_point(1, c(100, 100))
chart$projection(1)$stress
chart$projection(1)$relax()
chart$projection(1)$stress

# create new projection in 2d without minimum column basis with random layout without optimisation (dimension annealing cannot be used)
projection <- chart$new_projection("none", 2)

# randomize layout in the projection using "table-max-distance" method
projection$randomize_layout()

# randomize layout in the projection using specific method (see \link{acmacs.Projection})
projection$randomize_layout("sample-optimization", 2.0)
projection$randomize_layout("table-max-distance", 2.0)
projection$randomize_layout("current-layout-area", 2.0)

# perform optimization step by step
while(projection$relax_one_iteration()) { print(projection$stress); print(projection$layout); }

# stress object for custom optimiser
stress <- chart$stress_evaluator(number_of_dimensions, minimum_column_basis)
# calculating stress value for given layout
stress_value <- stress$value(matrix(...))
# calculating stress contibution for a point, point_no is either antigen_no or (serum_no + number_of_antigens)
contibution <- stress$contribution(1, matrix(...))
#  calculating stress gradient for given layout
stress_gradient_vector <- stress$gradient(matrix(...))

}\cr
\bold{# ========= Procrustes =========}\cr
\preformatted{
# make procrustes between two projections of the same or different charts
# 3rd argument (scaling) is scaling (usually false)
# 4th argument (match) is passage and serum_id matching: "auto","strict", "relaxed", "ignore"
#   (just the first letter is enough: "a", "s", "r", "i")
# 5th argument (subset) is subset to use for matching: "all", "antigens", "sera"
# function returns acmacs.ProcrustesData object
pc12 <- acmacs.procrustes(chart$projections[[1]], chart$projections[[2]])
pc12 <- acmacs.procrustes(chart$projections[[1]], chart$projections[[2]], scaling=FALSE, match="auto")
pc12 <- acmacs.procrustes(chart$projections[[1]], chart$projections[[2]], subset="antigens")
# get RMS value (real) of the procrustes
rms <- pc12$rms
# get transformation matrix that rotates the second projection to be as
# close as possible to the first projection.
# result is a 3x3 matrix, where the third column is 1, and the third row
# containts translation.
transformation_matrix <- pc12$transformation
# reorient projection using procrustes
chart$projection(1)$reorient(chart2$projections[[1]])
chart$projection(1)$reorient(chart2$projections[[1]], "auto", "all")
# apply procrustes (transformation and translation) to layout (R matrix)
transformed_translated <- pc12$apply(matrix(ncol=2, c(...)))
}\cr
\bold{# ========= Matching antigens and sera in two charts =========}\cr
\preformatted{
match_data <- acmacs.match_antigens_sera(chart1, chart2)
# match_data is a named list of two vectors (antigens and sera),
# each vector provides mapping between antigen or serum index in chart1 to index in chart2.
}\cr
\bold{# ========= Grid test =========}\cr
\preformatted{
# Grid test is similar to hemi/local/grid test of acmacs-web
gr <- new(acmacs.GridTest, chart)
# Run multithreaded (omp) version of the test (faster), may conflict with other packages and crash R
gr_result <- gr$test()
# Run single thread version of the test, use it if multithreaded version crashes R
gr_result <- gr$test_single_thread()
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
\bold{# ========= Merging charts =========}\cr
\preformatted{
# merge two tables, source projections ignored, result has no projections
table_merge <- acmacs.merge(chart1, chart2)

# merge two charts incrementally (see \link{acmacs.merge})
incremental_merge <- acmacs.merge_incremental(chart1, chart2, optimizations=100, threads=0)

# make frozen merge of two charts with projections (see \link{acmacs.merge})
frozen_merge <- acmacs.merge_frozen(chart1, chart2)

# make overlay merge of two charts with projections (see \link{acmacs.merge})
overlay_merge <- acmacs.merge_overlay(chart1, chart2)

# generate text report of merging titers (see \link{acmacs.merge})
report <- acmacs.merge_report(chart1, chart2)
cat(report)
}
\bold{# ========= Map resolution test =========}\cr
\preformatted{
results <- acmacs.map_resolution_test(chart, number_of_dimensions=1:5, proportions_to_dont_care=c(0.1, 0.2 0.3), minimum_column_basis="none", column_bases_from_master=TRUE, relax_from_full_table=FALSE)
# column_bases_from_master: converting titers to dont-care may change column bases, force master chart column bases
# relax_from_full_table: additional projection in each replicate, first full table is relaxed, then titers dont-cared and the best projection relaxed again from already found starting coordinates.
}
\bold{# ========= Extension fields =========}\cr\cr
Chart (ace format only) can keep user's data in JSON format, e.g. GUI
specific data\cr
\preformatted{
# returns NA_character_ if chart has no specified extension field stored
ext.data <- fromJSON(chart$extension_field("my-key"))
# reports an error, if passed data is not a valid json
chart5$set_extension_field("mydata", "{\"aa\": 13}")
my.data <- data.frame(no=c(15:19), name=c("Fridrich", "Wilhelm", "Barbarossa", "Sonnenkoenig", "Oedipa"))
chart5$set_extension_field("my.data", toJSON(my.data))
}
}
\docType{package}
\keyword{ package }
\seealso{
  \code{\link{acmacs.r-package}}\cr
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.Relax}}\cr
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
  \code{\link{acmacs.GridTest}}\cr
  \code{\link{acmacs.merge}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
