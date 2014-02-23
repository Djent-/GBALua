#define GBAREG_H
#define GBAREG_H

#define REG_DISPCNT     *(unsigned*)0x04000000
#define REG_DISPSTAT    *(volatile unsigned short*)0x04000004
#define REG_VCOUNT      *(volatile unsigned short*)0x04000006
#define REG_BG0CNT      *(unsigned short*)0x04000008
#define REG_BG1CNT      *(unsigned short*)0x0400000A
#define REG_BG2CNT      *(unsigned short*)0x0400000C
#define REG_BG3CNT      *(unsigned short*)0x0400000E
#define REG_BG0HOFS     *(unsigned short*)0x04000010
#define REG_BG0VOFS     *(unsigned short*)0x04000012
#define REG_BG1HOFS     *(unsigned short*)0x04000014
#define REG_BG1VOFS     *(unsigned short*)0x04000016
#define REG_BG2HOFS     *(unsigned short*)0x04000018
#define REG_BG2VOFS     *(unsigned short*)0x0400001A
#define REG_BG3HOFS     *(unsigned short*)0x0400001C
#define REG_BG3VOFS     *(unsigned short*)0x0400001E
#define REG_BG2PA       *(unsigned short*)0x04000020
#define REG_BG2PB       *(unsigned short*)0x04000022
#define REG_BG2PC       *(unsigned short*)0x04000024
#define REG_BG2PD       *(unsigned short*)0x04000026
#define REG_BG2X        *(unsigned*)0x04000028
#define REG_BG2X_L      *(unsigned short*)0x04000028
#define REG_BG2X_H      *(unsigned short*)0x0400002A
#define REG_BG2Y        *(unsigned*)0x0400002C
#define REG_BG2Y_L      *(unsigned short*)0x0400002C
#define REG_BG2Y_H      *(unsigned short*)0x0400002E
#define REG_BG3PA       *(unsigned short*)0x04000030
#define REG_BG3PB       *(unsigned short*)0x04000032
#define REG_BG3PC       *(unsigned short*)0x04000034
#define REG_BG3PD       *(unsigned short*)0x04000036
#define REG_BG3X        *(unsigned*)0x04000038
#define REG_BG3X_L      *(unsigned short*)0x04000038
#define REG_BG3X_H      *(unsigned short*)0x0400003A
#define REG_BG3Y        *(unsigned*)0x0400003C
#define REG_BG3Y_L      *(unsigned short*)0x0400003C
#define REG_BG3Y_H      *(unsigned short*)0x0400003E
#define REG_WIN0H       *(unsigned short*)0x04000040
#define REG_WIN1H       *(unsigned short*)0x04000042
#define REG_WIN0V       *(unsigned short*)0x04000044
#define REG_WIN1V       *(unsigned short*)0x04000046
#define REG_WININ       *(unsigned short*)0x04000048
#define REG_WINOUT      *(unsigned short*)0x0400004A
#define REG_MOSAIC      *(unsigned*)0x0400004C
#define REG_MOSAIC_L    *(unsigned*)0x0400004C
#define REG_MOSAIC_H    *(unsigned*)0x0400004E
#define REG_BLDMOD      *(unsigned short*)0x04000050
#define REG_COLEY1      *(unsigned short*)0x04000052
#define REG_COLEY2      *(unsigned short*)0x04000054
#define REG_SG10        *(unsigned*)0x04000060
#define REG_SG10_L      *(unsigned short*)0x04000060
#define REG_SG10_H      *(unsigned short*)0x04000062
#define REG_SG11        *(unsigned short*)0x04000064
#define REG_SG20        *(unsigned short*)0x04000068
#define REG_SG21        *(unsigned short*)0x0400006C
#define REG_SG30        *(unsigned*)0x04000070
#define REG_SG30_L      *(unsigned short*)0x04000070
#define REG_SG30_H      *(unsigned short*)0x04000072
#define REG_SG31        *(unsigned short*)0x04000074
#define REG_SG40        *(unsigned short*)0x04000078
#define REG_SG41        *(unsigned short*)0x0400007C
#define REG_SGCNT0      *(unsigned*)0x04000080
#define REG_SGCNT0_L    *(unsigned short*)0x04000080
#define REG_SGCNT0_H    *(unsigned short*)0x04000082
#define REG_SGCNT1      *(unsigned short*)0x04000084
#define REG_SGBIAS      *(unsigned short*)0x04000088
#define REG_SGWR0       *(unsigned*)0x04000090
#define REG_SGWR0_L     *(unsigned short*)0x04000090
#define REG_SGWR0_H     *(unsigned short*)0x04000092
#define REG_SGWR1       *(unsigned*)0x04000094
#define REG_SGWR1_L     *(unsigned short*)0x04000094
#define REG_SGWR1_H     *(unsigned short*)0x04000096
#define REG_SGWR2       *(unsigned*)0x04000098
#define REG_SGWR2_L     *(unsigned short*)0x04000098
#define REG_SGWR2_H     *(unsigned short*)0x0400009A
#define REG_SGWR3       *(unsigned*)0x0400009C
#define REG_SGWR3_L     *(unsigned short*)0x0400009C
#define REG_SGWR3_H     *(unsigned short*)0x0400009E
#define REG_SGFIFOA     *(unsigned*)0x040000A0
#define REG_SGFIFOA_L   *(unsigned short*)0x040000A0
#define REG_SGFIFOA_H   *(unsigned short*)0x040000A2
#define REG_SGFIFOB     *(unsigned*)0x040000A4
#define REG_SGFIFOB_L   *(unsigned short*)0x040000A4
#define REG_SGFIFOB_H   *(unsigned short*)0x040000A6
#define REG_DM0SAD      *(unsigned*)0x040000B0
#define REG_DM0SAD_L    *(unsigned short*)0x040000B0
#define REG_DM0SAD_H    *(unsigned short*)0x040000B2
#define REG_DM0DAD      *(unsigned*)0x040000B4
#define REG_DM0DAD_L    *(unsigned short*)0x040000B4
#define REG_DM0DAD_H    *(unsigned short*)0x040000B6
#define REG_DM0CNT      *(unsigned*)0x040000B8
#define REG_DM0CNT_L    *(unsigned short*)0x040000B8
#define REG_DM0CNT_H    *(unsigned short*)0x040000BA
#define REG_DM1SAD      *(unsigned*)0x040000BC
#define REG_DM1SAD_L    *(unsigned short*)0x040000BC
#define REG_DM1SAD_H    *(unsigned short*)0x040000BE
#define REG_DM1DAD      *(unsigned*)0x040000C0
#define REG_DM1DAD_L    *(unsigned short*)0x040000C0
#define REG_DM1DAD_H    *(unsigned short*)0x040000C2
#define REG_DM1CNT      *(unsigned*)0x040000C4
#define REG_DM1CNT_L    *(unsigned short*)0x040000C4
#define REG_DM1CNT_H    *(unsigned short*)0x040000C6
#define REG_DM2SAD      *(unsigned*)0x040000C8
#define REG_DM2SAD_L    *(unsigned short*)0x040000C8
#define REG_DM2SAD_H    *(unsigned short*)0x040000CA
#define REG_DM2DAD      *(unsigned*)0x040000CC
#define REG_DM2DAD_L    *(unsigned short*)0x040000CC
#define REG_DM2DAD_H    *(unsigned short*)0x040000CE
#define REG_DM2CNT      *(unsigned*)0x040000D0
#define REG_DM2CNT_L    *(unsigned short*)0x040000D0
#define REG_DM2CNT_H    *(unsigned short*)0x040000D2
#define REG_DM3SAD      *(unsigned*)0x040000D4
#define REG_DM3SAD_L    *(unsigned short*)0x040000D4
#define REG_DM3SAD_H    *(unsigned short*)0x040000D6
#define REG_DM3DAD      *(unsigned*)0x040000D8
#define REG_DM3DAD_L    *(unsigned short*)0x040000D8
#define REG_DM3DAD_H    *(unsigned short*)0x040000DA
#define REG_DM3CNT      *(unsigned*)0x040000DC
#define REG_DM3CNT_L    *(unsigned short*)0x040000DC
#define REG_DM3CNT_H    *(unsigned short*)0x040000DE
#define REG_TM0D        *(volatile unsigned short*)0x04000100
#define REG_TM0CNT      *(unsigned short*)0x04000102
#define REG_TM1D        *(volatile unsigned short*)0x04000104
#define REG_TM1CNT      *(unsigned short*)0x04000106
#define REG_TM2D        *(volatile unsigned short*)0x04000108
#define REG_TM2CNT      *(unsigned short*)0x0400010A
#define REG_TM3D        *(volatile unsigned short*)0x0400010C
#define REG_TM3CNT      *(unsigned short*)0x0400010E
#define REG_SCD0        *(unsigned short*)0x04000120
#define REG_SCD1        *(unsigned short*)0x04000122
#define REG_SCD2        *(unsigned short*)0x04000124
#define REG_SCD3        *(unsigned short*)0x04000126
#define REG_SCCNT       *(unsigned*)0x04000128
#define REG_SCCNT_L     *(volatile unsigned short*)0x04000128
#define REG_SCCNT_H     *(unsigned short*)0x0400012A
#define REG_P1          *(unsigned short*)0x04000130
#define REG_P1CNT       *(unsigned short*)0x04000132
#define REG_R           *(unsigned short*)0x04000134
#define REG_HS_CTRL     *(unsigned short*)0x04000140
#define REG_JOYRE       *(unsigned*)0x04000150
#define REG_JOYRE_L     *(unsigned short*)0x04000150
#define REG_JOYRE_H     *(unsigned short*)0x04000152
#define REG_JOYTR       *(unsigned*)0x04000154
#define REG_JOYTR_L     *(unsigned short*)0x04000154
#define REG_JOYTR_H     *(unsigned short*)0x04000156
#define REG_JSTAT       *(unsigned*)0x04000158
#define REG_JSTAT_L     *(unsigned short*)0x04000158
#define REG_JSTAT_H     *(unsigned short*)0x0400015A
#define REG_IE          *(unsigned short*)0x04000200
#define REG_IF          *(volatile unsigned short*)0x04000202
#define REG_WSCNT       *(unsigned short*)0x04000204
#define REG_IME         *(unsigned short*)0x04000208
#define REG_PAUSE       *(unsigned short*)0x04000300

#define GBAREG_H

