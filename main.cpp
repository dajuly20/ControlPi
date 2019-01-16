/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: julian
 *
 * Created on 13. Oktober 2018, 21:49
 */

#include <iostream> // cout
#include <string>   // std::string
#include <signal.h> // signal
#include <fstream>  // ifstream 

#include <regex>    // regex_replace
#include <stdexcept> // throw

#include <thread>
#include <mutex>
#include <condition_variable>


#include "pifacedigitalcpp.h"
#include "boolLogicParser.h"    
#include "regReplaceExtension.h" // Extended reg-replace for inserting callback function.

#include "src/ChannelEntitys/Channel_Entity.h"
#include "src/ChannelEntitys/Channel_Entities_PiFace.h"
#include "src/IOChannels/IO_Channel.h"
#include "src/IOChannels/IO_Channel_Hw_PiFace.h"
#include "src/IOChannels/IO_Channel_Virtual_Memory.h"
#include "IO_Channel_AccesWrapper.h"
#include "src/IOChannels/IO_Channel_Virtual_Timer.h"

using namespace std;

// Globals 
static volatile int  keepRunning = 1;     //Used to interrupt mainloop
static volatile bool configRead  = false; //Used to re-read configuration

typedef shared_ptr<IO_Channel_AccesWrapper> IO_Channel_AccessWrapperPTR;

condition_variable itCond;
mutex              itCondMutex;
bool               itCondSwitch;

/*
 Functor to inject the IO Object into the replace identifier function.
 */
class replaceIdentifier{
        IO_Channel_AccesWrapper& chnl;
    
    public:
        replaceIdentifier(IO_Channel_AccesWrapper& _chnl) : chnl(_chnl){
        }

        // TODO Change to return char? 
        std::string operator()(const std::smatch& match) {
            bool dbg = false;
            if(dbg) cout << "call on: " << match[1].str() << endl;
            string dependantStr = match[1].str();

            // Splits String in characters.
            char frstChar = dependantStr.at(0);
            char scndChar = dependantStr.at(1);
            char thrdChar = dependantStr.at(2);

            // explicit cast 3th character to integer 
            // (regular expression checks for numberic)
            int  dependantNum    = thrdChar - '0';
            bool dependantState  = chnl[frstChar][scndChar]->read_pin(dependantNum);

            if(dbg) cout << "FrstChar: " << frstChar << endl << "ScndChar: " << scndChar << endl << " Number:" << thrdChar << endl << endl;

            return std::to_string(dependantState);
        }  
};

/*
 * Signal Handler
 * Sets the condition of the main loop to zero, 
 * to end the program.
 * 
 */
void intHandler(int dummy) {
    keepRunning = 0;
}

/* Signal Handler
 * Handles SIGUSR1 Signal to trigger re-read of config.
 */
void usrSigHandler(int dummy) {
    printf("\n Reveived reload signal.\n ");
    configRead = false;
}

/*
 * evaulateLogicString
 * Evaluates a given logic string e.g. !0 &1 | 0 ... ; to a boolean result. 
 */
bool evaluateLogicString(string input){
    bool dbg = false;
    //string format like:  a0 = "!0 & 1;";
            
    typedef std::string::const_iterator It;
    It f(input.begin()), l(input.end());
    parser<It> p;

    try{
        expr result;
        bool ok = qi::phrase_parse(f,l,p > ';',qi::space,result);

        if (!ok)
            std::cerr << "Error: Given logic string invalid >" << input << "<" << endl;
        else
        {
            if(dbg) std::cout << "parseLogic: Input " << input << " resulting into" << result << " evaluated to " << evaluate(result) << endl;
            return evaluate(result);

        }

    } 
    catch (const qi::expectation_failure<It>& e){
        std::cerr << "parseLogic: expectation_failure at '" << std::string(e.first, e.last) << "'\n";
    }  
}

/*
 * Prints a given soft-logic. P
 * Pretty selfexplandantory
 */
void printSoftLogic(std::vector<std::string>& softLogic){
    
    cout << endl << endl << "Uising following config:" << endl << "---------------------------------------------" << endl;
    
    for (std::string outStr: softLogic) { 
        cout << outStr << endl;
    } 
    
    cout << endl <<  "---------------------------------------------" << endl << endl;
    
    
}

