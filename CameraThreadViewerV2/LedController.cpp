#include "LedController.h"

#ifndef Q_MOC_RUN
#include <boost/asio.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#endif


#include <regex>
#include "..\CPPCommon\StringBuilder.h"
#include "Logger.h"
#include "Inifile.h"


struct time_t_traits
{
  // The time type.
  typedef std::time_t time_type;

  // The duration type.
  struct duration_type
  {
    duration_type() : value(0) {}
    duration_type(std::time_t v) : value(v) {}
    std::time_t value;
  };

  // Get the current time.
  static time_type now()
  {
    return std::time(0);
  }

  // Add a duration to a time.
  static time_type add(const time_type& t, const duration_type& d)
  {
    return t + d.value;
  }

  // Subtract one time from another.
  static duration_type subtract(const time_type& t1, const time_type& t2)
  {
    return duration_type(t1 - t2);
  }

  // Test whether one time is less than another.
  static bool less_than(const time_type& t1, const time_type& t2)
  {
    return t1 < t2;
  }

  // Convert to POSIX duration type.
  static boost::posix_time::time_duration to_posix_duration(
      const duration_type& d)
  {
    return boost::posix_time::seconds(d.value);
  }
};
typedef boost::asio::basic_deadline_timer<
    std::time_t, time_t_traits> time_t_timer;

class SocketImpl
{	
public:
		boost::asio::io_service m_IoService;
		//boost::asio::ip::tcp::socket m_Socket;
		boost::scoped_ptr<boost::asio::ip::tcp::socket> m_Socket;
		//time_t_timer m_timer;

		SocketImpl() //: m_Socket(m_IoService)//,m_timer(m_IoService)
		{		
			m_Socket.reset(new boost::asio::ip::tcp::socket(m_IoService));			
		};
		~SocketImpl()
		{			
			
			
		};
		
};



LedController::LedController() //: m_Socket(m_IoService)
{

	m_SocketImpl = 0;	
}


LedController::~LedController()
{	
	if (0 != m_SocketImpl)
	{
		//m_SocketImpl->m_Socket->close();
		delete m_SocketImpl;
	}
    m_Channels.clear();
}

bool LedController::initialize(std::string sIniFile,unsigned channels, bool bReadFromDevice)//,std::string sId, std::string sPort)
{
	m_sIniFile = sIniFile;
	char szMsg[32];
	using boost::asio::ip::tcp;

	//if (NULL == m_Logger)
		m_Logger = new Logger("log_LedController");
	
	m_IniFile = new IniFile( m_sIniFile.c_str());
    std::string sId = m_IniFile->GetSetValue("Controller","IP", "16.0.0.10","");
    std::string sPort = m_IniFile->GetSetValue("Controller","Port","30313","");

    if (m_IniFile->IsDirty())
        m_IniFile->Save();

	m_Logger->Log(StringBuilder() <<"Began to initialize LedController " << sId << ":" << sPort);
	m_Logger->Log(StringBuilder() <<"Initial logfile " << m_IniFile->getCurrentFileName());

	// scoped_ptr...
	
	if (0 != m_SocketImpl)
	{
	//	m_SocketImpl->m_Socket.close();
		delete m_SocketImpl;
	}
	
		
	m_SocketImpl = new SocketImpl;

	 m_Logger->Log(StringBuilder() << "Created socket");
	

    tcp::resolver resolver(m_SocketImpl->m_IoService);    
	tcp::resolver::query query((const char*)sId.c_str(),(char*)sPort.c_str());
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

	
	
	
    boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end)
    {			
		m_SocketImpl->m_Socket->connect(*endpoint_iterator++, error);
    }
    if (error)
	{		
      m_Logger->Log(StringBuilder() << "Error opening socket");
	  return false;
	}
	if (m_SocketImpl->m_Socket->is_open())
        m_Logger->Log(StringBuilder() <<  "Socket opened succesfully ");



    m_Logger->Log(StringBuilder() << "Device version: "  << SendAndGetReply("VR"));
    m_uChannelCount = m_IniFile->GetSetValue("Controller", "ChannelCount", channels,"Channel count");


	for (int i = 1; i <  m_uChannelCount + 1 ; i++)
    {
        struct_LVT_CHANNEL_PARAMS params;
		m_Channels[i] = params;
	}
//
    if (bReadFromDevice)
        this->readCurrentValuesFromDevice();
    return true;


}

