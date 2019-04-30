#ifndef ErrorTypeH
#define ErrorTypeH
//-----------------------------------------------------------------------------
enum {
  ERROR_COMM_PORT = 0x01, ERROR_USER_OP, ERROR_SD_CARD, ERROR_CARD_SYSTEM,
  ERROR_INI_FILE, ERROR_FLASH, ERROR_SD_CMD, ERROR_FILE, ERROR_QC_TEST,
  ERROR_FILE_COPY, ERROR_CPRM, ERROR_USB_PORT,
	ERROR_SD_VENDOR_MODE, ERROR_USB, ERROR_MEMORY, ERROR_CONTROLLER_TEST,
	ERROR_DRIVE_VOLUMN, ERROR_READ_CARD_SYS, ERROR_READ_SMART
};
//-----------------------------------------------------------------------------
#endif
