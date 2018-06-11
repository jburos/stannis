#include <fstream>

#include <zoom.cpp>
#include <Rcpp.h>

/* Function to call from R to get header and parameter sample
 * in a list.
 *
 * @param 
 */
// [[Rcpp::export]]
Rcpp::List read_cmdstan_csv(Rcpp::StringVector file) {
  header_t header;
  parameter_t parameters;
  std::ifstream f(file[0]);
  Rcpp::Rcout << f.fail() << std::endl;
  Rcpp::Rcout << f.bad() << std::endl;
  Rcpp::Rcout << f.good() << std::endl;
  std::tie(header, parameters) = read_samples(f);
  return Rcpp::List::create(
    Rcpp::Named("n_col") = std::get<0>(header),
    Rcpp::Named("n_parameters") = std::get<1>(header),
    Rcpp::Named("p_names") = std::get<2>(header),
    Rcpp::Named("n_dim") = std::get<3>(header),
    Rcpp::Named("dimensions") = std::get<4>(header),
    Rcpp::Named("index") = std::get<5>(header),
    Rcpp::Named("parameters") = parameters);
}