void LedController::SetTriggersOff()
{  
    for (int i = 0; i < m_Channels.size(); i++)
    {
        if (i+i > 4)
            m_Channels[i+1].m_uTrigger = 2;
        else
            m_Channels[i+1].m_uTrigger = 2;

    }

}

void LedController::SetTriggersOn()
{       
    m_Channels[1].m_uTrigger = 1;
    m_Channels[2].m_uTrigger = 1;
    m_Channels[3].m_uTrigger = 1;
    m_Channels[4].m_uTrigger = 1;
    m_Channels[5].m_uTrigger = 5;
    m_Channels[6].m_uTrigger = 5;
    m_Channels[7].m_uTrigger = 5;
    m_Channels[8].m_uTrigger = 5;

}

void LedController::setAllChannelsContinuous()
{
	/*
    m_Channels[1].m_bOn = false;
    m_Channels[2].m_bOn = false;
    m_Channels[3].m_bOn = false;
    m_Channels[4].m_bOn = false;
    m_Channels[5].m_bOn = false;
    m_Channels[6].m_bOn = false;
    m_Channels[7].m_bOn = true;
    m_Channels[8].m_bOn = true;

	m_Channels[1].m_Mode = LED_PULSE;
	m_Channels[2].m_Mode = LED_PULSE;
	m_Channels[3].m_Mode = LED_PULSE;
	m_Channels[4].m_Mode = LED_PULSE;
	m_Channels[5].m_Mode = LED_PULSE;
	m_Channels[6].m_Mode = LED_PULSE;
	m_Channels[7].m_Mode = LED_CONTINUOUS;
	m_Channels[8].m_Mode = LED_CONTINUOUS;
*/
    for (int i = 0; i < m_Channels.size(); i++)
        m_Channels[i+1].m_Mode = LED_CONTINUOUS;

	std::map<CHANNEL,struct_LVT_CHANNEL_PARAMS>::iterator it;
	
	for (it = m_Channels.begin(); it != m_Channels.end(); it++)
	{
		if (LED_CONTINUOUS == it->second.m_Mode)
			setContinuousBrigthnessAbsolute(it->first,5);
		setMode(it->first,it->second.m_Mode);

	}
	
}

void LedController::setAllChannelsPulsed()
{
	/*
    m_Channels[1].m_bOn = true;	
    m_Channels[2].m_bOn = true;
    m_Channels[3].m_bOn = true;
    m_Channels[4].m_bOn = true;
    m_Channels[5].m_bOn = true;
    m_Channels[6].m_bOn = true;
    m_Channels[7].m_bOn = true;
    m_Channels[8].m_bOn = true;

	m_Channels[1].m_Mode = LED_PULSE;
	m_Channels[2].m_Mode = LED_PULSE;
	m_Channels[3].m_Mode = LED_PULSE;
	m_Channels[4].m_Mode = LED_PULSE;
	m_Channels[5].m_Mode = LED_PULSE;
	m_Channels[6].m_Mode = LED_PULSE;
	m_Channels[7].m_Mode = LED_PULSE;
	m_Channels[8].m_Mode = LED_PULSE;
    */

    for (int i =§ 0; i < m_Channels.size(); i++)
        m_Channels[i+1].m_Mode = LED_PULSE;

	std::map<CHANNEL,struct_LVT_CHANNEL_PARAMS>::iterator it;
	
	for (it = m_Channels.begin(); it != m_Channels.end(); it++)
	{
		setMode(it->first,it->second.m_Mode);
	}
	
}


bool LedController::resetDeviceErrors()
{
	char szMsg[32];
    strcpy(szMsg,"GR>");
            
    
    if (0 == SendAndGetReply(szMsg).length())
        return false;
	    
    return true;
}



