#ifndef READ_TRANSF
#define READ_TRANSF
#include <Eigen/Core>
#include <vector>

void read_transformation(const std::string filename, std::vector<Eigen::Matrix4d> & Transformations);


#endif
