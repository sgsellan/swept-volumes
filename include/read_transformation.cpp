#include <igl/readDMAT.h>
#include <Eigen/Core>




void read_transformation(const std::string filename, std::vector<Eigen::Matrix4d> & Transformations){
    Eigen::MatrixXd bigT;
    igl::readDMAT(filename,bigT);
    for (int i = 0; i<(bigT.rows()/4); i++) {
        Transformations.push_back(bigT.block(4*i,0,4,4));
    }
}
