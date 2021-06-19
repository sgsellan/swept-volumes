#ifndef SWEPT_VOLUME_FUN
#define SWEPT_VOLUME_FUN
#include <Eigen/Core>
#include <vector>

void swept_volume(const Eigen::MatrixXd & V, const Eigen::MatrixXi & F, const Eigen::MatrixXd & UV, const Eigen::MatrixXi & UVF, const std::vector<Eigen::Matrix4d> Transformations, const double eps, const int num_seeds, const std::string dir_name, Eigen::MatrixXd & U, Eigen::MatrixXi & G, std::vector<Eigen::MatrixXd> & strobo_V_list, std::vector<Eigen::MatrixXi> & strobo_F_list);

void swept_volume(const Eigen::MatrixXd & V, const Eigen::MatrixXi & F, const std::vector<Eigen::Matrix4d> Transformations, const double eps, const int num_seeds, const std::string dir_name, Eigen::MatrixXd & U, Eigen::MatrixXi & G, std::vector<Eigen::MatrixXd> & strobo_V_list, std::vector<Eigen::MatrixXi> & strobo_F_list);

#endif
