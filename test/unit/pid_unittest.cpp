#include <gtest/gtest.h>
#include "pid.h"

TEST(PIDTest, Initialization) {
    pid_controller_t pid;
    pid_init(&pid, 1.0, 0.1, 0.01, -100, 100);
    EXPECT_DOUBLE_EQ(pid.Kp, 1.0);
    EXPECT_DOUBLE_EQ(pid.Ki, 0.1);
    EXPECT_DOUBLE_EQ(pid.Kd, 0.01);
    EXPECT_DOUBLE_EQ(pid.min_output, -100);
    EXPECT_DOUBLE_EQ(pid.max_output, 100);
}

TEST(PIDTest, Compute_StableResponse) {
    pid_controller_t pid;
    pid_init(&pid, 1.0, 0.1, 0.01, -100, 100);
    pid_setpoint(&pid, 50.0);
    
    double output = 0.0;
    for (int i = 0; i < 50; i++) {
        output = pid_compute(&pid, output, 0.1);
    }
    EXPECT_NEAR(output,
         50.0, 1.0);
}

TEST(PIDTest, OutputLimit) {
    pid_controller_t pid;
    pid_init(&pid, 2.0, 0.1, 0.01, -10, 10);
    pid_setpoint(&pid, 100.0);
    double output = pid_compute(&pid, 0.0, 0.1);
    EXPECT_LE(output, 10.0);
    EXPECT_GE(output, -10.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
