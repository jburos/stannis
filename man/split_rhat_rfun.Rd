% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/diagnostics.R
\name{split_rhat_rfun}
\alias{split_rhat_rfun}
\title{rstan's split-r-hat implementation in R}
\usage{
split_rhat_rfun(sims)
}
\arguments{
\item{sims}{a 2-d array _without_ warmup samples (# iter * # chains)}
}
\value{
a single split-r-hat value.
}
\description{
Compute the split rhat for the diagnostics of converging; 
see the C++ code of split_potential_scale_reduction in chains.cpp.
}
\details{
Note: 
  The R function wrapping the C++ implementation is defined 
  in chains.R with name rstan_splitrhat2_cpp
}