std::string LedController::SendAndGetReply(const char* szMsg)
{
    using namespace std;
    m_Logger->Log(StringBuilder() << "Write: " << szMsg);
	
	try
	{
		boost::system::error_code ignored_error;
		boost::asio::write(*m_SocketImpl->m_Socket, boost::asio::buffer(string(szMsg)),
			  boost::asio::transfer_all(), ignored_error);
	
		
		boost::array<char, 128> buf;
		boost::system::error_code error;

		size_t len = m_SocketImpl->m_Socket->read_some(boost::asio::buffer(buf), error);
		
		if (error)
			throw boost::system::system_error(error); // Some other error.

		
        //m_Logger->Log(StringBuilder() <<"Read: " << buf.data());

		// error handling
		/*unsigned uStart = string(buf.data()).find_last_of(">");
		string sReply = string(buf.data()).substr(uStart,string(buf.data()).length() - uStart);

		if (sReply.substr(0,3) == "Err")
			m_Logger->Log(StringBuilder() <<"LED: ERROR FROM CONTROLLER: " << buf.data());
	*/
			
		
        unsigned uEnd = (std::string(buf.data()).find_last_of(">"));
        std::string ret = std::string(buf.data(), uEnd);
        if (boost::starts_with(ret,"Err"))
            m_Logger->Log(StringBuilder() << "ERROR: " << ret);
        return ret;
		
	}
	catch (std::exception& e)
	{
        //std::cerr <<"ERROR: "  <<e.what() << std::endl;
        m_Logger->Log(StringBuilder() << "ERROR: " << e.what());
		return "";
	}

    return "";
}


bool LedController::setPulsedBrigthnessAbsolute(unsigned uChannel,unsigned uNewValue)
{    
    char szMsg[32];
	double dPulseWidth = (double)m_Channels[uChannel].m_dPulseWidth/1000;
	if (dPulseWidth > 0.01)
		dPulseWidth = 0.002;

    sprintf(szMsg,"RT%d,%.3f,%.3f,%d,%.3f>",
	//sprintf(szMsg,"RT%d,0.002,%.3f,%d,%.3f>",
            uChannel,
			dPulseWidth,
            m_Channels[uChannel].m_dPulseDelay/1000,
            uNewValue
			,m_Channels[uChannel].m_dRetriggerDelay
            );
    
    if (0 == SendAndGetReply(szMsg).length())
        return false;
	m_Channels[uChannel].m_uPulseBrightness  = uNewValue;
    //cout << "Set pulsed brightness to " << m_Channels[uChannel].m_uPulseBrightness << " % " << endl;
    return true;
}

bool LedController::setContinuousBrigthnessAbsolute(CHANNEL channel,unsigned uNewValue)
{
	 char szMsg[32];
    sprintf(szMsg,"RS%d,%d>",
            channel,
			uNewValue
            );
    
    if (0 == SendAndGetReply(szMsg).length())
        return false;
	m_Channels[channel].m_uContinousBrightness  = uNewValue;
	m_Logger->Log(StringBuilder() << "Set continuous brightness to " << m_Channels[channel].m_uContinousBrightness << " % ");
    return true;
}


bool LedController::setTrigger(CHANNEL channel, unsigned uNewValue)
{
	char szMsg[32];
    sprintf(szMsg,"RP%d,%d>",
            channel,
            uNewValue
            );
    
    
    if (0 == SendAndGetReply(szMsg).length())
        return false;
	//cout << "Set channel # << " << channel << " trigger to " << uNewValue << endl;
	m_Channels[channel].m_uTrigger  = uNewValue;
	return true;
}

