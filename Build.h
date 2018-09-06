#define APP_IDENTIFIER  25
#define APP_FW_VERSION  "V2.0"

// we declare the weak method getBuildTime() which returns the compile time. This method can be overloaded externally with the real BuildTime (defined in boards.txt)
extern const __FlashStringHelper* getBuildTime() __attribute__((weak));
const __FlashStringHelper* getBuildTime()
{
  static const char __BuildTime__[] PROGMEM = "Compiled: " __DATE__ " " __TIME__;
  return reinterpret_cast<const __FlashStringHelper *> (&__BuildTime__[0]);
}
