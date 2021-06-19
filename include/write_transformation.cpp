#include <igl/writeDMAT.h>
#include <Eigen/Core>




void write_transformation(const std::string filename, const std::vector<Eigen::Matrix4d> Transformations){
    Eigen::MatrixXd bigT;
    bigT.resize(4*Transformations.size(),4);
    for (int i = 0; i<Transformations.size(); i++) {
        bigT.block(4*i,0,4,4) = Transformations[i];
    }
    igl::writeDMAT(filename,bigT);
}
