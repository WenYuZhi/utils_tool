\ Model _copy
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  s1 + s2 + s3 + s4
Subject To
 C1_: s1 + 17 x1 + 75 x2 + 9 x3 + 87 x4 + 58 x5 + 79 x6 + 69 x7 + 37 x8
   + 88 x9 + 75 x10 + 45 x11 + 35 x12 + 73 x13 + 26 x14 + 39 x15 + 78 x16
   + 85 x17 + 58 x18 + 72 x19 + 8 x20 + 46 x21 + 11 x22 + 55 x23 + 39 x24
   + 57 x25 + 96 x26 + 87 x27 + 16 x28 + 27 x29 + 26 x30 = 786
 C2_: s2 + 93 x1 + 44 x2 + 79 x3 + 12 x4 + 8 x5 + 95 x6 + 2 x7 + 15 x8
   + 38 x9 + 15 x10 + 53 x11 + 88 x12 + 43 x13 + 26 x14 + 31 x15 + 77 x16
   + 10 x17 + 77 x18 + 71 x19 + 22 x20 + 76 x21 + 41 x22 + 65 x23 + 93 x24
   + 50 x25 + 69 x26 + 44 x27 + 61 x28 + 58 x29 + 63 x30 = 759
 C3_: s3 + 46 x1 + 63 x2 + 13 x3 + 97 x4 + 14 x5 + 45 x6 + 32 x7 + 96 x8
   + 36 x9 + 40 x10 + 10 x11 + 96 x12 + 99 x13 + 58 x14 + 87 x15 + 15 x16
   + 91 x17 + 65 x18 + 6 x19 + 96 x20 + 97 x21 + 79 x22 + 81 x23 + 57 x24
   + 28 x25 + 97 x26 + 58 x27 + 44 x28 + 37 x29 + 93 x30 = 888
 C4_: s4 + 2 x1 + 77 x2 + 73 x3 + 59 x4 + 43 x5 + 64 x6 + 75 x7 + 6 x8
   + 5 x9 + 78 x10 + 71 x11 + 12 x12 + 30 x13 + 7 x14 + 69 x15 + 36 x16
   + 73 x17 + 19 x18 + 15 x19 + 16 x20 + 84 x21 + 55 x22 + 32 x23 + 53 x24
   + 43 x25 + 21 x26 + 73 x27 + 59 x29 + 48 x30 = 649
 R4: 2 s1 + 2 s2 + 2 s3 + 11 x1 <= 10
Bounds
 x1 <= 1
 x2 <= 1
 x3 <= 1
 x4 <= 1
 x5 <= 1
 x6 <= 1
 x7 <= 1
 x8 <= 1
 x9 <= 1
 x10 <= 1
 x11 <= 1
 x12 <= 1
 x13 <= 1
 x14 <= 1
 x15 <= 1
 x16 <= 1
 x17 <= 1
 x18 <= 1
 x19 <= 1
 x20 <= 1
 x21 <= 1
 x22 <= 1
 x23 <= 1
 x24 <= 1
 x25 <= 1
 x26 <= 1
 x27 <= 1
 x28 <= 1
 x29 <= 1
 x30 <= 1
Generals
 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16 x17 x18 x19 x20 x21
 x22 x23 x24 x25 x26 x27 x28 x29 x30
End
