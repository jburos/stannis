

#' Create margin labels following a pattern
#'
#' @param A array to create labels for (or matrix).
#' @return list of margin labels
#' @export
name_margins = function(A) {
  n_dim = dim(A) %>% length()
  l = list()
  groups = paste0("group_", 1:(n_dim - 1))
  for (i in 1:length(groups)) {
    group = groups[i]
    l[[group]] = as.character(1:(dim(A)[i + 1]))
  }
  return(l)
}

#' Create labelled samples.
#'
#' @param array array of samples extracted from sample file set
#' @param labels list with a named elements for each array margin
#'        and each element is a data frame of groupings to join with
#'        that margin's index. 
#' @return list with a component for a matrix of values (one group
#'         per row, one iteration per column), and a data.frame
#'         with string labels for each row.
#' @export
label = function(A, labels = NULL) {
  n_dim = dim(A) %>% length
  margins = name_margins(A)
  lf = matrix(data = A, ncol = dim(A)[1], byrow=TRUE)
  dimnames(lf) = list(group = 1:nrow(lf), iteration = 1:ncol(lf))
  grouping = do.call(what = expand.grid, 
    args = c(margins, list(stringsAsFactors=FALSE)))
  if (!is.null(labels)) {
    for (group in names(grouping)) {
      grouping = dplyr::left_join(grouping, labels[[group]], by = group)
    }
  }
  return(list(values = lf, grouping = grouping))  ## FIXME: Future proper type.
}

#' Summarize iterations to estimates!
#'
#' @param x list from with named elements 'values' and 'grouping'.  The 'values' 
#'          element is a matrix (n-groups by n-iterations) with sampled
#'          parameter values.  The 'grouping' element is a data.frame with 
#'          n-groups rows and one column per index.
#' @param f function to use to aggregate (applied row-wise)
#' @return data frame (further merging of estimates is dissalowed due to
#'         "transform then summarize" pattern.
#' @export
summarize = function(x, f, ...) {
  x[['values']] <- apply(x[['values']], 1, f, ...) %>% t
  x <- data.frame(x[['grouping']], x[['values']], check.names=FALSE)
  return(x)
}

#' Standard summary function.
#'
#' @param x per-iteration parameter values.
#' @return per-estimate summary.
#' @export 
std_estimates <- function(x) {
  x = c(quantile(x, probs=0.025), mean(x), quantile(x, probs=0.975))
  names(x) <- c('lb', 'estimate', 'ub')
  attr(x, 'summaries') <- c('2.5%', 'mean', '97.5%')
  return(x)
}


