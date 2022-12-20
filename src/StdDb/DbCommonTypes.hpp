typedef enum NavAidClass{
  UNKNOWN,
  APT,
  DME,
  ILS,
  ILSDME,
  MILTAC,
  MLSDME,
  NDB,
  TACAN,
  VOR,
  VORDME,
  VORTAC,
} E_NavAidClass;

enum E_VhfRange{
  TERMINAL = 0,
  LOW_ALT = 1,
  HI_ALT = 2,
  EXT_HI_ALT = 3,
  VHF_IS_MIL = 7,
  OUT_OF_SERVICE = 9
};

enum E_ChannelMode{
  X,
  Y
};

typedef enum DbErrorCode{
  NO_ERROR,
  IO_ERROR,
  OUT_OF_MEMORY,
  RECORD_MALFORMED
} E_DbError;

const char C_SECTION_CODE     = 4;
const char C_SUBSECTION_CODE  = 5;
const char C_ICAO_IDENT       = 13;
const char C_COUNTRY_CODE     = 19;
const char C_CONT_INDEX       = 21;
const char C_FREQ             = 22;
const char C_NAVAID_CLASS     = 27;
const char C_NAVAID_LAT       = 32;
const char C_NAVAID_LON       = 41;
const char C_APT_MAGVAR       = 51;
const char C_DME_LAT          = 55;
const char C_DME_LON          = 64;
const char C_NAVAID_MAGVAR    = 74;
const char C_DME_ELEV         = 79;
const char C_APT_LONG_NAME    = 93;
const char C_VHF_FIGURE_MERIT = 84;
const char C_BLANK_CHAR       = ' ';
