#include <catch2/catch_test_macros.hpp>
#include <bitforge/hardware/mux.hpp>

using namespace bitforge::hardware;

TEST_CASE("Mux selects input by sel", "[mux]")
{
  REQUIRE(Mux(false, false, false) == false);
  REQUIRE(Mux(true, false, false) == true);
  REQUIRE(Mux(false, true, false) == false);
  REQUIRE(Mux(true, true, false) == true);

  REQUIRE(Mux(false, false, true) == false);
  REQUIRE(Mux(true, false, true) == false);
  REQUIRE(Mux(false, true, true) == true);
  REQUIRE(Mux(true, true, true) == true);
}

TEST_CASE("Mux16 selects input by sel", "[mux]")
{
  Bits16 a{};
  Bits16 b{};
  a[0] = true;
  b[15] = true;

  auto s0 = Mux16(a, b, false);
  REQUIRE(s0[0] == true);
  REQUIRE(s0[15] == false);

  auto s1 = Mux16(a, b, true);
  REQUIRE(s1[0] == false);
  REQUIRE(s1[15] == true);
}

TEST_CASE("Mux4Way16 picks one of four", "[mux]")
{
  Bits16 a{}, b{}, c{}, d{};
  a[0] = true;
  b[1] = true;
  c[2] = true;
  d[3] = true;

  // sel2 = {sel[0] , sel[1]} -> index 0..3
  auto r0 = Mux4Way16(a, b, c, d, {false, false});
  REQUIRE(r0[0] == true);

  auto r1 = Mux4Way16(a, b, c, d, {true, false});
  REQUIRE(r1[1] == true);

  auto r2 = Mux4Way16(a, b, c, d, {false, true});
  REQUIRE(r2[2] == true);

  auto r3 = Mux4Way16(a, b, c, d, {true, true});
  REQUIRE(r3[3] == true);
}

TEST_CASE("Mux8Way16 picks one of eight", "[mux]")
{
  Bits16 a{}, b{}, c{}, d{}, e{}, f{}, g{}, h{};
  a[0] = true;
  b[1] = true;
  c[2] = true;
  d[3] = true;
  e[4] = true;
  f[5] = true;
  g[6] = true;
  h[7] = true;

  // sel3 = {sel[0], sel[1], sel[2]} -> index 0..7
  REQUIRE(Mux8Way16(a, b, c, d, e, f, g, h, {false, false, false})[0] == true);
  REQUIRE(Mux8Way16(a, b, c, d, e, f, g, h, {true, false, false})[1] == true);
  REQUIRE(Mux8Way16(a, b, c, d, e, f, g, h, {false, true, false})[2] == true);
  REQUIRE(Mux8Way16(a, b, c, d, e, f, g, h, {true, true, false})[3] == true);
  REQUIRE(Mux8Way16(a, b, c, d, e, f, g, h, {false, false, true})[4] == true);
  REQUIRE(Mux8Way16(a, b, c, d, e, f, g, h, {true, false, true})[5] == true);
  REQUIRE(Mux8Way16(a, b, c, d, e, f, g, h, {false, true, true})[6] == true);
  REQUIRE(Mux8Way16(a, b, c, d, e, f, g, h, {true, true, true})[7] == true);
}

TEST_CASE("Dmux routes input to one of two outputs", "[mux]")
{
  // sel=0 -> out a = in, out b = 0
  auto r0 = Dmux(true, false);
  REQUIRE(r0.first == true);
  REQUIRE(r0.second == false);

  // sel=1 -> out a = 0, out b = in
  auto r1 = Dmux(true, true);
  REQUIRE(r1.first == false);
  REQUIRE(r1.second == true);

  // input false -> both outputs false
  auto r2 = Dmux(false, true);
  REQUIRE(r2.first == false);
  REQUIRE(r2.second == false);
}

TEST_CASE("Dmux4Way routes input to one of four outputs", "[mux]")
{
  // in = false -> all outputs false
  auto r0 = Dmux4Way(false, {false, false});
  REQUIRE(r0[0] == false);
  REQUIRE(r0[1] == false);
  REQUIRE(r0[2] == false);
  REQUIRE(r0[3] == false);

  // in = true, sel = {s1, s0}
  auto r1 = Dmux4Way(true, {false, false});
  REQUIRE(r1[0] == true);
  REQUIRE(r1[3] == false);

  auto r2 = Dmux4Way(true, {true, false});
  REQUIRE(r2[1] == true);
  REQUIRE(r2[0] == false);

  auto r3 = Dmux4Way(true, {false, true});
  REQUIRE(r3[2] == true);

  auto r4 = Dmux4Way(true, {true, true});
  REQUIRE(r4[3] == true);
}

TEST_CASE("Dmux8Way routes input to one of eight outputs", "[mux]")
{
  // in = false -> all outputs false
  auto r0 = Dmux8Way(false, {false, false, false});
  for (int i = 0; i < 8; ++i)
    REQUIRE(r0[i] == false);

  // sel3 = {s0, s1, s2} -> index 0..7
  REQUIRE(Dmux8Way(true, {false, false, false})[0] == true);
  REQUIRE(Dmux8Way(true, {true, false, false})[1] == true);
  REQUIRE(Dmux8Way(true, {false, true, false})[2] == true);
  REQUIRE(Dmux8Way(true, {true, true, false})[3] == true);
  REQUIRE(Dmux8Way(true, {false, false, true})[4] == true);
  REQUIRE(Dmux8Way(true, {true, false, true})[5] == true);
  REQUIRE(Dmux8Way(true, {false, true, true})[6] == true);
  REQUIRE(Dmux8Way(true, {true, true, true})[7] == true);
}
