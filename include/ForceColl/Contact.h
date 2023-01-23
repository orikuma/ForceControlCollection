#pragma once

#include <unordered_map>

#include <mc_rtc/Configuration.h>
#include <mc_rtc/gui/StateBuilder.h>
#include <SpaceVecAlg/SpaceVecAlg>

namespace ForceColl
{
/** \brief Friction pyramid. */
class FrictionPyramid
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  /** \brief Constructor.
      \param fricCoeff friction coefficient
      \param ridgeNum number of ridges of friction pyramid
   */
  FrictionPyramid(double fricCoeff, int ridgeNum = 4);

  /** \brief Calculate ridge vector list in global coordinates. */
  std::vector<Eigen::Vector3d> calcGlobalRidgeList(const Eigen::Matrix3d & rot) const;

  /** \brief Number of ridges. */
  inline int ridgeNum() const
  {
    return static_cast<int>(localRidgeList_.size());
  }

public:
  //! Local ridge list
  std::vector<Eigen::Vector3d> localRidgeList_;
};

/** \brief Contact constraint. */
class Contact
{
public:
  /** \brief Vertex with ridges. */
  struct VertexWithRidge
  {
    //! Vertex
    Eigen::Vector3d vertex;

    //! Global ridge list
    std::vector<Eigen::Vector3d> ridgeList;

    /** \brief Constructor.
        \param _vertex vertex
        \param _ridgeList global ridge list
     */
    VertexWithRidge(const Eigen::Vector3d & _vertex, const std::vector<Eigen::Vector3d> & _ridgeList)
    : vertex(_vertex), ridgeList(_ridgeList)
    {
    }
  };

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  /** \brief Constructor.
      \param name name of contact
      \param fricCoeff friction coefficient
      \param localVertexList vertices of surface in local coordinates
      \param pose pose of contact
   */
  Contact(const std::string & name,
          double fricCoeff,
          const std::vector<Eigen::Vector3d> & localVertexList,
          const sva::PTransformd & pose);

  /** \brief Calculate wrench.
      \param wrenchRatio wrench ratio of each ridge
      \param momentOrigin moment origin
      \returns contact wrench
   */
  sva::ForceVecd calcWrench(const Eigen::VectorXd & wrenchRatio,
                            const Eigen::Vector3d & momentOrigin = Eigen::Vector3d::Zero()) const;

  /** \brief Add markers to GUI.
      \param gui GUI
      \param category category of GUI entries
      \param wrenchRatio wrench ratio of each ridge
      \param forceScale scale of force markers (set non-positive for no visualization)
      \param fricPyramidScale scale of friction pyramid markers (set non-positive for no visualization)
   */
  void addToGUI(mc_rtc::gui::StateBuilder & gui,
                const std::vector<std::string> & category,
                const Eigen::VectorXd & wrenchRatio,
                double forceScale = 2e-3,
                double fricPyramidScale = 5e-2);

public:
  //! Name of contact
  std::string name_;

  //! Grasp matrix
  Eigen::Matrix<double, 6, Eigen::Dynamic> graspMat_;

  //! List of vertex with ridges
  std::vector<VertexWithRidge> vertexWithRidgeList_;
};
} // namespace ForceColl