bool LedController::setPulseWidth(CHANNEL channel, double dNewValue)
{
	char szMsg[32];	

	double dPulseWidth = (double)dNewValue/1000;
	if (dPulseWidth > 0.01)
		dPulseWidth = 0.002;

	if (LED_PULSE == m_Channels[channel].m_Mode)
	{
        sprintf(szMsg,"RT%d,%.3f,%.3f,%d,%.3f>",
        //sprintf(szMsg,"RT%d,0.002,%.3f,%d,%.3f>",
					channel,
                    dPulseWidth,
					m_Channels[channel].m_dPulseDelay/1000,
					m_Channels[channel].m_uPulseBrightness  
					,m_Channels[channel].m_dRetriggerDelay
					);
	}
	if (LED_CONTINUOUS == m_Channels[channel].m_Mode)
	{
        sprintf(szMsg,"RT%d,%.3f,%.3f,%d,%.3f>",
        //sprintf(szMsg,"RT%d,0.002,%.3f,%d,%.3f>",
					channel,
                    dPulseWidth,
					m_Channels[channel].m_dPulseDelay/1000,
					m_Channels[channel].m_uContinousBrightness
					,m_Channels[channel].m_dRetriggerDelay
					);
	}
    
	
    if (0 == SendAndGetReply(szMsg).length())
        return false;
	//cout << "Set channel # << " << channel << " pulse width to " << dNewValue << endl;
	m_Channels[channel].m_dPulseWidth  = dNewValue;
	return true;
}
bool LedController::setPulseDelay(CHANNEL channel, double dNewValue)
{
	if (LED_PULSE !=  m_Channels[channel].m_Mode && LED_CONTINUOUS !=  m_Channels[channel].m_Mode)
		return false;

	char szMsg[32];	
	double dPulseWidth = (double)m_Channels[channel].m_dPulseWidth/1000;
	if (dPulseWidth > 0.01)
		dPulseWidth = 0.002;

	if (LED_PULSE == m_Channels[channel].m_Mode)
	{
        //sprintf(szMsg,"RT%d,%.3f,%.3f,%d,%.3f>",
        sprintf(szMsg,"RT%d,0.002,%.3f,%d,%.3f>",
					channel,
                //	dPulseWidth,
					dNewValue/1000,
					m_Channels[channel].m_uPulseBrightness
					,m_Channels[channel].m_dRetriggerDelay
					);
	}
	if (LED_CONTINUOUS == m_Channels[channel].m_Mode)
	{
        //sprintf(szMsg,"RT%d,%.3f,%.3f,%d,%.3f>",
        sprintf(szMsg,"RT%d,0.002,%.3f,%d,%.3f>",
					channel,
                //	dPulseWidth,
					dNewValue/1000,
					m_Channels[channel].m_uContinousBrightness
					,m_Channels[channel].m_dRetriggerDelay
					);
	}
    
    
    if (0 == SendAndGetReply(szMsg).length())
        return false;
	//cout << "Set channel # << " << channel << " pulse delay to " << dNewValue << endl;
	m_Channels[channel].m_dPulseDelay  = dNewValue;
	return true;
	
}

bool LedController::setRetriggerDelay(CHANNEL channel, double dNewValue)
{
	if (LED_PULSE !=  m_Channels[channel].m_Mode && LED_CONTINUOUS !=  m_Channels[channel].m_Mode)
		return false;

	char szMsg[32];	
	double dPulseWidth = (double)m_Channels[channel].m_dPulseWidth/1000;
	if (dPulseWidth > 0.01)
		dPulseWidth = 0.002;

	if (LED_PULSE == m_Channels[channel].m_Mode)
	{
        //sprintf(szMsg,"RT%d,%.3f,%.3f,%d,%.3f>",
        sprintf(szMsg,"RT%d,0.002,%.3f,%d,%.3f>",
					channel,
            //		dPulseWidth,
					m_Channels[channel].m_dPulseDelay/1000,
					m_Channels[channel].m_uPulseBrightness,
					dNewValue
					);
	}
	if (LED_CONTINUOUS == m_Channels[channel].m_Mode)
	{
        //sprintf(szMsg,"RT%d,%.3f,%.3f,%d,%.3f>",//,%.0f>",
        sprintf(szMsg,"RT%d,0.002,%.3f,%d,%.3f>",
					channel,
                //	dPulseWidth,
					m_Channels[channel].m_dPulseDelay/1000,
					m_Channels[channel].m_uContinousBrightness,
					dNewValue
					);
	}
    
    
    if (0 == SendAndGetReply(szMsg).length())
        return false;
	//cout << "Set channel # << " << channel << " pulse delay to " << dNewValue << endl;
	m_Channels[channel].m_dPulseDelay  = dNewValue;
	return true;
}

bool LedController::setCurrent(CHANNEL channel, double dNewValue)
{	
	
	char szMsg[32];
	sprintf(szMsg,"VL%d,0,%.2f>",channel, dNewValue);
    
    
    if (0 == SendAndGetReply(szMsg).length())
        return false;
	//cout << "Set channel # << " << channel << " current to " << dNewValue << endl;
	m_Channels[channel].m_fCurrent  = dNewValue;
	return true;	
}
bool LedController::setMode(CHANNEL channel, LEDCONTOLLER_MODE newMode)
{
	char szMsg[32];
	
    
    if (newMode == LED_CONTINUOUS)
	{
		setContinuousBrigthnessAbsolute(channel, m_Channels[channel].m_uContinousBrightness);		
	}
	if (newMode == LED_PULSE)
	{
		setPulsedBrigthnessAbsolute(channel, m_Channels[channel].m_uPulseBrightness);
	}
	if (newMode == LED_SWITCHED)
	{
	}
	if (newMode == LED_SELECT)
	{
	}
		
    if (0 == SendAndGetReply(szMsg).length())
        return false;
   // cout << "Set channel # << " << channel << " mode to " << newMode << endl;
	m_Channels[channel].m_Mode = newMode;
	return true;	
}



