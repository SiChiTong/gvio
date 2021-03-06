#include "gvio/munit.hpp"
#include "gvio/msckf/imu_state.hpp"
#include "gvio/control/carrot_controller.hpp"
#include "gvio/quadrotor/quadrotor_model.hpp"

namespace gvio {

#define TEST_MISSION "test_configs/missions/mission_local.yaml"

int test_QuadrotorModel_constructor() {
  QuadrotorModel quad;
  return 0;
}

int setup_output_files(std::ofstream &gnd_file,
                       std::ofstream &mea_file,
                       std::ofstream &est_file) {
  // Ground truth file
  const std::string gnd_file_path = "/tmp/quadrotor_gnd.dat";
  gnd_file.open(gnd_file_path);
  if (gnd_file.good() == false) {
    LOG_ERROR("Failed to open ground truth file for recording [%s]",
              gnd_file_path.c_str());
    return -1;
  }

  // Measurement file
  const std::string mea_file_path = "/tmp/quadrotor_mea.dat";
  mea_file.open(mea_file_path);
  if (mea_file.good() == false) {
    LOG_ERROR("Failed to open measurement file for recording [%s]",
              mea_file_path.c_str());
    return -1;
  }

  // Estimate file
  const std::string est_file_path = "/tmp/quadrotor_est.dat";
  est_file.open(est_file_path);
  if (est_file.good() == false) {
    LOG_ERROR("Failed to open estimate file for recording [%s]",
              est_file_path.c_str());
    return -1;
  }

  // Write header
  const std::string gnd_header = "t,x,y,z,vx,vy,vz,roll,pitch,yaw";
  gnd_file << gnd_header << std::endl;
  const std::string mea_header = "t,ax_B,ay_B,az_B,wx_B,wy_B,wz_B";
  mea_file << mea_header << std::endl;
  const std::string est_header = "t,x,y,z,vx,vy,vz,roll,pitch,yaw";
  est_file << est_header << std::endl;

  return 0;
}

void record_timestep(const double t,
                     const QuadrotorModel &quad,
                     const IMUState &imu,
                     std::ofstream &gnd_file,
                     std::ofstream &mea_file,
                     std::ofstream &est_file) {
  // Record quadrotor ground truth
  // -- Time
  gnd_file << t << ",";
  // -- Position
  gnd_file << quad.p_G(0) << ",";
  gnd_file << quad.p_G(1) << ",";
  gnd_file << quad.p_G(2) << ",";
  // -- Velocity
  gnd_file << quad.v_G(0) << ",";
  gnd_file << quad.v_G(1) << ",";
  gnd_file << quad.v_G(2) << ",";
  // -- Attitude
  gnd_file << quad.rpy_G(0) << ",";
  gnd_file << quad.rpy_G(1) << ",";
  gnd_file << quad.rpy_G(2) << std::endl;

  // Record quadrotor body acceleration and angular velocity
  // -- Time
  mea_file << t << ",";
  // -- Body acceleration
  mea_file << quad.a_B(0) << ",";
  mea_file << quad.a_B(1) << ",";
  mea_file << quad.a_B(2) << ",";
  // -- Body angular velocity
  mea_file << quad.w_B(0) << ",";
  mea_file << quad.w_B(1) << ",";
  mea_file << quad.w_B(2) << std::endl;

  // Record estimate
  // -- Time
  est_file << t << ",";
  // -- Position
  est_file << imu.p_G(0) << ",";
  est_file << imu.p_G(1) << ",";
  est_file << imu.p_G(2) << ",";
  // -- Velocity
  est_file << imu.v_G(0) << ",";
  est_file << imu.v_G(1) << ",";
  est_file << imu.v_G(2) << ",";
  // -- Attitude
  const Vec3 rpy = quat2euler(imu.q_IG);
  est_file << rpy(0) << ",";
  est_file << rpy(1) << ",";
  est_file << rpy(2) << std::endl;
}

int test_QuadrotorModel_update() {
  // Setup output files
  std::ofstream gnd_file;
  std::ofstream mea_file;
  std::ofstream est_file;

  int retval = setup_output_files(gnd_file, mea_file, est_file);
  if (retval != 0) {
    LOG_ERROR("Failed to setup output files!");
    return -1;
  }

  // Setup quadrotor model
  QuadrotorModel quad(Vec3{0.0, 0.0, 0.0}, Vec3{0.0, 0.0, 5.0});
  // if (quad.loadMission(TEST_MISSION) != 0) {
  //   LOG_ERROR("Failed to load mission!");
  //   return -1;
  // }

  // Setup carrot controller
  CarrotController controller;
  std::vector<Vec3> waypoints;
  waypoints.emplace_back(0.0, 0.0, 5.0);
  waypoints.emplace_back(5.0, 0.0, 5.0);
  waypoints.emplace_back(10.0, 1.0, 5.0);
  // waypoints.emplace_back(0.0, 5.0, 5.0);
  // waypoints.emplace_back(0.0, 0.0, 5.0);
  controller.configure(waypoints, 0.5);

  // Setup IMU state
  IMUState imu;
  imu.g_G = Vec3{0.0, 0.0, -quad.g};
  imu.p_G = quad.p_G;
  imu.q_IG = euler2quat(quad.rpy_G);

  // Record initial quadrotor state
  record_timestep(0.0, quad, imu, gnd_file, mea_file, est_file);

  quad.setPosition(Vec3{10, 10, 5.0});

  // Simulate
  double imu_dt = 0.01;
  const double dt = 0.001;
  for (double t = 0.0; t <= 30.0; t += dt) {
    // Calculate carrot point
    // Vec3 carrot_pt;
    // controller.update(quad.p_G, carrot_pt);

    // Update quadrotor model
    quad.update(dt);

    // Update imu
    const Vec3 a_m = quad.a_B;
    const Vec3 w_m = quad.w_B;

    imu_dt += dt;
    if (imu_dt > 0.1) {
      imu.update(a_m, w_m, imu_dt);
      imu_dt = 0.0;
    }

    // Record
    record_timestep(t, quad, imu, gnd_file, mea_file, est_file);
  }
  gnd_file.close();
  mea_file.close();
  est_file.close();

  // Plot quadrotor trajectory
  PYTHON_SCRIPT("scripts/plot_quadrotor.py "
                "/tmp/quadrotor_gnd.dat "
                "/tmp/quadrotor_mea.dat "
                "/tmp/quadrotor_est.dat");

  return 0;
}

void test_suite() {
  MU_ADD_TEST(test_QuadrotorModel_constructor);
  MU_ADD_TEST(test_QuadrotorModel_update);
}

} // namespace gvio

MU_RUN_TESTS(gvio::test_suite);
