timeit <- function(expr, title) {
    tm <- system.time(expr)
    cat(paste(title, ": ", tm[["elapsed"]], "\n", sep=""))
}

timeit(library(acmacs.r), "loading acmacs.r")
# print(sessionInfo())

chart_filename <- "/Users/eu/ac/results/ssm/2018-0924-ssm/merges/cdc-h3-hi.ace"
timeit(chart <- new(acmacs.Chart, chart_filename), "reading chart")

prj = chart$projections[[1]]
move_to <- c(1967.0, -2017.0)
timeit(
  for (p_no in 1:chart$number_of_antigens) {
      prj$move_point(p_no, move_to)
  }
, paste("move", chart$number_of_antigens, "points"))
# print(prj$layout)

tts <- chart$titers
titer <- "40"
timeit(chart$remove_layers(), "remove_layers")
n_ags <- 273 # chart$number_of_antigens
n_srs <- 79 # chart$number_of_sera
timeit(
  for (ag_no in 1:n_ags) {
      for (sr_no in 1:n_srs) {
          tt <- tts$titer(ag_no, sr_no)
      }
  }
, paste("get", n <- n_ags * n_srs, "titers"))

timeit(
  for (ag_no in 1:n_ags) {
      for (sr_no in 1:n_srs) {
          tts$set_titer(ag_no, sr_no, titer)
      }
  }
, paste("set", n <- n_ags * n_srs, "titers"))