double LedController::getChannelContinuousBrightness(CHANNEL uChannel)
{
	return (double)m_Channels[uChannel].m_uContinousBrightness;
}

double LedController::getChannelPulsedBrightness(CHANNEL uChannel)
{
	return (double)m_Channels[uChannel].m_uPulseBrightness;
}

double  LedController::getChannelCurrent(CHANNEL uChannel)
{
	return (double)m_Channels[uChannel].m_fCurrent;
}

LedController::LEDCONTOLLER_MODE LedController::getMode(CHANNEL uChannel)
{
	//
	return m_Channels[uChannel].m_Mode;
}


unsigned LedController::getTrigger(CHANNEL uChannel)
{
	return m_Channels[uChannel].m_uTrigger;
}


double LedController::getPulseWidth(CHANNEL uChannel)
{
	return m_Channels[uChannel].m_dPulseWidth;
}

double LedController::getPulseDelay(CHANNEL uChannel)
{
	return m_Channels[uChannel].m_dPulseDelay;
}

double LedController::getRetriggerDelay(CHANNEL uChannel)
{
	return m_Channels[uChannel].m_dRetriggerDelay;
}


// PP420 CH 1, MD 1, CS 1.000A, SE 310.0, DL   2.0us, PU   3.5us, PW 0, ST 2, EV 11.6V, IP 1, FL 0
// RT820 CH1,MD1,S265.0,  0.0,DL  2.0us,PU  2.0us,RT10.00ms,IP1,FL1,CS1.000A,RA1.000A
/*
bool LedController::readCurrentValuesFromDevice()
{
    using namespace std;
    char szMsg[32];
    sprintf(szMsg,"VR");
    std::string sRep = SendAndGetReply(szMsg);
    cout << "Firmware version " << sRep << endl;
    if (sRep.length() > 7)
    {
        if (sRep.substr(2,5) == "PP420")
        {
            cout << "This is a 4-channel PP420 " <<  endl;
            model = PP420;
        }
        else
        {
            cout << "This is most probably a 8-channel RT820 "  << sRep.substr(2,5) << endl;
            model = RT820;
        }
    }

    std::map<CHANNEL,struct_LVT_CHANNEL_PARAMS>::iterator it;
    for (it = m_Channels.begin(); it != m_Channels.end(); it++)
    {
        sprintf(szMsg,"ST%d>",it->first);
        sRep = SendAndGetReply(szMsg);
        //cout << sRep << endl;
        boost::char_separator<char> sep(",");
        boost::tokenizer<boost::char_separator<char>> tokens(sRep, sep);
        BOOST_FOREACH(string t, tokens)
            cout << "\t" << t << endl;
    }
    return true;
}
*/

std::pair<int, int> LedController::findFirstDigitInString(std::string s)
{
    std::pair<int,int> ret = std::make_pair(-1,-1);
    std::regex integer("(\\+|-)?[[:digit:]]+");
    for (int i = 0; i < s.length();i++)
    {
        if (std::regex_match(s.substr(i,1),integer))
        {
            if (ret.first == -1)
                ret.first = i;
            ret.second = i;
        }
    }

    if (ret.second == ret.first)
        ret.second = 1;
    else
        ret.second = ret.second - ret.first; // pituus
    return ret;
}

std::pair<int, int> LedController::findCmdInString(std::string s)
{
    std::pair<int,int> ret = std::make_pair(-1,-1);
    std::regex letter("[A-Za-z]");
    for (int i = 0; i < s.length();i++)
    {
        if (std::regex_match(s.substr(i,1),letter))
        {

            if (ret.first == -1)
                ret.first = i;
            ret.second = i;
        }
        if (ret.first > -1 && s.substr(i,1) == " ")
            break;
    }

    if (ret.second == ret.first)
        ret.second = 1;
    else
        ret.second = ret.second - ret.first + 1; // pituus
    return ret;

}

