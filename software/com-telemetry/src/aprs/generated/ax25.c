#include "ax25.h"

const AX25EncodedData byte2ax25EncodedData[] =
{
  { 85,  8,  0}, // idx = 0000, oldNumberOfOnes = 0, byte2Encode = 000
  {170,  8,  0}, // idx = 0001, oldNumberOfOnes = 0, byte2Encode = 001
  {171,  8,  0}, // idx = 0002, oldNumberOfOnes = 0, byte2Encode = 002
  { 84,  8,  0}, // idx = 0003, oldNumberOfOnes = 0, byte2Encode = 003
  {169,  8,  0}, // idx = 0004, oldNumberOfOnes = 0, byte2Encode = 004
  { 86,  8,  0}, // idx = 0005, oldNumberOfOnes = 0, byte2Encode = 005
  { 87,  8,  0}, // idx = 0006, oldNumberOfOnes = 0, byte2Encode = 006
  {168,  8,  0}, // idx = 0007, oldNumberOfOnes = 0, byte2Encode = 007
  {173,  8,  0}, // idx = 0008, oldNumberOfOnes = 0, byte2Encode = 008
  { 82,  8,  0}, // idx = 0009, oldNumberOfOnes = 0, byte2Encode = 009
  { 83,  8,  0}, // idx = 0010, oldNumberOfOnes = 0, byte2Encode = 010
  {172,  8,  0}, // idx = 0011, oldNumberOfOnes = 0, byte2Encode = 011
  { 81,  8,  0}, // idx = 0012, oldNumberOfOnes = 0, byte2Encode = 012
  {174,  8,  0}, // idx = 0013, oldNumberOfOnes = 0, byte2Encode = 013
  {175,  8,  0}, // idx = 0014, oldNumberOfOnes = 0, byte2Encode = 014
  { 80,  8,  0}, // idx = 0015, oldNumberOfOnes = 0, byte2Encode = 015
  {165,  8,  0}, // idx = 0016, oldNumberOfOnes = 0, byte2Encode = 016
  { 90,  8,  0}, // idx = 0017, oldNumberOfOnes = 0, byte2Encode = 017
  { 91,  8,  0}, // idx = 0018, oldNumberOfOnes = 0, byte2Encode = 018
  {164,  8,  0}, // idx = 0019, oldNumberOfOnes = 0, byte2Encode = 019
  { 89,  8,  0}, // idx = 0020, oldNumberOfOnes = 0, byte2Encode = 020
  {166,  8,  0}, // idx = 0021, oldNumberOfOnes = 0, byte2Encode = 021
  {167,  8,  0}, // idx = 0022, oldNumberOfOnes = 0, byte2Encode = 022
  { 88,  8,  0}, // idx = 0023, oldNumberOfOnes = 0, byte2Encode = 023
  { 93,  8,  0}, // idx = 0024, oldNumberOfOnes = 0, byte2Encode = 024
  {162,  8,  0}, // idx = 0025, oldNumberOfOnes = 0, byte2Encode = 025
  {163,  8,  0}, // idx = 0026, oldNumberOfOnes = 0, byte2Encode = 026
  { 92,  8,  0}, // idx = 0027, oldNumberOfOnes = 0, byte2Encode = 027
  {161,  8,  0}, // idx = 0028, oldNumberOfOnes = 0, byte2Encode = 028
  { 94,  8,  0}, // idx = 0029, oldNumberOfOnes = 0, byte2Encode = 029
  { 95,  8,  0}, // idx = 0030, oldNumberOfOnes = 0, byte2Encode = 030
  {160,  9,  0}, // idx = 0031, oldNumberOfOnes = 0, byte2Encode = 031
  {181,  8,  0}, // idx = 0032, oldNumberOfOnes = 0, byte2Encode = 032
  { 74,  8,  0}, // idx = 0033, oldNumberOfOnes = 0, byte2Encode = 033
  { 75,  8,  0}, // idx = 0034, oldNumberOfOnes = 0, byte2Encode = 034
  {180,  8,  0}, // idx = 0035, oldNumberOfOnes = 0, byte2Encode = 035
  { 73,  8,  0}, // idx = 0036, oldNumberOfOnes = 0, byte2Encode = 036
  {182,  8,  0}, // idx = 0037, oldNumberOfOnes = 0, byte2Encode = 037
  {183,  8,  0}, // idx = 0038, oldNumberOfOnes = 0, byte2Encode = 038
  { 72,  8,  0}, // idx = 0039, oldNumberOfOnes = 0, byte2Encode = 039
  { 77,  8,  0}, // idx = 0040, oldNumberOfOnes = 0, byte2Encode = 040
  {178,  8,  0}, // idx = 0041, oldNumberOfOnes = 0, byte2Encode = 041
  {179,  8,  0}, // idx = 0042, oldNumberOfOnes = 0, byte2Encode = 042
  { 76,  8,  0}, // idx = 0043, oldNumberOfOnes = 0, byte2Encode = 043
  {177,  8,  0}, // idx = 0044, oldNumberOfOnes = 0, byte2Encode = 044
  { 78,  8,  0}, // idx = 0045, oldNumberOfOnes = 0, byte2Encode = 045
  { 79,  8,  0}, // idx = 0046, oldNumberOfOnes = 0, byte2Encode = 046
  {176,  8,  0}, // idx = 0047, oldNumberOfOnes = 0, byte2Encode = 047
  { 69,  8,  0}, // idx = 0048, oldNumberOfOnes = 0, byte2Encode = 048
  {186,  8,  0}, // idx = 0049, oldNumberOfOnes = 0, byte2Encode = 049
  {187,  8,  0}, // idx = 0050, oldNumberOfOnes = 0, byte2Encode = 050
  { 68,  8,  0}, // idx = 0051, oldNumberOfOnes = 0, byte2Encode = 051
  {185,  8,  0}, // idx = 0052, oldNumberOfOnes = 0, byte2Encode = 052
  { 70,  8,  0}, // idx = 0053, oldNumberOfOnes = 0, byte2Encode = 053
  { 71,  8,  0}, // idx = 0054, oldNumberOfOnes = 0, byte2Encode = 054
  {184,  8,  0}, // idx = 0055, oldNumberOfOnes = 0, byte2Encode = 055
  {189,  8,  0}, // idx = 0056, oldNumberOfOnes = 0, byte2Encode = 056
  { 66,  8,  0}, // idx = 0057, oldNumberOfOnes = 0, byte2Encode = 057
  { 67,  8,  0}, // idx = 0058, oldNumberOfOnes = 0, byte2Encode = 058
  {188,  8,  0}, // idx = 0059, oldNumberOfOnes = 0, byte2Encode = 059
  { 65,  8,  0}, // idx = 0060, oldNumberOfOnes = 0, byte2Encode = 060
  {190,  8,  0}, // idx = 0061, oldNumberOfOnes = 0, byte2Encode = 061
  {191,  9,  0}, // idx = 0062, oldNumberOfOnes = 0, byte2Encode = 062
  {352,  9,  0}, // idx = 0063, oldNumberOfOnes = 0, byte2Encode = 063
  {149,  8,  0}, // idx = 0064, oldNumberOfOnes = 0, byte2Encode = 064
  {106,  8,  0}, // idx = 0065, oldNumberOfOnes = 0, byte2Encode = 065
  {107,  8,  0}, // idx = 0066, oldNumberOfOnes = 0, byte2Encode = 066
  {148,  8,  0}, // idx = 0067, oldNumberOfOnes = 0, byte2Encode = 067
  {105,  8,  0}, // idx = 0068, oldNumberOfOnes = 0, byte2Encode = 068
  {150,  8,  0}, // idx = 0069, oldNumberOfOnes = 0, byte2Encode = 069
  {151,  8,  0}, // idx = 0070, oldNumberOfOnes = 0, byte2Encode = 070
  {104,  8,  0}, // idx = 0071, oldNumberOfOnes = 0, byte2Encode = 071
  {109,  8,  0}, // idx = 0072, oldNumberOfOnes = 0, byte2Encode = 072
  {146,  8,  0}, // idx = 0073, oldNumberOfOnes = 0, byte2Encode = 073
  {147,  8,  0}, // idx = 0074, oldNumberOfOnes = 0, byte2Encode = 074
  {108,  8,  0}, // idx = 0075, oldNumberOfOnes = 0, byte2Encode = 075
  {145,  8,  0}, // idx = 0076, oldNumberOfOnes = 0, byte2Encode = 076
  {110,  8,  0}, // idx = 0077, oldNumberOfOnes = 0, byte2Encode = 077
  {111,  8,  0}, // idx = 0078, oldNumberOfOnes = 0, byte2Encode = 078
  {144,  8,  0}, // idx = 0079, oldNumberOfOnes = 0, byte2Encode = 079
  {101,  8,  0}, // idx = 0080, oldNumberOfOnes = 0, byte2Encode = 080
  {154,  8,  0}, // idx = 0081, oldNumberOfOnes = 0, byte2Encode = 081
  {155,  8,  0}, // idx = 0082, oldNumberOfOnes = 0, byte2Encode = 082
  {100,  8,  0}, // idx = 0083, oldNumberOfOnes = 0, byte2Encode = 083
  {153,  8,  0}, // idx = 0084, oldNumberOfOnes = 0, byte2Encode = 084
  {102,  8,  0}, // idx = 0085, oldNumberOfOnes = 0, byte2Encode = 085
  {103,  8,  0}, // idx = 0086, oldNumberOfOnes = 0, byte2Encode = 086
  {152,  8,  0}, // idx = 0087, oldNumberOfOnes = 0, byte2Encode = 087
  {157,  8,  0}, // idx = 0088, oldNumberOfOnes = 0, byte2Encode = 088
  { 98,  8,  0}, // idx = 0089, oldNumberOfOnes = 0, byte2Encode = 089
  { 99,  8,  0}, // idx = 0090, oldNumberOfOnes = 0, byte2Encode = 090
  {156,  8,  0}, // idx = 0091, oldNumberOfOnes = 0, byte2Encode = 091
  { 97,  8,  0}, // idx = 0092, oldNumberOfOnes = 0, byte2Encode = 092
  {158,  8,  0}, // idx = 0093, oldNumberOfOnes = 0, byte2Encode = 093
  {159,  8,  0}, // idx = 0094, oldNumberOfOnes = 0, byte2Encode = 094
  {288,  9,  0}, // idx = 0095, oldNumberOfOnes = 0, byte2Encode = 095
  {117,  8,  0}, // idx = 0096, oldNumberOfOnes = 0, byte2Encode = 096
  {138,  8,  0}, // idx = 0097, oldNumberOfOnes = 0, byte2Encode = 097
  {139,  8,  0}, // idx = 0098, oldNumberOfOnes = 0, byte2Encode = 098
  {116,  8,  0}, // idx = 0099, oldNumberOfOnes = 0, byte2Encode = 099
  {137,  8,  0}, // idx = 0100, oldNumberOfOnes = 0, byte2Encode = 100
  {118,  8,  0}, // idx = 0101, oldNumberOfOnes = 0, byte2Encode = 101
  {119,  8,  0}, // idx = 0102, oldNumberOfOnes = 0, byte2Encode = 102
  {136,  8,  0}, // idx = 0103, oldNumberOfOnes = 0, byte2Encode = 103
  {141,  8,  0}, // idx = 0104, oldNumberOfOnes = 0, byte2Encode = 104
  {114,  8,  0}, // idx = 0105, oldNumberOfOnes = 0, byte2Encode = 105
  {115,  8,  0}, // idx = 0106, oldNumberOfOnes = 0, byte2Encode = 106
  {140,  8,  0}, // idx = 0107, oldNumberOfOnes = 0, byte2Encode = 107
  {113,  8,  0}, // idx = 0108, oldNumberOfOnes = 0, byte2Encode = 108
  {142,  8,  0}, // idx = 0109, oldNumberOfOnes = 0, byte2Encode = 109
  {143,  8,  0}, // idx = 0110, oldNumberOfOnes = 0, byte2Encode = 110
  {112,  8,  0}, // idx = 0111, oldNumberOfOnes = 0, byte2Encode = 111
  {133,  8,  0}, // idx = 0112, oldNumberOfOnes = 0, byte2Encode = 112
  {122,  8,  0}, // idx = 0113, oldNumberOfOnes = 0, byte2Encode = 113
  {123,  8,  0}, // idx = 0114, oldNumberOfOnes = 0, byte2Encode = 114
  {132,  8,  0}, // idx = 0115, oldNumberOfOnes = 0, byte2Encode = 115
  {121,  8,  0}, // idx = 0116, oldNumberOfOnes = 0, byte2Encode = 116
  {134,  8,  0}, // idx = 0117, oldNumberOfOnes = 0, byte2Encode = 117
  {135,  8,  0}, // idx = 0118, oldNumberOfOnes = 0, byte2Encode = 118
  {120,  8,  0}, // idx = 0119, oldNumberOfOnes = 0, byte2Encode = 119
  {125,  8,  0}, // idx = 0120, oldNumberOfOnes = 0, byte2Encode = 120
  {130,  8,  0}, // idx = 0121, oldNumberOfOnes = 0, byte2Encode = 121
  {131,  8,  0}, // idx = 0122, oldNumberOfOnes = 0, byte2Encode = 122
  {124,  8,  0}, // idx = 0123, oldNumberOfOnes = 0, byte2Encode = 123
  {129,  9,  0}, // idx = 0124, oldNumberOfOnes = 0, byte2Encode = 124
  {382,  9,  0}, // idx = 0125, oldNumberOfOnes = 0, byte2Encode = 125
  {319,  9,  0}, // idx = 0126, oldNumberOfOnes = 0, byte2Encode = 126
  {224,  9,  0}, // idx = 0127, oldNumberOfOnes = 0, byte2Encode = 127
  {213,  8,  1}, // idx = 0128, oldNumberOfOnes = 0, byte2Encode = 128
  { 42,  8,  1}, // idx = 0129, oldNumberOfOnes = 0, byte2Encode = 129
  { 43,  8,  1}, // idx = 0130, oldNumberOfOnes = 0, byte2Encode = 130
  {212,  8,  1}, // idx = 0131, oldNumberOfOnes = 0, byte2Encode = 131
  { 41,  8,  1}, // idx = 0132, oldNumberOfOnes = 0, byte2Encode = 132
  {214,  8,  1}, // idx = 0133, oldNumberOfOnes = 0, byte2Encode = 133
  {215,  8,  1}, // idx = 0134, oldNumberOfOnes = 0, byte2Encode = 134
  { 40,  8,  1}, // idx = 0135, oldNumberOfOnes = 0, byte2Encode = 135
  { 45,  8,  1}, // idx = 0136, oldNumberOfOnes = 0, byte2Encode = 136
  {210,  8,  1}, // idx = 0137, oldNumberOfOnes = 0, byte2Encode = 137
  {211,  8,  1}, // idx = 0138, oldNumberOfOnes = 0, byte2Encode = 138
  { 44,  8,  1}, // idx = 0139, oldNumberOfOnes = 0, byte2Encode = 139
  {209,  8,  1}, // idx = 0140, oldNumberOfOnes = 0, byte2Encode = 140
  { 46,  8,  1}, // idx = 0141, oldNumberOfOnes = 0, byte2Encode = 141
  { 47,  8,  1}, // idx = 0142, oldNumberOfOnes = 0, byte2Encode = 142
  {208,  8,  1}, // idx = 0143, oldNumberOfOnes = 0, byte2Encode = 143
  { 37,  8,  1}, // idx = 0144, oldNumberOfOnes = 0, byte2Encode = 144
  {218,  8,  1}, // idx = 0145, oldNumberOfOnes = 0, byte2Encode = 145
  {219,  8,  1}, // idx = 0146, oldNumberOfOnes = 0, byte2Encode = 146
  { 36,  8,  1}, // idx = 0147, oldNumberOfOnes = 0, byte2Encode = 147
  {217,  8,  1}, // idx = 0148, oldNumberOfOnes = 0, byte2Encode = 148
  { 38,  8,  1}, // idx = 0149, oldNumberOfOnes = 0, byte2Encode = 149
  { 39,  8,  1}, // idx = 0150, oldNumberOfOnes = 0, byte2Encode = 150
  {216,  8,  1}, // idx = 0151, oldNumberOfOnes = 0, byte2Encode = 151
  {221,  8,  1}, // idx = 0152, oldNumberOfOnes = 0, byte2Encode = 152
  { 34,  8,  1}, // idx = 0153, oldNumberOfOnes = 0, byte2Encode = 153
  { 35,  8,  1}, // idx = 0154, oldNumberOfOnes = 0, byte2Encode = 154
  {220,  8,  1}, // idx = 0155, oldNumberOfOnes = 0, byte2Encode = 155
  { 33,  8,  1}, // idx = 0156, oldNumberOfOnes = 0, byte2Encode = 156
  {222,  8,  1}, // idx = 0157, oldNumberOfOnes = 0, byte2Encode = 157
  {223,  8,  1}, // idx = 0158, oldNumberOfOnes = 0, byte2Encode = 158
  {416,  9,  1}, // idx = 0159, oldNumberOfOnes = 0, byte2Encode = 159
  { 53,  8,  1}, // idx = 0160, oldNumberOfOnes = 0, byte2Encode = 160
  {202,  8,  1}, // idx = 0161, oldNumberOfOnes = 0, byte2Encode = 161
  {203,  8,  1}, // idx = 0162, oldNumberOfOnes = 0, byte2Encode = 162
  { 52,  8,  1}, // idx = 0163, oldNumberOfOnes = 0, byte2Encode = 163
  {201,  8,  1}, // idx = 0164, oldNumberOfOnes = 0, byte2Encode = 164
  { 54,  8,  1}, // idx = 0165, oldNumberOfOnes = 0, byte2Encode = 165
  { 55,  8,  1}, // idx = 0166, oldNumberOfOnes = 0, byte2Encode = 166
  {200,  8,  1}, // idx = 0167, oldNumberOfOnes = 0, byte2Encode = 167
  {205,  8,  1}, // idx = 0168, oldNumberOfOnes = 0, byte2Encode = 168
  { 50,  8,  1}, // idx = 0169, oldNumberOfOnes = 0, byte2Encode = 169
  { 51,  8,  1}, // idx = 0170, oldNumberOfOnes = 0, byte2Encode = 170
  {204,  8,  1}, // idx = 0171, oldNumberOfOnes = 0, byte2Encode = 171
  { 49,  8,  1}, // idx = 0172, oldNumberOfOnes = 0, byte2Encode = 172
  {206,  8,  1}, // idx = 0173, oldNumberOfOnes = 0, byte2Encode = 173
  {207,  8,  1}, // idx = 0174, oldNumberOfOnes = 0, byte2Encode = 174
  { 48,  8,  1}, // idx = 0175, oldNumberOfOnes = 0, byte2Encode = 175
  {197,  8,  1}, // idx = 0176, oldNumberOfOnes = 0, byte2Encode = 176
  { 58,  8,  1}, // idx = 0177, oldNumberOfOnes = 0, byte2Encode = 177
  { 59,  8,  1}, // idx = 0178, oldNumberOfOnes = 0, byte2Encode = 178
  {196,  8,  1}, // idx = 0179, oldNumberOfOnes = 0, byte2Encode = 179
  { 57,  8,  1}, // idx = 0180, oldNumberOfOnes = 0, byte2Encode = 180
  {198,  8,  1}, // idx = 0181, oldNumberOfOnes = 0, byte2Encode = 181
  {199,  8,  1}, // idx = 0182, oldNumberOfOnes = 0, byte2Encode = 182
  { 56,  8,  1}, // idx = 0183, oldNumberOfOnes = 0, byte2Encode = 183
  { 61,  8,  1}, // idx = 0184, oldNumberOfOnes = 0, byte2Encode = 184
  {194,  8,  1}, // idx = 0185, oldNumberOfOnes = 0, byte2Encode = 185
  {195,  8,  1}, // idx = 0186, oldNumberOfOnes = 0, byte2Encode = 186
  { 60,  8,  1}, // idx = 0187, oldNumberOfOnes = 0, byte2Encode = 187
  {193,  8,  1}, // idx = 0188, oldNumberOfOnes = 0, byte2Encode = 188
  { 62,  8,  1}, // idx = 0189, oldNumberOfOnes = 0, byte2Encode = 189
  {447,  9,  1}, // idx = 0190, oldNumberOfOnes = 0, byte2Encode = 190
  { 96,  9,  1}, // idx = 0191, oldNumberOfOnes = 0, byte2Encode = 191
  { 21,  8,  2}, // idx = 0192, oldNumberOfOnes = 0, byte2Encode = 192
  {234,  8,  2}, // idx = 0193, oldNumberOfOnes = 0, byte2Encode = 193
  {235,  8,  2}, // idx = 0194, oldNumberOfOnes = 0, byte2Encode = 194
  { 20,  8,  2}, // idx = 0195, oldNumberOfOnes = 0, byte2Encode = 195
  {233,  8,  2}, // idx = 0196, oldNumberOfOnes = 0, byte2Encode = 196
  { 22,  8,  2}, // idx = 0197, oldNumberOfOnes = 0, byte2Encode = 197
  { 23,  8,  2}, // idx = 0198, oldNumberOfOnes = 0, byte2Encode = 198
  {232,  8,  2}, // idx = 0199, oldNumberOfOnes = 0, byte2Encode = 199
  {237,  8,  2}, // idx = 0200, oldNumberOfOnes = 0, byte2Encode = 200
  { 18,  8,  2}, // idx = 0201, oldNumberOfOnes = 0, byte2Encode = 201
  { 19,  8,  2}, // idx = 0202, oldNumberOfOnes = 0, byte2Encode = 202
  {236,  8,  2}, // idx = 0203, oldNumberOfOnes = 0, byte2Encode = 203
  { 17,  8,  2}, // idx = 0204, oldNumberOfOnes = 0, byte2Encode = 204
  {238,  8,  2}, // idx = 0205, oldNumberOfOnes = 0, byte2Encode = 205
  {239,  8,  2}, // idx = 0206, oldNumberOfOnes = 0, byte2Encode = 206
  { 16,  8,  2}, // idx = 0207, oldNumberOfOnes = 0, byte2Encode = 207
  {229,  8,  2}, // idx = 0208, oldNumberOfOnes = 0, byte2Encode = 208
  { 26,  8,  2}, // idx = 0209, oldNumberOfOnes = 0, byte2Encode = 209
  { 27,  8,  2}, // idx = 0210, oldNumberOfOnes = 0, byte2Encode = 210
  {228,  8,  2}, // idx = 0211, oldNumberOfOnes = 0, byte2Encode = 211
  { 25,  8,  2}, // idx = 0212, oldNumberOfOnes = 0, byte2Encode = 212
  {230,  8,  2}, // idx = 0213, oldNumberOfOnes = 0, byte2Encode = 213
  {231,  8,  2}, // idx = 0214, oldNumberOfOnes = 0, byte2Encode = 214
  { 24,  8,  2}, // idx = 0215, oldNumberOfOnes = 0, byte2Encode = 215
  { 29,  8,  2}, // idx = 0216, oldNumberOfOnes = 0, byte2Encode = 216
  {226,  8,  2}, // idx = 0217, oldNumberOfOnes = 0, byte2Encode = 217
  {227,  8,  2}, // idx = 0218, oldNumberOfOnes = 0, byte2Encode = 218
  { 28,  8,  2}, // idx = 0219, oldNumberOfOnes = 0, byte2Encode = 219
  {225,  8,  2}, // idx = 0220, oldNumberOfOnes = 0, byte2Encode = 220
  { 30,  8,  2}, // idx = 0221, oldNumberOfOnes = 0, byte2Encode = 221
  { 31,  8,  2}, // idx = 0222, oldNumberOfOnes = 0, byte2Encode = 222
  { 32,  9,  2}, // idx = 0223, oldNumberOfOnes = 0, byte2Encode = 223
  {245,  8,  3}, // idx = 0224, oldNumberOfOnes = 0, byte2Encode = 224
  { 10,  8,  3}, // idx = 0225, oldNumberOfOnes = 0, byte2Encode = 225
  { 11,  8,  3}, // idx = 0226, oldNumberOfOnes = 0, byte2Encode = 226
  {244,  8,  3}, // idx = 0227, oldNumberOfOnes = 0, byte2Encode = 227
  {  9,  8,  3}, // idx = 0228, oldNumberOfOnes = 0, byte2Encode = 228
  {246,  8,  3}, // idx = 0229, oldNumberOfOnes = 0, byte2Encode = 229
  {247,  8,  3}, // idx = 0230, oldNumberOfOnes = 0, byte2Encode = 230
  {  8,  8,  3}, // idx = 0231, oldNumberOfOnes = 0, byte2Encode = 231
  { 13,  8,  3}, // idx = 0232, oldNumberOfOnes = 0, byte2Encode = 232
  {242,  8,  3}, // idx = 0233, oldNumberOfOnes = 0, byte2Encode = 233
  {243,  8,  3}, // idx = 0234, oldNumberOfOnes = 0, byte2Encode = 234
  { 12,  8,  3}, // idx = 0235, oldNumberOfOnes = 0, byte2Encode = 235
  {241,  8,  3}, // idx = 0236, oldNumberOfOnes = 0, byte2Encode = 236
  { 14,  8,  3}, // idx = 0237, oldNumberOfOnes = 0, byte2Encode = 237
  { 15,  8,  3}, // idx = 0238, oldNumberOfOnes = 0, byte2Encode = 238
  {240,  8,  3}, // idx = 0239, oldNumberOfOnes = 0, byte2Encode = 239
  {  5,  8,  4}, // idx = 0240, oldNumberOfOnes = 0, byte2Encode = 240
  {250,  8,  4}, // idx = 0241, oldNumberOfOnes = 0, byte2Encode = 241
  {251,  8,  4}, // idx = 0242, oldNumberOfOnes = 0, byte2Encode = 242
  {  4,  8,  4}, // idx = 0243, oldNumberOfOnes = 0, byte2Encode = 243
  {249,  8,  4}, // idx = 0244, oldNumberOfOnes = 0, byte2Encode = 244
  {  6,  8,  4}, // idx = 0245, oldNumberOfOnes = 0, byte2Encode = 245
  {  7,  8,  4}, // idx = 0246, oldNumberOfOnes = 0, byte2Encode = 246
  {248,  8,  4}, // idx = 0247, oldNumberOfOnes = 0, byte2Encode = 247
  {253,  9,  0}, // idx = 0248, oldNumberOfOnes = 0, byte2Encode = 248
  {258,  9,  0}, // idx = 0249, oldNumberOfOnes = 0, byte2Encode = 249
  {259,  9,  0}, // idx = 0250, oldNumberOfOnes = 0, byte2Encode = 250
  {252,  9,  0}, // idx = 0251, oldNumberOfOnes = 0, byte2Encode = 251
  {385,  9,  1}, // idx = 0252, oldNumberOfOnes = 0, byte2Encode = 252
  {126,  9,  1}, // idx = 0253, oldNumberOfOnes = 0, byte2Encode = 253
  { 63,  9,  2}, // idx = 0254, oldNumberOfOnes = 0, byte2Encode = 254
  {480,  9,  3}, // idx = 0255, oldNumberOfOnes = 0, byte2Encode = 255
  { 85,  8,  0}, // idx = 0256, oldNumberOfOnes = 1, byte2Encode = 000
  {170,  8,  0}, // idx = 0257, oldNumberOfOnes = 1, byte2Encode = 001
  {171,  8,  0}, // idx = 0258, oldNumberOfOnes = 1, byte2Encode = 002
  { 84,  8,  0}, // idx = 0259, oldNumberOfOnes = 1, byte2Encode = 003
  {169,  8,  0}, // idx = 0260, oldNumberOfOnes = 1, byte2Encode = 004
  { 86,  8,  0}, // idx = 0261, oldNumberOfOnes = 1, byte2Encode = 005
  { 87,  8,  0}, // idx = 0262, oldNumberOfOnes = 1, byte2Encode = 006
  {168,  8,  0}, // idx = 0263, oldNumberOfOnes = 1, byte2Encode = 007
  {173,  8,  0}, // idx = 0264, oldNumberOfOnes = 1, byte2Encode = 008
  { 82,  8,  0}, // idx = 0265, oldNumberOfOnes = 1, byte2Encode = 009
  { 83,  8,  0}, // idx = 0266, oldNumberOfOnes = 1, byte2Encode = 010
  {172,  8,  0}, // idx = 0267, oldNumberOfOnes = 1, byte2Encode = 011
  { 81,  8,  0}, // idx = 0268, oldNumberOfOnes = 1, byte2Encode = 012
  {174,  8,  0}, // idx = 0269, oldNumberOfOnes = 1, byte2Encode = 013
  {175,  8,  0}, // idx = 0270, oldNumberOfOnes = 1, byte2Encode = 014
  {336,  9,  0}, // idx = 0271, oldNumberOfOnes = 1, byte2Encode = 015
  {165,  8,  0}, // idx = 0272, oldNumberOfOnes = 1, byte2Encode = 016
  { 90,  8,  0}, // idx = 0273, oldNumberOfOnes = 1, byte2Encode = 017
  { 91,  8,  0}, // idx = 0274, oldNumberOfOnes = 1, byte2Encode = 018
  {164,  8,  0}, // idx = 0275, oldNumberOfOnes = 1, byte2Encode = 019
  { 89,  8,  0}, // idx = 0276, oldNumberOfOnes = 1, byte2Encode = 020
  {166,  8,  0}, // idx = 0277, oldNumberOfOnes = 1, byte2Encode = 021
  {167,  8,  0}, // idx = 0278, oldNumberOfOnes = 1, byte2Encode = 022
  { 88,  8,  0}, // idx = 0279, oldNumberOfOnes = 1, byte2Encode = 023
  { 93,  8,  0}, // idx = 0280, oldNumberOfOnes = 1, byte2Encode = 024
  {162,  8,  0}, // idx = 0281, oldNumberOfOnes = 1, byte2Encode = 025
  {163,  8,  0}, // idx = 0282, oldNumberOfOnes = 1, byte2Encode = 026
  { 92,  8,  0}, // idx = 0283, oldNumberOfOnes = 1, byte2Encode = 027
  {161,  8,  0}, // idx = 0284, oldNumberOfOnes = 1, byte2Encode = 028
  { 94,  8,  0}, // idx = 0285, oldNumberOfOnes = 1, byte2Encode = 029
  { 95,  8,  0}, // idx = 0286, oldNumberOfOnes = 1, byte2Encode = 030
  {176,  9,  0}, // idx = 0287, oldNumberOfOnes = 1, byte2Encode = 031
  {181,  8,  0}, // idx = 0288, oldNumberOfOnes = 1, byte2Encode = 032
  { 74,  8,  0}, // idx = 0289, oldNumberOfOnes = 1, byte2Encode = 033
  { 75,  8,  0}, // idx = 0290, oldNumberOfOnes = 1, byte2Encode = 034
  {180,  8,  0}, // idx = 0291, oldNumberOfOnes = 1, byte2Encode = 035
  { 73,  8,  0}, // idx = 0292, oldNumberOfOnes = 1, byte2Encode = 036
  {182,  8,  0}, // idx = 0293, oldNumberOfOnes = 1, byte2Encode = 037
  {183,  8,  0}, // idx = 0294, oldNumberOfOnes = 1, byte2Encode = 038
  { 72,  8,  0}, // idx = 0295, oldNumberOfOnes = 1, byte2Encode = 039
  { 77,  8,  0}, // idx = 0296, oldNumberOfOnes = 1, byte2Encode = 040
  {178,  8,  0}, // idx = 0297, oldNumberOfOnes = 1, byte2Encode = 041
  {179,  8,  0}, // idx = 0298, oldNumberOfOnes = 1, byte2Encode = 042
  { 76,  8,  0}, // idx = 0299, oldNumberOfOnes = 1, byte2Encode = 043
  {177,  8,  0}, // idx = 0300, oldNumberOfOnes = 1, byte2Encode = 044
  { 78,  8,  0}, // idx = 0301, oldNumberOfOnes = 1, byte2Encode = 045
  { 79,  8,  0}, // idx = 0302, oldNumberOfOnes = 1, byte2Encode = 046
  {144,  9,  0}, // idx = 0303, oldNumberOfOnes = 1, byte2Encode = 047
  { 69,  8,  0}, // idx = 0304, oldNumberOfOnes = 1, byte2Encode = 048
  {186,  8,  0}, // idx = 0305, oldNumberOfOnes = 1, byte2Encode = 049
  {187,  8,  0}, // idx = 0306, oldNumberOfOnes = 1, byte2Encode = 050
  { 68,  8,  0}, // idx = 0307, oldNumberOfOnes = 1, byte2Encode = 051
  {185,  8,  0}, // idx = 0308, oldNumberOfOnes = 1, byte2Encode = 052
  { 70,  8,  0}, // idx = 0309, oldNumberOfOnes = 1, byte2Encode = 053
  { 71,  8,  0}, // idx = 0310, oldNumberOfOnes = 1, byte2Encode = 054
  {184,  8,  0}, // idx = 0311, oldNumberOfOnes = 1, byte2Encode = 055
  {189,  8,  0}, // idx = 0312, oldNumberOfOnes = 1, byte2Encode = 056
  { 66,  8,  0}, // idx = 0313, oldNumberOfOnes = 1, byte2Encode = 057
  { 67,  8,  0}, // idx = 0314, oldNumberOfOnes = 1, byte2Encode = 058
  {188,  8,  0}, // idx = 0315, oldNumberOfOnes = 1, byte2Encode = 059
  { 65,  8,  0}, // idx = 0316, oldNumberOfOnes = 1, byte2Encode = 060
  {190,  8,  0}, // idx = 0317, oldNumberOfOnes = 1, byte2Encode = 061
  {191,  9,  0}, // idx = 0318, oldNumberOfOnes = 1, byte2Encode = 062
  {368,  9,  0}, // idx = 0319, oldNumberOfOnes = 1, byte2Encode = 063
  {149,  8,  0}, // idx = 0320, oldNumberOfOnes = 1, byte2Encode = 064
  {106,  8,  0}, // idx = 0321, oldNumberOfOnes = 1, byte2Encode = 065
  {107,  8,  0}, // idx = 0322, oldNumberOfOnes = 1, byte2Encode = 066
  {148,  8,  0}, // idx = 0323, oldNumberOfOnes = 1, byte2Encode = 067
  {105,  8,  0}, // idx = 0324, oldNumberOfOnes = 1, byte2Encode = 068
  {150,  8,  0}, // idx = 0325, oldNumberOfOnes = 1, byte2Encode = 069
  {151,  8,  0}, // idx = 0326, oldNumberOfOnes = 1, byte2Encode = 070
  {104,  8,  0}, // idx = 0327, oldNumberOfOnes = 1, byte2Encode = 071
  {109,  8,  0}, // idx = 0328, oldNumberOfOnes = 1, byte2Encode = 072
  {146,  8,  0}, // idx = 0329, oldNumberOfOnes = 1, byte2Encode = 073
  {147,  8,  0}, // idx = 0330, oldNumberOfOnes = 1, byte2Encode = 074
  {108,  8,  0}, // idx = 0331, oldNumberOfOnes = 1, byte2Encode = 075
  {145,  8,  0}, // idx = 0332, oldNumberOfOnes = 1, byte2Encode = 076
  {110,  8,  0}, // idx = 0333, oldNumberOfOnes = 1, byte2Encode = 077
  {111,  8,  0}, // idx = 0334, oldNumberOfOnes = 1, byte2Encode = 078
  {208,  9,  0}, // idx = 0335, oldNumberOfOnes = 1, byte2Encode = 079
  {101,  8,  0}, // idx = 0336, oldNumberOfOnes = 1, byte2Encode = 080
  {154,  8,  0}, // idx = 0337, oldNumberOfOnes = 1, byte2Encode = 081
  {155,  8,  0}, // idx = 0338, oldNumberOfOnes = 1, byte2Encode = 082
  {100,  8,  0}, // idx = 0339, oldNumberOfOnes = 1, byte2Encode = 083
  {153,  8,  0}, // idx = 0340, oldNumberOfOnes = 1, byte2Encode = 084
  {102,  8,  0}, // idx = 0341, oldNumberOfOnes = 1, byte2Encode = 085
  {103,  8,  0}, // idx = 0342, oldNumberOfOnes = 1, byte2Encode = 086
  {152,  8,  0}, // idx = 0343, oldNumberOfOnes = 1, byte2Encode = 087
  {157,  8,  0}, // idx = 0344, oldNumberOfOnes = 1, byte2Encode = 088
  { 98,  8,  0}, // idx = 0345, oldNumberOfOnes = 1, byte2Encode = 089
  { 99,  8,  0}, // idx = 0346, oldNumberOfOnes = 1, byte2Encode = 090
  {156,  8,  0}, // idx = 0347, oldNumberOfOnes = 1, byte2Encode = 091
  { 97,  8,  0}, // idx = 0348, oldNumberOfOnes = 1, byte2Encode = 092
  {158,  8,  0}, // idx = 0349, oldNumberOfOnes = 1, byte2Encode = 093
  {159,  8,  0}, // idx = 0350, oldNumberOfOnes = 1, byte2Encode = 094
  {304,  9,  0}, // idx = 0351, oldNumberOfOnes = 1, byte2Encode = 095
  {117,  8,  0}, // idx = 0352, oldNumberOfOnes = 1, byte2Encode = 096
  {138,  8,  0}, // idx = 0353, oldNumberOfOnes = 1, byte2Encode = 097
  {139,  8,  0}, // idx = 0354, oldNumberOfOnes = 1, byte2Encode = 098
  {116,  8,  0}, // idx = 0355, oldNumberOfOnes = 1, byte2Encode = 099
  {137,  8,  0}, // idx = 0356, oldNumberOfOnes = 1, byte2Encode = 100
  {118,  8,  0}, // idx = 0357, oldNumberOfOnes = 1, byte2Encode = 101
  {119,  8,  0}, // idx = 0358, oldNumberOfOnes = 1, byte2Encode = 102
  {136,  8,  0}, // idx = 0359, oldNumberOfOnes = 1, byte2Encode = 103
  {141,  8,  0}, // idx = 0360, oldNumberOfOnes = 1, byte2Encode = 104
  {114,  8,  0}, // idx = 0361, oldNumberOfOnes = 1, byte2Encode = 105
  {115,  8,  0}, // idx = 0362, oldNumberOfOnes = 1, byte2Encode = 106
  {140,  8,  0}, // idx = 0363, oldNumberOfOnes = 1, byte2Encode = 107
  {113,  8,  0}, // idx = 0364, oldNumberOfOnes = 1, byte2Encode = 108
  {142,  8,  0}, // idx = 0365, oldNumberOfOnes = 1, byte2Encode = 109
  {143,  8,  0}, // idx = 0366, oldNumberOfOnes = 1, byte2Encode = 110
  {272,  9,  0}, // idx = 0367, oldNumberOfOnes = 1, byte2Encode = 111
  {133,  8,  0}, // idx = 0368, oldNumberOfOnes = 1, byte2Encode = 112
  {122,  8,  0}, // idx = 0369, oldNumberOfOnes = 1, byte2Encode = 113
  {123,  8,  0}, // idx = 0370, oldNumberOfOnes = 1, byte2Encode = 114
  {132,  8,  0}, // idx = 0371, oldNumberOfOnes = 1, byte2Encode = 115
  {121,  8,  0}, // idx = 0372, oldNumberOfOnes = 1, byte2Encode = 116
  {134,  8,  0}, // idx = 0373, oldNumberOfOnes = 1, byte2Encode = 117
  {135,  8,  0}, // idx = 0374, oldNumberOfOnes = 1, byte2Encode = 118
  {120,  8,  0}, // idx = 0375, oldNumberOfOnes = 1, byte2Encode = 119
  {125,  8,  0}, // idx = 0376, oldNumberOfOnes = 1, byte2Encode = 120
  {130,  8,  0}, // idx = 0377, oldNumberOfOnes = 1, byte2Encode = 121
  {131,  8,  0}, // idx = 0378, oldNumberOfOnes = 1, byte2Encode = 122
  {124,  8,  0}, // idx = 0379, oldNumberOfOnes = 1, byte2Encode = 123
  {129,  9,  0}, // idx = 0380, oldNumberOfOnes = 1, byte2Encode = 124
  {382,  9,  0}, // idx = 0381, oldNumberOfOnes = 1, byte2Encode = 125
  {319,  9,  0}, // idx = 0382, oldNumberOfOnes = 1, byte2Encode = 126
  {240,  9,  0}, // idx = 0383, oldNumberOfOnes = 1, byte2Encode = 127
  {213,  8,  1}, // idx = 0384, oldNumberOfOnes = 1, byte2Encode = 128
  { 42,  8,  1}, // idx = 0385, oldNumberOfOnes = 1, byte2Encode = 129
  { 43,  8,  1}, // idx = 0386, oldNumberOfOnes = 1, byte2Encode = 130
  {212,  8,  1}, // idx = 0387, oldNumberOfOnes = 1, byte2Encode = 131
  { 41,  8,  1}, // idx = 0388, oldNumberOfOnes = 1, byte2Encode = 132
  {214,  8,  1}, // idx = 0389, oldNumberOfOnes = 1, byte2Encode = 133
  {215,  8,  1}, // idx = 0390, oldNumberOfOnes = 1, byte2Encode = 134
  { 40,  8,  1}, // idx = 0391, oldNumberOfOnes = 1, byte2Encode = 135
  { 45,  8,  1}, // idx = 0392, oldNumberOfOnes = 1, byte2Encode = 136
  {210,  8,  1}, // idx = 0393, oldNumberOfOnes = 1, byte2Encode = 137
  {211,  8,  1}, // idx = 0394, oldNumberOfOnes = 1, byte2Encode = 138
  { 44,  8,  1}, // idx = 0395, oldNumberOfOnes = 1, byte2Encode = 139
  {209,  8,  1}, // idx = 0396, oldNumberOfOnes = 1, byte2Encode = 140
  { 46,  8,  1}, // idx = 0397, oldNumberOfOnes = 1, byte2Encode = 141
  { 47,  8,  1}, // idx = 0398, oldNumberOfOnes = 1, byte2Encode = 142
  { 80,  9,  1}, // idx = 0399, oldNumberOfOnes = 1, byte2Encode = 143
  { 37,  8,  1}, // idx = 0400, oldNumberOfOnes = 1, byte2Encode = 144
  {218,  8,  1}, // idx = 0401, oldNumberOfOnes = 1, byte2Encode = 145
  {219,  8,  1}, // idx = 0402, oldNumberOfOnes = 1, byte2Encode = 146
  { 36,  8,  1}, // idx = 0403, oldNumberOfOnes = 1, byte2Encode = 147
  {217,  8,  1}, // idx = 0404, oldNumberOfOnes = 1, byte2Encode = 148
  { 38,  8,  1}, // idx = 0405, oldNumberOfOnes = 1, byte2Encode = 149
  { 39,  8,  1}, // idx = 0406, oldNumberOfOnes = 1, byte2Encode = 150
  {216,  8,  1}, // idx = 0407, oldNumberOfOnes = 1, byte2Encode = 151
  {221,  8,  1}, // idx = 0408, oldNumberOfOnes = 1, byte2Encode = 152
  { 34,  8,  1}, // idx = 0409, oldNumberOfOnes = 1, byte2Encode = 153
  { 35,  8,  1}, // idx = 0410, oldNumberOfOnes = 1, byte2Encode = 154
  {220,  8,  1}, // idx = 0411, oldNumberOfOnes = 1, byte2Encode = 155
  { 33,  8,  1}, // idx = 0412, oldNumberOfOnes = 1, byte2Encode = 156
  {222,  8,  1}, // idx = 0413, oldNumberOfOnes = 1, byte2Encode = 157
  {223,  8,  1}, // idx = 0414, oldNumberOfOnes = 1, byte2Encode = 158
  {432,  9,  1}, // idx = 0415, oldNumberOfOnes = 1, byte2Encode = 159
  { 53,  8,  1}, // idx = 0416, oldNumberOfOnes = 1, byte2Encode = 160
  {202,  8,  1}, // idx = 0417, oldNumberOfOnes = 1, byte2Encode = 161
  {203,  8,  1}, // idx = 0418, oldNumberOfOnes = 1, byte2Encode = 162
  { 52,  8,  1}, // idx = 0419, oldNumberOfOnes = 1, byte2Encode = 163
  {201,  8,  1}, // idx = 0420, oldNumberOfOnes = 1, byte2Encode = 164
  { 54,  8,  1}, // idx = 0421, oldNumberOfOnes = 1, byte2Encode = 165
  { 55,  8,  1}, // idx = 0422, oldNumberOfOnes = 1, byte2Encode = 166
  {200,  8,  1}, // idx = 0423, oldNumberOfOnes = 1, byte2Encode = 167
  {205,  8,  1}, // idx = 0424, oldNumberOfOnes = 1, byte2Encode = 168
  { 50,  8,  1}, // idx = 0425, oldNumberOfOnes = 1, byte2Encode = 169
  { 51,  8,  1}, // idx = 0426, oldNumberOfOnes = 1, byte2Encode = 170
  {204,  8,  1}, // idx = 0427, oldNumberOfOnes = 1, byte2Encode = 171
  { 49,  8,  1}, // idx = 0428, oldNumberOfOnes = 1, byte2Encode = 172
  {206,  8,  1}, // idx = 0429, oldNumberOfOnes = 1, byte2Encode = 173
  {207,  8,  1}, // idx = 0430, oldNumberOfOnes = 1, byte2Encode = 174
  {400,  9,  1}, // idx = 0431, oldNumberOfOnes = 1, byte2Encode = 175
  {197,  8,  1}, // idx = 0432, oldNumberOfOnes = 1, byte2Encode = 176
  { 58,  8,  1}, // idx = 0433, oldNumberOfOnes = 1, byte2Encode = 177
  { 59,  8,  1}, // idx = 0434, oldNumberOfOnes = 1, byte2Encode = 178
  {196,  8,  1}, // idx = 0435, oldNumberOfOnes = 1, byte2Encode = 179
  { 57,  8,  1}, // idx = 0436, oldNumberOfOnes = 1, byte2Encode = 180
  {198,  8,  1}, // idx = 0437, oldNumberOfOnes = 1, byte2Encode = 181
  {199,  8,  1}, // idx = 0438, oldNumberOfOnes = 1, byte2Encode = 182
  { 56,  8,  1}, // idx = 0439, oldNumberOfOnes = 1, byte2Encode = 183
  { 61,  8,  1}, // idx = 0440, oldNumberOfOnes = 1, byte2Encode = 184
  {194,  8,  1}, // idx = 0441, oldNumberOfOnes = 1, byte2Encode = 185
  {195,  8,  1}, // idx = 0442, oldNumberOfOnes = 1, byte2Encode = 186
  { 60,  8,  1}, // idx = 0443, oldNumberOfOnes = 1, byte2Encode = 187
  {193,  8,  1}, // idx = 0444, oldNumberOfOnes = 1, byte2Encode = 188
  { 62,  8,  1}, // idx = 0445, oldNumberOfOnes = 1, byte2Encode = 189
  {447,  9,  1}, // idx = 0446, oldNumberOfOnes = 1, byte2Encode = 190
  {112,  9,  1}, // idx = 0447, oldNumberOfOnes = 1, byte2Encode = 191
  { 21,  8,  2}, // idx = 0448, oldNumberOfOnes = 1, byte2Encode = 192
  {234,  8,  2}, // idx = 0449, oldNumberOfOnes = 1, byte2Encode = 193
  {235,  8,  2}, // idx = 0450, oldNumberOfOnes = 1, byte2Encode = 194
  { 20,  8,  2}, // idx = 0451, oldNumberOfOnes = 1, byte2Encode = 195
  {233,  8,  2}, // idx = 0452, oldNumberOfOnes = 1, byte2Encode = 196
  { 22,  8,  2}, // idx = 0453, oldNumberOfOnes = 1, byte2Encode = 197
  { 23,  8,  2}, // idx = 0454, oldNumberOfOnes = 1, byte2Encode = 198
  {232,  8,  2}, // idx = 0455, oldNumberOfOnes = 1, byte2Encode = 199
  {237,  8,  2}, // idx = 0456, oldNumberOfOnes = 1, byte2Encode = 200
  { 18,  8,  2}, // idx = 0457, oldNumberOfOnes = 1, byte2Encode = 201
  { 19,  8,  2}, // idx = 0458, oldNumberOfOnes = 1, byte2Encode = 202
  {236,  8,  2}, // idx = 0459, oldNumberOfOnes = 1, byte2Encode = 203
  { 17,  8,  2}, // idx = 0460, oldNumberOfOnes = 1, byte2Encode = 204
  {238,  8,  2}, // idx = 0461, oldNumberOfOnes = 1, byte2Encode = 205
  {239,  8,  2}, // idx = 0462, oldNumberOfOnes = 1, byte2Encode = 206
  {464,  9,  2}, // idx = 0463, oldNumberOfOnes = 1, byte2Encode = 207
  {229,  8,  2}, // idx = 0464, oldNumberOfOnes = 1, byte2Encode = 208
  { 26,  8,  2}, // idx = 0465, oldNumberOfOnes = 1, byte2Encode = 209
  { 27,  8,  2}, // idx = 0466, oldNumberOfOnes = 1, byte2Encode = 210
  {228,  8,  2}, // idx = 0467, oldNumberOfOnes = 1, byte2Encode = 211
  { 25,  8,  2}, // idx = 0468, oldNumberOfOnes = 1, byte2Encode = 212
  {230,  8,  2}, // idx = 0469, oldNumberOfOnes = 1, byte2Encode = 213
  {231,  8,  2}, // idx = 0470, oldNumberOfOnes = 1, byte2Encode = 214
  { 24,  8,  2}, // idx = 0471, oldNumberOfOnes = 1, byte2Encode = 215
  { 29,  8,  2}, // idx = 0472, oldNumberOfOnes = 1, byte2Encode = 216
  {226,  8,  2}, // idx = 0473, oldNumberOfOnes = 1, byte2Encode = 217
  {227,  8,  2}, // idx = 0474, oldNumberOfOnes = 1, byte2Encode = 218
  { 28,  8,  2}, // idx = 0475, oldNumberOfOnes = 1, byte2Encode = 219
  {225,  8,  2}, // idx = 0476, oldNumberOfOnes = 1, byte2Encode = 220
  { 30,  8,  2}, // idx = 0477, oldNumberOfOnes = 1, byte2Encode = 221
  { 31,  8,  2}, // idx = 0478, oldNumberOfOnes = 1, byte2Encode = 222
  { 48,  9,  2}, // idx = 0479, oldNumberOfOnes = 1, byte2Encode = 223
  {245,  8,  3}, // idx = 0480, oldNumberOfOnes = 1, byte2Encode = 224
  { 10,  8,  3}, // idx = 0481, oldNumberOfOnes = 1, byte2Encode = 225
  { 11,  8,  3}, // idx = 0482, oldNumberOfOnes = 1, byte2Encode = 226
  {244,  8,  3}, // idx = 0483, oldNumberOfOnes = 1, byte2Encode = 227
  {  9,  8,  3}, // idx = 0484, oldNumberOfOnes = 1, byte2Encode = 228
  {246,  8,  3}, // idx = 0485, oldNumberOfOnes = 1, byte2Encode = 229
  {247,  8,  3}, // idx = 0486, oldNumberOfOnes = 1, byte2Encode = 230
  {  8,  8,  3}, // idx = 0487, oldNumberOfOnes = 1, byte2Encode = 231
  { 13,  8,  3}, // idx = 0488, oldNumberOfOnes = 1, byte2Encode = 232
  {242,  8,  3}, // idx = 0489, oldNumberOfOnes = 1, byte2Encode = 233
  {243,  8,  3}, // idx = 0490, oldNumberOfOnes = 1, byte2Encode = 234
  { 12,  8,  3}, // idx = 0491, oldNumberOfOnes = 1, byte2Encode = 235
  {241,  8,  3}, // idx = 0492, oldNumberOfOnes = 1, byte2Encode = 236
  { 14,  8,  3}, // idx = 0493, oldNumberOfOnes = 1, byte2Encode = 237
  { 15,  8,  3}, // idx = 0494, oldNumberOfOnes = 1, byte2Encode = 238
  { 16,  9,  3}, // idx = 0495, oldNumberOfOnes = 1, byte2Encode = 239
  {  5,  8,  4}, // idx = 0496, oldNumberOfOnes = 1, byte2Encode = 240
  {250,  8,  4}, // idx = 0497, oldNumberOfOnes = 1, byte2Encode = 241
  {251,  8,  4}, // idx = 0498, oldNumberOfOnes = 1, byte2Encode = 242
  {  4,  8,  4}, // idx = 0499, oldNumberOfOnes = 1, byte2Encode = 243
  {249,  8,  4}, // idx = 0500, oldNumberOfOnes = 1, byte2Encode = 244
  {  6,  8,  4}, // idx = 0501, oldNumberOfOnes = 1, byte2Encode = 245
  {  7,  8,  4}, // idx = 0502, oldNumberOfOnes = 1, byte2Encode = 246
  {248,  8,  4}, // idx = 0503, oldNumberOfOnes = 1, byte2Encode = 247
  {253,  9,  0}, // idx = 0504, oldNumberOfOnes = 1, byte2Encode = 248
  {258,  9,  0}, // idx = 0505, oldNumberOfOnes = 1, byte2Encode = 249
  {259,  9,  0}, // idx = 0506, oldNumberOfOnes = 1, byte2Encode = 250
  {252,  9,  0}, // idx = 0507, oldNumberOfOnes = 1, byte2Encode = 251
  {385,  9,  1}, // idx = 0508, oldNumberOfOnes = 1, byte2Encode = 252
  {126,  9,  1}, // idx = 0509, oldNumberOfOnes = 1, byte2Encode = 253
  { 63,  9,  2}, // idx = 0510, oldNumberOfOnes = 1, byte2Encode = 254
  {496,  9,  4}, // idx = 0511, oldNumberOfOnes = 1, byte2Encode = 255
  { 85,  8,  0}, // idx = 0512, oldNumberOfOnes = 2, byte2Encode = 000
  {170,  8,  0}, // idx = 0513, oldNumberOfOnes = 2, byte2Encode = 001
  {171,  8,  0}, // idx = 0514, oldNumberOfOnes = 2, byte2Encode = 002
  { 84,  8,  0}, // idx = 0515, oldNumberOfOnes = 2, byte2Encode = 003
  {169,  8,  0}, // idx = 0516, oldNumberOfOnes = 2, byte2Encode = 004
  { 86,  8,  0}, // idx = 0517, oldNumberOfOnes = 2, byte2Encode = 005
  { 87,  8,  0}, // idx = 0518, oldNumberOfOnes = 2, byte2Encode = 006
  {168,  9,  0}, // idx = 0519, oldNumberOfOnes = 2, byte2Encode = 007
  {173,  8,  0}, // idx = 0520, oldNumberOfOnes = 2, byte2Encode = 008
  { 82,  8,  0}, // idx = 0521, oldNumberOfOnes = 2, byte2Encode = 009
  { 83,  8,  0}, // idx = 0522, oldNumberOfOnes = 2, byte2Encode = 010
  {172,  8,  0}, // idx = 0523, oldNumberOfOnes = 2, byte2Encode = 011
  { 81,  8,  0}, // idx = 0524, oldNumberOfOnes = 2, byte2Encode = 012
  {174,  8,  0}, // idx = 0525, oldNumberOfOnes = 2, byte2Encode = 013
  {175,  8,  0}, // idx = 0526, oldNumberOfOnes = 2, byte2Encode = 014
  {344,  9,  0}, // idx = 0527, oldNumberOfOnes = 2, byte2Encode = 015
  {165,  8,  0}, // idx = 0528, oldNumberOfOnes = 2, byte2Encode = 016
  { 90,  8,  0}, // idx = 0529, oldNumberOfOnes = 2, byte2Encode = 017
  { 91,  8,  0}, // idx = 0530, oldNumberOfOnes = 2, byte2Encode = 018
  {164,  8,  0}, // idx = 0531, oldNumberOfOnes = 2, byte2Encode = 019
  { 89,  8,  0}, // idx = 0532, oldNumberOfOnes = 2, byte2Encode = 020
  {166,  8,  0}, // idx = 0533, oldNumberOfOnes = 2, byte2Encode = 021
  {167,  8,  0}, // idx = 0534, oldNumberOfOnes = 2, byte2Encode = 022
  {328,  9,  0}, // idx = 0535, oldNumberOfOnes = 2, byte2Encode = 023
  { 93,  8,  0}, // idx = 0536, oldNumberOfOnes = 2, byte2Encode = 024
  {162,  8,  0}, // idx = 0537, oldNumberOfOnes = 2, byte2Encode = 025
  {163,  8,  0}, // idx = 0538, oldNumberOfOnes = 2, byte2Encode = 026
  { 92,  8,  0}, // idx = 0539, oldNumberOfOnes = 2, byte2Encode = 027
  {161,  8,  0}, // idx = 0540, oldNumberOfOnes = 2, byte2Encode = 028
  { 94,  8,  0}, // idx = 0541, oldNumberOfOnes = 2, byte2Encode = 029
  { 95,  8,  0}, // idx = 0542, oldNumberOfOnes = 2, byte2Encode = 030
  {184,  9,  0}, // idx = 0543, oldNumberOfOnes = 2, byte2Encode = 031
  {181,  8,  0}, // idx = 0544, oldNumberOfOnes = 2, byte2Encode = 032
  { 74,  8,  0}, // idx = 0545, oldNumberOfOnes = 2, byte2Encode = 033
  { 75,  8,  0}, // idx = 0546, oldNumberOfOnes = 2, byte2Encode = 034
  {180,  8,  0}, // idx = 0547, oldNumberOfOnes = 2, byte2Encode = 035
  { 73,  8,  0}, // idx = 0548, oldNumberOfOnes = 2, byte2Encode = 036
  {182,  8,  0}, // idx = 0549, oldNumberOfOnes = 2, byte2Encode = 037
  {183,  8,  0}, // idx = 0550, oldNumberOfOnes = 2, byte2Encode = 038
  {360,  9,  0}, // idx = 0551, oldNumberOfOnes = 2, byte2Encode = 039
  { 77,  8,  0}, // idx = 0552, oldNumberOfOnes = 2, byte2Encode = 040
  {178,  8,  0}, // idx = 0553, oldNumberOfOnes = 2, byte2Encode = 041
  {179,  8,  0}, // idx = 0554, oldNumberOfOnes = 2, byte2Encode = 042
  { 76,  8,  0}, // idx = 0555, oldNumberOfOnes = 2, byte2Encode = 043
  {177,  8,  0}, // idx = 0556, oldNumberOfOnes = 2, byte2Encode = 044
  { 78,  8,  0}, // idx = 0557, oldNumberOfOnes = 2, byte2Encode = 045
  { 79,  8,  0}, // idx = 0558, oldNumberOfOnes = 2, byte2Encode = 046
  {152,  9,  0}, // idx = 0559, oldNumberOfOnes = 2, byte2Encode = 047
  { 69,  8,  0}, // idx = 0560, oldNumberOfOnes = 2, byte2Encode = 048
  {186,  8,  0}, // idx = 0561, oldNumberOfOnes = 2, byte2Encode = 049
  {187,  8,  0}, // idx = 0562, oldNumberOfOnes = 2, byte2Encode = 050
  { 68,  8,  0}, // idx = 0563, oldNumberOfOnes = 2, byte2Encode = 051
  {185,  8,  0}, // idx = 0564, oldNumberOfOnes = 2, byte2Encode = 052
  { 70,  8,  0}, // idx = 0565, oldNumberOfOnes = 2, byte2Encode = 053
  { 71,  8,  0}, // idx = 0566, oldNumberOfOnes = 2, byte2Encode = 054
  {136,  9,  0}, // idx = 0567, oldNumberOfOnes = 2, byte2Encode = 055
  {189,  8,  0}, // idx = 0568, oldNumberOfOnes = 2, byte2Encode = 056
  { 66,  8,  0}, // idx = 0569, oldNumberOfOnes = 2, byte2Encode = 057
  { 67,  8,  0}, // idx = 0570, oldNumberOfOnes = 2, byte2Encode = 058
  {188,  8,  0}, // idx = 0571, oldNumberOfOnes = 2, byte2Encode = 059
  { 65,  8,  0}, // idx = 0572, oldNumberOfOnes = 2, byte2Encode = 060
  {190,  8,  0}, // idx = 0573, oldNumberOfOnes = 2, byte2Encode = 061
  {191,  9,  0}, // idx = 0574, oldNumberOfOnes = 2, byte2Encode = 062
  {376,  9,  0}, // idx = 0575, oldNumberOfOnes = 2, byte2Encode = 063
  {149,  8,  0}, // idx = 0576, oldNumberOfOnes = 2, byte2Encode = 064
  {106,  8,  0}, // idx = 0577, oldNumberOfOnes = 2, byte2Encode = 065
  {107,  8,  0}, // idx = 0578, oldNumberOfOnes = 2, byte2Encode = 066
  {148,  8,  0}, // idx = 0579, oldNumberOfOnes = 2, byte2Encode = 067
  {105,  8,  0}, // idx = 0580, oldNumberOfOnes = 2, byte2Encode = 068
  {150,  8,  0}, // idx = 0581, oldNumberOfOnes = 2, byte2Encode = 069
  {151,  8,  0}, // idx = 0582, oldNumberOfOnes = 2, byte2Encode = 070
  {296,  9,  0}, // idx = 0583, oldNumberOfOnes = 2, byte2Encode = 071
  {109,  8,  0}, // idx = 0584, oldNumberOfOnes = 2, byte2Encode = 072
  {146,  8,  0}, // idx = 0585, oldNumberOfOnes = 2, byte2Encode = 073
  {147,  8,  0}, // idx = 0586, oldNumberOfOnes = 2, byte2Encode = 074
  {108,  8,  0}, // idx = 0587, oldNumberOfOnes = 2, byte2Encode = 075
  {145,  8,  0}, // idx = 0588, oldNumberOfOnes = 2, byte2Encode = 076
  {110,  8,  0}, // idx = 0589, oldNumberOfOnes = 2, byte2Encode = 077
  {111,  8,  0}, // idx = 0590, oldNumberOfOnes = 2, byte2Encode = 078
  {216,  9,  0}, // idx = 0591, oldNumberOfOnes = 2, byte2Encode = 079
  {101,  8,  0}, // idx = 0592, oldNumberOfOnes = 2, byte2Encode = 080
  {154,  8,  0}, // idx = 0593, oldNumberOfOnes = 2, byte2Encode = 081
  {155,  8,  0}, // idx = 0594, oldNumberOfOnes = 2, byte2Encode = 082
  {100,  8,  0}, // idx = 0595, oldNumberOfOnes = 2, byte2Encode = 083
  {153,  8,  0}, // idx = 0596, oldNumberOfOnes = 2, byte2Encode = 084
  {102,  8,  0}, // idx = 0597, oldNumberOfOnes = 2, byte2Encode = 085
  {103,  8,  0}, // idx = 0598, oldNumberOfOnes = 2, byte2Encode = 086
  {200,  9,  0}, // idx = 0599, oldNumberOfOnes = 2, byte2Encode = 087
  {157,  8,  0}, // idx = 0600, oldNumberOfOnes = 2, byte2Encode = 088
  { 98,  8,  0}, // idx = 0601, oldNumberOfOnes = 2, byte2Encode = 089
  { 99,  8,  0}, // idx = 0602, oldNumberOfOnes = 2, byte2Encode = 090
  {156,  8,  0}, // idx = 0603, oldNumberOfOnes = 2, byte2Encode = 091
  { 97,  8,  0}, // idx = 0604, oldNumberOfOnes = 2, byte2Encode = 092
  {158,  8,  0}, // idx = 0605, oldNumberOfOnes = 2, byte2Encode = 093
  {159,  8,  0}, // idx = 0606, oldNumberOfOnes = 2, byte2Encode = 094
  {312,  9,  0}, // idx = 0607, oldNumberOfOnes = 2, byte2Encode = 095
  {117,  8,  0}, // idx = 0608, oldNumberOfOnes = 2, byte2Encode = 096
  {138,  8,  0}, // idx = 0609, oldNumberOfOnes = 2, byte2Encode = 097
  {139,  8,  0}, // idx = 0610, oldNumberOfOnes = 2, byte2Encode = 098
  {116,  8,  0}, // idx = 0611, oldNumberOfOnes = 2, byte2Encode = 099
  {137,  8,  0}, // idx = 0612, oldNumberOfOnes = 2, byte2Encode = 100
  {118,  8,  0}, // idx = 0613, oldNumberOfOnes = 2, byte2Encode = 101
  {119,  8,  0}, // idx = 0614, oldNumberOfOnes = 2, byte2Encode = 102
  {232,  9,  0}, // idx = 0615, oldNumberOfOnes = 2, byte2Encode = 103
  {141,  8,  0}, // idx = 0616, oldNumberOfOnes = 2, byte2Encode = 104
  {114,  8,  0}, // idx = 0617, oldNumberOfOnes = 2, byte2Encode = 105
  {115,  8,  0}, // idx = 0618, oldNumberOfOnes = 2, byte2Encode = 106
  {140,  8,  0}, // idx = 0619, oldNumberOfOnes = 2, byte2Encode = 107
  {113,  8,  0}, // idx = 0620, oldNumberOfOnes = 2, byte2Encode = 108
  {142,  8,  0}, // idx = 0621, oldNumberOfOnes = 2, byte2Encode = 109
  {143,  8,  0}, // idx = 0622, oldNumberOfOnes = 2, byte2Encode = 110
  {280,  9,  0}, // idx = 0623, oldNumberOfOnes = 2, byte2Encode = 111
  {133,  8,  0}, // idx = 0624, oldNumberOfOnes = 2, byte2Encode = 112
  {122,  8,  0}, // idx = 0625, oldNumberOfOnes = 2, byte2Encode = 113
  {123,  8,  0}, // idx = 0626, oldNumberOfOnes = 2, byte2Encode = 114
  {132,  8,  0}, // idx = 0627, oldNumberOfOnes = 2, byte2Encode = 115
  {121,  8,  0}, // idx = 0628, oldNumberOfOnes = 2, byte2Encode = 116
  {134,  8,  0}, // idx = 0629, oldNumberOfOnes = 2, byte2Encode = 117
  {135,  8,  0}, // idx = 0630, oldNumberOfOnes = 2, byte2Encode = 118
  {264,  9,  0}, // idx = 0631, oldNumberOfOnes = 2, byte2Encode = 119
  {125,  8,  0}, // idx = 0632, oldNumberOfOnes = 2, byte2Encode = 120
  {130,  8,  0}, // idx = 0633, oldNumberOfOnes = 2, byte2Encode = 121
  {131,  8,  0}, // idx = 0634, oldNumberOfOnes = 2, byte2Encode = 122
  {124,  8,  0}, // idx = 0635, oldNumberOfOnes = 2, byte2Encode = 123
  {129,  9,  0}, // idx = 0636, oldNumberOfOnes = 2, byte2Encode = 124
  {382,  9,  0}, // idx = 0637, oldNumberOfOnes = 2, byte2Encode = 125
  {319,  9,  0}, // idx = 0638, oldNumberOfOnes = 2, byte2Encode = 126
  {248,  9,  0}, // idx = 0639, oldNumberOfOnes = 2, byte2Encode = 127
  {213,  8,  1}, // idx = 0640, oldNumberOfOnes = 2, byte2Encode = 128
  { 42,  8,  1}, // idx = 0641, oldNumberOfOnes = 2, byte2Encode = 129
  { 43,  8,  1}, // idx = 0642, oldNumberOfOnes = 2, byte2Encode = 130
  {212,  8,  1}, // idx = 0643, oldNumberOfOnes = 2, byte2Encode = 131
  { 41,  8,  1}, // idx = 0644, oldNumberOfOnes = 2, byte2Encode = 132
  {214,  8,  1}, // idx = 0645, oldNumberOfOnes = 2, byte2Encode = 133
  {215,  8,  1}, // idx = 0646, oldNumberOfOnes = 2, byte2Encode = 134
  {424,  9,  1}, // idx = 0647, oldNumberOfOnes = 2, byte2Encode = 135
  { 45,  8,  1}, // idx = 0648, oldNumberOfOnes = 2, byte2Encode = 136
  {210,  8,  1}, // idx = 0649, oldNumberOfOnes = 2, byte2Encode = 137
  {211,  8,  1}, // idx = 0650, oldNumberOfOnes = 2, byte2Encode = 138
  { 44,  8,  1}, // idx = 0651, oldNumberOfOnes = 2, byte2Encode = 139
  {209,  8,  1}, // idx = 0652, oldNumberOfOnes = 2, byte2Encode = 140
  { 46,  8,  1}, // idx = 0653, oldNumberOfOnes = 2, byte2Encode = 141
  { 47,  8,  1}, // idx = 0654, oldNumberOfOnes = 2, byte2Encode = 142
  { 88,  9,  1}, // idx = 0655, oldNumberOfOnes = 2, byte2Encode = 143
  { 37,  8,  1}, // idx = 0656, oldNumberOfOnes = 2, byte2Encode = 144
  {218,  8,  1}, // idx = 0657, oldNumberOfOnes = 2, byte2Encode = 145
  {219,  8,  1}, // idx = 0658, oldNumberOfOnes = 2, byte2Encode = 146
  { 36,  8,  1}, // idx = 0659, oldNumberOfOnes = 2, byte2Encode = 147
  {217,  8,  1}, // idx = 0660, oldNumberOfOnes = 2, byte2Encode = 148
  { 38,  8,  1}, // idx = 0661, oldNumberOfOnes = 2, byte2Encode = 149
  { 39,  8,  1}, // idx = 0662, oldNumberOfOnes = 2, byte2Encode = 150
  { 72,  9,  1}, // idx = 0663, oldNumberOfOnes = 2, byte2Encode = 151
  {221,  8,  1}, // idx = 0664, oldNumberOfOnes = 2, byte2Encode = 152
  { 34,  8,  1}, // idx = 0665, oldNumberOfOnes = 2, byte2Encode = 153
  { 35,  8,  1}, // idx = 0666, oldNumberOfOnes = 2, byte2Encode = 154
  {220,  8,  1}, // idx = 0667, oldNumberOfOnes = 2, byte2Encode = 155
  { 33,  8,  1}, // idx = 0668, oldNumberOfOnes = 2, byte2Encode = 156
  {222,  8,  1}, // idx = 0669, oldNumberOfOnes = 2, byte2Encode = 157
  {223,  8,  1}, // idx = 0670, oldNumberOfOnes = 2, byte2Encode = 158
  {440,  9,  1}, // idx = 0671, oldNumberOfOnes = 2, byte2Encode = 159
  { 53,  8,  1}, // idx = 0672, oldNumberOfOnes = 2, byte2Encode = 160
  {202,  8,  1}, // idx = 0673, oldNumberOfOnes = 2, byte2Encode = 161
  {203,  8,  1}, // idx = 0674, oldNumberOfOnes = 2, byte2Encode = 162
  { 52,  8,  1}, // idx = 0675, oldNumberOfOnes = 2, byte2Encode = 163
  {201,  8,  1}, // idx = 0676, oldNumberOfOnes = 2, byte2Encode = 164
  { 54,  8,  1}, // idx = 0677, oldNumberOfOnes = 2, byte2Encode = 165
  { 55,  8,  1}, // idx = 0678, oldNumberOfOnes = 2, byte2Encode = 166
  {104,  9,  1}, // idx = 0679, oldNumberOfOnes = 2, byte2Encode = 167
  {205,  8,  1}, // idx = 0680, oldNumberOfOnes = 2, byte2Encode = 168
  { 50,  8,  1}, // idx = 0681, oldNumberOfOnes = 2, byte2Encode = 169
  { 51,  8,  1}, // idx = 0682, oldNumberOfOnes = 2, byte2Encode = 170
  {204,  8,  1}, // idx = 0683, oldNumberOfOnes = 2, byte2Encode = 171
  { 49,  8,  1}, // idx = 0684, oldNumberOfOnes = 2, byte2Encode = 172
  {206,  8,  1}, // idx = 0685, oldNumberOfOnes = 2, byte2Encode = 173
  {207,  8,  1}, // idx = 0686, oldNumberOfOnes = 2, byte2Encode = 174
  {408,  9,  1}, // idx = 0687, oldNumberOfOnes = 2, byte2Encode = 175
  {197,  8,  1}, // idx = 0688, oldNumberOfOnes = 2, byte2Encode = 176
  { 58,  8,  1}, // idx = 0689, oldNumberOfOnes = 2, byte2Encode = 177
  { 59,  8,  1}, // idx = 0690, oldNumberOfOnes = 2, byte2Encode = 178
  {196,  8,  1}, // idx = 0691, oldNumberOfOnes = 2, byte2Encode = 179
  { 57,  8,  1}, // idx = 0692, oldNumberOfOnes = 2, byte2Encode = 180
  {198,  8,  1}, // idx = 0693, oldNumberOfOnes = 2, byte2Encode = 181
  {199,  8,  1}, // idx = 0694, oldNumberOfOnes = 2, byte2Encode = 182
  {392,  9,  1}, // idx = 0695, oldNumberOfOnes = 2, byte2Encode = 183
  { 61,  8,  1}, // idx = 0696, oldNumberOfOnes = 2, byte2Encode = 184
  {194,  8,  1}, // idx = 0697, oldNumberOfOnes = 2, byte2Encode = 185
  {195,  8,  1}, // idx = 0698, oldNumberOfOnes = 2, byte2Encode = 186
  { 60,  8,  1}, // idx = 0699, oldNumberOfOnes = 2, byte2Encode = 187
  {193,  8,  1}, // idx = 0700, oldNumberOfOnes = 2, byte2Encode = 188
  { 62,  8,  1}, // idx = 0701, oldNumberOfOnes = 2, byte2Encode = 189
  {447,  9,  1}, // idx = 0702, oldNumberOfOnes = 2, byte2Encode = 190
  {120,  9,  1}, // idx = 0703, oldNumberOfOnes = 2, byte2Encode = 191
  { 21,  8,  2}, // idx = 0704, oldNumberOfOnes = 2, byte2Encode = 192
  {234,  8,  2}, // idx = 0705, oldNumberOfOnes = 2, byte2Encode = 193
  {235,  8,  2}, // idx = 0706, oldNumberOfOnes = 2, byte2Encode = 194
  { 20,  8,  2}, // idx = 0707, oldNumberOfOnes = 2, byte2Encode = 195
  {233,  8,  2}, // idx = 0708, oldNumberOfOnes = 2, byte2Encode = 196
  { 22,  8,  2}, // idx = 0709, oldNumberOfOnes = 2, byte2Encode = 197
  { 23,  8,  2}, // idx = 0710, oldNumberOfOnes = 2, byte2Encode = 198
  { 40,  9,  2}, // idx = 0711, oldNumberOfOnes = 2, byte2Encode = 199
  {237,  8,  2}, // idx = 0712, oldNumberOfOnes = 2, byte2Encode = 200
  { 18,  8,  2}, // idx = 0713, oldNumberOfOnes = 2, byte2Encode = 201
  { 19,  8,  2}, // idx = 0714, oldNumberOfOnes = 2, byte2Encode = 202
  {236,  8,  2}, // idx = 0715, oldNumberOfOnes = 2, byte2Encode = 203
  { 17,  8,  2}, // idx = 0716, oldNumberOfOnes = 2, byte2Encode = 204
  {238,  8,  2}, // idx = 0717, oldNumberOfOnes = 2, byte2Encode = 205
  {239,  8,  2}, // idx = 0718, oldNumberOfOnes = 2, byte2Encode = 206
  {472,  9,  2}, // idx = 0719, oldNumberOfOnes = 2, byte2Encode = 207
  {229,  8,  2}, // idx = 0720, oldNumberOfOnes = 2, byte2Encode = 208
  { 26,  8,  2}, // idx = 0721, oldNumberOfOnes = 2, byte2Encode = 209
  { 27,  8,  2}, // idx = 0722, oldNumberOfOnes = 2, byte2Encode = 210
  {228,  8,  2}, // idx = 0723, oldNumberOfOnes = 2, byte2Encode = 211
  { 25,  8,  2}, // idx = 0724, oldNumberOfOnes = 2, byte2Encode = 212
  {230,  8,  2}, // idx = 0725, oldNumberOfOnes = 2, byte2Encode = 213
  {231,  8,  2}, // idx = 0726, oldNumberOfOnes = 2, byte2Encode = 214
  {456,  9,  2}, // idx = 0727, oldNumberOfOnes = 2, byte2Encode = 215
  { 29,  8,  2}, // idx = 0728, oldNumberOfOnes = 2, byte2Encode = 216
  {226,  8,  2}, // idx = 0729, oldNumberOfOnes = 2, byte2Encode = 217
  {227,  8,  2}, // idx = 0730, oldNumberOfOnes = 2, byte2Encode = 218
  { 28,  8,  2}, // idx = 0731, oldNumberOfOnes = 2, byte2Encode = 219
  {225,  8,  2}, // idx = 0732, oldNumberOfOnes = 2, byte2Encode = 220
  { 30,  8,  2}, // idx = 0733, oldNumberOfOnes = 2, byte2Encode = 221
  { 31,  8,  2}, // idx = 0734, oldNumberOfOnes = 2, byte2Encode = 222
  { 56,  9,  2}, // idx = 0735, oldNumberOfOnes = 2, byte2Encode = 223
  {245,  8,  3}, // idx = 0736, oldNumberOfOnes = 2, byte2Encode = 224
  { 10,  8,  3}, // idx = 0737, oldNumberOfOnes = 2, byte2Encode = 225
  { 11,  8,  3}, // idx = 0738, oldNumberOfOnes = 2, byte2Encode = 226
  {244,  8,  3}, // idx = 0739, oldNumberOfOnes = 2, byte2Encode = 227
  {  9,  8,  3}, // idx = 0740, oldNumberOfOnes = 2, byte2Encode = 228
  {246,  8,  3}, // idx = 0741, oldNumberOfOnes = 2, byte2Encode = 229
  {247,  8,  3}, // idx = 0742, oldNumberOfOnes = 2, byte2Encode = 230
  {488,  9,  3}, // idx = 0743, oldNumberOfOnes = 2, byte2Encode = 231
  { 13,  8,  3}, // idx = 0744, oldNumberOfOnes = 2, byte2Encode = 232
  {242,  8,  3}, // idx = 0745, oldNumberOfOnes = 2, byte2Encode = 233
  {243,  8,  3}, // idx = 0746, oldNumberOfOnes = 2, byte2Encode = 234
  { 12,  8,  3}, // idx = 0747, oldNumberOfOnes = 2, byte2Encode = 235
  {241,  8,  3}, // idx = 0748, oldNumberOfOnes = 2, byte2Encode = 236
  { 14,  8,  3}, // idx = 0749, oldNumberOfOnes = 2, byte2Encode = 237
  { 15,  8,  3}, // idx = 0750, oldNumberOfOnes = 2, byte2Encode = 238
  { 24,  9,  3}, // idx = 0751, oldNumberOfOnes = 2, byte2Encode = 239
  {  5,  8,  4}, // idx = 0752, oldNumberOfOnes = 2, byte2Encode = 240
  {250,  8,  4}, // idx = 0753, oldNumberOfOnes = 2, byte2Encode = 241
  {251,  8,  4}, // idx = 0754, oldNumberOfOnes = 2, byte2Encode = 242
  {  4,  8,  4}, // idx = 0755, oldNumberOfOnes = 2, byte2Encode = 243
  {249,  8,  4}, // idx = 0756, oldNumberOfOnes = 2, byte2Encode = 244
  {  6,  8,  4}, // idx = 0757, oldNumberOfOnes = 2, byte2Encode = 245
  {  7,  8,  4}, // idx = 0758, oldNumberOfOnes = 2, byte2Encode = 246
  {  8,  9,  4}, // idx = 0759, oldNumberOfOnes = 2, byte2Encode = 247
  {253,  9,  0}, // idx = 0760, oldNumberOfOnes = 2, byte2Encode = 248
  {258,  9,  0}, // idx = 0761, oldNumberOfOnes = 2, byte2Encode = 249
  {259,  9,  0}, // idx = 0762, oldNumberOfOnes = 2, byte2Encode = 250
  {252,  9,  0}, // idx = 0763, oldNumberOfOnes = 2, byte2Encode = 251
  {385,  9,  1}, // idx = 0764, oldNumberOfOnes = 2, byte2Encode = 252
  {126,  9,  1}, // idx = 0765, oldNumberOfOnes = 2, byte2Encode = 253
  { 63,  9,  2}, // idx = 0766, oldNumberOfOnes = 2, byte2Encode = 254
  {504, 10,  0}, // idx = 0767, oldNumberOfOnes = 2, byte2Encode = 255
  { 85,  8,  0}, // idx = 0768, oldNumberOfOnes = 3, byte2Encode = 000
  {170,  8,  0}, // idx = 0769, oldNumberOfOnes = 3, byte2Encode = 001
  {171,  8,  0}, // idx = 0770, oldNumberOfOnes = 3, byte2Encode = 002
  {340,  9,  0}, // idx = 0771, oldNumberOfOnes = 3, byte2Encode = 003
  {169,  8,  0}, // idx = 0772, oldNumberOfOnes = 3, byte2Encode = 004
  { 86,  8,  0}, // idx = 0773, oldNumberOfOnes = 3, byte2Encode = 005
  { 87,  8,  0}, // idx = 0774, oldNumberOfOnes = 3, byte2Encode = 006
  {172,  9,  0}, // idx = 0775, oldNumberOfOnes = 3, byte2Encode = 007
  {173,  8,  0}, // idx = 0776, oldNumberOfOnes = 3, byte2Encode = 008
  { 82,  8,  0}, // idx = 0777, oldNumberOfOnes = 3, byte2Encode = 009
  { 83,  8,  0}, // idx = 0778, oldNumberOfOnes = 3, byte2Encode = 010
  {164,  9,  0}, // idx = 0779, oldNumberOfOnes = 3, byte2Encode = 011
  { 81,  8,  0}, // idx = 0780, oldNumberOfOnes = 3, byte2Encode = 012
  {174,  8,  0}, // idx = 0781, oldNumberOfOnes = 3, byte2Encode = 013
  {175,  8,  0}, // idx = 0782, oldNumberOfOnes = 3, byte2Encode = 014
  {348,  9,  0}, // idx = 0783, oldNumberOfOnes = 3, byte2Encode = 015
  {165,  8,  0}, // idx = 0784, oldNumberOfOnes = 3, byte2Encode = 016
  { 90,  8,  0}, // idx = 0785, oldNumberOfOnes = 3, byte2Encode = 017
  { 91,  8,  0}, // idx = 0786, oldNumberOfOnes = 3, byte2Encode = 018
  {180,  9,  0}, // idx = 0787, oldNumberOfOnes = 3, byte2Encode = 019
  { 89,  8,  0}, // idx = 0788, oldNumberOfOnes = 3, byte2Encode = 020
  {166,  8,  0}, // idx = 0789, oldNumberOfOnes = 3, byte2Encode = 021
  {167,  8,  0}, // idx = 0790, oldNumberOfOnes = 3, byte2Encode = 022
  {332,  9,  0}, // idx = 0791, oldNumberOfOnes = 3, byte2Encode = 023
  { 93,  8,  0}, // idx = 0792, oldNumberOfOnes = 3, byte2Encode = 024
  {162,  8,  0}, // idx = 0793, oldNumberOfOnes = 3, byte2Encode = 025
  {163,  8,  0}, // idx = 0794, oldNumberOfOnes = 3, byte2Encode = 026
  {324,  9,  0}, // idx = 0795, oldNumberOfOnes = 3, byte2Encode = 027
  {161,  8,  0}, // idx = 0796, oldNumberOfOnes = 3, byte2Encode = 028
  { 94,  8,  0}, // idx = 0797, oldNumberOfOnes = 3, byte2Encode = 029
  { 95,  8,  0}, // idx = 0798, oldNumberOfOnes = 3, byte2Encode = 030
  {188,  9,  0}, // idx = 0799, oldNumberOfOnes = 3, byte2Encode = 031
  {181,  8,  0}, // idx = 0800, oldNumberOfOnes = 3, byte2Encode = 032
  { 74,  8,  0}, // idx = 0801, oldNumberOfOnes = 3, byte2Encode = 033
  { 75,  8,  0}, // idx = 0802, oldNumberOfOnes = 3, byte2Encode = 034
  {148,  9,  0}, // idx = 0803, oldNumberOfOnes = 3, byte2Encode = 035
  { 73,  8,  0}, // idx = 0804, oldNumberOfOnes = 3, byte2Encode = 036
  {182,  8,  0}, // idx = 0805, oldNumberOfOnes = 3, byte2Encode = 037
  {183,  8,  0}, // idx = 0806, oldNumberOfOnes = 3, byte2Encode = 038
  {364,  9,  0}, // idx = 0807, oldNumberOfOnes = 3, byte2Encode = 039
  { 77,  8,  0}, // idx = 0808, oldNumberOfOnes = 3, byte2Encode = 040
  {178,  8,  0}, // idx = 0809, oldNumberOfOnes = 3, byte2Encode = 041
  {179,  8,  0}, // idx = 0810, oldNumberOfOnes = 3, byte2Encode = 042
  {356,  9,  0}, // idx = 0811, oldNumberOfOnes = 3, byte2Encode = 043
  {177,  8,  0}, // idx = 0812, oldNumberOfOnes = 3, byte2Encode = 044
  { 78,  8,  0}, // idx = 0813, oldNumberOfOnes = 3, byte2Encode = 045
  { 79,  8,  0}, // idx = 0814, oldNumberOfOnes = 3, byte2Encode = 046
  {156,  9,  0}, // idx = 0815, oldNumberOfOnes = 3, byte2Encode = 047
  { 69,  8,  0}, // idx = 0816, oldNumberOfOnes = 3, byte2Encode = 048
  {186,  8,  0}, // idx = 0817, oldNumberOfOnes = 3, byte2Encode = 049
  {187,  8,  0}, // idx = 0818, oldNumberOfOnes = 3, byte2Encode = 050
  {372,  9,  0}, // idx = 0819, oldNumberOfOnes = 3, byte2Encode = 051
  {185,  8,  0}, // idx = 0820, oldNumberOfOnes = 3, byte2Encode = 052
  { 70,  8,  0}, // idx = 0821, oldNumberOfOnes = 3, byte2Encode = 053
  { 71,  8,  0}, // idx = 0822, oldNumberOfOnes = 3, byte2Encode = 054
  {140,  9,  0}, // idx = 0823, oldNumberOfOnes = 3, byte2Encode = 055
  {189,  8,  0}, // idx = 0824, oldNumberOfOnes = 3, byte2Encode = 056
  { 66,  8,  0}, // idx = 0825, oldNumberOfOnes = 3, byte2Encode = 057
  { 67,  8,  0}, // idx = 0826, oldNumberOfOnes = 3, byte2Encode = 058
  {132,  9,  0}, // idx = 0827, oldNumberOfOnes = 3, byte2Encode = 059
  { 65,  8,  0}, // idx = 0828, oldNumberOfOnes = 3, byte2Encode = 060
  {190,  8,  0}, // idx = 0829, oldNumberOfOnes = 3, byte2Encode = 061
  {191,  9,  0}, // idx = 0830, oldNumberOfOnes = 3, byte2Encode = 062
  {380,  9,  0}, // idx = 0831, oldNumberOfOnes = 3, byte2Encode = 063
  {149,  8,  0}, // idx = 0832, oldNumberOfOnes = 3, byte2Encode = 064
  {106,  8,  0}, // idx = 0833, oldNumberOfOnes = 3, byte2Encode = 065
  {107,  8,  0}, // idx = 0834, oldNumberOfOnes = 3, byte2Encode = 066
  {212,  9,  0}, // idx = 0835, oldNumberOfOnes = 3, byte2Encode = 067
  {105,  8,  0}, // idx = 0836, oldNumberOfOnes = 3, byte2Encode = 068
  {150,  8,  0}, // idx = 0837, oldNumberOfOnes = 3, byte2Encode = 069
  {151,  8,  0}, // idx = 0838, oldNumberOfOnes = 3, byte2Encode = 070
  {300,  9,  0}, // idx = 0839, oldNumberOfOnes = 3, byte2Encode = 071
  {109,  8,  0}, // idx = 0840, oldNumberOfOnes = 3, byte2Encode = 072
  {146,  8,  0}, // idx = 0841, oldNumberOfOnes = 3, byte2Encode = 073
  {147,  8,  0}, // idx = 0842, oldNumberOfOnes = 3, byte2Encode = 074
  {292,  9,  0}, // idx = 0843, oldNumberOfOnes = 3, byte2Encode = 075
  {145,  8,  0}, // idx = 0844, oldNumberOfOnes = 3, byte2Encode = 076
  {110,  8,  0}, // idx = 0845, oldNumberOfOnes = 3, byte2Encode = 077
  {111,  8,  0}, // idx = 0846, oldNumberOfOnes = 3, byte2Encode = 078
  {220,  9,  0}, // idx = 0847, oldNumberOfOnes = 3, byte2Encode = 079
  {101,  8,  0}, // idx = 0848, oldNumberOfOnes = 3, byte2Encode = 080
  {154,  8,  0}, // idx = 0849, oldNumberOfOnes = 3, byte2Encode = 081
  {155,  8,  0}, // idx = 0850, oldNumberOfOnes = 3, byte2Encode = 082
  {308,  9,  0}, // idx = 0851, oldNumberOfOnes = 3, byte2Encode = 083
  {153,  8,  0}, // idx = 0852, oldNumberOfOnes = 3, byte2Encode = 084
  {102,  8,  0}, // idx = 0853, oldNumberOfOnes = 3, byte2Encode = 085
  {103,  8,  0}, // idx = 0854, oldNumberOfOnes = 3, byte2Encode = 086
  {204,  9,  0}, // idx = 0855, oldNumberOfOnes = 3, byte2Encode = 087
  {157,  8,  0}, // idx = 0856, oldNumberOfOnes = 3, byte2Encode = 088
  { 98,  8,  0}, // idx = 0857, oldNumberOfOnes = 3, byte2Encode = 089
  { 99,  8,  0}, // idx = 0858, oldNumberOfOnes = 3, byte2Encode = 090
  {196,  9,  0}, // idx = 0859, oldNumberOfOnes = 3, byte2Encode = 091
  { 97,  8,  0}, // idx = 0860, oldNumberOfOnes = 3, byte2Encode = 092
  {158,  8,  0}, // idx = 0861, oldNumberOfOnes = 3, byte2Encode = 093
  {159,  8,  0}, // idx = 0862, oldNumberOfOnes = 3, byte2Encode = 094
  {316,  9,  0}, // idx = 0863, oldNumberOfOnes = 3, byte2Encode = 095
  {117,  8,  0}, // idx = 0864, oldNumberOfOnes = 3, byte2Encode = 096
  {138,  8,  0}, // idx = 0865, oldNumberOfOnes = 3, byte2Encode = 097
  {139,  8,  0}, // idx = 0866, oldNumberOfOnes = 3, byte2Encode = 098
  {276,  9,  0}, // idx = 0867, oldNumberOfOnes = 3, byte2Encode = 099
  {137,  8,  0}, // idx = 0868, oldNumberOfOnes = 3, byte2Encode = 100
  {118,  8,  0}, // idx = 0869, oldNumberOfOnes = 3, byte2Encode = 101
  {119,  8,  0}, // idx = 0870, oldNumberOfOnes = 3, byte2Encode = 102
  {236,  9,  0}, // idx = 0871, oldNumberOfOnes = 3, byte2Encode = 103
  {141,  8,  0}, // idx = 0872, oldNumberOfOnes = 3, byte2Encode = 104
  {114,  8,  0}, // idx = 0873, oldNumberOfOnes = 3, byte2Encode = 105
  {115,  8,  0}, // idx = 0874, oldNumberOfOnes = 3, byte2Encode = 106
  {228,  9,  0}, // idx = 0875, oldNumberOfOnes = 3, byte2Encode = 107
  {113,  8,  0}, // idx = 0876, oldNumberOfOnes = 3, byte2Encode = 108
  {142,  8,  0}, // idx = 0877, oldNumberOfOnes = 3, byte2Encode = 109
  {143,  8,  0}, // idx = 0878, oldNumberOfOnes = 3, byte2Encode = 110
  {284,  9,  0}, // idx = 0879, oldNumberOfOnes = 3, byte2Encode = 111
  {133,  8,  0}, // idx = 0880, oldNumberOfOnes = 3, byte2Encode = 112
  {122,  8,  0}, // idx = 0881, oldNumberOfOnes = 3, byte2Encode = 113
  {123,  8,  0}, // idx = 0882, oldNumberOfOnes = 3, byte2Encode = 114
  {244,  9,  0}, // idx = 0883, oldNumberOfOnes = 3, byte2Encode = 115
  {121,  8,  0}, // idx = 0884, oldNumberOfOnes = 3, byte2Encode = 116
  {134,  8,  0}, // idx = 0885, oldNumberOfOnes = 3, byte2Encode = 117
  {135,  8,  0}, // idx = 0886, oldNumberOfOnes = 3, byte2Encode = 118
  {268,  9,  0}, // idx = 0887, oldNumberOfOnes = 3, byte2Encode = 119
  {125,  8,  0}, // idx = 0888, oldNumberOfOnes = 3, byte2Encode = 120
  {130,  8,  0}, // idx = 0889, oldNumberOfOnes = 3, byte2Encode = 121
  {131,  8,  0}, // idx = 0890, oldNumberOfOnes = 3, byte2Encode = 122
  {260,  9,  0}, // idx = 0891, oldNumberOfOnes = 3, byte2Encode = 123
  {129,  9,  0}, // idx = 0892, oldNumberOfOnes = 3, byte2Encode = 124
  {382,  9,  0}, // idx = 0893, oldNumberOfOnes = 3, byte2Encode = 125
  {319,  9,  0}, // idx = 0894, oldNumberOfOnes = 3, byte2Encode = 126
  {764, 10,  0}, // idx = 0895, oldNumberOfOnes = 3, byte2Encode = 127
  {213,  8,  1}, // idx = 0896, oldNumberOfOnes = 3, byte2Encode = 128
  { 42,  8,  1}, // idx = 0897, oldNumberOfOnes = 3, byte2Encode = 129
  { 43,  8,  1}, // idx = 0898, oldNumberOfOnes = 3, byte2Encode = 130
  { 84,  9,  1}, // idx = 0899, oldNumberOfOnes = 3, byte2Encode = 131
  { 41,  8,  1}, // idx = 0900, oldNumberOfOnes = 3, byte2Encode = 132
  {214,  8,  1}, // idx = 0901, oldNumberOfOnes = 3, byte2Encode = 133
  {215,  8,  1}, // idx = 0902, oldNumberOfOnes = 3, byte2Encode = 134
  {428,  9,  1}, // idx = 0903, oldNumberOfOnes = 3, byte2Encode = 135
  { 45,  8,  1}, // idx = 0904, oldNumberOfOnes = 3, byte2Encode = 136
  {210,  8,  1}, // idx = 0905, oldNumberOfOnes = 3, byte2Encode = 137
  {211,  8,  1}, // idx = 0906, oldNumberOfOnes = 3, byte2Encode = 138
  {420,  9,  1}, // idx = 0907, oldNumberOfOnes = 3, byte2Encode = 139
  {209,  8,  1}, // idx = 0908, oldNumberOfOnes = 3, byte2Encode = 140
  { 46,  8,  1}, // idx = 0909, oldNumberOfOnes = 3, byte2Encode = 141
  { 47,  8,  1}, // idx = 0910, oldNumberOfOnes = 3, byte2Encode = 142
  { 92,  9,  1}, // idx = 0911, oldNumberOfOnes = 3, byte2Encode = 143
  { 37,  8,  1}, // idx = 0912, oldNumberOfOnes = 3, byte2Encode = 144
  {218,  8,  1}, // idx = 0913, oldNumberOfOnes = 3, byte2Encode = 145
  {219,  8,  1}, // idx = 0914, oldNumberOfOnes = 3, byte2Encode = 146
  {436,  9,  1}, // idx = 0915, oldNumberOfOnes = 3, byte2Encode = 147
  {217,  8,  1}, // idx = 0916, oldNumberOfOnes = 3, byte2Encode = 148
  { 38,  8,  1}, // idx = 0917, oldNumberOfOnes = 3, byte2Encode = 149
  { 39,  8,  1}, // idx = 0918, oldNumberOfOnes = 3, byte2Encode = 150
  { 76,  9,  1}, // idx = 0919, oldNumberOfOnes = 3, byte2Encode = 151
  {221,  8,  1}, // idx = 0920, oldNumberOfOnes = 3, byte2Encode = 152
  { 34,  8,  1}, // idx = 0921, oldNumberOfOnes = 3, byte2Encode = 153
  { 35,  8,  1}, // idx = 0922, oldNumberOfOnes = 3, byte2Encode = 154
  { 68,  9,  1}, // idx = 0923, oldNumberOfOnes = 3, byte2Encode = 155
  { 33,  8,  1}, // idx = 0924, oldNumberOfOnes = 3, byte2Encode = 156
  {222,  8,  1}, // idx = 0925, oldNumberOfOnes = 3, byte2Encode = 157
  {223,  8,  1}, // idx = 0926, oldNumberOfOnes = 3, byte2Encode = 158
  {444,  9,  1}, // idx = 0927, oldNumberOfOnes = 3, byte2Encode = 159
  { 53,  8,  1}, // idx = 0928, oldNumberOfOnes = 3, byte2Encode = 160
  {202,  8,  1}, // idx = 0929, oldNumberOfOnes = 3, byte2Encode = 161
  {203,  8,  1}, // idx = 0930, oldNumberOfOnes = 3, byte2Encode = 162
  {404,  9,  1}, // idx = 0931, oldNumberOfOnes = 3, byte2Encode = 163
  {201,  8,  1}, // idx = 0932, oldNumberOfOnes = 3, byte2Encode = 164
  { 54,  8,  1}, // idx = 0933, oldNumberOfOnes = 3, byte2Encode = 165
  { 55,  8,  1}, // idx = 0934, oldNumberOfOnes = 3, byte2Encode = 166
  {108,  9,  1}, // idx = 0935, oldNumberOfOnes = 3, byte2Encode = 167
  {205,  8,  1}, // idx = 0936, oldNumberOfOnes = 3, byte2Encode = 168
  { 50,  8,  1}, // idx = 0937, oldNumberOfOnes = 3, byte2Encode = 169
  { 51,  8,  1}, // idx = 0938, oldNumberOfOnes = 3, byte2Encode = 170
  {100,  9,  1}, // idx = 0939, oldNumberOfOnes = 3, byte2Encode = 171
  { 49,  8,  1}, // idx = 0940, oldNumberOfOnes = 3, byte2Encode = 172
  {206,  8,  1}, // idx = 0941, oldNumberOfOnes = 3, byte2Encode = 173
  {207,  8,  1}, // idx = 0942, oldNumberOfOnes = 3, byte2Encode = 174
  {412,  9,  1}, // idx = 0943, oldNumberOfOnes = 3, byte2Encode = 175
  {197,  8,  1}, // idx = 0944, oldNumberOfOnes = 3, byte2Encode = 176
  { 58,  8,  1}, // idx = 0945, oldNumberOfOnes = 3, byte2Encode = 177
  { 59,  8,  1}, // idx = 0946, oldNumberOfOnes = 3, byte2Encode = 178
  {116,  9,  1}, // idx = 0947, oldNumberOfOnes = 3, byte2Encode = 179
  { 57,  8,  1}, // idx = 0948, oldNumberOfOnes = 3, byte2Encode = 180
  {198,  8,  1}, // idx = 0949, oldNumberOfOnes = 3, byte2Encode = 181
  {199,  8,  1}, // idx = 0950, oldNumberOfOnes = 3, byte2Encode = 182
  {396,  9,  1}, // idx = 0951, oldNumberOfOnes = 3, byte2Encode = 183
  { 61,  8,  1}, // idx = 0952, oldNumberOfOnes = 3, byte2Encode = 184
  {194,  8,  1}, // idx = 0953, oldNumberOfOnes = 3, byte2Encode = 185
  {195,  8,  1}, // idx = 0954, oldNumberOfOnes = 3, byte2Encode = 186
  {388,  9,  1}, // idx = 0955, oldNumberOfOnes = 3, byte2Encode = 187
  {193,  8,  1}, // idx = 0956, oldNumberOfOnes = 3, byte2Encode = 188
  { 62,  8,  1}, // idx = 0957, oldNumberOfOnes = 3, byte2Encode = 189
  {447,  9,  1}, // idx = 0958, oldNumberOfOnes = 3, byte2Encode = 190
  {124,  9,  1}, // idx = 0959, oldNumberOfOnes = 3, byte2Encode = 191
  { 21,  8,  2}, // idx = 0960, oldNumberOfOnes = 3, byte2Encode = 192
  {234,  8,  2}, // idx = 0961, oldNumberOfOnes = 3, byte2Encode = 193
  {235,  8,  2}, // idx = 0962, oldNumberOfOnes = 3, byte2Encode = 194
  {468,  9,  2}, // idx = 0963, oldNumberOfOnes = 3, byte2Encode = 195
  {233,  8,  2}, // idx = 0964, oldNumberOfOnes = 3, byte2Encode = 196
  { 22,  8,  2}, // idx = 0965, oldNumberOfOnes = 3, byte2Encode = 197
  { 23,  8,  2}, // idx = 0966, oldNumberOfOnes = 3, byte2Encode = 198
  { 44,  9,  2}, // idx = 0967, oldNumberOfOnes = 3, byte2Encode = 199
  {237,  8,  2}, // idx = 0968, oldNumberOfOnes = 3, byte2Encode = 200
  { 18,  8,  2}, // idx = 0969, oldNumberOfOnes = 3, byte2Encode = 201
  { 19,  8,  2}, // idx = 0970, oldNumberOfOnes = 3, byte2Encode = 202
  { 36,  9,  2}, // idx = 0971, oldNumberOfOnes = 3, byte2Encode = 203
  { 17,  8,  2}, // idx = 0972, oldNumberOfOnes = 3, byte2Encode = 204
  {238,  8,  2}, // idx = 0973, oldNumberOfOnes = 3, byte2Encode = 205
  {239,  8,  2}, // idx = 0974, oldNumberOfOnes = 3, byte2Encode = 206
  {476,  9,  2}, // idx = 0975, oldNumberOfOnes = 3, byte2Encode = 207
  {229,  8,  2}, // idx = 0976, oldNumberOfOnes = 3, byte2Encode = 208
  { 26,  8,  2}, // idx = 0977, oldNumberOfOnes = 3, byte2Encode = 209
  { 27,  8,  2}, // idx = 0978, oldNumberOfOnes = 3, byte2Encode = 210
  { 52,  9,  2}, // idx = 0979, oldNumberOfOnes = 3, byte2Encode = 211
  { 25,  8,  2}, // idx = 0980, oldNumberOfOnes = 3, byte2Encode = 212
  {230,  8,  2}, // idx = 0981, oldNumberOfOnes = 3, byte2Encode = 213
  {231,  8,  2}, // idx = 0982, oldNumberOfOnes = 3, byte2Encode = 214
  {460,  9,  2}, // idx = 0983, oldNumberOfOnes = 3, byte2Encode = 215
  { 29,  8,  2}, // idx = 0984, oldNumberOfOnes = 3, byte2Encode = 216
  {226,  8,  2}, // idx = 0985, oldNumberOfOnes = 3, byte2Encode = 217
  {227,  8,  2}, // idx = 0986, oldNumberOfOnes = 3, byte2Encode = 218
  {452,  9,  2}, // idx = 0987, oldNumberOfOnes = 3, byte2Encode = 219
  {225,  8,  2}, // idx = 0988, oldNumberOfOnes = 3, byte2Encode = 220
  { 30,  8,  2}, // idx = 0989, oldNumberOfOnes = 3, byte2Encode = 221
  { 31,  8,  2}, // idx = 0990, oldNumberOfOnes = 3, byte2Encode = 222
  { 60,  9,  2}, // idx = 0991, oldNumberOfOnes = 3, byte2Encode = 223
  {245,  8,  3}, // idx = 0992, oldNumberOfOnes = 3, byte2Encode = 224
  { 10,  8,  3}, // idx = 0993, oldNumberOfOnes = 3, byte2Encode = 225
  { 11,  8,  3}, // idx = 0994, oldNumberOfOnes = 3, byte2Encode = 226
  { 20,  9,  3}, // idx = 0995, oldNumberOfOnes = 3, byte2Encode = 227
  {  9,  8,  3}, // idx = 0996, oldNumberOfOnes = 3, byte2Encode = 228
  {246,  8,  3}, // idx = 0997, oldNumberOfOnes = 3, byte2Encode = 229
  {247,  8,  3}, // idx = 0998, oldNumberOfOnes = 3, byte2Encode = 230
  {492,  9,  3}, // idx = 0999, oldNumberOfOnes = 3, byte2Encode = 231
  { 13,  8,  3}, // idx = 1000, oldNumberOfOnes = 3, byte2Encode = 232
  {242,  8,  3}, // idx = 1001, oldNumberOfOnes = 3, byte2Encode = 233
  {243,  8,  3}, // idx = 1002, oldNumberOfOnes = 3, byte2Encode = 234
  {484,  9,  3}, // idx = 1003, oldNumberOfOnes = 3, byte2Encode = 235
  {241,  8,  3}, // idx = 1004, oldNumberOfOnes = 3, byte2Encode = 236
  { 14,  8,  3}, // idx = 1005, oldNumberOfOnes = 3, byte2Encode = 237
  { 15,  8,  3}, // idx = 1006, oldNumberOfOnes = 3, byte2Encode = 238
  { 28,  9,  3}, // idx = 1007, oldNumberOfOnes = 3, byte2Encode = 239
  {  5,  8,  4}, // idx = 1008, oldNumberOfOnes = 3, byte2Encode = 240
  {250,  8,  4}, // idx = 1009, oldNumberOfOnes = 3, byte2Encode = 241
  {251,  8,  4}, // idx = 1010, oldNumberOfOnes = 3, byte2Encode = 242
  {500,  9,  4}, // idx = 1011, oldNumberOfOnes = 3, byte2Encode = 243
  {249,  8,  4}, // idx = 1012, oldNumberOfOnes = 3, byte2Encode = 244
  {  6,  8,  4}, // idx = 1013, oldNumberOfOnes = 3, byte2Encode = 245
  {  7,  8,  4}, // idx = 1014, oldNumberOfOnes = 3, byte2Encode = 246
  { 12,  9,  4}, // idx = 1015, oldNumberOfOnes = 3, byte2Encode = 247
  {253,  9,  0}, // idx = 1016, oldNumberOfOnes = 3, byte2Encode = 248
  {258,  9,  0}, // idx = 1017, oldNumberOfOnes = 3, byte2Encode = 249
  {259,  9,  0}, // idx = 1018, oldNumberOfOnes = 3, byte2Encode = 250
  {516, 10,  0}, // idx = 1019, oldNumberOfOnes = 3, byte2Encode = 251
  {385,  9,  1}, // idx = 1020, oldNumberOfOnes = 3, byte2Encode = 252
  {126,  9,  1}, // idx = 1021, oldNumberOfOnes = 3, byte2Encode = 253
  { 63,  9,  2}, // idx = 1022, oldNumberOfOnes = 3, byte2Encode = 254
  {252, 10,  1}, // idx = 1023, oldNumberOfOnes = 3, byte2Encode = 255
  { 85,  8,  0}, // idx = 1024, oldNumberOfOnes = 4, byte2Encode = 000
  {170,  9,  0}, // idx = 1025, oldNumberOfOnes = 4, byte2Encode = 001
  {171,  8,  0}, // idx = 1026, oldNumberOfOnes = 4, byte2Encode = 002
  {342,  9,  0}, // idx = 1027, oldNumberOfOnes = 4, byte2Encode = 003
  {169,  8,  0}, // idx = 1028, oldNumberOfOnes = 4, byte2Encode = 004
  {338,  9,  0}, // idx = 1029, oldNumberOfOnes = 4, byte2Encode = 005
  { 87,  8,  0}, // idx = 1030, oldNumberOfOnes = 4, byte2Encode = 006
  {174,  9,  0}, // idx = 1031, oldNumberOfOnes = 4, byte2Encode = 007
  {173,  8,  0}, // idx = 1032, oldNumberOfOnes = 4, byte2Encode = 008
  {346,  9,  0}, // idx = 1033, oldNumberOfOnes = 4, byte2Encode = 009
  { 83,  8,  0}, // idx = 1034, oldNumberOfOnes = 4, byte2Encode = 010
  {166,  9,  0}, // idx = 1035, oldNumberOfOnes = 4, byte2Encode = 011
  { 81,  8,  0}, // idx = 1036, oldNumberOfOnes = 4, byte2Encode = 012
  {162,  9,  0}, // idx = 1037, oldNumberOfOnes = 4, byte2Encode = 013
  {175,  8,  0}, // idx = 1038, oldNumberOfOnes = 4, byte2Encode = 014
  {350,  9,  0}, // idx = 1039, oldNumberOfOnes = 4, byte2Encode = 015
  {165,  8,  0}, // idx = 1040, oldNumberOfOnes = 4, byte2Encode = 016
  {330,  9,  0}, // idx = 1041, oldNumberOfOnes = 4, byte2Encode = 017
  { 91,  8,  0}, // idx = 1042, oldNumberOfOnes = 4, byte2Encode = 018
  {182,  9,  0}, // idx = 1043, oldNumberOfOnes = 4, byte2Encode = 019
  { 89,  8,  0}, // idx = 1044, oldNumberOfOnes = 4, byte2Encode = 020
  {178,  9,  0}, // idx = 1045, oldNumberOfOnes = 4, byte2Encode = 021
  {167,  8,  0}, // idx = 1046, oldNumberOfOnes = 4, byte2Encode = 022
  {334,  9,  0}, // idx = 1047, oldNumberOfOnes = 4, byte2Encode = 023
  { 93,  8,  0}, // idx = 1048, oldNumberOfOnes = 4, byte2Encode = 024
  {186,  9,  0}, // idx = 1049, oldNumberOfOnes = 4, byte2Encode = 025
  {163,  8,  0}, // idx = 1050, oldNumberOfOnes = 4, byte2Encode = 026
  {326,  9,  0}, // idx = 1051, oldNumberOfOnes = 4, byte2Encode = 027
  {161,  8,  0}, // idx = 1052, oldNumberOfOnes = 4, byte2Encode = 028
  {322,  9,  0}, // idx = 1053, oldNumberOfOnes = 4, byte2Encode = 029
  { 95,  8,  0}, // idx = 1054, oldNumberOfOnes = 4, byte2Encode = 030
  {190,  9,  0}, // idx = 1055, oldNumberOfOnes = 4, byte2Encode = 031
  {181,  8,  0}, // idx = 1056, oldNumberOfOnes = 4, byte2Encode = 032
  {362,  9,  0}, // idx = 1057, oldNumberOfOnes = 4, byte2Encode = 033
  { 75,  8,  0}, // idx = 1058, oldNumberOfOnes = 4, byte2Encode = 034
  {150,  9,  0}, // idx = 1059, oldNumberOfOnes = 4, byte2Encode = 035
  { 73,  8,  0}, // idx = 1060, oldNumberOfOnes = 4, byte2Encode = 036
  {146,  9,  0}, // idx = 1061, oldNumberOfOnes = 4, byte2Encode = 037
  {183,  8,  0}, // idx = 1062, oldNumberOfOnes = 4, byte2Encode = 038
  {366,  9,  0}, // idx = 1063, oldNumberOfOnes = 4, byte2Encode = 039
  { 77,  8,  0}, // idx = 1064, oldNumberOfOnes = 4, byte2Encode = 040
  {154,  9,  0}, // idx = 1065, oldNumberOfOnes = 4, byte2Encode = 041
  {179,  8,  0}, // idx = 1066, oldNumberOfOnes = 4, byte2Encode = 042
  {358,  9,  0}, // idx = 1067, oldNumberOfOnes = 4, byte2Encode = 043
  {177,  8,  0}, // idx = 1068, oldNumberOfOnes = 4, byte2Encode = 044
  {354,  9,  0}, // idx = 1069, oldNumberOfOnes = 4, byte2Encode = 045
  { 79,  8,  0}, // idx = 1070, oldNumberOfOnes = 4, byte2Encode = 046
  {158,  9,  0}, // idx = 1071, oldNumberOfOnes = 4, byte2Encode = 047
  { 69,  8,  0}, // idx = 1072, oldNumberOfOnes = 4, byte2Encode = 048
  {138,  9,  0}, // idx = 1073, oldNumberOfOnes = 4, byte2Encode = 049
  {187,  8,  0}, // idx = 1074, oldNumberOfOnes = 4, byte2Encode = 050
  {374,  9,  0}, // idx = 1075, oldNumberOfOnes = 4, byte2Encode = 051
  {185,  8,  0}, // idx = 1076, oldNumberOfOnes = 4, byte2Encode = 052
  {370,  9,  0}, // idx = 1077, oldNumberOfOnes = 4, byte2Encode = 053
  { 71,  8,  0}, // idx = 1078, oldNumberOfOnes = 4, byte2Encode = 054
  {142,  9,  0}, // idx = 1079, oldNumberOfOnes = 4, byte2Encode = 055
  {189,  8,  0}, // idx = 1080, oldNumberOfOnes = 4, byte2Encode = 056
  {378,  9,  0}, // idx = 1081, oldNumberOfOnes = 4, byte2Encode = 057
  { 67,  8,  0}, // idx = 1082, oldNumberOfOnes = 4, byte2Encode = 058
  {134,  9,  0}, // idx = 1083, oldNumberOfOnes = 4, byte2Encode = 059
  { 65,  8,  0}, // idx = 1084, oldNumberOfOnes = 4, byte2Encode = 060
  {130,  9,  0}, // idx = 1085, oldNumberOfOnes = 4, byte2Encode = 061
  {191,  9,  0}, // idx = 1086, oldNumberOfOnes = 4, byte2Encode = 062
  {382, 10,  0}, // idx = 1087, oldNumberOfOnes = 4, byte2Encode = 063
  {149,  8,  0}, // idx = 1088, oldNumberOfOnes = 4, byte2Encode = 064
  {298,  9,  0}, // idx = 1089, oldNumberOfOnes = 4, byte2Encode = 065
  {107,  8,  0}, // idx = 1090, oldNumberOfOnes = 4, byte2Encode = 066
  {214,  9,  0}, // idx = 1091, oldNumberOfOnes = 4, byte2Encode = 067
  {105,  8,  0}, // idx = 1092, oldNumberOfOnes = 4, byte2Encode = 068
  {210,  9,  0}, // idx = 1093, oldNumberOfOnes = 4, byte2Encode = 069
  {151,  8,  0}, // idx = 1094, oldNumberOfOnes = 4, byte2Encode = 070
  {302,  9,  0}, // idx = 1095, oldNumberOfOnes = 4, byte2Encode = 071
  {109,  8,  0}, // idx = 1096, oldNumberOfOnes = 4, byte2Encode = 072
  {218,  9,  0}, // idx = 1097, oldNumberOfOnes = 4, byte2Encode = 073
  {147,  8,  0}, // idx = 1098, oldNumberOfOnes = 4, byte2Encode = 074
  {294,  9,  0}, // idx = 1099, oldNumberOfOnes = 4, byte2Encode = 075
  {145,  8,  0}, // idx = 1100, oldNumberOfOnes = 4, byte2Encode = 076
  {290,  9,  0}, // idx = 1101, oldNumberOfOnes = 4, byte2Encode = 077
  {111,  8,  0}, // idx = 1102, oldNumberOfOnes = 4, byte2Encode = 078
  {222,  9,  0}, // idx = 1103, oldNumberOfOnes = 4, byte2Encode = 079
  {101,  8,  0}, // idx = 1104, oldNumberOfOnes = 4, byte2Encode = 080
  {202,  9,  0}, // idx = 1105, oldNumberOfOnes = 4, byte2Encode = 081
  {155,  8,  0}, // idx = 1106, oldNumberOfOnes = 4, byte2Encode = 082
  {310,  9,  0}, // idx = 1107, oldNumberOfOnes = 4, byte2Encode = 083
  {153,  8,  0}, // idx = 1108, oldNumberOfOnes = 4, byte2Encode = 084
  {306,  9,  0}, // idx = 1109, oldNumberOfOnes = 4, byte2Encode = 085
  {103,  8,  0}, // idx = 1110, oldNumberOfOnes = 4, byte2Encode = 086
  {206,  9,  0}, // idx = 1111, oldNumberOfOnes = 4, byte2Encode = 087
  {157,  8,  0}, // idx = 1112, oldNumberOfOnes = 4, byte2Encode = 088
  {314,  9,  0}, // idx = 1113, oldNumberOfOnes = 4, byte2Encode = 089
  { 99,  8,  0}, // idx = 1114, oldNumberOfOnes = 4, byte2Encode = 090
  {198,  9,  0}, // idx = 1115, oldNumberOfOnes = 4, byte2Encode = 091
  { 97,  8,  0}, // idx = 1116, oldNumberOfOnes = 4, byte2Encode = 092
  {194,  9,  0}, // idx = 1117, oldNumberOfOnes = 4, byte2Encode = 093
  {159,  8,  0}, // idx = 1118, oldNumberOfOnes = 4, byte2Encode = 094
  {318,  9,  0}, // idx = 1119, oldNumberOfOnes = 4, byte2Encode = 095
  {117,  8,  0}, // idx = 1120, oldNumberOfOnes = 4, byte2Encode = 096
  {234,  9,  0}, // idx = 1121, oldNumberOfOnes = 4, byte2Encode = 097
  {139,  8,  0}, // idx = 1122, oldNumberOfOnes = 4, byte2Encode = 098
  {278,  9,  0}, // idx = 1123, oldNumberOfOnes = 4, byte2Encode = 099
  {137,  8,  0}, // idx = 1124, oldNumberOfOnes = 4, byte2Encode = 100
  {274,  9,  0}, // idx = 1125, oldNumberOfOnes = 4, byte2Encode = 101
  {119,  8,  0}, // idx = 1126, oldNumberOfOnes = 4, byte2Encode = 102
  {238,  9,  0}, // idx = 1127, oldNumberOfOnes = 4, byte2Encode = 103
  {141,  8,  0}, // idx = 1128, oldNumberOfOnes = 4, byte2Encode = 104
  {282,  9,  0}, // idx = 1129, oldNumberOfOnes = 4, byte2Encode = 105
  {115,  8,  0}, // idx = 1130, oldNumberOfOnes = 4, byte2Encode = 106
  {230,  9,  0}, // idx = 1131, oldNumberOfOnes = 4, byte2Encode = 107
  {113,  8,  0}, // idx = 1132, oldNumberOfOnes = 4, byte2Encode = 108
  {226,  9,  0}, // idx = 1133, oldNumberOfOnes = 4, byte2Encode = 109
  {143,  8,  0}, // idx = 1134, oldNumberOfOnes = 4, byte2Encode = 110
  {286,  9,  0}, // idx = 1135, oldNumberOfOnes = 4, byte2Encode = 111
  {133,  8,  0}, // idx = 1136, oldNumberOfOnes = 4, byte2Encode = 112
  {266,  9,  0}, // idx = 1137, oldNumberOfOnes = 4, byte2Encode = 113
  {123,  8,  0}, // idx = 1138, oldNumberOfOnes = 4, byte2Encode = 114
  {246,  9,  0}, // idx = 1139, oldNumberOfOnes = 4, byte2Encode = 115
  {121,  8,  0}, // idx = 1140, oldNumberOfOnes = 4, byte2Encode = 116
  {242,  9,  0}, // idx = 1141, oldNumberOfOnes = 4, byte2Encode = 117
  {135,  8,  0}, // idx = 1142, oldNumberOfOnes = 4, byte2Encode = 118
  {270,  9,  0}, // idx = 1143, oldNumberOfOnes = 4, byte2Encode = 119
  {125,  8,  0}, // idx = 1144, oldNumberOfOnes = 4, byte2Encode = 120
  {250,  9,  0}, // idx = 1145, oldNumberOfOnes = 4, byte2Encode = 121
  {131,  8,  0}, // idx = 1146, oldNumberOfOnes = 4, byte2Encode = 122
  {262,  9,  0}, // idx = 1147, oldNumberOfOnes = 4, byte2Encode = 123
  {129,  9,  0}, // idx = 1148, oldNumberOfOnes = 4, byte2Encode = 124
  {258, 10,  0}, // idx = 1149, oldNumberOfOnes = 4, byte2Encode = 125
  {319,  9,  0}, // idx = 1150, oldNumberOfOnes = 4, byte2Encode = 126
  {638, 10,  0}, // idx = 1151, oldNumberOfOnes = 4, byte2Encode = 127
  {213,  8,  1}, // idx = 1152, oldNumberOfOnes = 4, byte2Encode = 128
  {426,  9,  1}, // idx = 1153, oldNumberOfOnes = 4, byte2Encode = 129
  { 43,  8,  1}, // idx = 1154, oldNumberOfOnes = 4, byte2Encode = 130
  { 86,  9,  1}, // idx = 1155, oldNumberOfOnes = 4, byte2Encode = 131
  { 41,  8,  1}, // idx = 1156, oldNumberOfOnes = 4, byte2Encode = 132
  { 82,  9,  1}, // idx = 1157, oldNumberOfOnes = 4, byte2Encode = 133
  {215,  8,  1}, // idx = 1158, oldNumberOfOnes = 4, byte2Encode = 134
  {430,  9,  1}, // idx = 1159, oldNumberOfOnes = 4, byte2Encode = 135
  { 45,  8,  1}, // idx = 1160, oldNumberOfOnes = 4, byte2Encode = 136
  { 90,  9,  1}, // idx = 1161, oldNumberOfOnes = 4, byte2Encode = 137
  {211,  8,  1}, // idx = 1162, oldNumberOfOnes = 4, byte2Encode = 138
  {422,  9,  1}, // idx = 1163, oldNumberOfOnes = 4, byte2Encode = 139
  {209,  8,  1}, // idx = 1164, oldNumberOfOnes = 4, byte2Encode = 140
  {418,  9,  1}, // idx = 1165, oldNumberOfOnes = 4, byte2Encode = 141
  { 47,  8,  1}, // idx = 1166, oldNumberOfOnes = 4, byte2Encode = 142
  { 94,  9,  1}, // idx = 1167, oldNumberOfOnes = 4, byte2Encode = 143
  { 37,  8,  1}, // idx = 1168, oldNumberOfOnes = 4, byte2Encode = 144
  { 74,  9,  1}, // idx = 1169, oldNumberOfOnes = 4, byte2Encode = 145
  {219,  8,  1}, // idx = 1170, oldNumberOfOnes = 4, byte2Encode = 146
  {438,  9,  1}, // idx = 1171, oldNumberOfOnes = 4, byte2Encode = 147
  {217,  8,  1}, // idx = 1172, oldNumberOfOnes = 4, byte2Encode = 148
  {434,  9,  1}, // idx = 1173, oldNumberOfOnes = 4, byte2Encode = 149
  { 39,  8,  1}, // idx = 1174, oldNumberOfOnes = 4, byte2Encode = 150
  { 78,  9,  1}, // idx = 1175, oldNumberOfOnes = 4, byte2Encode = 151
  {221,  8,  1}, // idx = 1176, oldNumberOfOnes = 4, byte2Encode = 152
  {442,  9,  1}, // idx = 1177, oldNumberOfOnes = 4, byte2Encode = 153
  { 35,  8,  1}, // idx = 1178, oldNumberOfOnes = 4, byte2Encode = 154
  { 70,  9,  1}, // idx = 1179, oldNumberOfOnes = 4, byte2Encode = 155
  { 33,  8,  1}, // idx = 1180, oldNumberOfOnes = 4, byte2Encode = 156
  { 66,  9,  1}, // idx = 1181, oldNumberOfOnes = 4, byte2Encode = 157
  {223,  8,  1}, // idx = 1182, oldNumberOfOnes = 4, byte2Encode = 158
  {446,  9,  1}, // idx = 1183, oldNumberOfOnes = 4, byte2Encode = 159
  { 53,  8,  1}, // idx = 1184, oldNumberOfOnes = 4, byte2Encode = 160
  {106,  9,  1}, // idx = 1185, oldNumberOfOnes = 4, byte2Encode = 161
  {203,  8,  1}, // idx = 1186, oldNumberOfOnes = 4, byte2Encode = 162
  {406,  9,  1}, // idx = 1187, oldNumberOfOnes = 4, byte2Encode = 163
  {201,  8,  1}, // idx = 1188, oldNumberOfOnes = 4, byte2Encode = 164
  {402,  9,  1}, // idx = 1189, oldNumberOfOnes = 4, byte2Encode = 165
  { 55,  8,  1}, // idx = 1190, oldNumberOfOnes = 4, byte2Encode = 166
  {110,  9,  1}, // idx = 1191, oldNumberOfOnes = 4, byte2Encode = 167
  {205,  8,  1}, // idx = 1192, oldNumberOfOnes = 4, byte2Encode = 168
  {410,  9,  1}, // idx = 1193, oldNumberOfOnes = 4, byte2Encode = 169
  { 51,  8,  1}, // idx = 1194, oldNumberOfOnes = 4, byte2Encode = 170
  {102,  9,  1}, // idx = 1195, oldNumberOfOnes = 4, byte2Encode = 171
  { 49,  8,  1}, // idx = 1196, oldNumberOfOnes = 4, byte2Encode = 172
  { 98,  9,  1}, // idx = 1197, oldNumberOfOnes = 4, byte2Encode = 173
  {207,  8,  1}, // idx = 1198, oldNumberOfOnes = 4, byte2Encode = 174
  {414,  9,  1}, // idx = 1199, oldNumberOfOnes = 4, byte2Encode = 175
  {197,  8,  1}, // idx = 1200, oldNumberOfOnes = 4, byte2Encode = 176
  {394,  9,  1}, // idx = 1201, oldNumberOfOnes = 4, byte2Encode = 177
  { 59,  8,  1}, // idx = 1202, oldNumberOfOnes = 4, byte2Encode = 178
  {118,  9,  1}, // idx = 1203, oldNumberOfOnes = 4, byte2Encode = 179
  { 57,  8,  1}, // idx = 1204, oldNumberOfOnes = 4, byte2Encode = 180
  {114,  9,  1}, // idx = 1205, oldNumberOfOnes = 4, byte2Encode = 181
  {199,  8,  1}, // idx = 1206, oldNumberOfOnes = 4, byte2Encode = 182
  {398,  9,  1}, // idx = 1207, oldNumberOfOnes = 4, byte2Encode = 183
  { 61,  8,  1}, // idx = 1208, oldNumberOfOnes = 4, byte2Encode = 184
  {122,  9,  1}, // idx = 1209, oldNumberOfOnes = 4, byte2Encode = 185
  {195,  8,  1}, // idx = 1210, oldNumberOfOnes = 4, byte2Encode = 186
  {390,  9,  1}, // idx = 1211, oldNumberOfOnes = 4, byte2Encode = 187
  {193,  8,  1}, // idx = 1212, oldNumberOfOnes = 4, byte2Encode = 188
  {386,  9,  1}, // idx = 1213, oldNumberOfOnes = 4, byte2Encode = 189
  {447,  9,  1}, // idx = 1214, oldNumberOfOnes = 4, byte2Encode = 190
  {894, 10,  1}, // idx = 1215, oldNumberOfOnes = 4, byte2Encode = 191
  { 21,  8,  2}, // idx = 1216, oldNumberOfOnes = 4, byte2Encode = 192
  { 42,  9,  2}, // idx = 1217, oldNumberOfOnes = 4, byte2Encode = 193
  {235,  8,  2}, // idx = 1218, oldNumberOfOnes = 4, byte2Encode = 194
  {470,  9,  2}, // idx = 1219, oldNumberOfOnes = 4, byte2Encode = 195
  {233,  8,  2}, // idx = 1220, oldNumberOfOnes = 4, byte2Encode = 196
  {466,  9,  2}, // idx = 1221, oldNumberOfOnes = 4, byte2Encode = 197
  { 23,  8,  2}, // idx = 1222, oldNumberOfOnes = 4, byte2Encode = 198
  { 46,  9,  2}, // idx = 1223, oldNumberOfOnes = 4, byte2Encode = 199
  {237,  8,  2}, // idx = 1224, oldNumberOfOnes = 4, byte2Encode = 200
  {474,  9,  2}, // idx = 1225, oldNumberOfOnes = 4, byte2Encode = 201
  { 19,  8,  2}, // idx = 1226, oldNumberOfOnes = 4, byte2Encode = 202
  { 38,  9,  2}, // idx = 1227, oldNumberOfOnes = 4, byte2Encode = 203
  { 17,  8,  2}, // idx = 1228, oldNumberOfOnes = 4, byte2Encode = 204
  { 34,  9,  2}, // idx = 1229, oldNumberOfOnes = 4, byte2Encode = 205
  {239,  8,  2}, // idx = 1230, oldNumberOfOnes = 4, byte2Encode = 206
  {478,  9,  2}, // idx = 1231, oldNumberOfOnes = 4, byte2Encode = 207
  {229,  8,  2}, // idx = 1232, oldNumberOfOnes = 4, byte2Encode = 208
  {458,  9,  2}, // idx = 1233, oldNumberOfOnes = 4, byte2Encode = 209
  { 27,  8,  2}, // idx = 1234, oldNumberOfOnes = 4, byte2Encode = 210
  { 54,  9,  2}, // idx = 1235, oldNumberOfOnes = 4, byte2Encode = 211
  { 25,  8,  2}, // idx = 1236, oldNumberOfOnes = 4, byte2Encode = 212
  { 50,  9,  2}, // idx = 1237, oldNumberOfOnes = 4, byte2Encode = 213
  {231,  8,  2}, // idx = 1238, oldNumberOfOnes = 4, byte2Encode = 214
  {462,  9,  2}, // idx = 1239, oldNumberOfOnes = 4, byte2Encode = 215
  { 29,  8,  2}, // idx = 1240, oldNumberOfOnes = 4, byte2Encode = 216
  { 58,  9,  2}, // idx = 1241, oldNumberOfOnes = 4, byte2Encode = 217
  {227,  8,  2}, // idx = 1242, oldNumberOfOnes = 4, byte2Encode = 218
  {454,  9,  2}, // idx = 1243, oldNumberOfOnes = 4, byte2Encode = 219
  {225,  8,  2}, // idx = 1244, oldNumberOfOnes = 4, byte2Encode = 220
  {450,  9,  2}, // idx = 1245, oldNumberOfOnes = 4, byte2Encode = 221
  { 31,  8,  2}, // idx = 1246, oldNumberOfOnes = 4, byte2Encode = 222
  { 62,  9,  2}, // idx = 1247, oldNumberOfOnes = 4, byte2Encode = 223
  {245,  8,  3}, // idx = 1248, oldNumberOfOnes = 4, byte2Encode = 224
  {490,  9,  3}, // idx = 1249, oldNumberOfOnes = 4, byte2Encode = 225
  { 11,  8,  3}, // idx = 1250, oldNumberOfOnes = 4, byte2Encode = 226
  { 22,  9,  3}, // idx = 1251, oldNumberOfOnes = 4, byte2Encode = 227
  {  9,  8,  3}, // idx = 1252, oldNumberOfOnes = 4, byte2Encode = 228
  { 18,  9,  3}, // idx = 1253, oldNumberOfOnes = 4, byte2Encode = 229
  {247,  8,  3}, // idx = 1254, oldNumberOfOnes = 4, byte2Encode = 230
  {494,  9,  3}, // idx = 1255, oldNumberOfOnes = 4, byte2Encode = 231
  { 13,  8,  3}, // idx = 1256, oldNumberOfOnes = 4, byte2Encode = 232
  { 26,  9,  3}, // idx = 1257, oldNumberOfOnes = 4, byte2Encode = 233
  {243,  8,  3}, // idx = 1258, oldNumberOfOnes = 4, byte2Encode = 234
  {486,  9,  3}, // idx = 1259, oldNumberOfOnes = 4, byte2Encode = 235
  {241,  8,  3}, // idx = 1260, oldNumberOfOnes = 4, byte2Encode = 236
  {482,  9,  3}, // idx = 1261, oldNumberOfOnes = 4, byte2Encode = 237
  { 15,  8,  3}, // idx = 1262, oldNumberOfOnes = 4, byte2Encode = 238
  { 30,  9,  3}, // idx = 1263, oldNumberOfOnes = 4, byte2Encode = 239
  {  5,  8,  4}, // idx = 1264, oldNumberOfOnes = 4, byte2Encode = 240
  { 10,  9,  4}, // idx = 1265, oldNumberOfOnes = 4, byte2Encode = 241
  {251,  8,  4}, // idx = 1266, oldNumberOfOnes = 4, byte2Encode = 242
  {502,  9,  4}, // idx = 1267, oldNumberOfOnes = 4, byte2Encode = 243
  {249,  8,  4}, // idx = 1268, oldNumberOfOnes = 4, byte2Encode = 244
  {498,  9,  4}, // idx = 1269, oldNumberOfOnes = 4, byte2Encode = 245
  {  7,  8,  4}, // idx = 1270, oldNumberOfOnes = 4, byte2Encode = 246
  { 14,  9,  4}, // idx = 1271, oldNumberOfOnes = 4, byte2Encode = 247
  {253,  9,  0}, // idx = 1272, oldNumberOfOnes = 4, byte2Encode = 248
  {506, 10,  0}, // idx = 1273, oldNumberOfOnes = 4, byte2Encode = 249
  {259,  9,  0}, // idx = 1274, oldNumberOfOnes = 4, byte2Encode = 250
  {518, 10,  0}, // idx = 1275, oldNumberOfOnes = 4, byte2Encode = 251
  {385,  9,  1}, // idx = 1276, oldNumberOfOnes = 4, byte2Encode = 252
  {770, 10,  1}, // idx = 1277, oldNumberOfOnes = 4, byte2Encode = 253
  { 63,  9,  2}, // idx = 1278, oldNumberOfOnes = 4, byte2Encode = 254
  {126, 10,  2}, // idx = 1279, oldNumberOfOnes = 4, byte2Encode = 255
  {341,  9,  0}, // idx = 1280, oldNumberOfOnes = 5, byte2Encode = 000
  {171,  9,  0}, // idx = 1281, oldNumberOfOnes = 5, byte2Encode = 001
  {169,  9,  0}, // idx = 1282, oldNumberOfOnes = 5, byte2Encode = 002
  {343,  9,  0}, // idx = 1283, oldNumberOfOnes = 5, byte2Encode = 003
  {173,  9,  0}, // idx = 1284, oldNumberOfOnes = 5, byte2Encode = 004
  {339,  9,  0}, // idx = 1285, oldNumberOfOnes = 5, byte2Encode = 005
  {337,  9,  0}, // idx = 1286, oldNumberOfOnes = 5, byte2Encode = 006
  {175,  9,  0}, // idx = 1287, oldNumberOfOnes = 5, byte2Encode = 007
  {165,  9,  0}, // idx = 1288, oldNumberOfOnes = 5, byte2Encode = 008
  {347,  9,  0}, // idx = 1289, oldNumberOfOnes = 5, byte2Encode = 009
  {345,  9,  0}, // idx = 1290, oldNumberOfOnes = 5, byte2Encode = 010
  {167,  9,  0}, // idx = 1291, oldNumberOfOnes = 5, byte2Encode = 011
  {349,  9,  0}, // idx = 1292, oldNumberOfOnes = 5, byte2Encode = 012
  {163,  9,  0}, // idx = 1293, oldNumberOfOnes = 5, byte2Encode = 013
  {161,  9,  0}, // idx = 1294, oldNumberOfOnes = 5, byte2Encode = 014
  {351,  9,  0}, // idx = 1295, oldNumberOfOnes = 5, byte2Encode = 015
  {181,  9,  0}, // idx = 1296, oldNumberOfOnes = 5, byte2Encode = 016
  {331,  9,  0}, // idx = 1297, oldNumberOfOnes = 5, byte2Encode = 017
  {329,  9,  0}, // idx = 1298, oldNumberOfOnes = 5, byte2Encode = 018
  {183,  9,  0}, // idx = 1299, oldNumberOfOnes = 5, byte2Encode = 019
  {333,  9,  0}, // idx = 1300, oldNumberOfOnes = 5, byte2Encode = 020
  {179,  9,  0}, // idx = 1301, oldNumberOfOnes = 5, byte2Encode = 021
  {177,  9,  0}, // idx = 1302, oldNumberOfOnes = 5, byte2Encode = 022
  {335,  9,  0}, // idx = 1303, oldNumberOfOnes = 5, byte2Encode = 023
  {325,  9,  0}, // idx = 1304, oldNumberOfOnes = 5, byte2Encode = 024
  {187,  9,  0}, // idx = 1305, oldNumberOfOnes = 5, byte2Encode = 025
  {185,  9,  0}, // idx = 1306, oldNumberOfOnes = 5, byte2Encode = 026
  {327,  9,  0}, // idx = 1307, oldNumberOfOnes = 5, byte2Encode = 027
  {189,  9,  0}, // idx = 1308, oldNumberOfOnes = 5, byte2Encode = 028
  {323,  9,  0}, // idx = 1309, oldNumberOfOnes = 5, byte2Encode = 029
  {321,  9,  0}, // idx = 1310, oldNumberOfOnes = 5, byte2Encode = 030
  {703, 10,  0}, // idx = 1311, oldNumberOfOnes = 5, byte2Encode = 031
  {149,  9,  0}, // idx = 1312, oldNumberOfOnes = 5, byte2Encode = 032
  {363,  9,  0}, // idx = 1313, oldNumberOfOnes = 5, byte2Encode = 033
  {361,  9,  0}, // idx = 1314, oldNumberOfOnes = 5, byte2Encode = 034
  {151,  9,  0}, // idx = 1315, oldNumberOfOnes = 5, byte2Encode = 035
  {365,  9,  0}, // idx = 1316, oldNumberOfOnes = 5, byte2Encode = 036
  {147,  9,  0}, // idx = 1317, oldNumberOfOnes = 5, byte2Encode = 037
  {145,  9,  0}, // idx = 1318, oldNumberOfOnes = 5, byte2Encode = 038
  {367,  9,  0}, // idx = 1319, oldNumberOfOnes = 5, byte2Encode = 039
  {357,  9,  0}, // idx = 1320, oldNumberOfOnes = 5, byte2Encode = 040
  {155,  9,  0}, // idx = 1321, oldNumberOfOnes = 5, byte2Encode = 041
  {153,  9,  0}, // idx = 1322, oldNumberOfOnes = 5, byte2Encode = 042
  {359,  9,  0}, // idx = 1323, oldNumberOfOnes = 5, byte2Encode = 043
  {157,  9,  0}, // idx = 1324, oldNumberOfOnes = 5, byte2Encode = 044
  {355,  9,  0}, // idx = 1325, oldNumberOfOnes = 5, byte2Encode = 045
  {353,  9,  0}, // idx = 1326, oldNumberOfOnes = 5, byte2Encode = 046
  {159,  9,  0}, // idx = 1327, oldNumberOfOnes = 5, byte2Encode = 047
  {373,  9,  0}, // idx = 1328, oldNumberOfOnes = 5, byte2Encode = 048
  {139,  9,  0}, // idx = 1329, oldNumberOfOnes = 5, byte2Encode = 049
  {137,  9,  0}, // idx = 1330, oldNumberOfOnes = 5, byte2Encode = 050
  {375,  9,  0}, // idx = 1331, oldNumberOfOnes = 5, byte2Encode = 051
  {141,  9,  0}, // idx = 1332, oldNumberOfOnes = 5, byte2Encode = 052
  {371,  9,  0}, // idx = 1333, oldNumberOfOnes = 5, byte2Encode = 053
  {369,  9,  0}, // idx = 1334, oldNumberOfOnes = 5, byte2Encode = 054
  {143,  9,  0}, // idx = 1335, oldNumberOfOnes = 5, byte2Encode = 055
  {133,  9,  0}, // idx = 1336, oldNumberOfOnes = 5, byte2Encode = 056
  {379,  9,  0}, // idx = 1337, oldNumberOfOnes = 5, byte2Encode = 057
  {377,  9,  0}, // idx = 1338, oldNumberOfOnes = 5, byte2Encode = 058
  {135,  9,  0}, // idx = 1339, oldNumberOfOnes = 5, byte2Encode = 059
  {381,  9,  0}, // idx = 1340, oldNumberOfOnes = 5, byte2Encode = 060
  {131,  9,  0}, // idx = 1341, oldNumberOfOnes = 5, byte2Encode = 061
  {641, 10,  0}, // idx = 1342, oldNumberOfOnes = 5, byte2Encode = 062
  {319, 10,  0}, // idx = 1343, oldNumberOfOnes = 5, byte2Encode = 063
  {213,  9,  0}, // idx = 1344, oldNumberOfOnes = 5, byte2Encode = 064
  {299,  9,  0}, // idx = 1345, oldNumberOfOnes = 5, byte2Encode = 065
  {297,  9,  0}, // idx = 1346, oldNumberOfOnes = 5, byte2Encode = 066
  {215,  9,  0}, // idx = 1347, oldNumberOfOnes = 5, byte2Encode = 067
  {301,  9,  0}, // idx = 1348, oldNumberOfOnes = 5, byte2Encode = 068
  {211,  9,  0}, // idx = 1349, oldNumberOfOnes = 5, byte2Encode = 069
  {209,  9,  0}, // idx = 1350, oldNumberOfOnes = 5, byte2Encode = 070
  {303,  9,  0}, // idx = 1351, oldNumberOfOnes = 5, byte2Encode = 071
  {293,  9,  0}, // idx = 1352, oldNumberOfOnes = 5, byte2Encode = 072
  {219,  9,  0}, // idx = 1353, oldNumberOfOnes = 5, byte2Encode = 073
  {217,  9,  0}, // idx = 1354, oldNumberOfOnes = 5, byte2Encode = 074
  {295,  9,  0}, // idx = 1355, oldNumberOfOnes = 5, byte2Encode = 075
  {221,  9,  0}, // idx = 1356, oldNumberOfOnes = 5, byte2Encode = 076
  {291,  9,  0}, // idx = 1357, oldNumberOfOnes = 5, byte2Encode = 077
  {289,  9,  0}, // idx = 1358, oldNumberOfOnes = 5, byte2Encode = 078
  {223,  9,  0}, // idx = 1359, oldNumberOfOnes = 5, byte2Encode = 079
  {309,  9,  0}, // idx = 1360, oldNumberOfOnes = 5, byte2Encode = 080
  {203,  9,  0}, // idx = 1361, oldNumberOfOnes = 5, byte2Encode = 081
  {201,  9,  0}, // idx = 1362, oldNumberOfOnes = 5, byte2Encode = 082
  {311,  9,  0}, // idx = 1363, oldNumberOfOnes = 5, byte2Encode = 083
  {205,  9,  0}, // idx = 1364, oldNumberOfOnes = 5, byte2Encode = 084
  {307,  9,  0}, // idx = 1365, oldNumberOfOnes = 5, byte2Encode = 085
  {305,  9,  0}, // idx = 1366, oldNumberOfOnes = 5, byte2Encode = 086
  {207,  9,  0}, // idx = 1367, oldNumberOfOnes = 5, byte2Encode = 087
  {197,  9,  0}, // idx = 1368, oldNumberOfOnes = 5, byte2Encode = 088
  {315,  9,  0}, // idx = 1369, oldNumberOfOnes = 5, byte2Encode = 089
  {313,  9,  0}, // idx = 1370, oldNumberOfOnes = 5, byte2Encode = 090
  {199,  9,  0}, // idx = 1371, oldNumberOfOnes = 5, byte2Encode = 091
  {317,  9,  0}, // idx = 1372, oldNumberOfOnes = 5, byte2Encode = 092
  {195,  9,  0}, // idx = 1373, oldNumberOfOnes = 5, byte2Encode = 093
  {193,  9,  0}, // idx = 1374, oldNumberOfOnes = 5, byte2Encode = 094
  {447, 10,  0}, // idx = 1375, oldNumberOfOnes = 5, byte2Encode = 095
  {277,  9,  0}, // idx = 1376, oldNumberOfOnes = 5, byte2Encode = 096
  {235,  9,  0}, // idx = 1377, oldNumberOfOnes = 5, byte2Encode = 097
  {233,  9,  0}, // idx = 1378, oldNumberOfOnes = 5, byte2Encode = 098
  {279,  9,  0}, // idx = 1379, oldNumberOfOnes = 5, byte2Encode = 099
  {237,  9,  0}, // idx = 1380, oldNumberOfOnes = 5, byte2Encode = 100
  {275,  9,  0}, // idx = 1381, oldNumberOfOnes = 5, byte2Encode = 101
  {273,  9,  0}, // idx = 1382, oldNumberOfOnes = 5, byte2Encode = 102
  {239,  9,  0}, // idx = 1383, oldNumberOfOnes = 5, byte2Encode = 103
  {229,  9,  0}, // idx = 1384, oldNumberOfOnes = 5, byte2Encode = 104
  {283,  9,  0}, // idx = 1385, oldNumberOfOnes = 5, byte2Encode = 105
  {281,  9,  0}, // idx = 1386, oldNumberOfOnes = 5, byte2Encode = 106
  {231,  9,  0}, // idx = 1387, oldNumberOfOnes = 5, byte2Encode = 107
  {285,  9,  0}, // idx = 1388, oldNumberOfOnes = 5, byte2Encode = 108
  {227,  9,  0}, // idx = 1389, oldNumberOfOnes = 5, byte2Encode = 109
  {225,  9,  0}, // idx = 1390, oldNumberOfOnes = 5, byte2Encode = 110
  {287,  9,  0}, // idx = 1391, oldNumberOfOnes = 5, byte2Encode = 111
  {245,  9,  0}, // idx = 1392, oldNumberOfOnes = 5, byte2Encode = 112
  {267,  9,  0}, // idx = 1393, oldNumberOfOnes = 5, byte2Encode = 113
  {265,  9,  0}, // idx = 1394, oldNumberOfOnes = 5, byte2Encode = 114
  {247,  9,  0}, // idx = 1395, oldNumberOfOnes = 5, byte2Encode = 115
  {269,  9,  0}, // idx = 1396, oldNumberOfOnes = 5, byte2Encode = 116
  {243,  9,  0}, // idx = 1397, oldNumberOfOnes = 5, byte2Encode = 117
  {241,  9,  0}, // idx = 1398, oldNumberOfOnes = 5, byte2Encode = 118
  {271,  9,  0}, // idx = 1399, oldNumberOfOnes = 5, byte2Encode = 119
  {261,  9,  0}, // idx = 1400, oldNumberOfOnes = 5, byte2Encode = 120
  {251,  9,  0}, // idx = 1401, oldNumberOfOnes = 5, byte2Encode = 121
  {249,  9,  0}, // idx = 1402, oldNumberOfOnes = 5, byte2Encode = 122
  {263,  9,  0}, // idx = 1403, oldNumberOfOnes = 5, byte2Encode = 123
  {765, 10,  0}, // idx = 1404, oldNumberOfOnes = 5, byte2Encode = 124
  {259, 10,  0}, // idx = 1405, oldNumberOfOnes = 5, byte2Encode = 125
  {385, 10,  0}, // idx = 1406, oldNumberOfOnes = 5, byte2Encode = 126
  {575, 10,  0}, // idx = 1407, oldNumberOfOnes = 5, byte2Encode = 127
  { 85,  9,  1}, // idx = 1408, oldNumberOfOnes = 5, byte2Encode = 128
  {427,  9,  1}, // idx = 1409, oldNumberOfOnes = 5, byte2Encode = 129
  {425,  9,  1}, // idx = 1410, oldNumberOfOnes = 5, byte2Encode = 130
  { 87,  9,  1}, // idx = 1411, oldNumberOfOnes = 5, byte2Encode = 131
  {429,  9,  1}, // idx = 1412, oldNumberOfOnes = 5, byte2Encode = 132
  { 83,  9,  1}, // idx = 1413, oldNumberOfOnes = 5, byte2Encode = 133
  { 81,  9,  1}, // idx = 1414, oldNumberOfOnes = 5, byte2Encode = 134
  {431,  9,  1}, // idx = 1415, oldNumberOfOnes = 5, byte2Encode = 135
  {421,  9,  1}, // idx = 1416, oldNumberOfOnes = 5, byte2Encode = 136
  { 91,  9,  1}, // idx = 1417, oldNumberOfOnes = 5, byte2Encode = 137
  { 89,  9,  1}, // idx = 1418, oldNumberOfOnes = 5, byte2Encode = 138
  {423,  9,  1}, // idx = 1419, oldNumberOfOnes = 5, byte2Encode = 139
  { 93,  9,  1}, // idx = 1420, oldNumberOfOnes = 5, byte2Encode = 140
  {419,  9,  1}, // idx = 1421, oldNumberOfOnes = 5, byte2Encode = 141
  {417,  9,  1}, // idx = 1422, oldNumberOfOnes = 5, byte2Encode = 142
  { 95,  9,  1}, // idx = 1423, oldNumberOfOnes = 5, byte2Encode = 143
  {437,  9,  1}, // idx = 1424, oldNumberOfOnes = 5, byte2Encode = 144
  { 75,  9,  1}, // idx = 1425, oldNumberOfOnes = 5, byte2Encode = 145
  { 73,  9,  1}, // idx = 1426, oldNumberOfOnes = 5, byte2Encode = 146
  {439,  9,  1}, // idx = 1427, oldNumberOfOnes = 5, byte2Encode = 147
  { 77,  9,  1}, // idx = 1428, oldNumberOfOnes = 5, byte2Encode = 148
  {435,  9,  1}, // idx = 1429, oldNumberOfOnes = 5, byte2Encode = 149
  {433,  9,  1}, // idx = 1430, oldNumberOfOnes = 5, byte2Encode = 150
  { 79,  9,  1}, // idx = 1431, oldNumberOfOnes = 5, byte2Encode = 151
  { 69,  9,  1}, // idx = 1432, oldNumberOfOnes = 5, byte2Encode = 152
  {443,  9,  1}, // idx = 1433, oldNumberOfOnes = 5, byte2Encode = 153
  {441,  9,  1}, // idx = 1434, oldNumberOfOnes = 5, byte2Encode = 154
  { 71,  9,  1}, // idx = 1435, oldNumberOfOnes = 5, byte2Encode = 155
  {445,  9,  1}, // idx = 1436, oldNumberOfOnes = 5, byte2Encode = 156
  { 67,  9,  1}, // idx = 1437, oldNumberOfOnes = 5, byte2Encode = 157
  { 65,  9,  1}, // idx = 1438, oldNumberOfOnes = 5, byte2Encode = 158
  {191, 10,  1}, // idx = 1439, oldNumberOfOnes = 5, byte2Encode = 159
  {405,  9,  1}, // idx = 1440, oldNumberOfOnes = 5, byte2Encode = 160
  {107,  9,  1}, // idx = 1441, oldNumberOfOnes = 5, byte2Encode = 161
  {105,  9,  1}, // idx = 1442, oldNumberOfOnes = 5, byte2Encode = 162
  {407,  9,  1}, // idx = 1443, oldNumberOfOnes = 5, byte2Encode = 163
  {109,  9,  1}, // idx = 1444, oldNumberOfOnes = 5, byte2Encode = 164
  {403,  9,  1}, // idx = 1445, oldNumberOfOnes = 5, byte2Encode = 165
  {401,  9,  1}, // idx = 1446, oldNumberOfOnes = 5, byte2Encode = 166
  {111,  9,  1}, // idx = 1447, oldNumberOfOnes = 5, byte2Encode = 167
  {101,  9,  1}, // idx = 1448, oldNumberOfOnes = 5, byte2Encode = 168
  {411,  9,  1}, // idx = 1449, oldNumberOfOnes = 5, byte2Encode = 169
  {409,  9,  1}, // idx = 1450, oldNumberOfOnes = 5, byte2Encode = 170
  {103,  9,  1}, // idx = 1451, oldNumberOfOnes = 5, byte2Encode = 171
  {413,  9,  1}, // idx = 1452, oldNumberOfOnes = 5, byte2Encode = 172
  { 99,  9,  1}, // idx = 1453, oldNumberOfOnes = 5, byte2Encode = 173
  { 97,  9,  1}, // idx = 1454, oldNumberOfOnes = 5, byte2Encode = 174
  {415,  9,  1}, // idx = 1455, oldNumberOfOnes = 5, byte2Encode = 175
  {117,  9,  1}, // idx = 1456, oldNumberOfOnes = 5, byte2Encode = 176
  {395,  9,  1}, // idx = 1457, oldNumberOfOnes = 5, byte2Encode = 177
  {393,  9,  1}, // idx = 1458, oldNumberOfOnes = 5, byte2Encode = 178
  {119,  9,  1}, // idx = 1459, oldNumberOfOnes = 5, byte2Encode = 179
  {397,  9,  1}, // idx = 1460, oldNumberOfOnes = 5, byte2Encode = 180
  {115,  9,  1}, // idx = 1461, oldNumberOfOnes = 5, byte2Encode = 181
  {113,  9,  1}, // idx = 1462, oldNumberOfOnes = 5, byte2Encode = 182
  {399,  9,  1}, // idx = 1463, oldNumberOfOnes = 5, byte2Encode = 183
  {389,  9,  1}, // idx = 1464, oldNumberOfOnes = 5, byte2Encode = 184
  {123,  9,  1}, // idx = 1465, oldNumberOfOnes = 5, byte2Encode = 185
  {121,  9,  1}, // idx = 1466, oldNumberOfOnes = 5, byte2Encode = 186
  {391,  9,  1}, // idx = 1467, oldNumberOfOnes = 5, byte2Encode = 187
  {125,  9,  1}, // idx = 1468, oldNumberOfOnes = 5, byte2Encode = 188
  {387,  9,  1}, // idx = 1469, oldNumberOfOnes = 5, byte2Encode = 189
  {129, 10,  1}, // idx = 1470, oldNumberOfOnes = 5, byte2Encode = 190
  {831, 10,  1}, // idx = 1471, oldNumberOfOnes = 5, byte2Encode = 191
  {469,  9,  2}, // idx = 1472, oldNumberOfOnes = 5, byte2Encode = 192
  { 43,  9,  2}, // idx = 1473, oldNumberOfOnes = 5, byte2Encode = 193
  { 41,  9,  2}, // idx = 1474, oldNumberOfOnes = 5, byte2Encode = 194
  {471,  9,  2}, // idx = 1475, oldNumberOfOnes = 5, byte2Encode = 195
  { 45,  9,  2}, // idx = 1476, oldNumberOfOnes = 5, byte2Encode = 196
  {467,  9,  2}, // idx = 1477, oldNumberOfOnes = 5, byte2Encode = 197
  {465,  9,  2}, // idx = 1478, oldNumberOfOnes = 5, byte2Encode = 198
  { 47,  9,  2}, // idx = 1479, oldNumberOfOnes = 5, byte2Encode = 199
  { 37,  9,  2}, // idx = 1480, oldNumberOfOnes = 5, byte2Encode = 200
  {475,  9,  2}, // idx = 1481, oldNumberOfOnes = 5, byte2Encode = 201
  {473,  9,  2}, // idx = 1482, oldNumberOfOnes = 5, byte2Encode = 202
  { 39,  9,  2}, // idx = 1483, oldNumberOfOnes = 5, byte2Encode = 203
  {477,  9,  2}, // idx = 1484, oldNumberOfOnes = 5, byte2Encode = 204
  { 35,  9,  2}, // idx = 1485, oldNumberOfOnes = 5, byte2Encode = 205
  { 33,  9,  2}, // idx = 1486, oldNumberOfOnes = 5, byte2Encode = 206
  {479,  9,  2}, // idx = 1487, oldNumberOfOnes = 5, byte2Encode = 207
  { 53,  9,  2}, // idx = 1488, oldNumberOfOnes = 5, byte2Encode = 208
  {459,  9,  2}, // idx = 1489, oldNumberOfOnes = 5, byte2Encode = 209
  {457,  9,  2}, // idx = 1490, oldNumberOfOnes = 5, byte2Encode = 210
  { 55,  9,  2}, // idx = 1491, oldNumberOfOnes = 5, byte2Encode = 211
  {461,  9,  2}, // idx = 1492, oldNumberOfOnes = 5, byte2Encode = 212
  { 51,  9,  2}, // idx = 1493, oldNumberOfOnes = 5, byte2Encode = 213
  { 49,  9,  2}, // idx = 1494, oldNumberOfOnes = 5, byte2Encode = 214
  {463,  9,  2}, // idx = 1495, oldNumberOfOnes = 5, byte2Encode = 215
  {453,  9,  2}, // idx = 1496, oldNumberOfOnes = 5, byte2Encode = 216
  { 59,  9,  2}, // idx = 1497, oldNumberOfOnes = 5, byte2Encode = 217
  { 57,  9,  2}, // idx = 1498, oldNumberOfOnes = 5, byte2Encode = 218
  {455,  9,  2}, // idx = 1499, oldNumberOfOnes = 5, byte2Encode = 219
  { 61,  9,  2}, // idx = 1500, oldNumberOfOnes = 5, byte2Encode = 220
  {451,  9,  2}, // idx = 1501, oldNumberOfOnes = 5, byte2Encode = 221
  {449,  9,  2}, // idx = 1502, oldNumberOfOnes = 5, byte2Encode = 222
  {959, 10,  2}, // idx = 1503, oldNumberOfOnes = 5, byte2Encode = 223
  { 21,  9,  3}, // idx = 1504, oldNumberOfOnes = 5, byte2Encode = 224
  {491,  9,  3}, // idx = 1505, oldNumberOfOnes = 5, byte2Encode = 225
  {489,  9,  3}, // idx = 1506, oldNumberOfOnes = 5, byte2Encode = 226
  { 23,  9,  3}, // idx = 1507, oldNumberOfOnes = 5, byte2Encode = 227
  {493,  9,  3}, // idx = 1508, oldNumberOfOnes = 5, byte2Encode = 228
  { 19,  9,  3}, // idx = 1509, oldNumberOfOnes = 5, byte2Encode = 229
  { 17,  9,  3}, // idx = 1510, oldNumberOfOnes = 5, byte2Encode = 230
  {495,  9,  3}, // idx = 1511, oldNumberOfOnes = 5, byte2Encode = 231
  {485,  9,  3}, // idx = 1512, oldNumberOfOnes = 5, byte2Encode = 232
  { 27,  9,  3}, // idx = 1513, oldNumberOfOnes = 5, byte2Encode = 233
  { 25,  9,  3}, // idx = 1514, oldNumberOfOnes = 5, byte2Encode = 234
  {487,  9,  3}, // idx = 1515, oldNumberOfOnes = 5, byte2Encode = 235
  { 29,  9,  3}, // idx = 1516, oldNumberOfOnes = 5, byte2Encode = 236
  {483,  9,  3}, // idx = 1517, oldNumberOfOnes = 5, byte2Encode = 237
  {481,  9,  3}, // idx = 1518, oldNumberOfOnes = 5, byte2Encode = 238
  { 31,  9,  3}, // idx = 1519, oldNumberOfOnes = 5, byte2Encode = 239
  {501,  9,  4}, // idx = 1520, oldNumberOfOnes = 5, byte2Encode = 240
  { 11,  9,  4}, // idx = 1521, oldNumberOfOnes = 5, byte2Encode = 241
  {  9,  9,  4}, // idx = 1522, oldNumberOfOnes = 5, byte2Encode = 242
  {503,  9,  4}, // idx = 1523, oldNumberOfOnes = 5, byte2Encode = 243
  { 13,  9,  4}, // idx = 1524, oldNumberOfOnes = 5, byte2Encode = 244
  {499,  9,  4}, // idx = 1525, oldNumberOfOnes = 5, byte2Encode = 245
  {497,  9,  4}, // idx = 1526, oldNumberOfOnes = 5, byte2Encode = 246
  { 15,  9,  4}, // idx = 1527, oldNumberOfOnes = 5, byte2Encode = 247
  {517, 10,  0}, // idx = 1528, oldNumberOfOnes = 5, byte2Encode = 248
  {507, 10,  0}, // idx = 1529, oldNumberOfOnes = 5, byte2Encode = 249
  {505, 10,  0}, // idx = 1530, oldNumberOfOnes = 5, byte2Encode = 250
  {519, 10,  0}, // idx = 1531, oldNumberOfOnes = 5, byte2Encode = 251
  {253, 10,  1}, // idx = 1532, oldNumberOfOnes = 5, byte2Encode = 252
  {771, 10,  1}, // idx = 1533, oldNumberOfOnes = 5, byte2Encode = 253
  {897, 10,  2}, // idx = 1534, oldNumberOfOnes = 5, byte2Encode = 254
  { 63, 10,  3}, // idx = 1535, oldNumberOfOnes = 5, byte2Encode = 255
};
