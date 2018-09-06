#include <EEPROM.h>

struct Settings{
  const static uint8_t default_applicationType = APP_IDENTIFIER;
  const static uint8_t default_slaveId         = '+';
  const static uint8_t default_priority        = 0x1f;
  const static uint8_t eeprom_offset = 0;

  uint8_t ApplicationType;
  uint8_t SlaveId;
  uint8_t Priority;

  Settings(){
    load();  
  }

public:
  void load(){
    EEPROM.get(eeprom_offset, *this);
    if(ApplicationType != default_applicationType){
      reset();
    }
  }

  void reset(){
    SlaveId = default_slaveId;
    Priority = default_priority;
  }

  void save(){
    return EEPROM.put(eeprom_offset, *this);
  }
};

template<class reader_t, unsigned char led = 0>
class Application: public Settings // ==========================================================================================================================
{
  public:
  char myHeader[5];
  char* myAddress   = &myHeader[2];
  reader_t &reader;
  

  Application(reader_t &r): reader(r){
    strcpy(myHeader, "M*S*");
  }
  
  inline bool isVerboseLevel(int8_t test){
    //TODO
    return true; 
  }

  inline void setVerboseLevel(int8_t value){
    //TODO
  }

  inline int8_t getVerboseLevel(){
    return -1;
  }

  inline void setRS485_SlaveId(char theSlaveId)
  {
      SlaveId = theSlaveId;
      myAddress[1] = SlaveId;
  }
  
  inline void setRS485_Priority(unsigned char thePriority)    { Priority=thePriority <= 31 ?  thePriority : 31; }

  inline void startMessage(unsigned char aPriority=0xff)
  {
    bus.startTransaction(aPriority!=0xff ? aPriority:  Priority);
    bus.print((char) readerClass::SOM);
    bus.print(myHeader);
  }
  inline unsigned char endMessage()
  {
    bus.print((char) readerClass::EOM);
    bus.println();
    return bus.endTransaction();
  }

  void printAliveMessage(Print& aPrinter)
  {
      if(&aPrinter==&bus) startMessage();
      aPrinter.print(F("WellControll alive. Version "));  aPrinter.print(F(APP_FW_VERSION));
      aPrinter.print(F("("));                             aPrinter.print( getBuildTime() );
      aPrinter.print(F(") my address="));                 aPrinter.print(myAddress);
      aPrinter.print(F(" RS485_priority="));              aPrinter.print(Priority);
      if(&aPrinter==&bus) endMessage();
  }

  void communicate(){
    Print& thePrinter = bus;
    unsigned char state = reader.getState();
    if( reader.isMessage(state) )
    {
        char* messageParser      = reader.getMessage();
        char* messageDestination = reader.getMessageOrigin();
    //      char  messageLength      = reader.getMessageLength();
     
        // convenience macros for readability and to avoid typing errors, using progmem for test strings //TODO, consider to include in MessageReader Class??
        #define startWith( _message_,_string_)  (!strncmp_P((_message_),PSTR(_string_), sizeof((_string_))-1) )
        #define canRemove( _message_,_string_)  (!strncmp_P((_message_),PSTR(_string_), sizeof((_string_))-1) ? (_message_ += sizeof((_string_))-1, true) : false)
    
    
        if(false); // lekker puh
        else if(*messageParser=='\0')                      { startMessage(); endMessage(); }
        else if(startWith(messageParser,"?"))              { printAliveMessage(thePrinter); }
        else if(canRemove(messageParser,"SLID="))          { setRS485_SlaveId(messageParser[0]); }
        else if(canRemove(messageParser,"PRIO="))          { setRS485_Priority(atoi(messageParser)); }
        else if(canRemove(messageParser,"EEPROM."))
        {    if(false) {} //lekker puh
             else if(startWith(messageParser,"SAVE"))      { save(); }
             else if(startWith(messageParser,"RESTORE"))   { load(); }
        }
        else if(canRemove(messageParser,"Echo:"))          { startMessage(); thePrinter.print(messageParser); endMessage(); }
        else if(canRemove(messageParser,"VBL="))           { setVerboseLevel(atoi(messageParser)); }
        else if(startWith(messageParser,"VBL?"))           { startMessage(); thePrinter.print(F("VerboseLevel=0x")); thePrinter.println( getVerboseLevel(),HEX); endMessage(); }
        else
        {
            if( isVerboseLevel(-1) ) { thePrinter.print(myHeader); thePrinter.print(F("_?:")); thePrinter.println(messageParser); }
        }
        reader.nextMessage();
    
        return; // so we can read next message if needed
    }
  }
};
