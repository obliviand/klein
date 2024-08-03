#include <doctest/doctest.h>

#include <klein/klein.hpp>

using namespace kln;

TEST_CASE("measure-point-to-point")
{
    point p1{1.f, 0, 0};
    point p2{0.f, 1.f, 0};
    line l = p1 & p2;
    // Produce the squared distance between p1 and p2
    CHECK_EQ(l.squared_norm(), 2.f);
}

TEST_CASE("measure-point-to-plane")
{
    //    Plane p2
    //    /
    //   / \ line perpendicular to
    //  /   \ p2 through p1
    // 0------x--------->
    //        p1

    // (2, 0, 0)
    point p1{2.f, 0.f, 0.f};
    // Plane x - y = 0
    plane p2{1.f, -1.f, 0.f, 0.f};
    p2.normalize();
    // Distance from point p1 to plane p2
    auto root_two = doctest::Approx(std::sqrt(2.f));
    CHECK_EQ(std::abs((p1 & p2).scalar()), root_two);
    CHECK_EQ(std::abs((p1 ^ p2).e0123()), root_two);
}

TEST_CASE("plane-normalize")
{
    // d*e_0 + a*e_1 + b*e_2 + c*e_3
    plane p{1.f, 2.f, 3.f, 4.f};

    p.normalize();

    CHECK_EQ(p.e0(), doctest::Approx(1.069f).epsilon(0.001));
    CHECK_EQ(p.e1(), doctest::Approx(0.267f).epsilon(0.001));
    CHECK_EQ(p.e2(), doctest::Approx(0.534f).epsilon(0.001));
    CHECK_EQ(p.e3(), doctest::Approx(0.801f).epsilon(0.001));
}

TEST_CASE("normalized-plane-normalization")
{
    point a{-0.5, 2, -0.5};
    point b{+0.5, 2, -0.5};
    point c{+0.5, 2, +0.5};

    // p is already normalized
    plane p = a & b & c;
    CHECK_EQ(p.norm(), 1);
    CHECK_EQ(p.e0(), -2);
    CHECK_EQ(p.e1(), 0);
    CHECK_EQ(p.e2(), 1);
    CHECK_EQ(p.e3(), 0);

    p.normalize();
    // same checks as before
    CHECK_EQ(p.norm(), 1);
    CHECK_EQ(p.e0(), -2);
    CHECK_EQ(p.e1(), 0);
    CHECK_EQ(p.e2(), 1);
    CHECK_EQ(p.e3(), 0);
}

TEST_CASE("measure-point-to-line")
{
    line l{0, 1, 0, 1, 0, 0};
    point p{0, 1, 2};
    float distance = plane{l & p}.norm();
    CHECK_EQ(distance, doctest::Approx(std::sqrt(2.f)));
}

TEST_CASE("euler-angles")
{
    // Make 3 rotors about the x, y, and z-axes.
    rotor rx{1.f, 1.f, 0.f, 0.f};
    rotor ry{1.f, 0.f, 1.f, 0.f};
    rotor rz{1.f, 0.f, 0.f, 1.f};

    rotor r = rx * ry * rz;
    auto ea = r.as_euler_angles();
    CHECK_EQ(ea.roll, doctest::Approx(1.f));
    CHECK_EQ(ea.pitch, doctest::Approx(1.f));
    CHECK_EQ(ea.yaw, doctest::Approx(1.f));

    rotor r2{ea};

    float buf[8];
    r.store(buf);
    r2.store(buf + 4);
    for (size_t i = 0; i != 4; ++i)
    {
        CHECK_EQ(buf[i], doctest::Approx(buf[i + 4]));
    }
}

TEST_CASE("euler-angles-precision")
{
    euler_angles ea1{pi * 0.2f, pi * 0.2f, 0.f};
    rotor r1{ea1};
    euler_angles ea2 = r1.as_euler_angles();

    CHECK_EQ(ea1.roll, doctest::Approx(ea2.roll));
    CHECK_EQ(ea1.pitch, doctest::Approx(ea2.pitch));
    CHECK_EQ(ea1.yaw, doctest::Approx(ea2.yaw));
}