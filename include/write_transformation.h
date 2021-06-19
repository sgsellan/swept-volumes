#ifndef WRITE_TRANSF
#define WRITE_TRANSF
#include <Eigen/Core>
#include <vector>

void write_transformation(const std::string filename, const std::vector<Eigen::Matrix4d> Transformations);


#endif
