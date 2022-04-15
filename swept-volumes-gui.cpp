#include <igl/opengl/glfw/Viewer.h>
#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <igl/signed_distance.h>
#include <igl/sparse_voxel_grid.h>
#include <igl/copyleft/marching_cubes.h>
#include <igl/fast_winding_number.h>
#include <igl/winding_number.h>
#include <igl/writeDMAT.h>
#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuizmoPlugin.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imguizmo/ImGuizmo.h>
#include "swept_volume.h"
#include "read_transformation.h"
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>

int main(int argc, char *argv[])
{
    // Inline mesh of a cube
    Eigen::MatrixXd V,U;
    Eigen::MatrixXi F,G;
    Eigen::MatrixXd swept_verts;
    Eigen::MatrixXi swept_faces;
    Eigen::MatrixXd sv_10,sv_100,sv_1000;
    Eigen::MatrixXi sf_10,sf_100,sf_1000;
    std::vector<Eigen::MatrixXi> strobo_F_list;
    std::vector<Eigen::MatrixXd> strobo_V_list;
    int strobo_ind = 0;
    std::vector<Eigen::Matrix4d> Transformations;
    std::string input = "data/bunny.obj";
    double eps = 0.02;
    int num_seeds = 100;
    int argindex = 0;
    //    else{
    //        std::cout<<R"(
    //No input specified. To run in command line, issue
    //
    //./swept-volume [input.ext] [-e epsilon] [-n num_seeds]
    //
    //The mesh in input.ext must be closed and manifold,
    //and ext can be either of obj, ply, off, stl or mesh.
    //)";
    //        return argindex;
    //    }
    while(argindex+1<argc){
        if(strncmp(argv[argindex+1],"-e",2)==0){
            eps = atof(argv[argindex+2]);
            argindex = argindex+2;
        }else if(strncmp(argv[argindex+1],"-n",2)==0){
            num_seeds = atoi(argv[argindex+2]);
            argindex = argindex+2;
        }else if(strncmp(argv[argindex+1],"-i",2)==0){
            input = argv[argindex+2];
            argindex = argindex+2;
        }else if(strncmp(argv[argindex+1],"-load",2)==0){
            std::string dir_name = argv[argindex+2];
            read_transformation(dir_name + "transformations.dmat",Transformations);
            igl::read_triangle_mesh(dir_name + "ours.obj",swept_verts,swept_faces);
            igl::read_triangle_mesh(dir_name + "strobo_10.obj",sv_10,sf_10);
            strobo_V_list.push_back(sv_10);
            strobo_F_list.push_back(sf_10);
            igl::read_triangle_mesh(dir_name + "strobo_100.obj",sv_100,sf_100);
            strobo_V_list.push_back(sv_100);
            strobo_F_list.push_back(sf_100);
            input = dir_name + "input.obj";
            argindex = argindex+2;
        }
    }
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%H-%M");
    std::string dir_name = "data/experiments/" + oss.str();
    
    igl::read_triangle_mesh(input,V,F);
    // NORMALIZE
    Eigen::MatrixXd V_normalized;
    int n = V.rows();
    V_normalized.resize(n,3);
    V_normalized.col(0) = V.col(0) - Eigen::VectorXd::Constant(n,V.col(0).minCoeff());
    V_normalized.col(1) = V.col(1) - Eigen::VectorXd::Constant(n,V.col(1).minCoeff());
    V_normalized.col(2) = V.col(2) - Eigen::VectorXd::Constant(n,V.col(2).minCoeff());
    V_normalized = V_normalized/(1.3*V_normalized.maxCoeff());
    V_normalized.col(0) = V_normalized.col(0) + Eigen::VectorXd::Constant(n,-0.5);
    V_normalized.col(1) = V_normalized.col(1) + Eigen::VectorXd::Constant(n,-0.5);
    V_normalized.col(2) = V_normalized.col(2) + Eigen::VectorXd::Constant(n,-0.5);
    V = V_normalized;
    
    
    
    // Set up viewer
    igl::opengl::glfw::Viewer vr;
    std::vector<int> indeces;
    const int xid = vr.selected_data_index;
    indeces.push_back(xid);
    vr.append_mesh();
    vr.data_list[xid].set_mesh(V,F);
    // Custom menu
    igl::opengl::glfw::imgui::ImGuizmoPlugin plugin;
    vr.plugins.push_back(&plugin);
    // Initialize ImGuizmo at mesh centroid
    plugin.T.block(0,3,3,1) =
    0.5*(V.colwise().maxCoeff() + V.colwise().minCoeff()).transpose().cast<float>();
    // Update can be applied relative to this remembered initial transform
    const Eigen::Matrix4f T0 = plugin.T;
    // Attach callback to apply imguizmo's transform to mesh
    plugin.callback = [&](const Eigen::Matrix4f & T)
    {
        const Eigen::Matrix4d TT = (T*T0.inverse()).cast<double>().transpose();
        vr.data_list[xid].set_vertices(
                                       (V.rowwise().homogeneous()*TT).rowwise().hnormalized());
        vr.data_list[xid].compute_normals();
    };
    // Maya-style keyboard shortcuts for operation
    vr.callback_key_pressed = [&](decltype(vr) &,unsigned int key, int mod)
    {
        switch(key)
        {
            case 'W': case 'w': plugin.operation = ImGuizmo::TRANSLATE; return true;
            case 'E': case 'e': plugin.operation = ImGuizmo::ROTATE;    return true;
            case 'R': case 'r': plugin.operation = ImGuizmo::SCALE;     return true;
            case 'S': case 's':
                swept_volume(V,F,Transformations,eps,num_seeds,dir_name,swept_verts,swept_faces,strobo_V_list,strobo_F_list);
                for (int indd = 0; indd < indeces.size(); indd++) {
                    vr.data_list[indd].clear();
                }
                indeces.resize(0);
                vr.data().set_mesh(swept_verts,swept_faces);
                return true;
            case 'V': case 'v':
                strobo_ind = (strobo_ind + 1)%strobo_V_list.size();
                for (int indd = 0; indd < indeces.size(); indd++) {
                    vr.data_list[indd].clear();
                }
                indeces.resize(0);
                vr.data().clear();
                vr.data().set_mesh(strobo_V_list[strobo_ind],strobo_F_list[strobo_ind]);
                return true;
            case 'C': case 'c':
                for (int indd = 0; indd < indeces.size(); indd++) {
                    vr.data_list[indd].clear();
                }
                indeces.resize(0);
                vr.data().clear();
                vr.data().set_mesh(swept_verts,swept_faces);
                return true;
            case 'B': case 'b':
                Eigen::Matrix4d TT = (plugin.T*T0.inverse()).cast<double>().transpose();
                Transformations.push_back(TT.transpose());
                int yid = vr.selected_data_index;
                vr.data_list[yid].set_mesh((V.rowwise().homogeneous()*TT).rowwise().hnormalized(),F);
                indeces.push_back(yid);
                vr.append_mesh();
                return true;
                
        }
        return false;
    };
    std::cout<<R"(
%%%%%%%% SWEPT VOLUMES GUI INSTRUCTIONS  %%%%%%%%%%%%%
W,w  Switch to translate operation
E,e  Switch to rotate operation
R,r  Switch to scale operation
B,b  Bake in current position as new keyframe
S,s  Calculate swept volume (will show our output)
V,v  Flip between strobo outputs (after computation)
C,c  Show our output (after computation)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    )";
    
    if (Transformations.size()>0) {
        for (int tt = 0 ; tt<Transformations.size(); tt++) {
            Eigen::Matrix4d TT = Transformations[tt].transpose();
            //Transformations.push_back(TT.transpose()); //transposed because we use left-multiplication
            int yid = vr.selected_data_index;
            vr.data_list[yid].set_mesh((V.rowwise().homogeneous()*TT).rowwise().hnormalized(),F);
            indeces.push_back(yid);
            vr.append_mesh();
        }
    }
    
    vr.launch();
}
