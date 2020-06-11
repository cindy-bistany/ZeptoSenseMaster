#include "zstate.h"

// Install Information Set to 1 to configure the correct particle board
//#define NM01
#define NM02 0
#define NM03 0
#define NM04 0
#define NM05 0
#define NM06 0
#define NM07 0
#define NM08 0
#define NM09 0
#define NM10 0
#define NM11 0
#define NM12 0
#define NM13 0
#define NM14 0
#define NM15 0
#define NM16 0
#define NM17 0
#define NM18 0
#define NM19 0
#define NM20 0
#define MH01 0
#define MH02 0
#define CH01 0
#define CH02 0
#define DEMO1 1
#define DEMO2 0 

#ifdef NM01
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190001W ";
char auth[] = "TW3w4ipRRSzlWn5fLzN0HLyrERrYc1TK"; // Auditorium - Boys #1
String InstallLocation = " Loc = Auditorium - Boys #1  ";
#elif NM02
char auth[] = "oB6u2NFryk87-WKWpitb4f5sJEPVW5Qi";  // Auditorium - Boys #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190002 ";
String InstallLocation = " Loc = Auditorium - Boys #2 ";
#elif NM03
char auth[] = "wtHMksk1Vk-XG9vtrQ06R7Uywjpzn8C3";  // Auditorium - Boys #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190003 ";
String InstallLocation = " Loc = Auditorium - Boys #3 ";
#elif NM04 
char auth[] = "4AXE6GzownAQO6ryqIhh0XxS3Rkwpv77";  // Auditorium - Girls #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190004W ";
String InstallLocation = " Loc = Auditorium - Girls #1 ";
#elif NM05
char auth[] = "JnEYsIRf6uh_tMGvI1lFW6I7YvN-V3II";  // Auditorium - Girls #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190005W ";
String InstallLocation = " Loc = Auditorium - Girls #2 ";
#elif NM06
char auth[] = "sk0SVmeo2aqxjrI2rcXD_Lz86aMl_RGT";  // Auditorium - Girls #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190006W ";
String InstallLocation = " Loc = Auditorium - Girls #3 ";
#elif NM07
char auth[] = "pz6vWIMHyYtMphoQg8xLKqxgP3OGeTWY";  // Cafeteria - Boys #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190007W ";
String InstallLocation = " Loc = Cafeteria - Boys #1 ";
#elif NM08
char auth[] = "uxnWrECLKCmo_W3ybm5qFrQ6Sm1Yp4a3";  // Cafeteria - Boys #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190008W ";
String InstallLocation = " Loc = Cafeteria - Boys #2 ";
#elif NM09
char auth[] = "Rmlj1GhVlS54AUz5QtVZTQto7hwQiFHY";  // Cafeteria - Boys #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190009W ";
String InstallLocation = " Loc = Cafeteria - Boys #3 ";
#elif NM10
char auth[] = "Sc65ul1ZhwP-PXUASHu0f6w6ECESQtYk";  // Cafeteria - Girls #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190010W ";
String InstallLocation = " Loc = Cafeteria - Girls #1 ";
#elif NM11
char auth[] = "4iZj-BzJnEhb3vQdcMMv_UKeRqIu0q1l";  // Cafeteria - Girls #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190011W ";
String InstallLocation = " Loc = Cafeteria - Girls #2 ";
#elif NM12
char auth[] = "HM6P8AQSXlivj7qyx6PoP1XLA3N6kR9P";  // Cafeteria - Girls #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190012W ";
String InstallLocation = " Loc = Cafeteria - Girls #3 ";
#elif NM13
char auth[] = "f5sjoGvVioMghFn2ogo5DgZa9TWm8Wyl";  // 2nd Floor - Boys #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190013W ";
String InstallLocation = " Loc = 2nd Floor - Boys #1 ";
#elif NM14
char auth[] = "9JG4IXAL4YAtKhaeB1gny4ccVOA4Zfag";  // 2nd Floor - Boys #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190014W ";
String InstallLocation = " Loc = 2nd Floor - Boys #2 ";
#elif NM15
char auth[] = "ZBw9G8640LttTvMUMLQBhe5RGlpDCZrl";  // 2nd Floor - Boys #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190015W ";
String InstallLocation = " Loc = 2nd Floor - Boys #3 ";
#elif NM16
char auth[] = "IpJUfUT5y9ddjrKIq6IxX0coDerLSA4W";  // 2nd Floor - Girls #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190016 ";
String InstallLocation = " Loc = 2nd Floor - Girls #1 ";
#elif NM17
char auth[] = "5k-Ybn5b3cvoiT9cpXs1ea97IqEJmEbN";  // 2nd Floor - Girls #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190017 ";
String InstallLocation = " Loc = 2nd Floor - Girls #2 ";
#elif NM18
char auth[] = "ByGW-6MlpEmDyNplY570GWaPjlAa8vfb";  // 2nd Floor - Girls #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190018W ";
String InstallLocation = " Loc = 2nd Floor - Girls #3 ";
#elif NM19
char auth[] = "GFqcx624QTCu4JG6JK_QbV830J5zF6M0";  // Portable #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190019W ";
String InstallLocation = " Loc = Portable #1 ";
#elif NM20
char auth[] = "T2lc5d1JT7T70q-1ukKVnHq0YQ0fOQjo";  // Portable #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190020W ";
String InstallLocation = " Loc = Portable #2 ";
#elif MH01 
char auth[] = "Hzk-5ToU8Za1TUhirSOwewS5DaHsJMzi";  // Melrose #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = MHS_12002200001W ";
String InstallLocation = " Loc = Boys #1 ";
#elif MH02
char auth[] = "JRExJiLEcxwJfXaC3fIhRcWXybpSTGM2";  // Melrose #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = MHS_12002200002W ";
String InstallLocation = " Loc = Girls #1 ";
#elif CH01 
char auth[] = "W8Z09poq6n7wTGKUAi_fzl5Gv8HRFwT9";  // Chelmsford #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = CHS_10002200001 ";
String InstallLocation = " Loc = Sensor #1 ";
#elif CH02
char auth[] = "ydW-C263BLpJPl7U7s8_uyDrX-wlmRmf";  // Chelmsford #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = CHS_10002200002 ";
String InstallLocation = " Loc = Sensor #2 ";
#elif DEMO1
char auth[] = "AU7PDy_yGZjGSV9DwINUIustqPzeIw-b";  // Zeptive Demo #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = ZepDemo_001 ";
String InstallLocation = " Stoneham  ";
#elif DEMO2
char auth[] = "utaIzfAalhOaa76maecfcBefW0SRm26U";  // Zeptive Demo #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = ZepDemo_002 ";
String InstallLocation = " Wilmington ";
#else 
// char auth[] = "xDQv4KE4ZKS7DW8ba4OuwtY-XIXeVvYT"; //Bathroom Test
// char auth[] = "2VbD53DCkxcnlyu28w_KL9TWpdOom1nQ"; //Bathroom 1
//char auth[] = "JouQtmYm-Cs2QCjZA18AxFAvdFrs6Sr6"; //Bathroom 2
//char auth[] = "IODR4bIAgZKqMptIpYHIvpLx3L8qmLfS"; //Bathroom 2 (New)
//char auth[] = "Z8lveVS4uGA9G2iVjKZQKUEwJf5ZWClf "; //Bathroom 2 (Blynk 2 new)
// char auth[] = "2DNcMB9D3Q9IMJAHkHl0NqyCtE326uGm"; //Bathroom 3
// char auth[] = "wjKKdx0kjzS_33wY3C4CCtOJRn8Lfh3D"; //Bathroom 4
char auth[] = "pUSnK9eAbHcgOAi_v1vN1cPYUIID6Iwv"; //Bathroom 5 (Blynk 2 new)
String iSoftwareVersion = " SW Ver = 2019116 S/N = Zep2W ";
String InstallLocation = " Loc = Test Unit ";
#endif

//  char auth[] = "q4ey21hbwczzBHyRj_Z-f7EJieP491Qu"; // Sentient Things Test 2.0
 // I8213EIuNfVpyoW6dITCAy4glOI7s7Fs