/*
 * parseIdentifiers
 * Pases a vector of strings, and replaces square brakets to their 
 * 
 * One row may come as follows:
 * One OutputString might be Ora0=![Ira1] & [Ira0] | [Ira2];
 *                           ^^^^   ^^^^
 *                             |     |  
 *                           Output  Input 
 * 
 * In above example the logic string for O-utput r-eal a(..z) 0(..9) will be parsed.
 * For that anything in square brackets will be replaced by a boolean value representing
 * the state of the related "pin" or the related signal.
 * 
 * So given a value of Ira1=0; Ira0=1 and Ira2 = 0 the above example would end up as follows: 
 * Ora0=!0 & 1 | 0 
 *
 */
void parseIdentifiers(IO_Channel_AccesWrapper& chnl, std::vector<std::string>& softLogic){
      // Cut the string in halves at the "=" sign
    bool        dbg         =  true;
    std::string delimiter   = "=";
    size_t      found;

    // Output softlogic for debugging purposes.
    if(dbg) printSoftLogic(softLogic);
    
    for (std::string softLogicRow: softLogic) { 

        // Splits the output string by the '=' sign in two parts:
        // 1. the Output to use (e.g. Ora0) 2nd the rawLogicString including used inputs in brackets e.g. ![Ira1] & [Ira0] | [Ira2];
        if ((found = softLogicRow.find('=')) != string::npos){ // Changed "" to '' 27.12.2018. Remove comment when it works ;) 
            // The part before the '=' is the variable, to which whe outcome will be asigned to ==> 'asigned' part7
            // TODO Check for length of String
            string asignedEntityStr         = softLogicRow.substr(0,found);
            if(asignedEntityStr.size() != 3){
                throw std::invalid_argument("Error: Asignee needs to have 3 letters/digits in '"+asignedEntityStr+"'");
            }
            char   asigned_IOChannel        = asignedEntityStr.at(0);
            char   asigned_ChannelEntity    = asignedEntityStr.at(1);
            int    asigned_Pin              = asignedEntityStr.at(2) - '0'; 

           // The part after the '=' is the equation string. It consists of identifiers 
           //   (like [Ho0]) arithmetic operators ( &, | ) and round brakets. Also allowed  are literals (0 or 1)
           string equationString  = softLogicRow.substr(found+1, string::npos);

           if(dbg) cout << "HardwareOutput is: " << asigned_Pin << endl;
           if(dbg) cout << "RawLogicString is: " << equationString << endl;

           // Instantiate the replace-identifier Functor/Class that is used to
           //   inject the IO_Channel_AccessWrapper into the actual replace-function.
           replaceIdentifier rplacIds(chnl);
           
           // Calls a function for each match on "[ira0]" a string of 4 characters in '[' brackets,
           // 1st char of which may be i/o (in/out), 2nd r/v (real/virtual), 3rd hardware idendifier (a-z), 4re input identifier (0-8)
           // eventually every occurance of brackets should be replaced either by a 0 or 1. 
           // For the example state of (Ira1 = 0, Ira0 =1, Ira2 = 1), the example logic string would look like !0 & 1 | 1;
           string outLogicString  = regex_replace(  equationString, 
                                                    regex("\\[([A-Z][a-z][0-8])\\]"),
                                                    rplacIds
                                                  );
           
           if(dbg) cout << "Resulting logic string is: " << outLogicString << endl;

           // Eventually the example logic string (e.g. !0 & 1 | 1;) will be parsed to 1
           bool parsedOut = evaluateLogicString(outLogicString);

           if(dbg) cout << "IO_chnl: " << asigned_IOChannel;
           if(dbg) cout << " Entity: " << asigned_ChannelEntity;
           if(dbg) cout << " Pin: " << asigned_Pin;
           if(dbg) cout << " is assigned " << (parsedOut ? "true" : "false") << endl << endl;
           
           chnl[asigned_IOChannel][asigned_ChannelEntity]->write_pin(parsedOut, asigned_Pin);
           
        }
        else{
            // Todo: Error, = sign not in string.
        }
    }
}

/*
 * stripWhite
 * Stripes any whitespace from given string.
 * (used for reading configuration)
 */     
std::string strip_white(const std::string& input)
{
   size_t b = input.find_first_not_of(' ');
   if (b == std::string::npos) b = 0;
   return input.substr(b, input.find_last_not_of(' ') + 1 - b);
}
 
