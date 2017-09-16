/*
 * sin.c - Trigonometric functions
 */

#include "main.h"
#include "dac.h"

// Cannot fit 16384 entries in memory, go with fewer
#define SIN_TABLE_ENTRIES 512
#define SIN_SCALAR (B_RAD / (4 * SIN_TABLE_ENTRIES))

// SIN[x] should be 30000 * sin(pi * x / 32768), except having 16,384 16bit sine entries
// is too much; therefore, SIN[x] = 30000 * sin(pi * x / 1024)
//  30K is used because SCALE_AUDIO takes signed 16-bit, and we want to avoid saturating
static const uint16_t SIN[SIN_TABLE_ENTRIES + 1] = {
	0, 92, 184, 276, 368, 460, 552, 644, 736, 828, 920, 1012, 1104, 1196, 1288, 1380, 1472,
	1563, 1655, 1747, 1839, 1931, 2023, 2115, 2206, 2298, 2390, 2482, 2573, 2665, 2757, 2848,
	2940, 3032, 3123, 3215, 3306, 3398, 3489, 3580, 3672, 3763, 3854, 3946, 4037, 4128, 4219,
	4310, 4401, 4492, 4583, 4674, 4765, 4856, 4947, 5038, 5128, 5219, 5310, 5400, 5491, 5581,
	5672, 5762, 5852, 5942, 6033, 6123, 6213, 6303, 6393, 6483, 6573, 6662, 6752, 6842, 6931,
	7021, 7110, 7200, 7289, 7378, 7467, 7556, 7645, 7734, 7823, 7912, 8001, 8090, 8178, 8267,
	8355, 8443, 8532, 8620, 8708, 8796, 8884, 8972, 9060, 9147, 9235, 9323, 9410, 9497, 9585,
	9672, 9759, 9846, 9933, 10019, 10106, 10193, 10279, 10366, 10452, 10538, 10624, 10710,
	10796, 10882, 10968, 11054, 11139, 11224, 11310, 11395, 11480, 11565, 11650, 11735, 11819,
	11904, 11988, 12073, 12157, 12241, 12325, 12409, 12492, 12576, 12660, 12743, 12826, 12909,
	12992, 13075, 13158, 13241, 13323, 13406, 13488, 13570, 13652, 13734, 13816, 13897, 13979,
	14060, 14141, 14223, 14303, 14384, 14465, 14546, 14626, 14706, 14786, 14866, 14946, 15026,
	15106, 15185, 15264, 15344, 15423, 15501, 15580, 15659, 15737, 15815, 15894, 15972, 16049,
	16127, 16205, 16282, 16359, 16436, 16513, 16590, 16667, 16743, 16819, 16895, 16971, 17047,
	17123, 17198, 17274, 17349, 17424, 17499, 17573, 17648, 17722, 17796, 17870, 17944, 18018,
	18091, 18165, 18238, 18311, 18384, 18456, 18529, 18601, 18673, 18745, 18817, 18889, 18960,
	19031, 19102, 19173, 19244, 19314, 19385, 19455, 19525, 19595, 19664, 19734, 19803, 19872,
	19941, 20009, 20078, 20146, 20214, 20282, 20350, 20418, 20485, 20552, 20619, 20686, 20752,
	20819, 20885, 20951, 21017, 21082, 21148, 21213, 21278, 21342, 21407, 21471, 21536, 21600,
	21663, 21727, 21790, 21853, 21916, 21979, 22042, 22104, 22166, 22228, 22290, 22351, 22413,
	22474, 22534, 22595, 22656, 22716, 22776, 22836, 22895, 22955, 23014, 23073, 23131, 23190,
	23248, 23306, 23364, 23422, 23479, 23536, 23593, 23650, 23706, 23763, 23819, 23875, 23930,
	23986, 24041, 24096, 24150, 24205, 24259, 24313, 24367, 24421, 24474, 24527, 24580, 24633,
	24685, 24737, 24789, 24841, 24892, 24944, 24995, 25045, 25096, 25146, 25196, 25246, 25296,
	25345, 25394, 25443, 25492, 25540, 25588, 25636, 25684, 25731, 25779, 25826, 25872, 25919,
	25965, 26011, 26057, 26102, 26147, 26192, 26237, 26282, 26326, 26370, 26414, 26457, 26500,
	26543, 26586, 26629, 26671, 26713, 26755, 26796, 26837, 26878, 26919, 26960, 27000, 27040,
	27080, 27119, 27158, 27197, 27236, 27275, 27313, 27351, 27388, 27426, 27463, 27500, 27537,
	27573, 27609, 27645, 27681, 27716, 27751, 27786, 27820, 27855, 27889, 27923, 27956, 27989,
	28022, 28055, 28087, 28120, 28152, 28183, 28215, 28246, 28277, 28307, 28338, 28368, 28398,
	28427, 28456, 28485, 28514, 28543, 28571, 28599, 28626, 28654, 28681, 28708, 28734, 28761,
	28787, 28812, 28838, 28863, 28888, 28913, 28937, 28961, 28985, 29009, 29032, 29055, 29078,
	29100, 29123, 29145, 29166, 29188, 29209, 29230, 29250, 29271, 29291, 29310, 29330, 29349,
	29368, 29387, 29405, 29423, 29441, 29458, 29476, 29493, 29509, 29526, 29542, 29558, 29573,
	29589, 29604, 29619, 29633, 29647, 29661, 29675, 29688, 29701, 29714, 29727, 29739, 29751,
	29762, 29774, 29785, 29796, 29806, 29817, 29827, 29836, 29846, 29855, 29864, 29873, 29881,
	29889, 29897, 29904, 29911, 29918, 29925, 29931, 29937, 29943, 29949, 29954, 29959, 29963,
	29968, 29972, 29976, 29979, 29982, 29985, 29988, 29990, 29993, 29994, 29996, 29997, 29998,
	29999, 29999, 30000
};

// Calculates the sine of the unsigned angle in brad (65536)
int32_t sinfp(uint32_t angle) {
	bool sgn = false;
	uint32_t li, remainder;
	int32_t ret;
#if 0
	angle %= B_RAD;
#else
	angle &= (B_RAD - 1);
#endif
	// sin(x|x>pi) = -sin(x-pi)
	if (angle > (B_RAD >> 1)) {
		sgn = true;
		angle -= (B_RAD >> 1);
	}
	// sin(x|x>pi/2) = sin(pi-x)
	if (angle > (B_RAD >> 2))
		angle = (B_RAD >> 1) - angle;
	// Linearly interpolate between SIN table entries; if the compiler misses this optimization
	// into a shift, I will personally...
	// angle is in 0 .. (B_RAD >> 2)
	li = angle / SIN_SCALAR;
	remainder = angle - (li * SIN_SCALAR);
	// remainder is from 0..SIN_SCALAR
	ret = (int32_t)SIN[li];
	ret += (((int32_t)SIN[li + 1] - ret) * remainder + (SIN_SCALAR >> 1)) / SIN_SCALAR;
	if (sgn) ret = -ret;
	return ret;
}
