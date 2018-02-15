#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H


#ifndef _WIN32_WINNT 
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#endif

#ifndef BOOST_ASIO_NO_WIN32_LEAN_AND_MEAN
#define BOOST_ASIO_NO_WIN32_LEAN_AND_MEAN
#endif


#include <map>


class SocketImpl;
class Logger;
class IniFile;


class LedController
{

public:
    typedef enum
    {
        LED_CONTINUOUS = 0,
        LED_PULSE =1,
        LED_SWITCHED = 2,
        LED_SELECT = 3
    } LEDCONTOLLER_MODE;

    typedef enum
    {
        PP420 = 1000,
        RT820 = 1001,
        RT420 = 1002,
    } MODEL;


    typedef enum
    {
        CHANNELGROUP1 = 100,
        CHANNELGROUP2 = 101,
        CHANNELGROUP3 = 102,
        CHANNELGROUP4 = 103
    } LEDCONTROLLER_CHANNELGROUP;

    typedef unsigned CHANNEL;
    typedef struct
    {
       // bool    m_bOn;
        float   m_fCurrent;
        unsigned m_uTrigger;
        unsigned m_uPulseBrightness;
        unsigned m_uContinousBrightness;
        double	m_dPulseDelay;
        double	m_dPulseWidth;
        double	m_dRetriggerDelay;
        LEDCONTOLLER_MODE m_Mode;
    } struct_LVT_CHANNEL_PARAMS;

    LedController();
    ~LedController();
    bool initialize(std::string sIniFile = "LedController.ini", unsigned channels = 8, bool bReadFromDevice = true);//std::string sIp = "16.0.0.10", std::string sPort = "30313");
    bool initChannel(CHANNEL channel, struct_LVT_CHANNEL_PARAMS params);

    //bool DecreaseBrightness(LEDCONTROLLER_CHANNELGROUP, unsigned uStep, unsigned uMinimu);
  //
    void setAllChannelsPulsed();
    void setAllChannelsContinuous();



    bool setPulsedBrigthnessAbsolute(unsigned uChannel,unsigned uNewValue);
    bool setContinuousBrigthnessAbsolute(CHANNEL channel,unsigned uNewValue);
    bool setTrigger(CHANNEL channel, unsigned uNewValue);
    bool setPulseWidth(CHANNEL channel, double dNewValue);
    bool setPulseDelay(CHANNEL channel, double dNewValue);
    bool setCurrent(CHANNEL channel, double dNewValue);
    bool setMode(CHANNEL channel, LEDCONTOLLER_MODE newMode);
    bool setRetriggerDelay(CHANNEL channel, double dNewValue);


    double getChannelContinuousBrightness(CHANNEL uChannel);
    double getChannelPulsedBrightness(CHANNEL uChannel);

    double getChannelCurrent(CHANNEL uChannel);
    LEDCONTOLLER_MODE getMode(CHANNEL uChannel);

    unsigned getTrigger(CHANNEL uChannel);
    double getPulseWidth(CHANNEL uChannel);
    double getPulseDelay(CHANNEL uChannel);
    double getRetriggerDelay(CHANNEL uChannel);
    unsigned getChannelCount()
    {
        return m_uChannelCount;
    }

    bool readCurrentValuesFromDevice();
    bool readAndSetValuesFromIni(std::string sFile);
    bool setCurrentValueSetToDevice();
    bool saveCurrentValuesToFile(std::string sFile);

    bool resetDeviceErrors();

    void closeConnection();
    void test(std::string);




private: 
	//boost::asio::io_service m_IoService;
	//boost::asio::ip::tcp::socket m_Socket;
	SocketImpl	*m_SocketImpl;
	Logger	*m_Logger;
	IniFile	*m_IniFile;
    std::string  m_sIniFile;

	unsigned m_uChannelCount; 
    MODEL model;

    std::map<CHANNEL,struct_LVT_CHANNEL_PARAMS> m_Channels;

   
    void SetTriggersOff();
    void SetTriggersOn();
    
    //void SetPulsedLEDs();
    void SetPulsedBrightness();
    void SetContinuousBrightness();

    std::string SendAndGetReply(const char*);
	
    std::pair<int,int> findFirstDigitInString(std::string);
    std::pair<int,int> findCmdInString(std::string);

   // bool ReadInitValues();
	


};

#endif // LEDCONTROLLER_H

/*
CH  Channel number 
MD  Mode: 0 = continuous, 1 = pulse, 2 = switched, 3 = select 
S  Brightness percentage settings: 
1
st
 setting used in all modes 
2
nd
 setting only used for select mode 
DL  Pulse delay 
PU  Pulse width 
RT  Retrigger delay 
IP  Input Trigger (set using the RP command) 
FL  Flags (set using the RE command) 
CS  Rating of the light (after SafeSense has successfully completed sensing the light)
RA  Rating of the light (set using VL command) 
*/