/*
 * strip_comments
 * Strips anything starting from the first position of given delimiter.
 * e.g. "#" for Comments.
 */
std::string strip_comments(const std::string& input, const std::string& delimiters)
{
   return strip_white(input.substr(0, input.find_first_of(delimiters)));
}

/*
 * loadSoftLogic
 * Loads logic from given filename, and returns it as vector of strings.
 */
std::vector<std::string>  loadSoftLogic(std::string filename){
    
    std::string                 line;
    std::vector<std::string>    logic;
    std::ifstream               data(filename);
    
    if(!data.is_open()){
       throw std::invalid_argument("Error: File not exist: "+filename+" ");
    }    

    while (std::getline(data, line)){
        std::string delimiters("#;");
        line = strip_comments(line, delimiters);
        if(line.size() > 0){
            // TODO Check Syntax
            logic.push_back(line+';');
        }
    }

    if(logic.size() == 0){
        throw std::invalid_argument("Error: Config file empty!");
    }
   
    return logic;
}
 

int main( int argc, char *argv[] )
{
    uint8_t i = 0;          /**< Loop iterator */
    uint8_t inputs;         /**< Input bits (pins 0-7) */
    uint8_t outputs;         /**< Input bits (pins 0-7) */
    
    // Register signalHandlers
    signal(SIGINT,  intHandler);    // Stops program gracefully
    signal(SIGKILL, intHandler);    // " "
    signal(SIGHUP,  intHandler);    // " "
    signal(SIGTERM, intHandler);    // " "
    signal(SIGUSR1 ,usrSigHandler); // Re-Reads config, doesn't exit.

    // Push all the possible channels to the array. 
    // Could Access now via (*myte.io_channels['H'])['i']->read_pin(0) 
    // But IO_Channel_AccessWrapper hides it away, and simplyfies access. so obj['H']['i']->member
    IO_Channel_AccesWrapper chnl;
    chnl.io_channels.insert(std::make_pair('H', new IO_Channel_Hw_PiFace()));
    chnl.io_channels.insert(std::make_pair('M', new IO_Channel_Virtual_Memory()));
    chnl.io_channels.insert(std::make_pair('T', new IO_Channel_Virtual_Timer()));
    // Usage e.g.:  chnl['H']['i'][0];  ==> (*io_channels['H'])['i']->read_pin(0);
  
    // Mandatory for interrupt funktion.
    inputs = chnl['H']['o']->read_all();
    printf("Outputs: 0x%x\n", inputs);

    // Initially read the config    
    std::string filename = "logic.conf";
    std::vector<std::string>  softLogic;
    softLogic  = loadSoftLogic(filename);
    configRead = true;
    
    // Initially parse identifiers once.
    // (Necessary to let starting values take efect.
    // e.g. if Output 1 is assigned static true)
    parseIdentifiers(chnl, softLogic);
   
    // Enable Caching
    // TODO: Make infrastructure to enable caching on all channels at once
    chnl['H'].getIOChnl()->caching_enable();  

    // Main loop. 
    while(keepRunning ){
        if (chnl['H'].getIOChnl()->interrupts_enabled()) {
            printf("\n\nWaiting for input (press any button on the PiFaceDigital)\n");
            /*
             * Something like:

            condition_variable itCond;
            mutex              itCondMutex;
            bool               itCondSwitch;

             */
            // std::unique_lock<mutex> lock{itCondMutex};
            //itCond.wait(lock, [] { return itCondSwitch;});  // Waiting for change of itCondSwitch (that is set by another thread..) 
            //maybe even
            // itCond.wait(lock);
            if (chnl['H'].getIOChnl()->wait_for_interrupt()){

                if(configRead == false){
                    softLogic = loadSoftLogic(filename);
                    configRead = true;
                    printf("\n\nConfig has been reloaded!\n");
                }

                chnl['H'].getIOChnl()->flush();

                parseIdentifiers(chnl, softLogic);

                chnl['H'].getIOChnl()->flush();
          
                for (i = 0; i < 8; i++) {
                    uint8_t pinStateRev =  chnl['H']['i']->read_pin(i); 
                    printf("Input %d value: %d\n", (int)i, (int)pinStateRev);
                }
            }
            else{
                printf("Can't wait for input. Something went wrong!\n");
            }
        }
        else{
            printf("Interrupts disabled. Aborting.\n");
            keepRunning = 0;
        }
    }  
}
