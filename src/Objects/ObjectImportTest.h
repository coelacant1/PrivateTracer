#pragma once

#include "..\Render\Object3D.h"
#include "..\Materials\SimpleMaterial.h"

class ObjectImportTest{
private:
    Vector3D basisVertices[128] = {Vector3D(-180.2830f,22.1568f,-65.0000f),Vector3D(-152.1540f,43.1344f,-65.0000f),Vector3D(-171.2245f,13.0983f,-65.0000f),Vector3D(-148.8166f,31.6921f,-65.0000f),Vector3D(-105.4312f,15.9589f,-65.0000f),Vector3D(-106.3847f,25.9709f,-65.0000f),Vector3D(-59.1851f,40.7506f,-65.0000f),Vector3D(-63.4760f,49.3323f,-65.0000f),Vector3D(-45.3590f,71.7402f,-65.0000f),Vector3D(-41.8793f,56.3165f,-65.0000f),Vector3D(-32.1758f,56.7576f,-65.0000f),Vector3D(-23.4279f,72.2169f,-65.0000f),Vector3D(-16.2765f,48.3788f,-65.0000f),Vector3D(-21.9976f,41.2273f,-65.0000f),Vector3D(-100.9826f,80.5753f,-65.0000f),Vector3D(-83.0233f,63.1584f,-65.0000f),Vector3D(-80.2524f,88.5146f,-65.0000f),Vector3D(-75.8719f,66.0190f,-65.0000f),Vector3D(-70.1507f,68.4028f,-65.0000f),Vector3D(-49.1731f,86.0431f,-65.0000f),Vector3D(-73.1952f,92.0431f,-65.0000f),Vector3D(-59.9632f,97.3360f,-65.0000f),Vector3D(-34.3935f,88.4269f,-65.0000f),Vector3D(-42.4984f,88.9037f,-65.0000f),Vector3D(-50.2596f,100.4235f,-65.0000f),Vector3D(-31.2936f,107.4806f,-65.0000f),Vector3D(-118.7805f,122.2771f,-65.0000f),Vector3D(-125.4552f,97.0086f,-65.0000f),Vector3D(-101.1403f,107.9742f,-65.0000f),Vector3D(-112.5826f,141.3476f,-65.0000f),Vector3D(-77.7789f,117.9862f,-65.0000f),Vector3D(-94.8077f,153.7929f,-65.0000f),Vector3D(-59.6619f,125.6144f,-65.0000f),Vector3D(-74.9183f,167.0928f,-65.0000f),Vector3D(-52.9872f,152.7899f,-65.0000f),Vector3D(-30.1026f,138.4870f,-65.0000f),Vector3D(-129.2693f,113.6953f,-65.0000f),Vector3D(-133.0834f,124.1841f,-65.0000f),Vector3D(-119.7341f,146.1152f,-65.0000f),Vector3D(-124.5017f,154.2202f,-65.0000f),Vector3D(-97.8030f,173.7675f,-65.0000f),Vector3D(-81.5930f,171.3837f,-65.0000f),Vector3D(-197.9232f,58.3908f,-65.0000f),Vector3D(-190.7718f,86.0431f,-65.0000f),Vector3D(-189.3415f,64.5887f,-65.0000f),Vector3D(-185.5274f,79.3684f,-65.0000f),Vector3D(-173.1315f,82.2290f,-65.0000f),Vector3D(-166.9336f,91.2875f,-65.0000f),Vector3D(-4.8341f,133.2426f,-65.0000f),Vector3D(-1.0200f,140.8708f,-65.0000f),Vector3D(-58.2316f,202.8501f,-65.0000f),Vector3D(-21.0441f,196.6521f,-65.0000f),Vector3D(61.9127f,246.7123f,-65.0000f),Vector3D(96.2397f,284.3766f,-65.0000f),Vector3D(-10.0785f,192.8380f,-65.0000f),Vector3D(2.7941f,147.5455f,-65.0000f),Vector3D(20.9111f,166.6161f,-65.0000f),Vector3D(21.8646f,190.9310f,-65.0000f),Vector3D(32.3534f,200.9430f,-65.0000f),Vector3D(53.3310f,230.9791f,-65.0000f),Vector3D(15.6667f,132.2891f,-65.0000f),Vector3D(74.3086f,186.6401f,-65.0000f),Vector3D(48.5633f,145.1617f,-65.0000f),Vector3D(84.7973f,172.3372f,-65.0000f),Vector3D(151.5442f,176.6281f,-65.0000f),Vector3D(186.8247f,185.2098f,-65.0000f),Vector3D(67.1571f,147.5455f,-65.0000f),Vector3D(90.0417f,165.1858f,-65.0000f),Vector3D(107.2052f,147.0688f,-65.0000f),Vector3D(134.8575f,167.0928f,-65.0000f),Vector3D(19.0040f,127.0447f,-65.0000f),Vector3D(52.8542f,139.9173f,-65.0000f),Vector3D(129.6131f,124.6609f,-65.0000f),Vector3D(114.3567f,139.4406f,-65.0000f),Vector3D(153.9280f,171.3837f,-65.0000f),Vector3D(188.2550f,180.4422f,-65.0000f),Vector3D(2.3173f,108.4509f,-65.0000f),Vector3D(12.3293f,96.5319f,-65.0000f),Vector3D(15.6667f,40.2738f,-65.0000f),Vector3D(2.3173f,13.5751f,-65.0000f),Vector3D(30.9231f,37.4132f,-65.0000f),Vector3D(48.5633f,25.9709f,-65.0000f),Vector3D(10.4223f,112.7418f,-65.0000f),Vector3D(48.0866f,110.3580f,-65.0000f),Vector3D(20.9111f,99.3924f,-65.0000f),Vector3D(35.6907f,94.6248f,-65.0000f),Vector3D(41.4119f,46.4717f,-65.0000f),Vector3D(52.3774f,35.5062f,-65.0000f),Vector3D(58.5754f,110.8348f,-65.0000f),Vector3D(70.4944f,101.7763f,-65.0000f),Vector3D(62.3895f,33.1224f,-65.0000f),Vector3D(77.6459f,56.0070f,-65.0000f),Vector3D(90.0417f,53.1464f,-65.0000f),Vector3D(102.9143f,45.9950f,-65.0000f),Vector3D(70.0177f,113.2186f,-65.0000f),Vector3D(105.2982f,110.8348f,-65.0000f),Vector3D(79.5530f,104.6368f,-65.0000f),Vector3D(93.8558f,99.3924f,-65.0000f),Vector3D(100.5305f,58.3908f,-65.0000f),Vector3D(112.4496f,50.7626f,-65.0000f),Vector3D(112.4496f,110.8348f,-65.0000f),Vector3D(124.3687f,103.2065f,-65.0000f),Vector3D(123.4152f,51.7161f,-65.0000f),Vector3D(140.5786f,75.5543f,-65.0000f),Vector3D(160.1259f,74.6008f,-65.0000f),Vector3D(177.7662f,67.4493f,-65.0000f),Vector3D(-123.8090f,93.5348f,-65.0000f),Vector3D(-109.3982f,89.0057f,-65.0000f),Vector3D(-69.8712f,104.0342f,-65.0000f),Vector3D(-71.9299f,112.6807f,-65.0000f),Vector3D(-28.9032f,131.8266f,-65.0000f),Vector3D(-33.4323f,120.0920f,-65.0000f),Vector3D(-296.1408f,194.8713f,-65.0000f),Vector3D(-295.2308f,155.7414f,-65.0000f),Vector3D(-254.2810f,154.8314f,-65.0000f),Vector3D(-255.6460f,193.9613f,-65.0000f),Vector3D(-295.2308f,208.0662f,-65.0000f),Vector3D(-295.2308f,244.0111f,-65.0000f),Vector3D(-256.5560f,244.0111f,-65.0000f),Vector3D(-257.4660f,208.0662f,-65.0000f),Vector3D(-245.1811f,242.6461f,-65.0000f),Vector3D(-244.7261f,207.1562f,-65.0000f),Vector3D(-208.3262f,242.6461f,-65.0000f),Vector3D(-208.3262f,207.6112f,-65.0000f),Vector3D(-242.9061f,192.1413f,-65.0000f),Vector3D(-241.9961f,157.5614f,-65.0000f),Vector3D(-207.4162f,156.1964f,-65.0000f),Vector3D(-206.9612f,192.5963f,-65.0000f)};
    IndexGroup basisIndexes[84] = {IndexGroup(2,1,0),IndexGroup(3,1,2),IndexGroup(3,4,1),IndexGroup(4,5,1),IndexGroup(4,6,5),IndexGroup(6,7,5),IndexGroup(9,8,7),IndexGroup(9,7,6),IndexGroup(9,10,8),IndexGroup(10,11,8),IndexGroup(10,12,11),IndexGroup(10,13,12),IndexGroup(14,15,16),IndexGroup(15,17,16),IndexGroup(18,19,20),IndexGroup(19,21,20),IndexGroup(24,23,22),IndexGroup(22,25,24),IndexGroup(26,27,28),IndexGroup(28,29,26),IndexGroup(28,30,29),IndexGroup(30,31,29),IndexGroup(30,32,31),IndexGroup(32,33,31),IndexGroup(32,34,33),IndexGroup(32,35,34),IndexGroup(38,37,36),IndexGroup(38,39,37),IndexGroup(38,40,39),IndexGroup(38,41,40),IndexGroup(44,43,42),IndexGroup(44,45,43),IndexGroup(45,46,43),IndexGroup(46,47,43),IndexGroup(48,49,50),IndexGroup(49,51,50),IndexGroup(50,51,52),IndexGroup(52,53,50),IndexGroup(54,55,56),IndexGroup(57,54,56),IndexGroup(58,54,57),IndexGroup(54,58,59),IndexGroup(62,61,60),IndexGroup(63,61,62),IndexGroup(63,64,61),IndexGroup(64,65,61),IndexGroup(68,67,66),IndexGroup(68,69,67),IndexGroup(72,71,70),IndexGroup(72,73,71),IndexGroup(72,74,73),IndexGroup(72,75,74),IndexGroup(78,77,76),IndexGroup(76,79,78),IndexGroup(79,80,78),IndexGroup(79,81,80),IndexGroup(84,83,82),IndexGroup(84,85,83),IndexGroup(86,83,85),IndexGroup(86,87,83),IndexGroup(90,89,88),IndexGroup(90,91,89),IndexGroup(90,92,91),IndexGroup(90,93,92),IndexGroup(96,95,94),IndexGroup(96,97,95),IndexGroup(97,98,95),IndexGroup(95,98,99),IndexGroup(102,101,100),IndexGroup(102,103,101),IndexGroup(102,104,103),IndexGroup(102,105,104),IndexGroup(106,107,108),IndexGroup(108,109,106),IndexGroup(108,110,109),IndexGroup(108,111,110),IndexGroup(112,113,114),IndexGroup(114,115,112),IndexGroup(118,117,116),IndexGroup(116,119,118),IndexGroup(120,121,122),IndexGroup(121,123,122),IndexGroup(124,125,126),IndexGroup(126,127,124)};
    TriangleGroup triangleGroup = TriangleGroup(&basisVertices[0], &basisIndexes[0], 128, 84);
    SimpleMaterial simpleMaterial = SimpleMaterial(RGBColor(128, 128, 128));
    Object3D basisObj = Object3D(&triangleGroup, &simpleMaterial);

public:
    ObjectImportTest(){}

    Object3D* GetObject(){
        return &basisObj;
    }
};
