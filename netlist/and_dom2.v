/////////////////////////////////////////////////////////////
// Created by: Synopsys DC Expert(TM) in wire load mode
// Version   : N-2017.09-SP3
// Date      : Wed Oct 23 09:51:33 2019
/////////////////////////////////////////////////////////////


module and_dom2 ( clk, x1, x2, x3, y1, y2, y3, r1, r2, r3, z1, z2, z3 );
  input clk, x1, x2, x3, y1, y2, y3, r1, r2, r3;
  output z1, z2, z3;
  wire   z1_next, z2_next, z3_next, z4_next, z5_next, z6_next, z7_next,
         z8_next, z9_next, z1_reg, z2_reg, z3_reg, z4_reg, z5_reg, z6_reg,
         z7_reg, z8_reg, z9_reg, N0, N1, N2, N3, N4, N5, N6, N7, N8;

  DFF_X1 z1_reg_reg ( .D(z1_next), .CK(clk), .Q(z1_reg), .QN() );
  DFF_X1 z2_reg_reg ( .D(z2_next), .CK(clk), .Q(z2_reg), .QN() );
  DFF_X1 z3_reg_reg ( .D(z3_next), .CK(clk), .Q(z3_reg), .QN() );
  DFF_X1 z4_reg_reg ( .D(z4_next), .CK(clk), .Q(z4_reg), .QN() );
  DFF_X1 z5_reg_reg ( .D(z5_next), .CK(clk), .Q(z5_reg), .QN() );
  DFF_X1 z6_reg_reg ( .D(z6_next), .CK(clk), .Q(z6_reg), .QN() );
  DFF_X1 z7_reg_reg ( .D(z7_next), .CK(clk), .Q(z7_reg), .QN() );
  DFF_X1 z8_reg_reg ( .D(z8_next), .CK(clk), .Q(z8_reg), .QN() );
  DFF_X1 z9_reg_reg ( .D(z9_next), .CK(clk), .Q(z9_reg), .QN() );
  AND2_X1 U24 ( .A1(x3), .A2(y3), .ZN(z9_next) );
  XOR2_X1 U25 ( .A(r3), .B(N5), .Z(z8_next) );
  XOR2_X1 U26 ( .A(r2), .B(N4), .Z(z7_next) );
  XOR2_X1 U27 ( .A(r3), .B(N3), .Z(z6_next) );
  AND2_X1 U28 ( .A1(y2), .A2(x2), .ZN(z5_next) );
  XOR2_X1 U29 ( .A(r1), .B(N2), .Z(z4_next) );
  XOR2_X1 U30 ( .A(r2), .B(N1), .Z(z3_next) );
  XOR2_X1 U31 ( .A(z9_reg), .B(N8), .Z(z3) );
  XOR2_X1 U32 ( .A(r1), .B(N0), .Z(z2_next) );
  XOR2_X1 U33 ( .A(z6_reg), .B(N7), .Z(z2) );
  AND2_X1 U34 ( .A1(y1), .A2(x1), .ZN(z1_next) );
  XOR2_X1 U35 ( .A(z3_reg), .B(N6), .Z(z1) );
  XOR2_X1 U36 ( .A(z8_reg), .B(z7_reg), .Z(N8) );
  XOR2_X1 U37 ( .A(z5_reg), .B(z4_reg), .Z(N7) );
  XOR2_X1 U38 ( .A(z2_reg), .B(z1_reg), .Z(N6) );
  AND2_X1 U39 ( .A1(y2), .A2(x3), .ZN(N5) );
  AND2_X1 U40 ( .A1(y1), .A2(x3), .ZN(N4) );
  AND2_X1 U41 ( .A1(x2), .A2(y3), .ZN(N3) );
  AND2_X1 U42 ( .A1(x2), .A2(y1), .ZN(N2) );
  AND2_X1 U43 ( .A1(x1), .A2(y3), .ZN(N1) );
  AND2_X1 U44 ( .A1(x1), .A2(y2), .ZN(N0) );
endmodule

