\ SCIP STATISTICS
\   Problem name     : Max Flow
\   Variables        : 25 (0 binary, 0 integer, 0 implicit integer, 25 continuous)
\   Constraints      : 28
Maximize
 Obj: +1 x#0#0 +1 x#0#1 +1 x#0#2 +1 x#0#3 +1 x#0#4 +1 x#1#0 +1 x#1#1 +1 x#1#2 +1 x#1#3 +1 x#1#4 +1 x#2#0
      +1 x#2#1 +1 x#2#2 +1 x#2#3 +1 x#2#4 +1 x#3#0 +1 x#3#1 +1 x#3#2 +1 x#3#3 +1 x#3#4 +1 x#4#0 +1 x#4#1
      +1 x#4#2 +1 x#4#3 +1 x#4#4
Subject to
 flow_balance_1: +1 x#0#1 +1 x#2#1 +1 x#3#1 +1 x#4#1 -1 x#1#0 -1 x#1#2 -1 x#1#3 -1 x#1#4 = +0
 flow_balance_2: +1 x#0#2 +1 x#1#2 +1 x#3#2 +1 x#4#2 -1 x#2#0 -1 x#2#1 -1 x#2#3 -1 x#2#4 = +0
 flow_balance_3: +1 x#0#3 +1 x#1#3 +1 x#2#3 +1 x#4#3 -1 x#3#0 -1 x#3#1 -1 x#3#2 -1 x#3#4 = +0
 capa_cons_0_0: +1 x#0#0 = +0
 capa_cons_0_1_lhs: +1 x#0#1 >= +0
 capa_cons_0_1_rhs: +1 x#0#1 <= +14
 capa_cons_0_2_lhs: +1 x#0#2 >= +0
 capa_cons_0_2_rhs: +1 x#0#2 <= +17
 capa_cons_0_3_lhs: +1 x#0#3 >= +0
 capa_cons_0_3_rhs: +1 x#0#3 <= +8
 capa_cons_0_4_lhs: +1 x#0#4 >= +0
 capa_cons_0_4_rhs: +1 x#0#4 <= +15
 capa_cons_1_0_lhs: +1 x#1#0 >= +0
 capa_cons_1_0_rhs: +1 x#1#0 <= +14
 capa_cons_1_1: +1 x#1#1 = +0
 capa_cons_1_2_lhs: +1 x#1#2 >= +0
 capa_cons_1_2_rhs: +1 x#1#2 <= +34
 capa_cons_1_3_lhs: +1 x#1#3 >= +0
 capa_cons_1_3_rhs: +1 x#1#3 <= +17
 capa_cons_1_4_lhs: +1 x#1#4 >= +0
 capa_cons_1_4_rhs: +1 x#1#4 <= +21
 capa_cons_2_0_lhs: +1 x#2#0 >= +0
 capa_cons_2_0_rhs: +1 x#2#0 <= +17
 capa_cons_2_1_lhs: +1 x#2#1 >= +0
 capa_cons_2_1_rhs: +1 x#2#1 <= +34
 capa_cons_2_2: +1 x#2#2 = +0
 capa_cons_2_3_lhs: +1 x#2#3 >= +0
 capa_cons_2_3_rhs: +1 x#2#3 <= +15
 capa_cons_2_4_lhs: +1 x#2#4 >= +0
 capa_cons_2_4_rhs: +1 x#2#4 <= +8
 capa_cons_3_0_lhs: +1 x#3#0 >= +0
 capa_cons_3_0_rhs: +1 x#3#0 <= +8
 capa_cons_3_1_lhs: +1 x#3#1 >= +0
 capa_cons_3_1_rhs: +1 x#3#1 <= +17
 capa_cons_3_2_lhs: +1 x#3#2 >= +0
 capa_cons_3_2_rhs: +1 x#3#2 <= +15
 capa_cons_3_3: +1 x#3#3 = +0
 capa_cons_3_4_lhs: +1 x#3#4 >= +0
 capa_cons_3_4_rhs: +1 x#3#4 <= +16
 capa_cons_4_0_lhs: +1 x#4#0 >= +0
 capa_cons_4_0_rhs: +1 x#4#0 <= +15
 capa_cons_4_1_lhs: +1 x#4#1 >= +0
 capa_cons_4_1_rhs: +1 x#4#1 <= +21
 capa_cons_4_2_lhs: +1 x#4#2 >= +0
 capa_cons_4_2_rhs: +1 x#4#2 <= +8
 capa_cons_4_3_lhs: +1 x#4#3 >= +0
 capa_cons_4_3_rhs: +1 x#4#3 <= +16
 capa_cons_4_4: +1 x#4#4 = +0
Bounds
 0 <= x#0#0 <= 2147483647
 0 <= x#0#1 <= 2147483647
 0 <= x#0#2 <= 2147483647
 0 <= x#0#3 <= 2147483647
 0 <= x#0#4 <= 2147483647
 0 <= x#1#0 <= 2147483647
 0 <= x#1#1 <= 2147483647
 0 <= x#1#2 <= 2147483647
 0 <= x#1#3 <= 2147483647
 0 <= x#1#4 <= 2147483647
 0 <= x#2#0 <= 2147483647
 0 <= x#2#1 <= 2147483647
 0 <= x#2#2 <= 2147483647
 0 <= x#2#3 <= 2147483647
 0 <= x#2#4 <= 2147483647
 0 <= x#3#0 <= 2147483647
 0 <= x#3#1 <= 2147483647
 0 <= x#3#2 <= 2147483647
 0 <= x#3#3 <= 2147483647
 0 <= x#3#4 <= 2147483647
 0 <= x#4#0 <= 2147483647
 0 <= x#4#1 <= 2147483647
 0 <= x#4#2 <= 2147483647
 0 <= x#4#3 <= 2147483647
 0 <= x#4#4 <= 2147483647
End