bool LedController::readCurrentValuesFromDevice()
{	
    using namespace std;
	char szMsg[32];

    sprintf(szMsg,"VR");
    std::string sRep = SendAndGetReply(szMsg);
    //cout << "Firmware version " << sRep << endl;
    if (sRep.length() > 7)
    {
        if (sRep.substr(2,5) == "PP420")
        {
           // cout << "This is a 4-channel PP420 " <<  endl;
            model = PP420;
        }
        if (sRep.substr(2,5) == "RT820")
        {
            //cout << "This is most probably a 8-channel RT820 "  << sRep.substr(2,5) << endl;
            model = RT820;
        }
        if (sRep.substr(2,5) == "RT420")
        {
            //cout << "This is most probably a 4-channel RT420 "  << sRep.substr(2,5) << endl;
            model = RT420;
        }
    }


    std::map<CHANNEL,struct_LVT_CHANNEL_PARAMS>::iterator it;
    for (it = m_Channels.begin(); it != m_Channels.end(); it++)
    {
		sprintf(szMsg,"ST%d>",it->first);
        sRep = SendAndGetReply(szMsg);
		m_Logger->Log(StringBuilder() << sRep);		
		if (sRep.length() > 10)
		{			
			unsigned uDelim = sRep.find_first_of(">");
			sRep = sRep.substr(uDelim+1,sRep.length());
			 boost::char_separator<char> sep(",");
			 boost::tokenizer<boost::char_separator<char>> tokens(sRep, sep);			 
			 BOOST_FOREACH(string t, tokens)
             {

				 bool bOk = false;
				 char szKey[16];
				 memset(szKey,0,16);
				 double dValue = 0;
				 char szUnit[16];
				 memset(szUnit,0,16);

                 std::pair<int, int> poss = findFirstDigitInString(t);
                 std::pair<int, int> cmdPoss = findCmdInString(t);

                 if (-1 != cmdPoss.first && -1 != cmdPoss.second)
                 {
                     if ("CH" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         if ( it->first   ==   atoi(t.substr(poss.first,poss.second).c_str()))
                         {
                             m_Logger->Log (StringBuilder() << it->first   << " " <<  atoi(t.substr(poss.first,poss.second).c_str()));
                             bOk = true;
                         }
                         else
                         {
                            // cout << "Failed to read channels, exit" << endl;
                             m_Logger->Log(StringBuilder() << "Failed to read parameters from device");
                             return false;
                         }
                     }


                     if ("S" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"S");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"%");
                         m_Channels[it->first].m_uContinousBrightness = 1; // vakisella varmuuden vuoksi
                         m_Channels[it->first].m_uPulseBrightness = dValue;
                     }

                     if ("SE" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"S");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"%");
                         m_Channels[it->first].m_uContinousBrightness = 1; // vakisella varmuuden vuoksi
                         m_Channels[it->first].m_uPulseBrightness = dValue;
                     }

                     if ("RA" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"RA");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"A");
                     }
                     if ("RT" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"RT");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"us");
                         m_Channels[it->first].m_dRetriggerDelay = dValue;
                     }
                     if ("CS" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"CS");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"A");
                         m_Channels[it->first].m_fCurrent = dValue;
                     }
                     if ("DL" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                          strcpy(szKey,"DL");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"us");
                          if (t.substr(t.length() -2 , 2) == "ms")
                         {
                             m_Logger->Log(StringBuilder()<< "Pulse delay in ms, 1000x");
                             dValue = dValue*1000;
                         }
                         m_Channels[it->first].m_dPulseDelay = dValue;

                     }

                     if ("PU" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"PU");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"us");
                         if (t.substr(t.length() -2 , 2) == "ms")
                         {

                             m_Logger->Log(StringBuilder()<< "Pulse width in ms, 1000x");
                             dValue = dValue*1000;
                         }
                         m_Channels[it->first].m_dPulseWidth = dValue;

                     }

                     if ("IP" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"IP");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"Trigger");
                         m_Channels[it->first].m_uTrigger = dValue;
                     }
                     if ("MD" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"MD");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"MODE");
                         switch ((unsigned)dValue)
                         {
                            case 0: m_Channels[it->first].m_Mode = LED_CONTINUOUS ; break; // continuous
                            case 1: m_Channels[it->first].m_Mode = LED_PULSE ; break;
                            case 2: m_Channels[it->first].m_Mode = LED_SWITCHED; break;
                            case 3:  m_Channels[it->first].m_Mode = LED_SELECT; break;
                            case 4: break;
                         }
                     }
                     if ("FL" == t.substr(cmdPoss.first,cmdPoss.second))
                     {
                         strcpy(szKey,"FL");
                         sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                         strcpy(szUnit,"FLAGS");
                     }


                     //printf("Key: %s, Value: %.2f, Unit: %s",szKey,fValue,szUnit);
                     if (strlen(szKey)> 0)
                        m_Logger->Log(StringBuilder() << "Key: " << szKey << ", Value: " << dValue << ", Unit: " << szUnit);
                 }
			 }
		}
		else 
			return false;
    }
    return true;
}


