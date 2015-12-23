/* nagios_hpilo_snmp.h -- types and prototypes used by nagios iLO plug-in 
   (C) Copyright [2015] Hewlett-Packard Enterprise Development Company, L.P.

   This program is free software; you can redistribute it and/or modify 
   it under the terms of version 2 of the GNU General Public License as 
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License 
   along with this program; if not, write to:
    Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA 02110-1301, USA.  */

/* Written by Adrian Huang <adrian.huang@hp.com>.  */

#ifndef NAGIOS_HPILO_SNMP_H
#define NAGIOS_HPILO_SNMP_H

#include "nagios_hpilo_common.h"

/* Default SNMP port number of HP iLO.  */
#define NAGIOS_ILO_SNMP_PORT	161

/* 3-second timeout.  */
#define NAGIOS_ILO_SNMP_TIMEOUT		(3 * 1000000) 
#define NAGIOS_ILO_SNMP_RETRIES		3


#define SNMP_GETBULK_ERRINDEX	10

/* Power Supply Table name tags for detailed information */
char *ps_tbl_ele[18] = {"Chassis", "Bay","Present","Condition",\
			"Status","MainVoltage", "CapacityUsed",\
			"CapacityMaximum", "Redundant","Model",\
			"SerialNumber","AutoRev","HotPlug","FirmwareRev",\
			"HwLocation","SparePartNum","RedundantPartner","ErrorCondition" \
};

/* Fan Table name tags for detailed information */
char *fan_tbl_ele[12] = { "FanChassis","FanIndex","FanLocale","FanPresent","FanType","FanSpeed",\
			"FanRedundant","FanRedundantPartner","FanCondition","FanHotPlug",\
			"FanHwLocation","FanCurrentSpeed"\
};

/* Temperature Table name tags for detailed information */
char *temp_tbl_ele[8] = { "TmpChas","TmpInd","TmpLocale","TmpCelsius",\
			   "TmpThres","TmpCond","TmpThresType","TmpHwLocation"\
};

/* Storage Table name tags for detailed information */
char *st_tbl_ele[43] = { "st_Ind","st_Model","st_FWRev","st_StndIntr","st_Slot","st_Cond",\
			"st_Prod_Rev","st_PartnerSlot","st_Cur_Role","st_BoardStat","st_PartnerBoardStat",\
			"st_BoardCond","st_PartnerBoardCond","st_DriveOwnership","st_SerlNum",\
			"st_RedundancyType","st_RedundancyError","st_AccessModuleStat","st_DaughterBoardType",\
			"st_HwLoc","st_NumOfBuses","st_BlinkTime","st_RebuildPriority","st_ExpandPriority",\
			"st_NumOfInt_Ports","st_NumOfExt_Ports","st_DriveWriteCacheStat","st_PartnerSerNum",\
			"st_OptionRomRev","st_HbaFWRev","st_HBAModeOptionRomRev","st_Cur_Temp","st_LastLockupCode",\
			"st_EncryptionStat","st_ASICEncptSelfTestStat","st_EncryptCspNvramStat","st_EncryptRTCSramStat",\
			"st_EncryptCntlrPwdStat","st_EncryptBypassModeStat","NA","NA","NA","NA"\
};

/* Memory Table name tags for detailed information */
char *mem_tbl_ele[] = { "Mem2BoardInd","Mem2BoardSlotNum","Mem2BoardCpuNum","Mem2BoardRiserNum","Mem2BoardOnlineStat",\
			"Mem2BoardErrorStat","Mem2BoardLocked","Mem2BoardNumSock","Mem2BoardOsMemSize","Mem2BoardTotalMemSize",\
			"Mem2BoardCond","Mem2BoardHotPlug","Mem2BoardOper_Freq","Mem2BoardOper_Volt"\
};

/* Processor Table name tags for detailed information */
char *pro_tbl_ele[] = { "CpuUnitInd","CpuSlot","CpuName","CpuSpeed","CpuStep","CpuStat","CpuExtSpeed","CpuDesigner","CpuSocketNum",\
			"CpuThreshPassed","CpuHwLocation","CpuCellTablePtr","CpuPowerpodStat","CpuArchRev","CpuCore","CPUSerialNum",\
			"CPUPartNum","CPUSerialNumMfgr","CPUPartNumMfgr","CPUCoreInd","CPUMaxSpeed","CPUCoreThreadIndex",\
			"CPUChipGenerationName","CPUMultiThreadStat","CPUCoreMaxThreads","CpuLowPowerStat","CpuPrimary","CpuCoreStepText",\
			"CpuCurrentPerf","CpuMinPerfStat","CpuMaxPerfStat"\
};

/* Network Table name tags for detailed information */
char *net_tbl_ele[] = { "NIPAInd","NIPAIfNum","NIPARole","NIPAMACAddr","NIPASlot","NIPAIoAddr","NIPAIrq","NIPADma","NIPAMemAddr",\
			"NIPAPort","NIPADuplexState","NIPACond","NIPAState","NIPAStatus","NIPAStatsVal","NIPAGoodTrans",\
			"NIPAGoodRec","NIPABadTrans","NIPABadRec","NIPAAlignErr","NIPAFCSErr","NIPASinColFrames",\
			"NIPAMulColFrames","NIPADefTrans","NIPALateCol","NIPAExceCol","NIPAIntMacTrantErr","NIPACarSenseErr","NIPAFraTooLongs",\
			"NIPAIntMacRecErr","NIPAHwLoc","NIPAPartNo","NIPASpeed","NIPAConfSpeedDup","NIPAAggrGID","NIPASpeedMbps",\
			"NIPAInOct","NIPAOutOct","NIPANam","NIPAIoBayNo","NIPAFWVer","NIPAVirtPortNo"\
};
/* SNMP version defintion.  */
enum 
{
  SNMP_V1 = 1,
  SNMP_V2C,
  SNMP_V3_NOPRIV,
  SNMP_V3_PRIV
};

#endif
