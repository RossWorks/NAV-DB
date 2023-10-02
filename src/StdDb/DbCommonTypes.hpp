#ifndef DB_COMMON_TYPES
#define DB_COMMON_TYPES

#include <stdint.h>

#define DB_HEADER_SIZE_IN_BYTES 28
#define VHF_TABLE_ROW_SIZE_IN_BYTES 40
#define APT_TABLE_ROW_SIZE_IN_BYTES 32
#define NDB_TABLE_ROW_SIZE_IN_BYTES 28
#define WPT_TABLE_ROW_SIZE_IN_BYTES 16
#define TABLE_HEADER_SIZE_IN_BYTES 8

const char C_APT_ELEV         = 56;
const char C_APT_IATA_IDENT   = 13;
const char C_APT_ICAO_IDENT   =  6;
const char C_APT_IS_IFR       = 30;
const char C_APT_LONG_NAME    = 93;
const char C_APT_MAGVAR       = 51;
const char C_APT_RWY_LONG     = 27;
const char C_APT_RWY_SURFACE  = 31;
const char C_APT_SUBSECTION   = 12;
const char C_APT_TIMEZONE     = 81;
const char C_APT_USE          = 80;
const char C_BLANK_CHAR       = ' ';
const char C_CONT_INDEX       = 21;
const char C_COUNTRY_CODE     = 19;
const char C_DME_ELEV         = 79;
const char C_DME_LAT          = 55;
const char C_DME_LON          = 64;
const char C_FREQ             = 22;
const char C_ICAO_IDENT       = 13;
const char C_LONG_NAME_LEN    = 30;
const char C_NAVAID_CLASS     = 27;
const char C_NAVAID_LAT       = 32;
const char C_NAVAID_LON       = 41;
const char C_NAVAID_MAGVAR    = 74;
const char C_SECTION_CODE     =  4;
const char C_SUBSECTION_CODE  =  5;
const char C_VHF_FIGURE_MERIT = 84;
const char C_RWY_LATITUDE     = 32;
const char C_RWY_LONGITUDE    = 41;
const char C_RWY_IDENTIFIER   = 13;
const char C_RWY_LENGTH       = 22;
const char C_RWY_WIDTH        = 77;
const char C_RWY_ELEV         = 60;

/*VHF Field positions*/
const char C_ICD_VHF_OBJECT_ID_BYTEPOS    =  0;
const char C_ICD_VHF_COUNTRY_CODE_BYTEPOS =  8;
const char C_ICD_VHF_OBJECT_TYPE_BYTEPOS  = 12;
const char C_ICD_VHF_DME_COL_BYTEPOS      = 13;
const char C_ICD_VHF_LAT_BYTEPOS          = 16;
const char C_ICD_VHF_LON_BYTEPOS          = 20;
const char C_ICD_VHF_MAGVAR_BYTEPOS       = 24;
const char C_ICD_VHF_FREQ_BYTEPOS         = 28;
const char C_ICD_VHF_DME_LAT_BYTEPOS      = 32;
const char C_ICD_VHF_DME_LON_BYTEPOS      = 36;

/*APT Field positions*/
const char C_ICD_APT_OBJECT_ID_BYTEPOS    =  0;
const char C_ICD_APT_COUNTRY_CODE_BYTEPOS =  8;
const char C_ICD_APT_IATA_CODE_BYTEPOS    = 12;
const char C_ICD_APT_MAGVAR_BYTEPOS       = 16;
const char C_ICD_APT_LAT_BYTEPOS          = 20;
const char C_ICD_APT_LON_BYTEPOS          = 24;
const char C_ICD_APT_ELEVATION_BYTEPOS    = 28;

/*NDB Field positions*/
const char C_ICD_NDB_OBJECT_ID_BYTEPOS    =  0;
const char C_ICD_NDB_COUNTRY_CODE_BYTEPOS =  8;
const char C_ICD_NDB_LAT_BYTEPOS          = 12;
const char C_ICD_NDB_LON_BYTEPOS          = 16;
const char C_ICD_NDB_MAGVAR_BYTEPOS       = 20;
const char C_ICD_NDB_FREQ_BYTEPOS         = 24;

/*Waypoints Filed positions*/
const char C_ICD_WPT_OBJECT_ID_BYTEPOS    =  0;
const char C_ICD_WPT_LAT_BYTEPOS          =  8;
const char C_ICD_WPT_LON_BYTEPOS          = 12;

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
  WAYPOINT
} E_NavAidClass;

enum E_LIST_TYPE{
  APT_LIST,
  VHF_LIST,
  NDB_LIST,
  WP_LIST
};

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

enum E_Surf_Type{
  HARD_SURFACE,
  SOFT_SURFACE,
  WATER_SURFACE,
  UNDEF_SURFACE
};

enum E_AptPubMil{
  CIVIL,
  MILITARY,
  JOINT,
  PRIVATE
};

typedef enum DbErrorCode{
  NO_ERROR,
  IO_ERROR,
  OUT_OF_MEMORY,
  RECORD_MALFORMED,
  ALREADY_INITIALIZED,
  DB_NOT_SORTED
} E_DbError;

struct Validated_Float{
  float Value;
  bool Status;
};

struct Validated_Integer{
  uint32_t Value;
  bool Status;
};

struct Validated_Double{
  double Value;
  bool Status;
};

typedef struct Record{
  uint32_t ID;
  std::string ICAO;
  std::string LongName;
  std::string CountryCode;
  std::string DMEident;
  std::string IATA;
  E_NavAidClass Class;
  Validated_Double Lat, Lon;
  Validated_Double DmeLat, DmeLon;
  Validated_Integer Elev, DmeElev;
  Validated_Float MagVar;
  E_LIST_TYPE ListType;
  Validated_Integer Freq;
  uint8_t Channel;
  E_ChannelMode ChMode;
  E_VhfRange VhfRange;
  E_Surf_Type LongRwySurfType;
  uint32_t LongestRWYlength;
  E_AptPubMil AptUsage;
  bool AptIfrCapable;
  int TimeZoneOffset;
  std::string RecommendedNavaid;
  bool DMEisCollocated;
}DbRecord_t;

typedef struct{
  uint32_t ID;
  std::string RwyIdent;
  std::string AssAirport;
  Validated_Float MagBearing, Gradient;
  Validated_Double Lat, Lon;
  E_Surf_Type SurfaceType;
  Validated_Integer Elev, Length, LndZoneElev, DispThresDist;
  Validated_Integer Width;
} RwyRecord_t;

#endif