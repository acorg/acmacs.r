#include "plot-spec.hh"

// ----------------------------------------------------------------------

Rcpp::List PlotSpec::styles() const
{
    const auto styles = obj_->all_styles();
    return {styles.begin(), styles.end()};
}

// ----------------------------------------------------------------------

Rcpp::DataFrame PlotSpec::styles_as_data_frame() const
{
    using namespace Rcpp;

    const auto styles = obj_->all_styles();
    LogicalVector shown(styles.size());
    StringVector fill(styles.size());
    StringVector outline(styles.size());
    NumericVector outline_width(styles.size());
    NumericVector size(styles.size());
    NumericVector rotation(styles.size());
    NumericVector aspect(styles.size());
    StringVector shape(styles.size());
    LogicalVector label_shown(styles.size());
    NumericVector label_offset_x(styles.size());
    NumericVector label_offset_y(styles.size());
    NumericVector label_size(styles.size());
    StringVector label_color(styles.size());
    NumericVector label_rotation(styles.size());
    StringVector label_slant(styles.size());
    StringVector label_weight(styles.size());
    StringVector label_font_family(styles.size());
    StringVector label_text(styles.size());

    for (auto [index, entry] : acmacs::enumerate(styles)) {
        shown[index] = entry.shown;
        fill[index] = fmt::format("{}", entry.fill);
        outline[index] = fmt::format("{}", entry.outline);
        outline_width[index] = entry.outline_width->value();
        size[index] = entry.size->value();
        rotation[index] = entry.rotation->value();
        aspect[index] = entry.aspect->value();
        shape[index] = fmt::format("{}", *entry.shape);

        label_shown[index] = entry.label.shown;
        label_offset_x[index] = entry.label.offset->x();
        label_offset_y[index] = entry.label.offset->y();
        label_size[index] = entry.label.size->value();
        label_color[index] = fmt::format("{}", entry.label.color);
        label_rotation[index] = entry.label.rotation->value();
        label_slant[index] = fmt::format("{}", *entry.label.style.slant);
        label_weight[index] = fmt::format("{}", *entry.label.style.weight);
        label_font_family[index] = *entry.label.style.font_family;

        label_text[index] = static_cast<std::string>(*entry.label_text);
    }

    return DataFrame::create(_["shown"] = shown, _["fill"] = fill, _["outline"] = outline, _["outline_width"] = outline_width, _["size"] = size, _["rotation"] = rotation, _["aspect"] = aspect,
                             _["shape"] = shape, _["label_shown"] = label_shown, _["label_offset_x"] = label_offset_x, _["label_offset_y"] = label_offset_y, _["label_size"] = label_size,
                             _["label_color"] = label_color, _["label_rotation"] = label_rotation, _["label_slant"] = label_slant, _["label_weight"] = label_weight,
                             _["label_font_family"] = label_font_family, _["label_text"] = label_text, _["stringsAsFactors"] = false);

} // PlotSpec::styles_as_data_frame

// ----------------------------------------------------------------------

// df["shown"][[1]][[2]]=FALSE

void PlotSpec::set_styles(const Rcpp::DataFrame& new_styles)
{
    using namespace Rcpp;

    LogicalVector shown = new_styles["shown"];
    StringVector fill = new_styles["fill"];
    StringVector outline = new_styles["outline"];
    NumericVector outline_width = new_styles["outline_width"];
    NumericVector size = new_styles["size"];
    NumericVector rotation = new_styles["rotation"];
    NumericVector aspect = new_styles["aspect"];
    StringVector shape = new_styles["shape"];
    LogicalVector label_shown = new_styles["label_shown"];
    NumericVector label_offset_x = new_styles["label_offset_x"];
    NumericVector label_offset_y = new_styles["label_offset_y"];
    NumericVector label_size = new_styles["label_size"];
    StringVector label_color = new_styles["label_color"];
    NumericVector label_rotation = new_styles["label_rotation"];
    StringVector label_slant = new_styles["label_slant"];
    StringVector label_weight = new_styles["label_weight"];
    StringVector label_font_family = new_styles["label_font_family"];
    StringVector label_text = new_styles["label_text"];

    for (auto index : acmacs::range(obj_->number_of_points())) {
        obj_->shown(index, shown[index] == TRUE);
        obj_->fill(index, Color{fill[index]});
        obj_->outline(index, Color{outline[index]});
        obj_->outline_width(index, Pixels{outline_width[index]});
        obj_->size(index, Pixels{size[index]});
        obj_->rotation(index, Rotation{rotation[index]});
        obj_->aspect(index, Aspect{aspect[index]});
        obj_->shape(index, acmacs::PointShape(shape[index]));
        obj_->label_shown(index, label_shown[index]);
        obj_->label_offset_x(index, label_offset_x[index]);
        obj_->label_offset_y(index, label_offset_y[index]);
        obj_->label_size(index, Pixels{label_size[index]});
        obj_->label_color(index, Color{label_color[index]});
        obj_->label_rotation(index, Rotation{label_rotation[index]});
        obj_->label_slant(index, acmacs::FontSlant{label_slant[index]});
        obj_->label_weight(index, acmacs::FontWeight{label_weight[index]});
        obj_->label_font_family(index, static_cast<const char*>(label_font_family[index]));
        obj_->label_text(index, static_cast<const char*>(label_text[index]));
    }

} // PlotSpec::set_styles

// ----------------------------------------------------------------------

