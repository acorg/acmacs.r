context("Test chart$plot_spec")

chart <- new(acmacs.Chart, "cdc-h1pdm-20090612.ace")
plot_spec <- chart$plot_spec
cat(c("\n\nfill", plot_spec$styles[[1]]$fill, "\n\n"))
