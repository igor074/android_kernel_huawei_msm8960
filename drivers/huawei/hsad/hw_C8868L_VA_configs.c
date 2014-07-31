/*The file is auto generated by tools, don't modify it manully.*/
#include <hsad/configdata.h>
#include "hwconfig_enum.h"

config_pair  hw_C8868L_VA_configs [] = {
    {"camera/powerseq", (const unsigned int)(unsigned int*)"camera_power_seq_verizion", E_CONFIG_DATA_TYPE_STRING }, 
    {"charger/wireless", (unsigned int)1, E_CONFIG_DATA_TYPE_BOOL }, 
    {"hdmi/msm", (unsigned int)0, E_CONFIG_DATA_TYPE_BOOL }, 
    {"lcd/id0", (const unsigned int)(unsigned int*)"r61408_cmi_amoled_mipi_cmd_wvga", E_CONFIG_DATA_TYPE_STRING }, 
    {"lcd/id10", (const unsigned int)(unsigned int*)"simulator_mipi_video_vga", E_CONFIG_DATA_TYPE_STRING }, 
    {"leds/bl_ctl", (const unsigned int)(unsigned int*)"mpp", E_CONFIG_DATA_TYPE_STRING }, 
    {"lightsensor/apds99xx", (const unsigned int)(unsigned int*)"product_verizion", E_CONFIG_DATA_TYPE_STRING }, 
    {"mhl/sii9244", (unsigned int)0, E_CONFIG_DATA_TYPE_BOOL }, 
    {"nfc/felica", (unsigned int)0, E_CONFIG_DATA_TYPE_BOOL }, 
    {"pm/current_limt", (const unsigned int)(unsigned int*)"LIMIT_700MA", E_CONFIG_DATA_TYPE_STRING }, 
    {"pm/fuel_gauge", (unsigned int)0, E_CONFIG_DATA_TYPE_BOOL }, 
    {"pm/msm_therm", (unsigned int)1, E_CONFIG_DATA_TYPE_BOOL }, 
    {"product/name", (const unsigned int)(unsigned int*)"C8868L_VA", E_CONFIG_DATA_TYPE_STRING }, 
    {"sdcard/TLMM_HDRV_SDC3_CLK/drive_current", (const unsigned int)(unsigned int*)"GPIO_CFG_12MA", E_CONFIG_DATA_TYPE_STRING }, 
    {"sdcard/detect_voltage_level", (const unsigned int)(unsigned int*)"high", E_CONFIG_DATA_TYPE_STRING }, 
    {"sensors/compass", (const unsigned int)(unsigned int*)"top", E_CONFIG_DATA_TYPE_STRING }, 
    {"sensors/gsensor", (const unsigned int)(unsigned int*)"top", E_CONFIG_DATA_TYPE_STRING }, 
    {"sensors/gyro", (const unsigned int)(unsigned int*)"top", E_CONFIG_DATA_TYPE_STRING }, 
    {"touchkey/keyorder", (const unsigned int)(unsigned int*)"MenuBack", E_CONFIG_DATA_TYPE_STRING }, 
    {"tp/resolution", (const unsigned int)(unsigned int*)"WVGA", E_CONFIG_DATA_TYPE_STRING }, 
    {"usbswitch/fsa9485", (unsigned int)0, E_CONFIG_DATA_TYPE_BOOL }, 
    {0, 0, 0}
 };
struct board_id_general_struct config_common_C8868L_VA= {
	.name=COMMON_MODULE_NAME,
	.board_id=BOARD_ID_C8868L_VA,
	.data_array={.config_pair_ptr=hw_C8868L_VA_configs},
	.list={NULL,NULL},
};