Rcpp::IntegerVector PlotSpec::drawing_order() const
{
    const auto drawing_order = obj_->drawing_order();
    Rcpp::IntegerVector result(drawing_order.size());
    std::transform(drawing_order.begin(), drawing_order.end(), result.begin(), [](auto index) { return index + 1; });
    return result;
    // return {drawing_order.begin(), drawing_order.end()};
}

// ----------------------------------------------------------------------

void PlotSpec::drawing_order_raise(const Rcpp::IntegerVector& aIndexes)
{
    acmacs::chart::Indexes indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
    obj_->raise(indexes);
}

// ----------------------------------------------------------------------

void PlotSpec::drawing_order_lower(const Rcpp::IntegerVector& aIndexes)
{
    acmacs::chart::Indexes indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
    obj_->lower(indexes);
}

// ----------------------------------------------------------------------

void PlotSpec::drawing_order_raise_sera(const Rcpp::IntegerVector& aIndexes)
{
    acmacs::chart::Indexes indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
    obj_->raise_serum(indexes);
}

// ----------------------------------------------------------------------

void PlotSpec::drawing_order_lower_sera(const Rcpp::IntegerVector& aIndexes)
{
    acmacs::chart::Indexes indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
    obj_->lower_serum(indexes);
}

// ----------------------------------------------------------------------

void PlotSpec::set_style_size(const Rcpp::IntegerVector& aIndexes, double aSize)
{
    for (auto index : aIndexes)
        obj_->size(index - 1, Pixels{aSize});
}

// ----------------------------------------------------------------------

void PlotSpec::set_style_fill(const Rcpp::IntegerVector& aIndexes, std::string aFill)
{
    for (auto index : aIndexes)
        obj_->fill(index - 1, Color(aFill));
}

void PlotSpec::set_style_fill_opacity(const Rcpp::IntegerVector& aIndexes, double aOpacity)
{
    for (auto index : aIndexes)
        obj_->fill_opacity(index - 1, aOpacity);
}

// ----------------------------------------------------------------------

void PlotSpec::set_style_outline(const Rcpp::IntegerVector& aIndexes, std::string aOutline)
{
    for (auto index : aIndexes)
        obj_->outline(index - 1, Color(aOutline));
}

void PlotSpec::set_style_outline_opacity(const Rcpp::IntegerVector& aIndexes, double aOpacity)
{
    for (auto index : aIndexes)
        obj_->outline_opacity(index - 1, aOpacity);
}

void PlotSpec::set_style_outline_width(const Rcpp::IntegerVector& aIndexes, double aOutlineWidth)
{
    for (auto index : aIndexes)
        obj_->outline_width(index - 1, Pixels(aOutlineWidth));
}

void PlotSpec::set_style_shown(const Rcpp::IntegerVector& aIndexes, bool shown)
{
    for (auto index : aIndexes)
        obj_->shown(index - 1, shown);
}

void PlotSpec::set_style_rotation(const Rcpp::IntegerVector& aIndexes, double rotation)
{
    for (auto index : aIndexes)
        obj_->rotation(index - 1, Rotation{rotation});
}

void PlotSpec::set_style_aspect(const Rcpp::IntegerVector& aIndexes, double aspect)
{
    for (auto index : aIndexes)
        obj_->aspect(index - 1, Aspect{aspect});
}

void PlotSpec::set_style_shape(const Rcpp::IntegerVector& aIndexes, std::string shape)
{
    for (auto index : aIndexes)
        obj_->shape(index - 1, acmacs::PointShape(shape));
}

void PlotSpec::set_style_label_shown(const Rcpp::IntegerVector& aIndexes, bool shown)
{
    for (auto index : aIndexes)
        obj_->label_shown(index - 1, shown);
}

void PlotSpec::set_style_label_offset_x(const Rcpp::IntegerVector& aIndexes, double label_offset_x)
{
    for (auto index : aIndexes)
        obj_->label_offset_x(index - 1, label_offset_x);
}

void PlotSpec::set_style_label_offset_y(const Rcpp::IntegerVector& aIndexes, double label_offset_y)
{
    for (auto index : aIndexes)
        obj_->label_offset_y(index - 1, label_offset_y);
}

void PlotSpec::set_style_label_size(const Rcpp::IntegerVector& aIndexes, double label_size)
{
    for (auto index : aIndexes)
        obj_->label_size(index - 1, Pixels{label_size});
}

void PlotSpec::set_style_label_color(const Rcpp::IntegerVector& aIndexes, std::string label_color)
{
    for (auto index : aIndexes)
        obj_->label_color(index - 1, Color{label_color});
}

void PlotSpec::set_style_label_rotation(const Rcpp::IntegerVector& aIndexes, double rotation)
{
    for (auto index : aIndexes)
        obj_->label_rotation(index - 1, Rotation{rotation});
}

void PlotSpec::set_style_label_slant(const Rcpp::IntegerVector& aIndexes, std::string label_slant)
{
    for (auto index : aIndexes)
        obj_->label_slant(index - 1, acmacs::FontSlant{label_slant});
}

void PlotSpec::set_style_label_weight(const Rcpp::IntegerVector& aIndexes, std::string label_weight)
{
    for (auto index : aIndexes)
        obj_->label_weight(index - 1, acmacs::FontWeight{label_weight});
}

void PlotSpec::set_style_label_font_family(const Rcpp::IntegerVector& aIndexes, std::string font_family)
{
    for (auto index : aIndexes)
        obj_->label_font_family(index - 1, font_family);
}

void PlotSpec::set_style_label_text(const Rcpp::IntegerVector& aIndexes, std::string label_text)
{
    for (auto index : aIndexes)
        obj_->label_text(index - 1, label_text);
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