bool LedController::saveCurrentValuesToFile(std::string sFile)
{
	m_IniFile->SetFilename(sFile.c_str());

	std::map<CHANNEL,struct_LVT_CHANNEL_PARAMS>::iterator it;
	for (it = m_Channels.begin(); it != m_Channels.end(); it++)
	{
		CHANNEL i= it->first;		
		struct_LVT_CHANNEL_PARAMS params = it->second;
		
		m_IniFile->SetValue("Current",
            StringBuilder() << "Channel_#" << i << "_Current",
			params.m_fCurrent            
            );
		
        m_IniFile->SetValue(
            "ContinuousBrighntess",
            StringBuilder() << "Channel_#" << i << "_ContinuousBrighntess",
            params.m_uContinousBrightness          
            );

        m_IniFile->SetValue(
            "PulsedBrighntess",
            StringBuilder() << "Channel_#" << i << "_CPulsedBrighntess",
			params.m_uPulseBrightness            
            );

	
		
		m_IniFile->SetValue(
			"Trigger",
			StringBuilder() << "Channel_#" << i << "_Trigger",
			params.m_uTrigger				
		);

	
		 m_IniFile->SetValue(
            "Common",
            "PulseWidth",
            params.m_dPulseWidth        
            );
		
		 m_IniFile->SetValue(
            "Common",
            "PulseDelay",
            params.m_dPulseDelay            
            );
		 m_IniFile->SetValue(
            "Common",
            "RetriggerDelay",
			params.m_dRetriggerDelay
            );
		 
	}
	m_IniFile->Save();
    return true;
}

bool LedController::readAndSetValuesFromIni(std::string sFile)
{
	m_Logger->Log(StringBuilder() << "About to read values from " << sFile);
	m_IniFile->SetFilename(sFile.c_str());
	m_IniFile->Load(sFile.c_str());
	for (int i = 1; i <  m_uChannelCount + 1 ; i++)
    {
        struct_LVT_CHANNEL_PARAMS params;
		params.m_Mode = LED_PULSE;		
        params.m_fCurrent = m_IniFile->GetSetValue(
            "Current",
            StringBuilder() << "Channel_#" << i << "_Current",
            1,
            StringBuilder() << "Current value for Channel_#" << i << " ,float"
            );

		
        params.m_uContinousBrightness = m_IniFile->GetSetValue(
            "ContinuousBrighntess",
            StringBuilder() << "Channel_#" << i << "_ContinuousBrighntess",
            1,
            StringBuilder() << "Continuous brighntess value for Channel_#" << i << " ,int"
            );

        params.m_uPulseBrightness = m_IniFile->GetSetValue(
            "PulsedBrighntess",
            StringBuilder() << "Channel_#" << i << "_CPulsedBrighntess",
            1,
            StringBuilder() << "Pulsed brighntess value for Channel_#" << i << " ,int"
            );

		int iDefaultTrigger = 2;
		if (i > 4)
			iDefaultTrigger = 6;
		
			params.m_uTrigger = m_IniFile->GetSetValue(
				"Trigger",
				StringBuilder() << "Channel_#" << i << "_Trigger",
				iDefaultTrigger,
				StringBuilder() << "Trigger for Channel_#" << i << " ,int"
				);

	
		params.m_dPulseWidth = m_IniFile->GetSetValue(
            "Common",
            "PulseWidth",
            2,
            "Pulsed width, float. All channels"
            );
		
		// pakotus...
		//if (params.m_dPulseWidth > 0.00001)
		//	params.m_dPulseWidth = 0.00001;

		params.m_dPulseDelay=  m_IniFile->GetSetValue(
            "Common",
            "PulseDelay",
            2,
            "Pulsed delay, float. All channels"
            );
		params.m_dRetriggerDelay =  m_IniFile->GetSetValue(
            "Common",
            "RetriggerDelay",
            10,
            "Retrigger delay, float. All channels"
            );

		
		/*
    m_uPulseReTrigDel = m_IniFile->GetSetValue(
            "Common",
            "PulseReTriggerDelay",
            10,
            "Pulsed delay, int. All channels"
            );
		*/
        m_Channels[i] = params;
		
		initChannel(i,params);
		
        // tata jarjestysta ei tule muuttaa
		/*
		setCurrent(i,params.m_fCurrent);
		setPulsedBrigthnessAbsolute(i,params.m_uPulseBrightness);
		setMode(i,params.m_Mode);		
		setTrigger(i,params.m_uTrigger);
		*/
		

    }

	if (m_IniFile->IsDirty())
	{
		m_Logger->Log(StringBuilder() << "Saved " << m_IniFile->getCurrentFileName());
		m_IniFile->Save();
	}
   /*
	char szMsg[32];
	strcpy(szMsg,"AW>");
	if (0 == SendAndGetReply(szMsg).length())
			return false;
*/
	return true;
}

