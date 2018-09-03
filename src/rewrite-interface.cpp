#include <stannis/exporter.hpp>
#include <stannis/rewrite-stan-csv.hpp>

#include <stannis/read-header-data.hpp>
#include <stannis/read-dimension-data.hpp>
#include <stannis/read-name-data.hpp>
#include <stannis/read-parameter-data.hpp>

#include <Rcpp.h>

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <string>
#include <fstream>

RcppExport SEXP hash_to_uuid(SEXP hash_) {
  std::vector<std::string> hashes = Rcpp::as<std::vector<std::string>>(hash_);
  std::string tag_string;
  for (auto & s : hashes) 
    tag_string.append(s);
  boost::uuids::uuid tag = boost::uuids::name_generator_sha1(tag_string);
  return Rcpp::wrap(tag);
}

RcppExport SEXP rewrite_stan_csv(SEXP source_, SEXP root_, SEXP tag_, SEXP comment_
) {
  boost::filesystem::path source = Rcpp::as<boost::filesystem::path>(source_);
  boost::filesystem::path root = Rcpp::as<boost::filesystem::path>(root_);
  boost::uuids::uuid tag = Rcpp::as<boost::uuids::uuid>(tag_);
  std::string comment = Rcpp::as<std::string>(comment_);
  bool complete = stannis::rewrite_stan_csv(source, root, tag, comment);
  Rcpp::RObject result = Rcpp::wrap(complete);
  return result;
}

RcppExport SEXP get_dimensions(SEXP dim_path_, SEXP name_path_) {
  boost::filesystem::path dim_path = Rcpp::as<boost::filesystem::path>(dim_path_);
  boost::filesystem::path name_path = Rcpp::as<boost::filesystem::path>(name_path_);
  std::vector<std::vector<uint>> dims = stannis::get_dimensions(dim_path);
  std::vector<std::string> names = stannis::get_names(name_path);
  Rcpp::List result;
  for (int i = 0; i < names.size(); ++i)
    result.push_back(dims[i], names[i]);
  return result;
}

RcppExport SEXP get_parameter(SEXP root_, SEXP name_) {
  std::string name = Rcpp::as<std::vector<std::string>>(name_)[0];
  boost::filesystem::path root = Rcpp::as<boost::filesystem::path>(root_);
  boost::filesystem::path dim_path(root);
  dim_path /= name + "-dimensions.bin";
  std::vector<uint> dims = stannis::get_reshape_dimensions(dim_path);
  std::vector<double> draws = stannis::get_draws(root /= name + "-reshape.bin");
  return Rcpp::List::create(
    Rcpp::Named("data") = draws,
    Rcpp::Named("dims") = dims
  );
}

static const R_CallMethodDef CallEntries[] = {
    {"rewrite_stan_csv", (DL_FUNC) &rewrite_stan_csv, 4},
    {"get_dimensions", (DL_FUNC) &get_dimensions, 2},
    {"get_parameter", (DL_FUNC) &get_parameter, 2},
    {NULL, NULL, 0}
};

RcppExport void R_init_stannis(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}