bool LedController::initChannel(CHANNEL channel, struct_LVT_CHANNEL_PARAMS params)
{
	char szMsg[32];
	setTrigger(channel,params.m_uTrigger);
	sprintf(szMsg,"VL%d,0,%.2f>",channel, params.m_fCurrent);
    this->m_Logger->Log(StringBuilder() << "About to write current o f ch" << channel << " : " << params.m_fCurrent);
    
    
    if (0 == SendAndGetReply(szMsg).length())
		return false;
     

	double dPulseWidth = (double)params.m_dPulseWidth/1000;
	if (dPulseWidth > 0.01)
		dPulseWidth = 0.002;

    sprintf(szMsg,"RT%d,0.002,%.3f,%d,%.3f>",
    //sprintf(szMsg,"RT%d,%.3f,%.3f,%d,%.3f>",
					channel,
        //			dPulseWidth,
					params.m_dPulseDelay/1000,
					params.m_uPulseBrightness,
					params.m_dRetriggerDelay
					);
	if (0 == SendAndGetReply(szMsg).length())
		return false;
	return true;
}


bool LedController::setCurrentValueSetToDevice()
{
	return true;
}

void LedController::closeConnection()
{
	m_SocketImpl->m_Socket->close();
    m_SocketImpl->m_Socket.reset();
}

void LedController::test(std::string sRep)
{
    using namespace std;
    cout << "Test " << sRep << endl;
    unsigned uDelim = sRep.find_first_of(">");
    sRep = sRep.substr(uDelim+1,sRep.length());
     boost::char_separator<char> sep(",");
     boost::tokenizer<boost::char_separator<char>> tokens(sRep, sep);
     BOOST_FOREACH(string t, tokens)
     {

         bool bOk = false;
         char szKey[16];
         memset(szKey,0,16);
         double dValue = 0;
         char szUnit[16];
         memset(szUnit,0,16);

         std::pair<int, int> poss = findFirstDigitInString(t);
         std::pair<int, int> cmdPoss = findCmdInString(t);


         if (-1 != cmdPoss.first && -1 != cmdPoss.second)
         {
             if ("CH" == t.substr(cmdPoss.first,cmdPoss.second))
             {

                cout << "Channel "<<  atoi(t.substr(poss.first,poss.second).c_str()) << endl;

             }


             if ("S" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"S");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"%");
             }

             if ("SE" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"SE");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"%");

             }

             if ("RA" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"RA");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"A");
             }
             if ("RT" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"RT");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"us");

             }
             if ("CS" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"CS");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"A");

             }
             if ("DL" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                  strcpy(szKey,"DL");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"us");
                  if (t.substr(t.length() -2 , 2) == "ms")
                 {
                     dValue = dValue*1000;
                 }


             }

             if ("PU" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"PU");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"us");
                 if (t.substr(t.length() -2 , 2) == "ms")
                 {
                     cout << "Puls width in ms " << endl;
                     dValue = dValue*1000;
                 }

             }

             if ("IP" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"IP");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"Trigger");
             }
             if ("MD" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"MD");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"MODE");

             }
             if ("FL" == t.substr(cmdPoss.first,cmdPoss.second))
             {
                 strcpy(szKey,"FL");
                 sscanf((const char*)t.substr(poss.first,poss.second).c_str(),"%lf",&dValue);
                 strcpy(szUnit,"FLAGS");
             }


             //printf("Key: %s, Value: %.2f, Unit: %s",szKey,fValue,szUnit);
             cout << "Key: " << szKey << ", Value: " << dValue << ", Unit: " << szUnit << endl;;
         }
     }

}


